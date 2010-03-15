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
	byte[16] Address;

	/**
	*	Construct the address from a string
	*
	*	Params: 
	*		address = IP address ("xxx.xxx.xxx.xxx") or network name
	*
	*/
	static opCall(string address)
	{
		 return sfIpAddress_FromString(toStringz(address));
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
	static opCall(ubyte byte0, ubyte byte1, ubyte byte2, ubyte byte3)
	{
		return sfIpAddress_FromBytes(byte0, byte1, byte2, byte3);
	}

	/**
	*	Construct the address from a 32 bits integer
	*
	*	Params:
	*		address = 4 bytes of the address packed into a 32 bits integer
	*
	*/
	static opCall(uint address)
	{
		return sfIpAddress_FromInteger(address);
	}

	/**
	 *	Get the empty/invalid address
	 *
	 *	Returns:
	 *		Empty object that represents invalid addresses
	 */
	static IPAddress None()
	{
		return sfIpAddress_None();
	}

@property
{
	/**
	*	Get the computer's local IP address (from the LAN point of view)
	*
	*	Returns: 
	*		Local IP address
	*
	*/
	static IPAddress localAddress()
	{
		return sfIpAddress_GetLocalAddress();
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
	static IPAddress publicAddress()
	{
		return sfIpAddress_GetPublicAddress();
	}
	
	/**
	*	Local host address (to connect to the same computer). 
	*/	
	static IPAddress localHost()
	{
		return sfIpAddress_LocalHost();
	}
}

	const bool opEquals(ref const(IPAddress) other) 
	{
		return Address == other.Address;
	}
}

private:

static extern(C)
{
	IPAddress	function(cchar*)					sfIpAddress_FromString;
	IPAddress	function(ubyte, ubyte, ubyte, ubyte)sfIpAddress_FromBytes;
	IPAddress	function(uint)						sfIpAddress_FromInteger;
	IPAddress	function()							sfIpAddress_None;
	IPAddress	function()							sfIpAddress_GetLocalAddress;
	IPAddress	function()							sfIpAddress_GetPublicAddress;
	IPAddress	function()							sfIpAddress_LocalHost;
}

mixin(loadFromSharedLib2("csfml-network", "sfIpAddress",
	"FromBytes", "FromString", "FromInteger", "GetLocalAddress", "GetPublicAddress", "None", "LocalHost"));