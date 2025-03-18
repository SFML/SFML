////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007-2025 Laurent Gomila (laurent@sfml-dev.org)
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

#pragma once

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Window/Export.hpp>


namespace sf
{
class String;

////////////////////////////////////////////////////////////
/// \brief Give access to the real-time state of the keyboard
///
////////////////////////////////////////////////////////////
namespace Keyboard
{
////////////////////////////////////////////////////////////
/// \brief Key codes
///
/// The enumerators refer to the "localized" key; i.e. depending
/// on the layout set by the operating system, a key can be mapped
/// to `Y` or `Z`.
///
////////////////////////////////////////////////////////////
enum class Key
{
    Unknown = -1, //!< Unhandled key
    A       = 0,  //!< The A key
    B,            //!< The B key
    C,            //!< The C key
    D,            //!< The D key
    E,            //!< The E key
    F,            //!< The F key
    G,            //!< The G key
    H,            //!< The H key
    I,            //!< The I key
    J,            //!< The J key
    K,            //!< The K key
    L,            //!< The L key
    M,            //!< The M key
    N,            //!< The N key
    O,            //!< The O key
    P,            //!< The P key
    Q,            //!< The Q key
    R,            //!< The R key
    S,            //!< The S key
    T,            //!< The T key
    U,            //!< The U key
    V,            //!< The V key
    W,            //!< The W key
    X,            //!< The X key
    Y,            //!< The Y key
    Z,            //!< The Z key
    Num0,         //!< The 0 key
    Num1,         //!< The 1 key
    Num2,         //!< The 2 key
    Num3,         //!< The 3 key
    Num4,         //!< The 4 key
    Num5,         //!< The 5 key
    Num6,         //!< The 6 key
    Num7,         //!< The 7 key
    Num8,         //!< The 8 key
    Num9,         //!< The 9 key
    Escape,       //!< The Escape key
    LControl,     //!< The left Control key
    LShift,       //!< The left Shift key
    LAlt,         //!< The left Alt key
    LSystem,      //!< The left OS specific key: window (Windows and Linux), apple (macOS), ...
    RControl,     //!< The right Control key
    RShift,       //!< The right Shift key
    RAlt,         //!< The right Alt key
    RSystem,      //!< The right OS specific key: window (Windows and Linux), apple (macOS), ...
    Menu,         //!< The Menu key
    LBracket,     //!< The [ key
    RBracket,     //!< The ] key
    Semicolon,    //!< The ; key
    Comma,        //!< The , key
    Period,       //!< The . key
    Apostrophe,   //!< The ' key
    Slash,        //!< The / key
    Backslash,    //!< The \ key
    Grave,        //!< The ` key
    Equal,        //!< The = key
    Hyphen,       //!< The - key (hyphen)
    Space,        //!< The Space key
    Enter,        //!< The Enter/Return keys
    Backspace,    //!< The Backspace key
    Tab,          //!< The Tabulation key
    PageUp,       //!< The Page up key
    PageDown,     //!< The Page down key
    End,          //!< The End key
    Home,         //!< The Home key
    Insert,       //!< The Insert key
    Delete,       //!< The Delete key
    Add,          //!< The + key
    Subtract,     //!< The - key (minus, usually from numpad)
    Multiply,     //!< The * key
    Divide,       //!< The / key
    Left,         //!< Left arrow
    Right,        //!< Right arrow
    Up,           //!< Up arrow
    Down,         //!< Down arrow
    Numpad0,      //!< The numpad 0 key
    Numpad1,      //!< The numpad 1 key
    Numpad2,      //!< The numpad 2 key
    Numpad3,      //!< The numpad 3 key
    Numpad4,      //!< The numpad 4 key
    Numpad5,      //!< The numpad 5 key
    Numpad6,      //!< The numpad 6 key
    Numpad7,      //!< The numpad 7 key
    Numpad8,      //!< The numpad 8 key
    Numpad9,      //!< The numpad 9 key
    F1,           //!< The F1 key
    F2,           //!< The F2 key
    F3,           //!< The F3 key
    F4,           //!< The F4 key
    F5,           //!< The F5 key
    F6,           //!< The F6 key
    F7,           //!< The F7 key
    F8,           //!< The F8 key
    F9,           //!< The F9 key
    F10,          //!< The F10 key
    F11,          //!< The F11 key
    F12,          //!< The F12 key
    F13,          //!< The F13 key
    F14,          //!< The F14 key
    F15,          //!< The F15 key
    Pause,        //!< The Pause key
};

////////////////////////////////////////////////////////////
/// \brief The total number of keyboard keys, ignoring `Key::Unknown`
///
////////////////////////////////////////////////////////////
// NOLINTNEXTLINE(readability-identifier-naming)
static constexpr unsigned int KeyCount{static_cast<unsigned int>(Key::Pause) + 1};

////////////////////////////////////////////////////////////
/// \brief Scancodes
///
/// The enumerators are bound to a physical key and do not depend on
/// the keyboard layout used by the operating system. Usually, the AT-101
/// keyboard can be used as reference for the physical position of the keys.
///
////////////////////////////////////////////////////////////
enum class Scan
{
    Unknown = -1, //!< Represents any scancode not present in this enum
    A       = 0,  //!< Keyboard a and A key
    B,            //!< Keyboard b and B key
    C,            //!< Keyboard c and C key
    D,            //!< Keyboard d and D key
    E,            //!< Keyboard e and E key
    F,            //!< Keyboard f and F key
    G,            //!< Keyboard g and G key
    H,            //!< Keyboard h and H key
    I,            //!< Keyboard i and I key
    J,            //!< Keyboard j and J key
    K,            //!< Keyboard k and K key
    L,            //!< Keyboard l and L key
    M,            //!< Keyboard m and M key
    N,            //!< Keyboard n and N key
    O,            //!< Keyboard o and O key
    P,            //!< Keyboard p and P key
    Q,            //!< Keyboard q and Q key
    R,            //!< Keyboard r and R key
    S,            //!< Keyboard s and S key
    T,            //!< Keyboard t and T key
    U,            //!< Keyboard u and U key
    V,            //!< Keyboard v and V key
    W,            //!< Keyboard w and W key
    X,            //!< Keyboard x and X key
    Y,            //!< Keyboard y and Y key
    Z,            //!< Keyboard z and Z key
    Num1,         //!< Keyboard 1 and ! key
    Num2,         //!< Keyboard 2 and @ key
    Num3,         //!< Keyboard 3 and # key
    Num4,         //!< Keyboard 4 and $ key
    Num5,         //!< Keyboard 5 and % key
    Num6,         //!< Keyboard 6 and ^ key
    Num7,         //!< Keyboard 7 and & key
    Num8,         //!< Keyboard 8 and * key
    Num9,         //!< Keyboard 9 and ) key
    Num0,         //!< Keyboard 0 and ) key
    Enter,        //!< Keyboard Enter/Return key
    Escape,       //!< Keyboard Escape key
    Backspace,    //!< Keyboard Backspace key
    Tab,          //!< Keyboard Tab key
    Space,        //!< Keyboard Space key
    Hyphen,       //!< Keyboard - and _ key
    Equal,        //!< Keyboard = and +
    LBracket,     //!< Keyboard [ and { key
    RBracket,     //!< Keyboard ] and } key
    // For US keyboards mapped to key 29 (Microsoft Keyboard Scan Code Specification)
    // For Non-US keyboards mapped to key 42 (Microsoft Keyboard Scan Code Specification)
    // Typical language mappings: Belg:£µ` FrCa:<>} Dan:*' Dutch:`´ Fren:µ* Ger:'# Ital:§ù LatAm:[}` Nor:*@ Span:ç} Swed:*' Swiss:$£} UK:~# Brazil:}]
    Backslash,      //!< Keyboard \ and | key OR various keys for Non-US keyboards
    Semicolon,      //!< Keyboard ; and : key
    Apostrophe,     //!< Keyboard ' and " key
    Grave,          //!< Keyboard ` and ~ key
    Comma,          //!< Keyboard , and < key
    Period,         //!< Keyboard . and > key
    Slash,          //!< Keyboard / and ? key
    F1,             //!< Keyboard F1 key
    F2,             //!< Keyboard F2 key
    F3,             //!< Keyboard F3 key
    F4,             //!< Keyboard F4 key
    F5,             //!< Keyboard F5 key
    F6,             //!< Keyboard F6 key
    F7,             //!< Keyboard F7 key
    F8,             //!< Keyboard F8 key
    F9,             //!< Keyboard F9 key
    F10,            //!< Keyboard F10 key
    F11,            //!< Keyboard F11 key
    F12,            //!< Keyboard F12 key
    F13,            //!< Keyboard F13 key
    F14,            //!< Keyboard F14 key
    F15,            //!< Keyboard F15 key
    F16,            //!< Keyboard F16 key
    F17,            //!< Keyboard F17 key
    F18,            //!< Keyboard F18 key
    F19,            //!< Keyboard F19 key
    F20,            //!< Keyboard F20 key
    F21,            //!< Keyboard F21 key
    F22,            //!< Keyboard F22 key
    F23,            //!< Keyboard F23 key
    F24,            //!< Keyboard F24 key
    CapsLock,       //!< Keyboard Caps %Lock key
    PrintScreen,    //!< Keyboard Print Screen key
    ScrollLock,     //!< Keyboard Scroll %Lock key
    Pause,          //!< Keyboard Pause key
    Insert,         //!< Keyboard Insert key
    Home,           //!< Keyboard Home key
    PageUp,         //!< Keyboard Page Up key
    Delete,         //!< Keyboard Delete Forward key
    End,            //!< Keyboard End key
    PageDown,       //!< Keyboard Page Down key
    Right,          //!< Keyboard Right Arrow key
    Left,           //!< Keyboard Left Arrow key
    Down,           //!< Keyboard Down Arrow key
    Up,             //!< Keyboard Up Arrow key
    NumLock,        //!< Keypad Num %Lock and Clear key
    NumpadDivide,   //!< Keypad / key
    NumpadMultiply, //!< Keypad * key
    NumpadMinus,    //!< Keypad - key
    NumpadPlus,     //!< Keypad + key
    NumpadEqual,    //!< keypad = key
    NumpadEnter,    //!< Keypad Enter/Return key
    NumpadDecimal,  //!< Keypad . and Delete key
    Numpad1,        //!< Keypad 1 and End key
    Numpad2,        //!< Keypad 2 and Down Arrow key
    Numpad3,        //!< Keypad 3 and Page Down key
    Numpad4,        //!< Keypad 4 and Left Arrow key
    Numpad5,        //!< Keypad 5 key
    Numpad6,        //!< Keypad 6 and Right Arrow key
    Numpad7,        //!< Keypad 7 and Home key
    Numpad8,        //!< Keypad 8 and Up Arrow key
    Numpad9,        //!< Keypad 9 and Page Up key
    Numpad0,        //!< Keypad 0 and Insert key
    // For US keyboards doesn't exist
    // For Non-US keyboards mapped to key 45 (Microsoft Keyboard Scan Code Specification)
    // Typical language mappings: Belg:<\> FrCa:«°» Dan:<\> Dutch:]|[ Fren:<> Ger:<|> Ital:<> LatAm:<> Nor:<> Span:<> Swed:<|> Swiss:<\> UK:\| Brazil: \|.
    NonUsBackslash,     //!< Keyboard Non-US \ and | key
    Application,        //!< Keyboard Application key
    Execute,            //!< Keyboard Execute key
    ModeChange,         //!< Keyboard Mode Change key
    Help,               //!< Keyboard Help key
    Menu,               //!< Keyboard Menu key
    Select,             //!< Keyboard Select key
    Redo,               //!< Keyboard Redo key
    Undo,               //!< Keyboard Undo key
    Cut,                //!< Keyboard Cut key
    Copy,               //!< Keyboard Copy key
    Paste,              //!< Keyboard Paste key
    VolumeMute,         //!< Keyboard Volume Mute key
    VolumeUp,           //!< Keyboard Volume Up key
    VolumeDown,         //!< Keyboard Volume Down key
    MediaPlayPause,     //!< Keyboard Media Play Pause key
    MediaStop,          //!< Keyboard Media Stop key
    MediaNextTrack,     //!< Keyboard Media Next Track key
    MediaPreviousTrack, //!< Keyboard Media Previous Track key
    LControl,           //!< Keyboard Left Control key
    LShift,             //!< Keyboard Left Shift key
    LAlt,               //!< Keyboard Left Alt key
    LSystem,            //!< Keyboard Left System key
    RControl,           //!< Keyboard Right Control key
    RShift,             //!< Keyboard Right Shift key
    RAlt,               //!< Keyboard Right Alt key
    RSystem,            //!< Keyboard Right System key
    Back,               //!< Keyboard Back key
    Forward,            //!< Keyboard Forward key
    Refresh,            //!< Keyboard Refresh key
    Stop,               //!< Keyboard Stop key
    Search,             //!< Keyboard Search key
    Favorites,          //!< Keyboard Favorites key
    HomePage,           //!< Keyboard Home Page key
    LaunchApplication1, //!< Keyboard Launch Application 1 key
    LaunchApplication2, //!< Keyboard Launch Application 2 key
    LaunchMail,         //!< Keyboard Launch Mail key
    LaunchMediaSelect,  //!< Keyboard Launch Media Select key
};

using Scancode = Scan;

////////////////////////////////////////////////////////////
/// \brief The total number of scancodes, ignoring `Scan::Unknown`
///
////////////////////////////////////////////////////////////
// NOLINTNEXTLINE(readability-identifier-naming)
static constexpr unsigned int ScancodeCount{static_cast<unsigned int>(Scan::LaunchMediaSelect) + 1};

////////////////////////////////////////////////////////////
/// \brief Check if a key is pressed
///
/// \warning On macOS you're required to grant input monitoring access for
///          your application in order for `isKeyPressed` to work.
///
/// \param key Key to check
///
/// \return `true` if the key is pressed, `false` otherwise
///
////////////////////////////////////////////////////////////
[[nodiscard]] SFML_WINDOW_API bool isKeyPressed(Key key);

////////////////////////////////////////////////////////////
/// \brief Check if a key is pressed
///
/// \warning On macOS you're required to grant input monitoring access for
///          your application in order for `isKeyPressed` to work.
///
/// \param code Scancode to check
///
/// \return `true` if the physical key is pressed, `false` otherwise
///
////////////////////////////////////////////////////////////
[[nodiscard]] SFML_WINDOW_API bool isKeyPressed(Scancode code);

////////////////////////////////////////////////////////////
/// \brief Localize a physical key to a logical one
///
/// \param code Scancode to localize
///
/// \return The key corresponding to the scancode under the current
///         keyboard layout used by the operating system, or
///         `sf::Keyboard::Key::Unknown` when the scancode cannot be mapped
///         to a Key.
///
/// \see `delocalize`
///
////////////////////////////////////////////////////////////
[[nodiscard]] SFML_WINDOW_API Key localize(Scancode code);

////////////////////////////////////////////////////////////
/// \brief Identify the physical key corresponding to a logical one
///
/// \param key Key to "delocalize"
///
/// \return The scancode corresponding to the key under the current
///         keyboard layout used by the operating system, or
///         `sf::Keyboard::Scan::Unknown` when the key cannot be mapped
///         to a `sf::Keyboard::Scancode`.
///
/// \see `localize`
///
////////////////////////////////////////////////////////////
[[nodiscard]] SFML_WINDOW_API Scancode delocalize(Key key);

////////////////////////////////////////////////////////////
/// \brief Provide a string representation for a given scancode
///
/// The returned string is a short, non-technical description of
/// the key represented with the given scancode. Most effectively
/// used in user interfaces, as the description for the key takes
/// the users keyboard layout into consideration.
///
/// \warning The result is OS-dependent: for example, `sf::Keyboard::Scan::LSystem`
///          is "Left Meta" on Linux, "Left Windows" on Windows and
///          "Left Command" on macOS.
///
/// The current keyboard layout set by the operating system is used to
/// interpret the scancode: for example, `sf::Keyboard::Key::Semicolon` is
/// mapped to ";" for layout and to "é" for others.
///
/// \param code Scancode to check
///
/// \return The localized description of the code
///
////////////////////////////////////////////////////////////
[[nodiscard]] SFML_WINDOW_API String getDescription(Scancode code);

////////////////////////////////////////////////////////////
/// \brief Show or hide the virtual keyboard
///
/// \warning The virtual keyboard is not supported on all
///          systems. It will typically be implemented on mobile OSes
///          (Android, iOS) but not on desktop OSes (Windows, Linux, ...).
///
/// If the virtual keyboard is not available, this function does
/// nothing.
///
/// \param visible `true` to show, `false` to hide
///
////////////////////////////////////////////////////////////
SFML_WINDOW_API void setVirtualKeyboardVisible(bool visible);
} // namespace Keyboard

} // namespace sf


////////////////////////////////////////////////////////////
/// \namespace sf::Keyboard
/// \ingroup window
///
/// `sf::Keyboard` provides an interface to the state of the
/// keyboard.
///
/// This namespace allows users to query the keyboard state at any
/// time and directly, without having to deal with a window and
/// its events. Compared to the `KeyPressed` and `KeyReleased` events,
/// `sf::Keyboard` can retrieve the state of a key at any time
/// (you don't need to store and update a boolean on your side
/// in order to know if a key is pressed or released), and you
/// always get the real state of the keyboard, even if keys are
/// pressed or released when your window is out of focus and no
/// event is triggered.
///
/// Usage example:
/// \code
/// if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left))
/// {
///     // move left...
/// }
/// else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right))
/// {
///     // move right...
/// }
/// else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape))
/// {
///     // quit...
/// }
/// else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::Grave))
/// {
///     // open in-game command line (if it's not already open)
/// }
/// \endcode
///
/// \see `sf::Joystick`, `sf::Mouse`, `sf::Touch`
///
////////////////////////////////////////////////////////////
