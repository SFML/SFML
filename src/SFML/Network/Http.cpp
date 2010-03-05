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
#include <SFML/Network/Http.hpp>
#include <ctype.h>
#include <algorithm>
#include <iterator>
#include <sstream>


namespace
{
    ////////////////////////////////////////////////////////////
    // Convenience function to convert a string to lower case
    ////////////////////////////////////////////////////////////
    std::string ToLower(std::string str)
    {
        for (std::string::iterator i = str.begin(); i != str.end(); ++i)
            *i = static_cast<char>(tolower(*i));

        return str;
    }
}


namespace sf
{
////////////////////////////////////////////////////////////
/// Default constructor
////////////////////////////////////////////////////////////
Http::Request::Request(Method method, const std::string& URI, const std::string& body)
{
    SetMethod(method);
    SetURI(URI);
    SetHttpVersion(1, 0);
    SetBody(body);
}


////////////////////////////////////////////////////////////
/// Set the value of a field; the field is added if it doesn't exist
////////////////////////////////////////////////////////////
void Http::Request::SetField(const std::string& field, const std::string& value)
{
    myFields[ToLower(field)] = value;
}


////////////////////////////////////////////////////////////
/// Set the request method.
/// This parameter is Get by default
////////////////////////////////////////////////////////////
void Http::Request::SetMethod(Http::Request::Method method)
{
    myMethod = method;
}


////////////////////////////////////////////////////////////
/// Set the target URI of the request.
/// This parameter is "/" by default
////////////////////////////////////////////////////////////
void Http::Request::SetURI(const std::string& URI)
{
    myURI = URI;

    // Make sure it starts with a '/'
    if (myURI.empty() || (myURI[0] != '/'))
        myURI.insert(0, "/");
}


////////////////////////////////////////////////////////////
/// Set the HTTP version of the request.
/// This parameter is 1.0 by default
////////////////////////////////////////////////////////////
void Http::Request::SetHttpVersion(unsigned int major, unsigned int minor)
{
    myMajorVersion = major;
    myMinorVersion = minor;
}


////////////////////////////////////////////////////////////
/// Set the body of the request. This parameter is optional and
/// makes sense only for POST requests.
/// This parameter is empty by default
////////////////////////////////////////////////////////////
void Http::Request::SetBody(const std::string& body)
{
    myBody = body;
}


////////////////////////////////////////////////////////////
/// Get the string representation of a request header
////////////////////////////////////////////////////////////
std::string Http::Request::ToString() const
{
    std::ostringstream out;

    // Convert the method to its string representation
    std::string method;
    switch (myMethod)
    {
        default :
        case Get :  method = "GET";  break;
        case Post : method = "POST"; break;
        case Head : method = "HEAD"; break;
    }

    // Write the first line containing the request type
    out << method << " " << myURI << " ";
    out << "HTTP/" << myMajorVersion << "." << myMinorVersion << "\r\n";

    // Write fields
    for (FieldTable::const_iterator i = myFields.begin(); i != myFields.end(); ++i)
    {
        out << i->first << ": " << i->second << "\r\n";
    }

    // Use an extra \r\n to separate the header from the body
    out << "\r\n";

    // Add the body
    out << myBody;

    return out.str();
}


////////////////////////////////////////////////////////////
/// Check if the given field has been defined
////////////////////////////////////////////////////////////
bool Http::Request::HasField(const std::string& field) const
{
    return myFields.find(field) != myFields.end();
}


////////////////////////////////////////////////////////////
/// Default constructor
////////////////////////////////////////////////////////////
Http::Response::Response() :
myStatus      (ConnectionFailed),
myMajorVersion(0),
myMinorVersion(0)
{

}


////////////////////////////////////////////////////////////
/// Get the value of a field
////////////////////////////////////////////////////////////
const std::string& Http::Response::GetField(const std::string& field) const
{
    FieldTable::const_iterator it = myFields.find(ToLower(field));
    if (it != myFields.end())
    {
        return it->second;
    }
    else
    {
        static const std::string empty = "";
        return empty;
    }
}


////////////////////////////////////////////////////////////
/// Get the header's status code
////////////////////////////////////////////////////////////
Http::Response::Status Http::Response::GetStatus() const
{
    return myStatus;
}


////////////////////////////////////////////////////////////
/// Get the major HTTP version number of the response
////////////////////////////////////////////////////////////
unsigned int Http::Response::GetMajorHttpVersion() const
{
    return myMajorVersion;
}


////////////////////////////////////////////////////////////
/// Get the major HTTP version number of the response
////////////////////////////////////////////////////////////
unsigned int Http::Response::GetMinorHttpVersion() const
{
    return myMinorVersion;
}


////////////////////////////////////////////////////////////
/// Get the body of the response. The body can contain :
/// - the requested page (for GET requests)
/// - a response from the server (for POST requests)
/// - nothing (for HEAD requests)
/// - an error message (in case of an error)
////////////////////////////////////////////////////////////
const std::string& Http::Response::GetBody() const
{
    return myBody;
}


////////////////////////////////////////////////////////////
/// Construct the header from a response string
////////////////////////////////////////////////////////////
void Http::Response::FromString(const std::string& data)
{
    std::istringstream in(data);

    // Extract the HTTP version from the first line
    std::string version;
    if (in >> version)
    {
        if ((version.size() >= 8) && (version[6] == '.') &&
            (ToLower(version.substr(0, 5)) == "http/")   &&
             isdigit(version[5]) && isdigit(version[7]))
        {
            myMajorVersion = version[5] - '0';
            myMinorVersion = version[7] - '0';
        }
        else
        {
            // Invalid HTTP version
            myStatus = InvalidResponse;
            return;
        }
    }

    // Extract the status code from the first line
    int status;
    if (in >> status)
    {
        myStatus = static_cast<Status>(status);
    }
    else
    {
        // Invalid status code
        myStatus = InvalidResponse;
        return;
    }

    // Ignore the end of the first line
    in.ignore(10000, '\n');

    // Parse the other lines, which contain fields, one by one
    std::string line;
    while (std::getline(in, line) && (line.size() > 2))
    {
        std::string::size_type pos = line.find(": ");
        if (pos != std::string::npos)
        {
            // Extract the field name and its value
            std::string field = line.substr(0, pos);
            std::string value = line.substr(pos + 2);

            // Remove any trailing \r
            if (!value.empty() && (*value.rbegin() == '\r'))
                value.erase(value.size() - 1);

            // Add the field
            myFields[ToLower(field)] = value;
        }
    }

    // Finally extract the body
    myBody.clear();
    std::copy(std::istreambuf_iterator<char>(in), std::istreambuf_iterator<char>(), std::back_inserter(myBody));
}


////////////////////////////////////////////////////////////
/// Default constructor
////////////////////////////////////////////////////////////
Http::Http() :
myHost(),
myPort(0)
{

}


////////////////////////////////////////////////////////////
/// Construct the Http instance with the target host
////////////////////////////////////////////////////////////
Http::Http(const std::string& host, unsigned short port)
{
    SetHost(host, port);
}


////////////////////////////////////////////////////////////
/// Set the target host
////////////////////////////////////////////////////////////
void Http::SetHost(const std::string& host, unsigned short port)
{
    // Detect the protocol used
    std::string protocol = ToLower(host.substr(0, 8));
    if (protocol.substr(0, 7) == "http://")
    {
        // HTTP protocol
        myHostName = host.substr(7);
        myPort     = (port != 0 ? port : 80);
    }
    else if (protocol == "https://")
    {
        // HTTPS protocol
        myHostName = host.substr(8);
        myPort     = (port != 0 ? port : 443);
    }
    else
    {
        // Undefined protocol - use HTTP
        myHostName = host;
        myPort     = (port != 0 ? port : 80);
    }

    // Remove any trailing '/' from the host name
    if (!myHostName.empty() && (*myHostName.rbegin() == '/'))
        myHostName.erase(myHostName.size() - 1);

    myHost = IPAddress(myHostName);
}


////////////////////////////////////////////////////////////
/// Send a HTTP request and return the server's response.
/// You must be connected to a host before sending requests.
/// Any missing mandatory header field will be added with an appropriate value.
/// Warning : this function waits for the server's response and may
/// not return instantly; use a thread if you don't want to block your
/// application.
////////////////////////////////////////////////////////////
Http::Response Http::SendRequest(const Http::Request& request, float timeout)
{
    // First make sure the request is valid -- add missing mandatory fields
    Request toSend(request);
    if (!toSend.HasField("From"))
    {
        toSend.SetField("From", "user@sfml-dev.org");
    }
    if (!toSend.HasField("User-Agent"))
    {
        toSend.SetField("User-Agent", "libsfml-network/2.x");
    }
    if (!toSend.HasField("Host"))
    {
        toSend.SetField("Host", myHostName);
    }
    if (!toSend.HasField("Content-Length"))
    {
        std::ostringstream out;
        out << toSend.myBody.size();
        toSend.SetField("Content-Length", out.str());
    }
    if ((toSend.myMethod == Request::Post) && !toSend.HasField("Content-Type"))
    {
        toSend.SetField("Content-Type", "application/x-www-form-urlencoded");
    }
    if ((toSend.myMajorVersion * 10 + toSend.myMinorVersion >= 11) && !toSend.HasField("Connection"))
    {
        toSend.SetField("Connection", "close");
    }

    // Prepare the response
    Response received;

    // Connect the socket to the host
    if (myConnection.Connect(myPort, myHost, timeout) == Socket::Done)
    {
        // Convert the request to string and send it through the connected socket
        std::string requestStr = toSend.ToString();

        if (!requestStr.empty())
        {
            // Send it through the socket
            if (myConnection.Send(requestStr.c_str(), requestStr.size()) == Socket::Done)
            {
                // Wait for the server's response
                std::string receivedStr;
                std::size_t size = 0;
                char buffer[1024];
                while (myConnection.Receive(buffer, sizeof(buffer), size) == Socket::Done)
                {
                    receivedStr.append(buffer, buffer + size);
                }

                // Build the Response object from the received data
                received.FromString(receivedStr);
            }
        }

        // Close the connection
        myConnection.Close();
    }

    return received;
}

} // namespace sf
