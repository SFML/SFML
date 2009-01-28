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

VALUE g_cSoundBuffer;

void SoundBuffer_free(void *p) { delete (SoundBuffer *)p; }
VALUE SoundBuffer_new(int argc, VALUE *argv, VALUE vClass) {
	SoundBuffer *ptr = new SoundBuffer();
	VALUE tData = Data_Wrap_Struct(vClass, 0, SoundBuffer_free, ptr);
	rb_obj_call_init(tData, argc, argv);
	return tData;
}

static VALUE SoundBuffer_initialize(int argc, VALUE *argv, VALUE vSelf) {
	// Get C++ object pointer from vSelf
	SoundBuffer *pSelf;
	Data_Get_Struct(vSelf, SoundBuffer, pSelf);
	if(argc == 0) {
		// Nothing to initialize
	} else if(argc == 1 && IS(argv[0], g_cSoundBuffer)) {
		*pSelf = *(SoundBuffer *)DATA_PTR(argv[0]);
	} else if(argc == 1 && ISSTR(argv[0])) {
		if(!pSelf->LoadFromFile(STR2CSTR(argv[0])))
			rb_raise(rb_eRuntimeError, "Failed to load specified sound file");
	} else
		rb_raise(rb_eTypeError, "wrong argument type(s)");
	return vSelf;
}

static VALUE SoundBuffer_to_s(VALUE vSelf) {
	// Get C++ object pointer from vSelf
	SoundBuffer *pSelf;
	Data_Get_Struct(vSelf, SoundBuffer, pSelf);
	char szBuffer[256];
	sprintf(szBuffer, "Samples=%d, SampleRate=%d, Channels=%d, Duration=%f", pSelf->GetSamplesCount(), pSelf->GetSampleRate(), pSelf->GetChannelsCount(), pSelf->GetDuration());
	return rb_str_new2(szBuffer);
}

static VALUE SoundBuffer_loadFromFile(VALUE vSelf, VALUE vPath) {
	// Get C++ object pointer from vSelf
	SoundBuffer *pSelf;
	Data_Get_Struct(vSelf, SoundBuffer, pSelf);
	return pSelf->LoadFromFile(STR2CSTR(vPath)) ? Qtrue : Qfalse;
}

static VALUE SoundBuffer_saveToFile(VALUE vSelf, VALUE vPath) {
	// Get C++ object pointer from vSelf
	SoundBuffer *pSelf;
	Data_Get_Struct(vSelf, SoundBuffer, pSelf);
	return pSelf->SaveToFile(STR2CSTR(vPath)) ? Qtrue : Qfalse;
}

static VALUE SoundBuffer_loadFromMemory(VALUE vSelf, VALUE vSamples, VALUE vSize) {
	// Get C++ object pointer from vSelf
	SoundBuffer *pSelf;
	Data_Get_Struct(vSelf, SoundBuffer, pSelf);
	return pSelf->LoadFromMemory(STR2CSTR(vSamples), NUM2INT(vSize)) ? Qtrue : Qfalse;
}

static VALUE SoundBuffer_get_samples(VALUE vSelf) {
	// Get C++ object pointer from vSelf
	SoundBuffer *pSelf;
	Data_Get_Struct(vSelf, SoundBuffer, pSelf);
	return rb_str_new((const char *)pSelf->GetSamples(), pSelf->GetSamplesCount()*2);
}

static VALUE SoundBuffer_get_samplesCount(VALUE vSelf) {
	// Get C++ object pointer from vSelf
	SoundBuffer *pSelf;
	Data_Get_Struct(vSelf, SoundBuffer, pSelf);
	return INT2FIX(pSelf->GetSamplesCount());
}

static VALUE SoundBuffer_get_sampleRate(VALUE vSelf) {
	// Get C++ object pointer from vSelf
	SoundBuffer *pSelf;
	Data_Get_Struct(vSelf, SoundBuffer, pSelf);
	return INT2FIX(pSelf->GetSampleRate());
}

static VALUE SoundBuffer_get_channels(VALUE vSelf) {
	// Get C++ object pointer from vSelf
	SoundBuffer *pSelf;
	Data_Get_Struct(vSelf, SoundBuffer, pSelf);
	return INT2FIX(pSelf->GetChannelsCount());
}

static VALUE SoundBuffer_get_duration(VALUE vSelf) {
	// Get C++ object pointer from vSelf
	SoundBuffer *pSelf;
	Data_Get_Struct(vSelf, SoundBuffer, pSelf);
	return rb_float_new(pSelf->GetDuration());
}

void Init_SoundBuffer()
{
	g_cSoundBuffer = rb_define_class_under(g_vModule, "SoundBuffer", rb_cObject);
	DEFINE_CLASS_METHOD(SoundBuffer, new, -1);
	DEFINE_INSTANCE_METHOD(SoundBuffer, initialize, -1);

	DEFINE_GETTER(SoundBuffer, samples);
	DEFINE_GETTER(SoundBuffer, samplesCount);
	DEFINE_GETTER(SoundBuffer, sampleRate);
	DEFINE_GETTER(SoundBuffer, channels);
	DEFINE_GETTER(SoundBuffer, duration);

	DEFINE_INSTANCE_METHOD(SoundBuffer, to_s, 0);
	DEFINE_INSTANCE_METHOD(SoundBuffer, loadFromFile, 1);
	DEFINE_INSTANCE_METHOD(SoundBuffer, saveToFile, 1);
	DEFINE_INSTANCE_METHOD(SoundBuffer, loadFromMemory, 2);
}

