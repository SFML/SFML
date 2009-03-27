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

#include "Sound.hpp"
#include "SoundBuffer.hpp"

#include "compat.hpp"


extern PyTypeObject PySfSoundBufferType;


static void
PySfSound_dealloc(PySfSound *self)
{
	delete self->obj;
	free_object(self);
}

static PyObject *
PySfSound_new(PyTypeObject *type, PyObject *args, PyObject *kwds);

static int
PySfSound_init(PySfSound *self, PyObject *args, PyObject *kwds)
{
	const char *kwlist[] = {"Buffer", "Loop", "Pitch", "Volume", "X", "Y", "Z", NULL};
	PySfSoundBuffer *Buffer=NULL;
	PyObject *Loop=NULL;
	float Pitch=1.f, Volume=100.f, X=0.f, Y=0.f, Z=0.f;
	if (!PyArg_ParseTupleAndKeywords(args, kwds, "|O!Offfff:Sound.__new__", (char **)kwlist, &PySfSoundBufferType, &Buffer, &Loop, &Pitch, &Volume, &X, &Y, &Z))
		return -1;
	{
		if (Loop)
			self->obj->SetLoop(PyBool_AsBool(Loop));
		if (Buffer)
			self->obj->SetBuffer(*(Buffer->obj));
		self->obj->SetPitch(Pitch);
		self->obj->SetVolume(Volume);
		self->obj->SetPosition(X, Y, Z);
	}
	return 0;
}

static PyObject*
PySfSound_SetBuffer(PySfSound *self, PyObject *args)
{
	PySfSoundBuffer *Buffer = (PySfSoundBuffer *)args;
	if (!PyObject_TypeCheck(args, &PySfSoundBufferType))
	{
		PyErr_SetString(PyExc_TypeError, "Sound.SetBuffer() The argument must be a sf.SoundBuffer.");
		return NULL;
	}

	self->obj->SetBuffer(*(Buffer->obj));
	Py_RETURN_NONE;
}

static PyObject*
PySfSound_SetLoop(PySfSound *self, PyObject *args)
{
	self->obj->SetLoop(PyBool_AsBool(args));
	Py_RETURN_NONE;
}

static PyObject*
PySfSound_SetRelativeToListener(PySfSound *self, PyObject *args)
{
	self->obj->SetRelativeToListener(PyBool_AsBool(args));
	Py_RETURN_NONE;
}

static PyObject*
PySfSound_IsRelativeToListener(PySfSound *self)
{
	return PyBool_FromLong(self->obj->IsRelativeToListener());
}

static PyObject*
PySfSound_SetPitch(PySfSound *self, PyObject *args)
{
	self->obj->SetPitch(PyFloat_AsDouble(args));
	Py_RETURN_NONE;
}

static PyObject*
PySfSound_SetMinDistance(PySfSound *self, PyObject *args)
{
	self->obj->SetMinDistance(PyFloat_AsDouble(args));
	Py_RETURN_NONE;
}

static PyObject*
PySfSound_SetAttenuation(PySfSound *self, PyObject *args)
{
	self->obj->SetAttenuation(PyFloat_AsDouble(args));
	Py_RETURN_NONE;
}

static PyObject*
PySfSound_SetVolume(PySfSound *self, PyObject *args)
{
	self->obj->SetVolume(PyFloat_AsDouble(args));
	Py_RETURN_NONE;
}

static PyObject*
PySfSound_GetPitch(PySfSound *self)
{
	return PyFloat_FromDouble(self->obj->GetPitch());
}

static PyObject*
PySfSound_GetMinDistance(PySfSound *self)
{
	return PyFloat_FromDouble(self->obj->GetMinDistance());
}

static PyObject*
PySfSound_GetAttenuation(PySfSound *self)
{
	return PyFloat_FromDouble(self->obj->GetAttenuation());
}

static PyObject*
PySfSound_GetVolume(PySfSound *self)
{
	return PyFloat_FromDouble(self->obj->GetVolume());
}

static PyObject*
PySfSound_GetPosition(PySfSound *self)
{
	sf::Vector3f Vect = self->obj->GetPosition();
	return Py_BuildValue("fff", Vect.x, Vect.y, Vect.z);
}

static PyObject*
PySfSound_GetPlayingOffset(PySfSound *self)
{
	return PyFloat_FromDouble(self->obj->GetPlayingOffset());
}

static PyObject*
PySfSound_GetLoop(PySfSound *self)
{
	return PyBool_FromLong(self->obj->GetLoop());
}

static PyObject*
PySfSound_Play(PySfSound *self)
{
	self->obj->Play();
	Py_RETURN_NONE;
}

static PyObject*
PySfSound_Pause(PySfSound *self)
{
	self->obj->Pause();
	Py_RETURN_NONE;
}

static PyObject*
PySfSound_Stop(PySfSound *self)
{
	self->obj->Stop();
	Py_RETURN_NONE;
}

static PyObject*
PySfSound_GetStatus(PySfSound *self)
{
	return PyLong_FromUnsignedLong(self->obj->GetStatus());
}

static PyObject*
PySfSound_SetPosition(PySfSound *self, PyObject *args)
{
	float X, Y, Z;
	if (!PyArg_ParseTuple(args, "fff:Sound.SetPosition", &X, &Y, &Z))
		return NULL; 
	self->obj->SetPosition(X, Y, Z);
	Py_RETURN_NONE;
}

static PyObject *
PySfSound_GetBuffer(PySfSound *self)
{
	PySfSoundBuffer *Buffer;

	Buffer = GetNewPySfSoundBuffer();
	Buffer->obj = new sf::SoundBuffer(*(self->obj->GetBuffer()));

	return (PyObject *)Buffer;
}

static PyObject*
PySfSound_SetPlayingOffset(PySfSound *self, PyObject *args)
{
	self->obj->SetPlayingOffset(PyFloat_AsDouble(args));
	Py_RETURN_NONE;
}

static PyMethodDef PySfSound_methods[] = {
	{"SetRelativeToListener", (PyCFunction)PySfSound_SetRelativeToListener, METH_O, "SetRelativeToListener(Relative)\nMake the sound's position relative to the listener's position, or absolute. The default value is false (absolute)\n	Relative : True to set the position relative, false to set it absolute"},
	{"IsRelativeToListener", (PyCFunction)PySfSound_IsRelativeToListener, METH_NOARGS, "IsRelativeToListener()\nTell if the sound's position is relative to the listener's position, or if it's absolute."},
	{"SetPlayingOffset", (PyCFunction)PySfSound_SetPlayingOffset, METH_O, "SetPlayingOffset(TimeOffset)\nSet the current playing position of the sound.\n	TimeOffset : New playing position, expressed in seconds"},
	{"SetLoop", (PyCFunction)PySfSound_SetLoop, METH_O, "SetLoop(Loop)\nSet the Sound loop state.\n	Loop : True to play in loop, false to play once"},
	{"SetBuffer", (PyCFunction)PySfSound_SetBuffer, METH_O, "SetBuffer(Buffer)\nSet the source buffer.\n	Buffer : New sound buffer to bind to the sound "},
	{"SetPitch", (PyCFunction)PySfSound_SetPitch, METH_O, "SetPitch(Pitch)\nSet the sound pitch. The default pitch is 1.\n	Pitch : New pitch"},
	{"SetMinDistance", (PyCFunction)PySfSound_SetMinDistance, METH_O, "SetMinDistance(MinDistance)\nSet the minimum distance - closer than this distance, the listener will hear the sound at its maximum volume. The default minimum distance is 1.0.\n	MinDistance : New minimum distance for the sound"},
	{"SetAttenuation", (PyCFunction)PySfSound_SetAttenuation, METH_O, "SetAttenuation(Attenuation)\nSet the attenuation factor - the higher the attenuation, the more the sound will be attenuated with distance from listener. The default attenuation factor 1.0.\n	Attenuation : New attenuation factor for the sound"},
	{"SetVolume", (PyCFunction)PySfSound_SetVolume, METH_O, "SetVolume(Volume)\nSet the sound volume.\n	Volume : Volume (in range [0, 100])"},
	{"SetPosition", (PyCFunction)PySfSound_SetPosition, METH_VARARGS, "SetPosition(X, Y, Z)\nSet the sound position in the world.\n	X,Y,Z : Position of the sound in the world"},
	{"GetLoop", (PyCFunction)PySfSound_GetLoop, METH_NOARGS, "GetLoop()\nTell whether or not the Sound is looping."},
	{"GetBuffer", (PyCFunction)PySfSound_GetBuffer, METH_NOARGS, "GetBuffer()\nGet the source buffer. Returns a new sf.SoundBuffer object."},
	{"GetPitch", (PyCFunction)PySfSound_GetPitch, METH_NOARGS, "GetPitch()\nGet the sound pitch."},
	{"GetMinDistance", (PyCFunction)PySfSound_GetMinDistance, METH_NOARGS, "GetMinDistance()\nGet the minimum distance."},
	{"GetAttenuation", (PyCFunction)PySfSound_GetAttenuation, METH_NOARGS, "GetAttenuation()\nGet the attenuation factor."},
	{"GetVolume", (PyCFunction)PySfSound_GetVolume, METH_NOARGS, "GetVolume()\nGet the sound volume."},
	{"GetPosition", (PyCFunction)PySfSound_GetPosition, METH_NOARGS, "GetPosition()\nGet the sound position in the world. Returns a tuple."},
	{"Play", (PyCFunction)PySfSound_Play, METH_NOARGS, "Play()\nPlay the sound."},
	{"Pause", (PyCFunction)PySfSound_Pause, METH_NOARGS, "Pause()\nPause the sound."},
	{"Stop", (PyCFunction)PySfSound_Stop, METH_NOARGS, "Stop()\nStop the sound."},
	{"GetStatus", (PyCFunction)PySfSound_GetStatus, METH_NOARGS, "GetStatus()\nGet the status of the sound (stopped, paused, playing)."},
	{"GetPlayingOffset", (PyCFunction)PySfSound_GetPlayingOffset, METH_NOARGS, "GetPlayingOffset()\nGet the current playing position of the sound."},
	{NULL}  /* Sentinel */
};

PyTypeObject PySfSoundType = {
	head_init
	"Sound",				/*tp_name*/
	sizeof(PySfSound),		/*tp_basicsize*/
	0,						/*tp_itemsize*/
	(destructor)PySfSound_dealloc, /*tp_dealloc*/
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
	"sf.Sound defines the properties of a sound such as position, volume, pitch, etc.\n\
Default constructor : Sound()\n\
Construct the sound from its parameters : Sound(Buffer, Loop = False, Pitch = 1., Volume = 100., X = 0., Y = 0., Z = 0.);\n\
	Buffer : Sound buffer to play (None by default)\n\
	Loop :   Loop flag (False by default)\n\
	Pitch :  Value of the pitch (1. by default)\n\
	Volume : Volume (100. by default)\n\
	X :      X position (0. by default)\n\
	Y :      Y position (0. by default)\n\
	Z :      Z position (0. by default)\n\
Copy constructor : Sound(Copy) where Copy is a sf.Sound instance.", /* tp_doc */
	0,						/* tp_traverse */
	0,						/* tp_clear */
	0,						/* tp_richcompare */
	0,						/* tp_weaklistoffset */
	0,						/* tp_iter */
	0,						/* tp_iternext */
	PySfSound_methods,		/* tp_methods */
	0,						/* tp_members */
	0,						/* tp_getset */
	0,						/* tp_base */
	0,						/* tp_dict */
	0,						/* tp_descr_get */
	0,						/* tp_descr_set */
	0,						/* tp_dictoffset */
	(initproc)PySfSound_init, /* tp_init */
	0,						/* tp_alloc */
	PySfSound_new,			/* tp_new */
};

static PyObject *
PySfSound_new(PyTypeObject *type, PyObject *args, PyObject *kwds)
{
	PySfSound *self;
	self = (PySfSound *)type->tp_alloc(type, 0);
	if (self != NULL)
	{
		if (PyTuple_Size(args) == 1)
		{
			PySfSound *Copy;
			if (PyArg_ParseTuple(args, "O!:Sound.__new__", &PySfSoundType, &Copy))
			{
				self->obj = new sf::Sound(*(Copy->obj));
				return (PyObject *)self;
			}
			else PyErr_Clear();
		}
		self->obj = new sf::Sound();
	}
	return (PyObject *)self;
}

void
PySfSound_InitConst()
{
	PyObject *obj;
	obj = PyLong_FromLong(sf::Sound::Stopped);
	PyDict_SetItemString(PySfSoundType.tp_dict, "Stopped", obj);
	Py_DECREF(obj);
	obj = PyLong_FromLong(sf::Sound::Paused);
	PyDict_SetItemString(PySfSoundType.tp_dict, "Paused", obj);
	Py_DECREF(obj);
	obj = PyLong_FromLong(sf::Sound::Playing);
	PyDict_SetItemString(PySfSoundType.tp_dict, "Playing", obj);
	Py_DECREF(obj);
}

