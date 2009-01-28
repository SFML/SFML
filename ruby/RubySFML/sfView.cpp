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

VALUE g_cView;

//DECLARE_DOUBLE_RW(View, Left);
//DECLARE_DOUBLE_RW(View, Top);
//DECLARE_DOUBLE_RW(View, Width);
//DECLARE_DOUBLE_RW(View, Height);
DECLARE_OBJ_RW(View, Rect, FloatRect);
DECLARE_DOUBLE_RW(View, Zoom);


void View_free(void *p) { delete (View *)p; }
VALUE View_new(int argc, VALUE *argv, VALUE vClass) {
	View *ptr = new View();
	VALUE tData = Data_Wrap_Struct(vClass, 0, View_free, ptr);
	rb_obj_call_init(tData, argc, argv);
	return tData;
}

static VALUE View_initialize(int argc, VALUE *argv, VALUE vSelf) {
	// Get C++ object pointer from vSelf
	View *pSelf;
	Data_Get_Struct(vSelf, View, pSelf);
	if(argc == 0) {
		// Nothing to initialize
	} else if(argc >= 1 && argc <= 2 &&
			IS(argv[0], g_cFloatRect) &&
			(argc < 2 || ISNUM(argv[1]))) {
		pSelf->Rect = *(FloatRect *)DATA_PTR(argv[0]);
		if(argc >= 2)
			pSelf->Zoom = (float)NUM2DBL(argv[1]);
	} else
		rb_raise(rb_eTypeError, "wrong argument type(s)");

	return vSelf;
}

static VALUE View_to_s(VALUE vSelf)
{
	// Get C++ object pointer from vSelf
	View *pSelf;
	Data_Get_Struct(vSelf, View, pSelf);
	char szBuffer[256];
	sprintf(szBuffer, "Left=%f, Top=%f, Width=%f, Height=%f, Zoom=%f", pSelf->Rect.Left, pSelf->Rect.Top, pSelf->Rect.GetHeight(), pSelf->Rect.GetWidth(), pSelf->Zoom);
	return rb_str_new2(szBuffer);
}

void Init_View()
{
	g_cView = rb_define_class_under(g_vModule, "View", rb_cObject);
	DEFINE_CLASS_METHOD(View, new, -1);
	DEFINE_INSTANCE_METHOD(View, initialize, -1);

	//DEFINE_RW2(View, Left, l);
	//DEFINE_RW2(View, Top, top);
	//DEFINE_RW2(View, Top, t);
	//DEFINE_RW2(View, Width, width);
	//DEFINE_RW2(View, Width, w);
	//DEFINE_RW2(View, Height, height);
	//DEFINE_RW2(View, Height, h);
	DEFINE_RW2(View, Rect, rect);
	DEFINE_RW2(View, Zoom, zoom);

	DEFINE_INSTANCE_METHOD(View, to_s, 0);
}

