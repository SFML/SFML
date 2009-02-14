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

#include "Image.hpp"
#include "RenderWindow.hpp"

extern PyTypeObject PySfColorType;
extern PyTypeObject PySfIntRectType;
extern PyTypeObject PySfRenderWindowType;

static PyMemberDef PySfImage_members[] = {
	{NULL}  /* Sentinel */
};


static void
PySfImage_dealloc(PySfImage* self)
{
	delete self->obj;
	self->ob_type->tp_free((PyObject*)self);
}

static PyObject *
PySfImage_new(PyTypeObject *type, PyObject *args, PyObject *kwds)
{
	PySfImage *self;

	self = (PySfImage *)type->tp_alloc(type, 0);

	if (self != NULL)
	{
	}

	return (PyObject *)self;
}


static PyObject *
PySfImage_Create(PySfImage* self, PyObject *args, PyObject *kwds)
{
	PySfColor *ColorTmp=NULL;
	sf::Color *Color;
	unsigned int Width=0, Height=0;
	const char *kwlist[] = {"Width", "Height", "Color", NULL};

	if (! PyArg_ParseTupleAndKeywords(args, kwds, "|IIO!", (char **)kwlist, &Width, &Height, &PySfColorType, &ColorTmp))
		return NULL; 

	if (ColorTmp)
	{
		Color = ColorTmp->obj;
		PySfColorUpdate(ColorTmp);
		self->obj->Create(Width, Height, *Color);
	}
	else
		self->obj->Create(Width, Height);

	Py_RETURN_NONE;
}

static PyObject *
PySfImage_CopyScreen(PySfImage* self, PyObject *args)
{
// bool CopyScreen(RenderWindow& Window, const IntRect& SourceRect = IntRect(0, 0, 0, 0));
	PySfRenderWindow *RenderWindow;
	PySfIntRect *SourceRect=NULL;
	bool Result;

	if (! PyArg_ParseTuple(args, "O!|O!", &PySfRenderWindowType, &RenderWindow, &PySfIntRectType, &SourceRect))
		return NULL; 


	if (SourceRect)
	{
		PySfIntRectUpdateObj(SourceRect);
		Result = self->obj->CopyScreen(*(RenderWindow->obj), *(SourceRect->obj));
	}
	else
		Result = self->obj->CopyScreen(*(RenderWindow->obj));
	if (Result)
		Py_RETURN_TRUE;
	else
		Py_RETURN_FALSE;
}

static PyObject *
PySfImage_SetPixel(PySfImage* self, PyObject *args, PyObject *kwds)
{
	PySfColor *ColorTmp=NULL;
	sf::Color *Color;
	unsigned int x=0, y=0;
	const char *kwlist[] = {"x", "y", "Color", NULL};


	if (! PyArg_ParseTupleAndKeywords(args, kwds, "II|O!", (char **)kwlist, &x, &y, &PySfColorType, &ColorTmp))
		return NULL; 


	if (ColorTmp)
	{
		Color = ColorTmp->obj;
		PySfColorUpdate(ColorTmp);
		self->obj->SetPixel(x, y, *Color);
	}

	Py_RETURN_NONE;
}

static PyObject *
PySfImage_GetPixel(PySfImage* self, PyObject *args)
{
	PySfColor *Color;
	unsigned int x=0, y=0;


	if (! PyArg_ParseTuple(args, "II", &x, &y))
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
PySfImage_CreateMaskFromColor(PySfImage* self, PyObject *args)
{
	PySfColor *ColorTmp= (PySfColor *)args;
	sf::Color *Color;

	if ( ! PyObject_TypeCheck(ColorTmp, &PySfColorType))
		PyErr_SetString(PyExc_ValueError, "Argument must be a sf.Color");
	Color = ColorTmp->obj;
	PySfColorUpdate(ColorTmp);
	self->obj->CreateMaskFromColor(*Color);

	Py_RETURN_NONE;
}

static PyObject *
PySfImage_LoadFromMemory(PySfImage* self, PyObject *args)
{
	unsigned int SizeInBytes;
	char *Data;

	if (! PyArg_ParseTuple(args, "s#", &Data, &SizeInBytes))
		return NULL; 

	if (self->obj->LoadFromMemory(Data, (std::size_t) SizeInBytes))
		Py_RETURN_TRUE;
	else
		Py_RETURN_FALSE;

}

static PyObject *
PySfImage_LoadFromPixels(PySfImage* self, PyObject *args)
{
	unsigned int Width, Height, Size;
	char *Data;

	if (! PyArg_ParseTuple(args, "IIs#", &Width, &Height, &Data, &Size))
		return NULL; 

	self->obj->LoadFromPixels(Width, Height, (sf::Uint8*) Data);
	Py_RETURN_NONE;
}

static PyObject *
PySfImage_GetPixels(PySfImage *self)
{
	return PyString_FromStringAndSize((const char *)(self->obj->GetPixelsPtr()), self->obj->GetWidth()*self->obj->GetHeight()*4);
}

static PyObject *
PySfImage_LoadFromFile (PySfImage *self, PyObject *args)
{
	char *path = PyString_AsString(args);

	if (self->obj->LoadFromFile(path))
		Py_RETURN_TRUE;
	else
		Py_RETURN_FALSE;
}

static PyObject *
PySfImage_SaveToFile (PySfImage *self, PyObject *args)
{
	char *path = PyString_AsString(args);

	if (self->obj->SaveToFile(path))
		Py_RETURN_TRUE;
	else
		Py_RETURN_FALSE;
}

static int
PySfImage_init(PySfImage *self, PyObject *args, PyObject *kwds);

static PyObject *
PySfImage_Bind(PySfImage *self)
{
	self->obj->Bind();
	Py_RETURN_NONE;
}

static PyObject *
PySfImage_SetSmooth (PySfImage *self, PyObject *args)
{
	bool arg=false;
	if (PyObject_IsTrue(args))
		arg = true;
	self->obj->SetSmooth(arg);
	Py_RETURN_NONE;
}

static PyObject *
PySfImage_GetWidth(PySfImage *self)
{
	return PyLong_FromUnsignedLong(self->obj->GetWidth());
}

static PyObject *
PySfImage_GetHeight(PySfImage *self)
{
	return PyLong_FromUnsignedLong(self->obj->GetHeight());
}

static PyObject *
PySfImage_GetValidTextureSize(PySfImage* self, PyObject *args)
{
	unsigned long S = PyLong_AsUnsignedLong(args);
	return PyLong_FromUnsignedLong(sf::Image::GetValidTextureSize(S));
}

static PyObject *
PySfImage_GetTexCoords(PySfImage* self, PyObject *args)
{
	PySfIntRect *RectArg = NULL;
	bool Adjust = false;
	PyObject *AdjustObj = NULL;

	if (! PyArg_ParseTuple(args, "O!|O", &PySfIntRectType, &RectArg, &AdjustObj))
		return NULL;

	if (PyObject_IsTrue(AdjustObj))
		Adjust = true;

	PySfFloatRect *Rect;

	Rect = GetNewPySfFloatRect();
	Rect->obj = new sf::FloatRect ( self->obj->GetTexCoords(*(RectArg->obj), Adjust) );
	Rect->Left = Rect->obj->Left;
	Rect->Top = Rect->obj->Top;
	Rect->Right = Rect->obj->Right;
	Rect->Bottom = Rect->obj->Bottom;

	return (PyObject *)Rect;
}

static PyObject *
PySfImage_Copy(PySfImage* self, PyObject *args);

static PyMethodDef PySfImage_methods[] = {
	{"Copy", (PyCFunction)PySfImage_Copy, METH_VARARGS, "Copy(Source, DestX, DestY, SourceRect = sf.IntRect(0,0,0,0))\n\
Copy pixels from another image onto this one. This function does a slow pixel copy and should only be used at initialization time.\n\
	Source :     Source image to copy\n\
	DestX :      X coordinate of the destination position\n\
	DestY :      Y coordinate of the destination position\n\
	SourceRect : Sub-rectangle of the source image to copy (empty by default - entire image)"},
	{"Create", (PyCFunction)PySfImage_Create, METH_VARARGS, "Create(Width=0, Height=0, Color=sf.Color.Black)\n\
Create an empty image.\n\
	Width 	: Image width\n\
	Height 	: Image height\n\
	Col 	: Image color (black by default)"},
	{"CopyScreen", (PyCFunction)PySfImage_CopyScreen, METH_VARARGS, "CopyScreen(Window, SourceRect)\n\
Create the image from the current contents of the given window. Return True if copy was successful.\n\
	Window :     Window to capture\n\
	SourceRect : Sub-rectangle of the screen to copy (empty by default - entire image)"},
	{"SetPixel", (PyCFunction)PySfImage_SetPixel, METH_VARARGS | METH_KEYWORDS, "SetPixel(X, Y, Col)\nChange the color of a pixel.\n\
	X   : X coordinate of pixel in the image\n	Y   : Y coordinate of pixel in the image\n	Col : New color for pixel (X, Y)"},
	{"GetPixel", (PyCFunction)PySfImage_GetPixel, METH_VARARGS, "GetPixel(X, Y)\nGet a pixel from the image."},
	{"LoadFromFile", (PyCFunction)PySfImage_LoadFromFile, METH_O, "LoadFromFile(Path)\nLoad the surface from a file."},
	{"SaveToFile", (PyCFunction)PySfImage_SaveToFile, METH_O, "SaveToFile(Path)\nSave the content of the image to a file."},
	{"LoadFromMemory", (PyCFunction)PySfImage_LoadFromMemory, METH_VARARGS, "LoadFromMemory(Data)\nLoad the image from a file in memory."},
	{"LoadFromPixels", (PyCFunction)PySfImage_LoadFromPixels, METH_VARARGS, "LoadFromPixels(Width, Height, Data)\nLoad the image directly from a string of pixels."},
	{"GetPixels", (PyCFunction)PySfImage_GetPixels, METH_NOARGS, "GetPixels()\nGet a string representing the array of pixels (8 bits integers RGBA). String length is GetWidth() x GetHeight() x 4. This string becomes invalid if you reload or resize the image."},
	{"CreateMaskFromColor", (PyCFunction)PySfImage_CreateMaskFromColor, METH_O, "CreateMaskFromColor(Color)\nCreate transparency mask from a specified colorkey."},
	{"Bind", (PyCFunction)PySfImage_Bind, METH_NOARGS, "Bind()\nBind the image for rendering."},
	{"SetSmooth", (PyCFunction)PySfImage_SetSmooth, METH_VARARGS, "SetSmooth(Smooth)\nEnable or disable image smooth filter."},
	{"GetWidth", (PyCFunction)PySfImage_GetWidth, METH_NOARGS, "GetWidth()\nReturn the width of the image."},
	{"GetHeight", (PyCFunction)PySfImage_GetHeight, METH_NOARGS, "GetHeight()\nReturn the height of the image."},
	{"GetTexCoords", (PyCFunction)PySfImage_GetTexCoords, METH_VARARGS, "GetTexCoords(Rect, Adjust=True)\nConvert a subrect expressed in pixels, into float texture coordinates. Returns texture coordinates corresponding to the sub-rectangle (sf.FloatRect instance)\n\
	Rect 	: Sub-rectangle of image to convert\n\
	Adjust 	: Pass true to apply the half-texel adjustment"},
	{"GetValidTextureSize", (PyCFunction)PySfImage_GetValidTextureSize, METH_STATIC | METH_O, "GetValidTextureSize(Size)\nGet a valid texture size according to hardware support. Returns valid nearest size (greater than or equal to specified size).\n\
	Size 	: Size to convert"},
	{NULL}  /* Sentinel */
};

PyTypeObject PySfImageType = {
	PyObject_HEAD_INIT(NULL)
	0,						/*ob_size*/
	"Image",				/*tp_name*/
	sizeof(PySfImage),		/*tp_basicsize*/
	0,						/*tp_itemsize*/
	(destructor)PySfImage_dealloc, /*tp_dealloc*/
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
	"sf.Image is the low-level class for loading and manipulating images.\n\
Default constructor : sf.Image()\n\
Other constructors : sf.Image(Width=0, Height=0, Color=sf.Color.Black) or sf.Image(Width, Height, Data).\n\
Copy constructor : sf.Image(Copy) where Copy is a sf.Image instance.", /* tp_doc */
	0,						/* tp_traverse */
	0,						/* tp_clear */
	0,						/* tp_richcompare */
	0,						/* tp_weaklistoffset */
	0,						/* tp_iter */
	0,						/* tp_iternext */
	PySfImage_methods,		/* tp_methods */
	PySfImage_members,		/* tp_members */
	0,						/* tp_getset */
	0,						/* tp_base */
	0,						/* tp_dict */
	0,						/* tp_descr_get */
	0,						/* tp_descr_set */
	0,						/* tp_dictoffset */
	(initproc)PySfImage_init, /* tp_init */
	0,						/* tp_alloc */
	PySfImage_new,			/* tp_new */
};

static int
PySfImage_init(PySfImage *self, PyObject *args, PyObject *kwds)
{
	if (PyTuple_Size(args) == 1)
	{
		PySfImage *Image;
		if (PyArg_ParseTuple(args, "O!", &PySfImageType, &Image))
		{
			self->obj = new sf::Image(*(Image->obj));
			return 0;
		}
	}
	self->obj = new sf::Image();
	if (PyTuple_Size(args) > 0)
	{
		if (PySfImage_Create(self, args, kwds) == NULL)
			if (PySfImage_LoadFromPixels(self, args) == NULL)
				return -1;
	}
	return 0;
}

static PyObject *
PySfImage_Copy(PySfImage* self, PyObject *args)
{
	PySfIntRect *SourceRect = NULL;
	PySfImage *Source = NULL;
	unsigned int DestX, DestY;
	if (! PyArg_ParseTuple(args, "O!II|O!", &PySfImageType, &Source, &DestX, &DestY, &PySfIntRectType, &SourceRect))
		return NULL;

	if (SourceRect)
	{
		PySfIntRectUpdateObj(SourceRect);
		self->obj->Copy(*(Source->obj), DestX, DestY, *(SourceRect->obj));
	}
	else
		self->obj->Copy(*(Source->obj), DestX, DestY);

	Py_RETURN_NONE;
}

PySfImage *
GetNewPySfImage()
{
	return (PySfImage *)PySfImage_new(&PySfImageType, NULL, NULL);
}

