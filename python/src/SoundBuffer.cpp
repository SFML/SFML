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

#include "SoundBuffer.hpp"


static PyMemberDef PySfSoundBuffer_members[] = {
	{NULL}  /* Sentinel */
};


static void
PySfSoundBuffer_dealloc(PySfSoundBuffer *self)
{
	delete self->obj;
	self->ob_type->tp_free((PyObject*)self);
}

static PyObject *
PySfSoundBuffer_new(PyTypeObject *type, PyObject *args, PyObject *kwds)
{
	PySfSoundBuffer *self;

	self = (PySfSoundBuffer *)type->tp_alloc(type, 0);
	if (self != NULL)
	{
	}

	return (PyObject *)self;
}

static int
PySfSoundBuffer_init(PySfSoundBuffer *self, PyObject *args, PyObject *kwds);

static PyObject*
PySfSoundBuffer_LoadFromFile(PySfSoundBuffer *self, PyObject *args)
{
	char *path = PyString_AsString(args);
	if (self->obj->LoadFromFile(path))
		Py_RETURN_TRUE;
	else
		Py_RETURN_FALSE;
}

static PyObject *
PySfSoundBuffer_LoadFromMemory(PySfSoundBuffer* self, PyObject *args)
{
	unsigned int SizeInBytes;
	char *Data;

	if (! PyArg_ParseTuple(args, "s#", &Data, &SizeInBytes))
		return NULL; 

	if (self->obj->LoadFromMemory(Data, (std::size_t) SizeInBytes))
		Py_RETURN_TRUE;
	else
		Py_RETURN_FALSE;
}

static PyObject *
PySfSoundBuffer_LoadFromSamples(PySfSoundBuffer* self, PyObject *args)
{
	unsigned int SizeInBytes, ChannelsCount, SampleRate;
	char *Data;

	if (! PyArg_ParseTuple(args, "s#II", &Data, &SizeInBytes, &ChannelsCount, &SampleRate))
		return NULL; 

	if (self->obj->LoadFromSamples((const sf::Int16*)Data, (std::size_t) SizeInBytes/2, ChannelsCount, SampleRate))
		Py_RETURN_TRUE;
	else
		Py_RETURN_FALSE;
}

static PyObject*
PySfSoundBuffer_GetSamples(PySfSoundBuffer *self)
{
	return PyString_FromStringAndSize((const char *)(self->obj->GetSamples()), self->obj->GetSamplesCount()*2);
}

static PyObject*
PySfSoundBuffer_SaveToFile(PySfSoundBuffer *self, PyObject *args)
{
	char *path = PyString_AsString(args);
	if (self->obj->SaveToFile(path))
		Py_RETURN_TRUE;
	else
		Py_RETURN_FALSE;
}

static PyObject*
PySfSoundBuffer_GetDuration(PySfSoundBuffer *self)
{
	return PyFloat_FromDouble((double)(self->obj->GetDuration()));
}

static PyObject*
PySfSoundBuffer_GetChannelsCount(PySfSoundBuffer *self)
{
	return PyLong_FromUnsignedLong(self->obj->GetChannelsCount());
}

static PyObject*
PySfSoundBuffer_GetSampleRate(PySfSoundBuffer *self)
{
	return PyLong_FromUnsignedLong(self->obj->GetSampleRate());
}

static PyObject*
PySfSoundBuffer_GetSamplesCount(PySfSoundBuffer *self)
{
	return PyLong_FromUnsignedLong(self->obj->GetSamplesCount());
}


static PyMethodDef PySfSoundBuffer_methods[] = {
	{"LoadFromFile", (PyCFunction)PySfSoundBuffer_LoadFromFile, METH_O, "LoadFromFile(FileName)\nLoad the sound buffer from a file. Returns True if loading has been successful.\n	Filename : Path of the sound file to load"},
	{"SaveToFile", (PyCFunction)PySfSoundBuffer_SaveToFile, METH_O, "SaveToFile(Filename)\nSave the sound buffer to a file. Returns True if saving has been successful.\n	Filename : Path of the sound file to write"},
	{"LoadFromMemory", (PyCFunction)PySfSoundBuffer_LoadFromMemory, METH_O, "LoadFromMemory(Data)\nLoad the sound buffer from a string in memory.\n	Data : string representing the file data in memory "},
	{"LoadFromSamples", (PyCFunction)PySfSoundBuffer_LoadFromSamples, METH_VARARGS, "LoadFromSamples(Samples, ChannelsCount, SampleRate)\nLoad the sound buffer from an array of samples - assumed format for samples is 16 bits signed integer.\n\
	Samples :       Pointer to the samples in memory\n\
	ChannelsCount : Number of channels (1 = mono, 2 = stereo, ...)\n\
	SampleRate :    Frequency (number of samples to play per second)"},
	{"GetDuration", (PyCFunction)PySfSoundBuffer_GetDuration, METH_NOARGS, "GetDuration()\nGet the sound duration."},
	{"GetChannelsCount", (PyCFunction)PySfSoundBuffer_GetChannelsCount, METH_NOARGS, "GetChannelsCount()\nReturn the number of channels (1 = mono, 2 = stereo)."},
	{"GetSampleRate", (PyCFunction)PySfSoundBuffer_GetSampleRate, METH_NOARGS, "GetSampleRate()\nGet the sound frequency (sample rate)."},
	{"GetSamplesCount", (PyCFunction)PySfSoundBuffer_GetSamplesCount, METH_NOARGS, "GetSamplesCount()\nReturn the samples count."},
	{"GetSamples", (PyCFunction)PySfSoundBuffer_GetSamples, METH_NOARGS, "GetSamples()\nReturn the sound samples as a string."},
	{NULL}  /* Sentinel */
};

PyTypeObject PySfSoundBufferType = {
	PyObject_HEAD_INIT(NULL)
	0,						/*ob_size*/
	"SoundBuffer",			/*tp_name*/
	sizeof(PySfSoundBuffer), /*tp_basicsize*/
	0,						/*tp_itemsize*/
	(destructor)PySfSoundBuffer_dealloc, /*tp_dealloc*/
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
	"sf.SoundBuffer is the low-level for loading and manipulating sound buffers.\n\
Default constructor : SoundBuffer()\n\
Copy constructor : SoundBuffer(Copy) where Copy is a sf.SoundBuffer instance.", /* tp_doc */
	0,						/* tp_traverse */
	0,						/* tp_clear */
	0,						/* tp_richcompare */
	0,						/* tp_weaklistoffset */
	0,						/* tp_iter */
	0,						/* tp_iternext */
	PySfSoundBuffer_methods, /* tp_methods */
	PySfSoundBuffer_members, /* tp_members */
	0,						/* tp_getset */
	0,						/* tp_base */
	0,						/* tp_dict */
	0,						/* tp_descr_get */
	0,						/* tp_descr_set */
	0,						/* tp_dictoffset */
	(initproc)PySfSoundBuffer_init, /* tp_init */
	0,						/* tp_alloc */
	PySfSoundBuffer_new,	/* tp_new */
};

static int
PySfSoundBuffer_init(PySfSoundBuffer *self, PyObject *args, PyObject *kwds)
{
	if (PyTuple_Size(args) == 1)
	{
		PySfSoundBuffer *Copy;
		if (PyArg_ParseTuple(args, "O!", &PySfSoundBufferType, &Copy))
			self->obj = new sf::SoundBuffer(*(Copy->obj));
		else
			return -1;
	}
	else
		self->obj = new sf::SoundBuffer();
	return 0;
}

PySfSoundBuffer *
GetNewPySfSoundBuffer()
{
	return PyObject_New(PySfSoundBuffer, &PySfSoundBufferType);
}

