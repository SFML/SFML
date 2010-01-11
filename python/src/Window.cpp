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

#include "Window.hpp"

#include "Event.hpp"
#include "VideoMode.hpp"
#include "Input.hpp"
#include "ContextSettings.hpp"

#include <SFML/Window/WindowStyle.hpp>

#include "compat.hpp"


extern PyTypeObject PySfEventType;
extern PyTypeObject PySfContextSettingsType;
extern PyTypeObject PySfVideoModeType;


static void
PySfWindow_dealloc(PySfWindow* self)
{
	delete self->obj;
	free_object(self);
}

static PyObject *
PySfWindow_new(PyTypeObject *type, PyObject *args, PyObject *kwds)
{
	long Handle;
	PySfContextSettings *Params=NULL;
	PySfWindow *self;
	self = (PySfWindow *)type->tp_alloc(type, 0);
	if (self != NULL)
	{
		if (PyArg_ParseTuple(args, "l|O!:Window.__new__", &Handle, &PySfContextSettingsType, &Params))
		{
			if (Params)
			{
				PySfContextSettingsUpdate(Params);
				self->obj = new sf::Window((sf::WindowHandle)Handle, *(Params->obj));
			}
			else
				self->obj = new sf::Window((sf::WindowHandle)Handle);
		}
		else
		{
			PyErr_Clear();
			self->obj = new sf::Window();
		}
	}
	return (PyObject *)self;
}


static PyObject*
PySfWindow_GetEvent(PySfWindow *self, PyObject *args)
{
	PySfEvent *PyEvent = (PySfEvent *)args;

	if (! PyObject_TypeCheck(PyEvent, &PySfEventType))
	{
		PyErr_SetString(PyExc_TypeError, "Window.GetEvent() Argument is not a sfEvent");
		return NULL;
	}

	if (self->obj->GetEvent(*(PyEvent->obj)))
	{
		PyEvent->Type = PyEvent->obj->Type;
		PyEvent->Text->Unicode = PyEvent->obj->Text.Unicode;
		PyEvent->Key->Code = PyEvent->obj->Key.Code;
		Py_DECREF(PyEvent->Key->Alt);
		PyEvent->Key->Alt = PyBool_FromLong(PyEvent->obj->Key.Alt);
		Py_DECREF(PyEvent->Key->Control);
		PyEvent->Key->Control = PyBool_FromLong(PyEvent->obj->Key.Control);
		Py_DECREF(PyEvent->Key->Shift);
		PyEvent->Key->Shift = PyBool_FromLong(PyEvent->obj->Key.Shift);
		PyEvent->MouseButton->Button = PyEvent->obj->MouseButton.Button;
		PyEvent->MouseButton->X = PyEvent->obj->MouseButton.X;
		PyEvent->MouseButton->Y = PyEvent->obj->MouseButton.Y;
		PyEvent->MouseMove->X = PyEvent->obj->MouseMove.X;
		PyEvent->MouseMove->Y = PyEvent->obj->MouseMove.Y;
		PyEvent->JoyMove->JoystickId = PyEvent->obj->JoyMove.JoystickId;
		PyEvent->JoyButton->JoystickId = PyEvent->obj->JoyButton.JoystickId;
		PyEvent->JoyButton->Button = PyEvent->obj->JoyButton.Button;
		PyEvent->JoyMove->Axis = PyEvent->obj->JoyMove.Axis;
		PyEvent->JoyMove->Position = PyEvent->obj->JoyMove.Position;
		PyEvent->Size->Width = PyEvent->obj->Size.Width;
		PyEvent->Size->Height = PyEvent->obj->Size.Height;
		PyEvent->MouseWheel->Delta = PyEvent->obj->MouseWheel.Delta;
		Py_RETURN_TRUE;
	}
	else
		Py_RETURN_FALSE;
}


PyObject*
PySfWindow_Create(PySfWindow* self, PyObject *args, PyObject *kwds)
{
	PyObject *VideoModeTmp=NULL;
	sf::VideoMode *VideoMode;
	char *Title=NULL;
	unsigned long WindowStyle = sf::Style::Resize | sf::Style::Close;
	PySfContextSettings *Params=NULL;

	const char *kwlist[] = {"VideoMode", "Title", "WindowStyle", "Params", NULL};

	if (!PyArg_ParseTupleAndKeywords(args, kwds, "O!s|IO!:Window.Create", (char **)kwlist, &PySfVideoModeType, &VideoModeTmp, &Title, &WindowStyle, &PySfContextSettingsType, &Params))
		return NULL; 

	VideoMode = ((PySfVideoMode *)VideoModeTmp)->obj;

	if (Params)
	{
		PySfContextSettingsUpdate(Params);
		self->obj->Create(*VideoMode, Title, WindowStyle, *(Params->obj));
	}
	else
		self->obj->Create(*VideoMode, Title, WindowStyle);

	Py_RETURN_NONE;
}

static int
PySfWindow_init(PySfWindow *self, PyObject *args, PyObject *kwds)
{
	long Handle;
	PySfContextSettings *Params;

	if (args != NULL)
	{
		if (PyTuple_Size(args) == 0)
			return 0;
		if (PyArg_ParseTuple(args, "l|O!:Window.__new__", &Handle, &PySfContextSettingsType, &Params))
			return 0;
		PyErr_Clear();
		if (PySfWindow_Create(self, args, kwds) == NULL)
			return -1;
	}
	return 0;
}

static PyObject *
PySfWindow_Close(PySfWindow *self)
{
	self->obj->Close();
	Py_RETURN_NONE;
}
static PyObject *
PySfWindow_IsOpened(PySfWindow *self)
{
	return PyBool_FromLong(self->obj->IsOpened());
}
static PyObject *
PySfWindow_GetWidth(PySfWindow *self)
{
	return PyLong_FromUnsignedLong(self->obj->GetWidth());
}
static PyObject *
PySfWindow_GetHeight(PySfWindow *self)
{
	return PyLong_FromUnsignedLong(self->obj->GetHeight());
}

static PyObject *
PySfWindow_UseVerticalSync(PySfWindow *self, PyObject *args)
{
	self->obj->UseVerticalSync(PyBool_AsBool(args));
	Py_RETURN_NONE;
}
static PyObject *
PySfWindow_ShowMouseCursor(PySfWindow *self, PyObject *args)
{
	self->obj->ShowMouseCursor(PyBool_AsBool(args));
	Py_RETURN_NONE;
}

static PyObject *
PySfWindow_SetActive(PySfWindow *self, PyObject *args)
{
	return PyBool_FromLong(self->obj->SetActive(PyBool_AsBool(args)));
}
static PyObject *
PySfWindow_Display(PySfWindow *self)
{
	self->obj->Display();
	Py_RETURN_NONE;
}
static PyObject *
PySfWindow_GetFrameTime(PySfWindow *self)
{
	return PyFloat_FromDouble(self->obj->GetFrameTime());
}

static PyObject *
PySfWindow_GetInput(PySfWindow *self)
{
	PySfInput *Input;
	Input = GetNewPySfInput();
	Input->obj = (sf::Input *)&self->obj->GetInput();
	return (PyObject *)Input;
}

static PyObject *
PySfWindow_GetSettings(PySfWindow *self)
{
	PySfContextSettings *Settings;
	Settings = GetNewPySfContextSettings();
	Settings->obj = new sf::ContextSettings(self->obj->GetSettings());
	Settings->DepthBits = Settings->obj->DepthBits;
	Settings->StencilBits = Settings->obj->StencilBits;
	Settings->AntialiasingLevel = Settings->obj->AntialiasingLevel;
	return (PyObject *)Settings;
}

static PyObject *
PySfWindow_SetPosition(PySfWindow* self, PyObject *args)
{
	int Left=0, Top=0;
	if (!PyArg_ParseTuple(args, "ii:Window.SetPosition", &Left, &Top))
		return NULL; 
	self->obj->SetPosition(Left,Top);
	Py_RETURN_NONE;
}

static PyObject *
PySfWindow_SetFramerateLimit(PySfWindow *self, PyObject *args)
{
	self->obj->SetFramerateLimit(PyLong_AsUnsignedLong(args));
	Py_RETURN_NONE;
}

static PyObject *
PySfWindow_Show(PySfWindow *self, PyObject *args)
{
	self->obj->Show(PyBool_AsBool(args));
	Py_RETURN_NONE;
}

static PyObject *
PySfWindow_EnableKeyRepeat(PySfWindow *self, PyObject *args)
{
	self->obj->EnableKeyRepeat(PyBool_AsBool(args));
	Py_RETURN_NONE;
}

static PyObject *
PySfWindow_SetCursorPosition(PySfWindow* self, PyObject *args)
{
	unsigned int Left=0, Top=0;
	if (!PyArg_ParseTuple(args, "II:Window.SetCursorPosition", &Left, &Top))
		return NULL; 
	self->obj->SetCursorPosition(Left,Top);
	Py_RETURN_NONE;
}

static PyObject *
PySfWindow_SetSize(PySfWindow* self, PyObject *args)
{
	unsigned int Width=0, Height=0;
	if (!PyArg_ParseTuple(args, "II:Window.SetSize", &Width, &Height))
		return NULL; 
	self->obj->SetSize(Width, Height);
	Py_RETURN_NONE;
}

static PyObject *
PySfWindow_SetJoystickThreshold(PySfWindow* self, PyObject *args)
{
	self->obj->SetJoystickThreshold(PyFloat_AsDouble(args));
	Py_RETURN_NONE;
}

static PyObject *
PySfWindow_SetIcon(PySfWindow* self, PyObject *args)
{
	unsigned int Width, Height, Size;
	char *Data;

	if (! PyArg_ParseTuple(args, "IIs#:Window.SetIcon", &Width, &Height, &Data, &Size))
		return NULL; 

	self->obj->SetIcon(Width, Height, (sf::Uint8*) Data);
	Py_RETURN_NONE;
}

static PyMethodDef PySfWindow_methods[] = {
	{"Close", (PyCFunction)PySfWindow_Close, METH_NOARGS, "Close()\nClose (destroy) the window. The sf.Window instance remains valid and you can call Create to recreate the window."},
	{"Create", (PyCFunction)PySfWindow_Create, METH_VARARGS | METH_KEYWORDS, "Create(Mode, Title, sf.Style.Resize | sf.Style.Close, Params = sf.ContextSettings())\n\
Create a window.\n\
	Mode : Video mode to use (sf.VideoMode instance)\n\
	Title : Title of the window\n\
	WindowStyle : Window style (Resize | Close by default)\n\
	Params : Creation parameters (see default constructor for default values)"},
	{"Display", (PyCFunction)PySfWindow_Display, METH_NOARGS, "Display()\nDisplay the window on screen."},
	{"EnableKeyRepeat", (PyCFunction)PySfWindow_EnableKeyRepeat, METH_O, "EnableKeyRepeat(Enable)\nEnable or disable automatic key-repeat. Automatic key-repeat is enabled by default.\n	Enabled : True to enable, false to disable"},
	{"GetEvent", (PyCFunction)PySfWindow_GetEvent, METH_O, "GetEvent(Event)\nGet the event on top of events stack, if any, and pop it. Returns True if an event was returned, False if events stack was empty.\n	EventReceived : Event to fill, if any."},
	{"GetFrameTime", (PyCFunction)PySfWindow_GetFrameTime, METH_NOARGS, "GetFrameTime()\nGet time elapsed since last frame. Returns time elapsed, in seconds"},
	{"GetHeight", (PyCFunction)PySfWindow_GetHeight, METH_NOARGS, "GetHeight()\nGet the height of the rendering region of the window."},
	{"GetInput", (PyCFunction)PySfWindow_GetInput, METH_NOARGS, "GetInput()\nGet the input manager of the window."},
	{"GetSettings", (PyCFunction)PySfWindow_GetSettings, METH_NOARGS, "GetSettings()\nGet the creation settings of the window."},
	{"GetWidth", (PyCFunction)PySfWindow_GetWidth, METH_NOARGS, "GetWidth()\nGet the width of the rendering region of the window."},
	{"IsOpened", (PyCFunction)PySfWindow_IsOpened, METH_NOARGS, "IsOpened()\nTell whether or not the window is opened (ie. has been created). Note that a hidden window (Show(False)) will still return True."},
	{"SetActive", (PyCFunction)PySfWindow_SetActive, METH_O, "SetActive(Active)\nActivate of deactivate the window as the current target for rendering. Returns True if operation was successful, False otherwise.\n	Active : True to activate, False to deactivate (True by default)"},
	{"SetCursorPosition", (PyCFunction)PySfWindow_SetCursorPosition, METH_VARARGS, "SetCursorPosition(Left, Top)\nChange the position of the mouse cursor.\n	Left : Left coordinate of the cursor, relative to the window\n	Top : Top coordinate of the cursor, relative to the window"},
	{"SetSize", (PyCFunction)PySfWindow_SetSize, METH_VARARGS, "SetSize(Width, Height)\nChange the size of the rendering region of the window.\n\
	Width :  New width\n	Height : New height"},
	{"SetFramerateLimit", (PyCFunction)PySfWindow_SetFramerateLimit, METH_O, "SetFramerateLimit(Limit)\nSet the framerate at a fixed frequency.\n	Limit : Framerate limit, in frames per seconds (use 0 to disable limit)"},
	{"SetJoystickThreshold", (PyCFunction)PySfWindow_SetJoystickThreshold, METH_O, "SetJoystickThreshold(Threshold)\nChange the joystick threshold, ie. the value below which no move event will be generated.\n	Threshold : New threshold, in range [0., 100.]"},
	{"SetPosition", (PyCFunction)PySfWindow_SetPosition, METH_VARARGS, "SetPosition(X, Y)\nChange the position of the window on screen. Only works for top-level windows\n	Left : Left position\n	Top : Top position"},
	{"Show", (PyCFunction)PySfWindow_Show, METH_O, "Show(State)\nShow or hide the window.\n	State : True to show, false to hide."},
	{"ShowMouseCursor", (PyCFunction)PySfWindow_ShowMouseCursor, METH_O, "ShowMouseCursor(Show)\nShow or hide the mouse cursor.\n	Show : True to show, false to hide."},
	{"UseVerticalSync", (PyCFunction)PySfWindow_UseVerticalSync, METH_O, "UseVerticalSync(Enabled)\nEnable / disable vertical synchronization.\n	Enabled : True to enable v-sync, False to deactivate"},
	{"SetIcon", (PyCFunction)PySfWindow_SetIcon, METH_VARARGS, "SetIcon(Width, Height, Pixels)\n\
Change the window's icon.\n\
	Width :  Icon's width, in pixels\n\
	Height : Icon's height, in pixels\n\
	Pixels : Pointer to the pixels in memory, format must be RGBA 32 bits."},
	{NULL}  /* Sentinel */
};

PyTypeObject PySfWindowType = {
	head_init
	"Window",				/*tp_name*/
	sizeof(PySfWindow),		/*tp_basicsize*/
	0,						/*tp_itemsize*/
	(destructor)PySfWindow_dealloc, /*tp_dealloc*/
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
	"sf.Window is a rendering window ; it can create a new window or connect to an existing one.\n\
Default constructor : sf.Window()\n\
Construct a new window : sf.Window(Mode, Title, sf.Style.Resize | sf.Style.Close, Params = sf.ContextSettings())\n\
	Mode : Video mode to use (sf.VideoMode instance)\n\
	Title : Title of the window\n\
	WindowStyle : Window style (Resize | Close by default)\n\
	Params : Creation parameters (see default constructor for default values)\n\
Construct the window from an existing control : sf.Window(Handle, Params)\n\
	Handle 	: Platform-specific handle of the control\n\
	Params 	: Creation parameters (see default constructor for default values)", /* tp_doc */
	0,						/* tp_traverse */
	0,						/* tp_clear */
	0,						/* tp_richcompare */
	0,						/* tp_weaklistoffset */
	0,						/* tp_iter */
	0,						/* tp_iternext */
	PySfWindow_methods,		/* tp_methods */
	0,						/* tp_members */
	0,						/* tp_getset */
	0,						/* tp_base */
	0,						/* tp_dict */
	0,						/* tp_descr_get */
	0,						/* tp_descr_set */
	0,						/* tp_dictoffset */
	(initproc)PySfWindow_init, /* tp_init */
	0,						/* tp_alloc */
	PySfWindow_new,			/* tp_new */
};


