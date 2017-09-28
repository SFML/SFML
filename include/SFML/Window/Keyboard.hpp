////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007-2022 Laurent Gomila (laurent@sfml-dev.org)
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

#ifndef SFML_KEYBOARD_HPP
#define SFML_KEYBOARD_HPP

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
class SFML_WINDOW_API Keyboard
{
public:

    ////////////////////////////////////////////////////////////
    /// \brief Key codes
    ///
    /// The enumerators refer to the "localized" key; i.e. depending
    /// on the layout set by the operating system, a key can be mapped
    /// to `Y` or `Z`.
    ///
    ////////////////////////////////////////////////////////////
    enum Key
    {
        Unknown = -1, //!< Unhandled key
        A = 0,        //!< The A key
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
        LSystem,      //!< The left OS specific key: window (Windows and Linux), apple (MacOS X), ...
        RControl,     //!< The right Control key
        RShift,       //!< The right Shift key
        RAlt,         //!< The right Alt key
        RSystem,      //!< The right OS specific key: window (Windows and Linux), apple (MacOS X), ...
        Menu,         //!< The Menu key
        LBracket,     //!< The [ key
        RBracket,     //!< The ] key
        Semicolon,    //!< The ; key
        Comma,        //!< The , key
        Period,       //!< The . key
        Quote,        //!< The ' key
        Slash,        //!< The / key
        Backslash,    //!< The \ key
        Tilde,        //!< The ~ key
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

        KeyCount,     //!< Keep last -- the total number of keyboard keys

        // Deprecated values:

        Dash      = Hyphen,       //!< \deprecated Use Hyphen instead
        BackSpace = Backspace,    //!< \deprecated Use Backspace instead
        BackSlash = Backslash,    //!< \deprecated Use Backslash instead
        SemiColon = Semicolon,    //!< \deprecated Use Semicolon instead
        Return    = Enter         //!< \deprecated Use Enter instead
    };

    ////////////////////////////////////////////////////////////
    /// \brief Scancodes
    ///
    /// The enumerators are bound to a physical key and do not depend on
    /// the keyboard layout used by the operating system. Usually, the AT-101
    /// keyboard can be used as reference for the physical position of the keys.
    ///
    /// The scancodes are based on a subset of Table 12: Keyboard/Keypad Page
    /// of Universal Serial Bus (USB): HID Usage Tables, v1.12.
    ///
    /// \todo When porting this for SFML 3, remove the `Scan` prefix and use
    ///       enum class.
    ///
    ////////////////////////////////////////////////////////////
    enum Scancode
    {
        ScanUnknown = -1,      ///< Represents any scancode not present in this enum
        ScanA = 0,             ///< Keyboard a and A key
        ScanB,                 ///< Keyboard b and B key
        ScanC,                 ///< Keyboard c and C key
        ScanD,                 ///< Keyboard d and D key
        ScanE,                 ///< Keyboard e and E key
        ScanF,                 ///< Keyboard f and F key
        ScanG,                 ///< Keyboard g and G key
        ScanH,                 ///< Keyboard h and H key
        ScanI,                 ///< Keyboard i and I key
        ScanJ,                 ///< Keyboard j and J key
        ScanK,                 ///< Keyboard k and K key
        ScanL,                 ///< Keyboard l and L key
        ScanM,                 ///< Keyboard m and M key
        ScanN,                 ///< Keyboard n and N key
        ScanO,                 ///< Keyboard o and O key
        ScanP,                 ///< Keyboard p and P key
        ScanQ,                 ///< Keyboard q and Q key
        ScanR,                 ///< Keyboard r and R key
        ScanS,                 ///< Keyboard s and S key
        ScanT,                 ///< Keyboard t and T key
        ScanU,                 ///< Keyboard u and U key
        ScanV,                 ///< Keyboard v and V key
        ScanW,                 ///< Keyboard w and W key
        ScanX,                 ///< Keyboard x and X key
        ScanY,                 ///< Keyboard y and Y key
        ScanZ,                 ///< Keyboard z and Z key
        ScanNum1,              ///< Keyboard 1 and ! key
        ScanNum2,              ///< Keyboard 2 and @ key
        ScanNum3,              ///< Keyboard 3 and # key
        ScanNum4,              ///< Keyboard 4 and $ key
        ScanNum5,              ///< Keyboard 5 and % key
        ScanNum6,              ///< Keyboard 6 and ^ key
        ScanNum7,              ///< Keyboard 7 and & key
        ScanNum8,              ///< Keyboard 8 and * key
        ScanNum9,              ///< Keyboard 9 and ) key
        ScanNum0,              ///< Keyboard 0 and ) key
        ScanEnter,             ///< Keyboard Enter/Return key
        ScanEscape,            ///< Keyboard Escape key
        ScanBackspace,         ///< Keyboard Backspace key
        ScanTab,               ///< Keyboard Tab key
        ScanSpace,             ///< Keyboard Space key
        ScanHyphen,            ///< Keyboard - and _ key
        ScanEquals,            ///< Keyboard = and +
        ScanLBracket,          ///< Keyboard [ and { key
        ScanRBracket,          ///< Keyboard ] and } key
        ScanBackslash,         ///< Keyboard \ and | key
        ScanDash,              ///< Keyboard Non-US # and ~
        // TODO hyphen vs minus vs dash
        ScanSemicolon,         ///< Keyboard ; and : key
        ScanQuote,             ///< Keyboard ' and " key
        ScanGraveAccent,       ///< Keyboard ` and ~ key
        ScanComma,             ///< Keyboard , and < key
        ScanPeriod,            ///< Keyboard . and > key
        ScanSlash,             ///< Keyboard / and ? key
        ScanF1,                ///< Keyboard F1 key
        ScanF2,                ///< Keyboard F2 key
        ScanF3,                ///< Keyboard F3 key
        ScanF4,                ///< Keyboard F4 key
        ScanF5,                ///< Keyboard F5 key
        ScanF6,                ///< Keyboard F6 key
        ScanF7,                ///< Keyboard F7 key
        ScanF8,                ///< Keyboard F8 key
        ScanF9,                ///< Keyboard F9 key
        ScanF10,               ///< Keyboard F10 key
        ScanF11,               ///< Keyboard F11 key
        ScanF12,               ///< Keyboard F12 key
        ScanF13,               ///< Keyboard F13 key
        ScanF14,               ///< Keyboard F14 key
        ScanF15,               ///< Keyboard F15 key
        ScanCapsLock,          ///< Keyboard Caps Lock key
        ScanPrintScreen,       ///< Keyboard Print Screen key
        ScanScrollLock,        ///< Keyboard Scroll Lock key
        ScanPause,             ///< Keyboard Pause key
        ScanInsert,            ///< Keyboard Insert key
        ScanHome,              ///< Keyboard Home key
        ScanPageUp,            ///< Keyboard Page Up key
        ScanDelete,            ///< Keyboard Delete Forward key
        ScanEnd,               ///< Keyboard End key
        ScanPageDown,          ///< Keyboard Page Down key
        ScanRight,             ///< Keyboard Right Arrow key
        ScanLeft,              ///< Keyboard Left Arrow key
        ScanDown,              ///< Keyboard Down Arrow key
        ScanUp,                ///< Keyboard Up Arrow key
        ScanNumLock,           ///< Keypad Num Lock and Clear key
        ScanDivide,            ///< Keypad / key
        ScanMultiply,          ///< Keypad * key
        ScanMinus,             ///< Keypad - key
        ScanPlus,              ///< Keypad + key
        ScanNumpadEquals,      ///< keypad = key, probably Mac only
        ScanNumpadEnter,       ///< Keypad Enter/Return key
        ScanDecimal,           ///< Keypad . and Delete key
        ScanNumpad1,           ///< Keypad 1 and End key
        ScanNumpad2,           ///< Keypad 2 and Down Arrow key
        ScanNumpad3,           ///< Keypad 3 and Page Down key
        ScanNumpad4,           ///< Keypad 4 and Left Arrow key
        ScanNumpad5,           ///< Keypad 5 key
        ScanNumpad6,           ///< Keypad 6 and Right Arrow key
        ScanNumpad7,           ///< Keypad 7 and Home key
        ScanNumpad8,           ///< Keypad 8 and Up Arrow key
        ScanNumpad9,           ///< Keypad 9 and Page Up key
        ScanNumpad0,           ///< Keypad 0 and Insert key
        ScanReverseSolidus,    ///< Keyboard Non-US \ and | key
        // FIXME what is this one? Might need better name. The doc says:
        // - Typically near the Left-Shift key in AT-102 implementations.
        // - Typical language mappings: Belg:<\> FrCa:«°» Dan:<\> Dutch:]|[ Fren:<> Ger:<|> Ital:<> LatAm:<> Nor:<> Span:<> Swed:<|> Swiss:<\> UK:\| Brazil: \|.
        // What is the difference with "regular" \ and | key?
        ScanApplication,       ///< Keyboard Application key
        ScanExecute,           ///< Keyboard Execute key
        ScanHelp,              ///< Keyboard Help key
        ScanMenu,              ///< Keyboard Menu key
        ScanSelect,            ///< Keyboard Select key
        ScanStop,              ///< Keyboard Stop key
        ScanAgain,             ///< Keyboard Again key
        ScanUndo,              ///< Keyboard Undo key
        ScanCut,               ///< Keyboard Cut key
        ScanCopy,              ///< Keyboard Copy key
        ScanPaste,             ///< Keyboard Paste key
        ScanFind,              ///< Keyboard Find key
        ScanMute,              ///< Keyboard Mute key
        ScanVolumeUp,          ///< Keyboard Volume Up key
        ScanVolumeDown,        ///< Keyboard Volume Down key
        ScanLControl,          ///< Keyboard Left Control key
        ScanLShift,            ///< Keyboard Left Shift key
        ScanLAlt,              ///< Keyboard Left Alt key
        ScanLSystem,           ///< Keyboard Left System key
        ScanRControl,          ///< Keyboard Right Control key
        ScanRShift,            ///< Keyboard Right Shift key
        ScanRAlt,              ///< Keyboard Right Alt key
        ScanRSystem,           ///< Keyboard Right System key

        ScanCodeCount          ///< Keep last -- the total number of scancodes
    };

    ////////////////////////////////////////////////////////////
    /// \brief Check if a key is pressed
    ///
    /// \param key Key to check
    ///
    /// \return True if the key is pressed, false otherwise
    ///
    ////////////////////////////////////////////////////////////
    static bool isKeyPressed(Key key);

    ////////////////////////////////////////////////////////////
    /// \brief Check if a key is pressed
    ///
    /// \param code Scancode to check
    ///
    /// \return True if the physical key is pressed, false otherwise
    ///
    ////////////////////////////////////////////////////////////
    static bool isKeyPressed(Scancode code);

    ////////////////////////////////////////////////////////////
    /// \brief Localize a physical key to a logical one
    ///
    /// \param code Scancode to localize
    ///
    /// \return The key corresponding to the scancode under the current
    ///         keyboard layout used by the operating system, or
    ///         sf::Keyboard::Unknown when the scancode cannot be mapped
    ///         to a Key.
    ///
    /// \see unlocalize
    ///
    ////////////////////////////////////////////////////////////
    static Key localize(Scancode code);

    ////////////////////////////////////////////////////////////
    /// \brief Identify the physical key corresponding to a logical one
    ///
    /// \param key Key to "unlocalize"
    ///
    /// \return The scancode corresponding to the key under the current
    ///         keyboard layout used by the operating system, or
    ///         sf::Keyboard::ScanUnknown when the key cannot be mapped
    ///         to a Keyboard::Scancode.
    ///
    /// \see localize
    ///
    ////////////////////////////////////////////////////////////
    static Scancode unlocalize(Key key);

    ////////////////////////////////////////////////////////////
    /// \brief Provide a string representation for a given scancode
    ///
    /// From a high level point of view, this conversion corresponds
    /// somewhat to the string available through sf::Event::TextEvent
    /// when the given physical key is pressed by the user, when no
    /// modifiers are involved.
    ///
    /// \warning The result is OS-dependent: for example, sf::Keyboard::ScanLSystem
    ///          is "Left Meta" on Linux, "Left Windows" on Windows and
    ///          "Left Command" on macOS.
    ///
    /// The current keyboard layout set by the operating system is used to
    /// interpret the scancode: for example, sf::Keyboard::Semicolon is
    /// mapped to ";" for layout and to "é" for others.
    ///
    /// \return The localized description of the code
    ///
    ////////////////////////////////////////////////////////////
    static String getDescription(Scancode code);

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
    /// \param visible True to show, false to hide
    ///
    ////////////////////////////////////////////////////////////
    static void setVirtualKeyboardVisible(bool visible);
};

} // namespace sf


#endif // SFML_KEYBOARD_HPP


////////////////////////////////////////////////////////////
/// \class sf::Keyboard
/// \ingroup window
///
/// sf::Keyboard provides an interface to the state of the
/// keyboard. It only contains static functions (a single
/// keyboard is assumed), so it's not meant to be instantiated.
///
/// This class allows users to query the keyboard state at any
/// time and directly, without having to deal with a window and
/// its events. Compared to the KeyPressed and KeyReleased events,
/// sf::Keyboard can retrieve the state of a key at any time
/// (you don't need to store and update a boolean on your side
/// in order to know if a key is pressed or released), and you
/// always get the real state of the keyboard, even if keys are
/// pressed or released when your window is out of focus and no
/// event is triggered.
///
/// Usage example:
/// \code
/// if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
/// {
///     // move left...
/// }
/// else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
/// {
///     // move right...
/// }
/// else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
/// {
///     // quit...
/// }
/// \endcode
///
/// \see sf::Joystick, sf::Mouse, sf::Touch
///
////////////////////////////////////////////////////////////
