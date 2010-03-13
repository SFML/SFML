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

module dsfml.window.videomode;

import dsfml.system.common;

/**
 *	VideoMode defines a video mode (width, height, bpp, frequency)
 *	and provides static functions for getting modes supported
 *	by the display device
 */
struct VideoMode
{
	uint Width;				/// Video mode width, in pixels
	uint Height;			/// Video mode height, in pixels
	uint BitsPerPixel = 32;	/// Video mode pixel depth, in bits per pixels


@property
{
	/**
	 *	Get the current desktop video mode
	 *
	 *	Returns:
	 *		Current desktop video mode
	 */
	static VideoMode getDesktopMode()
	{
		return sfVideoMode_GetDesktopMode();
	}

	/**
	 *	Get all the supported video modes for fullscreen mode.
	 *	Modes are sorted from best to worst.
	 *
	 *	Returns:
	 *		video mode array
	 */
	static VideoMode[] getFullscreenModes()
	{
		size_t arraySize;
		VideoMode* array = sfVideoMode_GetFullscreenModes(&arraySize); // TODO: check pointer?
		return array[0 .. arraySize];
	}

	/**
	 *	Tell whether or not the video mode is supported
	 *
	 *	Returns:
	 *		True if video mode is supported, false otherwise
	 */
	bool isValid()
	{
		return cast(bool)sfVideoMode_IsValid(this);
	}
}

	/**
	 *	Comparison operator overload -- tell if two video modes are equal
	 *
	 *	Params: 
	 *		Other : Video mode to compare
	 *
	 *	Returns:
	 *		True if modes are equal
	 */
	const bool opEquals(ref const(VideoMode) other)
	{
		return ((other.Width == Width) && (other.Height == Height) && (other.BitsPerPixel == BitsPerPixel));
	}
}

extern (C)
{
	VideoMode	function()			sfVideoMode_GetDesktopMode;
	VideoMode*	function(size_t*)	sfVideoMode_GetFullscreenModes;
	int			function(VideoMode)	sfVideoMode_IsValid;
}

mixin(loadFromSharedLib2("csfml-window", "sfVideoMode",
	"GetDesktopMode", "GetFullscreenModes", "IsValid"));