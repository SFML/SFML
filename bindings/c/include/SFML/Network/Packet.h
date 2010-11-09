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

#ifndef SFML_PACKET_H
#define SFML_PACKET_H

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Config.h>
#include <SFML/Network/Types.h>


////////////////////////////////////////////////////////////
/// Create a new empty packet
///
/// \return A new sfPacket object
///
////////////////////////////////////////////////////////////
CSFML_API sfPacket* sfPacket_Create(void);

////////////////////////////////////////////////////////////
/// Copy an existing packet
///
/// \param packet : Packet to copy
///
/// \return Copied object
///
////////////////////////////////////////////////////////////
CSFML_API sfPacket* sfPacket_Copy(sfPacket* packet);

////////////////////////////////////////////////////////////
/// Destroy an existing packet
///
/// \param packet : Packet to delete
///
////////////////////////////////////////////////////////////
CSFML_API void sfPacket_Destroy(sfPacket* packet);

////////////////////////////////////////////////////////////
/// Append data to the end of a packet
///
/// \param packet :      Packet to fill
/// \param data :        Pointer to the bytes to append
/// \param sizeInBytes : Number of bytes to append
///
////////////////////////////////////////////////////////////
CSFML_API void sfPacket_Append(sfPacket* packet, const void* data, size_t sizeInBytes);

////////////////////////////////////////////////////////////
/// Clear all the data of a packet
///
/// \param packet : Packet to clear
///
///////////////////////////////////////////////////////////
CSFML_API void sfPacket_Clear(sfPacket* packet);

////////////////////////////////////////////////////////////
/// Get a pointer to the data contained in a packet
/// Warning : the returned pointer may be invalid after you
/// append data to the packet
///
/// \param packet : Packet to get data from
///
/// \return Pointer to the data
///
////////////////////////////////////////////////////////////
CSFML_API const char* sfPacket_GetData(const sfPacket* packet);

////////////////////////////////////////////////////////////
/// Get the size of the data contained in a packet
///
/// \param packet : Packet to get data size from
///
/// \return Data size, in bytes
///
////////////////////////////////////////////////////////////
CSFML_API size_t sfPacket_GetDataSize(const sfPacket* packet);

////////////////////////////////////////////////////////////
/// Tell if the reading position has reached the end of the packet
///
/// \param packet : Packet to check
///
/// \return sfTrue if all data have been read into the packet
///
////////////////////////////////////////////////////////////
CSFML_API sfBool sfPacket_EndOfPacket(const sfPacket* packet);

////////////////////////////////////////////////////////////
/// Return the validity of packet
///
/// \param packet : Packet to check
///
/// \return sfTrue if last data extraction from packet was successful
///
////////////////////////////////////////////////////////////
CSFML_API sfBool sfPacket_CanRead(const sfPacket* packet);

////////////////////////////////////////////////////////////
/// Functions to extract data from a packet
///
/// \param packet : Packet to read
///
////////////////////////////////////////////////////////////
CSFML_API sfBool   sfPacket_ReadBool(sfPacket* packet);
CSFML_API sfInt8   sfPacket_ReadInt8(sfPacket* packet);
CSFML_API sfUint8  sfPacket_ReadUint8(sfPacket* packet);
CSFML_API sfInt16  sfPacket_ReadInt16(sfPacket* packet);
CSFML_API sfUint16 sfPacket_ReadUint16(sfPacket* packet);
CSFML_API sfInt32  sfPacket_ReadInt32(sfPacket* packet);
CSFML_API sfUint32 sfPacket_ReadUint32(sfPacket* packet);
CSFML_API float    sfPacket_ReadFloat(sfPacket* packet);
CSFML_API double   sfPacket_ReadDouble(sfPacket* packet);
CSFML_API void     sfPacket_ReadString(sfPacket* packet, char* string);
CSFML_API void     sfPacket_ReadWideString(sfPacket* packet, wchar_t* string);

////////////////////////////////////////////////////////////
/// Functions to insert data into a packet
///
/// \param packet : Packet to write
///
////////////////////////////////////////////////////////////
CSFML_API void sfPacket_WriteBool(sfPacket* packet, sfBool);
CSFML_API void sfPacket_WriteInt8(sfPacket* packet, sfInt8);
CSFML_API void sfPacket_WriteUint8(sfPacket* packet, sfUint8);
CSFML_API void sfPacket_WriteInt16(sfPacket* packet, sfInt16);
CSFML_API void sfPacket_WriteUint16(sfPacket* packet, sfUint16);
CSFML_API void sfPacket_WriteInt32(sfPacket* packet, sfInt32);
CSFML_API void sfPacket_WriteUint32(sfPacket* packet, sfUint32);
CSFML_API void sfPacket_WriteFloat(sfPacket* packet, float);
CSFML_API void sfPacket_WriteDouble(sfPacket* packet, double);
CSFML_API void sfPacket_WriteString(sfPacket* packet, const char* string);
CSFML_API void sfPacket_WriteWideString(sfPacket* packet, const wchar_t* string);


#endif // SFML_PACKET_H
