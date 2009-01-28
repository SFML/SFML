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
#include <math.h>

#ifdef _MSC_VER
    #define COSF cosf
    #define SINF sinf
#else
    #define COSF cos
    #define SINF sin
#endif

using namespace sf;
#define PI 3.14159265f

// Wrapper for Drawable that calls the Ruby version of virtual methods in case a Ruby class derives from this one
class rDrawable : public Drawable {
protected:
	VALUE m_vSelf, m_vClass;
	ID m_idRender;
	bool m_bRender;

	virtual void Render(const RenderWindow &Window) const {
		// If this method is overridden in Ruby, call it
		if(m_bRender) {
			VALUE vWindow = Data_Wrap_Struct(g_cRenderWindow, 0, 0, (void*)&Window);
			VALUE vRet = rb_funcall(m_vSelf, m_idRender, 1, vWindow);
		} else 	// else raise error (pure virtual method)
			rb_raise(rb_eRuntimeError, "Pure virtual method Drawable::Render() called, but not defined.");
	}

public:
	// Call as soon as you get a Ruby VALUE pointing to this object
	void rInit(VALUE vSelf) {
		// Need these for rb_funcall
		m_vSelf = vSelf;
		m_vClass = CLASS_OF(m_vSelf);

		// Initialize members for Render() virtual method
		m_idRender = rb_intern("render");
		m_bRender = rb_method_boundp(m_vClass, m_idRender, 0) == Qtrue;
	}
};


VALUE g_cDrawable;

void Drawable_free(void *p) { delete (Drawable *)p; }
VALUE Drawable_new(int argc, VALUE *argv, VALUE vClass) {
	rDrawable *ptr = new rDrawable();
	VALUE tData = Data_Wrap_Struct(vClass, 0, Drawable_free, ptr);
	ptr->rInit(tData);
	rb_obj_call_init(tData, argc, argv);
	return tData;
}

static VALUE Drawable_initialize(int argc, VALUE *argv, VALUE vSelf) {
	// Get C++ object pointer from vSelf
	Drawable *pSelf;
	Data_Get_Struct(vSelf, Drawable, pSelf);
	if(argc == 0) {
		// Nothing to initialize
	} else if(argc >= 1 && argc <= 5 && (argc < 5 || IS(argv[4], g_cColor))) {
		if(argc >= 1)
			pSelf->SetLeft((float)NUM2DBL(argv[0]));
		if(argc >= 2)
			pSelf->SetTop((float)NUM2DBL(argv[1]));
		if(argc >= 3) {
			if(ISNUM(argv[2])) {
				float f = (float)NUM2DBL(argv[2]);
				pSelf->SetScale(f, f);
			} else if(IS(argv[2], rb_cArray)) {
				float x = (float)NUM2DBL(rb_ary_entry(argv[2], 0));
				float y = (float)NUM2DBL(rb_ary_entry(argv[2], 1));
				pSelf->SetScale(x, y);
			}
		}
		if(argc >= 4)
			pSelf->SetRotation((float)NUM2DBL(argv[3]));
		if(argc >= 5)
			pSelf->SetColor(*(Color *)DATA_PTR(argv[4]));
	} else
		rb_raise(rb_eTypeError, "wrong argument type(s)");
	return vSelf;
}

static VALUE Drawable_to_s(VALUE vSelf) {
	// Get C++ object pointer from vSelf
	Drawable *pSelf;
	Data_Get_Struct(vSelf, Drawable, pSelf);
	char szBuffer[256];
	sprintf(szBuffer, "Left=%f, Top=%f, Scale=[%f,%f], Rotation=%f", pSelf->GetLeft(), pSelf->GetTop(), pSelf->GetScaleX(), pSelf->GetScaleX(), pSelf->GetRotation());
	return rb_str_new2(szBuffer);
}

static VALUE Drawable_set_left(VALUE vSelf, VALUE vLeft) {
	// Get C++ object pointer from vSelf
	Drawable *pSelf;
	Data_Get_Struct(vSelf, Drawable, pSelf);
	pSelf->SetLeft((float)NUM2DBL(vLeft));
	return Qnil;
}

static VALUE Drawable_set_top(VALUE vSelf, VALUE vTop) {
	// Get C++ object pointer from vSelf
	Drawable *pSelf;
	Data_Get_Struct(vSelf, Drawable, pSelf);
	pSelf->SetTop((float)NUM2DBL(vTop));
	return Qnil;
}

static VALUE Drawable_set_scale(VALUE vSelf, VALUE vScale) {
	// Get C++ object pointer from vSelf
	Drawable *pSelf;
	Data_Get_Struct(vSelf, Drawable, pSelf);
	if(ISNUM(vScale)) {
		float f = (float)NUM2DBL(vScale);
		pSelf->SetScale(f, f);
	} else if(IS(vScale, rb_cArray)) {
		float x = (float)NUM2DBL(rb_ary_entry(vScale, 0));
		float y = (float)NUM2DBL(rb_ary_entry(vScale, 1));
		pSelf->SetScale(x, y);
	} else
		rb_raise(rb_eTypeError, "wrong argument type(s)");
	return Qnil;
}

static VALUE Drawable_set_rotation(VALUE vSelf, VALUE vRotation) {
	// Get C++ object pointer from vSelf
	Drawable *pSelf;
	Data_Get_Struct(vSelf, Drawable, pSelf);
	pSelf->SetRotation((float)NUM2DBL(vRotation));
	return Qnil;
}

static VALUE Drawable_setRotationCenter(VALUE vSelf, VALUE vX, VALUE vY) {
	// Get C++ object pointer from vSelf
	Drawable *pSelf;
	Data_Get_Struct(vSelf, Drawable, pSelf);
	pSelf->SetRotationCenter((float)NUM2DBL(vX), (float)NUM2DBL(vY));
	return Qnil;
}

static VALUE Drawable_set_color(VALUE vSelf, VALUE vColor) {
	// Get C++ object pointer from vSelf
	Drawable *pSelf;
	Data_Get_Struct(vSelf, Drawable, pSelf);
	if(!IS(vColor, g_cColor))
		rb_raise(rb_eTypeError, "wrong argument type(s)");
	pSelf->SetColor(*(Color *)DATA_PTR(vColor));
	return Qnil;
}

static VALUE Drawable_get_left(VALUE vSelf) {
	// Get C++ object pointer from vSelf
	Drawable *pSelf;
	Data_Get_Struct(vSelf, Drawable, pSelf);
	return rb_float_new(pSelf->GetLeft());
}

static VALUE Drawable_get_top(VALUE vSelf) {
	// Get C++ object pointer from vSelf
	Drawable *pSelf;
	Data_Get_Struct(vSelf, Drawable, pSelf);
	return rb_float_new(pSelf->GetTop());
}

static VALUE Drawable_get_scale(VALUE vSelf) {
	// Get C++ object pointer from vSelf
	Drawable *pSelf;
	Data_Get_Struct(vSelf, Drawable, pSelf);
	float x = pSelf->GetScaleX();
	float y = pSelf->GetScaleY();
	VALUE vArr = rb_ary_new();
	rb_ary_push(vArr, rb_float_new(x));
	rb_ary_push(vArr, rb_float_new(y));
	return vArr;
}

static VALUE Drawable_get_rotation(VALUE vSelf) {
	// Get C++ object pointer from vSelf
	Drawable *pSelf;
	Data_Get_Struct(vSelf, Drawable, pSelf);
	return rb_float_new(pSelf->GetRotation());
}

static VALUE Drawable_get_upVector(VALUE vSelf) {
	// Get C++ object pointer from vSelf
	Drawable *pSelf;
	Data_Get_Struct(vSelf, Drawable, pSelf);
	float f = (pSelf->GetRotation() + 90) * (PI/180.0f);
	VALUE v = rb_ary_new();
	rb_ary_push(v, rb_float_new(COSF(f)));
	rb_ary_push(v, rb_float_new(-SINF(f)));
	return v;
}

static VALUE Drawable_get_rightVector(VALUE vSelf) {
	// Get C++ object pointer from vSelf
	Drawable *pSelf;
	Data_Get_Struct(vSelf, Drawable, pSelf);
	float f = pSelf->GetRotation() * (PI/180.0f);
	VALUE v = rb_ary_new();
	rb_ary_push(v, rb_float_new(COSF(f)));
	rb_ary_push(v, rb_float_new(-SINF(f)));
	return v;
}

static VALUE Drawable_get_color(VALUE vSelf) {
	// Get C++ object pointer from vSelf
	Drawable *pSelf;
	Data_Get_Struct(vSelf, Drawable, pSelf);
	DECLARE_OBJ_VAR(Color, Color, pSelf->GetColor());
	return vColor;
}

static VALUE Drawable_setPosition(VALUE vSelf, float vX, float vY) {
	// Get C++ object pointer from vSelf
	Drawable *pSelf;
	Data_Get_Struct(vSelf, Drawable, pSelf);
	pSelf->SetPosition((float)NUM2DBL(vX), (float)NUM2DBL(vY));
	return Qnil;
}

static VALUE Drawable_move(VALUE vSelf, float vX, float vY) {
	// Get C++ object pointer from vSelf
	Drawable *pSelf;
	Data_Get_Struct(vSelf, Drawable, pSelf);
	pSelf->Move((float)NUM2DBL(vX), (float)NUM2DBL(vY));
	return Qnil;
}

static VALUE Drawable_rotate(VALUE vSelf, VALUE vRotate) {
	// Get C++ object pointer from vSelf
	Drawable *pSelf;
	Data_Get_Struct(vSelf, Drawable, pSelf);
	pSelf->Rotate((float)NUM2DBL(vRotate));
	return Qnil;
}

void Init_Drawable()
{
	g_cDrawable = rb_define_class_under(g_vModule, "Drawable", rb_cObject);
	DEFINE_CLASS_METHOD(Drawable, new, -1);
	DEFINE_INSTANCE_METHOD(Drawable, initialize, -1);

	DEFINE_RW(Drawable, left);
	DEFINE_RW(Drawable, top);
	DEFINE_RW(Drawable, scale);
	DEFINE_RW(Drawable, color);
	DEFINE_RW(Drawable, rotation);
	DEFINE_GETTER(Drawable, upVector);
	DEFINE_GETTER(Drawable, rightVector);

	DEFINE_INSTANCE_METHOD(Drawable, to_s, 0);
	DEFINE_INSTANCE_METHOD(Drawable, setRotationCenter, 2);
	DEFINE_INSTANCE_METHOD(Drawable, setPosition, 2);
	DEFINE_INSTANCE_METHOD(Drawable, move, 2);
	DEFINE_INSTANCE_METHOD(Drawable, rotate, 1);
}

