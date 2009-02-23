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

#include "RenderWindow.hpp"

#include "Event.hpp"
#include "VideoMode.hpp"
#include "Drawable.hpp"
#include "Color.hpp"
#include "Rect.hpp"
#include "View.hpp"
#include "Image.hpp"
#include "Window.hpp"

#include "SFML/Window/WindowStyle.hpp"


extern PyTypeObject PySfEventType;
extern PyTypeObject PySfViewType;
extern PyTypeObject PySfColorType;
extern PyTypeObject PySfWindowType;
extern PyTypeObject PySfWindowSettingsType;
extern PyTypeObject PySfVideoModeType;
extern PyTypeObject PySfDrawableType;

extern PyTypeObject PySfRenderTargetType;

static PyMemberDef PySfRenderWindow_members[] = {
	{NULL}  /* Sentinel */
};


static void
PySfRenderWindow_dealloc(PySfRenderWindow* self)
{
	delete self->obj;
	self->ob_type->tp_free((PyObject*)self);
}

static PyObject *
PySfRenderWindow_new(PyTypeObject *type, PyObject *args, PyObject *kwds)
{
	PySfRenderWindow *self;

	self = (PySfRenderWindow *)type->tp_alloc(type, 0);
	if (self != NULL)
	{
	}

	return (PyObject *)self;
}

static int
PySfRenderWindow_init(PySfRenderWindow *self, PyObject *args, PyObject *kwds)
{
	self->obj = new sf::RenderWindow();
	if (PyTuple_Size(args) > 0)
		PySfWindow_Create((PySfWindow *)self, args, kwds);
	return 0;
}

static PyObject *
PySfRenderWindow_Capture(PySfRenderWindow *self)
{
	PySfImage *Image;

	Image = GetNewPySfImage();
	Image->obj = new sf::Image(self->obj->Capture());
	return (PyObject *)Image;
}

static PyObject *
PySfRenderWindow_ConvertCoords(PySfRenderWindow *self, PyObject *args)
{
	unsigned int WindowX, WindowY;
	PySfView *PyTargetView = NULL;
	sf::View *TargetView = NULL;
	sf::Vector2f Vect;

	if (! PyArg_ParseTuple(args, "II|O!", &WindowX, &WindowY, &PySfViewType, &PyTargetView))
		return NULL;

	if (PyTargetView)
		TargetView = PyTargetView->obj;

	Vect = self->obj->ConvertCoords(WindowX, WindowY, TargetView);
	return Py_BuildValue("ff", Vect.x, Vect.y);
}

static bool
PySfRenderWindow_DrawObject(PySfRenderWindow *RenderWindow, PySfDrawable *Obj)
{
	if (PyObject_TypeCheck((PyObject *)Obj, &PySfDrawableType))
	{
		if (PyObject_HasAttrString((PyObject *)Obj, "Render"))
		{
			Obj->obj->RenderWindow = RenderWindow;
			Obj->obj->RenderFunction = PyObject_GetAttrString((PyObject *)Obj, "Render");
		}
		RenderWindow->obj->Draw( *(Obj->obj) );
		return true;
	}
	return false;
}

static PyObject *
PySfRenderWindow_Draw(PySfRenderWindow *self, PyObject *args)
{
	if (!args)
		return NULL;


	if (!PySfRenderWindow_DrawObject(self, (PySfDrawable *)args))
	{
		PyObject *iterator = PyObject_GetIter(args);
		PyObject *item;

		if (iterator == NULL)
		{
			PyErr_SetString(PyExc_TypeError, "Argument to Draw method is neither a Drawable nor an iterable.");
			return NULL;
		}

		while ((item = PyIter_Next(iterator)))
		{
			if (!PySfRenderWindow_DrawObject(self, (PySfDrawable *)item))
			{
				PyErr_SetString(PyExc_TypeError, "Object in iterable not a Drawable.");
				return NULL;
			}
			Py_DECREF(item);
		}

		Py_DECREF(iterator);

		if (PyErr_Occurred()) {
			return NULL;
		}
	}
	Py_RETURN_NONE;
}

static PyMethodDef PySfRenderWindow_methods[] = {
	{"Capture", (PyCFunction)PySfRenderWindow_Capture, METH_NOARGS, "Capture()\n\
Save the content of the window to an image. Returns a sf.Image object."},
	{"ConvertCoords", (PyCFunction)PySfRenderWindow_ConvertCoords, METH_VARARGS, "ConvertCoords(WindowX, WindowY, TargetView)\n\
Convert a point in window coordinates into view coordinates. Returns a tuple of two floats.\n\
	WindowX :    X coordinate of the point to convert, relative to the window\n\
	WindowY :    Y coordinate of the point to convert, relative to the window\n\
	TargetView : Target view to convert the point to (NULL by default -- uses the current view)."},
	{"Draw", (PyCFunction)PySfRenderWindow_Draw, METH_O, "Draw(Drawable)\n\
Draw something on the window. The argument can be a drawable or any object supporting the iterator protocol and containing drawables (for example a tuple of drawables)."},
	{NULL}  /* Sentinel */
};

PyTypeObject PySfRenderWindowType = {
	PyObject_HEAD_INIT(NULL)
	0,						/*ob_size*/
	"RenderWindow",			/*tp_name*/
	sizeof(PySfRenderWindow), /*tp_basicsize*/
	0,						/*tp_itemsize*/
	(destructor)PySfRenderWindow_dealloc, /*tp_dealloc*/
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
	"Simple wrapper for sf.Window that allows easy 2D rendering.", /* tp_doc */
	0,						/* tp_traverse */
	0,						/* tp_clear */
	0,						/* tp_richcompare */
	0,						/* tp_weaklistoffset */
	0,						/* tp_iter */
	0,						/* tp_iternext */
	PySfRenderWindow_methods, /* tp_methods */
	PySfRenderWindow_members, /* tp_members */
	0,						/* tp_getset */
	0,						/* tp_base */
	0,						/* tp_dict */
	0,						/* tp_descr_get */
	0,						/* tp_descr_set */
	0,						/* tp_dictoffset */
	(initproc)PySfRenderWindow_init, /* tp_init */
	0,						/* tp_alloc */
	PySfRenderWindow_new,	/* tp_new */
	0,						/* tp_free */
	0,						/* tp_is_gc */
	Py_BuildValue("(OO)", &PySfWindowType, &PySfRenderTargetType) /* tp_bases */
};


