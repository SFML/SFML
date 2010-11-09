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

#include "View.hpp"

#include "offsetof.hpp"
#include "compat.hpp"


extern PyTypeObject PySfFloatRectType;


static void
PySfView_dealloc(PySfView *self)
{
	if (self->Owner)
		delete self->obj;
	free_object(self);
}

static PyObject *
PySfView_new(PyTypeObject *type, PyObject *args, PyObject *kwds)
{
	PySfView *self;

	self = (PySfView *)type->tp_alloc(type, 0);
	if (self != NULL)
	{
		self->Owner = true;
		PySfFloatRect *Rect = NULL;
		if (!PyArg_ParseTuple(args, "|O!:View.__new__", &PySfFloatRectType, &Rect))
			return NULL;

		if (Rect != NULL)
			self->obj = new sf::View( (const sf::FloatRect) *(Rect->obj));
		else
			self->obj = new sf::View();
	}

	return (PyObject *)self;
}

static PyObject *
PySfView_GetCenter(PySfView* self)
{
	sf::Vector2f Vect = self->obj->GetCenter();
	return Py_BuildValue("ff", Vect.x, Vect.y);
}

static PyObject *
PySfView_Reset(PySfView* self, PyObject *args)
{
	PySfFloatRect *Rect = (PySfFloatRect *)args;
	if (!PyObject_TypeCheck(Rect, &PySfFloatRectType))
	{
		PyErr_SetString(PyExc_TypeError, "View.Reset() Argument is not a sf.FloatRect instance");
		return NULL;
	}
	self->obj->Reset(*(Rect->obj));
	Py_RETURN_NONE;
}

static PyObject *
PySfView_Move(PySfView* self, PyObject *args)
{
	float x, y;
	if (!PyArg_ParseTuple(args, "ff:View.Move", &x, &y) )
		return NULL;
	self->obj->Move(x, y);
	Py_RETURN_NONE;
}

static PyObject *
PySfView_SetCenter(PySfView* self, PyObject *args)
{
	float x, y;
	if (!PyArg_ParseTuple(args, "ff:View.SetCenter", &x, &y) )
		return NULL;
	self->obj->SetCenter(x, y);
	Py_RETURN_NONE;
}

static PyObject *
PySfView_Zoom(PySfView* self, PyObject *args)
{
	self->obj->Zoom(PyFloat_AsDouble(args));
	Py_RETURN_NONE;
}

static PyMethodDef PySfView_methods[] = {
	{"GetCenter", (PyCFunction)PySfView_GetCenter, METH_NOARGS, "GetCenter()\nGet the center of the view."},
	{"Move", (PyCFunction)PySfView_Move, METH_VARARGS, "Move(OffsetX, OffsetY)\nMove the view.\n\
	OffsetX 	: Offset to move the view, on X axis\n\
	OffsetY 	: Offset to move the view, on Y axis"},
	{"Reset", (PyCFunction)PySfView_Reset, METH_O, "Reset(ViewRect)\nRebuild the view from a rectangle.\n	ViewRect : Rectangle defining the position and size of the view."},
	{"SetCenter", (PyCFunction)PySfView_SetCenter, METH_VARARGS, "SetCenter(X, Y)\nChange the center of the view."},
	{"Zoom", (PyCFunction)PySfView_Zoom, METH_O, "Zoom(Factor)\nResize the view rectangle to simulate a zoom / unzoom effect."},
	{NULL}  /* Sentinel */
};

PyTypeObject PySfViewType = {
	head_init
	"View",				/*tp_name*/
	sizeof(PySfView),	/*tp_basicsize*/
	0,					/*tp_itemsize*/
	(destructor)PySfView_dealloc, /*tp_dealloc*/
	0,					/*tp_print*/
	0,					/*tp_getattr*/
	0,					/*tp_setattr*/
	0,					/*tp_compare*/
	0,					/*tp_repr*/
	0,					/*tp_as_number*/
	0,					/*tp_as_sequence*/
	0,					/*tp_as_mapping*/
	0,					/*tp_hash */
	0,					/*tp_call*/
	0,					/*tp_str*/
	0,					/*tp_getattro*/
	0,					/*tp_setattro*/
	0,					/*tp_as_buffer*/
	Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE, /*tp_flags*/
	"This class defines a view (position, size, etc.) ; you can consider it as a 2D camera.", /* tp_doc */
	0,					/* tp_traverse */
	0,					/* tp_clear */
	0,					/* tp_richcompare */
	0,					/* tp_weaklistoffset */
	0,					/* tp_iter */
	0,					/* tp_iternext */
	PySfView_methods,	/* tp_methods */
	0,					/* tp_members */
	0,					/* tp_getset */
	0,					/* tp_base */
	0,					/* tp_dict */
	0,					/* tp_descr_get */
	0,					/* tp_descr_set */
	0,					/* tp_dictoffset */
	0,					/* tp_init */
	0,					/* tp_alloc */
	PySfView_new,		/* tp_new */
};

PySfView *
GetNewPySfView()
{
	return PyObject_New(PySfView, &PySfViewType);
}

