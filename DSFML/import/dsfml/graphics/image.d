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



/*
*   TODO : FIX circular dependency with render window
*/
module dsfml.graphics.image;

import dsfml.graphics.color;
import dsfml.graphics.rect;

import dsfml.system.common;
import dsfml.system.exception;
import dsfml.system.stringutil;


/**
*   Image is the low-level class for loading and
*   manipulating images
*/
class Image : DSFMLObject
{
    /**
    *   Default constructor
    */
    this()
	{
		super(sfImage_Create());
	}

    /**
    *   Construct an empty image
    *
    *   Params:    
    *       width = Image width
    *       height = Image height
    *       col = Image color (black by default)
    */
    this(uint width, uint height, Color col = Color.BLACK)
	{
		super(sfImage_CreateFromColor(width, height, col));
	}

    /**
    *   Construct the image from a file
    *
    *   Params: 
    *       filename = Path of the image file to load
    *       
    *   Throws:
    *       LoadingException if filename is empty or null.
    */
    this(char[] filename)
	{
	   if (filename is null || filename.length == 0)
	       throw new LoadingException("LoadingException : Filename is invalid.");

		super(sfImage_CreateFromFile(toStringz(filename))); 
	}

    /**
    *   Construct the image from a file in memory
    *
    *   Params:
    *       data = array of data in memory
    *   Throws:
    *       LoadingException if data is empty or null.
    */
    this(byte[] data)
	{
        if (data is null || data.length == 0)
            throw new LoadingException("LoadingException : Memory stream is invalid.");
            
		super(sfImage_CreateFromMemory(data.ptr, data.length));
	}

    /**
    *   Construct the image directly from an array of pixels
    *
    *   Params: 
    *       width = Image width
    *       height = Image height
    *       data = array of pixels in memory (assumed format is RGBA)
    *   
    *   Throws:
    *       LoadingException if data length doesn't match Width * Height * 4
    */
    this(uint width, uint height, ubyte[] data)
	{
        if (width * height * 4 != data.length)
            throw new LoadingException("LoadingException : Pixels array length doesn't match the specified size.");
            
		super(sfImage_CreateFromPixels(width, height, data.ptr));
	}

    override void dispose()
	{
		sfImage_Destroy(m_ptr);
	}

    /**
    *   Save the content of the image to a file
    *
    *   Params: 
    *       filename = Path of the file to save (overwritten if already exist)
    *
    *   Returns: 
    *       True if saving was successful
    */
    bool saveToFile(char[] filename)
	{
		return cast(bool)sfImage_SaveToFile(m_ptr, toStringz(filename));
	}

    /**
    *   Create an empty image
    *   
    *   Params:    
    *       width = Image width
    *       height = Image height
    *       col = Image color (black by default)
    *
    *   Returns: 
    *       True if creation was successful
    */
    bool create(uint width, uint height, Color col = Color.BLACK)
	{
		m_ptr = sfImage_CreateFromColor(width, height, col);
		return (m_ptr !is null); 
	}

    /**
    *   Create transparency mask from a specified colorkey
    *
    *   Params: 
    *       colorKey = Color to become transparent
    *       alpha = Alpha value to use for transparent pixels (0 by default)
    */
    void createMaskFromColor(Color colorKey, ubyte alpha = 0)
	{
		sfImage_CreateMaskFromColor(m_ptr, colorKey, alpha);
	}
    
//     /**
//     *   Create the image from the current contents of the
//     *   given window
//     *
//     *   Params:    
//     *       window = Window to capture
//     *       sourceRect = Sub-rectangle of the screen to copy (empty by default - entire image)
//     *
//     *   Returns:
//     *       True if copy was successful
//     */
//     void copyScreen(RenderWindow window, IntRect sourceRect = new IntRect())
//     {
//         return cast(bool)sfImage_CopyScreen(m_ptr, window.getNativePointer, sourceRect.toCIntRect());
//     }
    
    /**
    *   Copy pixels from another image onto this one.
    *   This function does a slow pixel copy and should only
    *   be used at initialization time
    *
    *   Params:
    *       source = Source image to copy
    *       destX = X coordinate of the destination position
    *       destY = Y coordinate of the destination position
    *       sourceRect = Sub-rectangle of the source image to copy
    */
    void copy(Image source, uint destX, uint destY, IntRect sourceRect = new IntRect())
    {
        sfImage_Copy(m_ptr, source.getNativePointer, destX, destY, sourceRect.toCIntRect());
    } 

    /**
    *   Change the color of a pixel
    *   Don't forget to call Update when you end modifying pixels
    *   
    *   Params:    
    *       x = X coordinate of pixel in the image
    *       y = Y coordinate of pixel in the image
    *       col = New color for pixel (X, Y)
    */
    void setPixel(uint x, uint y, Color col)
	{
		sfImage_SetPixel(m_ptr, x, y, col);
	}

    /**
    *   Get a pixel from the image
    *
    *   Params:    
    *       x = X coordinate of pixel in the image
    *       y = Y coordinate of pixel in the image
    *
    *   Returns: 
    *       Color of pixel (x, y)
    */
    Color getPixel(uint x, uint y)
	{
		return sfImage_GetPixel(m_ptr, x, y);
	}

    /**
    *   Get an array of pixels (8 bits integers RGBA)
    *   Array size is GetWidth() x GetHeight() x 4
    *   This array becomes invalid if you reload or resize the image
    *
    *   Returns:
    *       array of pixels
    */
    ubyte[] getPixelsArray()
	{
		return sfImage_GetPixelsPtr(m_ptr)[0..getWidth() * getHeight() * 4];
	}

    /**
    *   Bind the image for rendering
    */
    void bind()
	{
		sfImage_Bind(m_ptr);
	}

    /**
    *   Enable or disable image smooth filter.
    *   This parameter is enabled by default
    *
    *   Params: 
    *       smooth = True to enable smoothing filter, false to disable it
    */
    void setSmooth(bool smooth)
	{
		sfImage_SetSmooth(m_ptr, smooth);
	}

    /**
    *   Return the width of the image
    *
    *   Returns: 
    *       Width in pixels
    */
    uint getWidth()
	{
		return sfImage_GetWidth(m_ptr);
	}

    /**
    *   Return the height of the image
    *
    *   Returns: 
    *       Height in pixels
    */
    uint getHeight()
	{
		return sfImage_GetHeight(m_ptr);
	}
    
    /**
    *   Tells whether the smooth filtering is enabled or not
    *
    *   Returns:
    *       True if image smoothing is enabled
    */
    bool isSmooth()
    {
        return cast(bool)sfImage_IsSmooth(m_ptr);
    }
    
package:
    this(void* ptr)
    {
        super(ptr);
    }

private:
    extern (C)
    {
        typedef void* function() pf_sfImage_Create;
    	typedef void* function(uint, uint, Color) pf_sfImage_CreateFromColor;
    	typedef void* function(uint, uint, ubyte*) pf_sfImage_CreateFromPixels;
    	typedef void* function(char*) pf_sfImage_CreateFromFile;
    	typedef void* function(byte* ,size_t) pf_sfImage_CreateFromMemory;
    	typedef void function(void*) pf_sfImage_Destroy;
    	typedef int function(void*, char*) pf_sfImage_SaveToFile;
    	typedef void function(void*, Color, ubyte) pf_sfImage_CreateMaskFromColor;
        typedef int function(void*, void*, sfIntRect) pf_sfImage_CopyScreen;
    	typedef void function(void*, void*, uint, uint, sfIntRect) pf_sfImage_Copy;
    	typedef void function(void*, uint, uint, Color) pf_sfImage_SetPixel;
    	typedef Color function(void*, uint, uint) pf_sfImage_GetPixel;
    	typedef ubyte* function(void*) pf_sfImage_GetPixelsPtr;
    	typedef void function(void*) pf_sfImage_Bind;
    	typedef void function(void*, int) pf_sfImage_SetSmooth;
    	typedef uint function(void*) pf_sfImage_GetWidth;
    	typedef uint function(void*) pf_sfImage_GetHeight;
    	typedef int function(void*) pf_sfImage_IsSmooth;
    
    	static pf_sfImage_Create sfImage_Create;
    	static pf_sfImage_CreateFromColor sfImage_CreateFromColor;
    	static pf_sfImage_CreateFromPixels sfImage_CreateFromPixels;
    	static pf_sfImage_CreateFromFile sfImage_CreateFromFile;
    	static pf_sfImage_CreateFromMemory sfImage_CreateFromMemory;
    	static pf_sfImage_Destroy sfImage_Destroy;
    	static pf_sfImage_SaveToFile sfImage_SaveToFile;
    	static pf_sfImage_CreateMaskFromColor sfImage_CreateMaskFromColor;
    	static pf_sfImage_CopyScreen sfImage_CopyScreen;
    	static pf_sfImage_Copy sfImage_Copy;
    	static pf_sfImage_SetPixel sfImage_SetPixel;
    	static pf_sfImage_GetPixel sfImage_GetPixel;
    	static pf_sfImage_GetPixelsPtr sfImage_GetPixelsPtr;
    	static pf_sfImage_Bind sfImage_Bind;
    	static pf_sfImage_SetSmooth sfImage_SetSmooth;
    	static pf_sfImage_GetWidth sfImage_GetWidth;
    	static pf_sfImage_GetHeight sfImage_GetHeight;
    	static pf_sfImage_IsSmooth sfImage_IsSmooth;
    }

    static this()
    {
        DllLoader dll = DllLoader.load("csfml-graphics");
        
        sfImage_Create = cast(pf_sfImage_Create)dll.getSymbol("sfImage_Create");
        sfImage_CreateFromColor = cast(pf_sfImage_CreateFromColor)dll.getSymbol("sfImage_CreateFromColor");
        sfImage_CreateFromPixels = cast(pf_sfImage_CreateFromPixels)dll.getSymbol("sfImage_CreateFromPixels");
        sfImage_CreateFromFile = cast(pf_sfImage_CreateFromFile)dll.getSymbol("sfImage_CreateFromFile");
        sfImage_CreateFromMemory = cast(pf_sfImage_CreateFromMemory)dll.getSymbol("sfImage_CreateFromMemory");
        sfImage_Destroy = cast(pf_sfImage_Destroy)dll.getSymbol("sfImage_Destroy");
        sfImage_SaveToFile = cast(pf_sfImage_SaveToFile)dll.getSymbol("sfImage_SaveToFile");
        sfImage_CreateMaskFromColor = cast(pf_sfImage_CreateMaskFromColor)dll.getSymbol("sfImage_CreateMaskFromColor");
        sfImage_CopyScreen = cast(pf_sfImage_CopyScreen)dll.getSymbol("sfImage_CopyScreen");
        sfImage_Copy = cast(pf_sfImage_Copy)dll.getSymbol("sfImage_Copy");
        sfImage_SetPixel = cast(pf_sfImage_SetPixel)dll.getSymbol("sfImage_SetPixel");
        sfImage_GetPixel = cast(pf_sfImage_GetPixel)dll.getSymbol("sfImage_GetPixel");
        sfImage_GetPixelsPtr = cast(pf_sfImage_GetPixelsPtr)dll.getSymbol("sfImage_GetPixelsPtr");
        sfImage_Bind = cast(pf_sfImage_Bind)dll.getSymbol("sfImage_Bind");
        sfImage_SetSmooth = cast(pf_sfImage_SetSmooth)dll.getSymbol("sfImage_SetSmooth");
        sfImage_GetWidth = cast(pf_sfImage_GetWidth)dll.getSymbol("sfImage_GetWidth");
        sfImage_GetHeight = cast(pf_sfImage_GetHeight)dll.getSymbol("sfImage_GetHeight");
        sfImage_IsSmooth = cast(pf_sfImage_IsSmooth)dll.getSymbol("sfImage_IsSmooth");
    }
}
