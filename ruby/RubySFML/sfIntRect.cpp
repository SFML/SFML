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

VALUE g_cIntRect;

DECLARE_INT_RW(IntRect, Left);
DECLARE_INT_RW(IntRect, Top);
DECLARE_INT_RW(IntRect, Right);
DECLARE_INT_RW(IntRect, Bottom);


void IntRect_free(void *p) { delete (IntRect *)p; }
VALUE IntRect_new(int argc, VALUE *argv, VALUE vClass) {
	// For each version of this method, convert Ruby args to C++ types (applying default values)
	IntRect *ptr = new IntRect();
	VALUE tData = Data_Wrap_Struct(vClass, 0, IntRect_free, ptr);
	rb_obj_call_init(tData, argc, argv);
	return tData;
}

static VALUE IntRect_initialize(int argc, VALUE *argv, VALUE vSelf) {
	// Get C++ object pointer from vSelf
	IntRect *pSelf;
	Data_Get_Struct(vSelf, IntRect, pSelf);
	if(argc == 0) {
		// Nothing to initialize
	} else if(argc == 4 &&
			ISNUM(argv[0]) &&
			ISNUM(argv[1]) &&
			ISNUM(argv[2]) &&
			ISNUM(argv[3])) {
		pSelf->Left = NUM2INT(argv[0]);
		pSelf->Top = NUM2INT(argv[1]);
		pSelf->Right = NUM2INT(argv[2]);
		pSelf->Bottom = NUM2INT(argv[3]);
	} else
		rb_raise(rb_eTypeError, "wrong argument type(s)");

	return vSelf;
}

static VALUE IntRect_to_s(VALUE vSelf)
{
	// Get C++ object pointer from vSelf
	IntRect *pSelf;
	Data_Get_Struct(vSelf, IntRect, pSelf);
	char szBuffer[256];
	sprintf(szBuffer, "Left=%d, Top=%d, Right=%d, Bottom=%d", pSelf->Left, pSelf->Top, pSelf->Right, pSelf->Bottom);
	return rb_str_new2(szBuffer);
}

static VALUE IntRect_get_Width(VALUE vSelf)
{
	// Get C++ object pointer from vSelf
	IntRect *pSelf;
	Data_Get_Struct(vSelf, IntRect, pSelf);
	return INT2NUM(pSelf->GetWidth());
}

static VALUE IntRect_set_Width(VALUE vSelf, VALUE vWidth)
{
	// Get C++ object pointer from vSelf
	IntRect *pSelf;
	Data_Get_Struct(vSelf, IntRect, pSelf);
	int nWidth = NUM2INT(vWidth);
	pSelf->Right = pSelf->Left + nWidth;
	return vWidth;
}

static VALUE IntRect_get_Height(VALUE vSelf)
{
	// Get C++ object pointer from vSelf
	IntRect *pSelf;
	Data_Get_Struct(vSelf, IntRect, pSelf);
	return INT2NUM(pSelf->GetHeight());
}

static VALUE IntRect_set_Height(VALUE vSelf, VALUE vHeight)
{
	// Get C++ object pointer from vSelf
	IntRect *pSelf;
	Data_Get_Struct(vSelf, IntRect, pSelf);
	int nHeight = NUM2INT(vHeight);
	pSelf->Bottom = pSelf->Top + nHeight;
	return vHeight;
}

static VALUE IntRect_contains(VALUE vSelf, VALUE x, VALUE y)
{
	// Get C++ object pointer from vSelf
	IntRect *pSelf;
	Data_Get_Struct(vSelf, IntRect, pSelf);
	return pSelf->Contains(NUM2INT(x), NUM2INT(y)) ? Qtrue : Qfalse;
}

static VALUE IntRect_intersects(VALUE vSelf, VALUE vRect)
{
	// Get C++ object pointer from vSelf
	IntRect *pSelf = NULL, *pRect = NULL;
	IntRect r;
	Data_Get_Struct(vSelf, IntRect, pSelf);
	Data_Get_Struct(vRect, IntRect, pRect);
	if(!pSelf->Intersects(*pRect, &r))
		return Qnil;
	DECLARE_OBJ_VAR(IntRect, Result, r);
	return vResult;
}

void Init_IntRect()
{
	g_cIntRect = rb_define_class_under(g_vModule, "IntRect", rb_cObject);
	DEFINE_CLASS_METHOD(IntRect, new, -1);
	DEFINE_INSTANCE_METHOD(IntRect, initialize, -1);

	DEFINE_RW2(IntRect, Left, left);
	DEFINE_RW2(IntRect, Left, l);
	DEFINE_RW2(IntRect, Top, top);
	DEFINE_RW2(IntRect, Top, t);
	DEFINE_RW2(IntRect, Right, right);
	DEFINE_RW2(IntRect, Right, r);
	DEFINE_RW2(IntRect, Bottom, bottom);
	DEFINE_RW2(IntRect, Bottom, b);
	DEFINE_RW2(IntRect, Width, width);
	DEFINE_RW2(IntRect, Width, w);
	DEFINE_RW2(IntRect, Height, height);
	DEFINE_RW2(IntRect, Height, h);

	DEFINE_INSTANCE_METHOD(IntRect, to_s, 0);
	DEFINE_INSTANCE_METHOD(IntRect, contains, 2);
	DEFINE_INSTANCE_METHOD(IntRect, intersects, 1);
}

