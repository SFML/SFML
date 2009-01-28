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

VALUE g_cImage;

void Image_free(void *p) { delete (Image *)p; }
VALUE Image_new(int argc, VALUE *argv, VALUE vClass) {
	Image *ptr = new Image();
	VALUE tData = Data_Wrap_Struct(vClass, 0, Image_free, ptr);
	rb_obj_call_init(tData, argc, argv);
	return tData;
}

static VALUE Image_initialize(int argc, VALUE *argv, VALUE vSelf) {
	// Get C++ object pointer from vSelf
	Image *pSelf;
	Data_Get_Struct(vSelf, Image, pSelf);
	if(argc == 0) {
		// Nothing to initialize
	} else if(argc == 1 && IS(argv[0], g_cImage)) {
		*pSelf = *(Image *)DATA_PTR(argv[0]);
	} else if(argc == 1 && ISSTR(argv[0])) {
		if(!pSelf->LoadFromFile(STR2CSTR(argv[0])))
			rb_raise(rb_eRuntimeError, "Failed to load specified image file");
	} else if(argc >= 2 && argc <= 3 &&
			ISNUM(argv[0]) &&
			ISNUM(argv[1]) &&
			(argc < 3 || IS(argv[2], g_cColor))) {
		pSelf->Create(NUM2INT(argv[0]), NUM2INT(argv[1]),
			argc < 3 ? Color::Black : *(Color *)DATA_PTR(argv[2]));
	} else if(argc == 2 &&
			ISSTR(argv[0]) &&
			ISNUM(argv[1])) {
		pSelf->LoadFromMemory(STR2CSTR(argv[2]), NUM2INT(argv[0]));
	} else
		rb_raise(rb_eTypeError, "wrong argument type(s)");
	return vSelf;
}

static VALUE Image_to_s(VALUE vSelf) {
	// Get C++ object pointer from vSelf
	Image *pSelf;
	Data_Get_Struct(vSelf, Image, pSelf);
	char szBuffer[256];
	sprintf(szBuffer, "Width=%d, Height=%d", pSelf->GetWidth(), pSelf->GetHeight());
	return rb_str_new2(szBuffer);
}

static VALUE Image_loadFromFile(VALUE vSelf, VALUE vPath) {
	// Get C++ object pointer from vSelf
	Image *pSelf;
	Data_Get_Struct(vSelf, Image, pSelf);
	return pSelf->LoadFromFile(STR2CSTR(vPath)) ? Qtrue : Qfalse;
}

static VALUE Image_saveToFile(VALUE vSelf, VALUE vPath) {
	// Get C++ object pointer from vSelf
	Image *pSelf;
	Data_Get_Struct(vSelf, Image, pSelf);
	return pSelf->SaveToFile(STR2CSTR(vPath)) ? Qtrue : Qfalse;
}

static VALUE Image_create(VALUE vSelf, VALUE vWidth, VALUE vHeight, VALUE vColor) {
	// Get C++ object pointer from vSelf
	Image *pSelf;
	Data_Get_Struct(vSelf, Image, pSelf);
	if(!IS(vColor, g_cColor))
		rb_raise(rb_eTypeError, "wrong argument type(s)");
	return pSelf->Create(NUM2INT(vWidth), NUM2INT(vHeight), *(Color *)DATA_PTR(vColor));
}

static VALUE Image_loadFromMemory(VALUE vSelf, VALUE vData, VALUE vSize) {
	// Get C++ object pointer from vSelf
	Image *pSelf;
	Data_Get_Struct(vSelf, Image, pSelf);
	return pSelf->LoadFromMemory(STR2CSTR(vData), NUM2INT(vSize));
}

static VALUE Image_createMaskFromColor(VALUE vSelf, VALUE vColor) {
	// Get C++ object pointer from vSelf
	Image *pSelf;
	Data_Get_Struct(vSelf, Image, pSelf);
	if(IS(vColor, g_cColor))
		pSelf->CreateMaskFromColor(*(Color *)DATA_PTR(vColor));
	else if(ISNUM(vColor)) {
		DWORD dw = NUM2UINT(vColor);
		Color c((dw >> 24) & 0xFF, (dw >> 16) & 0xFF, (dw >> 8) & 0xFF, (dw >> 0) & 0xFF);
		pSelf->CreateMaskFromColor(c);
	} else
		rb_raise(rb_eTypeError, "wrong argument type(s)");
	return Qnil;
}

static VALUE Image_resize(VALUE vSelf, VALUE vWidth, VALUE vHeight, VALUE vColor) {
	// Get C++ object pointer from vSelf
	Image *pSelf;
	Data_Get_Struct(vSelf, Image, pSelf);
	if(IS(vColor, g_cColor))
		return pSelf->Resize(NUM2INT(vWidth), NUM2INT(vHeight), *(Color *)DATA_PTR(vColor));
	else if(ISNUM(vColor)) {
		DWORD dw = NUM2UINT(vColor);
		Color c((dw >> 24) & 0xFF, (dw >> 16) & 0xFF, (dw >> 8) & 0xFF, (dw >> 0) & 0xFF);
		return pSelf->Resize(NUM2INT(vWidth), NUM2INT(vHeight), c);
	} else
		rb_raise(rb_eTypeError, "wrong argument type(s)");
	return Qnil;
}

static VALUE Image_setPixel(VALUE vSelf, VALUE vX, VALUE vY, VALUE vColor) {
	// Get C++ object pointer from vSelf
	Image *pSelf;
	Data_Get_Struct(vSelf, Image, pSelf);
	if(IS(vColor, g_cColor))
		pSelf->SetPixel(NUM2INT(vX), NUM2INT(vY), *(Color *)DATA_PTR(vColor));
	else if(ISNUM(vColor)) {
		DWORD dw = NUM2UINT(vColor);
		Color c((dw >> 24) & 0xFF, (dw >> 16) & 0xFF, (dw >> 8) & 0xFF, (dw >> 0) & 0xFF);
		pSelf->SetPixel(NUM2INT(vX), NUM2INT(vY), c);
	} else
		rb_raise(rb_eTypeError, "wrong argument type(s)");
	return Qnil;
}

static VALUE Image_getPixel(VALUE vSelf, VALUE vX, VALUE vY) {
	// Get C++ object pointer from vSelf
	Image *pSelf;
	Data_Get_Struct(vSelf, Image, pSelf);
	DECLARE_OBJ_VAR(Color, Color, pSelf->GetPixel(NUM2INT(vX), NUM2INT(vY)));
	return vColor;
}

static VALUE Image_bind(VALUE vSelf) {
	// Get C++ object pointer from vSelf
	Image *pSelf;
	Data_Get_Struct(vSelf, Image, pSelf);
	pSelf->Bind();
	return Qnil;
}

static VALUE Image_set_smooth(VALUE vSelf, VALUE vSmooth) {
	// Get C++ object pointer from vSelf
	Image *pSelf;
	Data_Get_Struct(vSelf, Image, pSelf);
	pSelf->SetSmooth(RTEST(vSmooth));
	return Qnil;
}

static VALUE Image_set_repeat(VALUE vSelf, VALUE vRepeat) {
	// Get C++ object pointer from vSelf
	Image *pSelf;
	Data_Get_Struct(vSelf, Image, pSelf);
	pSelf->SetRepeat(RTEST(vRepeat));
	return Qnil;
}

static VALUE Image_get_width(VALUE vSelf) {
	// Get C++ object pointer from vSelf
	Image *pSelf;
	Data_Get_Struct(vSelf, Image, pSelf);
	return INT2FIX(pSelf->GetWidth());
}

static VALUE Image_get_height(VALUE vSelf) {
	// Get C++ object pointer from vSelf
	Image *pSelf;
	Data_Get_Struct(vSelf, Image, pSelf);
	return INT2FIX(pSelf->GetHeight());
}

static VALUE Image_getTexCoords(VALUE vSelf, VALUE vRect) {
	// Get C++ object pointer from vSelf
	Image *pSelf;
	Data_Get_Struct(vSelf, Image, pSelf);
	if(!IS(vRect, g_cIntRect))
		rb_raise(rb_eTypeError, "wrong argument type(s)");
	DECLARE_OBJ_VAR(FloatRect, Coord, pSelf->GetTexCoords(*(IntRect *)DATA_PTR(vRect)));
	return vCoord;
}

static VALUE Image_getValidTextureSize(VALUE vClass, VALUE vSize) {
	// Get C++ object pointer from vSelf
	return INT2FIX(Image::GetValidTextureSize(NUM2INT(vSize)));
}

void Init_Image()
{
	g_cImage = rb_define_class_under(g_vModule, "Image", rb_cObject);
	DEFINE_CLASS_METHOD(Image, new, -1);
	DEFINE_CLASS_METHOD(Image, getValidTextureSize, 1);
	DEFINE_INSTANCE_METHOD(Image, initialize, -1);

	DEFINE_SETTER(Image, smooth);
	DEFINE_SETTER(Image, repeat);
	DEFINE_GETTER(Image, width);
	DEFINE_GETTER2(Image, width, w);
	DEFINE_GETTER(Image, height);
	DEFINE_GETTER2(Image, height, h);

	DEFINE_INSTANCE_METHOD(Image, to_s, 0);
	DEFINE_INSTANCE_METHOD(Image, loadFromFile, 1);
	DEFINE_INSTANCE_METHOD(Image, saveToFile, 1);
	DEFINE_INSTANCE_METHOD(Image, create, 3);
	DEFINE_INSTANCE_METHOD(Image, loadFromMemory, 2);
	DEFINE_INSTANCE_METHOD(Image, createMaskFromColor, 1);
	DEFINE_INSTANCE_METHOD(Image, resize, 3);
	DEFINE_INSTANCE_METHOD(Image, setPixel, 3);
	DEFINE_INSTANCE_METHOD(Image, getPixel, 2);
	DEFINE_INSTANCE_METHOD(Image, bind, 0);
	DEFINE_INSTANCE_METHOD(Image, getTexCoords, 1);
	DEFINE_INSTANCE_METHOD2(Image, getPixel, [], 2);
	DEFINE_INSTANCE_METHOD2(Image, setPixel, []=, 2);
}

