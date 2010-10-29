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

module dsfml.window.event;

/**
*	Definition of key codes for keyboard events
*	
*	$(B Possibles values:)$(BR)
*		Except letters and numbers, you can use :$(BR)
*			* LCONTROL, LSHIFT, LALT, LSYSTEM, RCONTROL, RSHIFT, RALT, RSYSTEM.$(BR)
*			* LBRACKET, RBRACKET, SEMICOLON, COMMA, PERIOD, QUOTE, SLASH, BACKSLASH, TILDE, EQUAL, DASH.$(BR)
*			* SPACE, RETURN, BACK, TAB, PAGEUP, PAGEDOWN, END, HOME, INSERT, DELETE.$(BR)
*			* ADD, SUBTRACT, MULTIPLY, DIVIDE, LEFT, RIGHT, UP, DOWN.$(BR)
*			* Numpad0, Numpad1, Numpad2, Numpad3, Numpad4, Numpad5, Numpad6, Numpad7, Numpad8, Numpad9.$(BR)
*			* F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12, F13, F14, F15.$(BR)
*/
enum KeyCode
{
		A = 'a',
		B = 'b',
		C = 'c',
		D = 'd',
		E = 'e',
		F = 'f',
		G = 'g',
		H = 'h',
		I = 'i',
		J = 'j',
		K = 'k',
		L = 'l',
		M = 'm',
		N = 'n',
		O = 'o',
		P = 'p',
		Q = 'q',
		R = 'r',
		S = 's',
		T = 't',
		U = 'u',
		V = 'v',
		W = 'w',
		X = 'x',
		Y = 'y',
		Z = 'z',
		Num0 = '0',
		Num1 = '1',
		Num2 = '2',
		Num3 = '3',
		Num4 = '4',
		Num5 = '5',
		Num6 = '6',
		Num7 = '7',
		Num8 = '8',
		Num9 = '9',
		Escape = 256,
		LControl,
		LShift,
		LAlt,
		LSystem,
		RControl,
		RShist,
		RAlt,
		RSystem,
		Menu,
		LBracket,
		RBracket,
		Semicolon,
		Comma,
		Period,
		Quote,
		Slash,
		Backslash,
		Tilde,
		Equal,
		Dash,
		Space,
		Return,
		Back,
		Tab,
		PageUp,
		PageDown,
		End,
		Home,
		Insert,
		Delete,
		Add,
		Subtract,
		Multiply,
		Divide,
		Left,
		Right,
		Up,
		Down,
		Numpad0,
		Numpad1,
		Numpad2,
		Numpad3,
		Numpad4,
		Numpad5,
		Numpad6,
		Numpad7,
		Numpad8,
		Numpad9,
		F1,
		F2,
		F3,
		F4,
		F5,
		F6,
		F7,
		F8,
		F9,
		F10,
		F11,
		F12,
		F13,
		F14,
		F15,
		Pause,
}


/**
*	Definition of button codes for mouse events 
*/
enum MouseButtons
{
	Left, ///
	Right, ///
	Middle, /// 
	XButton1, ///
	XButton2 ///
}



/**
*	Definition of joystick axis for joystick events
*/
enum JoyAxis
{
	AxisX, ///
	AxisY, ///
	AxisZ, ///
	AxisR, ///
	AxisU, ///
	AxisV, ///
	AxisPOV ///
}


/// EventType
enum EventType
{
	Closed,
	Resized,
	LostFocus,
	GainedFocus,
	TextEntered,
	KeyPressed,
	KeyReleased,
	MouseWheelMoved,
	MouseButtonPressed,
	MouseButtonReleased,
	MouseMoved,
	MouseEntered,
	MouseLeft,
	JoyButtonPressed,
	JoyButtonReleased,
	JoyMoved

}

/**
*	Event defines a system event and its parameters
*/
align(1) struct Event
{
	/**
	*	Enumeration of the different types of events. Accessing a value of another event that the one received (e.g. Event.Size.Width when receiving an KEYPRESSED event) will result in undefined behavior.
	*		$(UL
	*			$(LI CLOSED)  
	*			$(LI LOSTFOCUS)			  
	*			$(LI GAINEDFOCUS)
	*			$(LI RESIZED
	*				$(UL
	*					$(LI	Event.Size.Width : new Width, in pixels.)
	*					$(LI	Event.Size.Height : new height, in pixels.)	
	*				)					
	*			)		  
	*			$(LI TEXTENTERED
	*				$(UL
	*					$(LI	Event.Text.Unicode : dchar entered.)
	*				)	
	*			)			
	*			$(LI KEYPRESSED, KEYRELEASED
	*				$(UL
	*					$(LI	Event.Key.Code	  : Key code of the key.)
	*					$(LI	Event.Key.Alt		: Alt pressed ?)
	*					$(LI	Event.Key.Control	: Control pressed ?)
	*					$(LI	Event.Key.Shift	 : Shift pressed ?)
	*				)	
	*			)			  
	*			$(LI MOUSEWHEELMOVED
	*				$(UL
	*					$(LI	Event.MouseWheel.Delta : Wheel move (positive if forward, negative else.) ) 
	*				)	
	*			)			
	*			$(LI MOUSEBUTTONPRESSED, MOUSEBUTTONRELEASED
	*				$(UL
	*					$(LI	Event.MouseButton.Button : Mouse button pressed.)
	*					$(LI	Event.MouseButton.X : Cursor X position.)  
	*					$(LI	Event.MouseButton.Y : Cursor X position.)			  
	*				)	
	*			)			  
	*			$(LI MOUSEMOVED
	*				$(UL
	*					$(LI	Event.MouseMove.X	: Cursor X position. Local coordinates.)
	*					$(LI	Event.MouseMove.Y	: Cursor Y position. Local coordinates.)		
	*				)	
	*			)
	*			$(LI MOUSEENTERED)
	*			$(LI MOUSELEFT)					
	*			$(LI JOYBUTTONPRESSED, JOYBUTTONRELEASED
	*				$(UL
	*					$(LI	Event.JoyButton.JoystickId  : Id of the joystick.)	
	*					$(LI	Event.JoyButton.Button	  : Joystick button pressed.)	
	*				)
	*			)	
	*			$(LI JOYMOVED
	*				$(UL
	*					$(LI	Event.JoyMove.JoystickId	: Id of the joystick.)
	*					$(LI	Event.JoyMove.Axis		  : Moved axis.)
	*					$(LI	Event.JoyMove.Position	  : Actual position of the axis [-100, 100], except for POV [0, 360].)			
	*				)
	*			)								 
	*		)			  
	*/	
	EventType Type;
	
	union
	{
		struct SText
		{
			dchar Unicode;
		}		
		SText Text;
		
		struct SKey
		{
		align(4): // cause bool is size 1
			KeyCode Code;
			bool	Alt;
			bool	Control;
			bool	Shift;
		}
		SKey Key;
	
		struct SMouseMove
		{
			int X;
			int Y;
		}
		SMouseMove MouseMove;
		
		struct SMouseButton
		{
			MouseButtons Button;
			int X;
			int Y;
		}
		SMouseButton MouseButton;
		
		struct SMouseWheel
		{
			int Delta;
		}
		SMouseWheel MouseWheel;
		
		struct SJoyMove
		{
			uint	JoystickId;
			JoyAxis Axis;
			float	Position;
		}
		SJoyMove JoyMove;
		
		struct SJoyButton
		{
			uint JoystickId;
			uint Button;
		}
		SJoyButton JoyButton;
		
		struct SSize
		{
			uint Width;
			uint Height;
		}
		SSize Size;
	}
}