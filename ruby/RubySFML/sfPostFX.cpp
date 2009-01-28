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

// Wrapper for PostFX that calls the Ruby version of virtual methods in case a Ruby class derives from this one
class rPostFX : public PostFX {
protected:
	VALUE m_vSelf, m_vClass;
	ID m_idRender;
	bool m_bRender, m_bInRender;

	virtual void Render(const RenderWindow &Window) const {
		// If this method is overridden in Ruby, call it
		if(m_bRender) {
			VALUE vWindow = Data_Wrap_Struct(g_cRenderWindow, 0, 0, (void*)&Window);
			VALUE vRet = rb_funcall(m_vSelf, m_idRender, 1, vWindow);
		} else 	// else call parent
			PostFX::Render(Window);
	}

public:
	// Damn constructors should be inherited from base class
	rPostFX() : PostFX() {}
	rPostFX(const PostFX &Copy) : PostFX(Copy) {}

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
		PostFX::Render(*(RenderWindow *)DATA_PTR(vWindow));
		return Qnil;
	}
};


VALUE g_cPostFX;

void PostFX_free(void *p) { delete (PostFX *)p; }
VALUE PostFX_new(int argc, VALUE *argv, VALUE vClass) {
	// For each version of this method, convert Ruby args to C++ types (applying default values)
	rPostFX *ptr = new rPostFX();
	VALUE tData = Data_Wrap_Struct(vClass, 0, PostFX_free, ptr);
	ptr->rInit(tData);
	rb_obj_call_init(tData, argc, argv);
	return tData;
}

static VALUE PostFX_initialize(int argc, VALUE *argv, VALUE vSelf) {
	// Get C++ object pointer from vSelf
	PostFX *pSelf;
	Data_Get_Struct(vSelf, PostFX, pSelf);
	if(argc == 0) {
		// Nothing to initialize
	} else if(argc == 1 && ISSTR(argv[0])) {
		pSelf->LoadFromFile(STR2CSTR(argv[0]));
	} else if(argc == 1 && IS(argv[0], g_cPostFX)) {
		*pSelf = *(PostFX *)DATA_PTR(argv[0]);
	} else
		rb_raise(rb_eTypeError, "wrong argument type(s)");
	return vSelf;
}

static VALUE PostFX_loadFromFile(VALUE vSelf, VALUE vFile) {
	// Get C++ object pointer from vSelf
	PostFX *pSelf;
	Data_Get_Struct(vSelf, PostFX, pSelf);
	return pSelf->LoadFromFile(STR2CSTR(vFile)) ? Qtrue : Qfalse;
}

static VALUE PostFX_setParameter(int argc, VALUE *argv, VALUE vSelf) {
	PostFX *pSelf;
	Data_Get_Struct(vSelf, PostFX, pSelf);
	if(argc == 2 && ISSTR(argv[0]) && ISNUM(argv[1])) {
		pSelf->SetParameter(STR2CSTR(argv[0]), (float)NUM2DBL(argv[1]));
	} else if(argc == 3 && ISSTR(argv[0]) && ISNUM(argv[1]) && ISNUM(argv[2])) {
		pSelf->SetParameter(STR2CSTR(argv[0]), (float)NUM2DBL(argv[1]), (float)NUM2DBL(argv[2]));
	} else if(argc == 4 && ISSTR(argv[0]) && ISNUM(argv[1]) && ISNUM(argv[2]) && ISNUM(argv[3])) {
		pSelf->SetParameter(STR2CSTR(argv[0]), (float)NUM2DBL(argv[1]), (float)NUM2DBL(argv[2]), (float)NUM2DBL(argv[3]));
	} else
		rb_raise(rb_eTypeError, "wrong argument type(s)");
	return Qnil;
}

static VALUE PostFX_setTexture(VALUE vSelf, VALUE vName, VALUE vImage) {
	// Get C++ object pointer from vSelf
	PostFX *pSelf;
	Data_Get_Struct(vSelf, PostFX, pSelf);
	if(!IS(vImage, g_cImage))
		rb_raise(rb_eTypeError, "wrong argument type(s)");
	pSelf->SetTexture(STR2CSTR(vName), (Image *)DATA_PTR(vImage));
	return Qnil;
}

static VALUE PostFX_render(VALUE vSelf, VALUE vWindow) {
	// Get C++ object pointer from vSelf
	rPostFX *pSelf;
	Data_Get_Struct(vSelf, rPostFX, pSelf);
	return pSelf->Render(vWindow);
}

void Init_PostFX()
{
	g_cPostFX = rb_define_class_under(g_vModule, "PostFX", g_cDrawable);
	DEFINE_CLASS_METHOD(PostFX, new, -1);
	DEFINE_INSTANCE_METHOD(PostFX, initialize, -1);

	DEFINE_INSTANCE_METHOD(PostFX, setParameter, -1);
	DEFINE_INSTANCE_METHOD(PostFX, loadFromFile, 1);
	DEFINE_INSTANCE_METHOD(PostFX, setTexture, 2);

	// Virtual method
	DEFINE_INSTANCE_METHOD(PostFX, render, 1);
}

