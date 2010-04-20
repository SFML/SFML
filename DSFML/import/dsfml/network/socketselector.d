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

module dsfml.network.socketselector;

import dsfml.network.tcpsocket;
import dsfml.network.udpsocket;

import dsfml.system.common;

/**
 *	Multiplexer that allows to read from multiple sockets
 */
class SocketSelector : DSFMLObject
{
	/**
	*	Default constructor
	*/		
	this()
	{
		super(sfSocketSelector_Create());
	}
	
	override void dispose()
	{
		sfSocketSelector_Destroy(m_ptr);
	}
	
	/**
	*	Add a socket to watch
	*	
	*	Params:
	*		socket = A tcp or udp socket			
	*/		
	void add(T socket)
	{
		if (!(socket.nativePointer in m_watchedSockets))
		{
			sfSocketSelector_Add(m_ptr, socket.nativePointer);
			m_watchedSockets[socket.nativePointer] = socket;
			m_numSocketsWatched++;
		}
	}
	
	/**
	*	Remove a previously added socket
	*	
	*	Params:
	*		socket = A tcp or udp socket				 
	*/		
	void remove(T socket)
	{
		if (socket.nativePointer in m_watchedSockets)
		{
			sfSocketSelector_Remove(m_ptr, socket.nativePointer);
			m_watchedSockets.remove(socket.nativePointer);
			m_numSocketsWatched--;
		}
	}
	
	/**
	*	Clear all sockets being watched
	*/		
	void clear()
	{
		sfSocketSelector_Clear(m_ptr);
		foreach(key; m_watchedSockets.keys)
			m_watchedSockets.remove(key);
		m_numSocketsWatched = 0;
	}
	
	/**
	*	Wait and collect sockets which are ready for reading.
	*	This functions will return either when at least one socket
	*	is ready, or when the given time is out
	*
	*	Params: 
	*		timeout = Maximum time to wait, in seconds (0 to disable timeout)
	*
	*	Returns: 
	*		Number of sockets ready
	*/
	uint wait(float timeout = 0.f)
	{
		return sfSocketSelector_Wait(m_ptr, timeout);
	}
	
	/**
	*	After a call to Wait(), get the Index-th socket which is
	*	ready for reading. The total number of sockets ready
	*	is the integer returned by the previous call to Wait()
	*
	*	Params:
	*		index = Index of the socket to get
	*
	*	Returns:
	*		The Index-th socket
	*/
	T GetSocketsReady(uint index)
	{
		return m_watchedSockets[sfSocketSelector_GetSocketReady(m_ptr, index)];
	}
	

private: 
//	size_t m_numSocketsWatched;
//	T[void*] m_watchedSockets;
	
// External ====================================================================
	static extern(C)
	{
		SFMLClass	function()						sfSocketSelector_Create;
		void		function(SFMLClass)				sfSocketSelector_Destroy;
		void		function(SFMLClass, SFMLClass)	sfSocketSelector_AddTcpListener;
		void		function(SFMLClass, SFMLClass)	sfSocketSelector_AddTcpSocket;
		void		function(SFMLClass, SFMLClass)	sfSocketSelector_AddUdpSocket;
		void		function(SFMLClass, SFMLClass)	sfSocketSelector_RemoveTcpListener;
		void		function(SFMLClass, SFMLClass)	sfSocketSelector_RemoveTcpSocket;
		void		function(SFMLClass, SFMLClass)	sfSocketSelector_RemoveUdpSocket;
		void		function(SFMLClass)				sfSocketSelector_Clear;
		bool		function(SFMLClass, float)		sfSocketSelector_Wait;
		bool		function(SFMLClass, SFMLClass)	sfSocketSelector_IsTcpListenerReady;
		bool		function(SFMLClass, SFMLClass)	sfSocketSelector_IsTcpSocketReady;
		bool		function(SFMLClass, SFMLClass)	sfSocketSelector_IsUdpSocketReady;
	}

	mixin(loadFromSharedLib2("csfml-network", "sfSocketSelector",
		"Create", "Destroy", "AddTcpListener", "AddTcpSocket", "AddUdpSocket", "RemoveTcpListener", "RemoveTcpSocket", "RemoveUdpSocket",
		"Clear", "Wait", "IsTcpListenerReady", "IsTcpSocketReady", "IsUdpSocketReady"));
}