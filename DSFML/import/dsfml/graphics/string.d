/*
*   DSFML - SFML Library binding in D language.
*   Copyright (C) 2008 Julien Dagorn (sirjulio13@gmail.com)
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

module dsfml.graphics.string;

import dsfml.graphics.blendmode;
import dsfml.graphics.color;
import dsfml.graphics.font;
import dsfml.graphics.textstyle;
import dsfml.graphics.drawableimpl;
import dsfml.graphics.rect;

import dsfml.system.stringutil;
import dsfml.system.vector2;


/**
*   String defines a graphical 2D text, that can be drawn on screen
*   
*   All string litterals used must be prefixed with c for utf-8 
*   and d for utf-32 string.
*    
*   Examples :
*   ---------------------------------------------------------------
*   String s = new String("Hello"c);
*   //this(char[], Font, float)
*   s = new String("Hello"d);
*   //this(dchar[], Font, float)
*   ---------------------------------------------------------------
*   
*   See_Also:
*       IDrawable
*/
class String : Drawableimpl!(sfString)
{
    /**
    *   Construct the string from a text
    *   
    *   Prefixs string litterals with c        
    *
    *   Params:    
    *       text = Text assigned to the string
    *       font = Font used to draw the string (use default font)
    *       size = Characters size, in pixels (32 by default)
    */
    this(char[] text, Font font = Font.getDefaultFont(), float size = 30.f)
	{
		super();
		m_font = font;
		setFont(font);
		setText(text);
		setSize(size);
	}

    /**
    *   Construct the string from a unicode text
    *   
    *   Prefixs string litterals with d  
    *           
    *   Params:    
    *       text = Text assigned to the string
    *       font = Font used to draw the string (use default font)
    *       size = Characters size, in pixels (32 by default)
    */
    this(dchar[] text, Font font = Font.getDefaultFont(), float size = 30.f)
	{
		super();
		m_font = font;
		setFont(font);
		setText(text);
		setSize(size);
	}

    /**
    *   Set the text (from a multibyte string)
    *
    *   Params: 
    *       text = New text
    *
    */
    void setText(char[] text)
	{
		sfString_SetText(m_ptr,toStringz(text));
	}

    /**
    *   Set the text (from a unicode string)
    *
    *   Params:     
    *       text = New text
    */
    void setText(dchar[] text)
	{
		sfString_SetUnicodeText(m_ptr, toStringz(text));
	}

    /**
    *   Set the font of the string
    *
    *   Params:    
    *       font = Font filename
    */
    void setFont(Font font)
	{
        m_font = font;
		sfString_SetFont(m_ptr, font.getNativePointer);
	}

    /**
    *   Set the size of the string
    *
    *   Params:    
    *       size = New size, in pixels
    */
    void setSize(float size)
	{
		sfString_SetSize(m_ptr, size);
	}

    /**
    *   Set the style of the text
    *   The default style is Regular
    *
    *   Params:
    *       TextStyle = New text style, (combination of Style enum values)
    *
    */
    void setStyle(TextStyle style)
    {
        sfString_SetStyle(m_ptr, style);
    }        

    /**
    *   Get the text (returns a unicode string)
    *
    *   Returns: 
    *       Text
    */
    dchar[] getUnicodeText()
	{
		return fromStringz(sfString_GetUnicodeText(m_ptr));
	}

    /**
    *   Get the text (returns a multibyte string)
    *
    *   Returns: 
    *       Text
    */
    char[] getText()
	{
		return fromStringz(sfString_GetText(m_ptr));
	}

    /**
    *   Get the font used by the string
    *
    *   Returns: 
    *       Font name
    */
    Font getFont()
	{
		return m_font;
	}

    /**
    *   Get the size of the characters
    *
    *   Returns: 
    *       Size of the characters
    */
    float getSize()
	{
		return sfString_GetSize(m_ptr);
	}

    /**
    *   Get the current font style
    *   
    *   Returns:
    *       Font style            
    */
    TextStyle getStyle()
    {
        return sfString_GetStyle(m_ptr);
    }
    
    /**
    *   Return the visual position of the Index-th character of the string,
    *   in coordinates relative to the string
    *   (note : translation, center, rotation and scale are not applied)
    *
    *   Params:
    *       index = Index of the character
    *
    *   Returns:
    *       Position of the Index-th character (end of string of Index is out of range)
    */
    Vector2f getCharacterPos(size_t index)
    {
        Vector2f ret;
        sfString_GetCharacterPos(m_ptr, index, &ret.x, &ret.y);
        return ret;
    }
    
    /**
    *   Get the string rectangle on screen
    *
    *   Returns: 
    *       Rectangle contaning the string in screen coordinates
    */
    FloatRect getRect()
	{
		sfFloatRect sfRect = sfString_GetRect(m_ptr);
		
		return new Rect!(float)(sfRect.Left, sfRect.Top, sfRect.Right, sfRect.Bottom);
	}

private:
    Font m_font;
    
    extern (C)
    {
        typedef void function(void*, char*) pf_sfString_SetText;
    	typedef void function(void*, dchar*) pf_sfString_SetUnicodeText;
    	typedef void function(void*, void*) pf_sfString_SetFont;
    	typedef void function(void*, float) pf_sfString_SetSize;
    	typedef void function(void*, TextStyle) pf_sfString_SetStyle;
    	typedef dchar* function(void*) pf_sfString_GetUnicodeText;
    	typedef char* function(void*) pf_sfString_GetText;
    	typedef void* function(void*) pf_sfString_GetFont;
    	typedef float function(void*) pf_sfString_GetSize;
    	typedef TextStyle function (void*) pf_sfString_GetStyle;
    	typedef void function(void*, size_t, float*, float*) pf_sfString_GetCharacterPos;
    	typedef sfFloatRect function(void*) pf_sfString_GetRect;
    
    	static pf_sfString_SetText sfString_SetText;
    	static pf_sfString_SetUnicodeText sfString_SetUnicodeText;
    	static pf_sfString_SetFont sfString_SetFont;
    	static pf_sfString_SetSize sfString_SetSize;
    	static pf_sfString_SetStyle sfString_SetStyle;
    	static pf_sfString_GetUnicodeText sfString_GetUnicodeText;
    	static pf_sfString_GetText sfString_GetText;
    	static pf_sfString_GetFont sfString_GetFont;
    	static pf_sfString_GetSize sfString_GetSize;
    	static pf_sfString_GetStyle sfString_GetStyle;
    	static pf_sfString_GetCharacterPos sfString_GetCharacterPos;
    	static pf_sfString_GetRect sfString_GetRect;
    }

    static this()
    {
        DllLoader dll = DllLoader.load("csfml-graphics");
        
        sfString_SetText = cast(pf_sfString_SetText)dll.getSymbol("sfString_SetText");
        sfString_SetUnicodeText = cast(pf_sfString_SetUnicodeText)dll.getSymbol("sfString_SetUnicodeText");
        sfString_SetFont = cast(pf_sfString_SetFont)dll.getSymbol("sfString_SetFont");
        sfString_SetSize = cast(pf_sfString_SetSize)dll.getSymbol("sfString_SetSize");
        sfString_SetStyle = cast(pf_sfString_SetStyle)dll.getSymbol("sfString_SetStyle");
        sfString_GetUnicodeText = cast(pf_sfString_GetUnicodeText)dll.getSymbol("sfString_GetUnicodeText");
        sfString_GetText = cast(pf_sfString_GetText)dll.getSymbol("sfString_GetText");
        sfString_GetFont = cast(pf_sfString_GetFont)dll.getSymbol("sfString_GetFont");
        sfString_GetSize = cast(pf_sfString_GetSize)dll.getSymbol("sfString_GetSize");
        sfString_GetStyle = cast(pf_sfString_GetStyle)dll.getSymbol("sfString_GetStyle");
        sfString_GetCharacterPos = cast(pf_sfString_GetCharacterPos)dll.getSymbol("sfString_GetCharacterPos");
        sfString_GetRect = cast(pf_sfString_GetRect)dll.getSymbol("sfString_GetRect");
    }
}
