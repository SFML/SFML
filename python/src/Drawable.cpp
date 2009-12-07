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

#include "Drawable.hpp"
#include "Color.hpp"

#include "compat.hpp"


extern PyTypeObject PySfColorType;


void CustomDrawable::Render(sf::RenderTarget& Target, sf::RenderQueue& Queue) const
{
	if (RenderFunction)
		PyObject_CallFunction(RenderFunction, (char *)"OO", RenderWindow, Queue);
	else
	{
		PyErr_SetString(PyExc_RuntimeError, "Custom drawables must have a render method defined");
		PyErr_Print();
	}
}

static void
PySfDrawable_dealloc(PySfDrawable *self)
{
	delete self->obj;
	free_object(self);
}

static PyObject *
PySfDrawable_new(PyTypeObject *type, PyObject *args, PyObject *kwds)
{
	PySfDrawable *self;
	self = (PySfDrawable *)type->tp_alloc(type, 0);
	if (self != NULL)
	{
		self->IsCustom = true;
		self->obj = new CustomDrawable();
		if (PyObject_HasAttrString((PyObject *)self, "Render"))
			self->obj->RenderFunction = PyObject_GetAttrString((PyObject *)self, "Render");
		else
			self->obj->RenderFunction = NULL;
		self->obj->RenderWindow = NULL;
	}
	return (PyObject *)self;
}

static PyObject *
PySfDrawable_SetX(PySfDrawable* self, PyObject *args)
{
	self->obj->SetX(PyFloat_AsDouble(args));
	Py_RETURN_NONE;
}
static PyObject *
PySfDrawable_SetY(PySfDrawable* self, PyObject *args)
{
	self->obj->SetY(PyFloat_AsDouble(args));
	Py_RETURN_NONE;
}
static PyObject *
PySfDrawable_SetScale(PySfDrawable* self, PyObject *args)
{
	float ScaleX, ScaleY;
	if (!PyArg_ParseTuple(args, "ff:Drawable.SetScale", &ScaleX, &ScaleY) )
		return NULL;
	self->obj->SetScale(ScaleX, ScaleY);
	Py_RETURN_NONE;
}
static PyObject *
PySfDrawable_SetScaleX(PySfDrawable* self, PyObject *args)
{
	self->obj->SetScaleX(PyFloat_AsDouble(args));
	Py_RETURN_NONE;
}
static PyObject *
PySfDrawable_SetScaleY(PySfDrawable* self, PyObject *args)
{
	self->obj->SetScaleY(PyFloat_AsDouble(args));
	Py_RETURN_NONE;
}

static PyObject *
PySfDrawable_SetRotation(PySfDrawable* self, PyObject *args)
{
	self->obj->SetRotation(PyFloat_AsDouble(args));
	Py_RETURN_NONE;
}
static PyObject *
PySfDrawable_SetOrigin(PySfDrawable* self, PyObject *args)
{
	float x, y;
	if (!PyArg_ParseTuple(args, "ff:Drawable.SetOrigin", &x, &y) )
		return NULL;
	self->obj->SetOrigin(x, y);
	Py_RETURN_NONE;
}
static PyObject *
PySfDrawable_GetOrigin(PySfDrawable* self)
{
	sf::Vector2f Vect = self->obj->GetOrigin();
	return Py_BuildValue("ff", Vect.x, Vect.y);
}

static PyObject *
PySfDrawable_SetColor(PySfDrawable* self, PyObject *args)
{
	PySfColor *Color = (PySfColor *)args;
	if (!PyObject_TypeCheck(args, &PySfColorType))
	{
		PyErr_SetString(PyExc_TypeError, "Drawable.SetColor() Argument is not a sf.Color");
		return NULL;
	}
	PySfColorUpdate(Color);
	self->obj->SetColor(*(Color->obj));
	Py_RETURN_NONE;
}
static PyObject *
PySfDrawable_GetPosition(PySfDrawable* self)
{
	sf::Vector2f Vect = self->obj->GetPosition();
	return Py_BuildValue("ff", Vect.x, Vect.y);
}
static PyObject *
PySfDrawable_GetScale(PySfDrawable* self)
{
	sf::Vector2f Vect = self->obj->GetScale();
	return Py_BuildValue("ff", Vect.x, Vect.y);
}
static PyObject *
PySfDrawable_GetRotation(PySfDrawable* self)
{
	return PyFloat_FromDouble(self->obj->GetRotation());
}

static PyObject *
PySfDrawable_GetColor(PySfDrawable* self)
{
	PySfColor *Color;

	Color = GetNewPySfColor();
	Color->obj = new sf::Color( self->obj->GetColor() );
	Color->r = Color->obj->r;
	Color->g = Color->obj->g;
	Color->b = Color->obj->b;
	Color->a = Color->obj->a;

	return (PyObject *)Color;
}
static PyObject *
PySfDrawable_Move(PySfDrawable* self, PyObject *args)
{
	float x, y;
	if (!PyArg_ParseTuple(args, "ff:Drawable.Move", &x, &y) )
		return NULL;
	self->obj->Move(x, y);
	Py_RETURN_NONE;
}
static PyObject *
PySfDrawable_Rotate(PySfDrawable* self, PyObject *args)
{
	self->obj->Rotate(PyFloat_AsDouble(args));
	Py_RETURN_NONE;
}
static PyObject *
PySfDrawable_Scale(PySfDrawable* self, PyObject *args)
{
	float FactorX, FactorY;
	if (!PyArg_ParseTuple(args, "ff:Drawable.Scale", &FactorX, &FactorY) )
		return NULL;
	self->obj->Scale(FactorX, FactorY);
	Py_RETURN_NONE;
}

static PyObject *
PySfDrawable_SetBlendMode(PySfDrawable* self, PyObject *args)
{
	self->obj->SetBlendMode((sf::Blend::Mode)PyLong_AsUnsignedLong(args));
	Py_RETURN_NONE;
}

static PyObject *
PySfDrawable_SetPosition(PySfDrawable* self, PyObject *args)
{
	float Left, Top;
	if (!PyArg_ParseTuple(args, "ff:Drawable.SetPosition", &Left, &Top) )
		return NULL;
	self->obj->SetPosition(Left, Top);
	Py_RETURN_NONE;
}

static PyObject *
PySfDrawable_TransformToLocal(PySfDrawable* self, PyObject *args)
{
	float X, Y;
	if (!PyArg_ParseTuple(args, "ff:Drawable.TransformToLocal", &X, &Y) )
		return NULL;
	sf::Vector2f result = self->obj->TransformToLocal(sf::Vector2f(X, Y));
	return Py_BuildValue("ff", result.x, result.y);
}

static PyObject *
PySfDrawable_TransformToGlobal(PySfDrawable* self, PyObject *args)
{
	float X, Y;
	if (!PyArg_ParseTuple(args, "ff:Drawable.TransformToGlobal", &X, &Y) )
		return NULL;
	sf::Vector2f result = self->obj->TransformToGlobal(sf::Vector2f(X, Y));
	return Py_BuildValue("ff", result.x, result.y);
}

int PySfDrawable_setattro(PyObject* self, PyObject *attr_name, PyObject *v)
{
#ifdef IS_PY3K
	PyObject *string = PyUnicode_AsUTF8String(attr_name);
	if (string == NULL) return NULL;
	std::string Name(PyBytes_AsString(string));
#else
	std::string Name(PyString_AsString(attr_name));
#endif
	if (Name == "Render")
	{
		Py_CLEAR(((PySfDrawable*)self)->obj->RenderFunction);
		Py_INCREF(v);
		((PySfDrawable*)self)->obj->RenderFunction = v;
	}
#ifdef IS_PY3K
	Py_DECREF(string);
#endif
	return PyObject_GenericSetAttr(self, attr_name, v);
}

static PyMethodDef PySfDrawable_methods[] = {
	{"TransformToLocal", (PyCFunction)PySfDrawable_TransformToLocal, METH_VARARGS, "TransformToLocal(X, Y)\n\
Transform a point from global coordinates into local coordinates (ie it applies the inverse of object's origin, translation, rotation and scale to the point). Returns a tuple.\n\
	X : X coordinate of the point to transform\n\
	Y : Y coordinate of the point to transform"},
	{"TransformToGlobal", (PyCFunction)PySfDrawable_TransformToGlobal, METH_VARARGS, "TransformToGlobal(X, Y)\n\
Transform a point from local coordinates into global coordinates (ie it applies the object's origin, translation, rotation and scale to the point). Returns a tuple.\n\
	X : X coordinate of the point to transform\n\
	Y : Y coordinate of the point to transform"},
	{"SetX", (PyCFunction)PySfDrawable_SetX, METH_O, "SetX(X)\nSet the X position of the object.\n	X : New X coordinate"},
	{"SetY", (PyCFunction)PySfDrawable_SetY, METH_O, "SetY(Y)\nSet the Y position of the object.\n	Y : New Y coordinate"},
	{"SetScale", (PyCFunction)PySfDrawable_SetScale, METH_VARARGS, "SetScale(ScaleX, ScaleY)\nSet the scale of the object.\n	ScaleX 	: New horizontal scale (must be strictly positive)\n	ScaleY 	: New vertical scale (must be strictly positive)"},
	{"SetScaleX", (PyCFunction)PySfDrawable_SetScaleX, METH_O, "SetScaleX(ScaleX)\nSet the X scale factor of the object.\n	ScaleX 	: New horizontal scale (must be strictly positive)"},
	{"SetScaleY", (PyCFunction)PySfDrawable_SetScaleY, METH_O, "SetScaleY(ScaleY)\nSet the Y scale factor of the object.\n	ScaleY 	: New vertical scale (must be strictly positive)"},
	{"SetRotation", (PyCFunction)PySfDrawable_SetRotation, METH_O, "SetRotation(Rotation)\nSet the orientation of the object.\n	Rotation : Angle of rotation, in degrees"},
	{"SetOrigin", (PyCFunction)PySfDrawable_SetOrigin, METH_VARARGS, "SetOrigin(OriginX, OriginY)\nSet the origin of the object, in coordinates relative to the object.\n	OriginX : X coordinate of the origin\n	OriginY : Y coordinate of the origin"},
	{"GetOrigin", (PyCFunction)PySfDrawable_GetOrigin, METH_NOARGS, "GetOrigin()\nGet the origin of the object, in coordinates relative to the object."},
	{"SetColor", (PyCFunction)PySfDrawable_SetColor, METH_O, "SetColor(Color)\nSet the color of the object.\n	Color : New color"},
	{"GetPosition", (PyCFunction)PySfDrawable_GetPosition, METH_NOARGS, "GetPosition()\nGet the position of the object."},
	{"GetScale", (PyCFunction)PySfDrawable_GetScale, METH_NOARGS, "GetScale()\nGet the scale of the object."},
	{"GetRotation", (PyCFunction)PySfDrawable_GetRotation, METH_NOARGS, "GetRotation()\nGet the orientation of the object."},
	{"GetColor", (PyCFunction)PySfDrawable_GetColor, METH_NOARGS, "GetColor()\nGet the color of the object."},
	{"Move", (PyCFunction)PySfDrawable_Move, METH_VARARGS, "Move(OffsetX, OffsetY)\nMove the object.\n	OffsetX : X offset\n	OffsetY : Y offset "},
	{"Scale", (PyCFunction)PySfDrawable_Scale, METH_VARARGS, "Scale(FactorX, FactorY)\nScale the object.\n	FactorX : Scaling factor on X (must be strictly positive)\n	FactorY : Scaling factor on Y (must be strictly positive)"},
	{"Rotate", (PyCFunction)PySfDrawable_Rotate, METH_O, "Rotate(Angle)\nRotate the object.\n	Angle : Angle of rotation, in degrees"},
	{"SetBlendMode", (PyCFunction)PySfDrawable_SetBlendMode, METH_O, "SetBlendMode(Mode)\nSet the blending mode for the object. The default blend mode is sf.Blend.Alpha\n	Mode : New blending mode"},
	{"SetPosition", (PyCFunction)PySfDrawable_SetPosition, METH_VARARGS, "SetPosition(X, Y)\nSet the position of the object.\n	X : New X coordinate\n	Y : New Y coordinate"},
	{NULL}  /* Sentinel */
};

PyTypeObject PySfDrawableType = {
	head_init
	"Drawable",				/*tp_name*/
	sizeof(PySfDrawable),	/*tp_basicsize*/
	0,						/*tp_itemsize*/
	(destructor)PySfDrawable_dealloc, /*tp_dealloc*/
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
	PySfDrawable_setattro,	/*tp_setattro*/
	0,						/*tp_as_buffer*/
	Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE, /*tp_flags*/
	"Abstract base class for every object that can be drawn into a render window.", /* tp_doc */
	0,						/* tp_traverse */
	0,						/* tp_clear */
	0,						/* tp_richcompare */
	0,						/* tp_weaklistoffset */
	0,						/* tp_iter */
	0,						/* tp_iternext */
	PySfDrawable_methods,	/* tp_methods */
	0,						/* tp_members */
	0,						/* tp_getset */
	0,						/* tp_base */
	0,						/* tp_dict */
	0,						/* tp_descr_get */
	0,						/* tp_descr_set */
	0,						/* tp_dictoffset */
	0,						/* tp_init */
	0,						/* tp_alloc */
	PySfDrawable_new,		/* tp_new */
};


