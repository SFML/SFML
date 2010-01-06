/*
*   DSFML - SFML Library wrapper for the D programming language.
*   Copyright (C) 2008 Julien Dagorn (sirjulio13@gmail.com)
*   Copyright (C) 2010 Andreas Hollandt
*
*   This software is provided 'as-is', without any express or
*   implied warranty. In no event will the authors be held
*   liable for any damages arising from the use of this software.
*
*   Permission is granted to anyone to use this software for any purpose,
*   including commercial applications, and to alter it and redistribute
*   it freely, subject to the following restrictions:
*
*   1.  The origin of this software must not be misrepresented;
*       you must not claim that you wrote the original software.
*       If you use this software in a product, an acknowledgment
*       in the product documentation would be appreciated but
*       is not required.
*
*   2.  Altered source versions must be plainly marked as such,
*       and must not be misrepresented as being the original software.
*
*   3.  This notice may not be removed or altered from any
*       source distribution.
*/

module dsfml.window.common;

import dsfml.system.dllloader;

package extern(C)
{

void*	function()				sfContext_Create;
void	function(void*)			sfContext_Destroy;
void	function(void*, bool)	sfContext_SetActive;

}

static this()
{
debug
	DllLoader dll = DllLoader.load("csfml-window-d");
else
	DllLoader dll = DllLoader.load("csfml-window");

	mixin(loadFromSharedLib("sfContext_Create"));
	mixin(loadFromSharedLib("sfContext_Destroy"));
	mixin(loadFromSharedLib("sfContext_SetActive"));
}