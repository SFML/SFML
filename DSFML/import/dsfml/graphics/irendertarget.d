/*
 *	DSFML - SFML Library wrapper for the D programming language.
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

module dsfml.graphics.irendertarget;

import dsfml.system.vector;
import dsfml.graphics.idrawable;
import dsfml.graphics.rect;
import dsfml.graphics.shader;
import dsfml.graphics.view;
import dsfml.graphics.color;

interface IRenderTarget
{
	/**
	 * 	Clear the entire target with a single color
	 *
	 * 	\param color : Color to use to clear the render target
	 *
	 */
	void clear(Color color = Color());

	/**
	 * 	Draw something into the target
	 *
	 *	\param object : Object to draw
	 *
	 */
	void draw(IDrawable object);

	/**
	 *	Draw something into the target with a shader
	 *
	 * 	\param object : Object to draw
	 * 	\param shader : Shader to apply
	 *
	 */
	void draw(IDrawable object, Shader shader);

	/**
	 * 	Convert a point in target coordinates into view coordinates
	 *
	 * 	\param x :	X coordinate of the point to convert, relative to the target
	 * 	\param y :	Y coordinate of the point to convert, relative to the target
	 * 	\param view : Target view to convert the point to, null to use the currently associated view
	 *
	 * 	\return Converted point
	 *
	 */
	Vector2f convertCoords(uint x, uint y, View view = null);

	/**
	 * 	Save the current OpenGL render states and matrices
	 *
	 */
	void saveGLStates();

	/**
	 * 	Restore the previously saved OpenGL render states and matrices
	 *
	 */
	void restoreGLStates();

@property
{
	/**
	 * 	Get the width of the rendering region of the target
	 *
	 * 	\return Width in pixels
	 *
	 */
	uint width();

	/**
	 * 	Get the height of the rendering region of the target
	 *
	 * 	\return Height in pixels
	 *
	 */
	uint height();

	/**
	 * 	Change the current active view.
	 *
	 * 	\param view : New view to use (pass GetDefaultView() to set the default view)
	 *
	 */
	void view(View view);

	/**
	 * 	Get the current view
	 *
	 * 	\return Current view active in the window
	 *
	 */
	View view();

	/**
	 * 	Get the default view of the window
	 *
	 * 	\return Default view
	 *
	 */
	View defaultView();

	/**
	 * 	Get the viewport of a view applied to this target
	 *
	 * 	\param view Target view
	 *
	 * 	\return Viewport rectangle, expressed in pixels in the current target
	 *
	 */
	IntRect viewport(View view);
}
}