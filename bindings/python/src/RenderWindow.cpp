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
#include "Image.hpp"
#include "Window.hpp"
#include "Color.hpp"
#include "Drawable.hpp"

#include "compat.hpp"

#include <SFML/Window/WindowStyle.hpp>


extern PyTypeObject PySfViewType;
extern PyTypeObject PySfWindowType;
extern PyTypeObject PySfRenderWindowType;
extern PyTypeObject PySfColorType;
extern PyTypeObject PySfDrawableType;


static void
PySfRenderWindow_dealloc(PySfRenderWindow* self)
{
	Py_CLEAR(self->View);
	delete self->obj;
	free_object(self);
}

static PyObject *
PySfRenderWindow_new(PyTypeObject *type, PyObject *args, PyObject *kwds)
{
	PySfRenderWindow *self;
	self = (PySfRenderWindow *)type->tp_alloc(type, 0);
	if (self != NULL)
	{
		self->obj = new sf::RenderWindow();
		self->View = NULL;
	}
	return (PyObject *)self;
}

static PyObject *
PySfRenderWindow_ConvertCoords(PySfRenderWindow *self, PyObject *args)
{
	unsigned int WindowX, WindowY;
	PySfView *PyTargetView = NULL;
	sf::Vector2f Vect;

	if (!PyArg_ParseTuple(args, "II|O!:RenderWindow.ConvertCoords", &WindowX, &WindowY, &PySfViewType, &PyTargetView))
		return NULL;

	if (PyTargetView)
	{
		Vect = self->obj->ConvertCoords(WindowX, WindowY, *PyTargetView->obj);
	}
	else
	{
		Vect = self->obj->ConvertCoords(WindowX, WindowY);
	}

	return Py_BuildValue("ff", Vect.x, Vect.y);
}

static bool
PySfRenderWindow_DrawObject(PySfRenderWindow *RenderWindow, PySfDrawable *Obj)
{
	if (PyObject_TypeCheck((PyObject *)Obj, &PySfDrawableType))
	{
		if (Obj->IsCustom)
		{
			Py_CLEAR(Obj->obj->RenderWindow);
			Py_INCREF(RenderWindow);
			Obj->obj->RenderWindow = RenderWindow;
		}
		RenderWindow->obj->Draw(*(Obj->obj));
		return true;
	}
	return false;
}

static PyObject *
PySfRenderWindow_Draw(PySfRenderWindow *self, PyObject *args)
{
	if (args == NULL)
		return NULL;
	if (!PySfRenderWindow_DrawObject(self, (PySfDrawable *)args))
	{
		PyObject *iterator = PyObject_GetIter(args);
		PyObject *item;
		PyErr_Clear();
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
	}
	if (PyErr_Occurred())
		return NULL;
	Py_RETURN_NONE;
}


static PyObject *
PySfRenderWindow_Clear(PySfRenderWindow *self, PyObject *args)
{
	PySfColor *Color = NULL;
	if (!PyArg_ParseTuple(args, "|O!:RenderWindow.Clear", &PySfColorType, &Color))
		return NULL;
	if (Color == NULL) self->obj->Clear(sf::Color::Black);
	else
	{
		PySfColorUpdate(Color);
		self->obj->Clear(*(Color->obj));
	}
	Py_RETURN_NONE;
}

static PyObject *
PySfRenderWindow_SetActive(PySfRenderWindow *self, PyObject *args)
{
	PyObject*  Active( 0 );

	PyArg_ParseTuple( args, "|O", &Active );
	self->obj->SetActive( Active == 0 ? true : PyBool_AsBool( Active ) );

	Py_RETURN_NONE;
}

static PyObject *
PySfRenderWindow_GetView(PySfRenderWindow *self)
{
	if (self->View != NULL)
	{
		Py_INCREF(self->View);
		return (PyObject *)(self->View);
	}
	else
	{
		PySfView *View;

		View = GetNewPySfView();
		View->Owner = false;
		View->obj = (sf::View *)&(self->obj->GetView());
		Py_INCREF(View);
		self->View = View;
		return (PyObject *)View;
	}
}

static PyObject *
PySfRenderWindow_SetView(PySfRenderWindow* self, PyObject *args)
{
	PySfView *View = (PySfView *)args;
	if (!PyObject_TypeCheck(View, &PySfViewType))
	{
		PyErr_SetString(PyExc_TypeError, "RenderWindow.SetView() Argument is not a sf.View");
		return NULL;
	}
	Py_CLEAR(self->View);
	Py_INCREF(View);
	self->View = View;
	self->obj->SetView(*(View->obj));
	Py_RETURN_NONE;
}

static PyObject *
PySfRenderWindow_GetDefaultView(PySfRenderWindow *self)
{
	PySfView *View;

	View = GetNewPySfView();
	View->Owner = false;

	// Python doesn't know anything about 'const', so cast away. Be careful with
	// not changing the default view!
	View->obj = const_cast<sf::View*>( &( self->obj->GetDefaultView() ) );

	return (PyObject *)View;
}

static PyMethodDef PySfRenderWindow_methods[] = {
	{"SetActive", (PyCFunction)PySfRenderWindow_SetActive, METH_VARARGS, "SetActive(Active)\n\
Activate or deactivate the window as the current target for OpenGL rendering.\n\
	Active : True to activate window. (default: True)"},
	{"Clear", (PyCFunction)PySfRenderWindow_Clear, METH_VARARGS, "Clear(FillColor)\n\
Clear the entire target with a single color.\n\
	FillColor : Color to use to clear the render target."},
	{"GetDefaultView", (PyCFunction)PySfRenderWindow_GetDefaultView, METH_NOARGS, "GetDefaultView()\n\
Get the default view of the window for read / write (returns a sf.View instance)."},
	{"GetView", (PyCFunction)PySfRenderWindow_GetView, METH_NOARGS, "GetView()\n\
Get the current view rectangle (returns a sf.View instance)."},
	{"SetView", (PyCFunction)PySfRenderWindow_SetView, METH_O, "SetView(View)\n\
Change the current active view. View must be a sf.View instance."},
	{"Draw", (PyCFunction)PySfRenderWindow_Draw, METH_O, "Draw(Drawable)\n\
Draw something on the window. The argument can be a drawable or any object supporting the iterator protocol and containing drawables (for example a tuple of drawables)."},
	{"ConvertCoords", (PyCFunction)PySfRenderWindow_ConvertCoords, METH_VARARGS, "ConvertCoords(WindowX, WindowY, TargetView)\n\
Convert a point in window coordinates into view coordinates. Returns a tuple of two floats.\n\
	WindowX :    X coordinate of the point to convert, relative to the window\n\
	WindowY :    Y coordinate of the point to convert, relative to the window\n\
	TargetView : Target view to convert the point to (NULL by default -- uses the current view)."},
	{NULL}  /* Sentinel */
};

PyTypeObject PySfRenderWindowType = {
	head_init
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
	"Simple wrapper for sf.Window that allows easy 2D rendering.\n\
Default constructor : sf.RenderWindow()\n\
Construct a new window : sf.RenderWindow(Mode, Title, Style::Resize|Style::Close, Params = WindowSettings())\n\
	Mode 	: Video mode to use\n\
	Title 	: Title of the window\n\
	WindowStyle 	: Window style (Resize | Close by default)\n\
	Params 	: Creation parameters (see default constructor for default values)\n\
Construct the window from an existing control : sf.RenderWindow(Handle, Params)\n\
	Handle 	: handle of the control (long integer)\n\
	Params 	: Creation parameters (see default constructor for default values)", /* tp_doc */
	0,						/* tp_traverse */
	0,						/* tp_clear */
	0,						/* tp_richcompare */
	0,						/* tp_weaklistoffset */
	0,						/* tp_iter */
	0,						/* tp_iternext */
	PySfRenderWindow_methods, /* tp_methods */
	0,						/* tp_members */
	0,						/* tp_getset */
	&PySfWindowType,		/* tp_base */
	0,						/* tp_dict */
	0,						/* tp_descr_get */
	0,						/* tp_descr_set */
	0,						/* tp_dictoffset */
	0,						/* tp_init */
	0,						/* tp_alloc */
	PySfRenderWindow_new,	/* tp_new */
};


