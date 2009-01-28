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
#include <SFML/Audio.hpp>
#include "RubySFML.h"
using namespace sf;

VALUE g_cSoundBufferRecorder;

void SoundBufferRecorder_free(void *p) { delete (SoundBufferRecorder *)p; }
VALUE SoundBufferRecorder_new(int argc, VALUE *argv, VALUE vClass) {
	SoundBufferRecorder *ptr = new SoundBufferRecorder();
	VALUE tData = Data_Wrap_Struct(vClass, 0, SoundBufferRecorder_free, ptr);
	rb_obj_call_init(tData, argc, argv);
	return tData;
}

static VALUE SoundBufferRecorder_initialize(int argc, VALUE *argv, VALUE vSelf) {
	if(argc == 0) {
		// Nothing to initialize
	} else
		rb_raise(rb_eTypeError, "wrong argument type(s)");
	return vSelf;
}

VALUE SoundBufferRecorder_canCapture(VALUE vClass) {
	return SoundBufferRecorder::CanCapture() ? Qtrue : Qfalse;
}

VALUE SoundBufferRecorder_get_buffer(VALUE vSelf) {
	// Get C++ object pointer from vSelf
	SoundBufferRecorder *pSelf;
	Data_Get_Struct(vSelf, SoundBufferRecorder, pSelf);
	DECLARE_PTR_VAR(SoundBuffer, SoundBuffer, (SoundBuffer *)&pSelf->GetBuffer());
	return vSoundBuffer;
}

VALUE SoundBufferRecorder_start(int argc, VALUE *argv, VALUE vSelf) {
	// Get C++ object pointer from vSelf
	SoundBufferRecorder *pSelf;
	Data_Get_Struct(vSelf, SoundBufferRecorder, pSelf);
	if(argc == 0) {
		pSelf->Start();
	} else if(argc == 1) {
		pSelf->Start(NUM2INT(argv[0]));
	} else
		rb_raise(rb_eTypeError, "wrong argument type(s)");
	return Qnil;
}

VALUE SoundBufferRecorder_stop(VALUE vSelf) {
	// Get C++ object pointer from vSelf
	SoundBufferRecorder *pSelf;
	Data_Get_Struct(vSelf, SoundBufferRecorder, pSelf);
	pSelf->Stop();
	return Qnil;
}

VALUE SoundBufferRecorder_get_sampleRate(VALUE vSelf) {
	// Get C++ object pointer from vSelf
	SoundBufferRecorder *pSelf;
	Data_Get_Struct(vSelf, SoundBufferRecorder, pSelf);
	return INT2FIX(pSelf->GetSampleRate());
}

void Init_SoundBufferRecorder()
{
	g_cSoundBufferRecorder = rb_define_class_under(g_vModule, "SoundBufferRecorder", rb_cObject);
	DEFINE_CLASS_METHOD(SoundBufferRecorder, new, -1);
	DEFINE_INSTANCE_METHOD(SoundBufferRecorder, initialize, -1);

	DEFINE_CLASS_METHOD(SoundBufferRecorder, canCapture, 0);

	DEFINE_GETTER(SoundBufferRecorder, sampleRate);
	DEFINE_GETTER(SoundBufferRecorder, buffer);

	DEFINE_INSTANCE_METHOD(SoundBufferRecorder, start, -1);
	DEFINE_INSTANCE_METHOD(SoundBufferRecorder, stop, 0);
}

