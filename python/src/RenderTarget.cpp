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

#include "RenderTarget.hpp"
#include "Color.hpp"
#include "View.hpp"

#include "compat.hpp"


extern PyTypeObject PySfColorType;
extern PyTypeObject PySfViewType;


static void
PySfRenderTarget_dealloc(PySfRenderTarget *self)
{
	delete self->obj;
	free_object(self);
}

static PyObject *
PySfRenderTarget_new(PyTypeObject *type, PyObject *args, PyObject *kwds)
{
	PySfRenderTarget *self;
	self = (PySfRenderTarget *)type->tp_alloc(type, 0);
	return (PyObject *)self;
}

static PyObject *
PySfRenderTarget_Clear(PySfRenderTarget *self, PyObject *args)
{
	PySfColor *Color;
	int size = PyTuple_Size(args);
	if (size == 1)
	{
		if (!PyArg_ParseTuple(args, "O!:RenderTarget.Clear", &PySfColorType, &Color))
			return NULL;
		PySfColorUpdate(Color);
		self->obj->Clear(*(Color->obj));
	}
	else if (size == 0)
	{
		self->obj->Clear(sf::Color::Black);
	}
	else
	{
		PyErr_SetString(PyExc_TypeError, "sf.RenderTarget.Clear takes one or zero argument");
		return NULL;
	}
	Py_RETURN_NONE;
}


static PyObject *
PySfRenderTarget_GetView(PySfRenderTarget *self)
{
	PySfView *View;

	View = GetNewPySfView();
	View->obj = new sf::View(self->obj->GetView());

	return (PyObject *)View;
}

static PyObject *
PySfRenderTarget_PreserveOpenGLStates(PySfRenderTarget *self, PyObject *args)
{
	bool Optimize = PyBool_AsBool(args);
	self->obj->PreserveOpenGLStates(Optimize);
	Py_RETURN_NONE;
}

static PyObject *
PySfRenderTarget_SetView(PySfRenderTarget* self, PyObject *args)
{
	PySfView *View = (PySfView *)args;
	if (!PyObject_TypeCheck(View, &PySfViewType))
	{
		PyErr_SetString(PyExc_TypeError, "RenderTarget.SetView() Argument is not a sf.View");
		return NULL;
	}
	self->obj->SetView( *(View->obj));
	Py_RETURN_NONE;
}

static PyObject *
PySfRenderTarget_GetDefaultView(PySfRenderTarget *self)
{
	PySfView *View;

	View = GetNewPySfView();
	View->Owner = false;
	View->obj = &(self->obj->GetDefaultView());

	return (PyObject *)View;
}

static PyMethodDef PySfRenderTarget_methods[] = {
	{"Clear", (PyCFunction)PySfRenderTarget_Clear, METH_VARARGS, "Clear(FillColor)\n\
Clear the entire target with a single color.\n\
	FillColor : Color to use to clear the render target."},
	{"GetDefaultView", (PyCFunction)PySfRenderTarget_GetDefaultView, METH_NOARGS, "GetDefaultView()\n\
Get the default view of the window for read / write (returns a sf.View instance)."},
	{"GetView", (PyCFunction)PySfRenderTarget_GetView, METH_NOARGS, "GetView()\n\
Get the current view rectangle (returns a sf.View instance)."},
	{"PreserveOpenGLStates", (PyCFunction)PySfRenderTarget_PreserveOpenGLStates, METH_O, "PreserveOpenGLStates(Preserve)\n\
Tell SFML to preserve external OpenGL states, at the expense of more CPU charge. Use this function if you don't want SFML to mess up your own OpenGL states (if any). Don't enable state preservation if not needed, as it will allow SFML to do internal optimizations and improve performances. This parameter is false by default\n\
	Preserve : True to preserve OpenGL states, false to let SFML optimize"},
	{"SetView", (PyCFunction)PySfRenderTarget_SetView, METH_O, "SetView(View)\n\
Change the current active view. View must be a sf.View instance."},
	{NULL}  /* Sentinel */
};

PyTypeObject PySfRenderTargetType = {
	head_init
	"RenderTarget",			/*tp_name*/
	sizeof(PySfRenderTarget), /*tp_basicsize*/
	0,						/*tp_itemsize*/
	(destructor)PySfRenderTarget_dealloc, /*tp_dealloc*/
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
	"Base class for all render targets (window, image, ...).", /* tp_doc */
	0,						/* tp_traverse */
	0,						/* tp_clear */
	0,						/* tp_richcompare */
	0,						/* tp_weaklistoffset */
	0,						/* tp_iter */
	0,						/* tp_iternext */
	PySfRenderTarget_methods, /* tp_methods */
	0,						/* tp_members */
	0,						/* tp_getset */
	0,						/* tp_base */
	0,						/* tp_dict */
	0,						/* tp_descr_get */
	0,						/* tp_descr_set */
	0,						/* tp_dictoffset */
	0,						/* tp_init */
	0,						/* tp_alloc */
	PySfRenderTarget_new,	/* tp_new */
};


