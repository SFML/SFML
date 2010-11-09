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

#include "Event.hpp"

#include <structmember.h>

#include "compat.hpp"

////////////////////////////////
// Text Events Parameters 
////////////////////////////////

PyMemberDef PySfEventText_members[] = {
	{(char *)"Unicode", T_USHORT, offsetof(PySfEventText, Unicode), READONLY, (char *)""},
	{NULL}  /* Sentinel */
};

static PyObject *
PySfEventText_new(PyTypeObject *type, PyObject *args, PyObject *kwds)
{
	PySfEventText *self;

	self = (PySfEventText *)type->tp_alloc(type, 0);
	if (self != NULL)
	{
		self->Unicode = 0;
	}

	return (PyObject *)self;
}

void
PySfEventText_dealloc(PySfEventText* self)
{
	free_object(self);
}

PyTypeObject PySfEventTextType = {
	head_init
	"Event.Text",			/*tp_name*/
	sizeof(PySfEventText),	/*tp_basicsize*/
	0,						/*tp_itemsize*/
	(destructor)PySfEventText_dealloc, /*tp_dealloc*/
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
	Py_TPFLAGS_DEFAULT,		/*tp_flags*/
	"Text Events Parameters", /* tp_doc */
	0,						/* tp_traverse */
	0,						/* tp_clear */
	0,						/* tp_richcompare */
	0,						/* tp_weaklistoffset */
	0,						/* tp_iter */
	0,						/* tp_iternext */
	0,						/* tp_methods */
	PySfEventText_members,	/* tp_members */
	0,						/* tp_getset */
	0,						/* tp_base */
	0,						/* tp_dict */
	0,						/* tp_descr_get */
	0,						/* tp_descr_set */
	0,						/* tp_dictoffset */
	0,						/* tp_init */
	0,						/* tp_alloc */
	PySfEventText_new,		/* tp_new */
};



/////////////////////////////////////
// Keyboard Events Parameters
/////////////////////////////////////

static PyObject *
PySfEventKey_new(PyTypeObject *type, PyObject *args, PyObject *kwds)
{
	PySfEventKey *self;

	self = (PySfEventKey *)type->tp_alloc(type, 0);
	if (self != NULL)
	{
		Py_INCREF(Py_False);
		self->Alt = Py_False;
		Py_INCREF(Py_False);
		self->Control = Py_False;
		Py_INCREF(Py_False);
		self->Shift = Py_False;
	}

	return (PyObject *)self;
}

void
PySfEventKey_dealloc(PySfEventKey* self)
{
	free_object(self);
}

PyMemberDef PySfEventKey_members[] = {
	{(char *)"Alt", T_OBJECT, offsetof(PySfEventKey, Alt), READONLY, (char *)""},
	{(char *)"Control", T_OBJECT, offsetof(PySfEventKey, Control), READONLY, (char *)""},
	{(char *)"Shift", T_OBJECT, offsetof(PySfEventKey, Shift), READONLY, (char *)""},
	{(char *)"Code", T_UINT, offsetof(PySfEventKey, Code), READONLY, (char *)""},
	{NULL}  /* Sentinel */
};

PyTypeObject PySfEventKeyType = {
	head_init
	"Event.Key",			/*tp_name*/
	sizeof(PySfEventKey),	/*tp_basicsize*/
	0,						/*tp_itemsize*/
	(destructor)PySfEventKey_dealloc, /*tp_dealloc*/
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
	Py_TPFLAGS_DEFAULT,		/*tp_flags*/
	"Key Events Parameters", /* tp_doc */
	0,						/* tp_traverse */
	0,						/* tp_clear */
	0,						/* tp_richcompare */
	0,						/* tp_weaklistoffset */
	0,						/* tp_iter */
	0,						/* tp_iternext */
	0,						/* tp_methods */
	PySfEventKey_members,	/* tp_members */
	0,						/* tp_getset */
	0,						/* tp_base */
	0,						/* tp_dict */
	0,						/* tp_descr_get */
	0,						/* tp_descr_set */
	0,						/* tp_dictoffset */
	0,						/* tp_init */
	0,						/* tp_alloc */
	PySfEventKey_new,		/* tp_new */
};


////////////////////////////////////
// MouseMove Events Parameters
////////////////////////////////////

static PyObject *
PySfEventMouseMove_new(PyTypeObject *type, PyObject *args, PyObject *kwds)
{
	PySfEventMouseMove *self;

	self = (PySfEventMouseMove *)type->tp_alloc(type, 0);
	if (self != NULL)
	{
		self->X = 0;
		self->Y = 0;
	}

	return (PyObject *)self;
}

void
PySfEventMouseMove_dealloc(PySfEventMouseMove *self)
{
	free_object(self);
}


PyMemberDef PySfEventMouseMove_members[] = {
	{(char *)"X", T_INT, offsetof(PySfEventMouseMove, X), READONLY, (char *)""},
	{(char *)"Y", T_INT, offsetof(PySfEventMouseMove, Y), READONLY, (char *)""},
	{NULL}  /* Sentinel */
};

PyTypeObject PySfEventMouseMoveType = {
	head_init
	"Event.MouseMove",		/*tp_name*/
	sizeof(PySfEventMouseMove), /*tp_basicsize*/
	0,						/*tp_itemsize*/
	(destructor)PySfEventMouseMove_dealloc, /*tp_dealloc*/
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
	Py_TPFLAGS_DEFAULT,		/*tp_flags*/
	"MouseMove Events Parameters", /* tp_doc */
	0,						/* tp_traverse */
	0,						/* tp_clear */
	0,						/* tp_richcompare */
	0,						/* tp_weaklistoffset */
	0,						/* tp_iter */
	0,						/* tp_iternext */
	0,						/* tp_methods */
	PySfEventMouseMove_members,	/* tp_members */
	0,						/* tp_getset */
	0,						/* tp_base */
	0,						/* tp_dict */
	0,						/* tp_descr_get */
	0,						/* tp_descr_set */
	0,						/* tp_dictoffset */
	0,						/* tp_init */
	0,						/* tp_alloc */
	PySfEventMouseMove_new,	/* tp_new */
};


////////////////////////////////////
// MouseButton Events Parameters
////////////////////////////////////

static PyObject *
PySfEventMouseButton_new(PyTypeObject *type, PyObject *args, PyObject *kwds)
{
	PySfEventMouseButton *self;

	self = (PySfEventMouseButton *)type->tp_alloc(type, 0);
	if (self != NULL)
	{
		self->Button = 0;
		self->X = 0;
		self->Y = 0;
	}

	return (PyObject *)self;
}

void
PySfEventMouseButton_dealloc(PySfEventMouseButton* self)
{
	free_object(self);
}


PyMemberDef PySfEventMouseButton_members[] = {
	{(char *)"Button", T_UINT, offsetof(PySfEventMouseButton, Button), READONLY, (char *)""},
	{(char *)"X", T_INT, offsetof(PySfEventMouseButton, X), READONLY, (char *)""},
	{(char *)"Y", T_INT, offsetof(PySfEventMouseButton, Y), READONLY, (char *)""},
	{NULL}  /* Sentinel */
};

PyTypeObject PySfEventMouseButtonType = {
	head_init
	"Event.MouseButton",	/*tp_name*/
	sizeof(PySfEventMouseButton), /*tp_basicsize*/
	0,						/*tp_itemsize*/
	(destructor)PySfEventMouseButton_dealloc, /*tp_dealloc*/
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
	Py_TPFLAGS_DEFAULT,		/*tp_flags*/
	"MouseButton Events Parameters", /* tp_doc */
	0,						/* tp_traverse */
	0,						/* tp_clear */
	0,						/* tp_richcompare */
	0,						/* tp_weaklistoffset */
	0,						/* tp_iter */
	0,						/* tp_iternext */
	0,						/* tp_methods */
	PySfEventMouseButton_members, /* tp_members */
	0,						/* tp_getset */
	0,						/* tp_base */
	0,						/* tp_dict */
	0,						/* tp_descr_get */
	0,						/* tp_descr_set */
	0,						/* tp_dictoffset */
	0,						/* tp_init */
	0,						/* tp_alloc */
	PySfEventMouseButton_new, /* tp_new */
};


////////////////////////////////
// MouseWheel Events Parameters 
////////////////////////////////

static PyObject *
PySfEventMouseWheel_new(PyTypeObject *type, PyObject *args, PyObject *kwds)
{
	PySfEventMouseWheel *self;

	self = (PySfEventMouseWheel *)type->tp_alloc(type, 0);
	if (self != NULL)
	{
		self->Delta = 0;
	}

	return (PyObject *)self;
}

void
PySfEventMouseWheel_dealloc(PySfEventMouseWheel* self)
{
	free_object(self);
}

PyMemberDef PySfEventMouseWheel_members[] = {
	{(char *)"Delta", T_INT, offsetof(PySfEventMouseWheel,Delta), READONLY, (char *)""},
	{NULL}  /* Sentinel */
};

PyTypeObject PySfEventMouseWheelType = {
	head_init
	"Event.MouseWheel",		/*tp_name*/
	sizeof(PySfEventMouseWheel), /*tp_basicsize*/
	0,						/*tp_itemsize*/
	(destructor)PySfEventMouseWheel_dealloc, /*tp_dealloc*/
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
	Py_TPFLAGS_DEFAULT,		/*tp_flags*/
	"MouseWheel Events Parameters", /* tp_doc */
	0,						/* tp_traverse */
	0,						/* tp_clear */
	0,						/* tp_richcompare */
	0,						/* tp_weaklistoffset */
	0,						/* tp_iter */
	0,						/* tp_iternext */
	0,						/* tp_methods */
	PySfEventMouseWheel_members, /* tp_members */
	0,						/* tp_getset */
	0,						/* tp_base */
	0,						/* tp_dict */
	0,						/* tp_descr_get */
	0,						/* tp_descr_set */
	0,						/* tp_dictoffset */
	0,						/* tp_init */
	0,						/* tp_alloc */
	PySfEventMouseWheel_new, /* tp_new */
};


////////////////////////////////////
// JoyMove Events Parameters
////////////////////////////////////

static PyObject *
PySfEventJoyMove_new(PyTypeObject *type, PyObject *args, PyObject *kwds)
{
	PySfEventJoyMove *self;

	self = (PySfEventJoyMove *)type->tp_alloc(type, 0);
	if (self != NULL)
	{
		self->JoystickId = 0;
		self->Axis = 0;
		self->Position = 0.f;
	}

	return (PyObject *)self;
}

void
PySfEventJoyMove_dealloc(PySfEventJoyMove* self)
{
	free_object(self);
}


PyMemberDef PySfEventJoyMove_members[] = {
	{(char *)"JoystickId", T_UINT, offsetof(PySfEventJoyMove,JoystickId), READONLY, (char *)""},
	{(char *)"Axis", T_UINT, offsetof(PySfEventJoyMove,Axis), READONLY, (char *)""},
	{(char *)"Position", T_FLOAT, offsetof(PySfEventJoyMove,Position), READONLY, (char *)""},
	{NULL}  /* Sentinel */
};

PyTypeObject PySfEventJoyMoveType = {
	head_init
	"Event.JoyMove",		/*tp_name*/
	sizeof(PySfEventJoyMove), /*tp_basicsize*/
	0,						/*tp_itemsize*/
	(destructor)PySfEventJoyMove_dealloc, /*tp_dealloc*/
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
	Py_TPFLAGS_DEFAULT,		/*tp_flags*/
	"JoyMove Events Parameters", /* tp_doc */
	0,						/* tp_traverse */
	0,						/* tp_clear */
	0,						/* tp_richcompare */
	0,						/* tp_weaklistoffset */
	0,						/* tp_iter */
	0,						/* tp_iternext */
	0,						/* tp_methods */
	PySfEventJoyMove_members, /* tp_members */
	0,						/* tp_getset */
	0,						/* tp_base */
	0,						/* tp_dict */
	0,						/* tp_descr_get */
	0,						/* tp_descr_set */
	0,						/* tp_dictoffset */
	0,						/* tp_init */
	0,						/* tp_alloc */
	PySfEventJoyMove_new,	/* tp_new */
};


////////////////////////////////////
// JoyButton Events Parameters
////////////////////////////////////

static PyObject *
PySfEventJoyButton_new(PyTypeObject *type, PyObject *args, PyObject *kwds)
{
	PySfEventJoyButton *self;

	self = (PySfEventJoyButton *)type->tp_alloc(type, 0);
	if (self != NULL)
	{
		self->JoystickId = 0;
		self->Button = 0;
	}

	return (PyObject *)self;
}

void
PySfEventJoyButton_dealloc(PySfEventJoyButton* self)
{
	free_object(self);
}


PyMemberDef PySfEventJoyButton_members[] = {
	{(char *)"JoystickId", T_UINT, offsetof(PySfEventJoyButton, JoystickId), READONLY, (char *)""},
	{(char *)"Button", T_UINT, offsetof(PySfEventJoyButton, Button), READONLY, (char *)""},
	{NULL}  /* Sentinel */
};

PyTypeObject PySfEventJoyButtonType = {
	head_init
	"Event.JoyButton",		/*tp_name*/
	sizeof(PySfEventJoyButton),	/*tp_basicsize*/
	0,						/*tp_itemsize*/
	(destructor)PySfEventJoyButton_dealloc, /*tp_dealloc*/
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
	Py_TPFLAGS_DEFAULT,		/*tp_flags*/
	"JoyButton Events Parameters", /* tp_doc */
	0,						/* tp_traverse */
	0,						/* tp_clear */
	0,						/* tp_richcompare */
	0,						/* tp_weaklistoffset */
	0,						/* tp_iter */
	0,						/* tp_iternext */
	0,						/* tp_methods */
	PySfEventJoyButton_members, /* tp_members */
	0,						/* tp_getset */
	0,						/* tp_base */
	0,						/* tp_dict */
	0,						/* tp_descr_get */
	0,						/* tp_descr_set */
	0,						/* tp_dictoffset */
	0,						/* tp_init */
	0,						/* tp_alloc */
	PySfEventJoyButton_new,	/* tp_new */
};


////////////////////////////////////
// Size Events Parameters
////////////////////////////////////

static PyObject *
PySfEventSize_new(PyTypeObject *type, PyObject *args, PyObject *kwds)
{
	PySfEventSize *self;

	self = (PySfEventSize *)type->tp_alloc(type, 0);
	if (self != NULL)
	{
		self->Width = 0;
		self->Height = 0;
	}

	return (PyObject *)self;
}

void
PySfEventSize_dealloc(PySfEventSize* self)
{
	free_object(self);
}

PyMemberDef PySfEventSize_members[] = {
	{(char *)"Width", T_UINT, offsetof(PySfEventSize, Width), READONLY, (char *)""},
	{(char *)"Height", T_UINT, offsetof(PySfEventSize, Height), READONLY, (char *)""},
	{NULL}  /* Sentinel */
};

PyTypeObject PySfEventSizeType = {
	head_init
	"Event.Size",			/*tp_name*/
	sizeof(PySfEventSize),	/*tp_basicsize*/
	0,						/*tp_itemsize*/
	(destructor)PySfEventSize_dealloc, /*tp_dealloc*/
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
	Py_TPFLAGS_DEFAULT,		/*tp_flags*/
	"Size Events Parameters", /* tp_doc */
	0,						/* tp_traverse */
	0,						/* tp_clear */
	0,						/* tp_richcompare */
	0,						/* tp_weaklistoffset */
	0,						/* tp_iter */
	0,						/* tp_iternext */
	0,						/* tp_methods */
	PySfEventSize_members,	/* tp_members */
	0,						/* tp_getset */
	0,						/* tp_base */
	0,						/* tp_dict */
	0,						/* tp_descr_get */
	0,						/* tp_descr_set */
	0,						/* tp_dictoffset */
	0,						/* tp_init */
	0,						/* tp_alloc */
	PySfEventSize_new,		/* tp_new */
};






////////////////////////////////////
// sf.Event
////////////////////////////////////


static PyObject *
PySfEvent_new(PyTypeObject *type, PyObject *args, PyObject *kwds)
{
	PySfEvent *self;

	self = (PySfEvent *)type->tp_alloc(type, 0);
	if (self != NULL)
	{
		self->Text = (PySfEventText *)PySfEventText_new(&PySfEventTextType, NULL, NULL);
		self->Key = (PySfEventKey *)PySfEventKey_new(&PySfEventKeyType, NULL, NULL);
		self->MouseMove = (PySfEventMouseMove *)PySfEventMouseMove_new(&PySfEventMouseMoveType, NULL, NULL);
		self->MouseButton = (PySfEventMouseButton *)PySfEventMouseButton_new(&PySfEventMouseButtonType, NULL, NULL);
		self->MouseWheel = (PySfEventMouseWheel *)PySfEventMouseWheel_new(&PySfEventMouseWheelType, NULL, NULL);
		self->JoyMove = (PySfEventJoyMove *)PySfEventJoyMove_new(&PySfEventJoyMoveType, NULL, NULL);
		self->JoyButton = (PySfEventJoyButton *)PySfEventJoyButton_new(&PySfEventJoyButtonType, NULL, NULL);
		self->Size = (PySfEventSize *)PySfEventSize_new(&PySfEventSizeType, NULL, NULL);
		self->obj = new sf::Event();
	}

	return (PyObject *)self;
}

static void
PySfEvent_dealloc(PySfEvent* self)
{
	Py_DECREF(self->Text);
	Py_DECREF(self->Key);
	Py_DECREF(self->MouseMove);
	Py_DECREF(self->MouseButton);
	Py_DECREF(self->MouseWheel);
	Py_DECREF(self->JoyMove);
	Py_DECREF(self->JoyButton);
	Py_DECREF(self->Size);
	delete self->obj;
	free_object(self);
}

static PyMemberDef PySfEvent_members[] = {
	{(char *)"Text", T_OBJECT, offsetof(PySfEvent, Text), READONLY, (char *)"Text Events Parameters"},
	{(char *)"Key", T_OBJECT, offsetof(PySfEvent, Key), READONLY, (char *)"Keyboard Events Parameters"},
	{(char *)"MouseMove", T_OBJECT, offsetof(PySfEvent, MouseMove), READONLY, (char *)"MouseMove Events Parameters"},
	{(char *)"MouseButton", T_OBJECT, offsetof(PySfEvent, MouseButton), READONLY, (char *)"MouseButton Events Parameters"},
	{(char *)"MouseWheel", T_OBJECT, offsetof(PySfEvent, MouseWheel), READONLY, (char *)"MouseWheel Events Parameters"},
	{(char *)"JoyMove", T_OBJECT, offsetof(PySfEvent, JoyMove), READONLY, (char *)"JoyMove Events Parameters"},
	{(char *)"JoyButton", T_OBJECT, offsetof(PySfEvent, JoyButton), READONLY, (char *)"JoyButton Events Parameters"},
	{(char *)"Size", T_OBJECT, offsetof(PySfEvent, Size), READONLY, (char *)"Size Events Parameters"},
	{(char *)"Type", T_UINT, offsetof(PySfEvent, Type), READONLY, (char *)"Type Events Parameters"},
	{NULL}  /* Sentinel */
};

PyTypeObject PySfEventType = {
	head_init
	"Event",				/*tp_name*/
	sizeof(PySfEvent),		/*tp_basicsize*/
	0,						/*tp_itemsize*/
	(destructor)PySfEvent_dealloc, /*tp_dealloc*/
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
	"sf.Event defines a system event and its parameters", /* tp_doc */
	0,						/* tp_traverse */
	0,						/* tp_clear */
	0,						/* tp_richcompare */
	0,						/* tp_weaklistoffset */
	0,						/* tp_iter */
	0,						/* tp_iternext */
	0,						/* tp_methods */
	PySfEvent_members,		/* tp_members */
	0,						/* tp_getset */
	0,						/* tp_base */
	0,						/* tp_dict */
	0,						/* tp_descr_get */
	0,						/* tp_descr_set */
	0,						/* tp_dictoffset */
	0,						/* tp_init */
	0,						/* tp_alloc */
	PySfEvent_new,			/* tp_new */
};


void
PySfEvent_InitConst()
{
	PyObject *obj;
	obj = PyLong_FromLong(sf::Event::KeyReleased);
	PyDict_SetItemString(PySfEventType.tp_dict, "KeyReleased", obj);
	Py_DECREF(obj);
	obj = PyLong_FromLong(sf::Event::LostFocus);
	PyDict_SetItemString(PySfEventType.tp_dict, "LostFocus", obj);
	Py_DECREF(obj);
	obj = PyLong_FromLong(sf::Event::GainedFocus);
	PyDict_SetItemString(PySfEventType.tp_dict, "GainedFocus", obj);
	Py_DECREF(obj);
	obj = PyLong_FromLong(sf::Event::KeyPressed);
	PyDict_SetItemString(PySfEventType.tp_dict, "KeyPressed", obj);
	Py_DECREF(obj);
	obj = PyLong_FromLong(sf::Event::MouseWheelMoved);
	PyDict_SetItemString(PySfEventType.tp_dict, "MouseWheelMoved", obj);
	Py_DECREF(obj);
	obj = PyLong_FromLong(sf::Event::TextEntered);
	PyDict_SetItemString(PySfEventType.tp_dict, "TextEntered", obj);
	Py_DECREF(obj);
	obj = PyLong_FromLong(sf::Event::MouseMoved);
	PyDict_SetItemString(PySfEventType.tp_dict, "MouseMoved", obj);
	Py_DECREF(obj);
	obj = PyLong_FromLong(sf::Event::JoyButtonPressed);
	PyDict_SetItemString(PySfEventType.tp_dict, "JoyButtonPressed", obj);
	Py_DECREF(obj);
	obj = PyLong_FromLong(sf::Event::MouseButtonReleased);
	PyDict_SetItemString(PySfEventType.tp_dict, "MouseButtonReleased", obj);
	Py_DECREF(obj);
	obj = PyLong_FromLong(sf::Event::Closed);
	PyDict_SetItemString(PySfEventType.tp_dict, "Closed", obj);
	Py_DECREF(obj);
	obj = PyLong_FromLong(sf::Event::MouseButtonPressed);
	PyDict_SetItemString(PySfEventType.tp_dict, "MouseButtonPressed", obj);
	Py_DECREF(obj);
	obj = PyLong_FromLong(sf::Event::JoyMoved);
	PyDict_SetItemString(PySfEventType.tp_dict, "JoyMoved", obj);
	Py_DECREF(obj);
	obj = PyLong_FromLong(sf::Event::JoyButtonReleased);
	PyDict_SetItemString(PySfEventType.tp_dict, "JoyButtonReleased", obj);
	Py_DECREF(obj);
	obj = PyLong_FromLong(sf::Event::Resized);
	PyDict_SetItemString(PySfEventType.tp_dict, "Resized", obj);
	Py_DECREF(obj);
	obj = PyLong_FromLong(sf::Event::MouseEntered);
	PyDict_SetItemString(PySfEventType.tp_dict, "MouseEntered", obj);
	Py_DECREF(obj);
	obj = PyLong_FromLong(sf::Event::MouseLeft);
	PyDict_SetItemString(PySfEventType.tp_dict, "MouseLeft", obj);
	Py_DECREF(obj);
}

