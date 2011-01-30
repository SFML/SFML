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

module dsfml.network.udpsocket;

import dsfml.network.ipaddress;
import dsfml.network.packet;
import dsfml.network.socketstatus;

import dsfml.system.common;

/**
 *	UdpSocket wraps a socket using UDP protocol to
 *	send data fastly (but with less safety)
 */
class UdpSocket : DSFMLObject
{
	/**
	 *	Default constructor
	 */
	this()
	{
		super(sfUdpSocket_Create());
		m_intermediatePacket = new Packet();
	}

	override void dispose()
	{
		sfUdpSocket_Destroy(m_ptr);
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
		return cast(bool)sfUdpSocket_Bind(m_ptr, port);
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
		return cast(bool)sfUdpSocket_Unbind(m_ptr, m_port);
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
		return cast(SocketStatus) sfUdpSocket_Send(m_ptr, data.ptr, data.length, address, port);
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
		SocketStatus ret = sfUdpSocket_Receive(m_ptr, data.ptr, data.length, &sizeReceived, &address);
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
		SocketStatus stat = cast(SocketStatus)sfUdpSocket_SendPacket(m_ptr, m_intermediatePacket.nativePointer, address, port);
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
		SocketStatus ret = sfUdpSocket_ReceivePacket(m_ptr, m_intermediatePacket.nativePointer, &address);
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
		return cast(bool)sfUdpSocket_IsValid(m_ptr);
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

package:
	this (SFMLClass ptr)
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
		typedef SFMLClass function() pf_sfUdpSocket_Create;
		typedef void function(SFMLClass) pf_sfUdpSocket_Destroy;
		typedef int function(SFMLClass, ushort) pf_sfUdpSocket_Bind;
		typedef int function(SFMLClass, ushort) pf_sfUdpSocket_Unbind;
		typedef SocketStatus function(SFMLClass, byte*, size_t, IPAddress, ushort) pf_sfUdpSocket_Send;
		typedef SocketStatus function(SFMLClass, byte*, size_t, size_t*, IPAddress*) pf_sfUdpSocket_Receive;
		typedef SocketStatus function(SFMLClass, SFMLClass, IPAddress, ushort) pf_sfUdpSocket_SendPacket;
		typedef SocketStatus function(SFMLClass, SFMLClass, IPAddress*) pf_sfUdpSocket_ReceivePacket;
		typedef int function(SFMLClass) pf_sfUdpSocket_IsValid;
	
		static pf_sfUdpSocket_Create sfUdpSocket_Create;
		static pf_sfUdpSocket_Destroy sfUdpSocket_Destroy;
		static pf_sfUdpSocket_Bind sfUdpSocket_Bind;
		static pf_sfUdpSocket_Unbind sfUdpSocket_Unbind;
		static pf_sfUdpSocket_Send sfUdpSocket_Send;
		static pf_sfUdpSocket_Receive sfUdpSocket_Receive;
		static pf_sfUdpSocket_SendPacket sfUdpSocket_SendPacket;
		static pf_sfUdpSocket_ReceivePacket sfUdpSocket_ReceivePacket;
		static pf_sfUdpSocket_IsValid sfUdpSocket_IsValid;
	}

	static this()
	{
	debug
		DllLoader dll = DllLoader.load("csfml-network-d-2");
	else
		DllLoader dll = DllLoader.load("csfml-network-2");
		
		sfUdpSocket_Bind = cast(pf_sfUdpSocket_Bind)dll.getSymbol("sfUdpSocket_Bind");
		sfUdpSocket_Create = cast(pf_sfUdpSocket_Create)dll.getSymbol("sfUdpSocket_Create");
		sfUdpSocket_Destroy = cast(pf_sfUdpSocket_Destroy)dll.getSymbol("sfUdpSocket_Destroy");
		sfUdpSocket_IsValid = cast(pf_sfUdpSocket_IsValid)dll.getSymbol("sfUdpSocket_IsValid");
		sfUdpSocket_Receive = cast(pf_sfUdpSocket_Receive)dll.getSymbol("sfUdpSocket_Receive");
		sfUdpSocket_ReceivePacket = cast(pf_sfUdpSocket_ReceivePacket)dll.getSymbol("sfUdpSocket_ReceivePacket");
		sfUdpSocket_Send = cast(pf_sfUdpSocket_Send)dll.getSymbol("sfUdpSocket_Send");
		sfUdpSocket_SendPacket = cast(pf_sfUdpSocket_SendPacket)dll.getSymbol("sfUdpSocket_SendPacket");
		sfUdpSocket_Unbind = cast(pf_sfUdpSocket_Unbind)dll.getSymbol("sfUdpSocket_Unbind");
	}
}
