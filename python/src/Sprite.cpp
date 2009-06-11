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

#include "Sprite.hpp"
#include "Drawable.hpp"
#include "Color.hpp"

#include "compat.hpp"


extern PyTypeObject PySfColorType;
extern PyTypeObject PySfImageType;
extern PyTypeObject PySfIntRectType;
extern PyTypeObject PySfDrawableType;


static void
PySfSprite_dealloc(PySfSprite *self)
{
	Py_CLEAR(self->Image);
	Py_CLEAR(self->SubRect);
	delete self->obj;
	free_object(self);
}

static PyObject *
PySfSprite_new(PyTypeObject *type, PyObject *args, PyObject *kwds)
{
	PySfSprite *self;

	self = (PySfSprite *)type->tp_alloc(type, 0);

	if (self != NULL)
	{
		self->Image = NULL;
		self->SubRect = NULL;
		self->IsCustom = false;
	}

	return (PyObject *)self;
}


static int
PySfSprite_init(PySfSprite *self, PyObject *args, PyObject *kwds)
{
	const char *kwlist[] = {"Image", "X", "Y", "ScaleX", "ScaleY", "Rotation", "Color", NULL};
	float X=0, Y=0, ScaleX=1, ScaleY=1, Rotation=0;
	PySfImage *Image=NULL;
	PySfColor *Color=NULL;

	if (! PyArg_ParseTupleAndKeywords(args, kwds, "O!|fffffO!:Sprite.__init__", (char **)kwlist, &PySfImageType, &Image, &X, &Y, &ScaleX, &ScaleY, &Rotation, &PySfColorType, &Color))
		return -1;

	Py_INCREF(Image);
	self->Image = Image;
	if (Color != NULL)
		self->obj = new sf::Sprite(*(Image->obj), sf::Vector2f(X, Y), sf::Vector2f(ScaleX, ScaleY), Rotation, *(Color->obj));
	else
		self->obj = new sf::Sprite(*(Image->obj), sf::Vector2f(X, Y), sf::Vector2f(ScaleX, ScaleY), Rotation);

	return 0;
}



static PyObject *
PySfSprite_SetImage(PySfSprite* self, PyObject *args)
{
	PySfImage *Image = (PySfImage *)args;
	if (! PyObject_TypeCheck(Image, &PySfImageType))
	{
		PyErr_SetString(PyExc_TypeError, "Sprite.SetImage() Argument is not a sf.Image");
		return NULL;
	}
	Py_CLEAR(self->Image);
	Py_INCREF(Image);
	self->Image = Image;
	self->obj->SetImage(*(Image->obj));
	Py_RETURN_NONE;
}

static PyObject *
PySfSprite_GetImage(PySfSprite* self)
{
	Py_INCREF(self->Image);
	return (PyObject *)(self->Image);
}

static PyObject *
PySfSprite_GetPixel(PySfSprite* self, PyObject *args)
{
	PySfColor *Color;
	unsigned int x=0, y=0;

	if (!PyArg_ParseTuple(args, "II:Sprite.GetPixel", &x, &y))
		return NULL; 

	Color = GetNewPySfColor();
	Color->obj = new sf::Color(self->obj->GetPixel(x, y));
	Color->r = Color->obj->r;
	Color->g = Color->obj->g;
	Color->b = Color->obj->b;
	Color->a = Color->obj->a;

	return (PyObject *)Color;
}

static PyObject *
PySfSprite_Resize(PySfSprite* self, PyObject *args)
{
	float W=0, H=0;

	if (! PyArg_ParseTuple(args, "ff:Sprite.Resize", &W, &H))
		return NULL; 

	self->obj->Resize(W,H);
	Py_RETURN_NONE;
}

static PyObject *
PySfSprite_GetSubRect(PySfSprite* self)
{
	if (self->SubRect != NULL)
	{
		Py_INCREF(self->SubRect);
		return (PyObject *)(self->SubRect);
	}
	else
	{
		PySfIntRect *Rect;
		Rect = GetNewPySfIntRect();
		Rect->Owner = false;
		Rect->obj = (sf::IntRect *) &(self->obj->GetSubRect());
		PySfIntRectUpdateSelf(Rect);
		Py_INCREF(Rect);
		self->SubRect = Rect;
		return (PyObject *)Rect;
	}
}

static PyObject *
PySfSprite_SetSubRect(PySfSprite* self, PyObject *args)
{
	PySfIntRect *Rect = (PySfIntRect *)args;
	if (!PyObject_TypeCheck(Rect, &PySfIntRectType))
	{
		PyErr_SetString(PyExc_TypeError, "Sprite.SetSubRect() Argument is not a sf.IntRect instance");
		return NULL;
	}
	Py_CLEAR(self->SubRect);
	Py_INCREF(Rect);
	self->SubRect = Rect;
	self->obj->SetSubRect(*(Rect->obj));
	Py_RETURN_NONE;
}

static PyObject *
PySfSprite_FlipX(PySfSprite* self, PyObject *args)
{
	self->obj->FlipX(PyBool_AsBool(args));
	Py_RETURN_NONE;
}

static PyObject *
PySfSprite_FlipY(PySfSprite* self, PyObject *args)
{
	self->obj->FlipY(PyBool_AsBool(args));
	Py_RETURN_NONE;
}

static PyObject *
PySfSprite_GetSize(PySfSprite* self)
{
	sf::Vector2f Vect = self->obj->GetSize();
	return Py_BuildValue("ff", Vect.x, Vect.y);
}

static PyMethodDef PySfSprite_methods[] = {
	{"SetImage", (PyCFunction)PySfSprite_SetImage, METH_O, "SetImage(Image)\nChange the image of the sprite.\n	Image : new image (sf.Image instance)"},
	{"GetImage", (PyCFunction)PySfSprite_GetImage, METH_NOARGS, "GetImage()\nGet the source image of the sprite."},
	{"GetSize", (PyCFunction)PySfSprite_GetSize, METH_NOARGS, "GetSize()\nGet the sprite's size."},
	{"GetPixel", (PyCFunction)PySfSprite_GetPixel, METH_VARARGS, "GetPixel()\nGet the color of a given pixel in the sprite."},
	{"Resize", (PyCFunction)PySfSprite_Resize, METH_VARARGS, "Resize(Width, Height)\nResize the sprite (by changing its scale factors). The default size is defined by the subrect.\n\
	Width  : New width (must be strictly positive)\n\
	Height : New height (must be strictly positive)"},
	{"GetSubRect", (PyCFunction)PySfSprite_GetSubRect, METH_NOARGS, "GetSubRect()\nGet the sub-rectangle of the sprite inside the source image."},
	{"SetSubRect", (PyCFunction)PySfSprite_SetSubRect, METH_O, "SetSubRect(SubRect)\nSet the sub-rectangle of the sprite inside the source image. By default, the subrect covers the entire source image.\n	SubRect : New sub-rectangle"},
	{"FlipX", (PyCFunction)PySfSprite_FlipX, METH_O, "FlipX(Flipped)\nFlip the sprite horizontally.\n	Flipped : True to flip the sprite"},
	{"FlipY", (PyCFunction)PySfSprite_FlipY, METH_O, "FlipY(Flipped)\nFlip the sprite vertically.\n	Flipped : True to flip the sprite"},
	{NULL}  /* Sentinel */
};

PyTypeObject PySfSpriteType = {
	head_init
	"Sprite",				/*tp_name*/
	sizeof(PySfSprite),		/*tp_basicsize*/
	0,						/*tp_itemsize*/
	(destructor)PySfSprite_dealloc, /*tp_dealloc*/
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
	"sfSprite defines a sprite : texture, transformations, color, and draw on screen", /* tp_doc */
	0,						/* tp_traverse */
	0,						/* tp_clear */
	0,						/* tp_richcompare */
	0,						/* tp_weaklistoffset */
	0,						/* tp_iter */
	0,						/* tp_iternext */
	PySfSprite_methods,		/* tp_methods */
	0,						/* tp_members */
	0,						/* tp_getset */
	&PySfDrawableType,		/* tp_base */
	0,						/* tp_dict */
	0,						/* tp_descr_get */
	0,						/* tp_descr_set */
	0,						/* tp_dictoffset */
	(initproc)PySfSprite_init, /* tp_init */
	0,						/* tp_alloc */
	PySfSprite_new,			/* tp_new */
};


