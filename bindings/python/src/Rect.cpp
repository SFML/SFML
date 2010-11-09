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

#include "Rect.hpp"

#include <structmember.h>

#include "compat.hpp"
#include "offsetof.hpp"


static PyMemberDef PySfIntRect_members[] = {
	{(char *)"Left", T_INT, offsetof(PySfIntRect, Left), 0, (char *)"Left coordinate of the rectangle."},
	{(char *)"Top", T_INT, offsetof(PySfIntRect, Top), 0, (char *)"Top coordinate of the rectangle."},
	{(char *)"Right", T_INT, offsetof(PySfIntRect, Right), 0, (char *)"Right coordinate of the rectangle."},
	{(char *)"Bottom", T_INT, offsetof(PySfIntRect, Bottom), 0, (char *)"Bottom coordinate of the rectangle."},
	{NULL}  /* Sentinel */
};

static PyMemberDef PySfFloatRect_members[] = {
	{(char *)"Left", T_FLOAT, offsetof(PySfFloatRect, Left), 0, (char *)"Left coordinate of the rectangle."},
	{(char *)"Top", T_FLOAT, offsetof(PySfFloatRect, Top), 0, (char *)"Top coordinate of the rectangle."},
	{(char *)"Right", T_FLOAT, offsetof(PySfFloatRect, Right), 0, (char *)"Right coordinate of the rectangle."},
	{(char *)"Bottom", T_FLOAT, offsetof(PySfFloatRect, Bottom), 0, (char *)"Bottom coordinate of the rectangle."},
	{NULL}  /* Sentinel */
};

static void
PySfIntRect_dealloc(PySfIntRect* self)
{
	if (self->Owner)
		delete self->obj;
	free_object(self);
}

static void
PySfFloatRect_dealloc(PySfFloatRect* self)
{
	if (self->Owner)
		delete self->obj;
	free_object(self);
}

static PyObject *
PySfIntRect_new(PyTypeObject *type, PyObject *args, PyObject *kwds)
{
	const char *kwlist[] = {"Left", "Top", "Right", "Bottom", NULL};
	PySfIntRect *self;
	self = (PySfIntRect *)type->tp_alloc(type, 0);
	if (self != NULL)
	{
		self->Owner = true;
		if (!PyArg_ParseTupleAndKeywords(args, kwds, "iiii:IntRect.__new__", (char **)kwlist, &(self->Left), &(self->Top), &(self->Right), &(self->Bottom)))
			return NULL;
		self->obj = new sf::IntRect(self->Left, self->Top, self->Right, self->Bottom);
	}
	return (PyObject *)self;
}

static PyObject *
PySfFloatRect_new(PyTypeObject *type, PyObject *args, PyObject *kwds)
{
	const char *kwlist[] = {"Left", "Top", "Right", "Bottom", NULL};
	PySfFloatRect *self;
	self = (PySfFloatRect *)type->tp_alloc(type, 0);
	if (self != NULL)
	{
		self->Owner = true;
		if (!PyArg_ParseTupleAndKeywords(args, kwds, "ffff:FloatRect.__new__", (char **)kwlist, &(self->Left), &(self->Top), &(self->Right), &(self->Bottom)))
			return NULL;
		self->obj = new sf::FloatRect(self->Left, self->Top, self->Right, self->Bottom);
	}
	return (PyObject *)self;
}

static PyObject *
PySfIntRect_GetSize(PySfIntRect *self)
{
	sf::Vector2i  size( self->obj->GetSize() );
	return Py_BuildValue( "ii", size.x, size.y );
}

static PyObject *
PySfIntRect_Contains(PySfIntRect* self, PyObject *args);

static PyObject *
PySfIntRect_Intersects(PySfIntRect* self, PyObject *args);

static PyObject *
PySfFloatRect_GetSize(PySfFloatRect *self)
{
	sf::Vector2f  size( self->obj->GetSize() );
	return Py_BuildValue( "ff", size.x, size.y );
}

static PyObject *
PySfFloatRect_Contains(PySfFloatRect* self, PyObject *args);

static PyObject *
PySfFloatRect_Intersects(PySfFloatRect* self, PyObject *args);

static PyObject *
PySfIntRect_Offset(PySfIntRect* self, PyObject *args)
{
	int OffsetX, OffsetY;

	if (!PyArg_ParseTuple(args, "ii:IntRect.Offset", &OffsetX, &OffsetY))
		return NULL; 

	self->obj->Offset(OffsetX, OffsetY);
	PySfIntRectUpdateSelf(self);
	Py_RETURN_NONE;
}

static PyObject *
PySfFloatRect_Offset(PySfFloatRect* self, PyObject *args)
{
	float OffsetX, OffsetY;

	if (!PyArg_ParseTuple(args, "ff:FloatRect.Offset", &OffsetX, &OffsetY))
		return NULL; 

	self->obj->Offset(OffsetX, OffsetY);
	PySfFloatRectUpdateSelf(self);
	Py_RETURN_NONE;
}


static PyMethodDef PySfIntRect_methods[] = {
	{"Offset", (PyCFunction)PySfIntRect_Offset, METH_VARARGS, "Offset(OffsetX, OffsetY)\n\
Move the whole rectangle by the given offset.\n\
	OffsetX : Horizontal offset\n\
	OffsetY : Vertical offset\n\
"},
	{"GetSize", (PyCFunction)PySfIntRect_GetSize, METH_NOARGS, "GetSize()\nGet the rectangle's size."},
	{"Contains", (PyCFunction)PySfIntRect_Contains, METH_VARARGS, "Contains(X, Y)\n\
Check if a point is inside the rectangle's area.\n\
	X : X coordinate of the point to test\n\
	Y : Y coordinate of the point to test"},
	{"Intersects", (PyCFunction)PySfIntRect_Intersects, METH_VARARGS, "Intersects(Rectangle, OverlappingRect=None)\n\
Check intersection between two rectangles.\n\
	Rectangle :       Rectangle to test\n\
	OverlappingRect : Rectangle to be filled with overlapping rect (None by default)"},
	{NULL}  /* Sentinel */
};


static PyMethodDef PySfFloatRect_methods[] = {
	{"Offset", (PyCFunction)PySfFloatRect_Offset, METH_VARARGS, "Offset(OffsetX, OffsetY)\n\
Move the whole rectangle by the given offset.\n\
	OffsetX : Horizontal offset\n\
	OffsetY : Vertical offset\n\
"},
	{"GetSize", (PyCFunction)PySfFloatRect_GetSize, METH_NOARGS, "GetSize()\nGet the rectangle's size."},
	{"Contains", (PyCFunction)PySfFloatRect_Contains, METH_VARARGS, "Contains(X, Y)\n\
Check if a point is inside the rectangle's area.\n\
	X : X coordinate of the point to test\n\
	Y : Y coordinate of the point to test"},
	{"Intersects", (PyCFunction)PySfFloatRect_Intersects, METH_VARARGS, "Intersects(Rectangle, OverlappingRect=None)\n\
Check intersection between two rectangles.\n\
	Rectangle :       Rectangle to test\n\
	OverlappingRect : Rectangle to be filled with overlapping rect (None by default)"},
	{NULL}  /* Sentinel */
};

int
PySfIntRect_setattro(PyObject* self, PyObject *attr_name, PyObject *v)
{
	int result = PyObject_GenericSetAttr(self, attr_name, v);
	PySfIntRect *Rect = (PySfIntRect *)self;
	PySfIntRectUpdateObj(Rect);
	return result;
}

int
PySfFloatRect_setattro(PyObject* self, PyObject *attr_name, PyObject *v)
{
	int result = PyObject_GenericSetAttr(self, attr_name, v);
	PySfFloatRect *Rect = (PySfFloatRect *)self;
	PySfFloatRectUpdateObj(Rect);
	return result;
}

PyTypeObject PySfIntRectType = {
	head_init
	"IntRect",				/*tp_name*/
	sizeof(PySfIntRect),	/*tp_basicsize*/
	0,						/*tp_itemsize*/
	(destructor)PySfIntRect_dealloc, /*tp_dealloc*/
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
	PySfIntRect_setattro,	/*tp_setattro*/
	0,						/*tp_as_buffer*/
	Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE, /*tp_flags*/
	"sf.IntRect is an utility class for manipulating rectangles.", /* tp_doc */
	0,						/* tp_traverse */
	0,						/* tp_clear */
	0,						/* tp_richcompare */
	0,						/* tp_weaklistoffset */
	0,						/* tp_iter */
	0,						/* tp_iternext */
	PySfIntRect_methods,	/* tp_methods */
	PySfIntRect_members,	/* tp_members */
	0,						/* tp_getset */
	0,						/* tp_base */
	0,						/* tp_dict */
	0,						/* tp_descr_get */
	0,						/* tp_descr_set */
	0,						/* tp_dictoffset */
	0,						/* tp_init */
	0,						/* tp_alloc */
	PySfIntRect_new,		/* tp_new */
};


PyTypeObject PySfFloatRectType = {
	head_init
	"FloatRect",			/*tp_name*/
	sizeof(PySfFloatRect),	/*tp_basicsize*/
	0,						/*tp_itemsize*/
	(destructor)PySfFloatRect_dealloc, /*tp_dealloc*/
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
	PySfFloatRect_setattro,	/*tp_setattro*/
	0,						/*tp_as_buffer*/
	Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE, /*tp_flags*/
	"sf.FloatRect is an utility class for manipulating rectangles.", /* tp_doc */
	0,						/* tp_traverse */
	0,						/* tp_clear */
	0,						/* tp_richcompare */
	0,						/* tp_weaklistoffset */
	0,						/* tp_iter */
	0,						/* tp_iternext */
	PySfFloatRect_methods,	/* tp_methods */
	PySfFloatRect_members,	/* tp_members */
	0,						/* tp_getset */
	0,						/* tp_base */
	0,						/* tp_dict */
	0,						/* tp_descr_get */
	0,						/* tp_descr_set */
	0,						/* tp_dictoffset */
	0,						/* tp_init */
	0,						/* tp_alloc */
	PySfFloatRect_new,		/* tp_new */
};


static PyObject *
PySfFloatRect_Contains(PySfFloatRect* self, PyObject *args)
{
	float x=0, y=0;

	if (!PyArg_ParseTuple(args, "ff:FloatRect.Contains", &x, &y))
		return NULL; 

	return PyBool_FromLong(self->obj->Contains(x,y));
}

static PyObject *
PySfFloatRect_Intersects(PySfFloatRect* self, PyObject *args)
{
	PySfFloatRect *Rect=NULL, *Intersection=NULL;
	bool result;

	if (!PyArg_ParseTuple(args, "O!|O!:FloatRect.Intersects", &PySfFloatRectType, &Rect, &PySfFloatRectType, &Intersection))
		return NULL; 

	if (Intersection)
		result = self->obj->Intersects(*(Rect->obj), *(Intersection->obj));
	else
		result = self->obj->Intersects(*(Rect->obj));

	return PyBool_FromLong(result);
}


static PyObject *
PySfIntRect_Contains(PySfIntRect* self, PyObject *args)
{
	unsigned int x=0, y=0;

	if (!PyArg_ParseTuple(args, "II:IntRect.Contains",  &x, &y))
		return NULL;

	return PyBool_FromLong(self->obj->Contains(x,y));
}

static PyObject *
PySfIntRect_Intersects(PySfIntRect* self, PyObject *args)
{
	PySfIntRect *Rect=NULL, *Intersection=NULL;
	bool result;

	if (!PyArg_ParseTuple(args, "O!|O!:IntRect.Intersects", &PySfIntRectType, &Rect, &PySfIntRectType, &Intersection))
		return NULL; 

	if (Intersection)
		result = self->obj->Intersects(*(Rect->obj), *(Intersection->obj));
	else
		result = self->obj->Intersects(*(Rect->obj));

	return PyBool_FromLong(result);
}

void
PySfIntRectUpdateObj(PySfIntRect *self)
{
	self->obj->Left = self->Left;
	self->obj->Right = self->Right;
	self->obj->Top = self->Top;
	self->obj->Bottom = self->Bottom;
}

void
PySfFloatRectUpdateObj(PySfFloatRect *self)
{
	self->obj->Left = self->Left;
	self->obj->Right = self->Right;
	self->obj->Top = self->Top;
	self->obj->Bottom = self->Bottom;
}

void
PySfIntRectUpdateSelf(PySfIntRect *self)
{
	self->Left = self->obj->Left;
	self->Right = self->obj->Right;
	self->Top = self->obj->Top;
	self->Bottom = self->obj->Bottom;
}

void
PySfFloatRectUpdateSelf(PySfFloatRect *self)
{
	self->Left = self->obj->Left;
	self->Right = self->obj->Right;
	self->Top = self->obj->Top;
	self->Bottom = self->obj->Bottom;
}

PySfIntRect *
GetNewPySfIntRect()
{
	return PyObject_New(PySfIntRect, &PySfIntRectType);
}

PySfFloatRect *
GetNewPySfFloatRect()
{
	return PyObject_New(PySfFloatRect, &PySfFloatRectType);
}


