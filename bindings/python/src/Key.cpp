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

#include "Key.hpp"

#include "compat.hpp"

static PyObject *
PySfKey_new(PyTypeObject *type, PyObject *args, PyObject *kwds)
{
	PySfKey *self;
	self = (PySfKey *)type->tp_alloc(type, 0);
	return (PyObject *)self;
}

PyTypeObject PySfKeyType = {
	head_init
	"Key",					/*tp_name*/
	sizeof(PySfKey),		/*tp_basicsize*/
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
	"Definition of key codes for keyboard events.", /* tp_doc */
	0,						/* tp_traverse */
	0,						/* tp_clear */
	0,						/* tp_richcompare */
	0,						/* tp_weaklistoffset */
	0,						/* tp_iter */
	0,						/* tp_iternext */
	0,						/* tp_methods */
	0,						/* tp_members */
	0,						/* tp_getset */
	0,						/* tp_base */
	0,						/* tp_dict */
	0,						/* tp_descr_get */
	0,						/* tp_descr_set */
	0,						/* tp_dictoffset */
	0,						/* tp_init */
	0,						/* tp_alloc */
	PySfKey_new,			/* tp_new */
};

void PySfKey_InitConst()
{
	PyObject *obj;
	obj = PyLong_FromLong(sf::Key::Numpad2);
	PyDict_SetItemString(PySfKeyType.tp_dict, "Numpad2", obj);
	Py_DECREF(obj);
	obj = PyLong_FromLong(sf::Key::Numpad3);
	PyDict_SetItemString(PySfKeyType.tp_dict, "Numpad3", obj);
	Py_DECREF(obj);
	obj = PyLong_FromLong(sf::Key::Numpad0);
	PyDict_SetItemString(PySfKeyType.tp_dict, "Numpad0", obj);
	Py_DECREF(obj);
	obj = PyLong_FromLong(sf::Key::Numpad1);
	PyDict_SetItemString(PySfKeyType.tp_dict, "Numpad1", obj);
	Py_DECREF(obj);
	obj = PyLong_FromLong(sf::Key::Numpad6);
	PyDict_SetItemString(PySfKeyType.tp_dict, "Numpad6", obj);
	Py_DECREF(obj);
	obj = PyLong_FromLong(sf::Key::Numpad7);
	PyDict_SetItemString(PySfKeyType.tp_dict, "Numpad7", obj);
	Py_DECREF(obj);
	obj = PyLong_FromLong(sf::Key::Numpad4);
	PyDict_SetItemString(PySfKeyType.tp_dict, "Numpad4", obj);
	Py_DECREF(obj);
	obj = PyLong_FromLong(sf::Key::Numpad5);
	PyDict_SetItemString(PySfKeyType.tp_dict, "Numpad5", obj);
	Py_DECREF(obj);
	obj = PyLong_FromLong(sf::Key::Numpad8);
	PyDict_SetItemString(PySfKeyType.tp_dict, "Numpad8", obj);
	Py_DECREF(obj);
	obj = PyLong_FromLong(sf::Key::Numpad9);
	PyDict_SetItemString(PySfKeyType.tp_dict, "Numpad9", obj);
	Py_DECREF(obj);
	obj = PyLong_FromLong(sf::Key::RAlt);
	PyDict_SetItemString(PySfKeyType.tp_dict, "RAlt", obj);
	Py_DECREF(obj);
	obj = PyLong_FromLong(sf::Key::PageUp);
	PyDict_SetItemString(PySfKeyType.tp_dict, "PageUp", obj);
	Py_DECREF(obj);
	obj = PyLong_FromLong(sf::Key::Multiply);
	PyDict_SetItemString(PySfKeyType.tp_dict, "Multiply", obj);
	Py_DECREF(obj);
	obj = PyLong_FromLong(sf::Key::D);
	PyDict_SetItemString(PySfKeyType.tp_dict, "D", obj);
	Py_DECREF(obj);
	obj = PyLong_FromLong(sf::Key::SemiColon);
	PyDict_SetItemString(PySfKeyType.tp_dict, "SemiColon", obj);
	Py_DECREF(obj);
	obj = PyLong_FromLong(sf::Key::H);
	PyDict_SetItemString(PySfKeyType.tp_dict, "H", obj);
	Py_DECREF(obj);
	obj = PyLong_FromLong(sf::Key::L);
	PyDict_SetItemString(PySfKeyType.tp_dict, "L", obj);
	Py_DECREF(obj);
	obj = PyLong_FromLong(sf::Key::P);
	PyDict_SetItemString(PySfKeyType.tp_dict, "P", obj);
	Py_DECREF(obj);
	obj = PyLong_FromLong(sf::Key::Num7);
	PyDict_SetItemString(PySfKeyType.tp_dict, "Num7", obj);
	Py_DECREF(obj);
	obj = PyLong_FromLong(sf::Key::T);
	PyDict_SetItemString(PySfKeyType.tp_dict, "T", obj);
	Py_DECREF(obj);
	obj = PyLong_FromLong(sf::Key::X);
	PyDict_SetItemString(PySfKeyType.tp_dict, "X", obj);
	Py_DECREF(obj);
	obj = PyLong_FromLong(sf::Key::RSystem);
	PyDict_SetItemString(PySfKeyType.tp_dict, "RSystem", obj);
	Py_DECREF(obj);
	obj = PyLong_FromLong(sf::Key::F5);
	PyDict_SetItemString(PySfKeyType.tp_dict, "F5", obj);
	Py_DECREF(obj);
	obj = PyLong_FromLong(sf::Key::Num4);
	PyDict_SetItemString(PySfKeyType.tp_dict, "Num4", obj);
	Py_DECREF(obj);
	obj = PyLong_FromLong(sf::Key::Num5);
	PyDict_SetItemString(PySfKeyType.tp_dict, "Num5", obj);
	Py_DECREF(obj);
	obj = PyLong_FromLong(sf::Key::Num6);
	PyDict_SetItemString(PySfKeyType.tp_dict, "Num6", obj);
	Py_DECREF(obj);
	obj = PyLong_FromLong(sf::Key::Right);
	PyDict_SetItemString(PySfKeyType.tp_dict, "Right", obj);
	Py_DECREF(obj);
	obj = PyLong_FromLong(sf::Key::Num0);
	PyDict_SetItemString(PySfKeyType.tp_dict, "Num0", obj);
	Py_DECREF(obj);
	obj = PyLong_FromLong(sf::Key::Num1);
	PyDict_SetItemString(PySfKeyType.tp_dict, "Num1", obj);
	Py_DECREF(obj);
	obj = PyLong_FromLong(sf::Key::Num2);
	PyDict_SetItemString(PySfKeyType.tp_dict, "Num2", obj);
	Py_DECREF(obj);
	obj = PyLong_FromLong(sf::Key::Num3);
	PyDict_SetItemString(PySfKeyType.tp_dict, "Num3", obj);
	Py_DECREF(obj);
	obj = PyLong_FromLong(sf::Key::LControl);
	PyDict_SetItemString(PySfKeyType.tp_dict, "LControl", obj);
	Py_DECREF(obj);
	obj = PyLong_FromLong(sf::Key::Num8);
	PyDict_SetItemString(PySfKeyType.tp_dict, "Num8", obj);
	Py_DECREF(obj);
	obj = PyLong_FromLong(sf::Key::Num9);
	PyDict_SetItemString(PySfKeyType.tp_dict, "Num9", obj);
	Py_DECREF(obj);
	obj = PyLong_FromLong(sf::Key::Tab);
	PyDict_SetItemString(PySfKeyType.tp_dict, "Tab", obj);
	Py_DECREF(obj);
	obj = PyLong_FromLong(sf::Key::RBracket);
	PyDict_SetItemString(PySfKeyType.tp_dict, "RBracket", obj);
	Py_DECREF(obj);
	obj = PyLong_FromLong(sf::Key::End);
	PyDict_SetItemString(PySfKeyType.tp_dict, "End", obj);
	Py_DECREF(obj);
	obj = PyLong_FromLong(sf::Key::BackSlash);
	PyDict_SetItemString(PySfKeyType.tp_dict, "BackSlash", obj);
	Py_DECREF(obj);
	obj = PyLong_FromLong(sf::Key::LShift);
	PyDict_SetItemString(PySfKeyType.tp_dict, "LShift", obj);
	Py_DECREF(obj);
	obj = PyLong_FromLong(sf::Key::E);
	PyDict_SetItemString(PySfKeyType.tp_dict, "E", obj);
	Py_DECREF(obj);
	obj = PyLong_FromLong(sf::Key::C);
	PyDict_SetItemString(PySfKeyType.tp_dict, "C", obj);
	Py_DECREF(obj);
	obj = PyLong_FromLong(sf::Key::G);
	PyDict_SetItemString(PySfKeyType.tp_dict, "G", obj);
	Py_DECREF(obj);
	obj = PyLong_FromLong(sf::Key::K);
	PyDict_SetItemString(PySfKeyType.tp_dict, "K", obj);
	Py_DECREF(obj);
	obj = PyLong_FromLong(sf::Key::Up);
	PyDict_SetItemString(PySfKeyType.tp_dict, "Up", obj);
	Py_DECREF(obj);
	obj = PyLong_FromLong(sf::Key::O);
	PyDict_SetItemString(PySfKeyType.tp_dict, "O", obj);
	Py_DECREF(obj);
	obj = PyLong_FromLong(sf::Key::S);
	PyDict_SetItemString(PySfKeyType.tp_dict, "S", obj);
	Py_DECREF(obj);
	obj = PyLong_FromLong(sf::Key::W);
	PyDict_SetItemString(PySfKeyType.tp_dict, "W", obj);
	Py_DECREF(obj);
	obj = PyLong_FromLong(sf::Key::F12);
	PyDict_SetItemString(PySfKeyType.tp_dict, "F12", obj);
	Py_DECREF(obj);
	obj = PyLong_FromLong(sf::Key::F13);
	PyDict_SetItemString(PySfKeyType.tp_dict, "F13", obj);
	Py_DECREF(obj);
	obj = PyLong_FromLong(sf::Key::F10);
	PyDict_SetItemString(PySfKeyType.tp_dict, "F10", obj);
	Py_DECREF(obj);
	obj = PyLong_FromLong(sf::Key::F11);
	PyDict_SetItemString(PySfKeyType.tp_dict, "F11", obj);
	Py_DECREF(obj);
	obj = PyLong_FromLong(sf::Key::F14);
	PyDict_SetItemString(PySfKeyType.tp_dict, "F14", obj);
	Py_DECREF(obj);
	obj = PyLong_FromLong(sf::Key::Delete);
	PyDict_SetItemString(PySfKeyType.tp_dict, "Delete", obj);
	Py_DECREF(obj);
	obj = PyLong_FromLong(sf::Key::Back);
	PyDict_SetItemString(PySfKeyType.tp_dict, "Back", obj);
	Py_DECREF(obj);
	obj = PyLong_FromLong(sf::Key::Tilde);
	PyDict_SetItemString(PySfKeyType.tp_dict, "Tilde", obj);
	Py_DECREF(obj);
	obj = PyLong_FromLong(sf::Key::Home);
	PyDict_SetItemString(PySfKeyType.tp_dict, "Home", obj);
	Py_DECREF(obj);
	obj = PyLong_FromLong(sf::Key::Pause);
	PyDict_SetItemString(PySfKeyType.tp_dict, "Pause", obj);
	Py_DECREF(obj);
	obj = PyLong_FromLong(sf::Key::Add);
	PyDict_SetItemString(PySfKeyType.tp_dict, "Add", obj);
	Py_DECREF(obj);
	obj = PyLong_FromLong(sf::Key::F15);
	PyDict_SetItemString(PySfKeyType.tp_dict, "F15", obj);
	Py_DECREF(obj);
	obj = PyLong_FromLong(sf::Key::Subtract);
	PyDict_SetItemString(PySfKeyType.tp_dict, "Subtract", obj);
	Py_DECREF(obj);
	obj = PyLong_FromLong(sf::Key::B);
	PyDict_SetItemString(PySfKeyType.tp_dict, "B", obj);
	Py_DECREF(obj);
	obj = PyLong_FromLong(sf::Key::F);
	PyDict_SetItemString(PySfKeyType.tp_dict, "F", obj);
	Py_DECREF(obj);
	obj = PyLong_FromLong(sf::Key::J);
	PyDict_SetItemString(PySfKeyType.tp_dict, "J", obj);
	Py_DECREF(obj);
	obj = PyLong_FromLong(sf::Key::N);
	PyDict_SetItemString(PySfKeyType.tp_dict, "N", obj);
	Py_DECREF(obj);
	obj = PyLong_FromLong(sf::Key::LBracket);
	PyDict_SetItemString(PySfKeyType.tp_dict, "LBracket", obj);
	Py_DECREF(obj);
	obj = PyLong_FromLong(sf::Key::R);
	PyDict_SetItemString(PySfKeyType.tp_dict, "R", obj);
	Py_DECREF(obj);
	obj = PyLong_FromLong(sf::Key::V);
	PyDict_SetItemString(PySfKeyType.tp_dict, "V", obj);
	Py_DECREF(obj);
	obj = PyLong_FromLong(sf::Key::LSystem);
	PyDict_SetItemString(PySfKeyType.tp_dict, "LSystem", obj);
	Py_DECREF(obj);
	obj = PyLong_FromLong(sf::Key::Z);
	PyDict_SetItemString(PySfKeyType.tp_dict, "Z", obj);
	Py_DECREF(obj);
	obj = PyLong_FromLong(sf::Key::Left);
	PyDict_SetItemString(PySfKeyType.tp_dict, "Left", obj);
	Py_DECREF(obj);
	obj = PyLong_FromLong(sf::Key::F1);
	PyDict_SetItemString(PySfKeyType.tp_dict, "F1", obj);
	Py_DECREF(obj);
	obj = PyLong_FromLong(sf::Key::F2);
	PyDict_SetItemString(PySfKeyType.tp_dict, "F2", obj);
	Py_DECREF(obj);
	obj = PyLong_FromLong(sf::Key::F3);
	PyDict_SetItemString(PySfKeyType.tp_dict, "F3", obj);
	Py_DECREF(obj);
	obj = PyLong_FromLong(sf::Key::F4);
	PyDict_SetItemString(PySfKeyType.tp_dict, "F4", obj);
	Py_DECREF(obj);
	obj = PyLong_FromLong(sf::Key::Divide);
	PyDict_SetItemString(PySfKeyType.tp_dict, "Divide", obj);
	Py_DECREF(obj);
	obj = PyLong_FromLong(sf::Key::F6);
	PyDict_SetItemString(PySfKeyType.tp_dict, "F6", obj);
	Py_DECREF(obj);
	obj = PyLong_FromLong(sf::Key::F7);
	PyDict_SetItemString(PySfKeyType.tp_dict, "F7", obj);
	Py_DECREF(obj);
	obj = PyLong_FromLong(sf::Key::F8);
	PyDict_SetItemString(PySfKeyType.tp_dict, "F8", obj);
	Py_DECREF(obj);
	obj = PyLong_FromLong(sf::Key::F9);
	PyDict_SetItemString(PySfKeyType.tp_dict, "F9", obj);
	Py_DECREF(obj);
	obj = PyLong_FromLong(sf::Key::Period);
	PyDict_SetItemString(PySfKeyType.tp_dict, "Period", obj);
	Py_DECREF(obj);
	obj = PyLong_FromLong(sf::Key::Down);
	PyDict_SetItemString(PySfKeyType.tp_dict, "Down", obj);
	Py_DECREF(obj);
	obj = PyLong_FromLong(sf::Key::PageDown);
	PyDict_SetItemString(PySfKeyType.tp_dict, "PageDown", obj);
	Py_DECREF(obj);
	obj = PyLong_FromLong(sf::Key::Space);
	PyDict_SetItemString(PySfKeyType.tp_dict, "Space", obj);
	Py_DECREF(obj);
	obj = PyLong_FromLong(sf::Key::Menu);
	PyDict_SetItemString(PySfKeyType.tp_dict, "Menu", obj);
	Py_DECREF(obj);
	obj = PyLong_FromLong(sf::Key::RControl);
	PyDict_SetItemString(PySfKeyType.tp_dict, "RControl", obj);
	Py_DECREF(obj);
	obj = PyLong_FromLong(sf::Key::Slash);
	PyDict_SetItemString(PySfKeyType.tp_dict, "Slash", obj);
	Py_DECREF(obj);
	obj = PyLong_FromLong(sf::Key::Return);
	PyDict_SetItemString(PySfKeyType.tp_dict, "Return", obj);
	Py_DECREF(obj);
	obj = PyLong_FromLong(sf::Key::Quote);
	PyDict_SetItemString(PySfKeyType.tp_dict, "Quote", obj);
	Py_DECREF(obj);
	obj = PyLong_FromLong(sf::Key::A);
	PyDict_SetItemString(PySfKeyType.tp_dict, "A", obj);
	Py_DECREF(obj);
	obj = PyLong_FromLong(sf::Key::Insert);
	PyDict_SetItemString(PySfKeyType.tp_dict, "Insert", obj);
	Py_DECREF(obj);
	obj = PyLong_FromLong(sf::Key::RShift);
	PyDict_SetItemString(PySfKeyType.tp_dict, "RShift", obj);
	Py_DECREF(obj);
	obj = PyLong_FromLong(sf::Key::I);
	PyDict_SetItemString(PySfKeyType.tp_dict, "I", obj);
	Py_DECREF(obj);
	obj = PyLong_FromLong(sf::Key::Escape);
	PyDict_SetItemString(PySfKeyType.tp_dict, "Escape", obj);
	Py_DECREF(obj);
	obj = PyLong_FromLong(sf::Key::M);
	PyDict_SetItemString(PySfKeyType.tp_dict, "M", obj);
	Py_DECREF(obj);
	obj = PyLong_FromLong(sf::Key::Equal);
	PyDict_SetItemString(PySfKeyType.tp_dict, "Equal", obj);
	Py_DECREF(obj);
	obj = PyLong_FromLong(sf::Key::Q);
	PyDict_SetItemString(PySfKeyType.tp_dict, "Q", obj);
	Py_DECREF(obj);
	obj = PyLong_FromLong(sf::Key::U);
	PyDict_SetItemString(PySfKeyType.tp_dict, "U", obj);
	Py_DECREF(obj);
	obj = PyLong_FromLong(sf::Key::Y);
	PyDict_SetItemString(PySfKeyType.tp_dict, "Y", obj);
	Py_DECREF(obj);
	obj = PyLong_FromLong(sf::Key::Dash);
	PyDict_SetItemString(PySfKeyType.tp_dict, "Dash", obj);
	Py_DECREF(obj);
	obj = PyLong_FromLong(sf::Key::Comma);
	PyDict_SetItemString(PySfKeyType.tp_dict, "Comma", obj);
	Py_DECREF(obj);
	obj = PyLong_FromLong(sf::Key::LAlt);
	PyDict_SetItemString(PySfKeyType.tp_dict, "LAlt", obj);
	Py_DECREF(obj);
}

