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

module dsfml.network.tcpsocket;

import dsfml.network.ipaddress;
import dsfml.network.packet;
import dsfml.network.socketstatus;

import dsfml.system.common;

/**
*	TcpSocket wraps a socket using TCP protocol to send data safely (but a bit slower)
*/
class TcpSocket : DSFMLObject
{
	/**
	*	Default constructor
	*/
	this()
	{
		super(sfTcpSocket_Create());
		m_intermediatePacket = new Packet();
	}

	override void dispose()
	{
		sfTcpSocket_Destroy(m_ptr);
	}

	/**
	*	Connect to another computer on a specified port
	*
	*	Params: 
	*		port = Port to use for transfers (warning : ports < 1024 are reserved)
	*		hostAddress = IP Address of the host to connect to
	*		timeout = Maximum time to wait in seconds (0 by default : no timeout)	
	*
	*	Returns: 
	*		True if operation has been successful
	*/
	bool connect(ushort port, IPAddress hostAddress, float timeout = 0.f)
	{
		return cast(bool) !sfTcpSocket_Connect(m_ptr, port, hostAddress, timeout);
	}

	/**
	*	Listen to a specified port for incoming data or connections
	*
	*	Params: 
	*		port = Port to listen to
	*
	*	Returns: 
	*		True if operation has been successful
	*/
	bool listen(ushort port)
	{
		return cast(bool)sfTcpSocket_Listen(m_ptr, port);
	}

	/**
	*	Wait for a connection (must be listening to a port).
	*	This function is blocking.
	*
	*	Params: 
	*		connected = Socket containing the connection with the connected client
	*
	*	Returns: 
	*		Status code
	*/
	SocketStatus accept(TcpSocket connected)
	{
		SFMLClass temp = null;
		SocketStatus ret = sfTcpSocket_Accept(m_ptr, &temp, null);
		connected.m_ptr = temp;
		return ret;
	}
	
	/**
	*	Wait for a connection (must be listening to a port).
	*	This function is blocking.
	*
	*	Params: 
	*		connected = Socket containing the connection with the connected client
	*		address = Pointer to an address to fill with client infos
	*
	*	Returns: 
	*		Status code
	*/
	SocketStatus accept(TcpSocket connected, out IPAddress address)
	{
		SFMLClass temp = null;
		SocketStatus ret = sfTcpSocket_Accept(m_ptr, &temp, &address);
		connected.m_ptr = temp;
		return ret;
	}

	/**
	*	Send an array of bytes to the host (must be connected first)
	*
	*	Params: 
	*		data = array of bytes to send
	*
	*	Returns: 
	*		Status code
	*/
	SocketStatus send(byte[] data)
	in
	{
		assert(data && data.length);
	}
	body
	{
		return cast(SocketStatus)sfTcpSocket_Send(m_ptr, data.ptr, data.length);
	}

	/**
	*	Receive an array of bytes from the host (must be connected first).
	*	This function will block until a connection was accepted
	*
	*	Params: 
	*		data = array to fill (make sure it is big enough)
	*		sizeReceived = Number of bytes received
	*
	*	Returns: 
	*		Status code
	*		
	*	Remarks:
	*		Assert if data is null or length == 0			
	*
	*/
	SocketStatus receive(byte[] data, out size_t sizeReceived)
	in
	{
		assert(data && data.length);
	}
	body
	{
		return cast(SocketStatus)sfTcpSocket_Receive(m_ptr, data.ptr, data.length, &sizeReceived);
	}


	/**
	*	Send a packet of data to the host (must be connected first)
	*
	*	Params: 
	*		packetToSend = Packet to send
	*
	*	Returns: 
	*		Status code
	*
	*/
	SocketStatus send(Packet packetToSend)
	{
		byte[] dataArray = packetToSend.onSend();
		m_intermediatePacket.append(dataArray);
		SocketStatus stat = cast(SocketStatus)sfTcpSocket_SendPacket(m_ptr, m_intermediatePacket.nativePointer);
		m_intermediatePacket.clear();
		return stat;
	}

	/**
	*	Receive a packet from the host (must be connected first).
	*	This function will block if the socket is blocking
	*
	*	Params: 
	*		packetToReceive = Packet to fill with received data
	*
	*	Returns: 
	*		Status code
	*
	*/
	SocketStatus receive(Packet packetToReceive)
	{
		SocketStatus stat = cast(SocketStatus)sfTcpSocket_ReceivePacket(m_ptr, m_intermediatePacket.nativePointer);
		packetToReceive.onReceive(m_intermediatePacket.getData);
		m_intermediatePacket.clear();
		return stat;
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
		return cast(bool)sfTcpSocket_IsValid(m_ptr);
	}

package:

	this (SFMLClass ptr)
	{
		super(ptr);
		m_intermediatePacket = new Packet();
	}


private:
	Packet m_intermediatePacket;

// External ====================================================================
	extern (C)
	{
		typedef SFMLClass function() pf_sfTcpSocket_Create;
		typedef void function(SFMLClass) pf_sfTcpSocket_Destroy;
		typedef int function(SFMLClass, ushort, IPAddress, float) pf_sfTcpSocket_Connect;
		typedef int function(SFMLClass, ushort) pf_sfTcpSocket_Listen;
		typedef SocketStatus function(SFMLClass, SFMLClass*, IPAddress*) pf_sfTcpSocket_Accept;
		typedef SocketStatus function(SFMLClass, const(byte)*, size_t) pf_sfTcpSocket_Send;
		typedef SocketStatus function(SFMLClass, byte*, size_t, size_t*) pf_sfTcpSocket_Receive;
		typedef SocketStatus function(SFMLClass, SFMLClass) pf_sfTcpSocket_SendPacket;
		typedef SocketStatus function(SFMLClass, SFMLClass) pf_sfTcpSocket_ReceivePacket;
		typedef int function(SFMLClass) pf_sfTcpSocket_IsValid;
	
		static pf_sfTcpSocket_Create sfTcpSocket_Create;
		static pf_sfTcpSocket_Destroy sfTcpSocket_Destroy;
		static pf_sfTcpSocket_Connect sfTcpSocket_Connect;
		static pf_sfTcpSocket_Listen sfTcpSocket_Listen;
		static pf_sfTcpSocket_Accept sfTcpSocket_Accept;
		static pf_sfTcpSocket_Send sfTcpSocket_Send;
		static pf_sfTcpSocket_Receive sfTcpSocket_Receive;
		static pf_sfTcpSocket_SendPacket sfTcpSocket_SendPacket;
		static pf_sfTcpSocket_ReceivePacket sfTcpSocket_ReceivePacket;
		static pf_sfTcpSocket_IsValid sfTcpSocket_IsValid;
	}

	static this()
	{
	debug
		DllLoader dll = DllLoader.load("csfml-network-d-2");
	else
		DllLoader dll = DllLoader.load("csfml-network-2");
		
		sfTcpSocket_Accept = cast(pf_sfTcpSocket_Accept)dll.getSymbol("sfTcpSocket_Accept");
		sfTcpSocket_Connect = cast(pf_sfTcpSocket_Connect)dll.getSymbol("sfTcpSocket_Connect");
		sfTcpSocket_Create = cast(pf_sfTcpSocket_Create)dll.getSymbol("sfTcpSocket_Create");
		sfTcpSocket_Destroy = cast(pf_sfTcpSocket_Destroy)dll.getSymbol("sfTcpSocket_Destroy");
		sfTcpSocket_IsValid = cast(pf_sfTcpSocket_IsValid)dll.getSymbol("sfTcpSocket_IsValid");
		sfTcpSocket_Listen = cast(pf_sfTcpSocket_Listen)dll.getSymbol("sfTcpSocket_Listen");
		sfTcpSocket_Receive = cast(pf_sfTcpSocket_Receive)dll.getSymbol("sfTcpSocket_Receive");
		sfTcpSocket_ReceivePacket = cast(pf_sfTcpSocket_ReceivePacket)dll.getSymbol("sfTcpSocket_ReceivePacket");
		sfTcpSocket_Send = cast(pf_sfTcpSocket_Send)dll.getSymbol("sfTcpSocket_Send");
		sfTcpSocket_SendPacket = cast(pf_sfTcpSocket_SendPacket)dll.getSymbol("sfTcpSocket_SendPacket");
	}
}
