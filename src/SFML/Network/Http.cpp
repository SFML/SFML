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
#include <cctype>
#include <algorithm>
#include <iterator>
#include <sstream>


namespace
{
    // Convert a string to lower case
    std::string ToLower(std::string str)
    {
        for (std::string::iterator i = str.begin(); i != str.end(); ++i)
            *i = static_cast<char>(std::tolower(*i));
        return str;
    }
}


namespace sf
{
////////////////////////////////////////////////////////////
Http::Request::Request(const std::string& uri, Method method, const std::string& body)
{
    SetMethod(method);
    SetUri(uri);
    SetHttpVersion(1, 0);
    SetBody(body);
}


////////////////////////////////////////////////////////////
void Http::Request::SetField(const std::string& field, const std::string& value)
{
    m_fields[ToLower(field)] = value;
}


////////////////////////////////////////////////////////////
void Http::Request::SetMethod(Http::Request::Method method)
{
    m_method = method;
}


////////////////////////////////////////////////////////////
void Http::Request::SetUri(const std::string& uri)
{
    m_uRI = uri;

    // Make sure it starts with a '/'
    if (m_uRI.empty() || (m_uRI[0] != '/'))
        m_uRI.insert(0, "/");
}


////////////////////////////////////////////////////////////
void Http::Request::SetHttpVersion(unsigned int major, unsigned int minor)
{
    m_majorVersion = major;
    m_minorVersion = minor;
}


////////////////////////////////////////////////////////////
void Http::Request::SetBody(const std::string& body)
{
    m_body = body;
}


////////////////////////////////////////////////////////////
std::string Http::Request::Prepare() const
{
    std::ostringstream out;

    // Convert the method to its string representation
    std::string method;
    switch (m_method)
    {
        default :
        case Get :  method = "GET";  break;
        case Post : method = "POST"; break;
        case Head : method = "HEAD"; break;
    }

    // Write the first line containing the request type
    out << method << " " << m_uRI << " ";
    out << "HTTP/" << m_majorVersion << "." << m_minorVersion << "\r\n";

    // Write fields
    for (FieldTable::const_iterator i = m_fields.begin(); i != m_fields.end(); ++i)
    {
        out << i->first << ": " << i->second << "\r\n";
    }

    // Use an extra \r\n to separate the header from the body
    out << "\r\n";

    // Add the body
    out << m_body;

    return out.str();
}


////////////////////////////////////////////////////////////
bool Http::Request::HasField(const std::string& field) const
{
    return m_fields.find(ToLower(field)) != m_fields.end();
}


////////////////////////////////////////////////////////////
Http::Response::Response() :
m_status      (ConnectionFailed),
m_majorVersion(0),
m_minorVersion(0)
{

}


////////////////////////////////////////////////////////////
const std::string& Http::Response::GetField(const std::string& field) const
{
    FieldTable::const_iterator it = m_fields.find(ToLower(field));
    if (it != m_fields.end())
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
Http::Response::Status Http::Response::GetStatus() const
{
    return m_status;
}


////////////////////////////////////////////////////////////
unsigned int Http::Response::GetMajorHttpVersion() const
{
    return m_majorVersion;
}


////////////////////////////////////////////////////////////
unsigned int Http::Response::GetMinorHttpVersion() const
{
    return m_minorVersion;
}


////////////////////////////////////////////////////////////
const std::string& Http::Response::GetBody() const
{
    return m_body;
}


////////////////////////////////////////////////////////////
void Http::Response::Parse(const std::string& data)
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
            m_majorVersion = version[5] - '0';
            m_minorVersion = version[7] - '0';
        }
        else
        {
            // Invalid HTTP version
            m_status = InvalidResponse;
            return;
        }
    }

    // Extract the status code from the first line
    int status;
    if (in >> status)
    {
        m_status = static_cast<Status>(status);
    }
    else
    {
        // Invalid status code
        m_status = InvalidResponse;
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
            m_fields[ToLower(field)] = value;
        }
    }

    // Finally extract the body
    m_body.clear();
    std::copy(std::istreambuf_iterator<char>(in), std::istreambuf_iterator<char>(), std::back_inserter(m_body));
}


////////////////////////////////////////////////////////////
Http::Http() :
m_host(),
m_port(0)
{

}


////////////////////////////////////////////////////////////
Http::Http(const std::string& host, unsigned short port)
{
    SetHost(host, port);
}


////////////////////////////////////////////////////////////
void Http::SetHost(const std::string& host, unsigned short port)
{
    // Detect the protocol used
    std::string protocol = ToLower(host.substr(0, 8));
    if (protocol.substr(0, 7) == "http://")
    {
        // HTTP protocol
        m_hostName = host.substr(7);
        m_port     = (port != 0 ? port : 80);
    }
    else if (protocol == "https://")
    {
        // HTTPS protocol
        m_hostName = host.substr(8);
        m_port     = (port != 0 ? port : 443);
    }
    else
    {
        // Undefined protocol - use HTTP
        m_hostName = host;
        m_port     = (port != 0 ? port : 80);
    }

    // Remove any trailing '/' from the host name
    if (!m_hostName.empty() && (*m_hostName.rbegin() == '/'))
        m_hostName.erase(m_hostName.size() - 1);

    m_host = IpAddress(m_hostName);
}


////////////////////////////////////////////////////////////
Http::Response Http::SendRequest(const Http::Request& request, Time timeout)
{
    // First make sure that the request is valid -- add missing mandatory fields
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
        toSend.SetField("Host", m_hostName);
    }
    if (!toSend.HasField("Content-Length"))
    {
        std::ostringstream out;
        out << toSend.m_body.size();
        toSend.SetField("Content-Length", out.str());
    }
    if ((toSend.m_method == Request::Post) && !toSend.HasField("Content-Type"))
    {
        toSend.SetField("Content-Type", "application/x-www-form-urlencoded");
    }
    if ((toSend.m_majorVersion * 10 + toSend.m_minorVersion >= 11) && !toSend.HasField("Connection"))
    {
        toSend.SetField("Connection", "close");
    }

    // Prepare the response
    Response received;

    // Connect the socket to the host
    if (m_connection.Connect(m_host, m_port, timeout) == Socket::Done)
    {
        // Convert the request to string and send it through the connected socket
        std::string requestStr = toSend.Prepare();

        if (!requestStr.empty())
        {
            // Send it through the socket
            if (m_connection.Send(requestStr.c_str(), requestStr.size()) == Socket::Done)
            {
                // Wait for the server's response
                std::string receivedStr;
                std::size_t size = 0;
                char buffer[1024];
                while (m_connection.Receive(buffer, sizeof(buffer), size) == Socket::Done)
                {
                    receivedStr.append(buffer, buffer + size);
                }

                // Build the Response object from the received data
                received.Parse(receivedStr);
            }
        }

        // Close the connection
        m_connection.Disconnect();
    }

    return received;
}

} // namespace sf
