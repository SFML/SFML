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

#include "Shape.hpp"

#include <SFML/Graphics/Color.hpp>
#include "Color.hpp"

#include "compat.hpp"

extern PyTypeObject PySfColorType;
extern PyTypeObject PySfDrawableType;


static void
PySfShape_dealloc(PySfShape* self)
{
	delete self->obj;
	free_object(self);
}

static PyObject *
PySfShape_new(PyTypeObject *type, PyObject *args, PyObject *kwds)
{
	PySfShape *self;
	self = (PySfShape *)type->tp_alloc(type, 0);
	if (self != NULL)
	{
		self->obj = new sf::Shape();
		self->IsCustom = false;
	}
	return (PyObject *)self;
}

// void AddPoint(float X, float Y, const Color& Col = Color(255, 255, 255), const Color& OutlineCol = Color(0, 0, 0));
static PyObject *
PySfShape_AddPoint(PySfShape* self, PyObject *args, PyObject *kwds)
{
	const char *kwlist[] = {"X", "Y", "Col", "OutlineCol", NULL};
	float X, Y;
	sf::Color *Col, *OutlineCol;
	PySfColor *ColTmp=NULL, *OutlineColTmp=NULL;
	if (!PyArg_ParseTupleAndKeywords(args, kwds, "ff|O!O!:Shape.AddPoint", (char **)kwlist, &X, &Y, &PySfColorType, &ColTmp, &PySfColorType, &OutlineColTmp))
		return NULL;

	if (ColTmp)
	{
		PySfColorUpdate(ColTmp);
		Col = ColTmp->obj;
	}
	else
		Col = (sf::Color *)&sf::Color::White;

	if (OutlineColTmp)
	{
		PySfColorUpdate(OutlineColTmp);
		OutlineCol = OutlineColTmp->obj;
	}
	else
		OutlineCol = (sf::Color *)&sf::Color::Black;

	self->obj->AddPoint(X, Y, *Col, *OutlineCol);

	Py_RETURN_NONE;
}

static PyObject *
PySfShape_SetOutlineWidth(PySfShape* self, PyObject *args)
{
	self->obj->SetOutlineWidth(PyFloat_AsDouble(args));
	Py_RETURN_NONE;
}

static PyObject *
PySfShape_GetOutlineWidth(PySfShape* self)
{
	return PyFloat_FromDouble(self->obj->GetOutlineWidth());
}

// static Shape Line(float X0, float Y0, float X1, float Y1, float Thickness, const Color& Col, float Outline = 0.f, const Color& OutlineCol = sf::Color(0, 0, 0));
static PyObject *
PySfShape_Line(PySfShape* self, PyObject *args, PyObject *kwds)
{
	const char *kwlist[] = {"X0", "Y0", "X1", "Y1", "Thickness", "Col", "Outline", "OutlineCol", NULL};
	PySfShape *Line = GetNewPySfShape();
	float X0, Y0, X1, Y1, Thickness, Outline = 0.f;
	sf::Color *OutlineCol;
	PySfColor *ColTmp, *OutlineColTmp=NULL;
	if (!PyArg_ParseTupleAndKeywords(args, kwds, "fffffO!|fO!:Shape.Line", (char **)kwlist, &X0, &Y0, &X1, &Y1, &Thickness, &PySfColorType, &ColTmp, &Outline, &PySfColorType, &OutlineColTmp))
		return NULL;
	if (OutlineColTmp)
	{
		PySfColorUpdate(OutlineColTmp);
		OutlineCol = OutlineColTmp->obj;
	}
	else
		OutlineCol = (sf::Color *)&sf::Color::Black;

	PySfColorUpdate(ColTmp);
	Line->obj = new sf::Shape(sf::Shape::Line(X0, Y0, X1, Y1, Thickness, *(ColTmp->obj), Outline, *OutlineCol));
	return (PyObject *)Line;
}

// static Shape Rectangle(float X0, float Y0, float X1, float Y1, const Color& Col, float Outline = 0.f, const Color& OutlineCol = sf::Color(0, 0, 0));
static PyObject *
PySfShape_Rectangle(PySfShape* self, PyObject *args, PyObject *kwds)
{
	const char *kwlist[] = {"X0", "Y0", "X1", "Y1", "Col", "Outline", "OutlineCol", NULL};
	PySfShape *Rectangle = GetNewPySfShape();
	float X0, Y0, X1, Y1, Outline = 0.f;
	sf::Color *OutlineCol;
	PySfColor *ColTmp, *OutlineColTmp=NULL;
	if (!PyArg_ParseTupleAndKeywords(args, kwds, "ffffO!|fO!:Shape.Rectangle", (char **)kwlist, &X0, &Y0, &X1, &Y1, &PySfColorType, &ColTmp, &Outline, &PySfColorType, &OutlineColTmp))
		return NULL;
	if (OutlineColTmp)
	{
		PySfColorUpdate(OutlineColTmp);
		OutlineCol = OutlineColTmp->obj;
	}
	else
		OutlineCol = (sf::Color *)&sf::Color::Black;

	PySfColorUpdate(ColTmp);
	Rectangle->obj = new sf::Shape(sf::Shape::Rectangle(X0, Y0, X1, Y1, *(ColTmp->obj), Outline, *OutlineCol));
	return (PyObject *)Rectangle;
}

// static Shape Circle(float X, float Y, float Radius, const Color& Col, float Outline = 0.f, const Color& OutlineCol = sf::Color(0, 0, 0));
static PyObject *
PySfShape_Circle(PySfShape* self, PyObject *args, PyObject *kwds)
{
	const char *kwlist[] = {"X", "Y", "Radius", "Col", "Outline", "OutlineCol", NULL};
	PySfShape *Circle = GetNewPySfShape();
	float X, Y, Radius, Outline = 0.f;
	sf::Color *OutlineCol;
	PySfColor *ColTmp, *OutlineColTmp=NULL;
	if (!PyArg_ParseTupleAndKeywords(args, kwds, "fffO!|fO!:Shape.Circle", (char **)kwlist, &X, &Y, &Radius, &PySfColorType, &ColTmp, &Outline, &PySfColorType, &OutlineColTmp))
		return NULL;
	if (OutlineColTmp)
	{
		PySfColorUpdate(OutlineColTmp);
		OutlineCol = OutlineColTmp->obj;
	}
	else
		OutlineCol = (sf::Color *)&sf::Color::Black;

	PySfColorUpdate(ColTmp);
	Circle->obj = new sf::Shape(sf::Shape::Circle(X, Y, Radius, *(ColTmp->obj), Outline, *OutlineCol));
	return (PyObject *)Circle;
}

static PyObject *
PySfShape_GetPointPosition(PySfShape* self, PyObject *args)
{
	sf::Vector2f result = self->obj->GetPointPosition(PyLong_AsUnsignedLong(args));
	return Py_BuildValue("ff", result.x, result.y);
}

static PyObject *
PySfShape_GetPointColor(PySfShape* self, PyObject *args)
{
	PySfColor *PyColor = GetNewPySfColor();
	PyColor->obj = new sf::Color(self->obj->GetPointColor(PyLong_AsUnsignedLong(args)));
	PyColor->r = PyColor->obj->r;
	PyColor->g = PyColor->obj->g;
	PyColor->b = PyColor->obj->b;
	PyColor->a = PyColor->obj->a;
	return (PyObject *)PyColor;
}

static PyObject *
PySfShape_GetPointOutlineColor(PySfShape* self, PyObject *args)
{
	PySfColor *PyColor = GetNewPySfColor();
	PyColor->obj = new sf::Color(self->obj->GetPointOutlineColor(PyLong_AsUnsignedLong(args)));
	PyColor->r = PyColor->obj->r;
	PyColor->g = PyColor->obj->g;
	PyColor->b = PyColor->obj->b;
	PyColor->a = PyColor->obj->a;
	return (PyObject *)PyColor;
}

static PyObject *
PySfShape_SetPointPosition(PySfShape* self, PyObject *args)
{
	unsigned int Index;
	float X, Y;
	if (!PyArg_ParseTuple(args, "Iff:Shape.SetPointPosition", &Index, &X, &Y))
		return NULL;
	self->obj->SetPointPosition(Index, X, Y);
	Py_RETURN_NONE;
}

static PyObject *
PySfShape_SetPointColor(PySfShape* self, PyObject *args)
{
	unsigned int Index;
	PySfColor *Color;
	if (!PyArg_ParseTuple(args, "IO!:Shape.SetPointColor", &Index, &PySfColorType, &Color))
		return NULL;
	PySfColorUpdate(Color);
	self->obj->SetPointColor(Index, *(Color->obj));
	Py_RETURN_NONE;
}

static PyObject *
PySfShape_SetPointOutlineColor(PySfShape* self, PyObject *args)
{
	unsigned int Index;
	PySfColor *Color;
	if (!PyArg_ParseTuple(args, "IO!:Shape.SetPointOutlineColor", &Index, &PySfColorType, &Color))
		return NULL;
	PySfColorUpdate(Color);
	self->obj->SetPointOutlineColor(Index, *(Color->obj));
	Py_RETURN_NONE;
}

static PyObject *
PySfShape_GetNbPoints(PySfShape* self, PyObject *args)
{
	return PyLong_FromUnsignedLong(self->obj->GetNbPoints());
}

static PyObject *
PySfShape_EnableFill(PySfShape* self, PyObject *args)
{
	self->obj->EnableFill(PyBool_AsBool(args));
	Py_RETURN_NONE;
}

static PyObject *
PySfShape_EnableOutline(PySfShape* self, PyObject *args)
{
	self->obj->EnableOutline(PyBool_AsBool(args));
	Py_RETURN_NONE;
}

static PyMethodDef PySfShape_methods[] = {
	{"GetPointPosition", (PyCFunction)PySfShape_GetPointPosition, METH_O, "GetPointPosition(Index)\n\
Get the position of a point.\n\
	Index : Index-th point."},
	{"GetPointColor", (PyCFunction)PySfShape_GetPointColor, METH_O, "GetPointColor(Index)\n\
Get the color of a point.\n	Index : Index-th point."},
	{"GetPointOutlineColor", (PyCFunction)PySfShape_GetPointOutlineColor, METH_O, "GetPointOutlineColor(Index)\n\
Get the outline color of a point.\n	Index : Index-th point."},
	{"SetPointPosition", (PyCFunction)PySfShape_SetPointPosition, METH_VARARGS, "SetPointPosition(Index, X, Y).\n\
Set the position of a point\n\
	Index : Index of the point, in range [0, GetNbPoints() - 1]\n\
	X :     New X coordinate of the Index-th point\n\
	Y :     New Y coordinate of the Index-th point."},
	{"SetPointColor", (PyCFunction)PySfShape_SetPointColor, METH_VARARGS, "SetPointColor(Index, Color).\n\
Set the color of a point\n\
	Index : Index of the point, in range [0, GetNbPoints() - 1]\n\
	Col :   New color of the Index-th point."},
	{"SetPointOutlineColor", (PyCFunction)PySfShape_SetPointOutlineColor, METH_VARARGS, "SetPointOutlineColor(Index, Color).\n\
Set the outline color of a point\n\
	Index : Index of the point, in range [0, GetNbPoints() - 1]\n\
	Col :   New color of the Index-th point."},
	{"GetNbPoints", (PyCFunction)PySfShape_GetNbPoints, METH_NOARGS, "GetNbPoints()\n\
Get the number of points composing the shape."},
	{"EnableFill", (PyCFunction)PySfShape_EnableFill, METH_O, "EnableFill(Enable)\n\
Enable or disable filling the shape. Fill is enabled by default.\n\
	Enable : True to enable, false to disable."},
	{"EnableOutline", (PyCFunction)PySfShape_EnableOutline, METH_O, "EnableOutline(Enable)\n\
Enable or disable drawing the shape outline. Outline is enabled by default.\n\
	Enable : True to enable, false to disable"},
	{"AddPoint", (PyCFunction)PySfShape_AddPoint, METH_VARARGS | METH_KEYWORDS, "AddPoint(X, Y, Col=sf.Color.White, OutlineCol=sf.Color.Black)\n\
Add a point to the shape.\n\
	X :          X coordinate of the point\n\
	Y :          Y coordinate of the point\n\
	Col :        Color of the point (white by default)\n\
	OutlineCol : Outline color of the point (black by default)."},
	{"SetOutlineWidth", (PyCFunction)PySfShape_SetOutlineWidth, METH_O, "SetOutlineWidth(Width)\n\
Change the width of the shape outline.\n\
	Width : New width (use 0 to remove the outline)."},
	{"GetOutlineWidth", (PyCFunction)PySfShape_GetOutlineWidth, METH_NOARGS, "GetOutlineWidth()\n\
Get the width of the shape outline."},
	{"Line", (PyCFunction)PySfShape_Line, METH_STATIC | METH_VARARGS | METH_KEYWORDS, "Line(X0, Y0, X1, Y1, Thickness, Col, Outline = 0., OutlineCol = sf.Color(0, 0, 0))\n\
Create a shape made of a single line.\n\
	X0 :         X coordinate of the first point.\n\
	Y0 :         Y coordinate of the first point.\n\
	X1 :         X coordinate of the second point.\n\
	Y1 :         Y coordinate of the second point.\n\
	Thickness :  Line thickness.\n\
	Col :        Color used to draw the line\n\
	Outline :    Outline width (0 by default)\n\
	OutlineCol : Color used to draw the outline (black by default)."},
	{"Rectangle", (PyCFunction)PySfShape_Rectangle, METH_STATIC | METH_VARARGS | METH_KEYWORDS, "Rectangle(X0, Y0, X1, Y1, Col, Outline = 0., OutlineCol = sf.Color(0, 0, 0))\n\
Create a shape made of a single rectangle.\n\
	X0 :         X coordinate of the first point.\n\
	Y0 :         Y coordinate of the first point.\n\
	X1 :         X coordinate of the second point.\n\
	Y1 :         Y coordinate of the second point.\n\
	Col :        Color used to fill the rectangle.\n\
	Outline :    Outline width (0 by default).\n\
	OutlineCol : Color used to draw the outline (black by default)."},
	{"Circle", (PyCFunction)PySfShape_Circle, METH_STATIC | METH_VARARGS | METH_KEYWORDS, "Circle(X, Y, Radius, Col, Outline = 0., OutlineCol = sf.Color(0, 0, 0))\n\
Create a shape made of a single circle.\n\
	X :          X coordinate of the center.\n\
	Y :          Y coordinate of the center.\n\
	Radius :     Radius\n\
	Col :        Color used to fill the rectangle.\n\
	Outline :    Outline width (0 by default).\n\
	OutlineCol : Color used to draw the outline (black by default)."},
	{NULL}  /* Sentinel */
};


PyTypeObject PySfShapeType = {
	head_init
	"Shape",				/*tp_name*/
	sizeof(PySfShape),		/*tp_basicsize*/
	0,						/*tp_itemsize*/
	(destructor)PySfShape_dealloc, /*tp_dealloc*/
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
	"Shape defines a drawable convex shape ; it also defines helper functions to draw simple shapes like lines, rectangles, circles, etc.\nDefault constructor: Shape()", /* tp_doc */
	0,						/* tp_traverse */
	0,						/* tp_clear */
	0,						/* tp_richcompare */
	0,						/* tp_weaklistoffset */
	0,						/* tp_iter */
	0,						/* tp_iternext */
	PySfShape_methods,		/* tp_methods */
	0,						/* tp_members */
	0,						/* tp_getset */
	&PySfDrawableType,		/* tp_base */
	0,						/* tp_dict */
	0,						/* tp_descr_get */
	0,						/* tp_descr_set */
	0,						/* tp_dictoffset */
	0,						/* tp_init */
	0,						/* tp_alloc */
	PySfShape_new,			/* tp_new */
};


PySfShape *
GetNewPySfShape()
{
	PySfShape *Shape = PyObject_New(PySfShape, &PySfShapeType);
	Shape->IsCustom = false;
	return Shape;
}

