////////////////////////////////////////////////////////////
//
// RubySFML - Ruby extension for the SFML library
// Copyright (C) 2007 Sean O'Neil and Laurent Gomila
// (sean.p.oneil@gmail.com and laurent.gom@gmail.com)
//
// This software is provided 'as-is', without any express or implied warranty.
// In no event will the authors be held liable for any damages arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it freely,
// subject to the following restrictions:
//
// 1. The origin of this software must not be misrepresented;
//    you must not claim that you wrote the original software.
//    If you use this software in a product, an acknowledgment
//    in the product documentation would be appreciated but is not required.
//
// 2. Altered source versions must be plainly marked as such,
//    and must not be misrepresented as being the original software.
//
// 3. This notice may not be removed or altered from any source distribution.
//
////////////////////////////////////////////////////////////
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Window/OpenGLCaps.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "RubySFML.h"
using namespace sf;

VALUE NOARGV[1];
VALUE g_vModule;


/*
 * Returns true if the specified OpenGL extension is supported, false otherwise.
 */
VALUE SFML_checkExtension(VALUE vClass, VALUE vName) {
	return OpenGLCaps::CheckExtension(STR2CSTR(vName)) ? Qtrue : Qfalse;
}
 
/*
 * Returns the maximum texture size on the current display hardware.
 */
VALUE SFML_getMaxTextureSize() {
	return INT2NUM(OpenGLCaps::GetMaxTextureSize());
}
 
/*
 * Returns the maximum texture units on the current display hardware.
 */
VALUE SFML_getMaxTextureUnits() {
	return INT2NUM(OpenGLCaps::GetMaxTextureUnits());
}

extern "C" void Init_RubySFML() {
	g_vModule = rb_define_module("SFML");
	DEFINE_MODULE_METHOD(checkExtension, checkExtension, 1);
	DEFINE_MODULE_METHOD(checkExtension, getMaxTextureSize, 0);
	DEFINE_MODULE_METHOD(checkExtension, getMaxTextureUnits, 0);

	Init_Clock();
	Init_Color();
	Init_Drawable();
	Init_Event();
	Init_FloatRect();
	Init_Image();
	Init_Input();
	Init_IntRect();
	Init_Music();
	Init_PostFX();
	Init_RenderWindow();
	Init_Sound();
	Init_SoundBuffer();
	Init_SoundBufferRecorder();
	Init_Sprite();
	Init_String();
	Init_VideoMode();
	Init_View();
	Init_Window();

	// Window style constants
	VALUE vStyle = rb_define_module_under(g_vModule, "Style");
	rb_define_const(g_cWindow, "Resize", INT2FIX(Style::Resize));
	rb_define_const(g_cWindow, "Close", INT2FIX(Style::Close));
	rb_define_const(g_cWindow, "Fullscreen", INT2FIX(Style::Fullscreen));

	// Sound constants
	rb_define_const(g_cSound, "Stopped", INT2FIX(Sound::Stopped));
	rb_define_const(g_cSound, "Paused", INT2FIX(Sound::Paused));
	rb_define_const(g_cSound, "Playing", INT2FIX(Sound::Playing));

	// Event constants
	rb_define_const(g_cEvent, "Closed", INT2FIX(Event::Closed));
	rb_define_const(g_cEvent, "Resized", INT2FIX(Event::Resized));
	rb_define_const(g_cEvent, "LostFocus", INT2FIX(Event::LostFocus));
	rb_define_const(g_cEvent, "GainedFocus", INT2FIX(Event::GainedFocus));
	rb_define_const(g_cEvent, "TextEntered", INT2FIX(Event::TextEntered));
	rb_define_const(g_cEvent, "KeyPressed", INT2FIX(Event::KeyPressed));
	rb_define_const(g_cEvent, "KeyReleased", INT2FIX(Event::KeyReleased));
	rb_define_const(g_cEvent, "MouseWheelMoved", INT2FIX(Event::MouseWheelMoved));
	rb_define_const(g_cEvent, "MouseButtonPressed", INT2FIX(Event::MouseButtonPressed));
	rb_define_const(g_cEvent, "MouseButtonReleased", INT2FIX(Event::MouseButtonReleased));
	rb_define_const(g_cEvent, "MouseMoved", INT2FIX(Event::MouseMoved));
	rb_define_const(g_cEvent, "JoyButtonPressed", INT2FIX(Event::JoyButtonPressed));
	rb_define_const(g_cEvent, "JoyButtonReleased", INT2FIX(Event::JoyButtonReleased));
	rb_define_const(g_cEvent, "JoyMoved", INT2FIX(Event::JoyMoved));

	// Mouse constants
	VALUE vMouse = rb_define_module_under(g_vModule, "Mouse");
	rb_define_const(vMouse, "Left", INT2FIX(Mouse::Left));
	rb_define_const(vMouse, "Right", INT2FIX(Mouse::Right));
	rb_define_const(vMouse, "Middle", INT2FIX(Mouse::Middle));
	rb_define_const(vMouse, "XButton1", INT2FIX(Mouse::XButton1));
	rb_define_const(vMouse, "XButton2", INT2FIX(Mouse::XButton2));

	// Joystick constants
	VALUE vJoy = rb_define_module_under(g_vModule, "Joy");
	rb_define_const(vJoy, "AxisX", INT2FIX(Joy::AxisX));
	rb_define_const(vJoy, "AxisY", INT2FIX(Joy::AxisY));
	rb_define_const(vJoy, "AxisZ", INT2FIX(Joy::AxisZ));
	rb_define_const(vJoy, "AxisR", INT2FIX(Joy::AxisR));
	rb_define_const(vJoy, "AxisU", INT2FIX(Joy::AxisU));
	rb_define_const(vJoy, "AxisV", INT2FIX(Joy::AxisV));
	rb_define_const(vJoy, "AxisPOV", INT2FIX(Joy::AxisPOV));

	// Keyboard constants
	VALUE vKey = rb_define_module_under(g_vModule, "Key");
	rb_define_const(vKey, "A", INT2FIX(Key::A));
	rb_define_const(vKey, "B", INT2FIX(Key::B));
	rb_define_const(vKey, "C", INT2FIX(Key::C));
	rb_define_const(vKey, "D", INT2FIX(Key::D));
	rb_define_const(vKey, "E", INT2FIX(Key::E));
	rb_define_const(vKey, "F", INT2FIX(Key::F));
	rb_define_const(vKey, "G", INT2FIX(Key::G));
	rb_define_const(vKey, "H", INT2FIX(Key::H));
	rb_define_const(vKey, "I", INT2FIX(Key::I));
	rb_define_const(vKey, "J", INT2FIX(Key::J));
	rb_define_const(vKey, "K", INT2FIX(Key::K));
	rb_define_const(vKey, "L", INT2FIX(Key::L));
	rb_define_const(vKey, "M", INT2FIX(Key::M));
	rb_define_const(vKey, "N", INT2FIX(Key::N));
	rb_define_const(vKey, "O", INT2FIX(Key::O));
	rb_define_const(vKey, "P", INT2FIX(Key::P));
	rb_define_const(vKey, "Q", INT2FIX(Key::Q));
	rb_define_const(vKey, "R", INT2FIX(Key::R));
	rb_define_const(vKey, "S", INT2FIX(Key::S));
	rb_define_const(vKey, "T", INT2FIX(Key::T));
	rb_define_const(vKey, "U", INT2FIX(Key::U));
	rb_define_const(vKey, "V", INT2FIX(Key::V));
	rb_define_const(vKey, "W", INT2FIX(Key::W));
	rb_define_const(vKey, "X", INT2FIX(Key::X));
	rb_define_const(vKey, "Y", INT2FIX(Key::Y));
	rb_define_const(vKey, "Z", INT2FIX(Key::Z));
	rb_define_const(vKey, "Num0", INT2FIX(Key::Num0));
	rb_define_const(vKey, "Num1", INT2FIX(Key::Num1));
	rb_define_const(vKey, "Num2", INT2FIX(Key::Num2));
	rb_define_const(vKey, "Num3", INT2FIX(Key::Num3));
	rb_define_const(vKey, "Num4", INT2FIX(Key::Num4));
	rb_define_const(vKey, "Num5", INT2FIX(Key::Num5));
	rb_define_const(vKey, "Num6", INT2FIX(Key::Num6));
	rb_define_const(vKey, "Num7", INT2FIX(Key::Num7));
	rb_define_const(vKey, "Num8", INT2FIX(Key::Num8));
	rb_define_const(vKey, "Num9", INT2FIX(Key::Num9));
	rb_define_const(vKey, "Escape", INT2FIX(Key::Escape));
	rb_define_const(vKey, "Space", INT2FIX(Key::Space));
	rb_define_const(vKey, "Return", INT2FIX(Key::Return));
	rb_define_const(vKey, "Back", INT2FIX(Key::Space));
	rb_define_const(vKey, "Tab", INT2FIX(Key::Tab));
	rb_define_const(vKey, "PageUp", INT2FIX(Key::PageUp));
	rb_define_const(vKey, "PageDown", INT2FIX(Key::PageDown));
	rb_define_const(vKey, "End", INT2FIX(Key::End));
	rb_define_const(vKey, "Home", INT2FIX(Key::Home));
	rb_define_const(vKey, "Insert", INT2FIX(Key::Insert));
	rb_define_const(vKey, "Delete", INT2FIX(Key::Delete));
	rb_define_const(vKey, "Add", INT2FIX(Key::Add));
	rb_define_const(vKey, "Subtract", INT2FIX(Key::Subtract));
	rb_define_const(vKey, "Multiply", INT2FIX(Key::Multiply));
	rb_define_const(vKey, "Divide", INT2FIX(Key::Divide));
	rb_define_const(vKey, "Left", INT2FIX(Key::Left));
	rb_define_const(vKey, "Right", INT2FIX(Key::Right));
	rb_define_const(vKey, "Up", INT2FIX(Key::Up));
	rb_define_const(vKey, "Down", INT2FIX(Key::Down));
	rb_define_const(vKey, "Numpad0", INT2FIX(Key::Numpad0));
	rb_define_const(vKey, "Numpad1", INT2FIX(Key::Numpad1));
	rb_define_const(vKey, "Numpad2", INT2FIX(Key::Numpad2));
	rb_define_const(vKey, "Numpad3", INT2FIX(Key::Numpad3));
	rb_define_const(vKey, "Numpad4", INT2FIX(Key::Numpad4));
	rb_define_const(vKey, "Numpad5", INT2FIX(Key::Numpad5));
	rb_define_const(vKey, "Numpad6", INT2FIX(Key::Numpad6));
	rb_define_const(vKey, "Numpad7", INT2FIX(Key::Numpad7));
	rb_define_const(vKey, "Numpad8", INT2FIX(Key::Numpad8));
	rb_define_const(vKey, "Numpad9", INT2FIX(Key::Numpad9));
	rb_define_const(vKey, "F1", INT2FIX(Key::F1));
	rb_define_const(vKey, "F2", INT2FIX(Key::F2));
	rb_define_const(vKey, "F3", INT2FIX(Key::F3));
	rb_define_const(vKey, "F4", INT2FIX(Key::F4));
	rb_define_const(vKey, "F5", INT2FIX(Key::F5));
	rb_define_const(vKey, "F6", INT2FIX(Key::F6));
	rb_define_const(vKey, "F7", INT2FIX(Key::F7));
	rb_define_const(vKey, "F8", INT2FIX(Key::F8));
	rb_define_const(vKey, "F9", INT2FIX(Key::F9));
	rb_define_const(vKey, "F10", INT2FIX(Key::F10));
	rb_define_const(vKey, "F11", INT2FIX(Key::F11));
	rb_define_const(vKey, "F12", INT2FIX(Key::F12));
	rb_define_const(vKey, "F13", INT2FIX(Key::F13));
	rb_define_const(vKey, "F14", INT2FIX(Key::F14));
	rb_define_const(vKey, "F15", INT2FIX(Key::F15));
	rb_define_const(vKey, "Pause", INT2FIX(Key::Pause));
}

