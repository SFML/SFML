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

module dsfml.graphics.drawableimpl;

public import dsfml.system.common;
import dsfml.system.vector;

import dsfml.graphics.irendertarget;
import	dsfml.graphics.idrawable,
		dsfml.graphics.color,
		dsfml.graphics.blendmode,
		dsfml.graphics.renderwindow,
		dsfml.graphics.renderimage,
		dsfml.graphics.shader;


/*
 *	Package base class of all drawable.
 *	Provide implementation of IDrawable and functions aliases. 
 */
package class DrawableImpl(string derivedClassName) : DSFMLObject, IDrawable
{
protected:
	this()
	{
		super(sfDrawable_Create());
	}
	
	this(SFMLClass ptr)
	{
		super(ptr, true);
	}

	override void dispose()
	{
		sfDrawable_Destroy(m_ptr);
	}

public:
	override void rotate(float angle)
	{
		sfDrawable_Rotate(m_ptr, angle);
	}

	override void move(float offsetX, float offsetY)
	{
		sfDrawable_Move(m_ptr, offsetX, offsetY);
	}

	override void move(Vector2f offset)
	{
		sfDrawable_Move(m_ptr, offset.x, offset.y);
	}

	override Vector2f transformToLocal(Vector2f point) const
	{
		Vector2f ret;
		sfDrawable_TransformToLocal(m_ptr, point.x, point.y, &ret.x, &ret.y);
		return ret;
	}

	override Vector2f transformToGlobal(Vector2f point) const
	{
		Vector2f ret;
		sfDrawable_TransformToLocal(m_ptr, point.x, point.y, &ret.x, &ret.y);
		return ret;
	}

	override void render(IRenderTarget window)
	{
		sfRenderWindow_DrawThis((cast(DSFMLObject)window).nativePointer, m_ptr);
	}
	
	override void renderWithShader(IRenderTarget window, Shader shader)
	{
		sfRenderWindow_DrawThisWithShader((cast(DSFMLObject)window).nativePointer, m_ptr, shader.nativePointer);
	}

	override void setPosition(float x, float y)
	{
		sfDrawable_SetPosition(m_ptr, x, y);
	}

	override void setScale(float scaleX, float scaleY)
	{
		sfDrawable_SetScale(m_ptr, scaleX, scaleY);
	}
	
	override void setOrigin(float originX, float originY)
	{
		sfDrawable_SetOrigin(m_ptr, originX, originY);
	}

@property
{
	override void x(float x)
	{
		sfDrawable_SetX(m_ptr, x);
	}

	override void y(float y)
	{
		sfDrawable_SetY(m_ptr, y);
	}

	override void position(Vector2f vec)
	{
		sfDrawable_SetPosition(m_ptr, vec.x, vec.y);
	}

	override void scaleX(float scale)
	{
		if (scale > 0)
			sfDrawable_SetScaleX(m_ptr, scale);
	}
	
	override void scaleY(float scale)
	{
		if (scale > 0)
			sfDrawable_SetScaleY(m_ptr, scale);
	}

	override void scale(Vector2f scale)
	{
		if (scale.x > 0 && scale.y > 0)
			sfDrawable_SetScale(m_ptr, scale.x, scale.y);
	}

	override void origin(Vector2f origin)
	{
		sfDrawable_SetOrigin(m_ptr, origin.x, origin.y);
	}

	override void rotation(float angle)
	{
		sfDrawable_SetRotation(m_ptr, angle);
	}

	override void color(Color c)
	{
		sfDrawable_SetColor(m_ptr, c);
	}
			
	override void blendMode(BlendMode mode)
	{
		sfDrawable_SetBlendMode(m_ptr, mode);
	}

	override Vector2f position() const
	{
		return Vector2f(sfDrawable_GetX(m_ptr), sfDrawable_GetY(m_ptr));
	}

	override Vector2f scale() const
	{
		return Vector2f(sfDrawable_GetScaleX(m_ptr), sfDrawable_GetScaleY(m_ptr));
	}

	override Vector2f origin() const
	{
		return Vector2f(sfDrawable_GetOriginX(m_ptr),  sfDrawable_GetOriginY(m_ptr));
	}
	
	override float rotation() const
	{
		return sfDrawable_GetRotation(m_ptr);
	}

	override Color color() const
	{
		return sfDrawable_GetColor(m_ptr);
	}

	override BlendMode blendMode() const
	{
		return cast(BlendMode)(sfDrawable_GetBlendMode(m_ptr));
	}
	
	override void scale(Vector2f scale)
	{
		sfDrawable_SetScale(m_ptr, scale.x, scale.y);
	}
}

private:

	static extern(C)
	{
		SFMLClass	function()											sfDrawable_Create;
		void		function(SFMLClass)									sfDrawable_Destroy;
		void		function(SFMLClass, float)							sfDrawable_SetX;
		void		function(SFMLClass, float)							sfDrawable_SetY;
		void		function(SFMLClass, float, float)					sfDrawable_SetPosition;
		void		function(SFMLClass, float)							sfDrawable_SetScaleX;
		void		function(SFMLClass, float)							sfDrawable_SetScaleY;
		void		function(SFMLClass, float, float)					sfDrawable_SetScale;
		void		function(SFMLClass, float)							sfDrawable_SetRotation;
		void		function(SFMLClass, float, float)					sfDrawable_SetOrigin;
		void		function(SFMLClass, Color)							sfDrawable_SetColor;
		void		function(SFMLClass, BlendMode)						sfDrawable_SetBlendMode;
		float		function(SFMLClass)									sfDrawable_GetX;
		float		function(SFMLClass)									sfDrawable_GetY;
		float		function(SFMLClass)									sfDrawable_GetScaleX;
		float		function(SFMLClass)									sfDrawable_GetScaleY;
		float		function(SFMLClass)									sfDrawable_GetRotation;
		float		function(SFMLClass)									sfDrawable_GetOriginX;
		float		function(SFMLClass)									sfDrawable_GetOriginY;
		Color		function(SFMLClass)									sfDrawable_GetColor;
		BlendMode	function(SFMLClass)									sfDrawable_GetBlendMode;
		void		function(SFMLClass, float, float)					sfDrawable_Move;
		void		function(SFMLClass, float, float)					sfDrawable_Scale;
		void		function(SFMLClass, float)							sfDrawable_Rotate;
		void		function(SFMLClass, float, float, float*, float*)	sfDrawable_TransformToLocal;
		void		function(SFMLClass, float, float, float*, float*)	sfDrawable_TransformToGlobal;
		
		typedef void		function(SFMLClass, SFMLClass) pf_sfRenderWindow_DrawThis;
		typedef void		function(SFMLClass, SFMLClass, SFMLClass) pf_sfRenderWindow_DrawThisWithShader;
		typedef void		function(SFMLClass, SFMLClass) pf_sfRenderImage_DrawThis;
		typedef void		function(SFMLClass, SFMLClass, SFMLClass) pf_sfRenderImage_DrawThisWithShader;
		
		pf_sfRenderWindow_DrawThis sfRenderWindow_DrawThis;
		pf_sfRenderWindow_DrawThisWithShader sfRenderWindow_DrawThisWithShader;
		pf_sfRenderImage_DrawThis sfRenderImage_DrawThis;
		pf_sfRenderImage_DrawThisWithShader sfRenderImage_DrawThisWithShader;
	}

	mixin(loadDerivedFromSharedLib("csfml-graphics", "sfDrawable", derivedClassName,
	"Create", "Destroy", "SetX", "SetY", "SetPosition", "SetScaleX", "SetScaleY", "SetScale", "SetRotation", "SetOrigin", "SetColor", "SetBlendMode",
	"GetX", "GetY", "GetScaleX", "GetScaleY", "GetRotation", "GetOriginX", "GetOriginY", "GetColor", "GetBlendMode", "Move",
	"Scale", "Rotate", "TransformToLocal", "TransformToGlobal"));

	static this()
	{
		debug
			DllLoader dll = DllLoader.load("csfml-graphics-d-2");
		else
			DllLoader dll = DllLoader.load("csfml-graphics-2");
	
		sfRenderWindow_DrawThis = cast(pf_sfRenderWindow_DrawThis)dll.getSymbol("sfRenderWindow_Draw" ~ derivedClassName[2..$]);
		sfRenderWindow_DrawThisWithShader = cast(pf_sfRenderWindow_DrawThisWithShader)dll.getSymbol("sfRenderWindow_Draw" ~ derivedClassName[2..$] ~ "WithShader");
		sfRenderImage_DrawThis = cast(pf_sfRenderImage_DrawThis)dll.getSymbol("sfRenderImage_Draw" ~ derivedClassName[2..$]);
		sfRenderImage_DrawThisWithShader = cast(pf_sfRenderImage_DrawThisWithShader)dll.getSymbol("sfRenderImage_Draw" ~ derivedClassName[2..$] ~ "WithShader");
	}
}
