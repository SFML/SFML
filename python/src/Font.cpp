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
PySfFont_LoadFromFile( PySfFont* self, PyObject *args ) {
	char*  Filename;
	bool   result;

	if( PyArg_ParseTuple( args, "s:Font.LoadFromFile", &Filename ) ) {
		result = self->obj->LoadFromFile(Filename);
	}
	else {
		PyErr_BadArgument();
		return NULL;
	}

	return PyBool_FromLong( result );
}

static PyObject *
PySfFont_LoadFromMemory( PySfFont* self, PyObject *args ) {
	unsigned int  Size;
	char*         Data;
	bool          result;

	if( PyArg_ParseTuple( args, "s#:Font.LoadFromMemory", &Data, &Size ) ) {
		result = self->obj->LoadFromMemory( Data, Size );
	}
	else {
		PyErr_BadArgument();
		return NULL;
	}

	return PyBool_FromLong( result );
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
PySfFont_GetGlyph(PySfFont* self, PyObject *args) {
	unsigned int  codepoint( 0 );
	unsigned int  charsize( 0 );
	bool          bold( false );

	if( !PyArg_ParseTuple( args, "IIb:Font.LoadFromFile", &codepoint, &charsize, &bold ) ) {
		PyErr_BadArgument();
		return NULL;
	}

	PySfGlyph*  glyph( GetNewPySfGlyph() );

	glyph->Owner = false;
	glyph->Rectangle = GetNewPySfIntRect();
	glyph->Rectangle->Owner = false;
	glyph->TexCoords = GetNewPySfFloatRect();
	glyph->TexCoords->Owner = false;

	glyph->obj = const_cast<sf::Glyph*>( &( self->obj->GetGlyph( codepoint, charsize, bold ) ) );
	glyph->Rectangle->obj = &glyph->obj->Rectangle;
	glyph->TexCoords->obj = &glyph->obj->TexCoords;

	PySfGlyphUpdateSelf( glyph );

	return reinterpret_cast<PyObject*>( glyph );
}

static PyObject *
PySfFont_GetImage( PySfFont* self, PyObject* args ) {
	PySfImage*  image( GetNewPySfImage() );

	image->obj = new sf::Image( self->obj->GetImage( PyLong_AsUnsignedLong( args ) ) );

	return reinterpret_cast<PyObject*>( image );
}

static PyMethodDef PySfFont_methods[] = {
	{"LoadFromFile", (PyCFunction)PySfFont_LoadFromFile, METH_VARARGS, "LoadFromFile(Filename))\n\
Load the font from a file. Returns True if loading was successful.\n\
	Filename : Font file to load"},
	{"LoadFromMemory", (PyCFunction)PySfFont_LoadFromMemory, METH_VARARGS, "LoadFromMemory(Data)\n\
Load the font from a file in memory. Returns True if loading was successful.\n\
	Data : data to load"},
	{"GetDefaultFont", (PyCFunction)PySfFont_GetDefaultFont, METH_NOARGS | METH_STATIC, "GetDefaultFont()\n\
Get the SFML default built-in font (Arial)."},
	{"GetImage", (PyCFunction)PySfFont_GetImage, METH_O, "GetImage(characterSize)\n\
Get the image containing the rendered characters (glyphs).\n\
	characterSize: Character size."},
	{"GetGlyph", (PyCFunction)PySfFont_GetGlyph, METH_VARARGS, "GetGlyph(codePoint, characterSize, bold)\n\
Get the description of a glyph (character) given by its code point, character size and boldness. Returns glyph's visual settings, or an invalid glyph if character not found.\n\
	codePoint : Unicode code point value of the character to get.\n\
	characterSize: Size of character\n\
	bold: Bold character"},
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


