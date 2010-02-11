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

#include "Text.hpp"
#include "Font.hpp"
#include "Color.hpp"
#include "Rect.hpp"

#include "compat.hpp"


extern PyTypeObject PySfDrawableType;
extern PyTypeObject PySfFontType;


static void
PySfText_dealloc(PySfText *self)
{
	Py_CLEAR(self->font);
	delete self->obj;
	free_object(self);
}

static PyObject *
PySfText_new(PyTypeObject *type, PyObject *args, PyObject *kwds)
{
	PySfText *self;
	self = (PySfText *)type->tp_alloc(type, 0);
	if (self != NULL)
	{
		self->font = NULL;
		self->IsCustom = false;
		self->obj = new sf::Text();
	}
	return (PyObject *)self;
}

static PyObject *
PySfText_SetString(PySfText* self, PyObject *args)
{
	char *Text, *EncodingStr=NULL;
	int Length;
	std::string Encoding;
	if (PyArg_ParseTuple(args, "u:Text.SetString", &Text))
	{
#if Py_UNICODE_SIZE == 4
		self->obj->SetString((sf::Uint32 *)Text);
#else
		self->obj->SetString((sf::Uint16 *)Text);
#endif
	}
	else if (PyArg_ParseTuple(args, "s|#s:Text.SetString", &Text, &Length, &EncodingStr))
	{
		PyErr_Clear();
		if (EncodingStr == NULL)
			self->obj->SetString(Text);
		else
		{
			Encoding.assign(EncodingStr);
			if (Encoding == "utf8")
				self->obj->SetString(Text);
			else if (Encoding == "utf16")
				self->obj->SetString(Text+2);
			else if (Encoding == "utf32")
				self->obj->SetString(Text+4);
			else
			{
				PyErr_Format(PyExc_TypeError, "Text.SetString() Encoding %s not supported", EncodingStr);
				return NULL;
			}
		}
	}
	else
	{
		PyErr_BadArgument();
		return NULL;
	}
	Py_RETURN_NONE;
}

static PyObject *
PySfText_SetFont(PySfText* self, PyObject *args)
{
	PySfFont *Font = (PySfFont *)args;
	if (!PyObject_TypeCheck(Font, &PySfFontType))
	{
		PyErr_SetString(PyExc_ValueError, "Text.SetFont() Argument must be a sf.Font");
		return NULL;
	}
	Py_CLEAR(self->font);
	Py_INCREF(args);
	self->font = Font;
	self->obj->SetFont(*(Font->obj));
	Py_RETURN_NONE;
}

static PyObject *
PySfText_SetCharacterSize(PySfText* self, PyObject *args)
{
	self->obj->SetCharacterSize(PyFloat_AsDouble(args));
	Py_RETURN_NONE;
}

static PyObject *
PySfText_GetCharacterSize(PySfText* self)
{
	return PyFloat_FromDouble(self->obj->GetCharacterSize());
}

static PyObject *
PySfText_SetStyle(PySfText* self, PyObject *args)
{
	self->obj->SetStyle(PyLong_AsUnsignedLong(args));
	Py_RETURN_NONE;
}

static PyObject *
PySfText_GetStyle(PySfText* self)
{
	return PyLong_FromUnsignedLong(self->obj->GetStyle());
}

static PyObject *
PySfText_GetString(PySfText* self)
{
#if Py_UNICODE_SIZE == 4
	const sf::String& Text(self->obj->GetString());
#else
	const sf::String& Text(self->obj->GetString());
#endif
	return PyUnicode_FromUnicode((const Py_UNICODE*)Text.ToAnsiString().c_str(), Text.GetSize());
}

static PyObject *
PySfText_GetFont(PySfText* self)
{
	if (self->font == NULL)
	{
		PySfFont *Font = GetNewPySfFont();
		Font->obj = (sf::Font *)&(sf::Font::GetDefaultFont());
		Font->Owner = false;
		return (PyObject *)Font;
	}
	else
	{
		Py_INCREF(self->font);
		return (PyObject *)(self->font);
	}
}

static PyObject *
PySfText_GetRect(PySfText* self)
{
	PySfFloatRect *Rect;

	Rect = GetNewPySfFloatRect();
	Rect->Owner = true;
	Rect->obj = new sf::FloatRect (self->obj->GetRect());
	PySfFloatRectUpdateSelf(Rect);

	return (PyObject *)Rect;
}

static PyObject *
PySfText_GetCharacterPos(PySfText* self, PyObject *args)
{
	sf::Vector2f Pos = self->obj->GetCharacterPos(PyLong_AsUnsignedLong(args));
	return Py_BuildValue("ff", Pos.x, Pos.y);
}

static int
PySfText_init(PySfText *self, PyObject *args, PyObject *kwds)
{
	const char *kwlist[] = {"Text", "Font", "Size", NULL};
	float Size = 30.f;
	PyObject *Text=NULL;
	PySfFont *Font = NULL;
	if (!PyArg_ParseTupleAndKeywords(args, kwds, "|OO!f:Text.__new__", (char **)kwlist, &Text, &PySfFontType, &Font, &Size))
		return -1;

	if (Text != NULL)
	{
		if (PyUnicode_Check(Text))
		{
#if Py_UNICODE_SIZE == 4
			self->obj->SetString((sf::Uint32 *)PyUnicode_AS_UNICODE(Text));
#else
			self->obj->SetString((sf::Uint16 *)PyUnicode_AS_UNICODE(Text));
#endif
		}
#ifdef IS_PY3K
		else if (PyBytes_Check(Text))
			self->obj->SetString(PyBytes_AsString(Text));
#else
		else if (PyString_Check(Text))
			self->obj->SetString(PyString_AsString(Text));
#endif
		else
		{
			PyErr_SetString(PyExc_TypeError, "Text.__init__() first argument must be str");
			return -1;
		}
	}
	if (Font) PySfText_SetFont(self, (PyObject *)Font);
	self->obj->SetCharacterSize(Size);
	return 0;
}


static PyMethodDef PySfText_methods[] = {
	{"GetCharacterPos", (PyCFunction)PySfText_GetCharacterPos, METH_O, "GetCharacterPos(Index)\n\
Return the visual position (a tuple of two floats) of the Index-th character of the string, in coordinates relative to the string (note : translation, center, rotation and scale are not applied)\n\
	Index : Index of the character"},
	{"SetString", (PyCFunction)PySfText_SetString, METH_VARARGS, "SetString(UnicodeText) or SetString(Text, Encoding='utf8')\nSet the text. Valid encodings are 'utf8', 'utf16' and 'utf32'.\n	Text : New text"},
	{"GetString", (PyCFunction)PySfText_GetString, METH_NOARGS, "GetString()\nGet the text as an unicode string."},
	{"SetFont", (PyCFunction)PySfText_SetFont, METH_O, "SetFont(Font)\nSet the font of the string.\n	Font : font to use"},
	{"GetFont", (PyCFunction)PySfText_GetFont, METH_NOARGS, "GetFont()\nGet the font used by the string."},
	{"SetCharacterSize", (PyCFunction)PySfText_SetCharacterSize, METH_O, "SetCharacterSize(Size)\nSet the size of the text.\n	Size : New size, in pixels"},
	{"GetCharacterSize", (PyCFunction)PySfText_GetCharacterSize, METH_NOARGS, "GetCharacterSize()\nGet the size of the characters."},
	{"SetStyle", (PyCFunction)PySfText_SetStyle, METH_O, "SetStyle(TextSize)\nSet the style of the text. The default style is Regular.\n	TextSize : New text style, (combination of Style values)"},
	{"GetStyle", (PyCFunction)PySfText_GetStyle, METH_NOARGS, "GetStyle()\nGet the style of the text."},
	{"GetRect", (PyCFunction)PySfText_GetRect, METH_NOARGS, "GetRect()\nGet the string rectangle on screen."},
	{NULL}  /* Sentinel */
};

PyTypeObject PySfTextType = {
	head_init
	"Text",				/*tp_name*/
	sizeof(PySfText),		/*tp_basicsize*/
	0,						/*tp_itemsize*/
	(destructor)PySfText_dealloc, /*tp_dealloc*/
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
	"sf.Text defines a graphical 2D text, that can be drawn on screen.\n\
Default constructor : Text ()\nConstruct the string from an unicode or an ascii string : Text(Text, Font=sf.Font.GetDefaultFont(), Size=30.)\n	Text : Text assigned to the string\n	Font : Font used to draw the string (SFML built-in font by default)\n	Size : Characters size (30 by default)", /* tp_doc */
	0,						/* tp_traverse */
	0,						/* tp_clear */
	0,						/* tp_richcompare */
	0,						/* tp_weaklistoffset */
	0,						/* tp_iter */
	0,						/* tp_iternext */
	PySfText_methods,		/* tp_methods */
	0,						/* tp_members */
	0,						/* tp_getset */
	&PySfDrawableType,		/* tp_base */
	0,						/* tp_dict */
	0,						/* tp_descr_get */
	0,						/* tp_descr_set */
	0,						/* tp_dictoffset */
	(initproc)PySfText_init, /* tp_init */
	0,						/* tp_alloc */
	PySfText_new,			/* tp_new */
};



void PySfText_InitConst()
{
	PyObject *obj;
	obj = PyLong_FromLong(sf::Text::Regular);
	PyDict_SetItemString(PySfTextType.tp_dict, "Regular", obj);
	Py_DECREF(obj);
	obj = PyLong_FromLong(sf::Text::Bold);
	PyDict_SetItemString(PySfTextType.tp_dict, "Bold", obj);
	Py_DECREF(obj);
	obj = PyLong_FromLong(sf::Text::Italic);
	PyDict_SetItemString(PySfTextType.tp_dict, "Italic", obj);
	Py_DECREF(obj);
	obj = PyLong_FromLong(sf::Text::Underlined);
	PyDict_SetItemString(PySfTextType.tp_dict, "Underlined", obj);
	Py_DECREF(obj);
}

