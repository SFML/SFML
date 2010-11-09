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

#include "SoundRecorder.hpp"

#include "compat.hpp"

bool CustomSoundRecorder::OnStart()
{
	bool result = false;
	if (PyObject_HasAttrString(SoundRecorder, "OnStart"))
	{
		PyObject *OnStart = PyObject_GetAttrString(SoundRecorder, "OnStart");
		PyObject *Result = PyObject_CallFunction(OnStart, NULL);
		result = PyBool_AsBool(Result);
		Py_DECREF(OnStart);
		Py_DECREF(Result);
	}
    return result;
}

bool CustomSoundRecorder::OnProcessSamples(const sf::Int16* Samples, std::size_t SamplesCount)
{
	bool result = false;
	if (PyObject_HasAttrString(SoundRecorder, "OnGetData"))
	{
		PyObject *OnGetData = PyObject_GetAttrString(SoundRecorder, "OnGetData");
		PyObject *Result = PyObject_CallFunction(OnGetData, (char *)"#s", (char *)Samples, SamplesCount*2);
		result = PyBool_AsBool(Result);
		Py_DECREF(OnGetData);
		Py_DECREF(Result);
	}
    return result;
}

void CustomSoundRecorder::OnStop()
{
	if (PyObject_HasAttrString(SoundRecorder, "OnStop"))
	{
		PyObject *OnStop = PyObject_GetAttrString(SoundRecorder, "OnStop");
		PyObject_CallFunction(OnStop, NULL);
		Py_DECREF(OnStop);
	}
}

static void
PySfSoundRecorder_dealloc(PySfSoundRecorder* self)
{
	delete self->obj;
	free_object(self);
}

static PyObject *
PySfSoundRecorder_new(PyTypeObject *type, PyObject *args, PyObject *kwds)
{
	PySfSoundRecorder *self;
	self = (PySfSoundRecorder *)type->tp_alloc(type, 0);
	if (self != NULL)
	{
		self->obj = new CustomSoundRecorder();
		self->obj->SoundRecorder = (PyObject *)self;
	}
	return (PyObject *)self;
}

static PyObject *
PySfSoundRecorder_Start(PySfSoundRecorder* self, PyObject *args)
{
	self->obj->Start(PyLong_AsLong(args));
	Py_RETURN_NONE;
}

static PyObject *
PySfSoundRecorder_Stop(PySfSoundRecorder* self)
{
	self->obj->Stop();
	Py_RETURN_NONE;
}

static PyObject *
PySfSoundRecorder_GetSampleRate(PySfSoundRecorder* self)
{
	return PyLong_FromLong(self->obj->GetSampleRate());
}

static PyObject *
PySfSoundRecorder_IsAvailable(PySfSoundRecorder* self)
{
	return PyBool_FromLong(sf::SoundRecorder::IsAvailable());
}


static PyMethodDef PySfSoundRecorder_methods[] = {
	{"Start", (PyCFunction)PySfSoundRecorder_Start, METH_O, "Start(SampleRate=44100)\nStart the capture. Warning : only one capture can happen at the same time.\n	SampleRate : Sound frequency (the more samples, the higher the quality) (44100 by default = CD quality)."},
	{"Stop", (PyCFunction)PySfSoundRecorder_Stop, METH_NOARGS, "Stop()\nStop the capture."},
	{"GetSampleRate", (PyCFunction)PySfSoundRecorder_GetSampleRate, METH_NOARGS, "GetSampleRate()\nGet the sample rate. Returns the frequency, in samples per second."},
	{"IsAvailable", (PyCFunction)PySfSoundRecorder_IsAvailable, METH_STATIC | METH_NOARGS, "IsAvailable()\nTell if the system supports sound capture. If not, this class won't be usable. Returns True if audio capture is supported."},
	{NULL}  /* Sentinel */
};


PyTypeObject PySfSoundRecorderType = {
	head_init
	"SoundRecorder",		/*tp_name*/
	sizeof(PySfSoundRecorder), /*tp_basicsize*/
	0,						/*tp_itemsize*/
	(destructor)PySfSoundRecorder_dealloc, /*tp_dealloc*/
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
	"SoundRecorder is an interface for capturing sound data, it is meant to be used as a base class.\n\
Construct the sound recorder with a callback function for processing captured samples : SoundRecorder(Callback, UserData)\n\
	Callback : Callback function for processing captured samples. This function must take two parameters: the first one is a string containing captured samples, the second one is UserData.\n\
	UserData : Data to pass to the callback function (None by default).", /* tp_doc */
	0,						/* tp_traverse */
	0,						/* tp_clear */
	0,						/* tp_richcompare */
	0,						/* tp_weaklistoffset */
	0,						/* tp_iter */
	0,						/* tp_iternext */
	PySfSoundRecorder_methods, /* tp_methods */
	0,						/* tp_members */
	0,						/* tp_getset */
	0,						/* tp_base */
	0,						/* tp_dict */
	0,						/* tp_descr_get */
	0,						/* tp_descr_set */
	0,						/* tp_dictoffset */
	0,						/* tp_init */
	0,						/* tp_alloc */
	PySfSoundRecorder_new,	/* tp_new */
};

