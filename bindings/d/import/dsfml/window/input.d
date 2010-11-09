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

module dsfml.window.input;

import dsfml.system.common;

import dsfml.window.event;

/**
 *	Input handles real-time input from keyboard and mouse.
 *	Use it instead of events to handle continuous moves and more
 *	game-friendly inputs
 */
class Input : DSFMLObject
{
public: // TODO: try to fix this, doesn't work with package
	this(SFMLClass input)
	{
		super(input, true);
	}

	override void dispose()
	{
		// nothing to do
	}

public:
	/**
	 *	Get the state of a key
	 *
	 *	Params:
	 *		key = Key to check
	 *
	 *	Returns:
	 *		True if key is down, false if key is up
	 */
	bool isKeyDown(KeyCode key)
	{
		return cast(bool)sfInput_IsKeyDown(m_ptr, key);
	}

	/**
	 *	Get the state of a mouse button
	 *
	 *	Params: 
	 *		button = Button to check
	 *
	 *	Returns: 
	 *		True if button is down, false if button is up
	 */
	bool isMouseButtonDown(MouseButtons button)
	{
		return cast(bool)sfInput_IsMouseButtonDown(m_ptr, button);
	}

	/**
	 *	Get the state of a joystick button
	 *
	 *	Params: 
	 *		joyId = Identifier of the joystick to check (0 or 1)
	 *		button = Button to check
	 *
	 *	Returns: 
	 *		True if button is down, false if button is up
	 */
	bool isJoystickButtonDown(uint joyId, uint button)
	{
		return cast(bool)sfInput_IsJoystickButtonDown(m_ptr, joyId, button);
	}

	/**
	 * Get a joystick axis position
	 *
	 *	Params:	
	 *		joyId = Identifier of the joystick to check (0 or 1)
	 *		axis =  Axis to get
	 *
	 *	Returns:
	 *		Current axis position, in the range [-100, 100] (except for POV, which is [0, 360])
	 */
	float getJoystickAxis(uint joyId, JoyAxis axis)
	{
		return sfInput_GetJoystickAxis(m_ptr, joyId, axis);
	}

@property
{
	/**
	 *	Get the mouse X position
	 *
	 *	Returns:
	 *		Current mouse left position, relative to owner window
	 */
	int mouseX()
	{
		return sfInput_GetMouseX(m_ptr);
	}

	/**
	 *	Get the mouse Y position
	 *
	 *	Returns:
	 *		Current mouse top position, relative to owner window
	 *
	 */
	int mouseY()
	{
		return sfInput_GetMouseY(m_ptr);
	}
}

private:

// External ====================================================================

	static extern (C)
	{
		int		function(SFMLClass, KeyCode)		sfInput_IsKeyDown;
		int		function(SFMLClass, MouseButtons)	sfInput_IsMouseButtonDown;
		int		function(SFMLClass, uint, uint)		sfInput_IsJoystickButtonDown;
		int		function(SFMLClass)					sfInput_GetMouseX;
		int		function(SFMLClass)					sfInput_GetMouseY;
		float	function(SFMLClass, uint, JoyAxis)	sfInput_GetJoystickAxis;
	}

	mixin(loadFromSharedLib2("csfml-window", "sfInput",
	"IsKeyDown", "IsMouseButtonDown", "IsJoystickButtonDown", "GetMouseX", "GetMouseY", "GetJoystickAxis"));
}