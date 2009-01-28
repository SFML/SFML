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

// Wrapper for String that calls the Ruby version of virtual methods in case a Ruby class derives from this one
class rString : public String {
protected:
	VALUE m_vSelf, m_vClass;
	ID m_idRender;
	bool m_bRender;

	virtual void Render(const RenderWindow &Window) const {
		// If this method is overridden in Ruby, call it
		if(m_bRender) {
			VALUE vWindow = Data_Wrap_Struct(g_cRenderWindow, 0, 0, (void*)&Window);
			VALUE vRet = rb_funcall(m_vSelf, m_idRender, 1, vWindow);
		} else 	// else call parent
			String::Render(Window);
	}

public:
	// Damn constructors should be inherited from base class
	rString(const std::string &Text="", const std::string &Font="", float Size=32.f) :String(Text, Font, Size) {}

	// Call as soon as you get a Ruby VALUE pointing to this object
	void rInit(VALUE vSelf) {
		// Need these for rb_funcall
		m_vSelf = vSelf;
		m_vClass = CLASS_OF(m_vSelf);

		// Initialize members for Render() virtual method
		m_idRender = rb_intern("render");
		m_bRender = rb_method_boundp(m_vClass, m_idRender, 0) == Qtrue;
	}

	// When an overridden method in Ruby calls super(), it is called this way
	VALUE Render(VALUE vWindow) {
		String::Render(*(RenderWindow *)DATA_PTR(vWindow));
		return Qnil;
	}
};


VALUE g_cString;

void String_free(void *p) { delete (String *)p; }
VALUE String_new(int argc, VALUE *argv, VALUE vClass) {
	// For each version of this method, convert Ruby args to C++ types (applying default values)
	rString *ptr = new rString();
	VALUE tData = Data_Wrap_Struct(vClass, 0, String_free, ptr);
	rb_obj_call_init(tData, argc, argv);
	ptr->rInit(tData);
	return tData;
}

static VALUE String_initialize(int argc, VALUE *argv, VALUE vSelf) {
	// Get C++ object pointer from vSelf
	String *pSelf;
	Data_Get_Struct(vSelf, String, pSelf);
	if(argc >= 0 && argc <= 3 &&
			(argc < 1 || ISSTR(argv[0])) &&
			(argc < 2 || ISSTR(argv[1])) &&
			(argc < 3 || ISNUM(argv[2]))) {
		if(argc >= 1)
			pSelf->SetText(STR2CSTR(argv[0]));
		if(argc >= 2)
			pSelf->SetFont(STR2CSTR(argv[1]));
		if(argc >= 3)
			pSelf->SetSize((float)NUM2DBL(argv[2]));
	} else
		rb_raise(rb_eTypeError, "wrong argument type(s)");
	return vSelf;
}

static VALUE String_preloadFont(int argc, VALUE *argv, VALUE vClass) {
	if(argc >= 2 && argc <= 2)
		String::PreloadFont(STR2CSTR(argv[0]), NUM2INT(argv[1]));
	else
		rb_raise(rb_eTypeError, "wrong argument type(s)");
	return Qnil;
}

static VALUE String_set_text(VALUE vSelf, VALUE vText) {
	// Get C++ object pointer from vSelf
	String *pSelf;
	Data_Get_Struct(vSelf, String, pSelf);
	if(!ISSTR(vText))
		rb_raise(rb_eTypeError, "wrong argument type(s)");
	pSelf->SetText(STR2CSTR(vText));
	return Qnil;
}

static VALUE String_set_font(VALUE vSelf, VALUE vFont) {
	// Get C++ object pointer from vSelf
	String *pSelf;
	Data_Get_Struct(vSelf, String, pSelf);
	if(!ISSTR(vFont))
		rb_raise(rb_eTypeError, "wrong argument type(s)");
	pSelf->SetText(STR2CSTR(vFont));
	return Qnil;
}

static VALUE String_set_size(VALUE vSelf, VALUE vSize) {
	// Get C++ object pointer from vSelf
	String *pSelf;
	Data_Get_Struct(vSelf, String, pSelf);
	if(!ISNUM(vSize))
		rb_raise(rb_eTypeError, "wrong argument type(s)");
	pSelf->SetSize((float)NUM2DBL(vSize));
	return Qnil;
}

static VALUE String_get_text(VALUE vSelf) {
	// Get C++ object pointer from vSelf
	String *pSelf;
	Data_Get_Struct(vSelf, String, pSelf);
	return rb_str_new2(pSelf->GetText().c_str());
}

static VALUE String_get_font(VALUE vSelf) {
	// Get C++ object pointer from vSelf
	String *pSelf;
	Data_Get_Struct(vSelf, String, pSelf);
	return rb_str_new2(pSelf->GetFont().c_str());
}

static VALUE String_get_size(VALUE vSelf) {
	// Get C++ object pointer from vSelf
	String *pSelf;
	Data_Get_Struct(vSelf, String, pSelf);
	return rb_float_new(pSelf->GetSize());
}

static VALUE String_getRect(VALUE vSelf) {
	// Get C++ object pointer from vSelf
	String *pSelf;
	Data_Get_Struct(vSelf, String, pSelf);
	DECLARE_OBJ_VAR(FloatRect, Rect, pSelf->GetRect());
	return vRect;
}

static VALUE String_render(VALUE vSelf, VALUE vWindow) {
	// Get C++ object pointer from vSelf
	rString *pSelf;
	Data_Get_Struct(vSelf, rString, pSelf);
	return pSelf->Render(vWindow);
}

void Init_String()
{
	g_cString = rb_define_class_under(g_vModule, "Text", g_cDrawable);
	DEFINE_CLASS_METHOD(String, new, -1);
	DEFINE_INSTANCE_METHOD(String, initialize, -1);

	DEFINE_CLASS_METHOD(String, preloadFont, -1);

	DEFINE_RW(String, text);
	DEFINE_RW(String, font);
	DEFINE_RW(String, size);

	DEFINE_INSTANCE_METHOD(String, getRect, 0);

	// Virtual method
	DEFINE_INSTANCE_METHOD(String, render, 1);
}

