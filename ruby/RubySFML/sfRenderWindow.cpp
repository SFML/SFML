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
#include <SFML/Graphics.hpp>
#include "RubySFML.h"
using namespace sf;

VALUE g_cRenderWindow;

void RenderWindow_free(void *p) { delete (RenderWindow *)p; }
VALUE RenderWindow_new(int argc, VALUE *argv, VALUE vClass) {
	// For each version of this method, convert Ruby args to C++ types (applying default values)
	RenderWindow *ptr = new RenderWindow();
	VALUE tData = Data_Wrap_Struct(vClass, 0, RenderWindow_free, ptr);
	rb_obj_call_init(tData, argc, argv);
	return tData;
}

static VALUE RenderWindow_initialize(int argc, VALUE *argv, VALUE vSelf) {
	// Get C++ object pointer from vSelf
	RenderWindow *pSelf;
	Data_Get_Struct(vSelf, RenderWindow, pSelf);
	if(argc >= 2 && argc <= 4 &&
			IS(argv[0], g_cVideoMode) &&
			ISSTR(argv[1]) &&
			(argc < 3 || true) &&
			(argc < 4 || ISNUM(argv[3]))) {
		pSelf->Create(
			*(VideoMode *)DATA_PTR(argv[0]),
			STR2CSTR(argv[1]),
			argc < 3 ? (Style::Resize | Style::Close) : NUM2INT(argv[2]),
			argc < 4 ? 0 : NUM2INT(argv[3]));
	} else
		rb_raise(rb_eTypeError, "wrong argument type(s)");
	return vSelf;
}

static VALUE RenderWindow_to_s(VALUE vSelf) {
	// Get C++ object pointer from vSelf
	RenderWindow *pSelf;
	Data_Get_Struct(vSelf, RenderWindow, pSelf);
	char szBuffer[256];
	sprintf(szBuffer, "Height: %d, Width: %d, Depth Bits: %d, Stencil Bits: %d", pSelf->GetWidth(), pSelf->GetHeight(), pSelf->GetDepthBits(), pSelf->GetStencilBits());
	return rb_str_new2(szBuffer);
}

static VALUE RenderWindow_get_width(VALUE vSelf) {
	// Get C++ object pointer from vSelf
	RenderWindow *pSelf;
	Data_Get_Struct(vSelf, RenderWindow, pSelf);
	return INT2FIX(pSelf->GetWidth());
}

static VALUE RenderWindow_get_height(VALUE vSelf) {
	// Get C++ object pointer from vSelf
	RenderWindow *pSelf;
	Data_Get_Struct(vSelf, RenderWindow, pSelf);
	return INT2FIX(pSelf->GetHeight());
}

static VALUE RenderWindow_get_depthBits(VALUE vSelf) {
	// Get C++ object pointer from vSelf
	RenderWindow *pSelf;
	Data_Get_Struct(vSelf, RenderWindow, pSelf);
	return INT2FIX(pSelf->GetDepthBits());
}

static VALUE RenderWindow_get_stencilBits(VALUE vSelf) {
	// Get C++ object pointer from vSelf
	RenderWindow *pSelf;
	Data_Get_Struct(vSelf, RenderWindow, pSelf);
	return INT2FIX(pSelf->GetStencilBits());
}

static VALUE RenderWindow_get_frameTime(VALUE vSelf) {
	// Get C++ object pointer from vSelf
	RenderWindow *pSelf;
	Data_Get_Struct(vSelf, RenderWindow, pSelf);
	return rb_float_new(pSelf->GetFrameTime());
}

static VALUE RenderWindow_get_input(VALUE vSelf) {
	// Get C++ object pointer from vSelf
	RenderWindow *pSelf;
	Data_Get_Struct(vSelf, RenderWindow, pSelf);
	DECLARE_PTR_VAR(Input, Input, (Input *)&pSelf->GetInput());
	return vInput;
}

static VALUE RenderWindow_getEvent(VALUE vSelf) {
	// Get C++ object pointer from vSelf
	RenderWindow *pSelf;
	Data_Get_Struct(vSelf, RenderWindow, pSelf);
	Event ePoll;
	if(!pSelf->GetEvent(ePoll))
		return Qnil;
	DECLARE_OBJ_VAR(Event, Event, ePoll);
	return vEvent;
}

static VALUE RenderWindow_useVerticalSync(VALUE vSelf, VALUE vEnabled) {
	// Get C++ object pointer from vSelf
	RenderWindow *pSelf;
	Data_Get_Struct(vSelf, RenderWindow, pSelf);
	pSelf->UseVerticalSync(RTEST(vEnabled) != 0);
	return Qnil;
}

static VALUE RenderWindow_showMouseCursor(VALUE vSelf, VALUE vShow) {
	// Get C++ object pointer from vSelf
	RenderWindow *pSelf;
	Data_Get_Struct(vSelf, RenderWindow, pSelf);
	pSelf->ShowMouseCursor(RTEST(vShow) != 0);
	return Qnil;
}

static VALUE RenderWindow_display(VALUE vSelf) {
	// Get C++ object pointer from vSelf
	RenderWindow *pSelf;
	Data_Get_Struct(vSelf, RenderWindow, pSelf);
	pSelf->Display();
	return Qnil;
}

static VALUE RenderWindow_optimize(VALUE vSelf, VALUE vBool) {
	// Get C++ object pointer from vSelf
	RenderWindow *pSelf;
	Data_Get_Struct(vSelf, RenderWindow, pSelf);
	pSelf->OptimizeForNonOpenGL(RTEST(vBool));
	return Qnil;
}

static VALUE RenderWindow_capture(VALUE vSelf) {
	// Get C++ object pointer from vSelf
	RenderWindow *pSelf;
	Data_Get_Struct(vSelf, RenderWindow, pSelf);
	DECLARE_OBJ_VAR(Image, Image, pSelf->Capture());
	return vImage;
}

static VALUE RenderWindow_get_view(VALUE vSelf) {
	// Get C++ object pointer from vSelf
	RenderWindow *pSelf;
	Data_Get_Struct(vSelf, RenderWindow, pSelf);
	DECLARE_OBJ_VAR(FloatRect, Rect, pSelf->GetViewRect());
	return vRect;
}

static VALUE RenderWindow_set_view(VALUE vSelf, VALUE vView) {
	// Get C++ object pointer from vSelf
	RenderWindow *pSelf;
	Data_Get_Struct(vSelf, RenderWindow, pSelf);
	View *pView;
	Data_Get_Struct(vView, View, pView);
	pSelf->SetView(pView);
	return Qnil;
}

static VALUE RenderWindow_set_backgroundColor(VALUE vSelf, VALUE vColor) {
	// Get C++ object pointer from vSelf
	RenderWindow *pSelf;
	Data_Get_Struct(vSelf, RenderWindow, pSelf);
	Color *pColor;
	Data_Get_Struct(vColor, Color, pColor);
	pSelf->SetBackgroundColor(*pColor);
	return Qnil;
}

static VALUE RenderWindow_draw(VALUE vSelf, VALUE vObject) {
	if(!ISKO(vObject, g_cDrawable))
		rb_raise(rb_eTypeError, "wrong argument type(s)");
	// Get C++ object pointer from vSelf
	RenderWindow *pSelf;
	Data_Get_Struct(vSelf, RenderWindow, pSelf);
	Drawable *pObject;
	Data_Get_Struct(vObject, Drawable, pObject);
	pSelf->Draw(*pObject);
	return Qnil;
}

static VALUE RenderWindow_setFramerateLimit(VALUE vSelf, VALUE vLimit) {
	// Get C++ object pointer from vSelf
	RenderWindow *pSelf;
	Data_Get_Struct(vSelf, RenderWindow, pSelf);
	pSelf->SetFramerateLimit((unsigned int)NUM2INT(vLimit));
	return Qnil;
}

void Init_RenderWindow()
{
	g_cRenderWindow = rb_define_class_under(g_vModule, "RenderWindow", rb_cObject);
	DEFINE_CLASS_METHOD(RenderWindow, new, -1);
	DEFINE_INSTANCE_METHOD(RenderWindow, initialize, -1);

	DEFINE_GETTER(RenderWindow, width);
	DEFINE_GETTER2(RenderWindow, width, w);
	DEFINE_GETTER(RenderWindow, height);
	DEFINE_GETTER2(RenderWindow, height, h);
	DEFINE_GETTER(RenderWindow, input);
	DEFINE_GETTER(RenderWindow, frameTime);
	DEFINE_GETTER(RenderWindow, stencilBits);
	DEFINE_GETTER(RenderWindow, depthBits);
	DEFINE_RW(RenderWindow, view);
	DEFINE_SETTER(RenderWindow, backgroundColor);

	DEFINE_INSTANCE_METHOD(RenderWindow, to_s, 0);
	DEFINE_INSTANCE_METHOD(RenderWindow, getEvent, 0); // Don't use GETTER
	DEFINE_INSTANCE_METHOD(RenderWindow, useVerticalSync, 1);
	DEFINE_INSTANCE_METHOD(RenderWindow, showMouseCursor, 1);
	DEFINE_INSTANCE_METHOD(RenderWindow, display, 0);
	DEFINE_INSTANCE_METHOD(RenderWindow, optimize, 1);
	DEFINE_INSTANCE_METHOD(RenderWindow, capture, 0);
	DEFINE_INSTANCE_METHOD(RenderWindow, draw, 1);
	DEFINE_INSTANCE_METHOD(RenderWindow, setFramerateLimit, 1);
}

