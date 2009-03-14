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

#include "Color.hpp"

#include "offsetof.hpp"
#include "compat.hpp"

static PyMemberDef PySfColor_members[] = {
	{(char *)"r", T_UBYTE, offsetof(PySfColor, r), 0, (char *)"Red component."},
	{(char *)"g", T_UBYTE, offsetof(PySfColor, g), 0, (char *)"Green component."},
	{(char *)"b", T_UBYTE, offsetof(PySfColor, b), 0, (char *)"Blue component."},
	{(char *)"a", T_UBYTE, offsetof(PySfColor, a), 0, (char *)"Alpha (transparency) component."},
	{NULL}  /* Sentinel */
};



static void
PySfColor_dealloc(PySfColor *self)
{
	delete self->obj;
	free_object(self);
}

void
PySfColorUpdate(PySfColor *self)
{
	self->obj->r = self->r;
	self->obj->g = self->g;
	self->obj->b = self->b;
	self->obj->a = self->a;
}

static PyObject *
PySfColor_new(PyTypeObject *type, PyObject *args, PyObject *kwds)
{
	PySfColor *self;
	self = (PySfColor *)type->tp_alloc(type, 0);
	if (self != NULL)
	{
		self->r = 0;
		self->g = 0;
		self->b = 0;
		self->a = 255;
		self->obj = new sf::Color(0, 0, 0, 255);
	}
	return (PyObject *)self;
}

static int
PySfColor_init(PySfColor *self, PyObject *args, PyObject *kwds)
{
	const char *kwlist[] = {"r", "g", "b", "a", NULL};
	if (!PyArg_ParseTupleAndKeywords(args, kwds, "BBB|B:Color.__init__", (char **)kwlist, &(self->r), &(self->g), &(self->b), &(self->a)))
		return -1;
	PySfColorUpdate(self);
	return 0;
}

PyTypeObject PySfColorType = {
	head_init
	"Color",				/*tp_name*/
	sizeof(PySfColor),		/*tp_basicsize*/
	0,						/*tp_itemsize*/
	(destructor)PySfColor_dealloc, /*tp_dealloc*/
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
	"sf.Color is an utility class for manipulating 32-bits RGBA colors.", /* tp_doc */
	0,						/* tp_traverse */
	0,						/* tp_clear */
	0,						/* tp_richcompare */
	0,						/* tp_weaklistoffset */
	0,						/* tp_iter */
	0,						/* tp_iternext */
	0,						/* tp_methods */
	PySfColor_members,		/* tp_members */
	0,						/* tp_getset */
	0,						/* tp_base */
	0,						/* tp_dict */
	0,						/* tp_descr_get */
	0,						/* tp_descr_set */
	0,						/* tp_dictoffset */
	(initproc)PySfColor_init, /* tp_init */
	0,						/* tp_alloc */
	PySfColor_new,			/* tp_new */
};

PySfColor *
GetNewPySfColor()
{
	return PyObject_New(PySfColor, &PySfColorType);
}

void
PySfColor_InitConst()
{
	PySfColor *Black, *White, *Red, *Green, *Blue, *Yellow, *Magenta, *Cyan;
	Black = GetNewPySfColor();
	Black->obj = (sf::Color *) &(sf::Color::Black);
	Black->r = sf::Color::Black.r;
	Black->g = sf::Color::Black.g;
	Black->b = sf::Color::Black.b;
	Black->a = sf::Color::Black.a;
	PyDict_SetItemString(PySfColorType.tp_dict, "Black", (PyObject *)Black);
	Py_DECREF(Black);
	White = GetNewPySfColor();
	White->obj = (sf::Color *) &(sf::Color::White);
	White->r = sf::Color::White.r;
	White->g = sf::Color::White.g;
	White->b = sf::Color::White.b;
	White->a = sf::Color::White.a;
	PyDict_SetItemString(PySfColorType.tp_dict, "White", (PyObject *)White);
	Py_DECREF(White);
	Red = GetNewPySfColor();
	Red->obj = (sf::Color *) &(sf::Color::Red);
	Red->r = sf::Color::Red.r;
	Red->g = sf::Color::Red.g;
	Red->b = sf::Color::Red.b;
	Red->a = sf::Color::Red.a;
	PyDict_SetItemString(PySfColorType.tp_dict, "Red", (PyObject *)Red);
	Py_DECREF(Red);
	Green = GetNewPySfColor();
	Green->obj = (sf::Color *) &(sf::Color::Green);
	Green->r = sf::Color::Green.r;
	Green->g = sf::Color::Green.g;
	Green->b = sf::Color::Green.b;
	Green->a = sf::Color::Green.a;
	PyDict_SetItemString(PySfColorType.tp_dict, "Green", (PyObject *)Green);
	Py_DECREF(Green);
	Blue = GetNewPySfColor();
	Blue->obj = (sf::Color *) &(sf::Color::Blue);
	Blue->r = sf::Color::Blue.r;
	Blue->g = sf::Color::Blue.g;
	Blue->b = sf::Color::Blue.b;
	Blue->a = sf::Color::Blue.a;
	PyDict_SetItemString(PySfColorType.tp_dict, "Blue", (PyObject *)Blue);
	Py_DECREF(Blue);
	Yellow = GetNewPySfColor();
	Yellow->obj = (sf::Color *) &(sf::Color::Yellow);
	Yellow->r = sf::Color::Yellow.r;
	Yellow->g = sf::Color::Yellow.g;
	Yellow->b = sf::Color::Yellow.b;
	Yellow->a = sf::Color::Yellow.a;
	PyDict_SetItemString(PySfColorType.tp_dict, "Yellow", (PyObject *)Yellow);
	Py_DECREF(Yellow);
	Magenta = GetNewPySfColor();
	Magenta->obj = (sf::Color *) &(sf::Color::Magenta);
	Magenta->r = sf::Color::Magenta.r;
	Magenta->g = sf::Color::Magenta.g;
	Magenta->b = sf::Color::Magenta.b;
	Magenta->a = sf::Color::Magenta.a;
	PyDict_SetItemString(PySfColorType.tp_dict, "Magenta", (PyObject *)Magenta);
	Py_DECREF(Magenta);
	Cyan = GetNewPySfColor();
	Cyan->obj = (sf::Color *) &(sf::Color::Cyan);
	Cyan->r = sf::Color::Cyan.r;
	Cyan->g = sf::Color::Cyan.g;
	Cyan->b = sf::Color::Cyan.b;
	Cyan->a = sf::Color::Cyan.a;
	PyDict_SetItemString(PySfColorType.tp_dict, "Cyan", (PyObject *)Cyan);
	Py_DECREF(Cyan);
}

