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

#include "Blend.hpp"

#include "compat.hpp"


static PyObject *
PySfBlend_new(PyTypeObject *type, PyObject *args, PyObject *kwds)
{
	PySfBlend *self;
	self = (PySfBlend *)type->tp_alloc(type, 0);
	return (PyObject *)self;
}


PyTypeObject PySfBlendType = {
	head_init
	"Blend",				/*tp_name*/
	sizeof(PySfBlend),		/*tp_basicsize*/
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
	"Enumerate the blending modes for drawable objects.\n\
Alpha    Pixel = Src * a + Dest * (1 - a).\n\
Add      Pixel = Src + Dest.\n\
Multiply Pixel = Src * Dest.\n\
None     No blending.",		/* tp_doc */
	0,						/* tp_traverse */
	0,						/* tp_clear */
	0,						/* tp_richcompare */
	0,						/* tp_weaklistoffset */
	0,						/* tp_iter */
	0,						/* tp_iternext */
	0,						/* tp_methods */
	0,						/* tp_members */
	0,						/* tp_getset */
	0,						/* tp_base */
	0,						/* tp_dict */
	0,						/* tp_descr_get */
	0,						/* tp_descr_set */
	0,						/* tp_dictoffset */
	0,						/* tp_init */
	0,						/* tp_alloc */
	PySfBlend_new,			/* tp_new */
};

void PySfBlend_InitConst()
{
	PyObject *obj;
	obj = PyLong_FromLong(sf::Blend::Alpha);
	PyDict_SetItemString(PySfBlendType.tp_dict, "Alpha", obj);
	Py_DECREF(obj);
	obj = PyLong_FromLong(sf::Blend::Add);
	PyDict_SetItemString(PySfBlendType.tp_dict, "Add", obj);
	Py_DECREF(obj);
	obj = PyLong_FromLong(sf::Blend::Multiply);
	PyDict_SetItemString(PySfBlendType.tp_dict, "Multiply", obj);
	Py_DECREF(obj);
	obj = PyLong_FromLong(sf::Blend::None);
	PyDict_SetItemString(PySfBlendType.tp_dict, "None", obj);
	Py_DECREF(obj);
}

