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

VALUE g_cVideoMode;

DECLARE_INT_RW(VideoMode, Width);
DECLARE_INT_RW(VideoMode, Height);
DECLARE_INT_RW(VideoMode, BitsPerPixel);


void VideoMode_free(void *p) { delete (VideoMode *)p; }
VALUE VideoMode_new(int argc, VALUE *argv, VALUE vClass) {
	VideoMode *ptr = new VideoMode();
	VALUE tData = Data_Wrap_Struct(vClass, 0, VideoMode_free, ptr);
	rb_obj_call_init(tData, argc, argv);
	return tData;
}

static VALUE VideoMode_initialize(int argc, VALUE *argv, VALUE vSelf) {
	// Get C++ object pointer from vSelf
	VideoMode *pSelf;
	Data_Get_Struct(vSelf, VideoMode, pSelf);
	if(argc == 0) {
		// Nothing to initialize
	} else if(argc >= 2 && argc <= 3 &&
			ISNUM(argv[0]) &&
			ISNUM(argv[1]) &&
			(argc < 3 || ISNUM(argv[2]))) {
		*pSelf = VideoMode(NUM2INT(argv[0]), NUM2INT(argv[1]), argc < 3 ? 32 : NUM2INT(argv[2]));
	} else
		rb_raise(rb_eTypeError, "wrong argument type(s)");
	return vSelf;
}

static VALUE VideoMode_desktop(VALUE vClass)
{
	DECLARE_OBJ_VAR(VideoMode, Desktop, VideoMode::GetDesktopMode());
	return vDesktop;
}

// Ruby each iterator
static VALUE VideoMode_each(VALUE vClass)
{
	int nLength = VideoMode::GetModesCount();
	for(int i=0; i<nLength; i++) {
		VideoMode mode = VideoMode::GetMode(i);
		DECLARE_PTR_VAR(VideoMode, Element, &mode);
		rb_yield(vElement);
	}
	return Qnil;
}

static VALUE VideoMode_isValid(VALUE vSelf)
{
	// Get C++ object pointer from vSelf
	VideoMode *pSelf;
	Data_Get_Struct(vSelf, VideoMode, pSelf);
	return pSelf->IsValid() ? Qtrue : Qfalse;
}

static VALUE VideoMode_to_s(VALUE vSelf)
{
	// Get C++ object pointer from vSelf
	VideoMode *pSelf;
	Data_Get_Struct(vSelf, VideoMode, pSelf);
	char szBuffer[256];
	sprintf(szBuffer, "Width: %d, Height: %d, BPP: %d", pSelf->Width, pSelf->Height, pSelf->BitsPerPixel);
	return rb_str_new2(szBuffer);
}

void Init_VideoMode()
{
	g_cVideoMode = rb_define_class_under(g_vModule, "VideoMode", rb_cObject);
	DEFINE_CLASS_METHOD(VideoMode, new, -1);
	DEFINE_INSTANCE_METHOD(VideoMode, initialize, -1);

	DEFINE_RW2(VideoMode, Width, width);
	DEFINE_RW2(VideoMode, Width, w);
	DEFINE_RW2(VideoMode, Height, height);
	DEFINE_RW2(VideoMode, Height, h);
	DEFINE_RW2(VideoMode, BitsPerPixel, bitsPerPixel);
	DEFINE_RW2(VideoMode, BitsPerPixel, bpp);

	DEFINE_CLASS_METHOD(VideoMode, each, 0);
	DEFINE_CLASS_METHOD(VideoMode, desktop, 0);
	DEFINE_INSTANCE_METHOD(VideoMode, isValid, 0);
	DEFINE_INSTANCE_METHOD(VideoMode, to_s, 0);
}

