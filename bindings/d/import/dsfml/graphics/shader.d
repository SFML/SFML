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

module dsfml.graphics.shader;

import dsfml.graphics.image;

import dsfml.system.common;
import dsfml.system.exception;
import dsfml.system.stringutil;


/**
*	Define loading methods for effect
*/
enum LoadingType
{
	FROMFILE,	/// string represents a file path
	FROMSTRING  /// string represents effect code
}

/**
*	Shader is used to apply a post effect to a window
* 
*	See_Also:
*	$(LINK2 http://www.sfml-dev.org/tutorials/graphics-postfx.php, SFML post FX tutorial) from more informations about Post effects and GLSL fragment shaders syntax.
*/
class Shader : DSFMLObject
{
private:
	Image m_texture;

public:

	/**
	*	construct the effect
	*
	*	Params: 
	*		effect = Path of a file or string containing the effect.
	*		type = type of the effect (default is FROMFILE)	
	*/
	this(string effect, LoadingType type = LoadingType.FROMFILE)
	{
		if (effect is null || effect.length == 0)
			throw new LoadingException("LoadingException : Effect is invalid.");
		
		if (type == LoadingType.FROMFILE)
			super(sfShader_CreateFromFile(toStringz(effect)));
		else
			super(sfShader_CreateFromMemory(toStringz(effect)));
	}

	override void dispose()
	{
		sfShader_Destroy(m_ptr);
	}

	/**
	*	Change parameters of the effect			
	*
	*	Params:	
	*		name = Parameter name in the effect
	*/
	void setParameter(string name, float x)
	{
		sfShader_SetParameter1(m_ptr, toStringz(name), x);
	}

	/**
	*	ditto
	*/			
	void setParameter(string name, float x, float y)
	{
		sfShader_SetParameter2(m_ptr, toStringz(name), x, y);
	}

	/**
	*	ditto
	*/			
	void setParameter(string name, float x, float y, float z)
	{
		sfShader_SetParameter3(m_ptr, toStringz(name), x, y, z);
	}

	/**
	*	ditto
	*/			
	void setParameter(string name, float x, float y, float z, float w)
	{
		sfShader_SetParameter4(m_ptr, toStringz(name), x, y, z, w);
	}

	/**
	*	Set a texture parameter
	*
	*	Params: 
	*		name = Texture name in the effect
	*		texture = Image to set (pass NULL to use content of current framebuffer)
	*/
	void setTexture(string name, Image texture)
	{
		m_texture = texture;
		sfShader_SetTexture(m_ptr, toStringz(name), texture is null ? null : texture.nativePointer);
	}

	/**
	*	Tell whether or not the system supports shaders
	*
	*	Returns:
	*		True if the system can use shaders
	*/
	static bool isAvailable()
	{
		return cast(bool)sfShader_IsAvailable();
	}


private:
	
	static extern(C)
	{
		SFMLClass	function(cchar*)									sfShader_CreateFromFile;
		SFMLClass	function(cchar*)									sfShader_CreateFromMemory;
		void	function(SFMLClass)										sfShader_Destroy;
		void	function(SFMLClass, cchar*, float)						sfShader_SetParameter1;
		void	function(SFMLClass, cchar*, float, float)				sfShader_SetParameter2;
		void	function(SFMLClass, cchar*, float, float, float)		sfShader_SetParameter3;
		void	function(SFMLClass, cchar*, float, float, float, float)	sfShader_SetParameter4;
		void	function(SFMLClass, cchar*, SFMLClass)						sfShader_SetTexture;
		int		function()											sfShader_IsAvailable;
		void	function(SFMLClass)										sfShader_Bind;
		void	function(SFMLClass)										sfShader_Unbind;
	}
	
	static this()
	{
	debug
		DllLoader dll = DllLoader.load("csfml-graphics-d-2");
	else
		DllLoader dll = DllLoader.load("csfml-graphics-2");

		mixin(loadFromSharedLib("sfShader_CreateFromFile"));
		mixin(loadFromSharedLib("sfShader_CreateFromMemory"));
		mixin(loadFromSharedLib("sfShader_Destroy"));
		mixin(loadFromSharedLib("sfShader_SetParameter1"));
		mixin(loadFromSharedLib("sfShader_SetParameter2"));
		mixin(loadFromSharedLib("sfShader_SetParameter3"));
		mixin(loadFromSharedLib("sfShader_SetParameter4"));
		mixin(loadFromSharedLib("sfShader_SetTexture"));
		mixin(loadFromSharedLib("sfShader_IsAvailable"));
		mixin(loadFromSharedLib("sfShader_Bind"));
		mixin(loadFromSharedLib("sfShader_Unbind"));
	}
}