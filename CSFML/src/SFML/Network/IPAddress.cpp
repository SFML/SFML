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
#include <SFML/Network/IPAddress.h>
#include <SFML/Network/IPAddress.hpp>
#include <string.h>


namespace
{
    ////////////////////////////////////////////////////////////
    /// Helper function for converting a SFML address to a CSFML one
    ////////////////////////////////////////////////////////////
    sfIPAddress FromSFMLAddress(sf::IPAddress Address)
    {
        sfIPAddress Result;
        strncpy(Result.Address, Address.ToString().c_str(), 16);

        return Result;
    }

    ////////////////////////////////////////////////////////////
    /// Helper function for converting a CSFML address to a SFML one
    ////////////////////////////////////////////////////////////
    sf::IPAddress ToSFMLAddress(sfIPAddress Address)
    {
        return sf::IPAddress(Address.Address);
    }
}


////////////////////////////////////////////////////////////
/// Construct an address from a string
////////////////////////////////////////////////////////////
sfIPAddress sfIPAddress_FromString(const char* String)
{
    return FromSFMLAddress(sf::IPAddress(String));
}


////////////////////////////////////////////////////////////
/// Construct an address from 4 bytes
////////////////////////////////////////////////////////////
sfIPAddress sfIPAddress_FromBytes(sfUint8 Byte0, sfUint8 Byte1, sfUint8 Byte2, sfUint8 Byte3)
{
    return FromSFMLAddress(sf::IPAddress(Byte0, Byte1, Byte2, Byte3));
}


////////////////////////////////////////////////////////////
/// Construct the address from a 32-bits integer
////////////////////////////////////////////////////////////
sfIPAddress sfIPAddress_FromInteger(sfUint32 Address)
{
    return FromSFMLAddress(sf::IPAddress(Address));
}


////////////////////////////////////////////////////////////
/// Tell if an address is a valid one
////////////////////////////////////////////////////////////
sfBool sfIPAddress_IsValid(sfIPAddress Address)
{
    return ToSFMLAddress(Address).IsValid() ? sfTrue : sfFalse;
}


////////////////////////////////////////////////////////////
/// Get a string representation of an address
////////////////////////////////////////////////////////////
void sfIPAddress_ToString(sfIPAddress Address, char* String)
{
    if (String)
        strcpy(String, Address.Address);
}


////////////////////////////////////////////////////////////
/// Get an integer representation of the address
////////////////////////////////////////////////////////////
sfUint32 sfIPAddress_ToInteger(sfIPAddress Address)
{
    return ToSFMLAddress(Address).ToInteger();
}


////////////////////////////////////////////////////////////
/// Get the computer's local IP address (from the LAN point of view)
////////////////////////////////////////////////////////////
sfIPAddress sfIPAddress_GetLocalAddress()
{
    return FromSFMLAddress(sf::IPAddress::GetLocalAddress());
}


////////////////////////////////////////////////////////////
/// Get the computer's public IP address (from the web point of view).
/// The only way to get a public address is to ask it to a
/// distant website ; as a consequence, this function may be
/// very slow -- use it as few as possible !
////////////////////////////////////////////////////////////
sfIPAddress sfIPAddress_GetPublicAddress(float Timeout)
{
    return FromSFMLAddress(sf::IPAddress::GetPublicAddress(Timeout));
}


////////////////////////////////////////////////////////////
/// Get the computer's loopback address
////////////////////////////////////////////////////////////
sfIPAddress sfIPAddress_LocalHost()
{
    return FromSFMLAddress(sf::IPAddress::LocalHost);
}
