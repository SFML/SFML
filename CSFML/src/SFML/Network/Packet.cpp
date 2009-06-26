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
#include <SFML/Network/Packet.h>
#include <SFML/Network/Packet.hpp>
#include <SFML/Internal.h>


struct sfPacket
{
    sf::Packet This;
};


namespace
{
    ////////////////////////////////////////////////////////////
    /// Helper function to read a variable from a packet
    ////////////////////////////////////////////////////////////
    template <typename T>
    T PacketRead(sfPacket* Packet)
    {
        CSFML_CHECK_RETURN(Packet, 0);

        T Value;
        Packet->This >> Value;
        return Value;
    }

    ////////////////////////////////////////////////////////////
    /// Helper function to write a variable to a packet
    ////////////////////////////////////////////////////////////
    template <typename T>
    void PacketWrite(sfPacket* Packet, T Value)
    {
        CSFML_CHECK(Packet);

        Packet->This << Value;
    }
}


////////////////////////////////////////////////////////////
/// Create a new empty packet
////////////////////////////////////////////////////////////
sfPacket* sfPacket_Create()
{
    return new sfPacket;
}


////////////////////////////////////////////////////////////
/// Destroy an existing packet
////////////////////////////////////////////////////////////
void sfPacket_Destroy(sfPacket* Packet)
{
    delete Packet;
}


////////////////////////////////////////////////////////////
/// Append data to the end of a packet
////////////////////////////////////////////////////////////
void sfPacket_Append(sfPacket* Packet, void* Data, size_t SizeInBytes)
{
    CSFML_CALL(Packet, Append(Data, SizeInBytes));
}


////////////////////////////////////////////////////////////
/// Clear all the data of a packet
///////////////////////////////////////////////////////////
void sfPacket_Clear(sfPacket* Packet)
{
    CSFML_CALL(Packet, Clear());
}


////////////////////////////////////////////////////////////
/// Get a pointer to the data contained in a packet
/// Warning : the returned pointer may be invalid after you
/// append data to the packet
////////////////////////////////////////////////////////////
const char* sfPacket_GetData(sfPacket* Packet)
{
    CSFML_CALL_RETURN(Packet, GetData(), NULL);
}


////////////////////////////////////////////////////////////
/// Get the size of the data contained in a packet
////////////////////////////////////////////////////////////
size_t sfPacket_GetDataSize(sfPacket* Packet)
{
    CSFML_CALL_RETURN(Packet, GetDataSize(), 0);
}


////////////////////////////////////////////////////////////
/// Tell if the reading position has reached the end of the packet
////////////////////////////////////////////////////////////
sfBool sfPacket_EndOfPacket(sfPacket* Packet)
{
    CSFML_CALL_RETURN(Packet, EndOfPacket(), sfFalse);
}


////////////////////////////////////////////////////////////
/// Check if a packet is in a valid reading state
////////////////////////////////////////////////////////////
sfBool sfPacket_CanRead(sfPacket* Packet)
{
    CSFML_CALL_RETURN(Packet, operator bool(), sfFalse);
}


////////////////////////////////////////////////////////////
/// Functions to extract data from a packet
///
/// \param Packet : Packet to read
///
////////////////////////////////////////////////////////////
sfBool   sfPacket_ReadBool(sfPacket* Packet)   {return PacketRead<sfUint8>(Packet);}
sfInt8   sfPacket_ReadInt8(sfPacket* Packet)   {return PacketRead<sfInt8>(Packet);}
sfUint8  sfPacket_ReadUint8(sfPacket* Packet)  {return PacketRead<sfUint8>(Packet);}
sfInt16  sfPacket_ReadInt16(sfPacket* Packet)  {return PacketRead<sfInt16>(Packet);}
sfUint16 sfPacket_ReadUint16(sfPacket* Packet) {return PacketRead<sfUint16>(Packet);}
sfInt32  sfPacket_ReadInt32(sfPacket* Packet)  {return PacketRead<sfInt32>(Packet);}
sfUint32 sfPacket_ReadUint32(sfPacket* Packet) {return PacketRead<sfUint32>(Packet);}
float    sfPacket_ReadFloat(sfPacket* Packet)  {return PacketRead<float>(Packet);}
double   sfPacket_ReadDouble(sfPacket* Packet) {return PacketRead<double>(Packet);}

void sfPacket_ReadString(sfPacket* Packet, char* String)
{
    CSFML_CHECK(Packet);

    if (String)
        Packet->This >> String;
}

void sfPacket_ReadWideString(sfPacket* Packet, wchar_t* String)
{
    CSFML_CHECK(Packet);

    if (String)
        Packet->This >> String;
}


////////////////////////////////////////////////////////////
/// Functions to insert data into a packet
///
/// \param Packet : Packet to write
///
////////////////////////////////////////////////////////////
void sfPacket_WriteBool(sfPacket* Packet,   sfBool Value)      {PacketWrite(Packet, static_cast<sfUint8>(Value));}
void sfPacket_WriteInt8(sfPacket* Packet,   sfInt8 Value)      {PacketWrite(Packet, Value);}
void sfPacket_WriteUint8(sfPacket* Packet,  sfUint8 Value)     {PacketWrite(Packet, Value);}
void sfPacket_WriteInt16(sfPacket* Packet,  sfInt16 Value)     {PacketWrite(Packet, Value);}
void sfPacket_WriteUint16(sfPacket* Packet, sfUint16 Value)    {PacketWrite(Packet, Value);}
void sfPacket_WriteInt32(sfPacket* Packet,  sfInt32 Value)     {PacketWrite(Packet, Value);}
void sfPacket_WriteUint32(sfPacket* Packet, sfUint32 Value)    {PacketWrite(Packet, Value);}
void sfPacket_WriteFloat(sfPacket* Packet,  float Value)       {PacketWrite(Packet, Value);}
void sfPacket_WriteDouble(sfPacket* Packet, double Value)      {PacketWrite(Packet, Value);}

void sfPacket_WriteString(sfPacket* Packet, const char* String)
{
    CSFML_CHECK(Packet);

    if (String)
        Packet->This << String;
}

void sfPacket_WriteWideString(sfPacket* Packet, const wchar_t* String)
{
    CSFML_CHECK(Packet);

    if (String)
        Packet->This << String;
}
