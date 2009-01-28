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

module dsfml.graphics.postfx;

import dsfml.graphics.image;

import dsfml.system.common;
import dsfml.system.exception;
import dsfml.system.stringutil;


/**
*   Define loading methods for effect
*/
enum LoadingType
{
    FROMFILE,   /// string represents a file path
    FROMSTRING  /// string represents effect code
}

/**
*   PostFX is used to apply a post effect to a window
* 
*   See_Also:
*   $(LINK2 http://www.sfml-dev.org/tutorials/graphics-postfx.php, SFML post FX tutorial) from more informations about Post effects and GLSL fragment shaders syntax.
*/
class PostFX : DSFMLObject
{
    /**
    *   construct the effect
    *
    *   Params: 
    *       effect = Path of a file or string containing the effect.
    *       type = type of the effect (default is FROMFILE)    
    */
    this(char[] effect, LoadingType type = LoadingType.FROMFILE)
	{
        if (effect is null || effect.length == 0)
	       throw new LoadingException("LoadingException : Effect is invalid.");
        
        if (type == LoadingType.FROMFILE)
            super(sfPostFX_CreateFromFile(toStringz(effect)));
        else
            super(sfPostFX_CreateFromMemory(toStringz(effect)));
	}

    override void dispose()
	{
		sfPostFX_Destroy(m_ptr);
	}

    /**
    *   Change parameters of the effect           
    *
    *   Params:    
    *       name = Parameter name in the effect
    */
    void setParameter(char[] name, float x)
	{
		sfPostFX_SetParameter1(m_ptr, toStringz(name), x);
	}

    /**
    *   ditto
    */           
    void setParameter(char[] name, float x, float y)
	{
		sfPostFX_SetParameter2(m_ptr, toStringz(name), x, y);
	}

    /**
    *   ditto
    */           
    void setParameter(char[] name, float x, float y, float z)
	{
		sfPostFX_SetParameter3(m_ptr, toStringz(name), x, y, z);
	}

    /**
    *   ditto
    */           
    void setParameter(char[] name, float x, float y, float z, float w)
	{
		sfPostFX_SetParameter4(m_ptr, toStringz(name), x, y, z, w);
	}

    /**
    *   Set a texture parameter
    *
    *   Params: 
    *       name = Texture name in the effect
    *       texture = Image to set (pass NULL to use content of current framebuffer)
    */
    void setTexture(char[] name, Image texture)
	{
        m_texture = texture;
		sfPostFX_SetTexture(m_ptr, toStringz(name), texture is null ? null : texture.getNativePointer);
	}

    /**
    *   Tell whether or not the system supports post-effects
    *
    *   Returns:
    *       True if the system can use post-effects
    */
    static bool canUsePostFX()
	{
		return cast(bool)sfPostFX_CanUsePostFX();
	}


private:
	Image m_texture;
	
	extern (C)
	{
        typedef void* function(char*) pf_sfPostFX_CreateFromFile;
    	typedef void* function(char*) pf_sfPostFX_CreateFromMemory;
    	typedef void function(void*) pf_sfPostFX_Destroy;
    	typedef void function(void*, char*, float) pf_sfPostFX_SetParameter1;
    	typedef void function(void*, char*, float, float) pf_sfPostFX_SetParameter2;
    	typedef void function(void*, char*, float, float, float) pf_sfPostFX_SetParameter3;
    	typedef void function(void*, char*, float, float, float, float) pf_sfPostFX_SetParameter4;
    	typedef void function(void*, char*, void*) pf_sfPostFX_SetTexture;
    	typedef int function() pf_sfPostFX_CanUsePostFX;
    
    	static pf_sfPostFX_CreateFromFile sfPostFX_CreateFromFile;
    	static pf_sfPostFX_CreateFromMemory sfPostFX_CreateFromMemory;
    	static pf_sfPostFX_Destroy sfPostFX_Destroy;
    	static pf_sfPostFX_SetParameter1 sfPostFX_SetParameter1;
    	static pf_sfPostFX_SetParameter2 sfPostFX_SetParameter2;
    	static pf_sfPostFX_SetParameter3 sfPostFX_SetParameter3;
    	static pf_sfPostFX_SetParameter4 sfPostFX_SetParameter4;
    	static pf_sfPostFX_SetTexture sfPostFX_SetTexture;
    	static pf_sfPostFX_CanUsePostFX sfPostFX_CanUsePostFX;
    }

    static this()
    {
        DllLoader dll = DllLoader.load("csfml-graphics");
        
        sfPostFX_CreateFromFile = cast(pf_sfPostFX_CreateFromFile)dll.getSymbol("sfPostFX_CreateFromFile");
        sfPostFX_CreateFromMemory = cast(pf_sfPostFX_CreateFromMemory)dll.getSymbol("sfPostFX_CreateFromMemory");
        sfPostFX_Destroy = cast(pf_sfPostFX_Destroy)dll.getSymbol("sfPostFX_Destroy");
        sfPostFX_SetParameter1 = cast(pf_sfPostFX_SetParameter1)dll.getSymbol("sfPostFX_SetParameter1");
        sfPostFX_SetParameter2 = cast(pf_sfPostFX_SetParameter2)dll.getSymbol("sfPostFX_SetParameter2");
        sfPostFX_SetParameter3 = cast(pf_sfPostFX_SetParameter3)dll.getSymbol("sfPostFX_SetParameter3");
        sfPostFX_SetParameter4 = cast(pf_sfPostFX_SetParameter4)dll.getSymbol("sfPostFX_SetParameter4");
        sfPostFX_SetTexture = cast(pf_sfPostFX_SetTexture)dll.getSymbol("sfPostFX_SetTexture");
        sfPostFX_CanUsePostFX = cast(pf_sfPostFX_CanUsePostFX)dll.getSymbol("sfPostFX_CanUsePostFX");
    }	
}
