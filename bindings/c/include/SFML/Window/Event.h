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


////////////////////////////////////////////////////////////
/// Definition of key codes for keyboard events
////////////////////////////////////////////////////////////
typedef enum
{
    sfKeyA = 'a',
    sfKeyB = 'b',
    sfKeyC = 'c',
    sfKeyD = 'd',
    sfKeyE = 'e',
    sfKeyF = 'f',
    sfKeyG = 'g',
    sfKeyH = 'h',
    sfKeyI = 'i',
    sfKeyJ = 'j',
    sfKeyK = 'k',
    sfKeyL = 'l',
    sfKeyM = 'm',
    sfKeyN = 'n',
    sfKeyO = 'o',
    sfKeyP = 'p',
    sfKeyQ = 'q',
    sfKeyR = 'r',
    sfKeyS = 's',
    sfKeyT = 't',
    sfKeyU = 'u',
    sfKeyV = 'v',
    sfKeyW = 'w',
    sfKeyX = 'x',
    sfKeyY = 'y',
    sfKeyZ = 'z',
    sfKeyNum0 = '0',
    sfKeyNum1 = '1',
    sfKeyNum2 = '2',
    sfKeyNum3 = '3',
    sfKeyNum4 = '4',
    sfKeyNum5 = '5',
    sfKeyNum6 = '6',
    sfKeyNum7 = '7',
    sfKeyNum8 = '8',
    sfKeyNum9 = '9', 
    sfKeyEscape = 256,
    sfKeyLControl,
    sfKeyLShift,
    sfKeyLAlt,
    sfKeyLSystem,      ///< OS specific key (left side) : windows (Win and Linux), apple (MacOS), ...
    sfKeyRControl,
    sfKeyRShift,
    sfKeyRAlt,
    sfKeyRSystem,      ///< OS specific key (right side) : windows (Win and Linux), apple (MacOS), ...
    sfKeyMenu,
    sfKeyLBracket,     ///< [
    sfKeyRBracket,     ///< ]
    sfKeySemiColon,    ///< ;
    sfKeyComma,        ///< ,
    sfKeyPeriod,       ///< .
    sfKeyQuote,        ///< '
    sfKeySlash,        ///< /
    sfKeyBackSlash,
    sfKeyTilde,        ///< ~
    sfKeyEqual,        ///< =
    sfKeyDash,         ///< -
    sfKeySpace,
    sfKeyReturn,
    sfKeyBack,
    sfKeyTab,
    sfKeyPageUp,
    sfKeyPageDown,
    sfKeyEnd,
    sfKeyHome,
    sfKeyInsert,
    sfKeyDelete,
    sfKeyAdd,          ///< +
    sfKeySubtract,     ///< -
    sfKeyMultiply,     ///< *
    sfKeyDivide,       ///< /
    sfKeyLeft,         ///< Left arrow
    sfKeyRight,        ///< Right arrow
    sfKeyUp,           ///< Up arrow
    sfKeyDown,         ///< Down arrow
    sfKeyNumpad0,
    sfKeyNumpad1,
    sfKeyNumpad2,
    sfKeyNumpad3,
    sfKeyNumpad4,
    sfKeyNumpad5,
    sfKeyNumpad6,
    sfKeyNumpad7,
    sfKeyNumpad8,
    sfKeyNumpad9,
    sfKeyF1,
    sfKeyF2,
    sfKeyF3,
    sfKeyF4,
    sfKeyF5,
    sfKeyF6,
    sfKeyF7,
    sfKeyF8,
    sfKeyF9,
    sfKeyF10,
    sfKeyF11,
    sfKeyF12,
    sfKeyF13,
    sfKeyF14,
    sfKeyF15,
    sfKeyPause,

    sfKeyCount // For internal use
} sfKeyCode;


////////////////////////////////////////////////////////////
/// Definition of button codes for mouse events
////////////////////////////////////////////////////////////
typedef enum
{
    sfButtonLeft,
    sfButtonRight,
    sfButtonMiddle,
    sfButtonX1,
    sfButtonX2
} sfMouseButton;


////////////////////////////////////////////////////////////
/// Definition of joystick axis for joystick events
////////////////////////////////////////////////////////////
typedef enum
{
    sfJoyAxisX,
    sfJoyAxisY,
    sfJoyAxisZ,
    sfJoyAxisR,
    sfJoyAxisU,
    sfJoyAxisV,
    sfJoyAxisPOV
} sfJoyAxis;


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
    sfEvtJoyButtonPressed,
    sfEvtJoyButtonReleased,
    sfEvtJoyMoved
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
struct sfJoyMoveEvent
{
    sfEventType  Type;
    unsigned int JoystickId;
    sfJoyAxis    Axis;
    float        Position;
};

////////////////////////////////////////////////////////////
/// Joystick buttons events parameters
////////////////////////////////////////////////////////////
struct sfJoyButtonEvent
{
    sfEventType  Type;
    unsigned int JoystickId;
    unsigned int Button;
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
    sfEventType               Type; ///< Type of the event
    struct sfKeyEvent         Key;
    struct sfTextEvent        Text;
    struct sfMouseMoveEvent   MouseMove;
    struct sfMouseButtonEvent MouseButton;
    struct sfMouseWheelEvent  MouseWheel;
    struct sfJoyMoveEvent     JoyMove;
    struct sfJoyButtonEvent   JoyButton;
    struct sfSizeEvent        Size;
} sfEvent;


#endif // SFML_EVENT_H
