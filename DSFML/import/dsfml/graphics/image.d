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


module dsfml.graphics.image;

import	dsfml.graphics.color,
		dsfml.graphics.rect;

import	dsfml.system.common,
		dsfml.system.exception,
		dsfml.system.stringutil;


/**
*	Image is the low-level class for loading and
*	manipulating images
*/
class Image : DSFMLObject
{
package:
	this(void* ptr)
	{
		super(ptr, true);
	}
		
public:

	/**
	*	Default constructor
	*/
	this()
	{
		super(sfImage_Create());
	}

	/**
	*	Construct an empty image
	*
	*	Params:	
	*		width = Image width
	*		height = Image height
	*		col = Image color (black by default)
	*/
	this(uint width, uint height, Color col = Color.BLACK)
	{
		super(sfImage_CreateFromColor(width, height, col));
	}

	/**
	*	Construct the image from a file
	*
	*	Params: 
	*		filename = Path of the image file to load
	*		
	*	Throws:
	*		LoadingException if filename is empty or null.
	*/
	this(string filename)
	{
		if (filename is null || filename.length == 0)
			throw new LoadingException("LoadingException : Filename is invalid.");

		super(sfImage_CreateFromFile(toStringz(filename))); 
	}

	/**
	*	Construct the image from a file in memory
	*
	*	Params:
	*		data = array of data in memory
	*	Throws:
	*		LoadingException if data is empty or null.
	*/
	this(ubyte[] data)
	{
		if (data is null || data.length == 0)
			throw new LoadingException("LoadingException : Memory stream is invalid.");
			
		super(sfImage_CreateFromMemory(data.ptr, data.length));
	}

	/**
	*	Construct the image directly from an array of pixels
	*
	*	Params: 
	*		width = Image width
	*		height = Image height
	*		data = array of pixels in memory (assumed format is RGBA)
	*	
	*	Throws:
	*		LoadingException if data length doesn't match Width * Height * 4
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
	*	Save the content of the image to a file
	*
	*	Params: 
	*		filename = Path of the file to save (overwritten if already exist)
	*
	*	Returns: 
	*		True if saving was successful
	*/
	bool saveToFile(string filename)
	{
		return cast(bool)sfImage_SaveToFile(m_ptr, toStringz(filename));
	}

	/**
	*	Create an empty image
	*	
	*	Params:	
	*		width = Image width
	*		height = Image height
	*		col = Image color (black by default)
	*
	*	Returns: 
	*		True if creation was successful
	*/
	bool create(uint width, uint height, Color col = Color.BLACK)
	{
		m_ptr = sfImage_CreateFromColor(width, height, col);
		return (m_ptr !is null); 
	}

	/**
	*	Create transparency mask from a specified colorkey
	*
	*	Params: 
	*		colorKey = Color to become transparent
	*		alpha = Alpha value to use for transparent pixels (0 by default)
	*/
	void createMaskFromColor(Color colorKey, ubyte alpha = 0)
	{
		sfImage_CreateMaskFromColor(m_ptr, colorKey, alpha);
	}
	
//	 /**
//	 *	Create the image from the current contents of the
//	 *	given window
//	 *
//	 *	Params:	
//	 *		window = Window to capture
//	 *		sourceRect = Sub-rectangle of the screen to copy (empty by default - entire image)
//	 *
//	 *	Returns:
//	 *		True if copy was successful
//	 */
//	 void copyScreen(RenderWindow window, IntRect sourceRect = new IntRect())
//	 {
//		 return cast(bool)sfImage_CopyScreen(m_ptr, window.getNativePointer, sourceRect);
//	 }
	
	/**
	*	Copy pixels from another image onto this one.
	*	This function does a slow pixel copy and should only
	*	be used at initialization time
	*
	*	Params:
	*		source = Source image to copy
	*		destX = X coordinate of the destination position
	*		destY = Y coordinate of the destination position
	*		sourceRect = Sub-rectangle of the source image to copy
	*/
	void copy(Image source, uint destX, uint destY, IntRect sourceRect = IntRect())
	{
		sfImage_Copy(m_ptr, source.getNativePointer, destX, destY, sourceRect);
	} 

	/**
	*	Change the color of a pixel
	*	Don't forget to call Update when you end modifying pixels
	*	
	*	Params:	
	*		x = X coordinate of pixel in the image
	*		y = Y coordinate of pixel in the image
	*		col = New color for pixel (X, Y)
	*/
	void setPixel(uint x, uint y, Color col)
	{
		sfImage_SetPixel(m_ptr, x, y, col);
	}

	/**
	*	Get a pixel from the image
	*
	*	Params:	
	*		x = X coordinate of pixel in the image
	*		y = Y coordinate of pixel in the image
	*
	*	Returns: 
	*		Color of pixel (x, y)
	*/
	Color getPixel(uint x, uint y)
	{
		return sfImage_GetPixel(m_ptr, x, y);
	}

	/**
	*	Get an array of pixels (8 bits integers RGBA)
	*	Array size is GetWidth() x GetHeight() x 4
	*	This array becomes invalid if you reload or resize the image
	*
	*	Returns:
	*		array of pixels
	*/
	ubyte[] getPixelsArray()
	{
		return sfImage_GetPixelsPtr(m_ptr)[0..getWidth() * getHeight() * 4];
	}

	/**
	*	Bind the image for rendering
	*/
	void bind()
	{
		sfImage_Bind(m_ptr);
	}

	/**
	*	Enable or disable image smooth filter.
	*	This parameter is enabled by default
	*
	*	Params: 
	*		smooth = True to enable smoothing filter, false to disable it
	*/
	void setSmooth(bool smooth)
	{
		sfImage_SetSmooth(m_ptr, smooth);
	}

	/**
	*	Return the width of the image
	*
	*	Returns: 
	*		Width in pixels
	*/
	uint getWidth()
	{
		return sfImage_GetWidth(m_ptr);
	}

	/**
	*	Return the height of the image
	*
	*	Returns: 
	*		Height in pixels
	*/
	uint getHeight()
	{
		return sfImage_GetHeight(m_ptr);
	}
	
	/**
	*	Tells whether the smooth filtering is enabled or not
	*
	*	Returns:
	*		True if image smoothing is enabled
	*/
	bool isSmooth()
	{
		return cast(bool)sfImage_IsSmooth(m_ptr);
	}
	
private:
	extern (C)
	{
	static
	{
		void*	function()									sfImage_Create;
		void*	function(uint, uint, Color)					sfImage_CreateFromColor;
		void*	function(uint, uint, ubyte*)				sfImage_CreateFromPixels;
		void*	function(cchar*)							sfImage_CreateFromFile;
		void*	function(ubyte* ,size_t)					sfImage_CreateFromMemory;
		void	function(void*)								sfImage_Destroy;
		int		function(void*, cchar*)						sfImage_SaveToFile;
		void	function(void*, Color, ubyte)				sfImage_CreateMaskFromColor;
		int		function(void*, void*, IntRect)				sfImage_CopyScreen;
		void	function(void*, void*, uint, uint, IntRect)	sfImage_Copy;
		void	function(void*, uint, uint, Color)			sfImage_SetPixel;
		Color	function(void*, uint, uint)					sfImage_GetPixel;
		ubyte*	function(void*)								sfImage_GetPixelsPtr;
		void	function(void*)								sfImage_Bind;
		void	function(void*, int)						sfImage_SetSmooth;
		uint	function(void*)								sfImage_GetWidth;
		uint	function(void*)								sfImage_GetHeight;
		int		function(void*)								sfImage_IsSmooth;
	}
	}

	static this()
	{
	debug
		DllLoader dll = DllLoader.load("csfml-graphics-d");
	else
		DllLoader dll = DllLoader.load("csfml-graphics");
		
		mixin(loadFromSharedLib("sfImage_Create"));
		mixin(loadFromSharedLib("sfImage_CreateFromColor"));
		mixin(loadFromSharedLib("sfImage_CreateFromPixels"));
		mixin(loadFromSharedLib("sfImage_CreateFromFile"));
		mixin(loadFromSharedLib("sfImage_CreateFromMemory"));
		mixin(loadFromSharedLib("sfImage_Destroy"));
		mixin(loadFromSharedLib("sfImage_SaveToFile"));
		mixin(loadFromSharedLib("sfImage_CreateMaskFromColor"));
		mixin(loadFromSharedLib("sfImage_CopyScreen"));
		mixin(loadFromSharedLib("sfImage_Copy"));
		mixin(loadFromSharedLib("sfImage_SetPixel"));
		mixin(loadFromSharedLib("sfImage_GetPixel"));
		mixin(loadFromSharedLib("sfImage_GetPixelsPtr"));
		mixin(loadFromSharedLib("sfImage_Bind"));
		mixin(loadFromSharedLib("sfImage_SetSmooth"));
		mixin(loadFromSharedLib("sfImage_GetWidth"));
		mixin(loadFromSharedLib("sfImage_GetHeight"));
		mixin(loadFromSharedLib("sfImage_IsSmooth"));
	}
}