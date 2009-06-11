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

#include "Font.hpp"
#include "Glyph.hpp"
#include "Image.hpp"

#include "compat.hpp"


static void
PySfFont_dealloc(PySfFont *self)
{
	if (self->Owner)
		delete self->obj;
	free_object(self);
}

static PyObject *
PySfFont_new(PyTypeObject *type, PyObject *args, PyObject *kwds)
{
	PySfFont *self;
	self = (PySfFont *)type->tp_alloc(type, 0);
	if (self != NULL)
	{
		self->Owner = true;
		self->obj = new sf::Font();
	}
	return (PyObject *)self;
}

static PyObject *
PySfFont_LoadFromFile(PySfFont* self, PyObject *args, PyObject *kwds)
{
	const char *kwlist[] = {"Filename", "Charsize", "Charset", NULL};
	unsigned int Charsize=30;
	char *Filename;
	char *Charset=NULL, *EncodingStr;
	int Length;
	bool result;
	std::string Encoding;
	if (PyArg_ParseTuple(args, "s|I:Font.LoadFromFile", &Filename, &Charsize))
		result = self->obj->LoadFromFile(Filename, Charsize);
	else if (PyArg_ParseTupleAndKeywords(args, kwds, "s|Iu:Font.LoadFromFile", (char **)kwlist, &Filename, &Charsize, &Charset))
	{
		PyErr_Clear();
#if Py_UNICODE_SIZE == 4
		result = self->obj->LoadFromFile(Filename, Charsize, (sf::Uint32 *)Charset);
#else
		result = self->obj->LoadFromFile(Filename, Charsize, (sf::Uint16 *)Charset);
#endif
	}
	else if (PyArg_ParseTupleAndKeywords(args, kwds, "s|Is#s:Font.LoadFromFile", (char **)kwlist, &Filename, &Charsize, &Charset, &Length, &EncodingStr))
	{
		PyErr_Clear();
		if (EncodingStr == NULL)
			result = self->obj->LoadFromFile(Filename, Charsize, sf::Unicode::UTF8String((sf::Uint8 *)Charset));
		else
		{
			Encoding.assign(EncodingStr);
			if (Encoding == "utf8" || Encoding == "")
				result = self->obj->LoadFromFile(Filename, Charsize, sf::Unicode::UTF8String((sf::Uint8 *)Charset));
			else if (Encoding == "utf16")
				result = self->obj->LoadFromFile(Filename, Charsize, sf::Unicode::UTF16String((sf::Uint16 *)(Charset+2)));
			else if (Encoding == "utf32")
				result = self->obj->LoadFromFile(Filename, Charsize, sf::Unicode::UTF32String((sf::Uint32 *)(Charset+4)));
			else
			{
				PyErr_Format(PyExc_TypeError, "Font.LoadFromFile() Encoding %s not supported", EncodingStr);
				return NULL;
			}
		}
	}
	else
	{
		PyErr_BadArgument();
		return NULL;
	}
	return PyBool_FromLong(result);
}

static PyObject *
PySfFont_LoadFromMemory(PySfFont* self, PyObject *args, PyObject *kwds)
{
	const char *kwlist[] = {"Data", "Charsize", "Charset", NULL};
	unsigned int Charsize=30, Size;
	char *Data;
	char *Charset=NULL, *EncodingStr;
	int Length;
	bool result;
	std::string Encoding;
	if (PyArg_ParseTuple(args, "s#|I:Font.LoadFromMemory", &Data, &Size, &Charsize))
		result = self->obj->LoadFromMemory(Data, Size, Charsize);
	else if (PyArg_ParseTupleAndKeywords(args, kwds, "s#|Iu:Font.LoadFromMemory", (char **)kwlist, &Data, &Size, &Charsize, &Charset))
	{
		PyErr_Clear();
#if Py_UNICODE_SIZE == 4
		result = self->obj->LoadFromMemory(Data, Size, Charsize, (sf::Uint32 *)Charset);
#else
		result = self->obj->LoadFromMemory(Data, Size, Charsize, (sf::Uint16 *)Charset);
#endif
	}
	else if (PyArg_ParseTupleAndKeywords(args, kwds, "s#|Is#s:Font.LoadFromMemory", (char **)kwlist, &Data, &Size, &Charsize, &Charset, &Length, &EncodingStr))
	{
		PyErr_Clear();
		if (EncodingStr == NULL)
			result = self->obj->LoadFromMemory(Data, Size, Charsize, sf::Unicode::UTF8String((sf::Uint8 *)Charset));
		else
		{
			Encoding.assign(EncodingStr);
			if (Encoding == "utf8")
				result = self->obj->LoadFromMemory(Data, Size, Charsize, sf::Unicode::UTF8String((sf::Uint8 *)Charset));
			else if (Encoding == "utf16")
				result = self->obj->LoadFromMemory(Data, Size, Charsize, sf::Unicode::UTF16String((sf::Uint16 *)(Charset+2)));
			else if (Encoding == "utf32")
				result = self->obj->LoadFromMemory(Data, Size, Charsize, sf::Unicode::UTF32String((sf::Uint32 *)(Charset+4)));
			else
			{
				PyErr_Format(PyExc_TypeError, "Font.LoadFromMemory() Encoding %s not supported", EncodingStr);
				return NULL;
			}
		}
	}
	else
	{
		PyErr_BadArgument();
		return NULL;
	}
	return PyBool_FromLong(result);
}

static PyObject *
PySfFont_GetDefaultFont(PySfFont* self, PyObject *args)
{
	PySfFont *DefaultFont = GetNewPySfFont();
	DefaultFont->Owner = false;
	DefaultFont->obj = (sf::Font *)&(sf::Font::GetDefaultFont());
	return (PyObject *)DefaultFont;
}

static PyObject *
PySfFont_GetCharacterSize(PySfFont* self)
{
	return PyLong_FromUnsignedLong(self->obj->GetCharacterSize());
}

static PyObject *
PySfFont_GetGlyph(PySfFont* self, PyObject *args)
{
	PySfGlyph *Glyph = GetNewPySfGlyph();
	Glyph->Owner = false;
	Glyph->Rectangle = GetNewPySfIntRect();
	Glyph->Rectangle->Owner = false;
	Glyph->TexCoords = GetNewPySfFloatRect();
	Glyph->TexCoords->Owner = false;
	Glyph->obj = (sf::Glyph *) &(self->obj->GetGlyph(PyLong_AsUnsignedLong(args)));
	Glyph->Rectangle->obj = &(Glyph->obj->Rectangle);
	Glyph->TexCoords->obj = &(Glyph->obj->TexCoords);
	PySfGlyphUpdateSelf(Glyph);
	return (PyObject *)Glyph;
}

static PyObject *
PySfFont_GetImage(PySfFont* self)
{
	PySfImage *Image;
	Image = GetNewPySfImage();
	Image->obj = new sf::Image(self->obj->GetImage());
	return (PyObject *)Image;
}

static PyMethodDef PySfFont_methods[] = {
	{"LoadFromFile", (PyCFunction)PySfFont_LoadFromFile, METH_VARARGS | METH_KEYWORDS, "LoadFromFile(Filename, CharSize, UnicodeCharset) or LoadFromFile(Filename, CharSize, Charset, Encoding='utf8')\n\
Load the font from a file. Returns True if loading was successful.\n\
	Filename : Font file to load\n\
	CharSize : Size of characters in bitmap - the bigger, the higher quality (30 by default)\n\
	Charset : Characters set to generate (by default, contains the ISO-8859-1 printable characters)"},
	{"LoadFromMemory", (PyCFunction)PySfFont_LoadFromMemory, METH_VARARGS | METH_KEYWORDS, "LoadFromMemory(Data, CharSize, UnicodeCharset) or LoadFromMemory(Data, CharSize, Charset, Encoding='utf8')\n\
Load the font from a file in memory. Returns True if loading was successful.\n\
	Data : data to load\n\
	CharSize : Size of characters in bitmap - the bigger, the higher quality (30 by default)\n\
	Charset : Characters set to generate (by default, contains the ISO-8859-1 printable characters)"},
	{"GetDefaultFont", (PyCFunction)PySfFont_GetDefaultFont, METH_NOARGS | METH_STATIC, "GetDefaultFont()\n\
Get the SFML default built-in font (Arial)."},
	{"GetImage", (PyCFunction)PySfFont_GetImage, METH_NOARGS, "GetImage()\n\
Get the image containing the rendered characters (glyphs)."},
	{"GetCharacterSize", (PyCFunction)PySfFont_GetCharacterSize, METH_NOARGS, "GetCharacterSize()\n\
Get the base size of characters in the font; All glyphs dimensions are based on this value"},
	{"GetGlyph", (PyCFunction)PySfFont_GetGlyph, METH_O, "GetGlyph(CodePoint)\n\
Get the description of a glyph (character) given by its unicode value. Returns glyph's visual settings, or an invalid glyph if character not found.\n\
	CodePoint : Unicode value of the character to get."},
	{NULL}  /* Sentinel */
};


PyTypeObject PySfFontType = {
	head_init
	"Font",					/*tp_name*/
	sizeof(PySfFont),		/*tp_basicsize*/
	0,						/*tp_itemsize*/
	(destructor)PySfFont_dealloc, /*tp_dealloc*/
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
	"sf.Font is the low-level class for loading and manipulating character fonts. This class is meant to be used by sf.String.\nDefault constructor : sf.Font().", /* tp_doc */
	0,						/* tp_traverse */
	0,						/* tp_clear */
	0,						/* tp_richcompare */
	0,						/* tp_weaklistoffset */
	0,						/* tp_iter */
	0,						/* tp_iternext */
	PySfFont_methods,		/* tp_methods */
	0,						/* tp_members */
	0,						/* tp_getset */
	0,						/* tp_base */
	0,						/* tp_dict */
	0,						/* tp_descr_get */
	0,						/* tp_descr_set */
	0,						/* tp_dictoffset */
	0,						/* tp_init */
	0,						/* tp_alloc */
	PySfFont_new,			/* tp_new */
};

PySfFont *
GetNewPySfFont()
{
	return PyObject_New(PySfFont, &PySfFontType);
}


