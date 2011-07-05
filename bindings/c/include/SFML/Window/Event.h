////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007-2009 Laurent Gomila (laurent.gom@gmail.com)//
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

#ifndef SFML_EVENT_H
#define SFML_EVENT_H

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Config.h>
#include <SFML/Window/Joystick.h>
#include <SFML/Window/Keyboard.h>
#include <SFML/Window/Mouse.h>


////////////////////////////////////////////////////////////
/// Definition of all the event types
////////////////////////////////////////////////////////////
typedef enum
{
    sfEvtClosed,
    sfEvtResized,
    sfEvtLostFocus,
    sfEvtGainedFocus,
    sfEvtTextEntered,
    sfEvtKeyPressed,
    sfEvtKeyReleased,
    sfEvtMouseWheelMoved,
    sfEvtMouseButtonPressed,
    sfEvtMouseButtonReleased,
    sfEvtMouseMoved,
    sfEvtMouseEntered,
    sfEvtMouseLeft,
    sfEvtJoystickButtonPressed,
    sfEvtJoystickButtonReleased,
    sfEvtJoystickMoved,
    sfEvtJoystickConnected,
    sfEvtJoystickDisconnected
} sfEventType;


////////////////////////////////////////////////////////////
/// Keyboard event parameters
////////////////////////////////////////////////////////////
struct sfKeyEvent
{
    sfEventType Type;
    sfKeyCode   Code;
    sfBool      Alt;
    sfBool      Control;
    sfBool      Shift;
    sfBool      System;
};

////////////////////////////////////////////////////////////
/// Text event parameters
////////////////////////////////////////////////////////////
struct sfTextEvent
{
    sfEventType Type;
    sfUint32    Unicode;
};

////////////////////////////////////////////////////////////
/// Mouse move event parameters
////////////////////////////////////////////////////////////
struct sfMouseMoveEvent
{
    sfEventType Type;
    int         X;
    int         Y;
};

////////////////////////////////////////////////////////////
/// Mouse buttons events parameters
////////////////////////////////////////////////////////////
struct sfMouseButtonEvent
{
    sfEventType   Type;
    sfMouseButton Button;
    int           X;
    int           Y;
};

////////////////////////////////////////////////////////////
/// Mouse wheel events parameters
////////////////////////////////////////////////////////////
struct sfMouseWheelEvent
{
    sfEventType Type;
    int         Delta;
    int         X;
    int         Y;
};

////////////////////////////////////////////////////////////
/// Joystick axis move event parameters
////////////////////////////////////////////////////////////
struct sfJoystickMoveEvent
{
    sfEventType    Type;
    unsigned int   JoystickId;
    sfJoystickAxis Axis;
    float          Position;
};

////////////////////////////////////////////////////////////
/// Joystick buttons events parameters
////////////////////////////////////////////////////////////
struct sfJoystickButtonEvent
{
    sfEventType  Type;
    unsigned int JoystickId;
    unsigned int Button;
};

////////////////////////////////////////////////////////////
/// Joystick connection/disconnection event parameters
////////////////////////////////////////////////////////////
struct sfJoystickConnectEvent
{
    sfEventType  Type;
    unsigned int JoystickId;
};

////////////////////////////////////////////////////////////
/// Size events parameters
////////////////////////////////////////////////////////////
struct sfSizeEvent
{
    sfEventType  Type;
    unsigned int Width;
    unsigned int Height;
};


////////////////////////////////////////////////////////////
/// sfEvent defines a system event and its parameters
////////////////////////////////////////////////////////////
typedef union
{
    ////////////////////////////////////////////////////////////
    // Member data
    ////////////////////////////////////////////////////////////
    sfEventType                   Type; ///< Type of the event
    struct sfSizeEvent            Size;
    struct sfKeyEvent             Key;
    struct sfTextEvent            Text;
    struct sfMouseMoveEvent       MouseMove;
    struct sfMouseButtonEvent     MouseButton;
    struct sfMouseWheelEvent      MouseWheel;
    struct sfJoystickMoveEvent    JoystickMove;
    struct sfJoystickButtonEvent  JoystickButton;
    struct sfJoystickConnectEvent JoystickConnect;
} sfEvent;


#endif // SFML_EVENT_H
