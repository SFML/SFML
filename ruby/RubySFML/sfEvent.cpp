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
#include <SFML/Window.hpp>
#include "RubySFML.h"
using namespace sf;

VALUE g_cEvent;

void Event_free(void *p) { delete (Event *)p; }
VALUE Event_new(int argc, VALUE *argv, VALUE vClass) {
	Event *ptr = new Event();
	VALUE tData = Data_Wrap_Struct(vClass, 0, Event_free, ptr);
	rb_obj_call_init(tData, argc, argv);
	return tData;
}

static VALUE Event_initialize(int argc, VALUE *argv, VALUE vSelf) {
	// Get C++ object pointer from vSelf
	Event *pSelf;
	Data_Get_Struct(vSelf, Event, pSelf);
	if(argc == 0) {
		// Nothing to initialize
	} else
		rb_raise(rb_eTypeError, "wrong argument type(s)");
	return vSelf;
}

static VALUE Event_to_s(VALUE vSelf) {
	// Get C++ object pointer from vSelf
	Event *pSelf;
	Data_Get_Struct(vSelf, Event, pSelf);
	char szBuffer[1024];
	szBuffer[0] = 0;
	switch(pSelf->Type) {
		case Event::Closed:
			strcpy(szBuffer, "Closed");
			break;
		case Event::Resized:
			sprintf(szBuffer, "Resized: Width=%d, Height=%d", pSelf->Size.Width, pSelf->Size.Height);
			break;
		case Event::LostFocus:
			strcpy(szBuffer, "LostFocus");
			break;
		case Event::GainedFocus:
			strcpy(szBuffer, "GainedFocus");
			break;
		case Event::TextEntered:
			sprintf(szBuffer, "TextEntered: %C", pSelf->Text.Unicode);
			break;
		case Event::KeyPressed:
			sprintf(szBuffer, "KeyPressed: Code=%d, Alt=%s, Ctrl=%s, Shift=%s", pSelf->Key.Code, pSelf->Key.Alt ? "true" : "false", pSelf->Key.Control ? "true" : "false", pSelf->Key.Shift ? "true" : "false");
			break;
		case Event::KeyReleased:
			sprintf(szBuffer, "KeyReleased: Code=%d, Alt=%s, Ctrl=%s, Shift=%s", pSelf->Key.Code, pSelf->Key.Alt ? "true" : "false", pSelf->Key.Control ? "true" : "false", pSelf->Key.Shift ? "true" : "false");
			break;
		case Event::MouseWheelMoved:
			sprintf(szBuffer, "MouseWheelMoved: Delta=%d", pSelf->MouseWheel.Delta);
		case Event::MouseButtonPressed:
			sprintf(szBuffer, "MouseButtonPressed: Button=%d", pSelf->MouseButton.Button);
			break;
		case Event::MouseButtonReleased:
			sprintf(szBuffer, "MouseButtonReleased: Button=%d", pSelf->MouseButton.Button);
			break;
		case Event::MouseMoved:
			sprintf(szBuffer, "MouseMoved: X=%d, Y=%d", pSelf->MouseMove.X, pSelf->MouseMove.Y);
			break;
		case Event::JoyButtonPressed:
			sprintf(szBuffer, "JoyButtonPressed: Button=%d", pSelf->JoyButton.Button);
			break;
		case Event::JoyButtonReleased:
			sprintf(szBuffer, "JoyButtonReleased: Button=%d", pSelf->JoyButton.Button);
			break;
		case Event::JoyMoved:
			sprintf(szBuffer, "JoyMoved: Axis=%d, Pos=%f", pSelf->JoyMove.Axis, pSelf->JoyMove.Position);
			break;
		default:
			sprintf(szBuffer, "Unsupported event type: %d", pSelf->Type);
			break;
	}
	return rb_str_new2(szBuffer);
}

static VALUE Event_type(VALUE vSelf) {
	// Get C++ object pointer from vSelf
	Event *pSelf;
	Data_Get_Struct(vSelf, Event, pSelf);
	return INT2FIX(pSelf->Type);
}

static VALUE Event_char(VALUE vSelf) {
	// Get C++ object pointer from vSelf
	Event *pSelf;
	Data_Get_Struct(vSelf, Event, pSelf);
	char szBuffer[4];
	sprintf(szBuffer, "%C", pSelf->Text.Unicode);
	return rb_str_new2(szBuffer);
}

static VALUE Event_code(VALUE vSelf) {
	// Get C++ object pointer from vSelf
	Event *pSelf;
	Data_Get_Struct(vSelf, Event, pSelf);
	if(pSelf->Type == Event::KeyPressed || pSelf->Type == Event::KeyReleased)
		return INT2FIX(pSelf->Key.Code);
	rb_raise(rb_eTypeError, "wrong event type");
}

static VALUE Event_alt(VALUE vSelf) {
	// Get C++ object pointer from vSelf
	Event *pSelf;
	Data_Get_Struct(vSelf, Event, pSelf);
	if(pSelf->Type == Event::KeyPressed || pSelf->Type == Event::KeyReleased)
		return pSelf->Key.Alt ? Qtrue : Qfalse;
	rb_raise(rb_eTypeError, "wrong event type");
}

static VALUE Event_control(VALUE vSelf) {
	// Get C++ object pointer from vSelf
	Event *pSelf;
	Data_Get_Struct(vSelf, Event, pSelf);
	if(pSelf->Type == Event::KeyPressed || pSelf->Type == Event::KeyReleased)
		return pSelf->Key.Control ? Qtrue : Qfalse;
	rb_raise(rb_eTypeError, "wrong event type");
}

static VALUE Event_shift(VALUE vSelf) {
	// Get C++ object pointer from vSelf
	Event *pSelf;
	Data_Get_Struct(vSelf, Event, pSelf);
	if(pSelf->Type == Event::KeyPressed || pSelf->Type == Event::KeyReleased)
		return pSelf->Key.Shift ? Qtrue : Qfalse;
	rb_raise(rb_eTypeError, "wrong event type");
}

static VALUE Event_delta(VALUE vSelf) {
	// Get C++ object pointer from vSelf
	Event *pSelf;
	Data_Get_Struct(vSelf, Event, pSelf);
	if(pSelf->Type == Event::MouseWheelMoved)
		return INT2FIX(pSelf->MouseWheel.Delta);
	rb_raise(rb_eTypeError, "wrong event type");
}

static VALUE Event_buttons(VALUE vSelf) {
	// Get C++ object pointer from vSelf
	Event *pSelf;
	Data_Get_Struct(vSelf, Event, pSelf);
	if(pSelf->Type == Event::MouseButtonPressed || pSelf->Type == Event::MouseButtonReleased)
		return INT2FIX(pSelf->MouseButton.Button);
	rb_raise(rb_eTypeError, "wrong event type");
}

static VALUE Event_button(VALUE vSelf) {
	// Get C++ object pointer from vSelf
	Event *pSelf;
	Data_Get_Struct(vSelf, Event, pSelf);
	if(pSelf->Type == Event::JoyButtonPressed || pSelf->Type == Event::JoyButtonReleased)
		return INT2FIX(pSelf->JoyButton.Button);
	rb_raise(rb_eTypeError, "wrong event type");
}

static VALUE Event_x(VALUE vSelf) {
	// Get C++ object pointer from vSelf
	Event *pSelf;
	Data_Get_Struct(vSelf, Event, pSelf);
	if(pSelf->Type == Event::MouseMoved)
		return INT2FIX(pSelf->MouseMove.X);
	rb_raise(rb_eTypeError, "wrong event type");
}

static VALUE Event_y(VALUE vSelf) {
	// Get C++ object pointer from vSelf
	Event *pSelf;
	Data_Get_Struct(vSelf, Event, pSelf);
	if(pSelf->Type == Event::MouseMoved)
		return INT2FIX(pSelf->MouseMove.Y);
	rb_raise(rb_eTypeError, "wrong event type");
}

static VALUE Event_axis(VALUE vSelf) {
	// Get C++ object pointer from vSelf
	Event *pSelf;
	Data_Get_Struct(vSelf, Event, pSelf);
	if(pSelf->Type == Event::JoyMoved)
		return INT2FIX(pSelf->JoyMove.Axis);
	rb_raise(rb_eTypeError, "wrong event type");
}

static VALUE Event_pos(VALUE vSelf) {
	// Get C++ object pointer from vSelf
	Event *pSelf;
	Data_Get_Struct(vSelf, Event, pSelf);
	if(pSelf->Type == Event::JoyMoved)
		return rb_float_new(pSelf->JoyMove.Position);
	rb_raise(rb_eTypeError, "wrong event type");
}

static VALUE Event_width(VALUE vSelf) {
	// Get C++ object pointer from vSelf
	Event *pSelf;
	Data_Get_Struct(vSelf, Event, pSelf);
	if(pSelf->Type == Event::Resized)
		return INT2FIX(pSelf->Size.Width);
	rb_raise(rb_eTypeError, "wrong event type");
}

static VALUE Event_height(VALUE vSelf) {
	// Get C++ object pointer from vSelf
	Event *pSelf;
	Data_Get_Struct(vSelf, Event, pSelf);
	if(pSelf->Type == Event::Resized)
		return INT2FIX(pSelf->Size.Height);
	rb_raise(rb_eTypeError, "wrong event type");
}

void Init_Event()
{
	g_cEvent = rb_define_class_under(g_vModule, "Event", rb_cObject);
	DEFINE_CLASS_METHOD(Event, new, -1);
	DEFINE_INSTANCE_METHOD(Event, initialize, -1);

	// For any event type
	DEFINE_INSTANCE_METHOD(Event, to_s, 0);
	DEFINE_INSTANCE_METHOD(Event, type, 0);

	// For text events
	DEFINE_INSTANCE_METHOD(Event, char, 0);

	// For keyboard event types
	DEFINE_INSTANCE_METHOD(Event, code, 0);
	DEFINE_INSTANCE_METHOD(Event, alt, 0);
	DEFINE_INSTANCE_METHOD(Event, control, 0);
	DEFINE_INSTANCE_METHOD(Event, shift, 0);

	// For mouse/joystick event types
	DEFINE_INSTANCE_METHOD(Event, delta, 0);
	DEFINE_INSTANCE_METHOD(Event, buttons, 0);
	DEFINE_INSTANCE_METHOD(Event, button, 0);
	DEFINE_INSTANCE_METHOD(Event, x, 0);
	DEFINE_INSTANCE_METHOD(Event, y, 0);
	DEFINE_INSTANCE_METHOD(Event, axis, 0);
	DEFINE_INSTANCE_METHOD(Event, pos, 0);

	// For size event types
	DEFINE_INSTANCE_METHOD(Event, width, 0);
	DEFINE_INSTANCE_METHOD(Event, height, 0);
}

