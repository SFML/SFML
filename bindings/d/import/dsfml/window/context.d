/*
*	DSFML - SFML Library wrapper for the D programming language.
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

module dsfml.window.context;

import dsfml.system.common;

/**
 * 
 */
class Context : DSFMLObject
{
	/**
	 * 
	 */
	this()
	{
		super(sfContext_Create());
	}
	
	override void dispose()
	{
		sfContext_Destroy(m_ptr);
	}
	
	/**
	 * 
	 * Params:
	 *	 active =
	 */
	void setActive(bool active)
	{
		sfContext_SetActive(m_ptr, active);
	}

private:
	static extern(C)
	{
		SFMLClass	function()				sfContext_Create;
		void	function(SFMLClass)			sfContext_Destroy;
		void	function(SFMLClass, bool)	sfContext_SetActive;
	}

	mixin(loadFromSharedLib2("csfml-window", "sfContext",
	"Create", "Destroy", "SetActive"));
}