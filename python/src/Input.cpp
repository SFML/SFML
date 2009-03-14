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

#include "Input.hpp"

#include "compat.hpp"


static PyObject *
PySfInput_new(PyTypeObject *type, PyObject *args, PyObject *kwds)
{
	PyErr_SetString(PyExc_RuntimeError, "You can't create an Input object yourself, because an Input object must always be associated to its window.\nThe only way to get an Input is by creating a window and calling : Input = MyWindow.GetInput().");
	return NULL;
}

static PyObject*
PySfInput_IsKeyDown(PySfInput *self, PyObject *args)
{
	return PyBool_FromLong(self->obj->IsKeyDown( (sf::Key::Code) PyLong_AsLong(args) ));
}

static PyObject*
PySfInput_IsMouseButtonDown(PySfInput *self, PyObject *args)
{
	return PyBool_FromLong(self->obj->IsMouseButtonDown( (sf::Mouse::Button) PyLong_AsLong(args) ));
}

static PyObject*
PySfInput_IsJoystickButtonDown(PySfInput *self, PyObject *args)
{
	unsigned int JoyId, Button;
	if (! PyArg_ParseTuple (args, "II:Input.IsJoystickButtonDown", &JoyId, &Button))
		return NULL;
	return PyBool_FromLong(self->obj->IsJoystickButtonDown(JoyId, Button));
}

static PyObject*
PySfInput_GetMouseX(PySfInput *self)
{
	return PyLong_FromLong(self->obj->GetMouseX());
}
static PyObject*
PySfInput_GetMouseY(PySfInput *self)
{
	return PyLong_FromLong(self->obj->GetMouseY());
}

static PyObject*
PySfInput_GetJoystickAxis(PySfInput *self, PyObject *args)
{
	unsigned int JoyId, Axis;
	if (! PyArg_ParseTuple (args, "II:Input.GetJoystickAxis", &JoyId, &Axis))
		return NULL;
	return PyFloat_FromDouble(self->obj->GetJoystickAxis(JoyId, (sf::Joy::Axis) Axis));
}

static PyMethodDef PySfInput_methods[] = {
	{"IsKeyDown", (PyCFunction)PySfInput_IsKeyDown, METH_O, "IsKeyDown(KeyCode)\nGet the state of a key. Returns True if key is down, false if key is up.\n	KeyCode : Key to check"},
	{"IsMouseButtonDown", (PyCFunction)PySfInput_IsMouseButtonDown, METH_O, "IsMouseButtonDown(Button)\nGet the state of a mouse button. Returns True if button is down, false if button is up.\n	Button : Button to check"},
	{"IsJoystickButtonDown", (PyCFunction)PySfInput_IsJoystickButtonDown, METH_VARARGS, "IsJoystickButtonDown(JoyId, Button)\nGet the state of a joystick button. Returns True if button is down, false if button is up.\n	JoyId 	: Identifier of the joystick to check (0 or 1)\n	Button 	: Button to check"},
	{"GetMouseX", (PyCFunction)PySfInput_GetMouseX, METH_NOARGS, "GetMouseX()\nGet the mouse X position."},
	{"GetMouseY", (PyCFunction)PySfInput_GetMouseY, METH_NOARGS, "GetMouseY()\nGet the mouse Y position."},
	{"GetJoystickAxis", (PyCFunction)PySfInput_GetJoystickAxis, METH_VARARGS, "GetJoystickAxis(JoyId, Axis)\nGet a joystick axis position. Returns current axis position, in the range [-100, 100] (except for POV, which is [0, 360])\n	JoyId : Identifier of the joystick to check (0 or 1)\n	Axis : Axis to get"},
	{NULL}  /* Sentinel */
};

PyTypeObject PySfInputType = {
	head_init
	"Input",				/*tp_name*/
	sizeof(PySfInput),		/*tp_basicsize*/
	0,						/*tp_itemsize*/
	0,						/*tp_dealloc*/
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
	"sf.Input handles real-time input from keyboard and mouse. Use it instead of events to handle continuous moves and more game-friendly inputs.", /* tp_doc */
	0,						/* tp_traverse */
	0,						/* tp_clear */
	0,						/* tp_richcompare */
	0,						/* tp_weaklistoffset */
	0,						/* tp_iter */
	0,						/* tp_iternext */
	PySfInput_methods,		/* tp_methods */
	0,						/* tp_members */
	0,						/* tp_getset */
	0,						/* tp_base */
	0,						/* tp_dict */
	0,						/* tp_descr_get */
	0,						/* tp_descr_set */
	0,						/* tp_dictoffset */
	0,						/* tp_init */
	0,						/* tp_alloc */
	PySfInput_new,			/* tp_new */
};

PySfInput *
GetNewPySfInput()
{
	return PyObject_New(PySfInput, &PySfInputType);
}

