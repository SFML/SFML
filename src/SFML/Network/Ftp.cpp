////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007-2009 Laurent Gomila (laurent.gom@gmail.com)
//
// This software is provided 'as-is', without any express or implied warranty.
// In no event will the authors be held liable for any damages arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it freely,
// subject to the following restrictions:
//
// 1. The origin of this software must not be misrepresented;
//    you must not claim that you wrote the original software.
//    If you use this software in a product, an acknowledgment
//    in the product documentation would be appreciated but is not required.
//
// 2. Altered source versions must be plainly marked as such,
//    and must not be misrepresented as being the original software.
//
// 3. This notice may not be removed or altered from any source distribution.
//
////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Network/Ftp.hpp>
#include <SFML/Network/IPAddress.hpp>
#include <algorithm>
#include <fstream>
#include <iterator>
#include <sstream>


namespace sf
{
////////////////////////////////////////////////////////////
// Utility class for exchanging stuff with the server
// on the data channel
////////////////////////////////////////////////////////////
class Ftp::DataChannel : NonCopyable
{
public :

    ////////////////////////////////////////////////////////////
    // Constructor
    ////////////////////////////////////////////////////////////
    DataChannel(Ftp& owner);

    ////////////////////////////////////////////////////////////
    // Destructor
    ////////////////////////////////////////////////////////////
    ~DataChannel();

    ////////////////////////////////////////////////////////////
    // Open the data channel using the specified mode and port
    ////////////////////////////////////////////////////////////
    Ftp::Response Open(Ftp::TransferMode mode);

    ////////////////////////////////////////////////////////////
    // Send data on the data channel
    ////////////////////////////////////////////////////////////
    void Send(const std::vector<char>& data);

    ////////////////////////////////////////////////////////////
    // Receive data on the data channel until it is closed
    ////////////////////////////////////////////////////////////
    void Receive(std::vector<char>& data);

private :

    ////////////////////////////////////////////////////////////
    // Member data
    ////////////////////////////////////////////////////////////
    Ftp&      myFtp;        ///< Reference to the owner Ftp instance
    SocketTCP myDataSocket; ///< Socket used for data transfers
};


////////////////////////////////////////////////////////////
/// Default constructor
////////////////////////////////////////////////////////////
Ftp::Response::Response(Status code, const std::string& message) :
myStatus (code),
myMessage(message)
{

}


////////////////////////////////////////////////////////////
/// Convenience function to check if the response status code
/// means a success
////////////////////////////////////////////////////////////
bool Ftp::Response::IsOk() const
{
    return myStatus < 400;
}


////////////////////////////////////////////////////////////
/// Get the response status code
////////////////////////////////////////////////////////////
Ftp::Response::Status Ftp::Response::GetStatus() const
{
    return myStatus;
}


////////////////////////////////////////////////////////////
/// Get the full message contained in the response
////////////////////////////////////////////////////////////
const std::string& Ftp::Response::GetMessage() const
{
    return myMessage;
}


////////////////////////////////////////////////////////////
/// Default constructor
////////////////////////////////////////////////////////////
Ftp::DirectoryResponse::DirectoryResponse(Ftp::Response response) :
Ftp::Response(response)
{
    if (IsOk())
    {
        // Extract the directory from the server response
        std::string::size_type begin = response.GetMessage().find('"', 0);
        std::string::size_type end   = response.GetMessage().find('"', begin + 1);
        myDirectory = response.GetMessage().substr(begin + 1, end - begin - 1);
    }
}


////////////////////////////////////////////////////////////
/// Get the directory returned in the response
////////////////////////////////////////////////////////////
const std::string& Ftp::DirectoryResponse::GetDirectory() const
{
    return myDirectory;
}


////////////////////////////////////////////////////////////
/// Default constructor
////////////////////////////////////////////////////////////
Ftp::ListingResponse::ListingResponse(Ftp::Response response, const std::vector<char>& data) :
Ftp::Response(response)
{
    if (IsOk())
    {
        // Fill the array of strings
        std::string paths(data.begin(), data.end());
        std::string::size_type lastPos = 0;
        for (std::string::size_type pos = paths.find("\r\n"); pos != std::string::npos; pos = paths.find("\r\n", lastPos))
        {
            myFilenames.push_back(paths.substr(lastPos, pos - lastPos));
            lastPos = pos + 2;
        }
    }
}


////////////////////////////////////////////////////////////
/// Get the number of filenames in the listing
////////////////////////////////////////////////////////////
std::size_t Ftp::ListingResponse::GetCount() const
{
    return myFilenames.size();
}


////////////////////////////////////////////////////////////
/// Get the Index-th filename in the directory
////////////////////////////////////////////////////////////
const std::string& Ftp::ListingResponse::GetFilename(std::size_t index) const
{
    return myFilenames[index];
}


////////////////////////////////////////////////////////////
/// Destructor -- close the connection with the server
////////////////////////////////////////////////////////////
Ftp::~Ftp()
{
    Disconnect();
}


////////////////////////////////////////////////////////////
/// Connect to the specified FTP server
////////////////////////////////////////////////////////////
Ftp::Response Ftp::Connect(const IPAddress& server, unsigned short port, float timeout)
{
    // Connect to the server
    if (myCommandSocket.Connect(port, server, timeout) != Socket::Done)
        return Response(Response::ConnectionFailed);

    // Get the response to the connection
    return GetResponse();
}


////////////////////////////////////////////////////////////
/// Log in using anonymous account
////////////////////////////////////////////////////////////
Ftp::Response Ftp::Login()
{
    return Login("anonymous", "user@sfml-dev.org");
}


////////////////////////////////////////////////////////////
/// Log in using a username and a password
////////////////////////////////////////////////////////////
Ftp::Response Ftp::Login(const std::string& name, const std::string& password)
{
    Response response = SendCommand("USER", name);
    if (response.IsOk())
        response = SendCommand("PASS", password);

    return response;
}


////////////////////////////////////////////////////////////
/// Close the connection with FTP server
////////////////////////////////////////////////////////////
Ftp::Response Ftp::Disconnect()
{
    // Send the exit command
    Response response = SendCommand("QUIT");
    if (response.IsOk())
        myCommandSocket.Close();

    return response;
}


////////////////////////////////////////////////////////////
/// Send a null command just to prevent from being disconnected
////////////////////////////////////////////////////////////
Ftp::Response Ftp::KeepAlive()
{
    return SendCommand("NOOP");
}


////////////////////////////////////////////////////////////
/// Get the current working directory
////////////////////////////////////////////////////////////
Ftp::DirectoryResponse Ftp::GetWorkingDirectory()
{
    return DirectoryResponse(SendCommand("PWD"));
}


////////////////////////////////////////////////////////////
/// Get the contents of the given directory
/// (subdirectories and files)
////////////////////////////////////////////////////////////
Ftp::ListingResponse Ftp::GetDirectoryListing(const std::string& directory)
{
    // Open a data channel on default port (20) using ASCII transfer mode
    std::vector<char> directoryData;
    DataChannel data(*this);
    Response response = data.Open(Ascii);
    if (response.IsOk())
    {
        // Tell the server to send us the listing
        response = SendCommand("NLST", directory);
        if (response.IsOk())
        {
            // Receive the listing
            data.Receive(directoryData);

            // Get the response from the server
            response = GetResponse();
        }
    }

    return ListingResponse(response, directoryData);
}


////////////////////////////////////////////////////////////
/// Change the current working directory
////////////////////////////////////////////////////////////
Ftp::Response Ftp::ChangeDirectory(const std::string& directory)
{
    return SendCommand("CWD", directory);
}


////////////////////////////////////////////////////////////
/// Go to the parent directory of the current one
////////////////////////////////////////////////////////////
Ftp::Response Ftp::ParentDirectory()
{
    return SendCommand("CDUP");
}


////////////////////////////////////////////////////////////
/// Create a new directory
////////////////////////////////////////////////////////////
Ftp::Response Ftp::MakeDirectory(const std::string& name)
{
    return SendCommand("MKD", name);
}


////////////////////////////////////////////////////////////
/// Remove an existing directory
////////////////////////////////////////////////////////////
Ftp::Response Ftp::DeleteDirectory(const std::string& name)
{
    return SendCommand("RMD", name);
}


////////////////////////////////////////////////////////////
/// Rename a file
////////////////////////////////////////////////////////////
Ftp::Response Ftp::RenameFile(const std::string& file, const std::string& newName)
{
    Response response = SendCommand("RNFR", file);
    if (response.IsOk())
       response = SendCommand("RNTO", newName);

    return response;
}


////////////////////////////////////////////////////////////
/// Remove an existing file
////////////////////////////////////////////////////////////
Ftp::Response Ftp::DeleteFile(const std::string& name)
{
    return SendCommand("DELE", name);
}


////////////////////////////////////////////////////////////
/// Download a file from the server
////////////////////////////////////////////////////////////
Ftp::Response Ftp::Download(const std::string& distantFile, const std::string& destPath, TransferMode mode)
{
    // Open a data channel using the given transfer mode
    DataChannel data(*this);
    Response response = data.Open(mode);
    if (response.IsOk())
    {
        // Tell the server to start the transfer
        response = SendCommand("RETR", distantFile);
        if (response.IsOk())
        {
            // Receive the file data
            std::vector<char> fileData;
            data.Receive(fileData);

            // Get the response from the server
            response = GetResponse();
            if (response.IsOk())
            {
                // Extract the filename from the file path
                std::string filename = distantFile;
                std::string::size_type pos = filename.find_last_of("/\\");
                if (pos != std::string::npos)
                    filename = filename.substr(pos + 1);

                // Make sure the destination path ends with a slash
                std::string path = destPath;
                if (!path.empty() && (path[path.size() - 1] != '\\') && (path[path.size() - 1] != '/'))
                    path += "/";

                // Create the file and copy the received data into it
                std::ofstream file((path + filename).c_str(), std::ios_base::binary);
                if (!file)
                    return Response(Response::InvalidFile);

                if (!fileData.empty())
                    file.write(&fileData[0], static_cast<std::streamsize>(fileData.size()));
            }
        }
    }

    return response;
}


////////////////////////////////////////////////////////////
/// Upload a file to the server
////////////////////////////////////////////////////////////
Ftp::Response Ftp::Upload(const std::string& localFile, const std::string& destPath, TransferMode mode)
{
    // Get the contents of the file to send
    std::ifstream file(localFile.c_str(), std::ios_base::binary);
    if (!file)
        return Response(Response::InvalidFile);

    file.seekg(0, std::ios::end);
    std::size_t length = file.tellg();
    file.seekg(0, std::ios::beg);
    std::vector<char> fileData(length);
    if (length > 0)
        file.read(&fileData[0], static_cast<std::streamsize>(length));

    // Extract the filename from the file path
    std::string filename = localFile;
    std::string::size_type pos = filename.find_last_of("/\\");
    if (pos != std::string::npos)
        filename = filename.substr(pos + 1);

    // Make sure the destination path ends with a slash
    std::string path = destPath;
    if (!path.empty() && (path[path.size() - 1] != '\\') && (path[path.size() - 1] != '/'))
        path += "/";

    // Open a data channel using the given transfer mode
    DataChannel data(*this);
    Response response = data.Open(mode);
    if (response.IsOk())
    {
        // Tell the server to start the transfer
        response = SendCommand("STOR", path + filename);
        if (response.IsOk())
        {
            // Send the file data
            data.Send(fileData);

            // Get the response from the server
            response = GetResponse();
        }
    }

    return response;
}


////////////////////////////////////////////////////////////
/// Send a command to the FTP server
////////////////////////////////////////////////////////////
Ftp::Response Ftp::SendCommand(const std::string& command, const std::string& parameter)
{
    // Build the command string
    std::string commandStr;
    if (parameter != "")
        commandStr = command + " " + parameter + "\r\n";
    else
        commandStr = command + "\r\n";

    // Send it to the server
    if (myCommandSocket.Send(commandStr.c_str(), commandStr.length()) != Socket::Done)
        return Response(Response::ConnectionClosed);

    // Get the response
    return GetResponse();
}


////////////////////////////////////////////////////////////
/// Receive a response from the server
/// (usually after a command has been sent)
////////////////////////////////////////////////////////////
Ftp::Response Ftp::GetResponse()
{
    // We'll use a variable to keep track of the last valid code.
    // It is useful in case of multi-lines responses, because the end of such a response
    // will start by the same code
    unsigned int lastCode  = 0;
    bool isInsideMultiline = false;
    std::string message;

    for (;;)
    {
        // Receive the response from the server
        char buffer[1024];
        std::size_t length;
        if (myCommandSocket.Receive(buffer, sizeof(buffer), length) != Socket::Done)
            return Response(Response::ConnectionClosed);

        // There can be several lines inside the received buffer, extract them all
        std::istringstream in(std::string(buffer, length), std::ios_base::binary);
        while (in)
        {
            // Try to extract the code
            unsigned int code;
            if (in >> code)
            {
                // Extract the separator
                char separator;
                in.get(separator);

                // The '-' character means a multiline response
                if ((separator == '-') && !isInsideMultiline)
                {
                    // Set the multiline flag
                    isInsideMultiline = true;

                    // Keep track of the code
                    if (lastCode == 0)
                        lastCode = code;

                    // Extract the line
                    std::getline(in, message);

                    // Remove the ending '\r' (all lines are terminated by "\r\n")
                    message.erase(message.length() - 1);
                    message = separator + message + "\n";
                }
                else
                {
                    // We must make sure that the code is the same, otherwise it means
                    // we haven't reached the end of the multiline response
                    if ((separator != '-') && ((code == lastCode) || (lastCode == 0)))
                    {
                        // Clear the multiline flag
                        isInsideMultiline = false;

                        // Extract the line
                        std::string line;
                        std::getline(in, line);

                        // Remove the ending '\r' (all lines are terminated by "\r\n")
                        line.erase(line.length() - 1);

                        // Append it to the message
                        if (code == lastCode)
                        {
                            std::ostringstream out;
                            out << code << separator << line;
                            message += out.str();
                        }
                        else
                        {
                            message = separator + line;
                        }

                        // Return the response code and message
                        return Response(static_cast<Response::Status>(code), message);
                    }
                    else
                    {
                        // The line we just read was actually not a response,
                        // only a new part of the current multiline response

                        // Extract the line
                        std::string line;
                        std::getline(in, line);

                        if (!line.empty())
                        {
                            // Remove the ending '\r' (all lines are terminated by "\r\n")
                            line.erase(line.length() - 1);

                            // Append it to the current message
                            std::ostringstream out;
                            out << code << separator << line << "\n";
                            message += out.str();
                        }
                    }
                }
            }
            else if (lastCode != 0)
            {
                // It seems we are in the middle of a multiline response

                // Clear the error bits of the stream
                in.clear();

                // Extract the line
                std::string line;
                std::getline(in, line);

                if (!line.empty())
                {
                    // Remove the ending '\r' (all lines are terminated by "\r\n")
                    line.erase(line.length() - 1);

                    // Append it to the current message
                    message += line + "\n";
                }
            }
            else
            {
                // Error : cannot extract the code, and we are not in a multiline response
                return Response(Response::InvalidResponse);
            }
        }
    }

    // We never reach there
}


////////////////////////////////////////////////////////////
/// Constructor
////////////////////////////////////////////////////////////
Ftp::DataChannel::DataChannel(Ftp& owner) :
myFtp(owner)
{

}


////////////////////////////////////////////////////////////
/// Destructor
////////////////////////////////////////////////////////////
Ftp::DataChannel::~DataChannel()
{
    // Close the data socket
    myDataSocket.Close();
}


////////////////////////////////////////////////////////////
/// Open the data channel using the specified mode and port
////////////////////////////////////////////////////////////
Ftp::Response Ftp::DataChannel::Open(Ftp::TransferMode mode)
{
    // Open a data connection in active mode (we connect to the server)
    Ftp::Response response = myFtp.SendCommand("PASV");
    if (response.IsOk())
    {
        // Extract the connection address and port from the response
        std::string::size_type begin = response.GetMessage().find_first_of("0123456789");
        if (begin != std::string::npos)
        {
            Uint8 data[6] = {0, 0, 0, 0, 0, 0};
            std::string str = response.GetMessage().substr(begin);
            std::size_t index = 0;
            for (int i = 0; i < 6; ++i)
            {
                // Extract the current number
                while (isdigit(str[index]))
                {
                    data[i] = data[i] * 10 + (str[index] - '0');
                    index++;
                }

                // Skip separator
                index++;
            }

            // Reconstruct connection port and address
            unsigned short port = data[4] * 256 + data[5];
            IPAddress address(static_cast<Uint8>(data[0]),
                              static_cast<Uint8>(data[1]),
                              static_cast<Uint8>(data[2]),
                              static_cast<Uint8>(data[3]));

            // Connect the data channel to the server
            if (myDataSocket.Connect(port, address) == Socket::Done)
            {
                // Translate the transfer mode to the corresponding FTP parameter
                std::string modeStr;
                switch (mode)
                {
                    case Ftp::Binary : modeStr = "I"; break;
                    case Ftp::Ascii :  modeStr = "A"; break;
                    case Ftp::Ebcdic : modeStr = "E"; break;
                }

                // Set the transfer mode
                response = myFtp.SendCommand("TYPE", modeStr);
            }
            else
            {
                // Failed to connect to the server
                response = Ftp::Response(Ftp::Response::ConnectionFailed);
            }
        }
    }

    return response;
}


////////////////////////////////////////////////////////////
/// Receive data on the data channel until it is closed
////////////////////////////////////////////////////////////
void Ftp::DataChannel::Receive(std::vector<char>& data)
{
    // Receive data
    data.clear();
    char buffer[1024];
    std::size_t received;
    while (myDataSocket.Receive(buffer, sizeof(buffer), received) == Socket::Done)
    {
        std::copy(buffer, buffer + received, std::back_inserter(data));
    }

    // Close the data socket
    myDataSocket.Close();
}


////////////////////////////////////////////////////////////
/// Send data on the data channel
////////////////////////////////////////////////////////////
void Ftp::DataChannel::Send(const std::vector<char>& data)
{
    // Send data
    if (!data.empty())
        myDataSocket.Send(&data[0], data.size());

    // Close the data socket
    myDataSocket.Close();
}

} // namespace sf
