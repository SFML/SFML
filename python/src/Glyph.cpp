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

#include "Glyph.hpp"

#include <structmember.h>

#include "offsetof.hpp"
#include "compat.hpp"


static PyMemberDef PySfGlyph_members[] = {
	{(char *)"Advance", T_INT, offsetof(PySfGlyph, Advance), 0, (char *)"Offset to move horizontically to the next character."},
	{(char *)"Rectangle", T_OBJECT, offsetof(PySfGlyph, Rectangle), 0, (char *)"Bounding rectangle of the glyph, in relative coordinates."},
	{(char *)"TexCoords", T_OBJECT, offsetof(PySfGlyph, TexCoords), 0, (char *)"Texture coordinates of the glyph inside the bitmap font."},
	{NULL}  /* Sentinel */
};


static void
PySfGlyph_dealloc(PySfGlyph *self)
{
	self->Rectangle->obj = new sf::IntRect(self->obj->Rectangle);
	Py_DECREF(self->Rectangle);
	self->TexCoords->obj = new sf::FloatRect(self->obj->TexCoords);
	Py_DECREF(self->TexCoords);
	delete self->obj;
	free_object(self);
}

void
PySfGlyphUpdateObj(PySfGlyph *self)
{
	self->obj->Advance = self->Advance;
	PySfIntRectUpdateObj(self->Rectangle);
	PySfFloatRectUpdateObj(self->TexCoords);
	self->obj->Rectangle.Left = self->Rectangle->Left;
	self->obj->Rectangle.Top = self->Rectangle->Top;
	self->obj->Rectangle.Right = self->Rectangle->Right;
	self->obj->Rectangle.Bottom = self->Rectangle->Bottom;
	self->obj->TexCoords.Left = self->TexCoords->Left;
	self->obj->TexCoords.Top = self->TexCoords->Top;
	self->obj->TexCoords.Right = self->TexCoords->Right;
	self->obj->TexCoords.Bottom = self->TexCoords->Bottom;
}

void
PySfGlyphUpdateSelf(PySfGlyph *self)
{
	self->Advance = self->obj->Advance;
	self->Rectangle->Left = self->obj->Rectangle.Left;
	self->Rectangle->Top = self->obj->Rectangle.Top;
	self->Rectangle->Right = self->obj->Rectangle.Right;
	self->Rectangle->Bottom = self->obj->Rectangle.Bottom;
	self->TexCoords->Left = self->obj->TexCoords.Left;
	self->TexCoords->Top = self->obj->TexCoords.Top;
	self->TexCoords->Right = self->obj->TexCoords.Right;
	self->TexCoords->Bottom = self->obj->TexCoords.Bottom;
	PySfIntRectUpdateObj(self->Rectangle);
	PySfFloatRectUpdateObj(self->TexCoords);
}

static PyObject *
PySfGlyph_new(PyTypeObject *type, PyObject *args, PyObject *kwds)
{
	PySfGlyph *self;
	self = (PySfGlyph *)type->tp_alloc(type, 0);
	if (self != NULL)
	{
		self->Advance = 0;
		self->Rectangle = GetNewPySfIntRect();
		self->TexCoords = GetNewPySfFloatRect();
	}
	return (PyObject *)self;
}


static int
PySfGlyph_init(PySfGlyph *self, PyObject *args, PyObject *kwds)
{
	self->obj = new sf::Glyph();
	self->Rectangle->obj = &(self->obj->Rectangle);
	self->TexCoords->obj = &(self->obj->TexCoords);
	return 0;
}


PyTypeObject PySfGlyphType = {
	head_init
	"Glyph",				/*tp_name*/
	sizeof(PySfGlyph),		/*tp_basicsize*/
	0,						/*tp_itemsize*/
	(destructor)PySfGlyph_dealloc, /*tp_dealloc*/
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
	"Structure describing a glyph (a visual character).", /* tp_doc */
	0,						/* tp_traverse */
	0,						/* tp_clear */
	0,						/* tp_richcompare */
	0,						/* tp_weaklistoffset */
	0,						/* tp_iter */
	0,						/* tp_iternext */
	0,						/* tp_methods */
	PySfGlyph_members,		/* tp_members */
	0,						/* tp_getset */
	0,						/* tp_base */
	0,						/* tp_dict */
	0,						/* tp_descr_get */
	0,						/* tp_descr_set */
	0,						/* tp_dictoffset */
	(initproc)PySfGlyph_init, /* tp_init */
	0,						/* tp_alloc */
	PySfGlyph_new,			/* tp_new */
};

PySfGlyph *
GetNewPySfGlyph()
{
	return (PySfGlyph *)PySfGlyph_new(&PySfGlyphType, NULL, NULL);
}

