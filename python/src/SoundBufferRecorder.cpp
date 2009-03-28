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

#include "SoundBufferRecorder.hpp"
#include "SoundBuffer.hpp"

#include "compat.hpp"


extern PyTypeObject PySfSoundRecorderType;


static void
PySfSoundBufferRecorder_dealloc(PySfSoundBufferRecorder *self)
{
	delete self->obj;
	free_object(self);
}

static PyObject *
PySfSoundBufferRecorder_new(PyTypeObject *type, PyObject *args, PyObject *kwds)
{
	PySfSoundBufferRecorder *self;
	self = (PySfSoundBufferRecorder *)type->tp_alloc(type, 0);
	if (self != NULL)
		self->obj = new sf::SoundBufferRecorder();
	return (PyObject *)self;
}

static PyObject *
PySfSoundBufferRecorder_GetBuffer(PySfSoundBufferRecorder* self)
{
	PySfSoundBuffer *SoundBuffer = GetNewPySfSoundBuffer();
	SoundBuffer->obj = new sf::SoundBuffer(self->obj->GetBuffer());
	return (PyObject *)SoundBuffer;
}


static PyMethodDef PySfSoundBufferRecorder_methods[] = {
	{"GetBuffer", (PyCFunction)PySfSoundBufferRecorder_GetBuffer, METH_NOARGS, "GetBuffer()\nGet the sound buffer containing the captured audio data. Returns a SoundBuffer object. Returns a sf.SoundBuffer instance."},
	{NULL}  /* Sentinel */
};

PyTypeObject PySfSoundBufferRecorderType = {
	head_init
	"SoundBufferRecorder",	/*tp_name*/
	sizeof(PySfSoundBufferRecorder), /*tp_basicsize*/
	0,						/*tp_itemsize*/
	(destructor)PySfSoundBufferRecorder_dealloc, /*tp_dealloc*/
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
	"Specialized SoundRecorder which saves the captured audio data into a sound buffer.", /* tp_doc */
	0,						/* tp_traverse */
	0,						/* tp_clear */
	0,						/* tp_richcompare */
	0,						/* tp_weaklistoffset */
	0,						/* tp_iter */
	0,						/* tp_iternext */
	PySfSoundBufferRecorder_methods, /* tp_methods */
	0,						/* tp_members */
	0,						/* tp_getset */
	&PySfSoundRecorderType, /* tp_base */
	0,						/* tp_dict */
	0,						/* tp_descr_get */
	0,						/* tp_descr_set */
	0,						/* tp_dictoffset */
	0,						/* tp_init */
	0,						/* tp_alloc */
	PySfSoundBufferRecorder_new, /* tp_new */
};


