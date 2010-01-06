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

module dsfml.network.sockettcp;

import dsfml.network.ipaddress;
import dsfml.network.packet;
import dsfml.network.socketstatus;

import dsfml.system.common;

/**
*	SocketTCP wraps a socket using TCP protocol to send data safely (but a bit slower)
*/
class SocketTCP : DSFMLObject
{
	/**
	*	Default constructor
	*/
	this()
	{
		super(sfSocketTCP_Create());
		m_intermediatePacket = new Packet();
	}

	override void dispose()
	{
		sfSocketTCP_Destroy(m_ptr);
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
		return cast(bool) !sfSocketTCP_Connect(m_ptr, port, hostAddress, timeout);
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
		return cast(bool)sfSocketTCP_Listen(m_ptr, port);
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
	SocketStatus accept(SocketTCP connected)
	{
		void* temp = null;
		SocketStatus ret = sfSocketTCP_Accept(m_ptr, &temp, null);
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
	SocketStatus accept(SocketTCP connected, out IPAddress address)
	{
		void* temp = null;
		SocketStatus ret = sfSocketTCP_Accept(m_ptr, &temp, &address);
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
	{
		if (data && data.length > 0)
			return cast(SocketStatus)sfSocketTCP_Send(m_ptr, data.ptr, data.length);
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
	{
		if (data && data.length > 0)
			return cast(SocketStatus)sfSocketTCP_Receive(m_ptr, data.ptr, data.length, &sizeReceived);
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
		SocketStatus stat = cast(SocketStatus)sfSocketTCP_SendPacket(m_ptr, m_intermediatePacket.getNativePointer);
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
		SocketStatus stat = cast(SocketStatus)sfSocketTCP_ReceivePacket(m_ptr, m_intermediatePacket.getNativePointer);
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
		return cast(bool)sfSocketTCP_IsValid(m_ptr);
	}

	///		
	bool opEquals(SocketTCP other) 
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

// External ====================================================================
	extern (C)
	{
		typedef void* function() pf_sfSocketTCP_Create;
		typedef void function(void*) pf_sfSocketTCP_Destroy;
		typedef int function(void*, ushort, IPAddress, float) pf_sfSocketTCP_Connect;
		typedef int function(void*, ushort) pf_sfSocketTCP_Listen;
		typedef SocketStatus function(void*, void**, IPAddress*) pf_sfSocketTCP_Accept;
		typedef SocketStatus function(void*, byte*, size_t) pf_sfSocketTCP_Send;
		typedef SocketStatus function(void*, byte*, size_t, size_t*) pf_sfSocketTCP_Receive;
		typedef SocketStatus function(void*, void*) pf_sfSocketTCP_SendPacket;
		typedef SocketStatus function(void*, void*) pf_sfSocketTCP_ReceivePacket;
		typedef int function(void*) pf_sfSocketTCP_IsValid;
	
		static pf_sfSocketTCP_Create sfSocketTCP_Create;
		static pf_sfSocketTCP_Destroy sfSocketTCP_Destroy;
		static pf_sfSocketTCP_Connect sfSocketTCP_Connect;
		static pf_sfSocketTCP_Listen sfSocketTCP_Listen;
		static pf_sfSocketTCP_Accept sfSocketTCP_Accept;
		static pf_sfSocketTCP_Send sfSocketTCP_Send;
		static pf_sfSocketTCP_Receive sfSocketTCP_Receive;
		static pf_sfSocketTCP_SendPacket sfSocketTCP_SendPacket;
		static pf_sfSocketTCP_ReceivePacket sfSocketTCP_ReceivePacket;
		static pf_sfSocketTCP_IsValid sfSocketTCP_IsValid;
	}

	static this()
	{
	debug
		DllLoader dll = DllLoader.load("csfml-network-d");
	else
		DllLoader dll = DllLoader.load("csfml-network");
		
		sfSocketTCP_Accept = cast(pf_sfSocketTCP_Accept)dll.getSymbol("sfSocketTCP_Accept");
		sfSocketTCP_Connect = cast(pf_sfSocketTCP_Connect)dll.getSymbol("sfSocketTCP_Connect");
		sfSocketTCP_Create = cast(pf_sfSocketTCP_Create)dll.getSymbol("sfSocketTCP_Create");
		sfSocketTCP_Destroy = cast(pf_sfSocketTCP_Destroy)dll.getSymbol("sfSocketTCP_Destroy");
		sfSocketTCP_IsValid = cast(pf_sfSocketTCP_IsValid)dll.getSymbol("sfSocketTCP_IsValid");
		sfSocketTCP_Listen = cast(pf_sfSocketTCP_Listen)dll.getSymbol("sfSocketTCP_Listen");
		sfSocketTCP_Receive = cast(pf_sfSocketTCP_Receive)dll.getSymbol("sfSocketTCP_Receive");
		sfSocketTCP_ReceivePacket = cast(pf_sfSocketTCP_ReceivePacket)dll.getSymbol("sfSocketTCP_ReceivePacket");
		sfSocketTCP_Send = cast(pf_sfSocketTCP_Send)dll.getSymbol("sfSocketTCP_Send");
		sfSocketTCP_SendPacket = cast(pf_sfSocketTCP_SendPacket)dll.getSymbol("sfSocketTCP_SendPacket");
	}
}
