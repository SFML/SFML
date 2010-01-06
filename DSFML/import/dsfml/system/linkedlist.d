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

module dsfml.system.linkedlist;

/*
*	Trivial implementation of Queue linked list (for internal use)
*/
class LinkedList(T)
{
	Node!(T) head;
	Node!(T) tail;
	private size_t m_count;
	
	void enqueue(T object)
	{
		if (empty)
			head = tail = new Node!(T)(object);
		else
		{
			tail.Next = new Node!(T)(object);
			tail = tail.Next;
		}
		m_count++;
	}
	
	T dequeue()
	{
		T o;
		if (empty)
			o = T.init;
		else
		{
			o = head.Data;
			head = head.Next;
			m_count--;
		}	
		return o;
	}
	
	bool empty()
	{
		return (head is null);
	}

	size_t getCount()
	{
		return m_count;
	}

	void clear()
	{
		T data;
		while ((data = dequeue()) !is T.init) {}
	}

	int opApply(int delegate(ref T) dg)
	{
		T data;
		int result;
		while ((data = dequeue) !is T.init)
		{
			if ((result = dg(data)) != 0)
				break;
		}
		return result;
	}
}

private class Node(T)
{
	Node Next;
	T Data;
	
	this(T data)
	{
		Data = data;	
	}
}

