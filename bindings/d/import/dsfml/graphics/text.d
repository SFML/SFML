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

module dsfml.graphics.text;

import dsfml.graphics.blendmode;
import dsfml.graphics.color;
import dsfml.graphics.font;
import dsfml.graphics.drawableimpl;
import dsfml.graphics.rect;

import dsfml.system.stringutil;
import dsfml.system.vector;


/**
 *	Enumerate the text drawing styles
 */
enum TextStyle
{
	REGULAR	= 0,	  /// Regular characters, no style
	BOLD		= 1 << 0, /// Characters are bold
	ITALIC	 = 1 << 1, /// Characters are in italic
	UNDERLINED = 1 << 2  /// Characters are underlined
}

/**
*	Text defines a graphical 2D text, that can be drawn on screen
*	
*	All string litterals used must be prefixed with c for utf-8 
*	and d for utf-32 string.
*	
*	Examples :
*	---------------------------------------------------------------
*	Text s = new Text("Hello"c);
*	//this(string, Font, float)
*	s = new Text("Hello"d);
*	//this(dstring, Font, float)
*	---------------------------------------------------------------
*	
*	See_Also:
*		IDrawable
*/
class Text : DrawableImpl!("sfText")
{
private:
	Font m_font;

public:
	/**
	*	Construct the string from a text
	*	
	*	Prefixs string litterals with c		
	*
	*	Params:	
	*		s = Text assigned to the string
	*		f = Font used to draw the string (use default font)
	*		size = Characters size, in pixels (32 by default)
	*/
	this(string s, Font f = Font.getDefaultFont(), uint size = 30)
	{
		super();

		font = f;
		text = s;
		characterSize = size;
	}

	/**
	*	Construct the string from a unicode text
	*	
	*	Prefixs string litterals with d  
	*			
	*	Params:	
	*		s = Text assigned to the string
	*		f = Font used to draw the string (use default font)
	*		size = Characters size, in pixels (32 by default)
	*/
	this(dstring s, Font f = Font.getDefaultFont(), uint size = 30)
	{
		super();

		font = f;
		text = s;
		characterSize = size;
	}

@property
{
	/**
	 *	Set the text (from a multibyte string)
	 *
	 *	Params: 
	 *		text = New text
	 */
	void text(string text)
	{
		sfText_SetString(m_ptr, toStringz(text));
	}

	/**
	 *	Set the text (from a unicode string)
	 *
	 *	Params:	 
	 *		text = New text
	 */
	void text(dstring text)
	{
		sfText_SetUnicodeString(m_ptr, toStringz(text));
	}

	/**
	 *	Get the text (returns a multibyte string)
	 *
	 *	Returns: 
	 *		Text
	 */
	string text()
	{
		return fromStringz(sfText_GetString(m_ptr));
	}
	
	/**
	 *	Set the font of the string
	 *
	 *	Params:	
	 *		f = Font
	 */
	void font(Font f)
	{
		m_font = f;
		sfText_SetFont(m_ptr, f.nativePointer);
	}

	/**
	 *	Set the size of the string
	 *
	 *	Params:	
	 *		size = New size, in pixels
	 */
	void characterSize(uint size)
	{
		sfText_SetCharacterSize(m_ptr, size);
	}

	/**
	 *	Set the style of the text
	 *	The default style is Regular
	 *
	 *	Params:
	 *		TextStyle = New text style, (combination of Style enum values)
	 *
	 */
	void style(TextStyle tstyle)
	{
		sfText_SetStyle(m_ptr, tstyle);
	}		

	/**
	 *	Get the text (returns a unicode string)
	 *
	 *	Returns: 
	 *		Text
	 */
	dstring unicodeText()
	{
		return fromStringz(sfText_GetUnicodeString(m_ptr));
	}

	/**
	 *	Get the font used by the string
	 *
	 *	Returns: 
	 *		Font name
	 */
	Font font()
	{
		return m_font;
	}

	/**
	*	Get the size of the characters
	*
	*	Returns: 
	*		Size of the characters
	*/
	uint characterSize()
	{
		return sfText_GetCharacterSize(m_ptr);
	}

	/**
	*	Get the current font style
	*	
	*	Returns:
	*		Font style			
	*/
	TextStyle style()
	{
		return sfText_GetStyle(m_ptr);
	}
	
	/**
	 *	Get the string rectangle on screen
	 *
	 *	Returns: 
	 *		Rectangle contaning the string in screen coordinates
	 */
	FloatRect rect()
	{
		return sfText_GetRect(m_ptr);
	}
}

	/**
	 *	Return the visual position of the Index-th character of the string,
	 *	in coordinates relative to the string
	 *	(note : translation, center, rotation and scale are not applied)
	 *
	 *	Params:
	 *		index = Index of the character
	 *
	 *	Returns:
	 *		Position of the Index-th character (end of string of Index is out of range)
	 */
	Vector2f getCharacterPos(size_t index)
	{
		Vector2f ret;
		sfText_GetCharacterPos(m_ptr, index, &ret.x, &ret.y);
		return ret;
	}

private:
	
	static extern(C)
	{
		void		function(SFMLClass, cchar*)					sfText_SetString;
		void		function(SFMLClass, cdchar*)				sfText_SetUnicodeString;
		void		function(SFMLClass, SFMLClass)					sfText_SetFont;
		void		function(SFMLClass, uint)					sfText_SetCharacterSize;
		void		function(SFMLClass, TextStyle)				sfText_SetStyle;
		idchar*		function(SFMLClass)							sfText_GetUnicodeString;
		ichar*		function(SFMLClass)							sfText_GetString;
		SFMLClass		function(SFMLClass)							sfText_GetFont;
		uint		function(SFMLClass)							sfText_GetCharacterSize;
		TextStyle	function (SFMLClass)						sfText_GetStyle;
		void		function(SFMLClass, size_t, float*, float*)	sfText_GetCharacterPos;
		FloatRect	function(SFMLClass)							sfText_GetRect;
	}

	mixin(loadFromSharedLib2("csfml-graphics", "sfText",
	"SetString", "SetUnicodeString", "SetFont", "SetCharacterSize", "SetStyle", "GetUnicodeString", "GetString", "GetFont",
	"GetCharacterSize", "GetStyle", "GetCharacterPos", "GetRect"));
}