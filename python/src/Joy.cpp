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

#include <SFML/Window/Event.hpp>

#include <Python.h>
#include <structmember.h>

#include "Joy.hpp"


typedef struct {
	PyObject_HEAD
} PySfJoy;



static PyMemberDef PySfJoy_members[] = {
	{NULL}  /* Sentinel */
};


static void
PySfJoy_dealloc(PySfJoy *self)
{
	self->ob_type->tp_free((PyObject*)self);
}

static PyObject *
PySfJoy_new(PyTypeObject *type, PyObject *args, PyObject *kwds)
{
	PySfJoy *self;

	self = (PySfJoy *)type->tp_alloc(type, 0);
	if (self != NULL)
	{
	}

	return (PyObject *)self;
}


static int
PySfJoy_init(PySfJoy *self, PyObject *args, PyObject *kwds)
{
	return 0;
}

static PyMethodDef PySfJoy_methods[] = {
	{NULL}  /* Sentinel */
};

PyTypeObject PySfJoyType = {
	PyObject_HEAD_INIT(NULL)
	0,						/*ob_size*/
	"Joy",					/*tp_name*/
	sizeof(PySfJoy),		/*tp_basicsize*/
	0,						/*tp_itemsize*/
	(destructor)PySfJoy_dealloc, /*tp_dealloc*/
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
	"Definition of joystick axis for joystick events.", /* tp_doc */
	0,						/* tp_traverse */
	0,						/* tp_clear */
	0,						/* tp_richcompare */
	0,						/* tp_weaklistoffset */
	0,						/* tp_iter */
	0,						/* tp_iternext */
	PySfJoy_methods,		/* tp_methods */
	PySfJoy_members,		/* tp_members */
	0,						/* tp_getset */
	0,						/* tp_base */
	0,						/* tp_dict */
	0,						/* tp_descr_get */
	0,						/* tp_descr_set */
	0,						/* tp_dictoffset */
	(initproc)PySfJoy_init,	/* tp_init */
	0,						/* tp_alloc */
	PySfJoy_new,			/* tp_new */
};

void PySfJoy_InitConst()
{
	PyObject *obj;
	obj = PyInt_FromLong(sf::Joy::AxisX);
	PyDict_SetItemString(PySfJoyType.tp_dict, "AxisX", obj);
	Py_DECREF(obj);
	obj = PyInt_FromLong(sf::Joy::AxisY);
	PyDict_SetItemString(PySfJoyType.tp_dict, "AxisY", obj);
	Py_DECREF(obj);
	obj = PyInt_FromLong(sf::Joy::AxisZ);
	PyDict_SetItemString(PySfJoyType.tp_dict, "AxisZ", obj);
	Py_DECREF(obj);
	obj = PyInt_FromLong(sf::Joy::AxisR);
	PyDict_SetItemString(PySfJoyType.tp_dict, "AxisR", obj);
	Py_DECREF(obj);
	obj = PyInt_FromLong(sf::Joy::AxisU);
	PyDict_SetItemString(PySfJoyType.tp_dict, "AxisU", obj);
	Py_DECREF(obj);
	obj = PyInt_FromLong(sf::Joy::AxisV);
	PyDict_SetItemString(PySfJoyType.tp_dict, "AxisV", obj);
	Py_DECREF(obj);
	obj = PyInt_FromLong(sf::Joy::AxisPOV);
	PyDict_SetItemString(PySfJoyType.tp_dict, "AxisPOV", obj);
	Py_DECREF(obj);
}

