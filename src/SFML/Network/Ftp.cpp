////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007-2025 Laurent Gomila (laurent@sfml-dev.org)
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
#include <SFML/Network/IpAddress.hpp>

#include <SFML/System/Err.hpp>

#include <algorithm>
#include <array>
#include <fstream>
#include <ostream>
#include <sstream>
#include <utility>

#include <cctype>
#include <cstddef>
#include <cstdint>


namespace sf
{
////////////////////////////////////////////////////////////
class Ftp::DataChannel
{
public:
    ////////////////////////////////////////////////////////////
    explicit DataChannel(Ftp& owner);

    ////////////////////////////////////////////////////////////
    /// \brief Deleted copy constructor
    ///
    ////////////////////////////////////////////////////////////
    DataChannel(const DataChannel&) = delete;

    ////////////////////////////////////////////////////////////
    /// \brief Deleted copy assignment
    ///
    ////////////////////////////////////////////////////////////
    DataChannel& operator=(const DataChannel&) = delete;

    ////////////////////////////////////////////////////////////
    Ftp::Response open(Ftp::TransferMode mode);

    ////////////////////////////////////////////////////////////
    void send(std::istream& stream);

    ////////////////////////////////////////////////////////////
    void receive(std::ostream& stream);

private:
    ////////////////////////////////////////////////////////////
    // Member data
    ////////////////////////////////////////////////////////////
    Ftp&      m_ftp;        //!< Reference to the owner Ftp instance
    TcpSocket m_dataSocket; //!< Socket used for data transfers
};


////////////////////////////////////////////////////////////
Ftp::Response::Response(Status code, std::string message) : m_status(code), m_message(std::move(message))
{
}


////////////////////////////////////////////////////////////
bool Ftp::Response::isOk() const
{
    return static_cast<int>(m_status) < 400;
}


////////////////////////////////////////////////////////////
Ftp::Response::Status Ftp::Response::getStatus() const
{
    return m_status;
}


////////////////////////////////////////////////////////////
const std::string& Ftp::Response::getMessage() const
{
    return m_message;
}


////////////////////////////////////////////////////////////
Ftp::DirectoryResponse::DirectoryResponse(const Ftp::Response& response) : Ftp::Response(response)
{
    if (isOk())
    {
        // Extract the directory from the server response
        const std::string::size_type begin = getMessage().find('"', 0);
        const std::string::size_type end   = getMessage().find('"', begin + 1);
        m_directory                        = getMessage().substr(begin + 1, end - begin - 1);
    }
}


////////////////////////////////////////////////////////////
const std::filesystem::path& Ftp::DirectoryResponse::getDirectory() const
{
    return m_directory;
}


////////////////////////////////////////////////////////////
Ftp::ListingResponse::ListingResponse(const Ftp::Response& response, const std::string& data) : Ftp::Response(response)
{
    if (isOk())
    {
        // Fill the array of strings
        std::string::size_type lastPos = 0;
        for (std::string::size_type pos = data.find("\r\n"); pos != std::string::npos; pos = data.find("\r\n", lastPos))
        {
            m_listing.push_back(data.substr(lastPos, pos - lastPos));
            lastPos = pos + 2;
        }
    }
}


////////////////////////////////////////////////////////////
const std::vector<std::string>& Ftp::ListingResponse::getListing() const
{
    return m_listing;
}


////////////////////////////////////////////////////////////
Ftp::~Ftp()
{
    (void)disconnect();
}


////////////////////////////////////////////////////////////
Ftp::Response Ftp::connect(IpAddress server, unsigned short port, Time timeout)
{
    // Connect to the server
    if (m_commandSocket.connect(server, port, timeout) != Socket::Status::Done)
        return Response(Response::Status::ConnectionFailed);

    // Get the response to the connection
    return getResponse();
}


////////////////////////////////////////////////////////////
Ftp::Response Ftp::login()
{
    return login("anonymous", "user@sfml-dev.org");
}


////////////////////////////////////////////////////////////
Ftp::Response Ftp::login(const std::string& name, const std::string& password)
{
    Response response = sendCommand("USER", name);
    if (response.isOk())
        response = sendCommand("PASS", password);

    return response;
}


////////////////////////////////////////////////////////////
Ftp::Response Ftp::disconnect()
{
    // Send the exit command
    Response response = sendCommand("QUIT");
    if (response.isOk())
        m_commandSocket.disconnect();

    return response;
}


////////////////////////////////////////////////////////////
Ftp::Response Ftp::keepAlive()
{
    return sendCommand("NOOP");
}


////////////////////////////////////////////////////////////
Ftp::DirectoryResponse Ftp::getWorkingDirectory()
{
    return {sendCommand("PWD")};
}


////////////////////////////////////////////////////////////
Ftp::ListingResponse Ftp::getDirectoryListing(const std::string& directory)
{
    // Open a data channel on default port (20) using ASCII transfer mode
    std::ostringstream directoryData;
    DataChannel        data(*this);
    Response           response = data.open(TransferMode::Ascii);
    if (response.isOk())
    {
        // Tell the server to send us the listing
        response = sendCommand("NLST", directory);
        if (response.isOk())
        {
            // Receive the listing
            data.receive(directoryData);

            // Get the response from the server
            response = getResponse();
        }
    }

    return {response, directoryData.str()};
}


////////////////////////////////////////////////////////////
Ftp::Response Ftp::changeDirectory(const std::string& directory)
{
    return sendCommand("CWD", directory);
}


////////////////////////////////////////////////////////////
Ftp::Response Ftp::parentDirectory()
{
    return sendCommand("CDUP");
}


////////////////////////////////////////////////////////////
Ftp::Response Ftp::createDirectory(const std::string& name)
{
    return sendCommand("MKD", name);
}


////////////////////////////////////////////////////////////
Ftp::Response Ftp::deleteDirectory(const std::string& name)
{
    return sendCommand("RMD", name);
}


////////////////////////////////////////////////////////////
Ftp::Response Ftp::renameFile(const std::filesystem::path& file, const std::filesystem::path& newName)
{
    Response response = sendCommand("RNFR", file.string());
    if (response.isOk())
        response = sendCommand("RNTO", newName.string());

    return response;
}


////////////////////////////////////////////////////////////
Ftp::Response Ftp::deleteFile(const std::filesystem::path& name)
{
    return sendCommand("DELE", name.string());
}


////////////////////////////////////////////////////////////
Ftp::Response Ftp::download(const std::filesystem::path& remoteFile, const std::filesystem::path& localPath, TransferMode mode)
{
    // Open a data channel using the given transfer mode
    DataChannel data(*this);
    Response    response = data.open(mode);
    if (response.isOk())
    {
        // Tell the server to start the transfer
        response = sendCommand("RETR", remoteFile.string());
        if (response.isOk())
        {
            // Create the file and truncate it if necessary
            const std::filesystem::path filepath = localPath / remoteFile.filename();
            std::ofstream               file(filepath, std::ios_base::binary | std::ios_base::trunc);
            if (!file)
                return Response(Response::Status::InvalidFile);

            // Receive the file data
            data.receive(file);

            // Close the file
            file.close();

            // Get the response from the server
            response = getResponse();

            // If the download was unsuccessful, delete the partial file
            if (!response.isOk())
                std::filesystem::remove(filepath);
        }
    }

    return response;
}


////////////////////////////////////////////////////////////
Ftp::Response Ftp::upload(const std::filesystem::path& localFile,
                          const std::filesystem::path& remotePath,
                          TransferMode                 mode,
                          bool                         append)
{
    // Get the contents of the file to send
    std::ifstream file(localFile, std::ios_base::binary);
    if (!file)
        return Response(Response::Status::InvalidFile);

    // Open a data channel using the given transfer mode
    DataChannel data(*this);
    Response    response = data.open(mode);
    if (response.isOk())
    {
        // Tell the server to start the transfer
        response = sendCommand(append ? "APPE" : "STOR", (remotePath / localFile.filename()).string());
        if (response.isOk())
        {
            // Send the file data
            data.send(file);

            // Get the response from the server
            response = getResponse();
        }
    }

    return response;
}


////////////////////////////////////////////////////////////
Ftp::Response Ftp::sendCommand(const std::string& command, const std::string& parameter)
{
    // Build the command string
    const std::string commandStr = parameter.empty() ? command + "\r\n" : command + " " + parameter + "\r\n";

    // Send it to the server
    if (m_commandSocket.send(commandStr.c_str(), commandStr.length()) != Socket::Status::Done)
        return Response(Response::Status::ConnectionClosed);

    // Get the response
    return getResponse();
}


////////////////////////////////////////////////////////////
Ftp::Response Ftp::getResponse()
{
    // We'll use a variable to keep track of the last valid code.
    // It is useful in case of multi-lines responses, because the end of such a response
    // will start by the same code
    unsigned int lastCode          = 0;
    bool         isInsideMultiline = false;
    std::string  message;

    for (;;)
    {
        // Receive the response from the server
        std::array<char, 1024> buffer{};
        std::size_t            length = 0;

        if (m_receiveBuffer.empty())
        {
            if (m_commandSocket.receive(buffer.data(), buffer.size(), length) != Socket::Status::Done)
                return Response(Response::Status::ConnectionClosed);
        }
        else
        {
            std::copy(m_receiveBuffer.begin(), m_receiveBuffer.end(), buffer.data());
            length = m_receiveBuffer.size();
            m_receiveBuffer.clear();
        }

        // There can be several lines inside the received buffer, extract them all
        std::istringstream in(std::string(buffer.data(), length), std::ios_base::binary);
        while (in)
        {
            // Try to extract the code
            unsigned int code = 0;
            if (in >> code)
            {
                // Extract the separator
                char separator = 0;
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

                        // Save the remaining data for the next time getResponse() is called
                        m_receiveBuffer.assign(buffer.data() + static_cast<std::size_t>(in.tellg()),
                                               length - static_cast<std::size_t>(in.tellg()));

                        // Return the response code and message
                        return Response(static_cast<Response::Status>(code), message);
                    }

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
                        out << code << separator << line << '\n';
                        message += out.str();
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
                // Error: cannot extract the code, and we are not in a multiline response
                return Response(Response::Status::InvalidResponse);
            }
        }
    }

    // We never reach there
}


////////////////////////////////////////////////////////////
Ftp::DataChannel::DataChannel(Ftp& owner) : m_ftp(owner)
{
}


////////////////////////////////////////////////////////////
Ftp::Response Ftp::DataChannel::open(Ftp::TransferMode mode)
{
    // Open a data connection in active mode (we connect to the server)
    Ftp::Response response = m_ftp.sendCommand("PASV");
    if (response.isOk())
    {
        // Extract the connection address and port from the response
        const std::string::size_type begin = response.getMessage().find_first_of("0123456789");
        if (begin != std::string::npos)
        {
            std::array<std::uint8_t, 6> data{};
            std::string                 str   = response.getMessage().substr(begin);
            std::size_t                 index = 0;
            for (unsigned char& datum : data)
            {
                // Extract the current number
                while (std::isdigit(str[index]))
                {
                    datum = static_cast<std::uint8_t>(
                        static_cast<std::uint8_t>(datum * 10) + static_cast<std::uint8_t>(str[index] - '0'));
                    ++index;
                }

                // Skip separator
                ++index;
            }

            // Reconstruct connection port and address
            const auto      port = static_cast<std::uint16_t>(data[4] * 256 + data[5]);
            const IpAddress address(data[0], data[1], data[2], data[3]);

            // Connect the data channel to the server
            if (m_dataSocket.connect(address, port) == Socket::Status::Done)
            {
                // Translate the transfer mode to the corresponding FTP parameter
                std::string modeStr;
                switch (mode)
                {
                    case Ftp::TransferMode::Binary:
                        modeStr = "I";
                        break;
                    case Ftp::TransferMode::Ascii:
                        modeStr = "A";
                        break;
                    case Ftp::TransferMode::Ebcdic:
                        modeStr = "E";
                        break;
                }

                // Set the transfer mode
                response = m_ftp.sendCommand("TYPE", modeStr);
            }
            else
            {
                // Failed to connect to the server
                response = Ftp::Response(Ftp::Response::Status::ConnectionFailed);
            }
        }
    }

    return response;
}


////////////////////////////////////////////////////////////
void Ftp::DataChannel::receive(std::ostream& stream)
{
    // Receive data
    std::array<char, 1024> buffer{};
    std::size_t            received = 0;
    while (m_dataSocket.receive(buffer.data(), buffer.size(), received) == Socket::Status::Done)
    {
        stream.write(buffer.data(), static_cast<std::streamsize>(received));

        if (!stream.good())
        {
            err() << "FTP Error: Writing to the file has failed" << std::endl;
            break;
        }
    }

    // Close the data socket
    m_dataSocket.disconnect();
}


////////////////////////////////////////////////////////////
void Ftp::DataChannel::send(std::istream& stream)
{
    // Send data
    std::array<char, 1024> buffer{};
    std::size_t            count = 0;

    for (;;)
    {
        // read some data from the stream
        stream.read(buffer.data(), buffer.size());

        if (!stream.good() && !stream.eof())
        {
            err() << "FTP Error: Reading from the file has failed" << std::endl;
            break;
        }

        count = static_cast<std::size_t>(stream.gcount());

        if (count > 0)
        {
            // we could read more data from the stream: send them
            if (m_dataSocket.send(buffer.data(), count) != Socket::Status::Done)
                break;
        }
        else
        {
            // no more data: exit the loop
            break;
        }
    }

    // Close the data socket
    m_dataSocket.disconnect();
}

} // namespace sf
