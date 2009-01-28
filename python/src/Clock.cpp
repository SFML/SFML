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

#include "Clock.hpp"


typedef struct {
	PyObject_HEAD
	sf::Clock *obj;
} PySfClock;



static PyMemberDef PySfClock_members[] = {
	{NULL}  /* Sentinel */
};


static void
PySfClock_dealloc(PySfClock *self)
{
	delete self->obj;
	self->ob_type->tp_free((PyObject*)self);
}

static PyObject *
PySfClock_new(PyTypeObject *type, PyObject *args, PyObject *kwds)
{
	PySfClock *self;

	self = (PySfClock *)type->tp_alloc(type, 0);
	if (self != NULL)
	{
	}

	return (PyObject *)self;
}


static int
PySfClock_init(PySfClock *self, PyObject *args, PyObject *kwds)
{
	self->obj = new sf::Clock();
	return 0;
}


static PyObject*
PySfClock_GetElapsedTime(PySfClock *self)
{
	return PyFloat_FromDouble(self->obj->GetElapsedTime());
}

static PyObject*
PySfClock_Reset(PySfClock *self)
{
	self->obj->Reset();
	Py_RETURN_NONE;
}

static PyMethodDef PySfClock_methods[] = {
	{"GetElapsedTime", (PyCFunction)PySfClock_GetElapsedTime, METH_NOARGS, "GetElapsedTime()\nGet the time elapsed since last reset."},
	{"Reset", (PyCFunction)PySfClock_Reset, METH_NOARGS, "Reset()\nRestart the timer."},
	{NULL}  /* Sentinel */
};

PyTypeObject PySfClockType = {
	PyObject_HEAD_INIT(NULL)
	0,						/*ob_size*/
	"Clock",				/*tp_name*/
	sizeof(PySfClock),		/*tp_basicsize*/
	0,						/*tp_itemsize*/
	(destructor)PySfClock_dealloc, /*tp_dealloc*/
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
	"sf.Clock is an utility class for manipulating time.", /* tp_doc */
	0,						/* tp_traverse */
	0,						/* tp_clear */
	0,						/* tp_richcompare */
	0,						/* tp_weaklistoffset */
	0,						/* tp_iter */
	0,						/* tp_iternext */
	PySfClock_methods,		/* tp_methods */
	PySfClock_members,		/* tp_members */
	0,						/* tp_getset */
	0,						/* tp_base */
	0,						/* tp_dict */
	0,						/* tp_descr_get */
	0,						/* tp_descr_set */
	0,						/* tp_dictoffset */
	(initproc)PySfClock_init, /* tp_init */
	0,						/* tp_alloc */
	PySfClock_new,			/* tp_new */
};


