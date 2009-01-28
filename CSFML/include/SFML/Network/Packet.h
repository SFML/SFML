////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007-2008 Laurent Gomila (laurent.gom@gmail.com)
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


////////////////////////////////////////////////////////////
/// sfPacket wraps data to send / to receive through the network
////////////////////////////////////////////////////////////
typedef struct sfPacket sfPacket;


////////////////////////////////////////////////////////////
/// Create a new empty packet
///
/// \return A new sfPacket object
///
////////////////////////////////////////////////////////////
CSFML_API sfPacket* sfPacket_Create();

////////////////////////////////////////////////////////////
/// Destroy an existing packet
///
/// \param Packet : Packet to delete
///
////////////////////////////////////////////////////////////
CSFML_API void sfPacket_Destroy(sfPacket* Packet);

////////////////////////////////////////////////////////////
/// Append data to the end of a packet
///
/// \param Packet :      Packet to fill
/// \param Data :        Pointer to the bytes to append
/// \param SizeInBytes : Number of bytes to append
///
////////////////////////////////////////////////////////////
CSFML_API void sfPacket_Append(sfPacket* Packet, void* Data, size_t SizeInBytes);

////////////////////////////////////////////////////////////
/// Clear all the data of a packet
///
/// \param Packet : Packet to clear
///
///////////////////////////////////////////////////////////
CSFML_API void sfPacket_Clear(sfPacket* Packet);

////////////////////////////////////////////////////////////
/// Get a pointer to the data contained in a packet
/// Warning : the returned pointer may be invalid after you
/// append data to the packet
///
/// \param Packet : Packet to get data from
///
/// \return Pointer to the data
///
////////////////////////////////////////////////////////////
CSFML_API const char* sfPacket_GetData(sfPacket* Packet);

////////////////////////////////////////////////////////////
/// Get the size of the data contained in a packet
///
/// \param Packet : Packet to get data size from
///
/// \return Data size, in bytes
///
////////////////////////////////////////////////////////////
CSFML_API size_t sfPacket_GetDataSize(sfPacket* Packet);

////////////////////////////////////////////////////////////
/// Tell if the reading position has reached the end of the packet
///
/// \param Packet : Packet to check
///
/// \return sfTrue if all data have been read into the packet
///
////////////////////////////////////////////////////////////
CSFML_API sfBool sfPacket_EndOfPacket(sfPacket* Packet);

////////////////////////////////////////////////////////////
/// Return the validity of packet
///
/// \param Packet : Packet to check
///
/// \return sfTrue if last data extraction from packet was successful
///
////////////////////////////////////////////////////////////
CSFML_API sfBool sfPacket_CanRead(sfPacket* Packet);

////////////////////////////////////////////////////////////
/// Functions to extract data from a packet
///
/// \param Packet : Packet to read
///
////////////////////////////////////////////////////////////
CSFML_API sfBool   sfPacket_ReadBool(sfPacket* Packet);
CSFML_API sfInt8   sfPacket_ReadInt8(sfPacket* Packet);
CSFML_API sfUint8  sfPacket_ReadUint8(sfPacket* Packet);
CSFML_API sfInt16  sfPacket_ReadInt16(sfPacket* Packet);
CSFML_API sfUint16 sfPacket_ReadUint16(sfPacket* Packet);
CSFML_API sfInt32  sfPacket_ReadInt32(sfPacket* Packet);
CSFML_API sfUint32 sfPacket_ReadUint32(sfPacket* Packet);
CSFML_API float    sfPacket_ReadFloat(sfPacket* Packet);
CSFML_API double   sfPacket_ReadDouble(sfPacket* Packet);
CSFML_API void     sfPacket_ReadString(sfPacket* Packet, char* String);
CSFML_API void     sfPacket_ReadWideString(sfPacket* Packet, wchar_t* String);

////////////////////////////////////////////////////////////
/// Functions to insert data into a packet
///
/// \param Packet : Packet to write
///
////////////////////////////////////////////////////////////
CSFML_API void sfPacket_WriteBool(sfPacket* Packet, sfBool);
CSFML_API void sfPacket_WriteInt8(sfPacket* Packet, sfInt8);
CSFML_API void sfPacket_WriteUint8(sfPacket* Packet, sfUint8);
CSFML_API void sfPacket_WriteInt16(sfPacket* Packet, sfInt16);
CSFML_API void sfPacket_WriteUint16(sfPacket* Packet, sfUint16);
CSFML_API void sfPacket_WriteInt32(sfPacket* Packet, sfInt32);
CSFML_API void sfPacket_WriteUint32(sfPacket* Packet, sfUint32);
CSFML_API void sfPacket_WriteFloat(sfPacket* Packet, float);
CSFML_API void sfPacket_WriteDouble(sfPacket* Packet, double);
CSFML_API void sfPacket_WriteString(sfPacket* Packet, const char* String);
CSFML_API void sfPacket_WriteWideString(sfPacket* Packet, const wchar_t* String);


#endif // SFML_PACKET_H
