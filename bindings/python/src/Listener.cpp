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

#include "Listener.hpp"

#include "compat.hpp"


static PyObject *
PySfListener_SetGlobalVolume(PySfListener* self, PyObject *args)
{
	sf::Listener::SetGlobalVolume(PyFloat_AsDouble(args));
	Py_RETURN_NONE;
}

static PyObject *
PySfListener_GetGlobalVolume(PySfListener* self)
{
	return PyFloat_FromDouble(sf::Listener::GetGlobalVolume());
}

static PyObject *
PySfListener_SetPosition(PySfListener* self, PyObject *args)
{
	float X, Y, Z;
	if (!PyArg_ParseTuple(args, "fff:Listener.SetPosition", &X, &Y, &Z))
		return NULL; 
	sf::Listener::SetPosition(X, Y, Z);
	Py_RETURN_NONE;
}

static PyObject*
PySfListener_GetPosition(PySfListener *self)
{
	sf::Vector3f Vect = sf::Listener::GetPosition();
	return Py_BuildValue("fff", Vect.x, Vect.y, Vect.z);
}

static PyObject *
PySfListener_SetDirection(PySfListener* self, PyObject *args)
{
	float X, Y, Z;
	if (!PyArg_ParseTuple(args, "fff:Listener.SetDirection", &X, &Y, &Z))
		return NULL; 
	sf::Listener::SetDirection(X, Y, Z);
	Py_RETURN_NONE;
}

static PyObject*
PySfListener_GetDirection(PySfListener *self)
{
	sf::Vector3f Vect = sf::Listener::GetDirection();
	return Py_BuildValue("fff", Vect.x, Vect.y, Vect.z);
}

static PyMethodDef PySfListener_methods[] = {
	{"SetGlobalVolume", (PyCFunction)PySfListener_SetGlobalVolume, METH_STATIC | METH_O, "SetGlobalVolume(Volume)\nChange the global volume of all the sounds."},
	{"GetGlobalVolume", (PyCFunction)PySfListener_GetGlobalVolume, METH_STATIC | METH_NOARGS, "GetGlobalVolume()\nGet the current value of the global volume of all the sounds."},
	{"SetPosition", (PyCFunction)PySfListener_SetPosition, METH_STATIC | METH_VARARGS, "SetPosition(X, Y, Z)\nChange the position of the listener."},
	{"GetPosition", (PyCFunction)PySfListener_GetPosition, METH_STATIC | METH_NOARGS, "GetPosition()\nGet the current position of the listener."},
	{"SetDirection", (PyCFunction)PySfListener_SetDirection, METH_STATIC | METH_VARARGS, "SetDirection(X, Y, Z)\nChange the orientation of the listener (the point he must look at)"},
	{"GetDirection", (PyCFunction)PySfListener_GetDirection, METH_STATIC | METH_NOARGS, "GetDirection()\nGet the current orientation of the listener (the point he's looking at)"},
	{NULL}  /* Sentinel */
};

PyTypeObject PySfListenerType = {
	head_init
	"Listener",             /*tp_name*/
	sizeof(PySfListener),	/*tp_basicsize*/
	0,						/*tp_itemsize*/
	0,						/*tp_dealloc*/
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
	"Listener is a global interface for defining the audio listener properties ; the audio listener is the point in the scene from where all the sounds are heard.", /* tp_doc */
	0,						/* tp_traverse */
	0,						/* tp_clear */
	0,						/* tp_richcompare */
	0,						/* tp_weaklistoffset */
	0,						/* tp_iter */
	0,						/* tp_iternext */
	PySfListener_methods,	/* tp_methods */
	0,						/* tp_members */
	0,						/* tp_getset */
	0,						/* tp_base */
	0,						/* tp_dict */
	0,						/* tp_descr_get */
	0,						/* tp_descr_set */
	0,						/* tp_dictoffset */
	0,						/* tp_init */
	0,						/* tp_alloc */
	0,						/* tp_new */
};


