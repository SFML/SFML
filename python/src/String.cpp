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

#include "String.hpp"
#include "Font.hpp"
#include "Color.hpp"
#include "Rect.hpp"


extern PyTypeObject PySfColorType;
extern PyTypeObject PySfImageType;
extern PyTypeObject PySfDrawableType;
extern PyTypeObject PySfFontType;

static PyMemberDef PySfString_members[] = {
    {NULL}  /* Sentinel */
};



static void
PySfString_dealloc(PySfString *self)
{
	delete self->obj;
	self->ob_type->tp_free((PyObject*)self);
}

static PyObject *
PySfString_new(PyTypeObject *type, PyObject *args, PyObject *kwds)
{
	PySfString *self;

	self = (PySfString *)type->tp_alloc(type, 0);

	if (self != NULL)
	{
	}

	return (PyObject *)self;
}


static int
PySfString_init(PySfString *self, PyObject *args, PyObject *kwds)
{
	const char *kwlist[] = {"Text", "Font", "Size", NULL};
	float Size = 30.f;
	std::string Text = "";
	char *TextTmp = NULL;
	unsigned int TextSize;
	PySfFont *FontTmp = NULL;
	sf::Font *Font;

	if (! PyArg_ParseTupleAndKeywords(args, kwds, "|s#O!f", (char **)kwlist, &TextTmp, &TextSize, &PySfFontType, &FontTmp, &Size))
		return -1;

	if (FontTmp)
		Font = (FontTmp->obj);
	else
		Font = (sf::Font *)&(sf::Font::GetDefaultFont());

	if (TextSize >= 2 && TextTmp)
		if ((unsigned char)TextTmp[0] == 0xff && (unsigned char)TextTmp[1] == 0xfe)
		{
			self->obj = new sf::String(sf::Unicode::Text((const sf::Uint16 *)(TextTmp+2)), *Font, Size);
			return 0;
		}

	if (TextTmp != NULL)
		self->obj = new sf::String(sf::Unicode::Text((const sf::Uint8 *)(TextTmp)), *Font, Size);
	else
		self->obj = new sf::String();
	return 0;
}



static PyObject *
PySfString_SetText(PySfString* self, PyObject *args)
{
	char *TextTmp = NULL;
	int Size;
	if (!PyArg_Parse(args, "s#", &TextTmp, &Size))
		return NULL;

	if (Size >= 2)
	{
		if ((unsigned char)TextTmp[0] == 0xff && (unsigned char)TextTmp[1] == 0xfe)
		{
			self->obj->SetText(sf::Unicode::Text((const sf::Uint16 *)(TextTmp+2)));
			Py_RETURN_NONE;
		}
	}
	self->obj->SetText(sf::Unicode::Text((const sf::Uint8 *)(TextTmp)));
	Py_RETURN_NONE;
}

static PyObject *
PySfString_SetFont(PySfString* self, PyObject *args)
{
	PySfFont *Font = (PySfFont *)args;
	if (!PyObject_TypeCheck(Font, &PySfFontType))
		PyErr_SetString(PyExc_ValueError, "Argument must be a sf.Font");
	self->obj->SetFont(*(Font->obj));
	Py_RETURN_NONE;
}

static PyObject *
PySfString_SetSize(PySfString* self, PyObject *args)
{
	self->obj->SetSize(PyFloat_AsDouble(args));
	Py_RETURN_NONE;
}

static PyObject *
PySfString_GetSize(PySfString* self)
{
	return PyFloat_FromDouble(self->obj->GetSize());
}

static PyObject *
PySfString_SetStyle(PySfString* self, PyObject *args)
{
	self->obj->SetStyle(PyLong_AsUnsignedLong(args));
	Py_RETURN_NONE;
}

static PyObject *
PySfString_GetStyle(PySfString* self)
{
	return PyLong_FromUnsignedLong(self->obj->GetStyle());
}

static PyObject *
PySfString_GetText(PySfString* self)
{
	return PyString_FromString((std::string(self->obj->GetText())).c_str());
}

static PyObject *
PySfString_GetFont(PySfString* self)
{
	PySfFont *Font = GetNewPySfFont();
	Font->obj = new sf::Font(self->obj->GetFont());
	return (PyObject *)Font;
}

static PyObject *
PySfString_GetRect(PySfString* self)
{
	PySfFloatRect *Rect;

	Rect = GetNewPySfFloatRect();
	Rect->obj = new sf::FloatRect (self->obj->GetRect());
	Rect->Left = Rect->obj->Left;
	Rect->Top = Rect->obj->Top;
	Rect->Right = Rect->obj->Right;
	Rect->Bottom = Rect->obj->Bottom;

	return (PyObject *)Rect;
}

static PyObject *
PySfString_GetCharacterPos(PySfString* self, PyObject *args)
{
	sf::Vector2f Pos = self->obj->GetCharacterPos(PyLong_AsUnsignedLong(args));
	return Py_BuildValue("ff", Pos.x, Pos.y);
}


static PyMethodDef PySfString_methods[] = {
	{"GetCharacterPos", (PyCFunction)PySfString_GetCharacterPos, METH_O, "GetCharacterPos(Index)\n\
Return the visual position (a tuple of two floats) of the Index-th character of the string, in coordinates relative to the string (note : translation, center, rotation and scale are not applied)\n\
	Index : Index of the character"},
	{"SetText", (PyCFunction)PySfString_SetText, METH_O, "SetText(Text)\nSet the text (an utf-8 or utf-16 string).\n	Text : New text"},
	{"GetText", (PyCFunction)PySfString_GetText, METH_NOARGS, "GetText()\nGet the text."},
	{"SetFont", (PyCFunction)PySfString_SetFont, METH_O, "SetFont(Font)\nSet the font of the string.\n	Font : font to use"},
	{"GetFont", (PyCFunction)PySfString_GetFont, METH_NOARGS, "GetFont()\nGet the font used by the string."},
	{"SetSize", (PyCFunction)PySfString_SetSize, METH_O, "SetSize(Size)\nSet the size of the string.\n	Size : New size, in pixels"},
	{"GetSize", (PyCFunction)PySfString_GetSize, METH_NOARGS, "GetSize()\nGet the size of the characters."},
	{"SetStyle", (PyCFunction)PySfString_SetStyle, METH_O, "SetStyle(TextSize)\nSet the style of the text. The default style is Regular.\n	TextSize : New text style, (combination of Style values)"},
	{"GetStyle", (PyCFunction)PySfString_GetStyle, METH_NOARGS, "GetStyle()\nGet the style of the text."},
	{"GetRect", (PyCFunction)PySfString_GetRect, METH_NOARGS, "GetRect()\nGet the string rectangle on screen."},
	{NULL}  /* Sentinel */
};

PyTypeObject PySfStringType = {
	PyObject_HEAD_INIT(NULL)
	0,						/*ob_size*/
	"String",				/*tp_name*/
	sizeof(PySfString),		/*tp_basicsize*/
	0,						/*tp_itemsize*/
	(destructor)PySfString_dealloc, /*tp_dealloc*/
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
	"sf.String defines a graphical 2D text, that can be drawn on screen.\n\
Default constructor : String ()\nConstruct the string from a utf-8 or a utf-16 string : String(Text, Font=sf.Font.GetDefaultFont(), Size=30.)\n	Text : Text assigned to the string\n	Font : Font used to draw the string (SFML built-in font by default)\n	Size : Characters size (30 by default)", /* tp_doc */
	0,						/* tp_traverse */
	0,						/* tp_clear */
	0,						/* tp_richcompare */
	0,						/* tp_weaklistoffset */
	0,						/* tp_iter */
	0,						/* tp_iternext */
	PySfString_methods,		/* tp_methods */
	PySfString_members,		/* tp_members */
	0,						/* tp_getset */
	&PySfDrawableType,		/* tp_base */
	0,						/* tp_dict */
	0,						/* tp_descr_get */
	0,						/* tp_descr_set */
	0,						/* tp_dictoffset */
	(initproc)PySfString_init, /* tp_init */
	0,						/* tp_alloc */
	PySfString_new,			/* tp_new */
};



void PySfString_InitConst()
{
	PyObject *obj;
	obj = PyInt_FromLong(sf::String::Regular);
	PyDict_SetItemString(PySfStringType.tp_dict, "Regular", obj);
	Py_DECREF(obj);
	obj = PyInt_FromLong(sf::String::Bold);
	PyDict_SetItemString(PySfStringType.tp_dict, "Bold", obj);
	Py_DECREF(obj);
	obj = PyInt_FromLong(sf::String::Italic);
	PyDict_SetItemString(PySfStringType.tp_dict, "Italic", obj);
	Py_DECREF(obj);
	obj = PyInt_FromLong(sf::String::Underlined);
	PyDict_SetItemString(PySfStringType.tp_dict, "Underlined", obj);
	Py_DECREF(obj);
}

