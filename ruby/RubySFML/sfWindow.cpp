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

VALUE g_cWindow;

void Window_free(void *p) { delete (Window *)p; }
VALUE Window_new(int argc, VALUE *argv, VALUE vClass) {
	Window *ptr = new Window();
	VALUE tData = Data_Wrap_Struct(vClass, 0, Window_free, ptr);
	rb_obj_call_init(tData, argc, argv);
	return tData;
}

static VALUE Window_initialize(int argc, VALUE *argv, VALUE vSelf) {
	// Get C++ object pointer from vSelf
	Window *pSelf;
	Data_Get_Struct(vSelf, Window, pSelf);
	if(argc >= 2 && argc <= 3 &&
			IS(argv[0], g_cVideoMode) &&
			ISSTR(argv[1]) &&
			(argc < 3 || true) &&
			(argc < 4 || ISNUM(argv[3]))) {
		pSelf->Create(*(VideoMode *)DATA_PTR(argv[0]), STR2CSTR(argv[1]),
			argc < 3 ? (Style::Resize|Style::Close) : NUM2INT(argv[2]),
			argc < 4 ? 0 : NUM2INT(argv[3]));
	} else
		rb_raise(rb_eTypeError, "wrong argument type(s)");
	return vSelf;
}

static VALUE Window_to_s(VALUE vSelf) {
	// Get C++ object pointer from vSelf
	Window *pSelf;
	Data_Get_Struct(vSelf, Window, pSelf);
	char szBuffer[256];
	sprintf(szBuffer, "Height: %d, Width: %d, Depth Bits: %d, Stencil Bits: %d", pSelf->GetWidth(), pSelf->GetHeight(), pSelf->GetDepthBits(), pSelf->GetStencilBits());
	return rb_str_new2(szBuffer);
}

static VALUE Window_get_width(VALUE vSelf) {
	// Get C++ object pointer from vSelf
	Window *pSelf;
	Data_Get_Struct(vSelf, Window, pSelf);
	return INT2FIX(pSelf->GetWidth());
}

static VALUE Window_get_height(VALUE vSelf) {
	// Get C++ object pointer from vSelf
	Window *pSelf;
	Data_Get_Struct(vSelf, Window, pSelf);
	return INT2FIX(pSelf->GetHeight());
}

static VALUE Window_get_depthBits(VALUE vSelf) {
	// Get C++ object pointer from vSelf
	Window *pSelf;
	Data_Get_Struct(vSelf, Window, pSelf);
	return INT2FIX(pSelf->GetDepthBits());
}

static VALUE Window_get_stencilBits(VALUE vSelf) {
	// Get C++ object pointer from vSelf
	Window *pSelf;
	Data_Get_Struct(vSelf, Window, pSelf);
	return INT2FIX(pSelf->GetStencilBits());
}

static VALUE Window_get_frameTime(VALUE vSelf) {
	// Get C++ object pointer from vSelf
	Window *pSelf;
	Data_Get_Struct(vSelf, Window, pSelf);
	return rb_float_new(pSelf->GetFrameTime());
}

static VALUE Window_get_input(VALUE vSelf) {
	// Get C++ object pointer from vSelf
	Window *pSelf;
	Data_Get_Struct(vSelf, Window, pSelf);
	DECLARE_PTR_VAR(Input, Input, (Input *)&pSelf->GetInput());
	return vInput;
}

static VALUE Window_getEvent(VALUE vSelf) {
	// Get C++ object pointer from vSelf
	Window *pSelf;
	Data_Get_Struct(vSelf, Window, pSelf);
	Event ePoll;
	if(!pSelf->GetEvent(ePoll))
		return Qnil;
	DECLARE_OBJ_VAR(Event, Event, ePoll);
	return vEvent;
}

static VALUE Window_useVerticalSync(VALUE vSelf, VALUE vEnabled) {
	// Get C++ object pointer from vSelf
	Window *pSelf;
	Data_Get_Struct(vSelf, Window, pSelf);
	pSelf->UseVerticalSync(RTEST(vEnabled) != 0);
	return Qnil;
}

static VALUE Window_showMouseCursor(VALUE vSelf, VALUE vShow) {
	// Get C++ object pointer from vSelf
	Window *pSelf;
	Data_Get_Struct(vSelf, Window, pSelf);
	pSelf->ShowMouseCursor(RTEST(vShow) != 0);
	return Qnil;
}

static VALUE Window_setFramerateLimit(VALUE vSelf, VALUE vLimit) {
	// Get C++ object pointer from vSelf
	Window *pSelf;
	Data_Get_Struct(vSelf, Window, pSelf);
	pSelf->SetFramerateLimit((unsigned int)NUM2INT(vLimit));
	return Qnil;
}

static VALUE Window_display(VALUE vSelf) {
	// Get C++ object pointer from vSelf
	Window *pSelf;
	Data_Get_Struct(vSelf, Window, pSelf);
	pSelf->Display();
	return Qnil;
}

static VALUE Window_setCurrent(VALUE vSelf) {
	// Get C++ object pointer from vSelf
	Window *pSelf;
	Data_Get_Struct(vSelf, Window, pSelf);
	return pSelf->SetCurrent() ? Qtrue : Qfalse;
}

void Init_Window()
{
	g_cWindow = rb_define_class_under(g_vModule, "Window", rb_cObject);
	DEFINE_CLASS_METHOD(Window, new, -1);
	DEFINE_INSTANCE_METHOD(Window, initialize, -1);

	DEFINE_GETTER(Window, width);
	DEFINE_GETTER2(Window, width, w);
	DEFINE_GETTER(Window, height);
	DEFINE_GETTER2(Window, height, h);
	DEFINE_GETTER(Window, input);
	DEFINE_GETTER(Window, frameTime);
	DEFINE_GETTER(Window, stencilBits);
	DEFINE_GETTER(Window, depthBits);

	DEFINE_INSTANCE_METHOD(Window, to_s, 0);
	DEFINE_INSTANCE_METHOD(Window, getEvent, 0); // Don't use GETTER
	DEFINE_INSTANCE_METHOD(Window, useVerticalSync, 1);
	DEFINE_INSTANCE_METHOD(Window, showMouseCursor, 1);
	DEFINE_INSTANCE_METHOD(Window, setFramerateLimit, 1);
	DEFINE_INSTANCE_METHOD(Window, display, 0);
	DEFINE_INSTANCE_METHOD(Window, setCurrent, 0); // Dont use SETTER
}

