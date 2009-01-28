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

VALUE g_cFloatRect;

DECLARE_DOUBLE_RW(FloatRect, Left);
DECLARE_DOUBLE_RW(FloatRect, Top);
DECLARE_DOUBLE_RW(FloatRect, Right);
DECLARE_DOUBLE_RW(FloatRect, Bottom);


void FloatRect_free(void *p) { delete (FloatRect *)p; }
VALUE FloatRect_new(int argc, VALUE *argv, VALUE vClass) {
	FloatRect *ptr = new FloatRect();
	VALUE tData = Data_Wrap_Struct(vClass, 0, FloatRect_free, ptr);
	rb_obj_call_init(tData, argc, argv);
	return tData;
}

static VALUE FloatRect_initialize(int argc, VALUE *argv, VALUE vSelf) {
	// Get C++ object pointer from vSelf
	FloatRect *pSelf;
	Data_Get_Struct(vSelf, FloatRect, pSelf);
	if(argc == 0) {
		// Nothing to initialize
	} else if(argc == 4 &&
			ISNUM(argv[0]) &&
			ISNUM(argv[1]) &&
			ISNUM(argv[2]) &&
			ISNUM(argv[3])) {
		pSelf->Left = (float)NUM2DBL(argv[0]);
		pSelf->Top = (float)NUM2DBL(argv[1]);
		pSelf->Right = (float)NUM2DBL(argv[2]);
		pSelf->Bottom = (float)NUM2DBL(argv[3]);
	} else
		rb_raise(rb_eTypeError, "wrong argument type(s)");
	return vSelf;
}

static VALUE FloatRect_to_s(VALUE vSelf)
{
	// Get C++ object pointer from vSelf
	FloatRect *pSelf;
	Data_Get_Struct(vSelf, FloatRect, pSelf);
	char szBuffer[256];
	sprintf(szBuffer, "Left=%f, Top=%f, Right=%f, Bottom=%f", pSelf->Left, pSelf->Top, pSelf->Right, pSelf->Bottom);
	return rb_str_new2(szBuffer);
}

static VALUE FloatRect_get_Width(VALUE vSelf)
{
	// Get C++ object pointer from vSelf
	FloatRect *pSelf;
	Data_Get_Struct(vSelf, FloatRect, pSelf);
	return rb_float_new(pSelf->GetWidth());
}

static VALUE FloatRect_set_Width(VALUE vSelf, VALUE vWidth)
{
	// Get C++ object pointer from vSelf
	FloatRect *pSelf;
	Data_Get_Struct(vSelf, FloatRect, pSelf);
	float nWidth = (float)NUM2DBL(vWidth);
	pSelf->Right = pSelf->Left + nWidth;
	return vWidth;
}

static VALUE FloatRect_get_Height(VALUE vSelf)
{
	// Get C++ object pointer from vSelf
	FloatRect *pSelf;
	Data_Get_Struct(vSelf, FloatRect, pSelf);
	return rb_float_new(pSelf->GetHeight());
}

static VALUE FloatRect_set_Height(VALUE vSelf, VALUE vHeight)
{
	// Get C++ object pointer from vSelf
	FloatRect *pSelf;
	Data_Get_Struct(vSelf, FloatRect, pSelf);
	float nHeight = (float)NUM2DBL(vHeight);
	pSelf->Bottom = pSelf->Top + nHeight;
	return vHeight;
}

static VALUE FloatRect_contains(VALUE vSelf, VALUE x, VALUE y)
{
	// Get C++ object pointer from vSelf
	FloatRect *pSelf;
	Data_Get_Struct(vSelf, FloatRect, pSelf);
	return pSelf->Contains((float)NUM2DBL(x), (float)NUM2DBL(y)) ? Qtrue : Qfalse;
}

static VALUE FloatRect_intersects(VALUE vSelf, VALUE vRect)
{
	// Get C++ object pointer from vSelf
	FloatRect *pSelf = NULL, *pRect = NULL;
	FloatRect r;
	Data_Get_Struct(vSelf, FloatRect, pSelf);
	Data_Get_Struct(vRect, FloatRect, pRect);
	if(!pSelf->Intersects(*pRect, &r))
		return Qnil;
	DECLARE_OBJ_VAR(FloatRect, Result, r);
	return vResult;
}

void Init_FloatRect()
{
	g_cFloatRect = rb_define_class_under(g_vModule, "FloatRect", rb_cObject);
	DEFINE_CLASS_METHOD(FloatRect, new, -1);
	DEFINE_INSTANCE_METHOD(FloatRect, initialize, -1);

	DEFINE_RW2(FloatRect, Left, left);
	DEFINE_RW2(FloatRect, Left, l);
	DEFINE_RW2(FloatRect, Top, top);
	DEFINE_RW2(FloatRect, Top, t);
	DEFINE_RW2(FloatRect, Right, right);
	DEFINE_RW2(FloatRect, Right, r);
	DEFINE_RW2(FloatRect, Bottom, bottom);
	DEFINE_RW2(FloatRect, Bottom, b);
	DEFINE_RW2(FloatRect, Width, width);
	DEFINE_RW2(FloatRect, Width, w);
	DEFINE_RW2(FloatRect, Height, height);
	DEFINE_RW2(FloatRect, Height, h);

	DEFINE_INSTANCE_METHOD(FloatRect, to_s, 0);
	DEFINE_INSTANCE_METHOD(FloatRect, contains, 2);
	DEFINE_INSTANCE_METHOD(FloatRect, intersects, 1);
}

