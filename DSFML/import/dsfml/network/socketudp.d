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

module dsfml.network.socketudp;

import dsfml.network.ipaddress;
import dsfml.network.packet;
import dsfml.network.socketstatus;

import dsfml.system.common;

/**
*	SocketUDP wraps a socket using UDP protocol to
*	send data fastly (but with less safety)
*/
class SocketUDP : DSFMLObject
{
	/**
	*	Default constructor
	*/
	this()
	{
		super(sfSocketUDP_Create());
		m_intermediatePacket = new Packet();
	}

	override void dispose()
	{
		sfSocketUDP_Destroy(m_ptr);
	}

	/**
	*	Bind the socket to a specific port
	*
	*	Params: 
	*		port = Port to bind the socket to
	*
	*	Returns: 
	*		True if operation has been successful
	*
	*/
	bool bind(ushort port)
	{
		m_port = port;
		return cast(bool)sfSocketUDP_Bind(m_ptr, port);
	}

	/**
	*	Unbind the socket from its previous port, if any
	*
	*	Returns: True if operation has been successful
	*
	*/
	bool unbind()
	{
		m_port = 0;
		return cast(bool)sfSocketUDP_Unbind(m_ptr, m_port);
	}

	/**
	*	Send an array of bytes
	*
	*	Params: 
	*		data = bytes array to send
	*		address = Address of the computer to send the packet to
	*		port = Port to send the data to
	*
	*	Returns: 
	*		Status code
	*
	*/
	SocketStatus send(byte[] data, IPAddress address, ushort port)
	{
		return cast(SocketStatus) sfSocketUDP_Send(m_ptr, data.ptr, data.length, address, port);
	}

	/**
	*	Receive an array of bytes.
	*	This function is blocking.
	*
	*	Params: 
	*		data = Pointer to a byte array to fill (make sure it is big enough)
	*		sizeReceived = Number of bytes received
	*		address = Address of the computer which sent the data
	*
	*	Returns: 
	*		Status code
	*
	*	Remarks:
	*		Assert if data is null or length == 0			
	*
	*/
	SocketStatus receive(byte[] data, out size_t sizeReceived, out IPAddress address)
	{
		SocketStatus ret = sfSocketUDP_Receive(m_ptr, data.ptr, data.length, &sizeReceived, &address);
		return ret;
	}

	/**
	*	Send a packet of data
	*
	*	Params: 
	*		packetToSend = Packet to send
	*		address = Address of the computer to send the packet to
	*		port = Port to send the data to
	*
	*	Returns: 
	*		Status code
	*
	*/
	SocketStatus send(Packet packetToSend, IPAddress address, ushort port)
	{
			byte[] dataArray = packetToSend.onSend();
			m_intermediatePacket.append(dataArray);
		SocketStatus stat = cast(SocketStatus)sfSocketUDP_SendPacket(m_ptr, m_intermediatePacket.getNativePointer, address, port);
		m_intermediatePacket.clear();
		return stat;
	}

	/**
	*	Receive a packet.
	*	This function is blocking.
	*
	*	Params: 
	*		packetToReceive = Packet to fill with received data
	*		address = Address of the computer which sent the packet
	*
	*	Returns: 
	*		Status code
	*
	*/
	SocketStatus receive(Packet packetToReceive, out IPAddress address)
	{
		SocketStatus ret = sfSocketUDP_ReceivePacket(m_ptr, m_intermediatePacket.getNativePointer, &address);
		packetToReceive.onReceive(m_intermediatePacket.getData);
		m_intermediatePacket.clear();
		return ret;

	}

	/**
	*	Check if the socket is in a valid state ; this function
	*	can be called any time to check if the socket is OK
	*
	*	Returns: 
	*		True if the socket is valid
	*
	*/
	bool isValid()
	{
		return cast(bool)sfSocketUDP_IsValid(m_ptr);
	}

	/**
	*	Get the port the socket is currently bound to
	*
	*	Returns: 
	*		Current port (0 means the socket is not bound)
	*/
	ushort getPort()
	{
		return m_port;
	}

	///
	bool opEquals(SocketUDP other)
	{
		return (other.getNativePointer == this.getNativePointer);
	}

package:
	this (void* ptr)
	{
		super(ptr);
		m_intermediatePacket = new Packet();
	}
	
private:
	Packet m_intermediatePacket;
	ushort m_port;
	
// External ====================================================================

	extern (C)
	{
		typedef void* function() pf_sfSocketUDP_Create;
		typedef void function(void*) pf_sfSocketUDP_Destroy;
		typedef int function(void*, ushort) pf_sfSocketUDP_Bind;
		typedef int function(void*, ushort) pf_sfSocketUDP_Unbind;
		typedef SocketStatus function(void*, byte*, size_t, IPAddress, ushort) pf_sfSocketUDP_Send;
		typedef SocketStatus function(void*, byte*, size_t, size_t*, IPAddress*) pf_sfSocketUDP_Receive;
		typedef SocketStatus function(void*, void*, IPAddress, ushort) pf_sfSocketUDP_SendPacket;
		typedef SocketStatus function(void*, void*, IPAddress*) pf_sfSocketUDP_ReceivePacket;
		typedef int function(void*) pf_sfSocketUDP_IsValid;
	
		static pf_sfSocketUDP_Create sfSocketUDP_Create;
		static pf_sfSocketUDP_Destroy sfSocketUDP_Destroy;
		static pf_sfSocketUDP_Bind sfSocketUDP_Bind;
		static pf_sfSocketUDP_Unbind sfSocketUDP_Unbind;
		static pf_sfSocketUDP_Send sfSocketUDP_Send;
		static pf_sfSocketUDP_Receive sfSocketUDP_Receive;
		static pf_sfSocketUDP_SendPacket sfSocketUDP_SendPacket;
		static pf_sfSocketUDP_ReceivePacket sfSocketUDP_ReceivePacket;
		static pf_sfSocketUDP_IsValid sfSocketUDP_IsValid;
	}

	static this()
	{
	debug
		DllLoader dll = DllLoader.load("csfml-network-d");
	else
		DllLoader dll = DllLoader.load("csfml-network");
		
		sfSocketUDP_Bind = cast(pf_sfSocketUDP_Bind)dll.getSymbol("sfSocketUDP_Bind");
		sfSocketUDP_Create = cast(pf_sfSocketUDP_Create)dll.getSymbol("sfSocketUDP_Create");
		sfSocketUDP_Destroy = cast(pf_sfSocketUDP_Destroy)dll.getSymbol("sfSocketUDP_Destroy");
		sfSocketUDP_IsValid = cast(pf_sfSocketUDP_IsValid)dll.getSymbol("sfSocketUDP_IsValid");
		sfSocketUDP_Receive = cast(pf_sfSocketUDP_Receive)dll.getSymbol("sfSocketUDP_Receive");
		sfSocketUDP_ReceivePacket = cast(pf_sfSocketUDP_ReceivePacket)dll.getSymbol("sfSocketUDP_ReceivePacket");
		sfSocketUDP_Send = cast(pf_sfSocketUDP_Send)dll.getSymbol("sfSocketUDP_Send");
		sfSocketUDP_SendPacket = cast(pf_sfSocketUDP_SendPacket)dll.getSymbol("sfSocketUDP_SendPacket");
		sfSocketUDP_Unbind = cast(pf_sfSocketUDP_Unbind)dll.getSymbol("sfSocketUDP_Unbind");
	}
}
