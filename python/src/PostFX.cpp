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

#include "PostFX.hpp"
#include "Drawable.hpp"
#include "Image.hpp"

#include "compat.hpp"


extern PyTypeObject PySfImageType;
extern PyTypeObject PySfDrawableType;


static void
PySfPostFX_dealloc(PySfPostFX *self)
{
	delete self->obj;
	free_object(self);
}

static PyObject *
PySfPostFX_new(PyTypeObject *type, PyObject *args, PyObject *kwds);

static PyObject *
PySfPostFX_LoadFromFile (PySfPostFX *self, PyObject *args)
{
	load_from_file(self, args);
}

static PyObject *
PySfPostFX_LoadFromMemory (PySfPostFX *self, PyObject *args)
{
	char *effect;
#ifdef IS_PY3K
	PyObject *string = PyUnicode_AsUTF8String(args);
	if (string == NULL)
		return NULL;
	effect = PyBytes_AsString(string);
#else
	effect = PyString_AsString(args);
#endif
	bool result = self->obj->LoadFromMemory(effect);
#ifdef IS_PY3K
	Py_DECREF(string);
#endif
	return PyBool_FromLong(result);
}

static PyObject *PySfPostFX_SetParameter(PySfPostFX* self, PyObject *args){	char *Name;	float X, Y, Z, W;	int size = PyTuple_Size(args);	if (!PyArg_ParseTuple(args, "sf|fff:PostFX.SetParameter", &Name, &X, &Y, &Z, &W))		return NULL;

	switch (size)
	{
		case 2:
			self->obj->SetParameter(Name, X);
			break;
		case 3:
			self->obj->SetParameter(Name, X, Y);
			break;
		case 4:
			self->obj->SetParameter(Name, X, Y, Z);
			break;
		case 5:
			self->obj->SetParameter(Name, X, Y, Z, W);
			break;
		default:
			break;
	}

	Py_RETURN_NONE;
}

static PyObject *
PySfPostFX_SetTexture(PySfPostFX* self, PyObject *args)
{
	PySfImage *Image = NULL;
	char *Name;
	if (! PyArg_ParseTuple(args, "s|O", &Name, &Image))
		return NULL;
	if (Image == NULL || (PyObject *)Image == Py_None)
		self->obj->SetTexture(Name, NULL);
	else
	{
		if (!PyObject_TypeCheck(Image, &PySfImageType))
		{
			PyErr_SetString(PyExc_TypeError, "PostFX.SetTexture() Argument 2, if specified, must be a sf.Image instance or None.");
			return NULL;
		}
		self->obj->SetTexture(Name, Image->obj);
	}
	Py_RETURN_NONE;
}

static PyObject *
PySfPostFX_CanUsePostFX(PySfPostFX* self, PyObject *args)
{
	return PyBool_FromLong(sf::PostFX::CanUsePostFX());
}


static PyMethodDef PySfPostFX_methods[] = {
	{"LoadFromFile", (PyCFunction)PySfPostFX_LoadFromFile, METH_O, "LoadFromFile(Filename)\nLoad the effect from a file."},
	{"LoadFromMemory", (PyCFunction)PySfPostFX_LoadFromMemory, METH_O, "LoadFromMemory(Effect)\nLoad the effect from a text in memory."},
	{"SetParameter", (PyCFunction)PySfPostFX_SetParameter, METH_VARARGS, "SetParameter(X), SetParameter(X, Y), SetParameter(X, Y, Z), SetParameter(X, Y, Z, W)\nChange a parameter of the effect.\n\
	Name    : Parameter name in the effect\n\
	X,Y,Z,W : Values to assign."},
	{"SetTexture", (PyCFunction)PySfPostFX_SetTexture, METH_VARARGS, "SetTexture(Name, Texture)\nSet a texture parameter.\n\
	Name    : Texture name in the effect\n\
	Texture : Image to set (pass None to use content of current framebuffer)"},
	{"CanUsePostFX", (PyCFunction)PySfPostFX_CanUsePostFX, METH_STATIC | METH_NOARGS, "CanUsePostFX()\nTell wether or not the system supports post-effects."},
	{NULL}  /* Sentinel */
};

PyTypeObject PySfPostFXType = {
	head_init
	"PostFX",				/*tp_name*/
	sizeof(PySfPostFX),		/*tp_basicsize*/
	0,						/*tp_itemsize*/
	(destructor)PySfPostFX_dealloc, /*tp_dealloc*/
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
	"sf.PostFX is used to apply a post effect to a window.\n\
Default constructor : sf.PostFX()\n\
Copy constructor : sf.PostFX(Copy) where Copy is a sf.PostFX instance.", /* tp_doc */
	0,						/* tp_traverse */
	0,						/* tp_clear */
	0,						/* tp_richcompare */
	0,						/* tp_weaklistoffset */
	0,						/* tp_iter */
	0,						/* tp_iternext */
	PySfPostFX_methods,		/* tp_methods */
	0,						/* tp_members */
	0,						/* tp_getset */
	&PySfDrawableType,		/* tp_base */
	0,						/* tp_dict */
	0,						/* tp_descr_get */
	0,						/* tp_descr_set */
	0,						/* tp_dictoffset */
	0,						/* tp_init */
	0,						/* tp_alloc */
	PySfPostFX_new,			/* tp_new */
};


static PyObject *
PySfPostFX_new(PyTypeObject *type, PyObject *args, PyObject *kwds)
{
	PySfPostFX *self;
	self = (PySfPostFX *)type->tp_alloc(type, 0);
	if (self != NULL)
	{
		PySfPostFX *Copy = NULL;
		self->IsCustom = false;
		if (!PyArg_ParseTuple(args, "|O!", &PySfPostFXType, &Copy))
			return NULL;
		if (Copy) self->obj = new sf::PostFX(*(Copy->obj));
		else self->obj = new sf::PostFX();
	}
	return (PyObject *)self;
}
