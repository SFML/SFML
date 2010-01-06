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

module dsfml.graphics.font;

import	dsfml.system.common,
		dsfml.system.exception,
		dsfml.system.stringutil;

import	dsfml.graphics.common,
		dsfml.graphics.rect;


/// Glyph describes a glyph (a visual character)
struct Glyph
{
	int			Advance;	/// Offset to move horizontically to the next character
	sfIntRect	Rectangle;	/// Bounding rectangle of the glyph, in relative coordinates
	sfFloatRect	TexCoords;	/// Texture coordinates of the glyph inside the bitmap font
}

/**
*   Font is the low-level class for loading and
*   manipulating character fonts.
*/
class Font : DSFMLObject
{
private:
	static Font s_default;
public:

	/**
    *   Get SFML default built-in font (Arial)
    */        
    static Font getDefaultFont()
    {
        if (s_default is null)
            s_default = new Font(sfFont_GetDefaultFont());           
        return s_default;
    }

    /**
    *   construct the Font from a file
    *   
    *   Params:
    *       filename = font file to load
    */        
    this(string filename)
    {
        if (filename is null || filename.length == 0)
	       throw new LoadingException("LoadingException : Filename is invalid.");

        super(sfFont_CreateFromFile(toStringz(filename)));
    }
    
    /**
    *   construct the Font from a file in memory
    *   
    *   Params:
    *       data = data to load    
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


package:

    this(void* ptr)
    {
        super(ptr, true);
    }
} 
