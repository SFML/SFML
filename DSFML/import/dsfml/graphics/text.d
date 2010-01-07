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
import dsfml.system.vector2;


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
class Text : Drawableimpl!("sfText")
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
	*		text = Text assigned to the string
	*		font = Font used to draw the string (use default font)
	*		size = Characters size, in pixels (32 by default)
	*/
	this(string text, Font font = Font.getDefaultFont(), uint size = 30)
	{
		super();
		m_font = font;
		setFont(font);
		setString(text);
		setCharacterSize(size);
	}

	/**
	*	Construct the string from a unicode text
	*	
	*	Prefixs string litterals with d  
	*			
	*	Params:	
	*		text = Text assigned to the string
	*		font = Font used to draw the string (use default font)
	*		size = Characters size, in pixels (32 by default)
	*/
	this(dstring text, Font font = Font.getDefaultFont(), uint size = 30)
	{
		super();
		m_font = font;
		setFont(font);
		setString(text);
		setCharacterSize(size);
	}

	/**
	*	Set the text (from a multibyte string)
	*
	*	Params: 
	*		text = New text
	*/
	void setString(string text)
	{
		sfText_SetString(m_ptr, toStringz(text));
	}

	/**
	*	Set the text (from a unicode string)
	*
	*	Params:	 
	*		text = New text
	*/
	void setString(dstring text)
	{
		sfText_SetUnicodeString(m_ptr, toStringz(text));
	}

	/**
	*	Set the font of the string
	*
	*	Params:	
	*		font = Font filename
	*/
	void setFont(Font font)
	{
		m_font = font;
		sfText_SetFont(m_ptr, font.getNativePointer);
	}

	/**
	*	Set the size of the string
	*
	*	Params:	
	*		size = New size, in pixels
	*/
	void setCharacterSize(uint size)
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
	void setStyle(TextStyle style)
	{
		sfText_SetStyle(m_ptr, style);
	}		

	/**
	*	Get the text (returns a unicode string)
	*
	*	Returns: 
	*		Text
	*/
	dstring getUnicodeText()
	{
		return fromStringz(sfText_GetUnicodeString(m_ptr));
	}

	/**
	*	Get the text (returns a multibyte string)
	*
	*	Returns: 
	*		Text
	*/
	string getText()
	{
		return fromStringz(sfText_GetString(m_ptr));
	}

	/**
	*	Get the font used by the string
	*
	*	Returns: 
	*		Font name
	*/
	Font getFont()
	{
		return m_font;
	}

	/**
	*	Get the size of the characters
	*
	*	Returns: 
	*		Size of the characters
	*/
	uint getCharacterSize()
	{
		return sfText_GetCharacterSize(m_ptr);
	}

	/**
	*	Get the current font style
	*	
	*	Returns:
	*		Font style			
	*/
	TextStyle getStyle()
	{
		return sfText_GetStyle(m_ptr);
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
	
	/**
	*	Get the string rectangle on screen
	*
	*	Returns: 
	*		Rectangle contaning the string in screen coordinates
	*/
	FloatRect getRect()
	{
		return sfText_GetRect(m_ptr);
	}

private:
	
	extern (C)
	{
		typedef void function(void*, cchar*) pf_sfText_SetString;
		typedef void function(void*, cdchar*) pf_sfText_SetUnicodeString;
		typedef void function(void*, void*) pf_sfText_SetFont;
		typedef void function(void*, uint) pf_sfText_SetCharacterSize;
		typedef void function(void*, TextStyle) pf_sfText_SetStyle;
		typedef idchar* function(void*) pf_sfText_GetUnicodeString;
		typedef ichar* function(void*) pf_sfText_GetString;
		typedef void* function(void*) pf_sfText_GetFont;
		typedef uint function(void*) pf_sfText_GetCharacterSize;
		typedef TextStyle function (void*) pf_sfText_GetStyle;
		typedef void function(void*, size_t, float*, float*) pf_sfText_GetCharacterPos;
		typedef FloatRect function(void*) pf_sfText_GetRect;
	
		static pf_sfText_SetString sfText_SetString;
		static pf_sfText_SetUnicodeString sfText_SetUnicodeString;
		static pf_sfText_SetFont sfText_SetFont;
		static pf_sfText_SetCharacterSize sfText_SetCharacterSize;
		static pf_sfText_SetStyle sfText_SetStyle;
		static pf_sfText_GetUnicodeString sfText_GetUnicodeString;
		static pf_sfText_GetString sfText_GetString;
		static pf_sfText_GetFont sfText_GetFont;
		static pf_sfText_GetCharacterSize sfText_GetCharacterSize;
		static pf_sfText_GetStyle sfText_GetStyle;
		static pf_sfText_GetCharacterPos sfText_GetCharacterPos;
		static pf_sfText_GetRect sfText_GetRect;
	}

	static this()
	{
	debug
		DllLoader dll = DllLoader.load("csfml-graphics-d");
	else
		DllLoader dll = DllLoader.load("csfml-graphics");
		
		sfText_SetString = cast(pf_sfText_SetString)dll.getSymbol("sfText_SetString");
		sfText_SetUnicodeString = cast(pf_sfText_SetUnicodeString)dll.getSymbol("sfText_SetUnicodeString");
		sfText_SetFont = cast(pf_sfText_SetFont)dll.getSymbol("sfText_SetFont");
		sfText_SetCharacterSize = cast(pf_sfText_SetCharacterSize)dll.getSymbol("sfText_SetCharacterSize");
		sfText_SetStyle = cast(pf_sfText_SetStyle)dll.getSymbol("sfText_SetStyle");
		sfText_GetUnicodeString = cast(pf_sfText_GetUnicodeString)dll.getSymbol("sfText_GetUnicodeString");
		sfText_GetString = cast(pf_sfText_GetString)dll.getSymbol("sfText_GetString");
		sfText_GetFont = cast(pf_sfText_GetFont)dll.getSymbol("sfText_GetFont");
		sfText_GetCharacterSize = cast(pf_sfText_GetCharacterSize)dll.getSymbol("sfText_GetCharacterSize");
		sfText_GetStyle = cast(pf_sfText_GetStyle)dll.getSymbol("sfText_GetStyle");
		sfText_GetCharacterPos = cast(pf_sfText_GetCharacterPos)dll.getSymbol("sfText_GetCharacterPos");
		sfText_GetRect = cast(pf_sfText_GetRect)dll.getSymbol("sfText_GetRect");
	}
}