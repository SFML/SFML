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

#ifndef __PYCOMPAT_HPP
#define __PYCOMPAT_HPP

#if PY_MAJOR_VERSION >= 3

#define IS_PY3K
#define head_init	PyVarObject_HEAD_INIT(NULL, 0)

#define save_to_file(self, args) \
	PyObject *string = PyUnicode_AsUTF8String(args); \
	if (string == NULL) return NULL; \
	char *path = PyBytes_AsString(string); \
	bool result = self->obj->SaveToFile(path); \
	Py_DECREF(string); \
	return PyBool_FromLong(result)

#define load_from_file(self, args) \
	PyObject *string = PyUnicode_AsUTF8String(args); \
	if (string == NULL) return NULL; \
	char *path = PyBytes_AsString(string); \
	bool result = self->obj->LoadFromFile(path); \
	Py_DECREF(string); \
	return PyBool_FromLong(result)

#else

#define save_to_file(self, args) \
	return PyBool_FromLong(self->obj->SaveToFile(PyString_AsString(args)))
#define load_from_file(self, args) \
	return PyBool_FromLong(self->obj->LoadFromFile(PyString_AsString(args)))

#define head_init	PyObject_HEAD_INIT(NULL) 0,
#define PyBytes_FromStringAndSize	PyString_FromStringAndSize

#endif

#ifndef Py_TYPE
#define Py_TYPE(a)	a->ob_type
#endif

#define free_object(a)	Py_TYPE(a)->tp_free((PyObject*)a)

#define PyBool_AsBool(a)	((PyObject_IsTrue(a))?true:false)



#endif

