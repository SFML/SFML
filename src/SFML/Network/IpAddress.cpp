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
#include <SFML/Network/IpAddress.hpp>
#include <SFML/Network/Http.hpp>
#include <SFML/Network/SocketImpl.hpp>


namespace sf
{
////////////////////////////////////////////////////////////
const IpAddress IpAddress::None;
const IpAddress IpAddress::LocalHost(127, 0, 0, 1);


////////////////////////////////////////////////////////////
IpAddress::IpAddress() :
myAddress(INADDR_NONE)
{

}


////////////////////////////////////////////////////////////
IpAddress::IpAddress(const std::string& address)
{
    // First try to convert it as a byte representation ("xxx.xxx.xxx.xxx")
    myAddress = inet_addr(address.c_str());

    // If not successful, try to convert it as a host name
    if (myAddress == INADDR_NONE)
    {
        hostent* host = gethostbyname(address.c_str());
        if (host)
        {
            // Host found, extract its IP address
            myAddress = reinterpret_cast<in_addr*>(host->h_addr)->s_addr;
        }
        else
        {
            // Host name not found on the network
            myAddress = INADDR_NONE;
        }
    }
}


////////////////////////////////////////////////////////////
IpAddress::IpAddress(const char* address)
{
    // First try to convert it as a byte representation ("xxx.xxx.xxx.xxx")
    myAddress = inet_addr(address);

    // If not successful, try to convert it as a host name
    if (myAddress == INADDR_NONE)
    {
        hostent* host = gethostbyname(address);
        if (host)
        {
            // Host found, extract its IP address
            myAddress = reinterpret_cast<in_addr*>(host->h_addr)->s_addr;
        }
        else
        {
            // Host name not found on the network
            myAddress = INADDR_NONE;
        }
    }
}


////////////////////////////////////////////////////////////
IpAddress::IpAddress(Uint8 byte0, Uint8 byte1, Uint8 byte2, Uint8 byte3)
{
    myAddress = htonl((byte0 << 24) | (byte1 << 16) | (byte2 << 8) | byte3);
}


////////////////////////////////////////////////////////////
IpAddress::IpAddress(Uint32 address)
{
    myAddress = htonl(address);
}


////////////////////////////////////////////////////////////
std::string IpAddress::ToString() const
{
    in_addr address;
    address.s_addr = myAddress;

    return inet_ntoa(address);
}


////////////////////////////////////////////////////////////
Uint32 IpAddress::ToInteger() const
{
    return ntohl(myAddress);
}


////////////////////////////////////////////////////////////
IpAddress IpAddress::GetLocalAddress()
{
    // The method here is to connect a UDP socket to anyone (here to localhost),
    // and get the local socket address with the getsockname function.
    // UDP connection will not send anything to the network, so this function won't cause any overhead.

    IpAddress localAddress;

    // Create the socket
    SocketHandle sock = socket(PF_INET, SOCK_DGRAM, 0);
    if (sock == priv::SocketImpl::InvalidSocket())
        return localAddress;

    // Connect the socket to localhost on any port
    sockaddr_in address = priv::SocketImpl::CreateAddress(INADDR_LOOPBACK, 0);
    if (connect(sock, reinterpret_cast<sockaddr*>(&address), sizeof(address)) == -1)
    {
        priv::SocketImpl::Close(sock);
        return localAddress;
    }
 
    // Get the local address of the socket connection
    priv::SocketImpl::AddrLength size = sizeof(address);
    if (getsockname(sock, reinterpret_cast<sockaddr*>(&address), &size) == -1)
    {
        priv::SocketImpl::Close(sock);
        return localAddress;
    }

    // Close the socket
    priv::SocketImpl::Close(sock);

    // Finally build the IP address
    localAddress = IpAddress(ntohl(address.sin_addr.s_addr));

    return localAddress;
}


////////////////////////////////////////////////////////////
IpAddress IpAddress::GetPublicAddress(float timeout)
{
    // The trick here is more complicated, because the only way
    // to get our public IP address is to get it from a distant computer.
    // Here we get the web page from http://www.sfml-dev.org/ip-provider.php
    // and parse the result to extract our IP address
    // (not very hard: the web page contains only our IP address).

    Http server("www.sfml-dev.org");
    Http::Request request("/ip-provider.php", Http::Request::Get);
    Http::Response page = server.SendRequest(request, timeout);
    if (page.GetStatus() == Http::Response::Ok)
        return IpAddress(page.GetBody());

    // Something failed: return an invalid address
    return IpAddress();
}


////////////////////////////////////////////////////////////
bool operator ==(const IpAddress& left, const IpAddress& right)
{
    return left.ToInteger() == right.ToInteger();
}


////////////////////////////////////////////////////////////
bool operator !=(const IpAddress& left, const IpAddress& right)
{
    return !(left == right);
}


////////////////////////////////////////////////////////////
bool operator <(const IpAddress& left, const IpAddress& right)
{
    return left.ToInteger() < right.ToInteger();
}


////////////////////////////////////////////////////////////
bool operator >(const IpAddress& left, const IpAddress& right)
{
    return right < left;
}


////////////////////////////////////////////////////////////
bool operator <=(const IpAddress& left, const IpAddress& right)
{
    return !(right < left);
}


////////////////////////////////////////////////////////////
bool operator >=(const IpAddress& left, const IpAddress& right)
{
    return !(left < right);
}


////////////////////////////////////////////////////////////
std::istream& operator >>(std::istream& stream, IpAddress& address)
{
    std::string str;
    stream >> str;
    address = IpAddress(str);

    return stream;
}


////////////////////////////////////////////////////////////
std::ostream& operator <<(std::ostream& stream, const IpAddress& address)
{
    return stream << address.ToString();
}

} // namespace sf
