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

#include "VideoMode.hpp"

#include <structmember.h>

#include "offsetof.hpp"
#include "compat.hpp"


static PyMemberDef PySfVideoMode_members[] = {
    {(char *)"Width", T_UINT, offsetof(PySfVideoMode, Width), 0, (char *)"Video mode width, in pixels."},
    {(char *)"Height", T_UINT, offsetof(PySfVideoMode, Height), 0, (char *)"Video mode height, in pixels."},
    {(char *)"BitsPerPixel", T_UINT, offsetof(PySfVideoMode, BitsPerPixel), 0, (char *)"Video mode pixel depth, in bits per pixels."},
    {NULL}  /* Sentinel */
};


static void
PySfVideoMode_dealloc(PySfVideoMode* self)
{
	delete self->obj;
	free_object(self);
}

static PyObject *
PySfVideoMode_new(PyTypeObject *type, PyObject *args, PyObject *kwds)
{
	const char *kwlist[] = {"Width", "Height", "BitsPerPixel", NULL};
	PySfVideoMode *self;
	self = (PySfVideoMode *)type->tp_alloc(type, 0);
	if (self != NULL)
	{
		self->BitsPerPixel = 32;
		if (!PyArg_ParseTupleAndKeywords(args, kwds, "II|I:VideoMode.__new__", (char **)kwlist, &self->Width, &self->Height, &self->BitsPerPixel))
			return NULL;
		self->obj = new sf::VideoMode(self->Width, self->Height, self->BitsPerPixel);
	}
	return (PyObject *)self;
}

static PyObject *
PySfVideoMode_IsValid(PySfVideoMode* self)
{
	return PyBool_FromLong(self->obj->IsValid());
}

static PyObject *
PySfVideoMode_GetDesktopMode(PySfVideoMode* self)
{
	PySfVideoMode *VideoMode;

	VideoMode = GetNewPySfVideoMode();
	VideoMode->obj = new sf::VideoMode(sf::VideoMode::GetDesktopMode());
	VideoMode->Width = VideoMode->obj->Width;
	VideoMode->Height = VideoMode->obj->Height;
	VideoMode->BitsPerPixel = VideoMode->obj->BitsPerPixel;

	return (PyObject *)VideoMode;
}

static PyObject *
PySfVideoMode_GetMode(PySfVideoMode* self, PyObject *args)
{
	std::size_t index;
	PySfVideoMode *VideoMode;

	index = (std::size_t)PyLong_AsLong(args);

	VideoMode = GetNewPySfVideoMode();
	VideoMode->obj = new sf::VideoMode(sf::VideoMode::GetMode(index));
	VideoMode->Width = VideoMode->obj->Width;
	VideoMode->Height = VideoMode->obj->Height;
	VideoMode->BitsPerPixel = VideoMode->obj->BitsPerPixel;

	return (PyObject *)VideoMode;
}

static PyObject *
PySfVideoMode_GetModesCount(PySfVideoMode* self)
{
	return PyLong_FromLong(sf::VideoMode::GetModesCount());
}


static PyMethodDef PySfVideoMode_methods[] = {
	{"IsValid", (PyCFunction)PySfVideoMode_IsValid, METH_NOARGS, "IsValid()\nTell whether or not the video mode is supported."},
	{"GetDesktopMode", (PyCFunction)PySfVideoMode_GetDesktopMode, METH_STATIC | METH_NOARGS, "GetDesktopMode()\nGet the current desktop video mode."},
	{"GetMode", (PyCFunction)PySfVideoMode_GetMode, METH_STATIC | METH_O, "GetMode(Index)\nGet a valid video mode. Index must be in range [0, GetModesCount()[ Modes are sorted from best to worst.\n	Index : Index of video mode to get"},
	{"GetModesCount", (PyCFunction)PySfVideoMode_GetModesCount, METH_STATIC | METH_NOARGS, "GetModesCount()\nGet valid video modes count."},
	{NULL}  /* Sentinel */
};

PyObject *
PySfVideoMode_richcompare(PyObject *o1, PyObject *o2, int op)
{
	if (*(((PySfVideoMode *)o1)->obj) == *(((PySfVideoMode *)o2)->obj))
	{
		if (op==Py_EQ)
			Py_RETURN_TRUE;
		if (op==Py_NE)
			Py_RETURN_FALSE;
	}
	else
	{
		if (op==Py_EQ)
			Py_RETURN_FALSE;
		if (op==Py_NE)
			Py_RETURN_TRUE;
	}
	PyErr_SetString(PyExc_TypeError, "VideoMode comparison : only == and != make sens.");
	return NULL;
}

int
PySfVideoMode_setattro(PyObject* self, PyObject *attr_name, PyObject *v)
{
	int result = PyObject_GenericSetAttr(self, attr_name, v);
	PySfVideoMode *Mode = (PySfVideoMode *)self;
	Mode->obj->Width = Mode->Width;
	Mode->obj->Height = Mode->Height;
	Mode->obj->BitsPerPixel = Mode->BitsPerPixel;
	return result;
}

PyTypeObject PySfVideoModeType = {
	head_init
	"VideoMode",			/*tp_name*/
	sizeof(PySfVideoMode),	/*tp_basicsize*/
	0,						/*tp_itemsize*/
	(destructor)PySfVideoMode_dealloc, /*tp_dealloc*/
	0,						/*tp_print*/
	0,						/*tp_getattr*/
	0,						/*tp_setattr*/
	0,						/*tp_compare*/
	0,						/*tp_repr*/
	0,						/*tp_as_number*/
	0,						/*tp_as_sequence*/
	0,						/*tp_as_mapping*/
	0,						/*tp_hash */
	0,						/*tp_call*/
	0,						/*tp_str*/
	0,						/*tp_getattro*/
	0,						/*tp_setattro*/
	0,						/*tp_as_buffer*/
	Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE, /*tp_flags*/
	"sf.VideoMode defines a video mode (width, height, bpp, frequency) and provides functions for getting modes supported by the display device\n\
Default constructor : VideoMode()\n\
Construct the video mode with its attributes : VideoMode(ModeWidth, ModeHeight, ModeBpp = 32)\n	ModeWidth  : Width in pixels\n	ModeHeight : Height in pixels\n	ModeBpp    : Pixel depths in bits per pixel (32 by default)", /* tp_doc */
	0,						/* tp_traverse */
	0,						/* tp_clear */
	PySfVideoMode_richcompare, /* tp_richcompare */
	0,						/* tp_weaklistoffset */
	0,						/* tp_iter */
	0,						/* tp_iternext */
	PySfVideoMode_methods,	/* tp_methods */
	PySfVideoMode_members,	/* tp_members */
	0,						/* tp_getset */
	0,						/* tp_base */
	0,						/* tp_dict */
	0,						/* tp_descr_get */
	0,						/* tp_descr_set */
	0,						/* tp_dictoffset */
	0,						/* tp_init */
	0,						/* tp_alloc */
	PySfVideoMode_new,		/* tp_new */
};


PySfVideoMode *
GetNewPySfVideoMode()
{
	return PyObject_New(PySfVideoMode, &PySfVideoModeType);
}

