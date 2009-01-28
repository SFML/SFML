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

VALUE g_cClock;

void Clock_free(void *p) { delete (Clock *)p; }
VALUE Clock_new(int argc, VALUE *argv, VALUE vClass) {
	Clock *ptr = new Clock();
	VALUE tData = Data_Wrap_Struct(vClass, 0, Clock_free, ptr);
	rb_obj_call_init(tData, argc, argv);
	return tData;
}

static VALUE Clock_initialize(int argc, VALUE *argv, VALUE vSelf) {
	// Get C++ object pointer from vSelf
	Clock *pSelf;
	Data_Get_Struct(vSelf, Clock, pSelf);
	if(argc == 0) {
		// Nothing to initialize
	} else
		rb_raise(rb_eTypeError, "wrong argument type(s)");
	return vSelf;
}

static VALUE Clock_to_s(VALUE vSelf) {
	// Get C++ object pointer from vSelf
	Clock *pSelf;
	Data_Get_Struct(vSelf, Clock, pSelf);
	char szBuffer[256];
	sprintf(szBuffer, "Elapsed Time=%f", pSelf->GetElapsedTime());
	return rb_str_new2(szBuffer);
}

static VALUE Clock_get_elapsedTime(VALUE vSelf) {
	// Get C++ object pointer from vSelf
	Clock *pSelf;
	Data_Get_Struct(vSelf, Clock, pSelf);
	return rb_float_new(pSelf->GetElapsedTime());
}

static VALUE Clock_reset(VALUE vSelf) {
	// Get C++ object pointer from vSelf
	Clock *pSelf;
	Data_Get_Struct(vSelf, Clock, pSelf);
	pSelf->Reset();
	return Qnil;
}

void Init_Clock()
{
	g_cClock = rb_define_class_under(g_vModule, "Clock", rb_cObject);
	DEFINE_CLASS_METHOD(Clock, new, -1);
	DEFINE_INSTANCE_METHOD(Clock, initialize, -1);

	DEFINE_GETTER(Clock, elapsedTime);

	DEFINE_INSTANCE_METHOD(Clock, to_s, 0);
	DEFINE_INSTANCE_METHOD2(Clock, get_elapsedTime, to_f, 0);
	DEFINE_INSTANCE_METHOD(Clock, reset, 0);
}

