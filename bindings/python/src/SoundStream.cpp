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

#include "SoundStream.hpp"

#include "compat.hpp"


void CustomSoundStream::OnSeek(float TimeOffset)
{
	PyGILState_STATE gstate;
	gstate = PyGILState_Ensure();
	if (PyObject_HasAttrString(SoundStream, "OnSeek"))
	{
		PyObject *OnSeek = PyObject_GetAttrString(SoundStream, "OnSeek");
		if (OnSeek != NULL)
		{
			PyObject_CallFunction(OnSeek, const_cast<char*>( "f" ), TimeOffset);
			Py_CLEAR(OnSeek);
		}
	}
	if (PyErr_Occurred())
	{
		PyErr_Print();
	}
	PyGILState_Release(gstate);
}

bool CustomSoundStream::OnGetData(Chunk& Data)
{
	PyGILState_STATE gstate;
	bool result = false;
	PyObject *Function, *PyData;
	gstate = PyGILState_Ensure();
	Function = PyObject_GetAttrString(SoundStream, "OnGetData");
	if (Function != NULL)
	{
		PyData = PyObject_CallFunction(Function, NULL);
		if (PyData != NULL)
		{
			if (PyArg_Parse(PyData, "s#", &(Data.Samples), &(Data.NbSamples)))
			{
				Data.NbSamples /= 2;
				if (Data.NbSamples > 0)
					result = true;
			}
			Py_CLEAR(PyData);
		}
		Py_CLEAR(Function);
	}
	if (PyErr_Occurred())
	{
		PyErr_Print();
		result = false;
	}
	PyGILState_Release(gstate);
    return result;
}

void CustomSoundStream::Init(unsigned int ChannelsCount, unsigned int SampleRate)
{
	Initialize(ChannelsCount, SampleRate);
}

static void
PySfSoundStream_dealloc(PySfSoundStream *self)
{
	delete self->obj;
	free_object(self);
}

static PyObject *
PySfSoundStream_new(PyTypeObject *type, PyObject *args, PyObject *kwds)
{
	PySfSoundStream *self;
	self = (PySfSoundStream *)type->tp_alloc(type, 0);
	if (self != NULL)
	{
		self->obj = new CustomSoundStream();
		self->obj->SoundStream = (PyObject *)self;
	}
	return (PyObject *)self;
}

static PyObject *
PySfSoundStream_Initialize(PySfSoundStream *self, PyObject *args)
{
	unsigned int ChannelsCount, SampleRate;
	if (!PyArg_ParseTuple(args, "II:SoundStream.Initialize", &ChannelsCount, &SampleRate))
		return NULL;
	self->obj->Init(ChannelsCount, SampleRate);
	Py_RETURN_NONE;
}

static PyObject*
PySfSoundStream_Play(PySfSoundStream *self)
{
	self->obj->Play();
	Py_RETURN_NONE;
}

static PyObject*
PySfSoundStream_Stop(PySfSoundStream *self)
{
	self->obj->Stop();
	Py_RETURN_NONE;
}

static PyObject*
PySfSoundStream_GetChannelsCount(PySfSoundStream *self)
{
	return PyLong_FromUnsignedLong(self->obj->GetChannelsCount());
}

static PyObject*
PySfSoundStream_GetSampleRate(PySfSoundStream *self)
{
	return PyLong_FromUnsignedLong(self->obj->GetSampleRate());
}

static PyObject*
PySfSoundStream_SetPitch(PySfSoundStream *self, PyObject *args)
{
	self->obj->SetPitch(PyFloat_AsDouble(args));
	Py_RETURN_NONE;
}

static PyObject*
PySfSoundStream_SetMinDistance(PySfSoundStream *self, PyObject *args)
{
	self->obj->SetMinDistance(PyFloat_AsDouble(args));
	Py_RETURN_NONE;
}

static PyObject*
PySfSoundStream_SetAttenuation(PySfSoundStream *self, PyObject *args)
{
	self->obj->SetAttenuation(PyFloat_AsDouble(args));
	Py_RETURN_NONE;
}

static PyObject*
PySfSoundStream_SetVolume(PySfSoundStream *self, PyObject *args)
{
	self->obj->SetVolume(PyFloat_AsDouble(args));
	Py_RETURN_NONE;
}

static PyObject*
PySfSoundStream_GetPitch(PySfSoundStream *self)
{
	return PyFloat_FromDouble(self->obj->GetPitch());
}

static PyObject*
PySfSoundStream_GetMinDistance(PySfSoundStream *self)
{
	return PyFloat_FromDouble(self->obj->GetMinDistance());
}

static PyObject*
PySfSoundStream_GetAttenuation(PySfSoundStream *self)
{
	return PyFloat_FromDouble(self->obj->GetAttenuation());
}

static PyObject*
PySfSoundStream_GetVolume(PySfSoundStream *self)
{
	return PyFloat_FromDouble(self->obj->GetVolume());
}

static PyObject*
PySfSoundStream_GetPosition(PySfSoundStream *self)
{
	sf::Vector3f Vect = self->obj->GetPosition();
	return Py_BuildValue("fff", Vect.x, Vect.y, Vect.z);
}

static PyObject*
PySfSoundStream_Pause(PySfSoundStream *self)
{
	self->obj->Pause();
	Py_RETURN_NONE;
}

static PyObject*
PySfSoundStream_SetPosition(PySfSoundStream *self, PyObject *args)
{
	float X, Y, Z;
	if (!PyArg_ParseTuple(args, "fff:SoundStream.SetPosition", &X, &Y, &Z))
		return NULL; 
	self->obj->SetPosition(X, Y, Z);
	Py_RETURN_NONE;
}

static PyObject*
PySfSoundStream_GetStatus(PySfSoundStream *self)
{
	return PyLong_FromUnsignedLong(self->obj->GetStatus());
}

static PyObject*
PySfSoundStream_SetLoop(PySfSoundStream *self, PyObject *args)
{
	self->obj->SetLoop(PyBool_AsBool(args));
	Py_RETURN_NONE;
}

static PyObject*
PySfSoundStream_GetLoop(PySfSoundStream *self)
{
	return PyBool_FromLong(self->obj->GetLoop());
}

static PyObject*
PySfSoundStream_GetPlayingOffset(PySfSoundStream *self)
{
	return PyFloat_FromDouble(self->obj->GetPlayingOffset());
}

static PyObject*
PySfSoundStream_SetRelativeToListener(PySfSoundStream *self, PyObject *args)
{
	self->obj->SetRelativeToListener(PyBool_AsBool(args));
	Py_RETURN_NONE;
}

static PyObject*
PySfSoundStream_IsRelativeToListener(PySfSoundStream *self)
{
	return PyBool_FromLong(self->obj->IsRelativeToListener());
}

static PyMethodDef PySfSoundStream_methods[] = {
	{"SetRelativeToListener", (PyCFunction)PySfSoundStream_SetRelativeToListener, METH_O, "SetRelativeToListener(Relative)\nMake the sound's position relative to the listener's position, or absolute. The default value is false (absolute)\n	Relative : True to set the position relative, false to set it absolute"},
	{"IsRelativeToListener", (PyCFunction)PySfSoundStream_IsRelativeToListener, METH_NOARGS, "IsRelativeToListener()\nTell if the sound's position is relative to the listener's position, or if it's absolute."},
	{"Initialize", (PyCFunction)PySfSoundStream_Initialize, METH_VARARGS, "Initialize(ChannelsCount, SampleRate)\n\
Set the audio stream parameters, you must call it before Play()\n\
	ChannelsCount : Number of channels\n\
	SampleRate :    Sample rate."},
	{"Play", (PyCFunction)PySfSoundStream_Play, METH_NOARGS, "Play()\nPlay the sound."},
	{"Stop", (PyCFunction)PySfSoundStream_Stop, METH_NOARGS, "Stop()\nStop the sound."},
	{"GetChannelsCount", (PyCFunction)PySfSoundStream_GetChannelsCount, METH_NOARGS, "GetChannelsCount()\nReturn the number of channels (1 = mono, 2 = stereo)."},
	{"GetSampleRate", (PyCFunction)PySfSoundStream_GetSampleRate, METH_NOARGS, "GetSampleRate()\nGet the sound frequency (sample rate)."},
	{"GetStatus", (PyCFunction)PySfSoundStream_GetStatus, METH_NOARGS, "GetStatus()\nGet the status of the sound (stopped, paused, playing)."},
	{"SetLoop", (PyCFunction)PySfSoundStream_SetLoop, METH_O, "SetLoop(Loop)\nSet the music loop state. This parameter is disabled by default\n	Loop : True to play in loop, false to play once "},
	{"GetLoop", (PyCFunction)PySfSoundStream_GetLoop, METH_NOARGS, "GetLoop()\nTell whether or not the music is looping."},
	{"GetPlayingOffset", (PyCFunction)PySfSoundStream_GetPlayingOffset, METH_NOARGS, "GetPlayingOffset()\nGet the current playing position of the stream."},
/* The following methods should be inherited from sf.Sound */
	{"SetPitch", (PyCFunction)PySfSoundStream_SetPitch, METH_O, "SetPitch(Pitch)\nSet the sound pitch. The default pitch is 1.\n	Pitch : New pitch"},
	{"SetMinDistance", (PyCFunction)PySfSoundStream_SetMinDistance, METH_O, "SetMinDistance(MinDistance)\nSet the minimum distance - closer than this distance, the listener will hear the sound at its maximum volume. The default minimum distance is 1.0.\n	MinDistance : New minimum distance for the sound"},
	{"SetAttenuation", (PyCFunction)PySfSoundStream_SetAttenuation, METH_O, "SetAttenuation(Attenuation)\nSet the attenuation factor - the higher the attenuation, the more the sound will be attenuated with distance from listener. The default attenuation factor 1.0.\n	Attenuation : New attenuation factor for the sound"},
	{"SetVolume", (PyCFunction)PySfSoundStream_SetVolume, METH_O, "SetVolume(Volume)\nSet the sound volume.\n	Volume : Volume (in range [0, 100])"},
	{"SetPosition", (PyCFunction)PySfSoundStream_SetPosition, METH_VARARGS, "SetPosition(X, Y, Z)\nSet the sound position in the world.\n	X,Y,Z : Position of the sound in the world"},
	{"GetPitch", (PyCFunction)PySfSoundStream_GetPitch, METH_NOARGS, "GetPitch()\nGet the sound pitch."},
	{"GetMinDistance", (PyCFunction)PySfSoundStream_GetMinDistance, METH_NOARGS, "GetMinDistance()\nGet the minimum distance."},
	{"GetAttenuation", (PyCFunction)PySfSoundStream_GetAttenuation, METH_NOARGS, "GetAttenuation()\nGet the attenuation factor."},
	{"GetVolume", (PyCFunction)PySfSoundStream_GetVolume, METH_NOARGS, "GetVolume()\nGet the sound volume."},
	{"GetPosition", (PyCFunction)PySfSoundStream_GetPosition, METH_NOARGS, "GetPosition()\nGet the sound position in the world. Returns a tuple."},
	{"Pause", (PyCFunction)PySfSoundStream_Pause, METH_NOARGS, "Pause()\nPause the sound."},
	{NULL}  /* Sentinel */
};


PyTypeObject PySfSoundStreamType = {
	head_init
	"SoundStream",			/*tp_name*/
	sizeof(PySfSoundStream), /*tp_basicsize*/
	0,						/*tp_itemsize*/
	(destructor)PySfSoundStream_dealloc, /*tp_dealloc*/
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
	"SoundStream is a streamed sound, ie samples are acquired\
while the sound is playing. Use it for big sounds that would\
require hundreds of MB in memory (see Music),\
or for streaming sound from the network", /* tp_doc */
	0,						/* tp_traverse */
	0,						/* tp_clear */
	0,						/* tp_richcompare */
	0,						/* tp_weaklistoffset */
	0,						/* tp_iter */
	0,						/* tp_iternext */
	PySfSoundStream_methods, /* tp_methods */
	0,						/* tp_members */
	0,						/* tp_getset */
	0,						/* tp_base */
	0,						/* tp_dict */
	0,						/* tp_descr_get */
	0,						/* tp_descr_set */
	0,						/* tp_dictoffset */
	0,						/* tp_init */
	0,						/* tp_alloc */
	PySfSoundStream_new,	/* tp_new */
};


void
PySfSoundStream_InitConst()
{
	PyObject *obj;
	obj = PyLong_FromLong(sf::SoundStream::Stopped);
	PyDict_SetItemString(PySfSoundStreamType.tp_dict, "Stopped", obj);
	Py_DECREF(obj);
	obj = PyLong_FromLong(sf::SoundStream::Paused);
	PyDict_SetItemString(PySfSoundStreamType.tp_dict, "Paused", obj);
	Py_DECREF(obj);
	obj = PyLong_FromLong(sf::SoundStream::Playing);
	PyDict_SetItemString(PySfSoundStreamType.tp_dict, "Playing", obj);
	Py_DECREF(obj);
}

