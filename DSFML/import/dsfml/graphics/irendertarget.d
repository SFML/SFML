/**
 *	
 */
module dsfml.graphics.irendertarget;

import dsfml.system.vector2;
import dsfml.graphics.idrawable;
import dsfml.graphics.rect;
import dsfml.graphics.shader;
import dsfml.graphics.view;
import dsfml.graphics.color;

interface IRenderTarget
{
	/*
	 * 	Clear the entire target with a single color
	 *
	 * 	\param color : Color to use to clear the render target
	 *
	*/
	void clear(Color color = Color());

	/*
	 * 	Draw something into the target
	 *
	 *	\param object : Object to draw
	 *
	*/
	void draw(IDrawable object);

	/*
	 *	Draw something into the target with a shader
	 *
	 * 	\param object : Object to draw
	 * 	\param shader : Shader to apply
	 *
	*/
	void draw(IDrawable object, Shader shader);

	/*
	 * 	Get the width of the rendering region of the target
	 *
	 * 	\return Width in pixels
	 *
	*/
	uint getWidth();

	/*
	 * 	Get the height of the rendering region of the target
	 *
	 * 	\return Height in pixels
	 *
	*/
	uint getHeight();

	/*
	 * 	Change the current active view.
	 *
	 * 	\param view : New view to use (pass GetDefaultView() to set the default view)
	 *
	*/
	void setView(View view);

	/*
	 * 	Get the current view
	 *
	 * 	\return Current view active in the window
	 *
	*/
	View getView();

	/*
	 * 	Get the default view of the window
	 *
	 * 	\return Default view
	 *
	*/
	View getDefaultView();

	/*
	 * 	Get the viewport of a view applied to this target
	 *
	 * 	\param view Target view
	 *
	 * 	\return Viewport rectangle, expressed in pixels in the current target
	 *
	*/
	IntRect getViewport(View view);



	/*
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

	/*
	 * 	Save the current OpenGL render states and matrices
	 *
	*/
	void saveGLStates();

	/*
	 * 	Restore the previously saved OpenGL render states and matrices
	 *
	*/
	void restoreGLStates();
}