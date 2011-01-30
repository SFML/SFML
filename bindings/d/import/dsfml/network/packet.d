/*
*	DSFML - SFML Library wrapper for the D programming language.
*	Copyright (C) 2008 Julien Dagorn (sirjulio13@gmail.com)
*	Copyright (C) 2010 Andreas Hollandt
*
*	This software is provided 'as-is', without any express or
*	implied warranty. In no event will the authors be held
*	liable for any damages arising from the use of this software.
*
*	Permission is granted to anyone to use this software for any purpose,
*	including commercial applications, and to alter it and redistribute
*	it freely, subject to the following restrictions:
*
*	1.  The origin of this software must not be misrepresented;
*		you must not claim that you wrote the original software.
*		If you use this software in a product, an acknowledgment
*		in the product documentation would be appreciated but
*		is not required.
*
*	2.  Altered source versions must be plainly marked as such,
*		and must not be misrepresented as being the original software.
*
*	3.  This notice may not be removed or altered from any
*		source distribution.
*/

module dsfml.network.packet;

import dsfml.system.common;
import dsfml.system.stringutil;

/** 
*	Packet wraps data to send / to receive through the network
*	
*	The order of insertion and extraction must be the same.
*	
*	You can derive from Packet and override onSend and onReceive to do custom operations before send or after reception.	
*
*	Litterals integer are promoted to int.
*	Litterals floating point are promoted to float.
*	
*	Extraction or insertion can be specified with explicit template.
*	Examples:
*	------------------------------------------------------------
*	Packet p = new Packet();
*	
*	int i = 32, j = 42;
*	string k = hello;
*	
*	p.set(i, k, j); //Set the data in the packet
*	
*	int a, b;
*	string c;
*	p.get(a, c, b); //Get data from the packet
*	
*	//...
*	
*	Packet p = new Packet();
*	p.set!(byte)(5); // Litteral are inserted with byte type
*	------------------------------------------------------------
*	
*	See_Also:
*		$(LINK2 http://www.digitalmars.com/d/1.0/lex.html, D litterals Specification) for more informations.
*/
class Packet : DSFMLObject
{
	/**
	* Default constructor
	*
	*/
	this()
	{
		super(sfPacket_Create());
	}

	override void dispose()
	{
		sfPacket_Destroy(m_ptr);
	}

	/**
	*	Append data to the end of the packet.
	*	
	*	Params: 
	*		data = Array of data to append
	*
	*/
	void append(byte[] data)
	{
		if (data !is null)
			sfPacket_Append(m_ptr, data.ptr, data.length);
	}

	/**
	*	Clear the packet data
	*
	*/
	void clear()
	{
		sfPacket_Clear(m_ptr);
	}

	/**
	*	Get an array to the data contained in the packet
	*	$(B the returned array may be invalid after you
	*	append data to the packet)
	*
	*	Returns: 
	*		array of data
	*		
	*	Remarks:
	*		return an array of $(B all) data in the packet.
	*		
	*	----------
	*	Packet p = new Packet();
	*	
	*	string str1 = "Hi";
	*	string str2 = "Hello";
	*	
	*	p.set(str1, str2);
	*	
	*	// Retrieve str1 from packet	
	*	string str3;	
	*	p.get(str3);
	*	
	*	// Returns an array containing str1 and str2.	
	*	byte[] ptr = p.getData();										 
	*	----------						 
	*/
	byte[] getData()
	{
		if (canRead)
			return sfPacket_GetData(m_ptr)[0..getDataSize()];
		else
			return null;
	}

	
	/**
	*	Get the size of the data contained in the packet
	*
	*	Returns: 
	*		Data size, in bytes
	*/
	uint getDataSize() 
	{
		return sfPacket_GetDataSize(m_ptr);
	}
	
	/**
	*	Tell if the reading position has reached the end of the packet
	*	
	*	Returns:
	*		true if all data have been read.			
	*/		
	bool endOfPacket()
	{
		return cast(bool)sfPacket_EndOfPacket(m_ptr);
	}
	/**
	*	Tell if the packet is valid for reading
	*
	*	Returns: 
	*		True if data can be extracted from the packet
	*
	*/
	bool canRead() 
	{
		return cast(bool)sfPacket_CanRead(m_ptr);
	}
		
	
	/**
	*	Add new variables to the packet
	*	Accept (u)byte, (u)short, (u)int, float, double, string and wstring types  
	*/		
	Packet set(T...)(T t)
	{
		foreach (v; t)
			internalSet(t);
		return this;
	}

	/**
	*	Retrieve data from the packet
	*	Accept (u)byte, (u)short, (u)int, float, double, string and wstring types	
	*/		
	Packet get(T...)(ref T t)
	{
		foreach (v; t)
			internalGet(t);
		return this;
	}

	/**
	*	Called before packet is send
	*	
	*	Params:
	*		size = Variable to fill with the size of the data to send
	*	Returns:
	*		Array of byte to send					
	*/		
	byte[] onSend()
	{
		return getData();
	}
	
	/**
	*	Called after a packet has been received
	*	
	*	Params:
	*		data = Array of byte received			
	*/		
	void onReceive(byte[] data)
	{
		append(data);
	}

private:
	void internalGet(ref bool data)
	{
		data = cast(bool)sfPacket_ReadInt32(m_ptr);
	}
	void internalGet(ref byte data)
	{
		data = sfPacket_ReadInt8(m_ptr);
	}
	void internalGet(ref ubyte data)
	{
		data = sfPacket_ReadUint8(m_ptr);
	}
	void internalGet(ref short data)
	{
		data = sfPacket_ReadInt16(m_ptr);
	}
	void internalGet(ref ushort data)
	{
		data = sfPacket_ReadUint16(m_ptr);
	}
	void internalGet(ref int data)
	{
		data = sfPacket_ReadInt32(m_ptr);
	}
	void internalGet(ref uint data)
	{
		data = sfPacket_ReadUint32(m_ptr);
	}
	void internalGet(ref float data)
	{
		data = sfPacket_ReadFloat(m_ptr);
	}
	void internalGet(ref double data)
	{
		data = sfPacket_ReadDouble(m_ptr);
	}
	void internalGet(ref string data)
	{
		scope char[] temp = new char[sfPacket_GetDataSize(m_ptr)];
		sfPacket_ReadString(m_ptr, temp.ptr);
		size_t l = fromStringz(temp.ptr).length;
		data = cast(string) temp[0 .. l];
	}
	
	void internalGet(ref wstring data)
	{
		scope wchar[] temp = new wchar[sfPacket_GetDataSize(m_ptr)];
		sfPacket_ReadWideString(m_ptr, temp.ptr);
		size_t l = fromStringz(temp.ptr).length;
		data = cast(wstring) temp[0 .. l];
	}
	
	void internalSet(bool data)
	{
		sfPacket_WriteInt32(m_ptr, cast(int)data);
	}
	void internalSet(byte data)
	{
		sfPacket_WriteInt8(m_ptr, data);
	}
	void internalSet(ubyte data)
	{
		sfPacket_WriteUint8(m_ptr, data);
	}
	void internalSet(short data)
	{
		sfPacket_WriteInt16(m_ptr, data);
	}
	void internalSet(ushort data)
	{
		sfPacket_WriteUint16(m_ptr, data);
	}
	void internalSet(int data)
	{
		sfPacket_WriteInt32(m_ptr, data);
	}
	void internalSet(uint data)
	{
		sfPacket_WriteUint32(m_ptr, data);
	}
	void internalSet(float data)
	{
		sfPacket_WriteFloat(m_ptr, data);
	}
	void internalSet(double data)
	{
		sfPacket_WriteDouble(m_ptr, data);
	}
	void internalSet(string data)
	{
		sfPacket_WriteString(m_ptr, toStringz(data));
	}

	void internalSet(wstring data)
	{
		sfPacket_WriteWideString(m_ptr, toStringz(data));
	}

// External ====================================================================

	extern (C)
	{
		typedef SFMLClass function() pf_sfPacket_Create;
		typedef void function(SFMLClass) pf_sfPacket_Destroy;
		typedef void function(SFMLClass, const(void)*, size_t) pf_sfPacket_Append;
		typedef void function(SFMLClass) pf_sfPacket_Clear;
		typedef byte* function(SFMLClass) pf_sfPacket_GetData;
		typedef uint function(SFMLClass) pf_sfPacket_GetDataSize;
		typedef int function(SFMLClass) pf_sfPacket_EndOfPacket;
		typedef int function(SFMLClass) pf_sfPacket_CanRead;
		typedef byte function(SFMLClass) pf_sfPacket_ReadInt8;
		typedef ubyte function(SFMLClass) pf_sfPacket_ReadUint8;
		typedef short function(SFMLClass) pf_sfPacket_ReadInt16;
		typedef ushort function(SFMLClass) pf_sfPacket_ReadUint16;
		typedef int function(SFMLClass) pf_sfPacket_ReadInt32;
		typedef uint function(SFMLClass) pf_sfPacket_ReadUint32;
		typedef float function(SFMLClass) pf_sfPacket_ReadFloat;
		typedef double function(SFMLClass) pf_sfPacket_ReadDouble;
		typedef void function(SFMLClass, char*) pf_sfPacket_ReadString;
		typedef void function(SFMLClass, wchar*) pf_sfPacket_ReadWideString;
		typedef void function(SFMLClass, byte) pf_sfPacket_WriteInt8;
		typedef void function(SFMLClass, ubyte) pf_sfPacket_WriteUint8;
		typedef void function(SFMLClass, short) pf_sfPacket_WriteInt16;
		typedef void function(SFMLClass, ushort) pf_sfPacket_WriteUint16;
		typedef void function(SFMLClass, int) pf_sfPacket_WriteInt32;
		typedef void function(SFMLClass, uint) pf_sfPacket_WriteUint32;
		typedef void function(SFMLClass, float) pf_sfPacket_WriteFloat;
		typedef void function(SFMLClass, double) pf_sfPacket_WriteDouble;
		typedef void function(SFMLClass, cchar*) pf_sfPacket_WriteString;
		typedef void function(SFMLClass, cwchar*) pf_sfPacket_WriteWideString;
	
		static pf_sfPacket_Create sfPacket_Create;
		static pf_sfPacket_Destroy sfPacket_Destroy;
		static pf_sfPacket_Append sfPacket_Append;
		static pf_sfPacket_Clear sfPacket_Clear;
		static pf_sfPacket_GetData sfPacket_GetData;
		static pf_sfPacket_GetDataSize sfPacket_GetDataSize;
		static pf_sfPacket_EndOfPacket sfPacket_EndOfPacket;
		static pf_sfPacket_CanRead sfPacket_CanRead;
		static pf_sfPacket_ReadInt8 sfPacket_ReadInt8;
		static pf_sfPacket_ReadUint8 sfPacket_ReadUint8;
		static pf_sfPacket_ReadInt16 sfPacket_ReadInt16;
		static pf_sfPacket_ReadUint16 sfPacket_ReadUint16;
		static pf_sfPacket_ReadInt32 sfPacket_ReadInt32;
		static pf_sfPacket_ReadUint32 sfPacket_ReadUint32;
		static pf_sfPacket_ReadFloat sfPacket_ReadFloat;
		static pf_sfPacket_ReadDouble sfPacket_ReadDouble;
		static pf_sfPacket_ReadString sfPacket_ReadString;
		static pf_sfPacket_ReadWideString sfPacket_ReadWideString;
		static pf_sfPacket_WriteInt8 sfPacket_WriteInt8;
		static pf_sfPacket_WriteUint8 sfPacket_WriteUint8;
		static pf_sfPacket_WriteInt16 sfPacket_WriteInt16;
		static pf_sfPacket_WriteUint16 sfPacket_WriteUint16;
		static pf_sfPacket_WriteInt32 sfPacket_WriteInt32;
		static pf_sfPacket_WriteUint32 sfPacket_WriteUint32;
		static pf_sfPacket_WriteFloat sfPacket_WriteFloat;
		static pf_sfPacket_WriteDouble sfPacket_WriteDouble;
		static pf_sfPacket_WriteString sfPacket_WriteString;
		static pf_sfPacket_WriteWideString sfPacket_WriteWideString;
	}

	static this()
	{
	debug
		DllLoader dll = DllLoader.load("csfml-network-d-2");
	else
		DllLoader dll = DllLoader.load("csfml-network-2");
		
		sfPacket_Append = cast(pf_sfPacket_Append)dll.getSymbol("sfPacket_Append");
		sfPacket_CanRead = cast(pf_sfPacket_CanRead)dll.getSymbol("sfPacket_CanRead");
		sfPacket_Clear = cast(pf_sfPacket_Clear)dll.getSymbol("sfPacket_Clear");
		sfPacket_Create = cast(pf_sfPacket_Create)dll.getSymbol("sfPacket_Create");
		sfPacket_Destroy = cast(pf_sfPacket_Destroy)dll.getSymbol("sfPacket_Destroy");
		sfPacket_EndOfPacket = cast(pf_sfPacket_EndOfPacket)dll.getSymbol("sfPacket_EndOfPacket");
		sfPacket_GetData = cast(pf_sfPacket_GetData)dll.getSymbol("sfPacket_GetData");
		sfPacket_GetDataSize = cast(pf_sfPacket_GetDataSize)dll.getSymbol("sfPacket_GetDataSize");
		sfPacket_ReadDouble = cast(pf_sfPacket_ReadDouble)dll.getSymbol("sfPacket_ReadDouble");
		sfPacket_ReadFloat = cast(pf_sfPacket_ReadFloat)dll.getSymbol("sfPacket_ReadFloat");
		sfPacket_ReadInt16 = cast(pf_sfPacket_ReadInt16)dll.getSymbol("sfPacket_ReadInt16");
		sfPacket_ReadInt32 = cast(pf_sfPacket_ReadInt32)dll.getSymbol("sfPacket_ReadInt32");
		sfPacket_ReadInt8 = cast(pf_sfPacket_ReadInt8)dll.getSymbol("sfPacket_ReadInt8");
		sfPacket_ReadString = cast(pf_sfPacket_ReadString)dll.getSymbol("sfPacket_ReadString");
		sfPacket_ReadWideString = cast(pf_sfPacket_ReadWideString)dll.getSymbol("sfPacket_ReadWideString");
		sfPacket_ReadUint16 = cast(pf_sfPacket_ReadUint16)dll.getSymbol("sfPacket_ReadUint16");
		sfPacket_ReadUint32 = cast(pf_sfPacket_ReadUint32)dll.getSymbol("sfPacket_ReadUint32");
		sfPacket_ReadUint8 = cast(pf_sfPacket_ReadUint8)dll.getSymbol("sfPacket_ReadUint8");
		sfPacket_WriteDouble = cast(pf_sfPacket_WriteDouble)dll.getSymbol("sfPacket_WriteDouble");
		sfPacket_WriteFloat = cast(pf_sfPacket_WriteFloat)dll.getSymbol("sfPacket_WriteFloat");
		sfPacket_WriteInt16 = cast(pf_sfPacket_WriteInt16)dll.getSymbol("sfPacket_WriteInt16");
		sfPacket_WriteInt32 = cast(pf_sfPacket_WriteInt32)dll.getSymbol("sfPacket_WriteInt32");
		sfPacket_WriteInt8 = cast(pf_sfPacket_WriteInt8)dll.getSymbol("sfPacket_WriteInt8");
		sfPacket_WriteString = cast(pf_sfPacket_WriteString)dll.getSymbol("sfPacket_WriteString");
		sfPacket_WriteWideString = cast(pf_sfPacket_WriteWideString)dll.getSymbol("sfPacket_WriteWideString");
		sfPacket_WriteUint16 = cast(pf_sfPacket_WriteUint16)dll.getSymbol("sfPacket_WriteUint16");
		sfPacket_WriteUint32 = cast(pf_sfPacket_WriteUint32)dll.getSymbol("sfPacket_WriteUint32");
		sfPacket_WriteUint8 = cast(pf_sfPacket_WriteUint8)dll.getSymbol("sfPacket_WriteUint8");
	}
}
