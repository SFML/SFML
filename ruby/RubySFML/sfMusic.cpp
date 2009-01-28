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

VALUE g_cMusic;

void Music_free(void *p) { delete (Music *)p; }

VALUE Music_new(int argc, VALUE *argv, VALUE vClass) {
	Music *ptr = NULL;
	if(argc == 1 && ISNUM(argv[0]))
		ptr = new Music(NUM2INT(argv[0]));
	else
		ptr = new Music();
	VALUE tData = Data_Wrap_Struct(vClass, 0, Music_free, ptr);
	rb_obj_call_init(tData, argc, argv);
	return tData;
}

static VALUE Music_initialize(int argc, VALUE *argv, VALUE vSelf) {
	// Get C++ object pointer from vSelf
	Music *pSelf;
	Data_Get_Struct(vSelf, Music, pSelf);
	if(argc == 0) {
		// Nothing to initialize
	} else if(argc == 1 && ISNUM(argv[0])) {
		// Was forced to set this above in Music_new()
	} else if(argc == 1 && ISSTR(argv[0])) {
		if(!pSelf->Open(STR2CSTR(argv[0])))
			rb_raise(rb_eRuntimeError, "Failed to load specified music file");
	} else
		rb_raise(rb_eTypeError, "wrong argument type(s)");

	return vSelf;
}

static VALUE Music_to_s(VALUE vSelf) {
	// Get C++ object pointer from vSelf
	Music *pSelf;
	Data_Get_Struct(vSelf, Music, pSelf);
	char szBuffer[256];
	sprintf(szBuffer, "Channels=%d, SampleRate=%d, Duration=%f, Status=%s", pSelf->GetChannelsCount(), pSelf->GetSampleRate(), pSelf->GetDuration(), pSelf->GetStatus() == Sound::Playing ? "playing" : pSelf->GetStatus() == Sound::Paused ? "paused" : "stopped");
	return rb_str_new2(szBuffer);
}

static VALUE Music_open(VALUE vSelf, VALUE vFile) {
	// Get C++ object pointer from vSelf
	Music *pSelf;
	Data_Get_Struct(vSelf, Music, pSelf);
	return pSelf->Open(STR2CSTR(vFile)) ? Qtrue : Qfalse;
}

static VALUE Music_play(VALUE vSelf) {
	// Get C++ object pointer from vSelf
	Music *pSelf;
	Data_Get_Struct(vSelf, Music, pSelf);
	pSelf->Play();
	return Qnil;
}

static VALUE Music_stop(VALUE vSelf) {
	// Get C++ object pointer from vSelf
	Music *pSelf;
	Data_Get_Struct(vSelf, Music, pSelf);
	pSelf->Stop();
	return Qnil;
}

static VALUE Music_set_loop(VALUE vSelf, VALUE vLoop) {
	// Get C++ object pointer from vSelf
	Music *pSelf;
	Data_Get_Struct(vSelf, Music, pSelf);
	pSelf->SetLoop(RTEST(vLoop));
	return Qnil;
}

static VALUE Music_get_loop(VALUE vSelf) {
	// Get C++ object pointer from vSelf
	Music *pSelf;
	Data_Get_Struct(vSelf, Music, pSelf);
	return pSelf->GetLoop() ? Qtrue : Qfalse;
}

static VALUE Music_get_duration(VALUE vSelf) {
	// Get C++ object pointer from vSelf
	Music *pSelf;
	Data_Get_Struct(vSelf, Music, pSelf);
	return rb_float_new(pSelf->GetDuration());
}

static VALUE Music_get_channels(VALUE vSelf) {
	// Get C++ object pointer from vSelf
	Music *pSelf;
	Data_Get_Struct(vSelf, Music, pSelf);
	return INT2FIX(pSelf->GetChannelsCount());
}

static VALUE Music_get_sampleRate(VALUE vSelf) {
	// Get C++ object pointer from vSelf
	Music *pSelf;
	Data_Get_Struct(vSelf, Music, pSelf);
	return INT2FIX(pSelf->GetSampleRate());
}

static VALUE Music_get_status(VALUE vSelf) {
	// Get C++ object pointer from vSelf
	Music *pSelf;
	Data_Get_Struct(vSelf, Music, pSelf);
	return INT2FIX(pSelf->GetStatus());
}

void Init_Music()
{
	g_cMusic = rb_define_class_under(g_vModule, "Music", rb_cObject);
	DEFINE_CLASS_METHOD(Music, new, -1);
	DEFINE_INSTANCE_METHOD(Music, initialize, -1);

	DEFINE_RW(Music, loop);
	DEFINE_GETTER(Music, duration);
	DEFINE_GETTER(Music, status);
	DEFINE_GETTER(Music, channels);
	DEFINE_GETTER(Music, sampleRate);

	DEFINE_INSTANCE_METHOD(Music, to_s, 0);
	DEFINE_INSTANCE_METHOD(Music, open, 1);
	DEFINE_INSTANCE_METHOD(Music, play, 0);
	DEFINE_INSTANCE_METHOD(Music, stop, 0);
}

