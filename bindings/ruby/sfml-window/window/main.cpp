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
#include "Vector2.hpp"
#include "NonCopyable.hpp"
#include "Context.hpp"
#include "ContextSettings.hpp"
#include "Event.hpp"
#include "Input.hpp"
#include "VideoMode.hpp"
#include "Window.hpp"

#include <SFML/Window.hpp>

#include <iostream>

/* SFML Namespace which contains all classes in this module. */
VALUE globalKeyNamespace;
VALUE globalMouseNamespace;
VALUE globalJoyNamespace;
VALUE globalStyleNamespace;
extern VALUE globalVector2Class;
extern VALUE globalNonCopyableModule;

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

/* Definition of key codes for keyboard events. 
 *
 * All SFML::Key constants exists, I just haven't written them all so Rdoc can interpret them yet.
 */
static void CreateKeyEnum( void )
{
	globalKeyNamespace = rb_define_module_under( globalSFMLNamespace, "Key" );
	rb_define_const( globalKeyNamespace, "A", INT2FIX( sf::Key::A ) );
	rb_define_const( globalKeyNamespace, "B", INT2FIX( sf::Key::B ) );
	rb_define_const( globalKeyNamespace, "C", INT2FIX( sf::Key::C ) );
	rb_define_const( globalKeyNamespace, "D", INT2FIX( sf::Key::D ) );
	rb_define_const( globalKeyNamespace, "E", INT2FIX( sf::Key::E ) );
	rb_define_const( globalKeyNamespace, "F", INT2FIX( sf::Key::F ) );
	rb_define_const( globalKeyNamespace, "G", INT2FIX( sf::Key::G ) );
	rb_define_const( globalKeyNamespace, "H", INT2FIX( sf::Key::H ) );
	rb_define_const( globalKeyNamespace, "I", INT2FIX( sf::Key::I ) );
	rb_define_const( globalKeyNamespace, "J", INT2FIX( sf::Key::J ) );
	rb_define_const( globalKeyNamespace, "K", INT2FIX( sf::Key::K ) );
	rb_define_const( globalKeyNamespace, "L", INT2FIX( sf::Key::L ) );
	rb_define_const( globalKeyNamespace, "M", INT2FIX( sf::Key::M ) );
	rb_define_const( globalKeyNamespace, "N", INT2FIX( sf::Key::N ) );
	rb_define_const( globalKeyNamespace, "O", INT2FIX( sf::Key::O ) );
	rb_define_const( globalKeyNamespace, "P", INT2FIX( sf::Key::P ) );
	rb_define_const( globalKeyNamespace, "Q", INT2FIX( sf::Key::Q ) );
	rb_define_const( globalKeyNamespace, "R", INT2FIX( sf::Key::R ) );
	rb_define_const( globalKeyNamespace, "S", INT2FIX( sf::Key::S ) );
	rb_define_const( globalKeyNamespace, "T", INT2FIX( sf::Key::T ) );
	rb_define_const( globalKeyNamespace, "U", INT2FIX( sf::Key::U ) );
  rb_define_const( globalKeyNamespace, "W", INT2FIX( sf::Key::W ) );
	rb_define_const( globalKeyNamespace, "V", INT2FIX( sf::Key::V ) );
	rb_define_const( globalKeyNamespace, "X", INT2FIX( sf::Key::X ) );
	rb_define_const( globalKeyNamespace, "Y", INT2FIX( sf::Key::Y ) );
	rb_define_const( globalKeyNamespace, "Z", INT2FIX( sf::Key::Z ) );
	
	rb_define_const( globalKeyNamespace, "Num0", INT2FIX( sf::Key::Num0 ) );
	rb_define_const( globalKeyNamespace, "Num1", INT2FIX( sf::Key::Num1 ) );
	rb_define_const( globalKeyNamespace, "Num2", INT2FIX( sf::Key::Num2 ) );
	rb_define_const( globalKeyNamespace, "Num3", INT2FIX( sf::Key::Num3 ) );
	rb_define_const( globalKeyNamespace, "Num4", INT2FIX( sf::Key::Num4 ) );
	rb_define_const( globalKeyNamespace, "Num5", INT2FIX( sf::Key::Num5 ) );
	rb_define_const( globalKeyNamespace, "Num6", INT2FIX( sf::Key::Num6 ) );
	rb_define_const( globalKeyNamespace, "Num7", INT2FIX( sf::Key::Num7 ) );
	rb_define_const( globalKeyNamespace, "Num8", INT2FIX( sf::Key::Num8 ) );
	rb_define_const( globalKeyNamespace, "Num9", INT2FIX( sf::Key::Num9 ) );
	
	for( int index = static_cast< int >( sf::Key::Escape ); index <= sf::Key::Count; index++ )
	{
		rb_define_const( globalKeyNamespace, keyNamesMisc[ index - sf::Key::Escape ], INT2FIX( index ) );
	}
}

/* Definition of button codes for mouse events. */
static void CreateMouseEnum( void )
{
	globalMouseNamespace = rb_define_module_under( globalSFMLNamespace, "Mouse" );
	rb_define_const( globalMouseNamespace, "Left", INT2FIX( sf::Mouse::Left ) );
	rb_define_const( globalMouseNamespace, "Right", INT2FIX( sf::Mouse::Right ) );
	rb_define_const( globalMouseNamespace, "Middle", INT2FIX( sf::Mouse::Middle ) );
	rb_define_const( globalMouseNamespace, "XButton1", INT2FIX( sf::Mouse::XButton1 ) );
	rb_define_const( globalMouseNamespace, "XButton2", INT2FIX( sf::Mouse::XButton2 ) );
	rb_define_const( globalMouseNamespace, "ButtonCount", INT2FIX( sf::Mouse::ButtonCount ) );
}

/* Definition of joystick axis for joystick events. */
static void CreateJoyEnum( void )
{
	globalJoyNamespace = rb_define_module_under( globalSFMLNamespace, "Joy" );
	rb_define_const( globalJoyNamespace, "AxisX", INT2FIX( sf::Joy::AxisX ) );
	rb_define_const( globalJoyNamespace, "AxisY", INT2FIX( sf::Joy::AxisY ) );
	rb_define_const( globalJoyNamespace, "AxisZ", INT2FIX( sf::Joy::AxisZ ) );
	rb_define_const( globalJoyNamespace, "AxisR", INT2FIX( sf::Joy::AxisR ) );
	rb_define_const( globalJoyNamespace, "AxisU", INT2FIX( sf::Joy::AxisU ) );
	rb_define_const( globalJoyNamespace, "AxisV", INT2FIX( sf::Joy::AxisV ) );
	rb_define_const( globalJoyNamespace, "AxisPOV", INT2FIX( sf::Joy::AxisPOV ) );
	rb_define_const( globalJoyNamespace, "AxisCount", INT2FIX( sf::Joy::AxisCount ) );
}

/* Enumeration of the window styles. */
static void CreateStyleEnum( void )
{
	globalStyleNamespace = rb_define_module_under( globalSFMLNamespace, "Style" );
	rb_define_const( globalStyleNamespace, "None", INT2FIX( sf::Style::None ) );
	rb_define_const( globalStyleNamespace, "Titlebar", INT2FIX( sf::Style::Titlebar ) );
	rb_define_const( globalStyleNamespace, "Resize", INT2FIX( sf::Style::Resize ) );
	rb_define_const( globalStyleNamespace, "Close", INT2FIX( sf::Style::Close ) );
	rb_define_const( globalStyleNamespace, "Fullscreen", INT2FIX( sf::Style::Fullscreen ) );
	rb_define_const( globalStyleNamespace, "Default", INT2FIX( sf::Style::Default ) );
}

static bool CheckDependencies( void )
{
	if( rb_cvar_defined( globalSFMLNamespace, rb_intern( "SystemLoaded" ) ) == Qtrue )
	{
		return true;
	}

	return false;
}

void Init_window( void )
{
	/* SFML namespace which contains the classes of this module. */
	globalSFMLNamespace = rb_define_module( "SFML" );
	if( CheckDependencies() == false )
	{
		rb_raise( rb_eRuntimeError, "This module depends on sfml-system" );
	}
	
	globalVector2Class = rb_define_class_under(globalSFMLNamespace, "Vector2", rb_cObject );
	globalNonCopyableModule = rb_define_module_under(globalSFMLNamespace, "NonCopyable");
	
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

