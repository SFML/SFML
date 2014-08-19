////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007-2014 Laurent Gomila (laurent.gom@gmail.com)
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

#ifndef SFML_WIN32SCANCODES_HPP
#define SFML_WIN32SCANCODES_HPP

#include <SFML/Window/Keyboard.hpp>

namespace sf
{

namespace priv
{

// System code to SFML code table (source : SDL source code)
Keyboard::ScanCode win32scancodes[] =
{
    Keyboard::ScanUnknown,
    Keyboard::ScanEscape,
    Keyboard::Scan1,
    Keyboard::Scan2,
    Keyboard::Scan3,
    Keyboard::Scan4,
    Keyboard::Scan5,
    Keyboard::Scan6,
    Keyboard::Scan7,
    Keyboard::Scan8,
    Keyboard::Scan9,
    Keyboard::Scan0,
    Keyboard::ScanMinus,
    Keyboard::ScanEquals,
    Keyboard::ScanBackspace,
    Keyboard::ScanTab,
    Keyboard::ScanQ,
    Keyboard::ScanW,
    Keyboard::ScanE,
    Keyboard::ScanR,
    Keyboard::ScanT,
    Keyboard::ScanY,
    Keyboard::ScanU,
    Keyboard::ScanI,
    Keyboard::ScanO,
    Keyboard::ScanP,
    Keyboard::ScanLeftBracket,
    Keyboard::ScanRightBracket,
    Keyboard::ScanReturn,
    Keyboard::ScanLCtrl,
    Keyboard::ScanA,
    Keyboard::ScanS,
    Keyboard::ScanD,
    Keyboard::ScanF,
    Keyboard::ScanG,
    Keyboard::ScanH,
    Keyboard::ScanJ,
    Keyboard::ScanK,
    Keyboard::ScanL,
    Keyboard::ScanSemicolon,
    Keyboard::ScanApostrophe,
    Keyboard::ScanGrave,
    Keyboard::ScanLShift,
    Keyboard::ScanBackSlash,
    Keyboard::ScanZ,
    Keyboard::ScanX,
    Keyboard::ScanC,
    Keyboard::ScanV,
    Keyboard::ScanB,
    Keyboard::ScanN,
    Keyboard::ScanM,
    Keyboard::ScanComma,
    Keyboard::ScanPeriod,
    Keyboard::ScanSlash,
    Keyboard::ScanRShift,
    Keyboard::ScanPrintScreen,
    Keyboard::ScanLAlt,
    Keyboard::ScanSpace,
    Keyboard::ScanCapsLock,
    Keyboard::ScanF1,
    Keyboard::ScanF2,
    Keyboard::ScanF3,
    Keyboard::ScanF4,
    Keyboard::ScanF5,
    Keyboard::ScanF6,
    Keyboard::ScanF7,
    Keyboard::ScanF8,
    Keyboard::ScanF9,
    Keyboard::ScanF10,
    Keyboard::ScanNumLockClear,
    Keyboard::ScanScrollLock,
    Keyboard::ScanHome,
    Keyboard::ScanUp,
    Keyboard::ScanPageUp,
    Keyboard::ScanNumpadMinus,
    Keyboard::ScanLeft,
    Keyboard::ScanNumpad5,
    Keyboard::ScanRight,
    Keyboard::ScanNumpadPlus,
    Keyboard::ScanEnd,
    Keyboard::ScanDown,
    Keyboard::ScanPageDown,
    Keyboard::ScanInsert,
    Keyboard::ScanDelete,
    Keyboard::ScanUnknown,
    Keyboard::ScanUnknown,
    Keyboard::ScanNonUSBackSlash,
    Keyboard::ScanF11,
    Keyboard::ScanF12,
    Keyboard::ScanPause,
    Keyboard::ScanUnknown,
    Keyboard::ScanLGui,
    Keyboard::ScanRGui,
    Keyboard::ScanApplication,
    Keyboard::ScanUnknown,
    Keyboard::ScanUnknown,
    Keyboard::ScanUnknown,
    Keyboard::ScanUnknown,
    Keyboard::ScanUnknown,
    Keyboard::ScanUnknown,
    Keyboard::ScanF13,
    Keyboard::ScanF14,
    Keyboard::ScanF15,
    Keyboard::ScanF16,
    Keyboard::ScanF17,
    Keyboard::ScanF18,
    Keyboard::ScanF19,
    Keyboard::ScanUnknown,
    Keyboard::ScanUnknown,
    Keyboard::ScanUnknown,
    Keyboard::ScanUnknown,
    Keyboard::ScanUnknown,
    Keyboard::ScanUnknown,
    Keyboard::ScanUnknown,
    Keyboard::ScanUnknown,
    Keyboard::ScanUnknown,
    Keyboard::ScanUnknown,
    Keyboard::ScanUnknown,
    Keyboard::ScanUnknown,
    Keyboard::ScanUnknown,
    Keyboard::ScanUnknown,
    Keyboard::ScanUnknown,
    Keyboard::ScanUnknown,
    Keyboard::ScanUnknown,
    Keyboard::ScanUnknown,
    Keyboard::ScanUnknown,
    Keyboard::ScanUnknown,
    Keyboard::ScanUnknown
};

const unsigned int win32scancodescount = sizeof(win32scancodes) / sizeof(win32scancodes[0]);

} // namespace priv

} // namespace sf

#endif // SFML_WIN32SCANCODES_HPP
