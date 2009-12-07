////////////////////////////////////////////////////////////
//
// PySFML - Python binding for SFML (Simple and Fast Multimedia Library)
// Copyright (C) 2007, 2008 Rémi Koenig (remi.k2620@gmail.com)
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

#include "main.hpp"

#include "Color.hpp"
#include "Key.hpp"
#include "Joy.hpp"
#include "Event.hpp"
#include "Mouse.hpp"
#include "WindowStyle.hpp"
#include "ContextSettings.hpp"
#include "Blend.hpp"
#include "Sound.hpp"
#include "Text.hpp"
#include "SoundStream.hpp"

#include "compat.hpp"

extern PyTypeObject PySfClockType;

extern PyTypeObject PySfEventType;
extern PyTypeObject PySfEventTextType;
extern PyTypeObject PySfEventKeyType;
extern PyTypeObject PySfEventMouseMoveType;
extern PyTypeObject PySfEventMouseButtonType;
extern PyTypeObject PySfEventMouseWheelType;
extern PyTypeObject PySfEventJoyMoveType;
extern PyTypeObject PySfEventJoyButtonType;
extern PyTypeObject PySfEventSizeType;
extern PyTypeObject PySfKeyType;
extern PyTypeObject PySfJoyType;
extern PyTypeObject PySfMouseType;

extern PyTypeObject PySfVideoModeType;
extern PyTypeObject PySfWindowType;
extern PyTypeObject PySfStyleType;
extern PyTypeObject PySfContextSettingsType;
extern PyTypeObject PySfRenderWindowType;
extern PyTypeObject PySfViewType;
extern PyTypeObject PySfInputType;

extern PyTypeObject PySfDrawableType;
extern PyTypeObject PySfBlendType;
extern PyTypeObject PySfSpriteType;
extern PyTypeObject PySfFontType;
extern PyTypeObject PySfGlyphType;
extern PyTypeObject PySfTextType;
//extern PyTypeObject PySfPostFXType;

extern PyTypeObject PySfImageType;
extern PyTypeObject PySfColorType;

extern PyTypeObject PySfShapeType;

extern PyTypeObject PySfIntRectType;
extern PyTypeObject PySfFloatRectType;

extern PyTypeObject PySfMusicType;
extern PyTypeObject PySfSoundType;
extern PyTypeObject PySfSoundBufferType;
extern PyTypeObject PySfSoundRecorderType;
extern PyTypeObject PySfSoundBufferRecorderType;
extern PyTypeObject PySfSoundStreamType;
extern PyTypeObject PySfListenerType;


static PyMethodDef module_methods[] = {
	{"Sleep", (PyCFunction)PySFML_Sleep, METH_O, "Sleep(Duration)\nMake the current thread sleep for a given time.\n	Duration : Time to sleep, in seconds"},
	{NULL}  /* Sentinel */
};

#ifdef IS_PY3K
#define INITERROR return NULL
static PyModuleDef module_def = {
    PyModuleDef_HEAD_INIT,
    "sf",
    "Python binding for sfml (Simple Fast Media Library)",
    -1,
    module_methods, NULL, NULL, NULL, NULL
};

PyMODINIT_FUNC
PyInit_sf(void)
#else
#define INITERROR return

#ifndef PyMODINIT_FUNC	/* declarations for DLL import/export */
#define PyMODINIT_FUNC void
#endif
PyMODINIT_FUNC
initsf(void)
#endif
{
    PyObject *m;

	if (PyType_Ready(&PySfClockType) < 0)
		INITERROR;

	if (PyType_Ready(&PySfWindowType) < 0)
		INITERROR;
	if (PyType_Ready(&PySfStyleType) < 0)
		INITERROR;
	if (PyType_Ready(&PySfContextSettingsType) < 0)
		INITERROR;
	if (PyType_Ready(&PySfRenderWindowType) < 0)
		INITERROR;
	if (PyType_Ready(&PySfVideoModeType) < 0)
		INITERROR;
	if (PyType_Ready(&PySfViewType) < 0)
		INITERROR;
	if (PyType_Ready(&PySfInputType) < 0)
		INITERROR;

	if (PyType_Ready(&PySfEventType) < 0)
		INITERROR;
	if (PyType_Ready(&PySfEventTextType) < 0)
		INITERROR;
	if (PyType_Ready(&PySfEventKeyType) < 0)
		INITERROR;
	if (PyType_Ready(&PySfEventMouseMoveType) < 0)
		INITERROR;
	if (PyType_Ready(&PySfEventMouseButtonType) < 0)
		INITERROR;
	if (PyType_Ready(&PySfEventMouseWheelType) < 0)
		INITERROR;
	if (PyType_Ready(&PySfEventJoyMoveType) < 0)
		INITERROR;
	if (PyType_Ready(&PySfEventJoyButtonType) < 0)
		INITERROR;
	if (PyType_Ready(&PySfEventSizeType) < 0)
		INITERROR;
	if (PyType_Ready(&PySfKeyType) < 0)
		INITERROR;
	if (PyType_Ready(&PySfJoyType) < 0)
		INITERROR;
	if (PyType_Ready(&PySfMouseType) < 0)
		INITERROR;

	if (PyType_Ready(&PySfDrawableType) < 0)
		INITERROR;
	if (PyType_Ready(&PySfBlendType) < 0)
		INITERROR;
	if (PyType_Ready(&PySfSpriteType) < 0)
		INITERROR;
	if (PyType_Ready(&PySfFontType) < 0)
		INITERROR;
	if (PyType_Ready(&PySfGlyphType) < 0)
		INITERROR;
	if (PyType_Ready(&PySfTextType) < 0)
		INITERROR;
	/*if (PyType_Ready(&PySfPostFXType) < 0)
		INITERROR;*/

	if (PyType_Ready(&PySfImageType) < 0)
		INITERROR;

	if (PyType_Ready(&PySfShapeType) < 0)
		INITERROR;

	if (PyType_Ready(&PySfColorType) < 0)
		INITERROR;

	if (PyType_Ready(&PySfIntRectType) < 0)
		INITERROR;
	if (PyType_Ready(&PySfFloatRectType) < 0)
		INITERROR;

	if (PyType_Ready(&PySfMusicType) < 0)
		INITERROR;
	if (PyType_Ready(&PySfSoundType) < 0)
		INITERROR;
	if (PyType_Ready(&PySfSoundBufferType) < 0)
		INITERROR;
	if (PyType_Ready(&PySfSoundBufferRecorderType) < 0)
		INITERROR;
	if (PyType_Ready(&PySfSoundRecorderType) < 0)
		INITERROR;
	if (PyType_Ready(&PySfSoundStreamType) < 0)
		INITERROR;
	if (PyType_Ready(&PySfListenerType) < 0)
		INITERROR;

#ifdef IS_PY3K
	m = PyModule_Create(&module_def);
#else
	m = Py_InitModule3("sf", module_methods, "Python binding for sfml (Simple Fast Media Library)");
#endif

	if (m == NULL)
		INITERROR;

	Py_INCREF(&PySfClockType);
	PyModule_AddObject(m, "Clock", (PyObject *)&PySfClockType);

	Py_INCREF(&PySfWindowType);
	PyModule_AddObject(m, "Window", (PyObject *)&PySfWindowType);
	Py_INCREF(&PySfStyleType);
	PyModule_AddObject(m, "Style", (PyObject *)&PySfStyleType);
	Py_INCREF(&PySfContextSettingsType);
	PyModule_AddObject(m, "ContextSettings", (PyObject *)&PySfContextSettingsType);
	Py_INCREF(&PySfRenderWindowType);
	PyModule_AddObject(m, "RenderWindow", (PyObject *)&PySfRenderWindowType);
	Py_INCREF(&PySfVideoModeType);
	PyModule_AddObject(m, "VideoMode", (PyObject *)&PySfVideoModeType);
	Py_INCREF(&PySfViewType);
	PyModule_AddObject(m, "View", (PyObject *)&PySfViewType);
	Py_INCREF(&PySfInputType);
	PyModule_AddObject(m, "Input", (PyObject *)&PySfInputType);

	Py_INCREF(&PySfDrawableType);
	PyModule_AddObject(m, "Drawable", (PyObject *)&PySfDrawableType);
	Py_INCREF(&PySfBlendType);
	PyModule_AddObject(m, "Blend", (PyObject *)&PySfBlendType);
	Py_INCREF(&PySfSpriteType);
	PyModule_AddObject(m, "Sprite", (PyObject *)&PySfSpriteType);
	Py_INCREF(&PySfFontType);
	PyModule_AddObject(m, "Font", (PyObject *)&PySfFontType);
	Py_INCREF(&PySfGlyphType);
	PyModule_AddObject(m, "Glyph", (PyObject *)&PySfGlyphType);
	Py_INCREF(&PySfTextType);
	PyModule_AddObject(m, "Text", (PyObject *)&PySfTextType);
	/*Py_INCREF(&PySfPostFXType);
	PyModule_AddObject(m, "PostFX", (PyObject *)&PySfPostFXType);*/

	Py_INCREF(&PySfEventType);
	PyModule_AddObject(m, "Event", (PyObject *)&PySfEventType);
	Py_INCREF(&PySfKeyType);
	PyModule_AddObject(m, "Key", (PyObject *)&PySfKeyType);
	Py_INCREF(&PySfJoyType);
	PyModule_AddObject(m, "Joy", (PyObject *)&PySfJoyType);
	Py_INCREF(&PySfMouseType);
	PyModule_AddObject(m, "Mouse", (PyObject *)&PySfMouseType);

	Py_INCREF(&PySfImageType);
	PyModule_AddObject(m, "Image", (PyObject *)&PySfImageType);

	Py_INCREF(&PySfColorType);
	PyModule_AddObject(m, "Color", (PyObject *)&PySfColorType);

	Py_INCREF(&PySfShapeType);
	PyModule_AddObject(m, "Shape", (PyObject *)&PySfShapeType);

	Py_INCREF(&PySfIntRectType);
	PyModule_AddObject(m, "IntRect", (PyObject *)&PySfIntRectType);
	Py_INCREF(&PySfFloatRectType);
	PyModule_AddObject(m, "FloatRect", (PyObject *)&PySfFloatRectType);

	Py_INCREF(&PySfMusicType);
	PyModule_AddObject(m, "Music", (PyObject *)&PySfMusicType);
	Py_INCREF(&PySfSoundType);
	PyModule_AddObject(m, "Sound", (PyObject *)&PySfSoundType);
	Py_INCREF(&PySfSoundBufferType);
	PyModule_AddObject(m, "SoundBuffer", (PyObject *)&PySfSoundBufferType);
	Py_INCREF(&PySfSoundRecorderType);
	PyModule_AddObject(m, "SoundRecorder", (PyObject *)&PySfSoundRecorderType);
	Py_INCREF(&PySfSoundBufferRecorderType);
	PyModule_AddObject(m, "SoundBufferRecorder", (PyObject *)&PySfSoundBufferRecorderType);
	Py_INCREF(&PySfSoundStreamType);
	PyModule_AddObject(m, "SoundStream", (PyObject *)&PySfSoundStreamType);
	Py_INCREF(&PySfListenerType);
	PyModule_AddObject(m, "Listener", (PyObject *)&PySfListenerType);

	PyModule_AddStringConstant(m, "Version", "1.6");

	PySfColor_InitConst();
	PySfKey_InitConst();
	PySfJoy_InitConst();
	PySfEvent_InitConst();
	PySfMouse_InitConst();
	PySfStyle_InitConst();
	PySfBlend_InitConst();
	PySfSound_InitConst();
	PySfSoundStream_InitConst();
	PySfText_InitConst();

	PyEval_InitThreads();

#ifdef IS_PY3K
	return m;
#endif
}

