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

VALUE g_cSound;

void Sound_free(void *p) { delete (Sound *)p; }
VALUE Sound_new(int argc, VALUE *argv, VALUE vClass) {
	Sound *ptr = new Sound();
	VALUE tData = Data_Wrap_Struct(vClass, 0, Sound_free, ptr);
	rb_obj_call_init(tData, argc, argv);
	return tData;
}

static VALUE Sound_initialize(int argc, VALUE *argv, VALUE vSelf) {
	// Get C++ object pointer from vSelf
	Sound *pSelf;
	Data_Get_Struct(vSelf, Sound, pSelf);
	if(argc == 1 && IS(argv[0], g_cSound)) {
		*pSelf = *(Sound *)DATA_PTR(argv[0]);
	} else if(argc >= 0 && argc <= 7 &&
			(argc < 1 || IS(argv[0], g_cSoundBuffer))) {
		if(argc >= 1)
			pSelf->SetBuffer(*(SoundBuffer *)DATA_PTR(argv[0]));
		if(argc >= 2)
			pSelf->SetLoop(RTEST(argv[1]) != 0);
		if(argc >= 3)
			pSelf->SetPitch((float)NUM2DBL(argv[2]));
		if(argc >= 4)
			pSelf->SetVolume((float)NUM2DBL(argv[3]));
		if(argc >= 5) {
			float x = argc < 5 ? 0.0f : (float)NUM2DBL(argv[4]);
			float y = argc < 6 ? 0.0f : (float)NUM2DBL(argv[5]);
			float z = argc < 7 ? 0.0f : (float)NUM2DBL(argv[6]);
			pSelf->SetPosition(x, y, z);
		}
	} else
		rb_raise(rb_eTypeError, "wrong argument type(s)");
	return vSelf;
}

static VALUE Sound_to_s(VALUE vSelf) {
	// Get C++ object pointer from vSelf
	Sound *pSelf;
	Data_Get_Struct(vSelf, Sound, pSelf);
	char szBuffer[256];
	float x, y, z;
	pSelf->GetPosition(x, y, z);
	sprintf(szBuffer, "Loop=%s, Pitch=%2.2f, Volume=%2.2f, Position=(%2.2f,%2.2f,%2.2f), Status=%s", pSelf->GetLoop() ? "true":"false", pSelf->GetPitch(), pSelf->GetVolume(), x, y, z, pSelf->GetStatus() == Sound::Playing ? "playing" : pSelf->GetStatus() == Sound::Paused ? "paused" : "stopped");
	return rb_str_new2(szBuffer);
}

static VALUE Sound_play(VALUE vSelf) {
	// Get C++ object pointer from vSelf
	Sound *pSelf;
	Data_Get_Struct(vSelf, Sound, pSelf);
	pSelf->Play();
	return Qnil;
}

static VALUE Sound_pause(VALUE vSelf) {
	// Get C++ object pointer from vSelf
	Sound *pSelf;
	Data_Get_Struct(vSelf, Sound, pSelf);
	pSelf->Pause();
	return Qnil;
}

static VALUE Sound_stop(VALUE vSelf) {
	// Get C++ object pointer from vSelf
	Sound *pSelf;
	Data_Get_Struct(vSelf, Sound, pSelf);
	pSelf->Stop();
	return Qnil;
}

static VALUE Sound_set_buffer(VALUE vSelf, VALUE vBuffer) {
	// Get C++ object pointer from vSelf
	Sound *pSelf;
	Data_Get_Struct(vSelf, Sound, pSelf);
	if(!IS(vBuffer, g_cSoundBuffer))
		rb_raise(rb_eTypeError, "wrong argument type(s)");
	pSelf->SetBuffer(*(SoundBuffer *)DATA_PTR(vBuffer));
	return Qnil;
}

static VALUE Sound_set_loop(VALUE vSelf, VALUE vLoop) {
	// Get C++ object pointer from vSelf
	Sound *pSelf;
	Data_Get_Struct(vSelf, Sound, pSelf);
	pSelf->SetLoop(RTEST(vLoop));
	return Qnil;
}

static VALUE Sound_set_pitch(VALUE vSelf, VALUE vPitch) {
	// Get C++ object pointer from vSelf
	Sound *pSelf;
	Data_Get_Struct(vSelf, Sound, pSelf);
	pSelf->SetLoop((float)NUM2DBL(vPitch));
	return Qnil;
}

static VALUE Sound_set_volume(VALUE vSelf, VALUE vVolume) {
	// Get C++ object pointer from vSelf
	Sound *pSelf;
	Data_Get_Struct(vSelf, Sound, pSelf);
	pSelf->SetVolume((float)NUM2DBL(vVolume));
	return Qnil;
}

static VALUE Sound_set_position(VALUE vSelf, VALUE vX, VALUE vY, VALUE vZ) {
	// Get C++ object pointer from vSelf
	Sound *pSelf;
	Data_Get_Struct(vSelf, Sound, pSelf);
	pSelf->SetPosition((float)NUM2DBL(vX), (float)NUM2DBL(vY), (float)NUM2DBL(vZ));
	return Qnil;
}

static VALUE Sound_get_buffer(VALUE vSelf) {
	// Get C++ object pointer from vSelf
	Sound *pSelf;
	Data_Get_Struct(vSelf, Sound, pSelf);
	DECLARE_PTR_VAR(SoundBuffer, SoundBuffer, (SoundBuffer *)pSelf->GetBuffer());
	return vSoundBuffer;
}

static VALUE Sound_get_loop(VALUE vSelf) {
	// Get C++ object pointer from vSelf
	Sound *pSelf;
	Data_Get_Struct(vSelf, Sound, pSelf);
	return pSelf->GetLoop() ? Qtrue : Qfalse;
}

static VALUE Sound_get_pitch(VALUE vSelf) {
	// Get C++ object pointer from vSelf
	Sound *pSelf;
	Data_Get_Struct(vSelf, Sound, pSelf);
	return rb_float_new(pSelf->GetPitch());
}

static VALUE Sound_get_volume(VALUE vSelf) {
	// Get C++ object pointer from vSelf
	Sound *pSelf;
	Data_Get_Struct(vSelf, Sound, pSelf);
	return rb_float_new(pSelf->GetVolume());
}

static VALUE Sound_get_position(VALUE vSelf) {
	// Get C++ object pointer from vSelf
	Sound *pSelf;
	Data_Get_Struct(vSelf, Sound, pSelf);
	float x, y, z;
	pSelf->GetPosition(x, y, z);
	VALUE vArray = rb_ary_new();
	rb_ary_push(vArray, rb_float_new(x));
	rb_ary_push(vArray, rb_float_new(y));
	rb_ary_push(vArray, rb_float_new(z));
	return vArray;
}

static VALUE Sound_get_status(VALUE vSelf) {
	// Get C++ object pointer from vSelf
	Sound *pSelf;
	Data_Get_Struct(vSelf, Sound, pSelf);
	return INT2FIX(pSelf->GetStatus());
}

static VALUE Sound_get_playingOffset(VALUE vSelf) {
	// Get C++ object pointer from vSelf
	Sound *pSelf;
	Data_Get_Struct(vSelf, Sound, pSelf);
	return rb_float_new(pSelf->GetPlayingOffset());
}

void Init_Sound()
{
	g_cSound = rb_define_class_under(g_vModule, "Sound", rb_cObject);
	DEFINE_CLASS_METHOD(Sound, new, -1);

	DEFINE_INSTANCE_METHOD(Sound, initialize, -1);

	DEFINE_RW(Sound, buffer);
	DEFINE_RW(Sound, loop);
	DEFINE_RW(Sound, pitch);
	DEFINE_RW(Sound, volume);
	DEFINE_RW(Sound, position);
	DEFINE_GETTER(Sound, status);
	DEFINE_GETTER(Sound, playingOffset);

	DEFINE_INSTANCE_METHOD(Sound, to_s, 0);
	DEFINE_INSTANCE_METHOD(Sound, play, 0);
	DEFINE_INSTANCE_METHOD(Sound, pause, 0);
	DEFINE_INSTANCE_METHOD(Sound, stop, 0);
}

