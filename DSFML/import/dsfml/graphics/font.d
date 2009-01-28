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

module dsfml.graphics.font;

import dsfml.system.common;
import dsfml.system.exception;
import dsfml.system.stringutil;

/**
*   Font is the low-level class for loading and
*   manipulating character fonts.
*/
class Font : DSFMLObject
{
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
    *       charSize = size of characters (30 by default)
    *       charset = characters set to generate (empty by default - takes the ASCII range [31, 255])
    */        
    this(char[] filename, uint charSize = 30, dchar[] charset = null)
    {
        if (filename is null || filename.length == 0)
	       throw new LoadingException("LoadingException : Filename is invalid.");

        super(sfFont_CreateFromFile(toStringz(filename), charSize, toStringz(charset)));
    }
    
    /**
    *   construct the Font from a file in memory
    *   
    *   Params:
    *       data = data to load    
    *       charSize = size of characters (30 by default)
    *       charset = characters set to generate (empty by default - takes the ASCII range [31, 255])
    */            
    this(byte[] data, uint charSize = 30, dchar[] charset = null)
    {
        if (data is null || data.length == 0)
            throw new Exception("LoadingException : Memory stream is invalid.");

        super(sfFont_CreateFromMemory(data.ptr, data.length, charSize, toStringz(charset)));
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

private:
    static Font s_default;
    
    extern (C)
    {
        typedef void* function() pf_sfFont_Create;
        typedef void* function(char*, uint, dchar*) pf_sfFont_CreateFromFile;
        typedef void* function(byte*, size_t, uint, dchar*) pf_sfFont_CreateFromMemory;
        typedef void function(void*) pf_sfFont_Destroy;
        typedef void* function() pf_sfFont_GetDefaultFont;
        
        static pf_sfFont_Create sfFont_Create;
        static pf_sfFont_CreateFromFile sfFont_CreateFromFile;
        static pf_sfFont_CreateFromMemory sfFont_CreateFromMemory;
        static pf_sfFont_Destroy sfFont_Destroy;
        static pf_sfFont_GetDefaultFont sfFont_GetDefaultFont;
    }

    static this()
    {
        DllLoader dll = DllLoader.load("csfml-graphics");
        
        sfFont_Create = cast(pf_sfFont_Create) dll.getSymbol("sfFont_Create");
        sfFont_CreateFromFile = cast(pf_sfFont_CreateFromFile) dll.getSymbol("sfFont_CreateFromFile");
        sfFont_CreateFromMemory = cast(pf_sfFont_CreateFromMemory) dll.getSymbol("sfFont_CreateFromMemory");
        sfFont_Destroy = cast(pf_sfFont_Destroy) dll.getSymbol("sfFont_Destroy");
        sfFont_GetDefaultFont = cast(pf_sfFont_GetDefaultFont) dll.getSymbol("sfFont_GetDefaultFont");
    }
} 
