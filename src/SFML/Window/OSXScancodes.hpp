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

#ifndef SFML_OSXSCANCODES_HPP
#define SFML_OSXSCANCODES_HPP

#include <SFML/Window/Keyboard.hpp>

namespace sf
{

namespace priv
{

// System code to SFML code table (source : SDL source code)
Keyboard::ScanCode osxscancodes[] =
{
    Keyboard::ScanA,
    Keyboard::ScanS,
    Keyboard::ScanD,
    Keyboard::ScanF,
    Keyboard::ScanH,
    Keyboard::ScanG,
    Keyboard::ScanZ,
    Keyboard::ScanX,
    Keyboard::ScanC,
    Keyboard::ScanV,
    Keyboard::ScanNonUSBackSlash,
    Keyboard::ScanB,
    Keyboard::ScanQ,
    Keyboard::ScanW,
    Keyboard::ScanE,
    Keyboard::ScanR,
    Keyboard::ScanY,
    Keyboard::ScanT,
    Keyboard::Scan1,
    Keyboard::Scan2,
    Keyboard::Scan3,
    Keyboard::Scan4,
    Keyboard::Scan6,
    Keyboard::Scan5,
    Keyboard::ScanEquals,
    Keyboard::Scan9,
    Keyboard::Scan7,
    Keyboard::ScanMinus,
    Keyboard::Scan8,
    Keyboard::Scan0,
    Keyboard::ScanRightBracket,
    Keyboard::ScanO,
    Keyboard::ScanU,
    Keyboard::ScanLeftBracket,
    Keyboard::ScanI,
    Keyboard::ScanP,
    Keyboard::ScanReturn,
    Keyboard::ScanL,
    Keyboard::ScanJ,
    Keyboard::ScanApostrophe,
    Keyboard::ScanK,
    Keyboard::ScanSemicolon,
    Keyboard::ScanBackSlash,
    Keyboard::ScanComma,
    Keyboard::ScanSlash,
    Keyboard::ScanN,
    Keyboard::ScanM,
    Keyboard::ScanPeriod,
    Keyboard::ScanTab,
    Keyboard::ScanSpace,
    Keyboard::ScanGrave,
    Keyboard::ScanBackspace,
    Keyboard::ScanNumpadEnter,
    Keyboard::ScanEscape,
    Keyboard::ScanRGui,
    Keyboard::ScanLGui,
    Keyboard::ScanLShift,
    Keyboard::ScanCapsLock,
    Keyboard::ScanLAlt,
    Keyboard::ScanLCtrl,
    Keyboard::ScanRShift,
    Keyboard::ScanRAlt,
    Keyboard::ScanRCtrl,
    Keyboard::ScanRGui,
    Keyboard::ScanF17,
    Keyboard::ScanNumpadPeriod,
    Keyboard::ScanUnknown,
    Keyboard::ScanNumpadMultiply,
    Keyboard::ScanUnknown,
    Keyboard::ScanNumpadPlus,
    Keyboard::ScanUnknown,
    Keyboard::ScanNumLockClear,
    Keyboard::ScanVolumeUp,
    Keyboard::ScanVolumeDown,
    Keyboard::ScanMute,
    Keyboard::ScanNumpadDivide,
    Keyboard::ScanNumpadEnter,
    Keyboard::ScanUnknown,
    Keyboard::ScanNumpadMinus,
    Keyboard::ScanF18,
    Keyboard::ScanF19,
    Keyboard::ScanNumpadEquals,
    Keyboard::ScanNumpad0,
    Keyboard::ScanNumpad1,
    Keyboard::ScanNumpad2,
    Keyboard::ScanNumpad3,
    Keyboard::ScanNumpad4,
    Keyboard::ScanNumpad5,
    Keyboard::ScanNumpad6,
    Keyboard::ScanNumpad7,
    Keyboard::ScanUnknown,
    Keyboard::ScanNumpad8,
    Keyboard::ScanNumpad9,
    Keyboard::ScanInternational3,
    Keyboard::ScanInternational1,
    Keyboard::ScanNumpadComma,
    Keyboard::ScanF5,
    Keyboard::ScanF6,
    Keyboard::ScanF7,
    Keyboard::ScanF3,
    Keyboard::ScanF8,
    Keyboard::ScanF9,
    Keyboard::ScanLang2,
    Keyboard::ScanF11,
    Keyboard::ScanLang1,
    Keyboard::ScanPrintScreen,
    Keyboard::ScanF16,
    Keyboard::ScanScrollLock,
    Keyboard::ScanUnknown,
    Keyboard::ScanF10,
    Keyboard::ScanApplication,
    Keyboard::ScanF12,
    Keyboard::ScanUnknown,
    Keyboard::ScanPause,
    Keyboard::ScanInsert,
    Keyboard::ScanHome,
    Keyboard::ScanPageUp,
    Keyboard::ScanDelete,
    Keyboard::ScanF4,
    Keyboard::ScanEnd,
    Keyboard::ScanF2,
    Keyboard::ScanPageDown,
    Keyboard::ScanF1,
    Keyboard::ScanLeft,
    Keyboard::ScanRight,
    Keyboard::ScanDown,
    Keyboard::ScanUp,
    Keyboard::ScanPower
};

const unsigned int osxscancodescount = sizeof(osxscancodes) / sizeof(osxscancodes[0]);

} // namespace priv

} // namespace sf


#endif // SFML_OSXSCANCODES_HPP
