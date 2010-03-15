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

module dsfml.network.selector;

import dsfml.network.sockettcp;
import dsfml.network.socketudp;

import dsfml.system.common;

/**
 *	Selector TCP allow reading from multiple sockets
 *	without blocking. It's a kind of multiplexer. Use SocketTCP or 
 *	SocketUDP aliases.
 */
class Selector(T) : DSFMLObject
{
	//Ensure type is correct
	static if (!is(T : SocketTCP) && !is(T : SocketUDP))
		static assert("Only SocketTCP and SocketUDP are valid for Selector.");
	
	
	/**
	*	Default constructor
	*/		
	this()
	{
		super(sfSelector_Create());
	}
	
	override void dispose()
	{
		sfSelector_Destroy(m_ptr);
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
			sfSelector_Add(m_ptr, socket.nativePointer);
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
			sfSelector_Remove(m_ptr, socket.nativePointer);
			m_watchedSockets.remove(socket.nativePointer);
			m_numSocketsWatched--;
		}
	}
	
	/**
	*	Clear all sockets being watched
	*/		
	void clear()
	{
		sfSelector_Clear(m_ptr);
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
		return sfSelector_Wait(m_ptr, timeout);
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
		return m_watchedSockets[sfSelector_GetSocketReady(m_ptr, index)];
	}
	

private: 
	size_t m_numSocketsWatched;
	T[void*] m_watchedSockets;
	
// External ====================================================================
	extern (C)
	{
		typedef SFMLClass function() pf_sfSelector_Create;
		typedef void function(SFMLClass) pf_sfSelector_Destroy;
		typedef void function(SFMLClass, SFMLClass) pf_sfSelector_Add;
		typedef void function(SFMLClass, SFMLClass) pf_sfSelector_Remove;
		typedef void function(SFMLClass) pf_sfSelector_Clear;
		typedef uint function(SFMLClass, float) pf_sfSelector_Wait;
		typedef SFMLClass function(SFMLClass, uint) pf_sfSelector_GetSocketReady;

		static pf_sfSelector_Create sfSelector_Create;
		static pf_sfSelector_Destroy sfSelector_Destroy;
		static pf_sfSelector_Add sfSelector_Add;
		static pf_sfSelector_Remove sfSelector_Remove;
		static pf_sfSelector_Clear sfSelector_Clear;
		static pf_sfSelector_Wait sfSelector_Wait;
		static pf_sfSelector_GetSocketReady sfSelector_GetSocketReady;
	}

	static this()
	{
	debug
		DllLoader dll = DllLoader.load("csfml-network-d");
	else
		DllLoader dll = DllLoader.load("csfml-network");
		
		static if (is (T : SocketTCP))
		{
			string symbol = "sfSelectorTCP";
		}
		else static if (is (T : SocketUDP))
		{
			string symbol = "sfSelectorUDP";
		}
		
		sfSelector_Add = cast(pf_sfSelector_Add)dll.getSymbol(symbol ~ "_Add");
		sfSelector_Clear = cast(pf_sfSelector_Clear)dll.getSymbol(symbol ~ "_Clear");
		sfSelector_Create = cast(pf_sfSelector_Create)dll.getSymbol(symbol ~ "_Create");
		sfSelector_Destroy = cast(pf_sfSelector_Destroy)dll.getSymbol(symbol ~ "_Destroy");
		sfSelector_GetSocketReady = cast(pf_sfSelector_GetSocketReady)dll.getSymbol(symbol ~ "_GetSocketReady");
		sfSelector_Wait = cast(pf_sfSelector_Wait)dll.getSymbol(symbol ~ "_Wait");
		sfSelector_Remove = cast(pf_sfSelector_Remove)dll.getSymbol(symbol ~ "_Remove");
	}
}

/**
*	alias of selector for TCP or UDP Socket. 
*/
alias Selector!(SocketTCP) SelectorTCP;
/// ditto
alias Selector!(SocketUDP) SelectorUDP;