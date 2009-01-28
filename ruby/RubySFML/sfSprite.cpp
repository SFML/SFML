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
using namespace sf;

#ifdef _MSC_VER
    #define SQRTF sqrtf
#else
    #define SQRTF sqrt
#endif

// Wrapper for Sprite that calls the Ruby version of virtual methods in case a Ruby class derives from this one
class rSprite : public Sprite {
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
			Sprite::Render(Window);
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

	// When an overridden method in Ruby calls super(), it is called this way
	VALUE Render(VALUE vWindow) {
		Sprite::Render(*(RenderWindow *)DATA_PTR(vWindow));
		return Qnil;
	}
};


VALUE g_cSprite;

void Sprite_free(void *p) { delete (Sprite *)p; }
VALUE Sprite_new(int argc, VALUE *argv, VALUE vClass) {
	rSprite *ptr = new rSprite();
	VALUE tData = Data_Wrap_Struct(vClass, 0, Sprite_free, ptr);
	ptr->rInit(tData);
	rb_obj_call_init(tData, argc, argv);
	return tData;
}

static VALUE Sprite_initialize(int argc, VALUE *argv, VALUE vSelf) {
	Sprite *pSelf;
	Data_Get_Struct(vSelf, Sprite, pSelf);
	if(argc == 0) {
		// Nothing to initialize
	} else if(argc >= 1 && argc <= 6 && IS(argv[0], g_cImage) && (argc < 6 || IS(argv[5], g_cColor))) {
		pSelf->SetImage(*(Image *)DATA_PTR(argv[0]));
		if(argc >= 2)
			pSelf->SetLeft((float)NUM2DBL(argv[1]));
		if(argc >= 3)
			pSelf->SetTop((float)NUM2DBL(argv[2]));
		if(argc >= 4) {
			if(ISNUM(argv[3])) {
				float f = (float)NUM2DBL(argv[3]);
				pSelf->SetScale(f, f);
			} else if(IS(argv[3], rb_cArray)) {
				float x = (float)NUM2DBL(rb_ary_entry(argv[3], 0));
				float y = (float)NUM2DBL(rb_ary_entry(argv[3], 1));
				pSelf->SetScale(x, y);
			}
		}
		if(argc >= 5)
			pSelf->SetRotation((float)NUM2DBL(argv[4]));
		if(argc >= 6)
			pSelf->SetColor(*(Color *)DATA_PTR(argv[5]));
	} else
		rb_raise(rb_eTypeError, "wrong argument type(s)");
	pSelf->SetRotationCenter(pSelf->GetWidth()/2, pSelf->GetHeight()/2);
	return vSelf;
}

static VALUE Sprite_to_s(VALUE vSelf) {
	// Get C++ object pointer from vSelf
	Sprite *pSelf;
	Data_Get_Struct(vSelf, Sprite, pSelf);
	char szBuffer[256];
	sprintf(szBuffer, "Left=%f, Top=%f, Width=%f, Height=%f, Scale=[%f,%f]", pSelf->GetLeft(), pSelf->GetTop(), pSelf->GetWidth(), pSelf->GetHeight(), pSelf->GetScaleX(), pSelf->GetScaleY());
	return rb_str_new2(szBuffer);
}

static VALUE Sprite_set_image(VALUE vSelf, VALUE vImage) {
	// Get C++ object pointer from vSelf
	Sprite *pSelf;
	Data_Get_Struct(vSelf, Sprite, pSelf);
	if(!IS(vImage, g_cImage))
		rb_raise(rb_eTypeError, "wrong argument type(s)");
	pSelf->SetImage(*(Image *)DATA_PTR(vImage));
	return Qnil;
}

static VALUE Sprite_set_subRect(VALUE vSelf, VALUE vRect) {
	// Get C++ object pointer from vSelf
	Sprite *pSelf;
	Data_Get_Struct(vSelf, Sprite, pSelf);
	if(!IS(vRect, g_cIntRect))
		rb_raise(rb_eTypeError, "wrong argument type(s)");
	pSelf->SetSubRect(*(IntRect *)DATA_PTR(vRect));
	return Qnil;
}

static VALUE Sprite_get_image(VALUE vSelf) {
	// Get C++ object pointer from vSelf
	Sprite *pSelf;
	Data_Get_Struct(vSelf, Sprite, pSelf);
	DECLARE_PTR_VAR(Image, Image, (Image *)pSelf->GetImage());
	return vImage;
}

static VALUE Sprite_get_subRect(VALUE vSelf) {
	// Get C++ object pointer from vSelf
	Sprite *pSelf;
	Data_Get_Struct(vSelf, Sprite, pSelf);
	DECLARE_OBJ_VAR(IntRect, Rect, pSelf->GetSubRect());
	return vRect;
}

static VALUE Sprite_get_width(VALUE vSelf) {
	// Get C++ object pointer from vSelf
	Sprite *pSelf;
	Data_Get_Struct(vSelf, Sprite, pSelf);
	return rb_float_new(pSelf->GetWidth());
}

static VALUE Sprite_get_height(VALUE vSelf) {
	// Get C++ object pointer from vSelf
	Sprite *pSelf;
	Data_Get_Struct(vSelf, Sprite, pSelf);
	return rb_float_new(pSelf->GetHeight());
}

static VALUE Sprite_getPixel(VALUE vSelf, VALUE vX, VALUE vY) {
	// Get C++ object pointer from vSelf
	Sprite *pSelf;
	Data_Get_Struct(vSelf, Sprite, pSelf);
	DECLARE_OBJ_VAR(Color, Color, pSelf->GetPixel(NUM2INT(vX), NUM2INT(vY)));
	return vColor;
}

static VALUE Sprite_set_right(VALUE vSelf, VALUE v) {
	// Get C++ object pointer from vSelf
	Sprite *pSelf;
	Data_Get_Struct(vSelf, Sprite, pSelf);
	pSelf->SetLeft((float)NUM2DBL(v) - pSelf->GetWidth());
	return Qnil;
}

static VALUE Sprite_set_bottom(VALUE vSelf, VALUE v) {
	// Get C++ object pointer from vSelf
	Sprite *pSelf;
	Data_Get_Struct(vSelf, Sprite, pSelf);
	pSelf->SetTop((float)NUM2DBL(v) - pSelf->GetHeight());
	return Qnil;
}

static VALUE Sprite_set_x(VALUE vSelf, VALUE v) {
	// Get C++ object pointer from vSelf
	Sprite *pSelf;
	Data_Get_Struct(vSelf, Sprite, pSelf);
	pSelf->SetLeft((float)NUM2DBL(v) - pSelf->GetWidth()*0.5f);
	return Qnil;
}

static VALUE Sprite_set_y(VALUE vSelf, VALUE v) {
	// Get C++ object pointer from vSelf
	Sprite *pSelf;
	Data_Get_Struct(vSelf, Sprite, pSelf);
	pSelf->SetTop((float)NUM2DBL(v) - pSelf->GetHeight()*0.5f);
	return Qnil;
}

static VALUE Sprite_get_right(VALUE vSelf) {
	// Get C++ object pointer from vSelf
	Sprite *pSelf;
	Data_Get_Struct(vSelf, Sprite, pSelf);
	return rb_float_new(pSelf->GetLeft() + pSelf->GetWidth());
}

static VALUE Sprite_get_bottom(VALUE vSelf) {
	// Get C++ object pointer from vSelf
	Sprite *pSelf;
	Data_Get_Struct(vSelf, Sprite, pSelf);
	return rb_float_new(pSelf->GetTop() + pSelf->GetHeight());
}

static VALUE Sprite_get_x(VALUE vSelf) {
	// Get C++ object pointer from vSelf
	Sprite *pSelf;
	Data_Get_Struct(vSelf, Sprite, pSelf);
	return rb_float_new(pSelf->GetLeft() + pSelf->GetWidth()*0.5f);
}

static VALUE Sprite_get_y(VALUE vSelf) {
	// Get C++ object pointer from vSelf
	Sprite *pSelf;
	Data_Get_Struct(vSelf, Sprite, pSelf);
	return rb_float_new(pSelf->GetTop() + pSelf->GetHeight()*0.5f);
}

static VALUE Sprite_get_radius(VALUE vSelf) {
	// Get C++ object pointer from vSelf
	Sprite *pSelf;
	Data_Get_Struct(vSelf, Sprite, pSelf);
	float w = pSelf->GetWidth() * 0.5f;
	float h = pSelf->GetHeight() * 0.5f;
	return rb_float_new(SQRTF(w*w + h*h));
}

static VALUE Sprite_flipX(VALUE vSelf, VALUE vBool) {
	// Get C++ object pointer from vSelf
	Sprite *pSelf;
	Data_Get_Struct(vSelf, Sprite, pSelf);
	pSelf->FlipX(RTEST(vBool));
	return Qnil;
}

static VALUE Sprite_flipY(VALUE vSelf, VALUE vBool) {
	// Get C++ object pointer from vSelf
	Sprite *pSelf;
	Data_Get_Struct(vSelf, Sprite, pSelf);
	pSelf->FlipY(RTEST(vBool));
	return Qnil;
}

static VALUE Sprite_distance(VALUE vSelf, VALUE vOther) {
	if(!ISKO(vOther, g_cSprite))
		rb_raise(rb_eTypeError, "wrong argument type(s)");

	// Get C++ object pointer from vSelf
	Sprite *pSelf;
	Data_Get_Struct(vSelf, Sprite, pSelf);
	Sprite *pOther;
	Data_Get_Struct(vOther, Sprite, pOther);

	// Get distance from center positions
	float dx = (pSelf->GetLeft() + pSelf->GetWidth()*0.5f) -
				(pOther->GetLeft() + pOther->GetWidth()*0.5f);
	float dy = (pSelf->GetTop() + pSelf->GetHeight()*0.5f) -
				(pOther->GetTop() + pOther->GetHeight()*0.5f);
	return rb_float_new(SQRTF(dx*dx + dy*dy));
}

static VALUE Sprite_render(VALUE vSelf, VALUE vWindow) {
	// Get C++ object pointer from vSelf
	rSprite *pSelf;
	Data_Get_Struct(vSelf, rSprite, pSelf);
	return pSelf->Render(vWindow);
}

void Init_Sprite()
{
	g_cSprite = rb_define_class_under(g_vModule, "Sprite", g_cDrawable);
	DEFINE_CLASS_METHOD(Sprite, new, -1);
	DEFINE_INSTANCE_METHOD(Sprite, initialize, -1);

	DEFINE_RW(Sprite, right);		// Right x position
	DEFINE_RW(Sprite, bottom);		// Bottom x position
	DEFINE_RW(Sprite, x);			// Center x postion
	DEFINE_RW(Sprite, y);			// Center y position
	DEFINE_RW(Sprite, image);
	DEFINE_RW(Sprite, subRect);
	DEFINE_GETTER(Sprite, width);
	DEFINE_GETTER2(Sprite, width, w);
	DEFINE_GETTER(Sprite, height);
	DEFINE_GETTER2(Sprite, height, h);
	DEFINE_GETTER(Sprite, radius);

	DEFINE_INSTANCE_METHOD(Sprite, to_s, 0);
	DEFINE_INSTANCE_METHOD(Sprite, getPixel, 2);
	DEFINE_INSTANCE_METHOD2(Sprite, getPixel, [], 2);
	DEFINE_INSTANCE_METHOD(Sprite, distance, 1);
	DEFINE_INSTANCE_METHOD2(Sprite, distance, getDistance, 1);
	DEFINE_INSTANCE_METHOD(Sprite, flipX, 0);
	DEFINE_INSTANCE_METHOD(Sprite, flipY, 0);

	// Virtual method
	DEFINE_INSTANCE_METHOD(Sprite, render, 1);
}
