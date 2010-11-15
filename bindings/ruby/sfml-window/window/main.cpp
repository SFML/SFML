/* rbSFML - Copyright (c) 2010 Henrik Valter Vogelius Hansson - groogy@groogy.se
 * This software is provided 'as-is', without any express or
 * implied warranty. In no event will the authors be held
 * liable for any damages arising from the use of this software.
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute
 * it freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented;
 *    you must not claim that you wrote the original software.
 *    If you use this software in a product, an acknowledgment
 *    in the product documentation would be appreciated but
 *    is not required.
 *
 * 2. Altered source versions must be plainly marked as such,
 *    and must not be misrepresented as being the original software.
 *
 * 3. This notice may not be removed or altered from any
 *    source distribution.
 */
 
#include "main.hpp"
#include "Context.hpp"
#include "ContextSettings.hpp"
#include "Event.hpp"
#include "Input.hpp"
#include "VideoMode.hpp"
#include "Window.hpp"

#include <SFML/Window.hpp>

#include <iostream>

VALUE globalSFMLNamespace;
VALUE globalKeyNamespace;
VALUE globalMouseNamespace;
VALUE globalJoyNamespace;
VALUE globalStyleNamespace;

VALUE GetNamespace( void )
{
	return globalSFMLNamespace;
}

static const char * keyNamesLetters[] =
{
	"A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M", "N", "O",
	"P", "Q", "R", "S", "T", "U", "V", "W", "X", "Y", "Z"
};

static const char * keyNamesNum[] =
{
	"Num0", "Num1", "Num2", "Num3", "Num4", "Num5", "Num6", "Num7", "Num8", "Num9"
};

static const char * keyNamesMisc[] =
{
	"Escape", "LControl", "LShift", "LAlt", "LSystem", "RControl", "RShift", "RAlt", "RSystem", 
	"Menu", "LBracket", "RBracket", "SemiColon", "Comma", "Period", "Quote", "Slash", 
	"BackSlash", "Tilde", "Equal", "Dash", "Space", "Return", "Back", "Tab", "PageUp", 
	"PageDown", "End", "Home", "Insert", "Delete", "Add", "Subtract", "Multiply", 
	"Divide", "Left", "Right", "Up", "Down", "Numpad0", "Numpad1", "Numpad2", "Numpad3", 
	"Numpad4", "Numpad5", "Numpad6", "Numpad7", "Numpad8", "Numpad9", "F1", "F2", "F3", 
	"F4", "F5", "F6", "F7", "F8", "F9", "F10", "F11", "F12", "F13", "F14", "F15", "Pause",
	"Count"
};

static const char * mouseNames[] =
{
	"Left", "Right", "Middle", "XButton1", "XButton2", "ButtonCount"
};

static const char * axisNames[] =
{
	"AxisX", "AxisY", "AxisZ", "AxisR", "AxisU", "AxisV", "AxisPOV",
	"AxisCount"
};

void CreateKeyEnum( void )
{
	globalKeyNamespace = rb_define_module_under( globalSFMLNamespace, "Key" );
	for( int index = static_cast< int >( sf::Key::A ); index <= sf::Key::Z; index++ )
	{
		rb_define_const( globalKeyNamespace, keyNamesLetters[ index - sf::Key::A ], INT2FIX( index ) );
	}
	
	for( int index = static_cast< int >( sf::Key::Num0 ); index <= sf::Key::Num9; index++ )
	{
		rb_define_const( globalKeyNamespace, keyNamesNum[ index - sf::Key::Num0 ], INT2FIX( index ) );
	}
	
	for( int index = static_cast< int >( sf::Key::Escape ); index <= sf::Key::Count; index++ )
	{
		rb_define_const( globalKeyNamespace, keyNamesMisc[ index - sf::Key::Escape ], INT2FIX( index ) );
	}
}

void CreateMouseEnum( void )
{
	globalMouseNamespace = rb_define_module_under( globalSFMLNamespace, "Mouse" );
	for( int index = static_cast< int >( sf::Mouse::Left ); index <= sf::Mouse::ButtonCount; index++ )
	{
		rb_define_const( globalMouseNamespace, mouseNames[ index - sf::Mouse::Left ], INT2FIX( index ) );
	}
}

void CreateJoyEnum( void )
{
	globalJoyNamespace = rb_define_module_under( globalSFMLNamespace, "Joy" );
	for( int index = static_cast< int >( sf::Joy::AxisX ); index <= sf::Joy::AxisCount; index++ )
	{
		rb_define_const( globalJoyNamespace, axisNames[ index - sf::Joy::AxisX ], INT2FIX( index ) );
	}
}

void CreateStyleEnum( void )
{
	globalStyleNamespace = rb_define_module_under( globalSFMLNamespace, "Style" );
	rb_define_const( globalStyleNamespace, "None", sf::Style::None );
	rb_define_const( globalStyleNamespace, "Titlebar", sf::Style::Titlebar );
	rb_define_const( globalStyleNamespace, "Resize", sf::Style::Resize );
	rb_define_const( globalStyleNamespace, "Close", sf::Style::Close );
	rb_define_const( globalStyleNamespace, "Fullscreen", sf::Style::Fullscreen );
	rb_define_const( globalStyleNamespace, "Default", sf::Style::Default );
}

void Init_window( void )
{
	globalSFMLNamespace = rb_define_module( "SFML" );
	rb_define_const( globalSFMLNamespace, "WindowLoaded", Qtrue );
	
	CreateKeyEnum();
	CreateMouseEnum();
	CreateJoyEnum();
	CreateStyleEnum();
	
	Init_Context();
	Init_ContextSettings();
	Init_Event();
	Init_Input();
	Init_VideoMode();
	Init_Window();
}
