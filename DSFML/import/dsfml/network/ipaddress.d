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

module dsfml.network.ipaddress;

import dsfml.system.common;
import dsfml.system.stringutil;

/**
*	IPAddress provides easy manipulation of IP v4 addresses
*/
struct IPAddress
{
	/**
	*	Construct the address from a string
	*
	*	Params: 
	*		address = IP address ("xxx.xxx.xxx.xxx") or network name
	*
	*/
	static IPAddress opCall(string address)
	{
		return sfIPAddress_FromString(toStringz(address));
	}


	/**
	*	Construct the address from 4 bytes
	*
	*	Params:
	*		byte0 = First byte of the address
	*		byte1 = Second byte of the address
	*		byte2 = Third byte of the address
	*		byte3 = Fourth byte of the address
	*
	*/
	static IPAddress opCall(ubyte byte0, ubyte byte1, ubyte byte2, ubyte byte3)
	{
		return sfIPAddress_FromBytes(byte0, byte1, byte2, byte3);
	}

	/**
	*	Construct the address from a 32 bits integer
	*
	*	Params:
	*		address = 4 bytes of the address packed into a 32 bits integer
	*
	*/
	static IPAddress opCall(uint address)
	{
		return sfIPAddress_FromInteger(address);
	}

	/**
	*	Tell if the address is a valid one
	*
	*	Returns: 
	*		True if address has a valid syntax
	*
	*/
	bool isValid()
	{
		return cast(bool)sfIPAddress_IsValid(this);
	}
	
	/**
	*	Get the computer's local IP address (from the LAN point of view)
	*
	*	Returns: 
	*		Local IP address
	*
	*/
	static IPAddress getLocalAddress()
	{
		return sfIPAddress_GetLocalAddress();
	}

	/**
	*	Get the computer's public IP address (from the web point of view).
	*	The only way to get a public address is to ask it to a
	*	distant website ; as a consequence, this function may be
	*	very slow -- use it as few as possible !
	*
	*	Returns: 
	*		 Public IP address
	*
	*/
	static IPAddress getPublicAddress()
	{
		return sfIPAddress_GetPublicAddress();
	}

	bool opEqual(IPAddress other) 
	{
		return Address == other.Address;
	}

	/**
	*	Local host address (to connect to the same computer). 
	*/	
	static IPAddress LOCALHOST()
	{
		return sfIPAddress_LocalHost();
	}
 
	byte[16] Address;
	
}

private:

extern (C)
{
	typedef IPAddress function(char*) pf_sfIPAddress_FromString;
	typedef IPAddress function(ubyte, ubyte, ubyte, ubyte) pf_sfIPAddress_FromBytes;
	typedef IPAddress function(uint) pf_sfIPAddress_FromInteger;
	typedef int function(IPAddress) pf_sfIPAddress_IsValid;
	typedef IPAddress function() pf_sfIPAddress_GetLocalAddress;
	typedef IPAddress function() pf_sfIPAddress_GetPublicAddress;
	typedef IPAddress function() pf_sfIPAddress_LocalHost;

	static pf_sfIPAddress_FromString sfIPAddress_FromString;
	static pf_sfIPAddress_FromBytes sfIPAddress_FromBytes;
	static pf_sfIPAddress_FromInteger sfIPAddress_FromInteger;
	static pf_sfIPAddress_IsValid sfIPAddress_IsValid;
	static pf_sfIPAddress_GetLocalAddress sfIPAddress_GetLocalAddress;
	static pf_sfIPAddress_GetPublicAddress sfIPAddress_GetPublicAddress;
	static pf_sfIPAddress_LocalHost sfIPAddress_LocalHost;
}

static this()
{
	debug
		DllLoader dll = DllLoader.load("csfml-network-d");
	else
		DllLoader dll = DllLoader.load("csfml-network");
	
	sfIPAddress_FromBytes = cast(pf_sfIPAddress_FromBytes)dll.getSymbol("sfIPAddress_FromBytes");
	sfIPAddress_FromString = cast(pf_sfIPAddress_FromString)dll.getSymbol("sfIPAddress_FromString");
	sfIPAddress_FromInteger = cast(pf_sfIPAddress_FromInteger)dll.getSymbol("sfIPAddress_FromInteger");
	sfIPAddress_GetLocalAddress = cast(pf_sfIPAddress_GetLocalAddress)dll.getSymbol("sfIPAddress_GetLocalAddress");
	sfIPAddress_GetPublicAddress = cast(pf_sfIPAddress_GetPublicAddress)dll.getSymbol("sfIPAddress_GetPublicAddress");
	sfIPAddress_IsValid = cast(pf_sfIPAddress_IsValid)dll.getSymbol("sfIPAddress_IsValid");
	sfIPAddress_LocalHost = cast(pf_sfIPAddress_LocalHost)dll.getSymbol("sfIPAddress_LocalHost");
}
