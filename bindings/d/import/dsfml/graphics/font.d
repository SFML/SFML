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

module dsfml.graphics.font;

import	dsfml.system.common,
		dsfml.system.exception,
		dsfml.system.stringutil;

import	dsfml.graphics.rect,
		dsfml.graphics.image;


/// Glyph describes a glyph (a visual character)
struct Glyph
{
	int		Advance;	/// Offset to move horizontically to the next character
	IntRect	Bounds;		/// Bounding rectangle of the glyph, in coordinates relative to the baseline
	IntRect	SubRect;	/// Texture coordinates of the glyph inside the font's image
}

/**
*	Font is the low-level class for loading and
*	manipulating character fonts.
*/
class Font : DSFMLObject
{
private:
	static Font s_default;
public:

	/**
	*	Get SFML default built-in font (Arial)
	*/		
	static Font getDefaultFont()
	{
		if (s_default is null)
			s_default = new Font(sfFont_GetDefaultFont());			
		return s_default;
	}

	/**
	*	construct the Font from a file
	*	
	*	Params:
	*		filename = font file to load
	*/		
	this(string filename)
	{
		if (filename is null || filename.length == 0)
			throw new LoadingException("LoadingException : Filename is invalid.");

		super(sfFont_CreateFromFile(toStringz(filename)));
	}
	
	/**
	*	construct the Font from a file in memory
	*	
	*	Params:
	*		data = data to load	
	*/			
	this(ubyte[] data)
	{
		if (data is null || data.length == 0)
			throw new Exception("LoadingException : Memory stream is invalid.");

		super(sfFont_CreateFromMemory(data.ptr, data.length));
	}

	
	override void dispose()
	{
		sfFont_Destroy(m_ptr);
	}

	/**
	 *	get a glyph in a font
	 *
	 *	Params:
	 *	    codePoint = Unicode code point of the character to get
	 *	    charSize = Reference character size
	 *	    bold = Retrieve the bold version or the regular one?
	 *	Returns:
	 *		The glyph corresponding to codePoint and charSize
	 */
	Glyph getGlyph(uint codePoint, uint charSize, bool bold)
	{
		return sfFont_GetGlyph(m_ptr, codePoint, charSize, bold);
	}
	
	/**
	 *	Get the kerning offset of two glyphs
	 *
	 *	Params:
	 *	    first = Unicode code point of the first character
	 *	    second = Unicode code point of the second character
	 *	    charSize = Reference character size
	 *
	 *	Returns:
	 *		Kerning value for first and second, in pixels
	 */
	int getKerning(uint first, uint second, uint charSize)
	{
		return sfFont_GetKerning(m_ptr, first, second, charSize);
	}
	
	/**
	 *	Get the vertical offset to apply between two consecutive lines of text.
	 *
	 *	Params:
	 *	    charSize = Reference character size
	 *
	 *	Returns:
	 *		Line spacing, in pixels
	 */
	int getLineSpacing(uint charSize)
	{
		return sfFont_GetLineSpacing(m_ptr, charSize);
	}
	
	Image getImage(uint charSize)
	{
		return new Image(sfFont_GetImage(m_ptr, charSize));
	}

package:

	this(SFMLClass ptr)
	{
		super(ptr, true);
	}

private:
	static extern(C)
	{
		// sfFont
		SFMLClass	function()					sfFont_Create;
		SFMLClass	function(cchar*)			sfFont_CreateFromFile;
		SFMLClass	function(ubyte*, size_t)	sfFont_CreateFromMemory;
		void	function(SFMLClass)				sfFont_Destroy;
		SFMLClass	function()					sfFont_GetDefaultFont;
		
		// DSFML2
		Glyph	function(SFMLClass, uint, uint, bool)	sfFont_GetGlyph;
		int		function(SFMLClass, uint, uint, uint)	sfFont_GetKerning;
		int		function(SFMLClass, uint)				sfFont_GetLineSpacing;
		SFMLClass	function(SFMLClass, uint)				sfFont_GetImage;
	}
	
	static this()
	{
	debug
		DllLoader dll = DllLoader.load("csfml-graphics-d-2");
	else
		DllLoader dll = DllLoader.load("csfml-graphics-2");

		// sfFont
		mixin(loadFromSharedLib("sfFont_CreateFromFile"));
		mixin(loadFromSharedLib("sfFont_CreateFromMemory"));
		mixin(loadFromSharedLib("sfFont_Destroy"));
		mixin(loadFromSharedLib("sfFont_GetDefaultFont"));
		
		// DSFML2
		mixin(loadFromSharedLib("sfFont_GetGlyph"));
		mixin(loadFromSharedLib("sfFont_GetKerning"));
		mixin(loadFromSharedLib("sfFont_GetLineSpacing"));
		mixin(loadFromSharedLib("sfFont_GetImage"));

	}
}