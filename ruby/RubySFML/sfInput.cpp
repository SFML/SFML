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

VALUE g_cInput;

void Input_free(void *p) { delete (Input *)p; }
VALUE Input_new(int argc, VALUE *argv, VALUE vClass) {
	Input *ptr = new Input();
	VALUE tData = Data_Wrap_Struct(vClass, 0, Input_free, ptr);
	rb_obj_call_init(tData, argc, argv);
	return tData;
}

static VALUE Input_initialize(int argc, VALUE *argv, VALUE vSelf) {
	// Get C++ object pointer from vSelf
	Input *pSelf;
	Data_Get_Struct(vSelf, Input, pSelf);
	if(argc == 0) {
		// Nothing to initialize
	} else
		rb_raise(rb_eTypeError, "wrong argument type(s)");
	return vSelf;
}

static VALUE Input_isKeyDown(VALUE vSelf, VALUE vKey) {
	// Get C++ object pointer from vSelf
	Input *pSelf;
	Data_Get_Struct(vSelf, Input, pSelf);
	return pSelf->IsKeyDown((Key::Code)NUM2INT(vKey)) ? Qtrue : Qfalse;
}

static VALUE Input_isMouseButtonDown(VALUE vSelf, VALUE vButton) {
	// Get C++ object pointer from vSelf
	Input *pSelf;
	Data_Get_Struct(vSelf, Input, pSelf);
	return pSelf->IsMouseButtonDown((Mouse::Button)NUM2INT(vButton)) ? Qtrue : Qfalse;
}

static VALUE Input_isJoystickButtonDown(VALUE vSelf, VALUE vJoy, VALUE vButton) {
	// Get C++ object pointer from vSelf
	Input *pSelf;
	Data_Get_Struct(vSelf, Input, pSelf);
	return pSelf->IsJoystickButtonDown(NUM2INT(vJoy), NUM2INT(vButton)) ? Qtrue : Qfalse;
}

static VALUE Input_getMouseX(VALUE vSelf) {
	// Get C++ object pointer from vSelf
	Input *pSelf;
	Data_Get_Struct(vSelf, Input, pSelf);
	return INT2FIX(pSelf->GetMouseX());
}

static VALUE Input_getMouseY(VALUE vSelf) {
	// Get C++ object pointer from vSelf
	Input *pSelf;
	Data_Get_Struct(vSelf, Input, pSelf);
	return INT2FIX(pSelf->GetMouseY());
}

static VALUE Input_getJoystickAxis(VALUE vSelf, VALUE vJoy, VALUE vAxis) {
	// Get C++ object pointer from vSelf
	Input *pSelf;
	Data_Get_Struct(vSelf, Input, pSelf);
	return INT2FIX(pSelf->GetJoystickAxis(NUM2INT(vJoy), (Joy::Axis)NUM2INT(vAxis)));
}

void Init_Input()
{
	g_cInput = rb_define_class_under(g_vModule, "Input", rb_cObject);
	DEFINE_CLASS_METHOD(Input, new, -1);
	DEFINE_INSTANCE_METHOD(Input, initialize, -1);

	DEFINE_INSTANCE_METHOD(Input, isKeyDown, 1);
	DEFINE_INSTANCE_METHOD(Input, isMouseButtonDown, 1);
	DEFINE_INSTANCE_METHOD(Input, isJoystickButtonDown, 2);
	DEFINE_INSTANCE_METHOD(Input, getMouseX, 0);
	DEFINE_INSTANCE_METHOD(Input, getMouseY, 0);
	DEFINE_INSTANCE_METHOD(Input, getJoystickAxis, 2);
}

