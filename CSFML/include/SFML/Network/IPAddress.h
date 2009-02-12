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

#ifndef SFML_IPADDRESS_H
#define SFML_IPADDRESS_H

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Config.h>


////////////////////////////////////////////////////////////
/// sfIPAddress provides easy manipulation of IP v4 addresses
////////////////////////////////////////////////////////////
typedef struct
{
    char Address[16];
} sfIPAddress;


////////////////////////////////////////////////////////////
/// Construct an address from a string
///
/// \param String :  IP address ("xxx.xxx.xxx.xxx") or network name
///
/// \return Resulting address
///
////////////////////////////////////////////////////////////
CSFML_API sfIPAddress sfIPAddress_FromString(const char* String);

////////////////////////////////////////////////////////////
/// Construct an address from 4 bytes
///
/// \param Byte0 :   First byte of the address
/// \param Byte1 :   Second byte of the address
/// \param Byte2 :   Third byte of the address
/// \param Byte3 :   Fourth byte of the address
///
/// \return Resulting address
///
////////////////////////////////////////////////////////////
CSFML_API sfIPAddress sfIPAddress_FromBytes(sfUint8 Byte0, sfUint8 Byte1, sfUint8 Byte2, sfUint8 Byte3);

////////////////////////////////////////////////////////////
/// Construct the address from a 32-bits integer
///
/// \param Address : 4 bytes of the address packed into a 32-bits integer
///
////////////////////////////////////////////////////////////
CSFML_API sfIPAddress sfIPAddress_FromInteger(sfUint32 Address);

////////////////////////////////////////////////////////////
/// Tell if an address is a valid one
///
/// \param Address : Address to check
///
/// \return sfTrue if address has a valid syntax
///
////////////////////////////////////////////////////////////
CSFML_API sfBool sfIPAddress_IsValid(sfIPAddress Address);

////////////////////////////////////////////////////////////
/// Get a string representation of an address
///
/// \param Address : Address to convert
/// \param String :  Char array to fill (size must be >= 16)
///
////////////////////////////////////////////////////////////
CSFML_API void sfIPAddress_ToString(sfIPAddress Address, char* String);

////////////////////////////////////////////////////////////
/// Get an integer representation of the address
///
/// \param Address : Address to convert
///
/// \return 32-bits integer containing the 4 bytes of the address, in system endianness
///
////////////////////////////////////////////////////////////
CSFML_API sfUint32 sfIPAddress_ToInteger(sfIPAddress Address);

////////////////////////////////////////////////////////////
/// Get the computer's local IP address (from the LAN point of view)
///
/// \return Local IP address
///
////////////////////////////////////////////////////////////
CSFML_API sfIPAddress sfIPAddress_GetLocalAddress();

////////////////////////////////////////////////////////////
/// Get the computer's public IP address (from the web point of view).
/// The only way to get a public address is to ask it to a
/// distant website ; as a consequence, this function may be
/// very slow -- use it as few as possible !
///
/// \param Timeout : Maximum time to wait, in seconds (use 0 for no timeout)
///
/// \return Public IP address
///
////////////////////////////////////////////////////////////
CSFML_API sfIPAddress sfIPAddress_GetPublicAddress(float Timeout);

////////////////////////////////////////////////////////////
/// Get the computer's loopback address
///
/// \return Local host IP address (127.0.0.1, or "localhost")
///
////////////////////////////////////////////////////////////
CSFML_API sfIPAddress sfIPAddress_LocalHost();


#endif // SFML_IPADDRESS_H
