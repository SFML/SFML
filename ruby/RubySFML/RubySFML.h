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
#include "ruby_helper.h"

extern VALUE NOARGV[1];

// SFML Ruby module object
extern VALUE g_vModule;

// SFML Ruby class objects
extern VALUE g_cClock;
extern VALUE g_cColor;
extern VALUE g_cDrawable;
extern VALUE g_cEvent;
extern VALUE g_cFloatRect;
extern VALUE g_cImage;
extern VALUE g_cInput;
extern VALUE g_cIntRect;
extern VALUE g_cMusic;
extern VALUE g_cPostFX;
extern VALUE g_cRenderWindow;
extern VALUE g_cSound;
extern VALUE g_cSoundBuffer;
extern VALUE g_cSoundBufferRecorder;
extern VALUE g_cSprite;
extern VALUE g_cString;
extern VALUE g_cVideoMode;
extern VALUE g_cView;
extern VALUE g_cWindow;

// SMFL Ruby class init functions
extern void Init_Clock();
extern void Init_Color();
extern void Init_Drawable();
extern void Init_Event();
extern void Init_FloatRect();
extern void Init_Image();
extern void Init_Input();
extern void Init_IntRect();
extern void Init_Music();
extern void Init_PostFX();
extern void Init_RenderWindow();
extern void Init_Sound();
extern void Init_SoundBuffer();
extern void Init_SoundBufferRecorder();
extern void Init_Sprite();
extern void Init_String();
extern void Init_VideoMode();
extern void Init_View();
extern void Init_Window();

extern void Color_free(void *p);
extern VALUE Color_new(int argc, VALUE *argv, VALUE vClass);

extern void Event_free(void *p);
extern VALUE Event_new(int argc, VALUE *argv, VALUE vClass);

extern void FloatRect_free(void *p);
extern VALUE FloatRect_new(int argc, VALUE *argv, VALUE vClass);

extern void Image_free(void *p);
extern VALUE Image_new(int argc, VALUE *argv, VALUE vClass);

extern void IntRect_free(void *p);
extern VALUE IntRect_new(int argc, VALUE *argv, VALUE vClass);

