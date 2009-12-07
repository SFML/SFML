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

#include "Music.hpp"

#include "compat.hpp"


extern PyTypeObject PySfSoundStreamType;


static void
PySfMusic_dealloc(PySfMusic *self)
{
	delete self->obj;
	free_object(self);
}

static PyObject *
PySfMusic_new(PyTypeObject *type, PyObject *args, PyObject *kwds)
{
	PySfMusic *self;
	self = (PySfMusic *)type->tp_alloc(type, 0);
	if (self != NULL)
	{
		self->obj = new sf::Music();
	}
	return (PyObject *)self;
}

static PyObject*
PySfMusic_OpenFromMemory(PySfMusic *self, PyObject *args)
{
	unsigned int SizeInBytes;
	char *Data;

	if (!PyArg_ParseTuple(args, "s#:Music.OpenFromMemory", &Data, &SizeInBytes))
		return NULL; 

	return PyBool_FromLong(self->obj->OpenFromMemory(Data, (std::size_t) SizeInBytes));
}

static PyObject*
PySfMusic_OpenFromFile(PySfMusic *self, PyObject *args)
{
	char *path;
#ifdef IS_PY3K
	PyObject *string = PyUnicode_AsUTF8String(args);
	if (string == NULL)
		return NULL;
	path = PyBytes_AsString(string);
#else
	path = PyString_AsString(args);
#endif
	bool result = self->obj->OpenFromFile(path);
#ifdef IS_PY3K
	Py_DECREF(string);
#endif
	return PyBool_FromLong(result);
}

static PyObject*
PySfMusic_GetDuration(PySfMusic *self)
{
	return PyFloat_FromDouble((double)(self->obj->GetDuration()));
}


static PyMethodDef PySfMusic_methods[] = {
	{"OpenFromFile", (PyCFunction)PySfMusic_OpenFromFile, METH_O, "OpenFromFile(Filename)\nOpen a music file (doesn't play it -- call Play() for that). Returns True if loading has been successful.\n	Filename : Path of the music file to open"},
	{"OpenFromMemory", (PyCFunction)PySfMusic_OpenFromMemory, METH_VARARGS, "OpenFromMemory(Data)\nOpen a music file (doesn't play it -- call Play() for that). Returns True if loading has been successful.\n	Data : string representing the file data in memory"},
	{"GetDuration", (PyCFunction)PySfMusic_GetDuration, METH_NOARGS, "GetDuration()\nGet the sound duration."},
	{NULL}  /* Sentinel */
};


PyTypeObject PySfMusicType = {
	head_init
	"Music",				/*tp_name*/
	sizeof(PySfMusic),		/*tp_basicsize*/
	0,						/*tp_itemsize*/
	(destructor)PySfMusic_dealloc, /*tp_dealloc*/
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
	"sf.Music defines a big sound played using streaming, so usually what we call a music :).\n\
Constructor: sf.Music()", /* tp_doc */
	0,						/* tp_traverse */
	0,						/* tp_clear */
	0,						/* tp_richcompare */
	0,						/* tp_weaklistoffset */
	0,						/* tp_iter */
	0,						/* tp_iternext */
	PySfMusic_methods,		/* tp_methods */
	0,						/* tp_members */
	0,						/* tp_getset */
	&PySfSoundStreamType,	/* tp_base */
	0,						/* tp_dict */
	0,						/* tp_descr_get */
	0,						/* tp_descr_set */
	0,						/* tp_dictoffset */
	0,						/* tp_init */
	0,						/* tp_alloc */
	PySfMusic_new,			/* tp_new */
};


