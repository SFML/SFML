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
#include <SFML/Network/IpAddress.h>
#include <SFML/Network/IpAddress.hpp>
#include <string.h>


namespace
{
    ////////////////////////////////////////////////////////////
    /// Helper function for converting a SFML address to a CSFML one
    ////////////////////////////////////////////////////////////
    sfIpAddress FromSFMLAddress(sf::IpAddress address)
    {
        sfIpAddress result;
        strncpy(result.Address, address.ToString().c_str(), 16);

        return result;
    }

    ////////////////////////////////////////////////////////////
    /// Helper function for converting a CSFML address to a SFML one
    ////////////////////////////////////////////////////////////
    sf::IpAddress ToSFMLAddress(sfIpAddress address)
    {
        return sf::IpAddress(address.Address);
    }
}


////////////////////////////////////////////////////////////
/// Construct an address from a string
////////////////////////////////////////////////////////////
sfIpAddress sfIpAddress_FromString(const char* string)
{
    return FromSFMLAddress(sf::IpAddress(string));
}


////////////////////////////////////////////////////////////
/// Construct an address from 4 bytes
////////////////////////////////////////////////////////////
sfIpAddress sfIpAddress_FromBytes(sfUint8 byte0, sfUint8 byte1, sfUint8 byte2, sfUint8 byte3)
{
    return FromSFMLAddress(sf::IpAddress(byte0, byte1, byte2, byte3));
}


////////////////////////////////////////////////////////////
/// Construct the address from a 32-bits integer
////////////////////////////////////////////////////////////
sfIpAddress sfIpAddress_FromInteger(sfUint32 address)
{
    return FromSFMLAddress(sf::IpAddress(address));
}


////////////////////////////////////////////////////////////
/// Get a string representation of an address
////////////////////////////////////////////////////////////
void sfIpAddress_ToString(sfIpAddress address, char* string)
{
    if (string)
        strcpy(string, address.Address);
}


////////////////////////////////////////////////////////////
/// Get an integer representation of the address
////////////////////////////////////////////////////////////
sfUint32 sfIpAddress_ToInteger(sfIpAddress address)
{
    return ToSFMLAddress(address).ToInteger();
}


////////////////////////////////////////////////////////////
/// Get the computer's local IP address (from the LAN point of view)
////////////////////////////////////////////////////////////
sfIpAddress sfIpAddress_GetLocalAddress(void)
{
    return FromSFMLAddress(sf::IpAddress::GetLocalAddress());
}


////////////////////////////////////////////////////////////
/// Get the computer's public IP address (from the web point of view).
/// The only way to get a public address is to ask it to a
/// distant website ; as a consequence, this function may be
/// very slow -- use it as few as possible !
////////////////////////////////////////////////////////////
sfIpAddress sfIpAddress_GetPublicAddress(float timeout)
{
    return FromSFMLAddress(sf::IpAddress::GetPublicAddress(timeout));
}


////////////////////////////////////////////////////////////
/// Get the computer's loopback address
////////////////////////////////////////////////////////////
sfIpAddress sfIpAddress_LocalHost(void)
{
    return FromSFMLAddress(sf::IpAddress::LocalHost);
}

////////////////////////////////////////////////////////////
/// Get the empty/invalid address
////////////////////////////////////////////////////////////
sfIpAddress sfIpAddress_None(void)
{
    return FromSFMLAddress(sf::IpAddress::None);
}
