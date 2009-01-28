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

VALUE g_cColor;

DECLARE_STATIC_OBJ_GETTER(Color, Black, Color);
DECLARE_STATIC_OBJ_GETTER(Color, White, Color);
DECLARE_STATIC_OBJ_GETTER(Color, Red, Color);
DECLARE_STATIC_OBJ_GETTER(Color, Green, Color);
DECLARE_STATIC_OBJ_GETTER(Color, Blue, Color);
DECLARE_STATIC_OBJ_GETTER(Color, Yellow, Color);
DECLARE_STATIC_OBJ_GETTER(Color, Magenta, Color);
DECLARE_STATIC_OBJ_GETTER(Color, Cyan, Color);

DECLARE_INT_RW(Color, r);
DECLARE_INT_RW(Color, g);
DECLARE_INT_RW(Color, b);
DECLARE_INT_RW(Color, a);

void Color_free(void *p) { delete (Color *)p; }
VALUE Color_new(int argc, VALUE *argv, VALUE vClass) {
	Color *ptr = new Color();
	VALUE tData = Data_Wrap_Struct(vClass, 0, Color_free, ptr);
	rb_obj_call_init(tData, argc, argv);
	return tData;
}

static VALUE Color_initialize(int argc, VALUE *argv, VALUE vSelf) {
	// Get C++ object pointer from vSelf
	Color *pSelf;
	Data_Get_Struct(vSelf, Color, pSelf);
	if(argc == 0) {
		// Nothing to initialize
	} else if(argc == 1 && ISNUM(argv[0])) {
		DWORD dw = NUM2UINT(argv[0]);
		*pSelf = Color(
			(dw >> 24) & 0xFF,
			(dw >> 16) & 0xFF,
			(dw >> 8) & 0xFF,
			(dw >> 0) & 0xFF);
	} else if(argc >= 3 && argc <= 4 &&
			ISNUM(argv[0]) && ISNUM(argv[1]) && ISNUM(argv[2]) &&
			(argc < 4 || ISNUM(argv[3]))) {
		pSelf->r = (unsigned char)NUM2INT(argv[0]);
		pSelf->g = (unsigned char)NUM2INT(argv[1]);
		pSelf->b = (unsigned char)NUM2INT(argv[2]);
		pSelf->a = argc < 4 ? 255 : (unsigned char)NUM2INT(argv[3]);
	} else
		rb_raise(rb_eTypeError, "wrong argument type(s)");
	return vSelf;
}

static VALUE Color_to_s(VALUE vSelf)
{
	// Get C++ object pointer from vSelf
	Color *pSelf;
	Data_Get_Struct(vSelf, Color, pSelf);
	char szBuffer[256];
	sprintf(szBuffer, "r=%d, g=%d, b=%d, a=%d", pSelf->r, pSelf->g, pSelf->b, pSelf->a);
	return rb_str_new2(szBuffer);
}

static VALUE Color_toRGBA(VALUE vSelf)
{
	// Get C++ object pointer from vSelf
	Color *pSelf;
	Data_Get_Struct(vSelf, Color, pSelf);
	return UINT2NUM((pSelf->r << 24) | (pSelf->g << 16) | (pSelf->b << 8) | pSelf->a);
}

void Init_Color()
{
	g_cColor = rb_define_class_under(g_vModule, "Color", rb_cObject);
	DEFINE_CLASS_METHOD(Color, new, -1);
	DEFINE_INSTANCE_METHOD(Color, initialize, -1);

	DEFINE_STATIC_GETTER(Color, Black);
	DEFINE_STATIC_GETTER(Color, White);
	DEFINE_STATIC_GETTER(Color, Red);
	DEFINE_STATIC_GETTER(Color, Green);
	DEFINE_STATIC_GETTER(Color, Blue);
	DEFINE_STATIC_GETTER(Color, Yellow);
	DEFINE_STATIC_GETTER(Color, Magenta);
	DEFINE_STATIC_GETTER(Color, Cyan);

	DEFINE_RW(Color, r);
	DEFINE_RW(Color, g);
	DEFINE_RW(Color, b);
	DEFINE_RW(Color, a);

	DEFINE_INSTANCE_METHOD(Color, to_s, 0);
	DEFINE_INSTANCE_METHOD(Color, toRGBA, 0);
	DEFINE_INSTANCE_METHOD2(Color, toRGBA, to_i, 0);
}

