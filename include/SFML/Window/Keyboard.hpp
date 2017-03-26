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
#include <SFML/System/String.hpp>


namespace sf
{
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
    /// The enumerators are bound to a physical key and do *not* depend
    /// on the keyboard layout used by the operating system. Usually, the AT-101
    /// keyboard can be used as reference for the physical position of the keys.
    ///
    /// The scancodes are based on a subset of Table 12: Keyboard/Keypad Page
    /// of Universal Serial Bus (USB): HID Usage Tables, v1.12.
    ///
    /// \todo When porting this for SFML 3, remove the `s` prefix and use
    ///       enum class.
    ///
    ////////////////////////////////////////////////////////////
    enum Scancode
    {
        sUnknown = -1,      ///< Represents any scancode not present in this enum
        sA = 0,             ///< Keyboard a and A key
        sB,                 ///< Keyboard b and B key
        sC,                 ///< Keyboard c and C key
        sD,                 ///< Keyboard d and D key
        sE,                 ///< Keyboard e and E key
        sF,                 ///< Keyboard f and F key
        sG,                 ///< Keyboard g and G key
        sH,                 ///< Keyboard h and H key
        sI,                 ///< Keyboard i and I key
        sJ,                 ///< Keyboard j and J key
        sK,                 ///< Keyboard k and K key
        sL,                 ///< Keyboard l and L key
        sM,                 ///< Keyboard m and M key
        sN,                 ///< Keyboard n and N key
        sO,                 ///< Keyboard o and O key
        sP,                 ///< Keyboard p and P key
        sQ,                 ///< Keyboard q and Q key
        sR,                 ///< Keyboard r and R key
        sS,                 ///< Keyboard s and S key
        sT,                 ///< Keyboard t and T key
        sU,                 ///< Keyboard u and U key
        sV,                 ///< Keyboard v and V key
        sW,                 ///< Keyboard w and W key
        sX,                 ///< Keyboard x and X key
        sY,                 ///< Keyboard y and Y key
        sZ,                 ///< Keyboard z and Z key
        sNum1,              ///< Keyboard 1 and ! key
        sNum2,              ///< Keyboard 2 and @ key
        sNum3,              ///< Keyboard 3 and # key
        sNum4,              ///< Keyboard 4 and $ key
        sNum5,              ///< Keyboard 5 and % key
        sNum6,              ///< Keyboard 6 and ^ key
        sNum7,              ///< Keyboard 7 and & key
        sNum8,              ///< Keyboard 8 and * key
        sNum9,              ///< Keyboard 9 and ) key
        sNum0,              ///< Keyboard 0 and ) key
        sEnter,             ///< Keyboard Return (ENTER) key
        sEscape,            ///< Keyboard Escape key
        sBackspace,         ///< Keyboard Backspace key
        // TODO above it's BackSpace, but is it correct? What do we use here?
        sTab,               ///< Keyboard Tab key
        sSpace,             ///< Keyboard Space key
        sHyphen,            ///< Keyboard - and _ key
        sEquals,            ///< Keyboard = and +
        sLBracket,          ///< Keyboard [ and { key
        sRBracket,          ///< Keyboard ] and } key
        sBackslash,         ///< Keyboard \ and | key
        // TODO capitalisation
        sDash,              ///< Keyboard Non-US # and ~
        sSemicolon,         ///< Keyboard ; and : key
        // TODO capitalisation
        sQuote,             ///< Keyboard ' and " key
        sGraveAccent,       ///< Keyboard ` and ~ key
        sComma,             ///< Keyboard , and < key
        sPeriod,            ///< Keyboard . and > key
        sForwardSlash,      ///< Keyboard / and ? key
        sF1,                ///< Keyboard F1 key
        sF2,                ///< Keyboard F2 key
        sF3,                ///< Keyboard F3 key
        sF4,                ///< Keyboard F4 key
        sF5,                ///< Keyboard F5 key
        sF6,                ///< Keyboard F6 key
        sF7,                ///< Keyboard F7 key
        sF8,                ///< Keyboard F8 key
        sF9,                ///< Keyboard F9 key
        sF10,               ///< Keyboard F10 key
        sF11,               ///< Keyboard F11 key
        sF12,               ///< Keyboard F12 key
        sF13,               ///< Keyboard F13 key
        sF14,               ///< Keyboard F14 key
        sF15,               ///< Keyboard F15 key
        sCapsLock,          ///< Keyboard Caps Lock key
        sPrintScreen,       ///< Keyboard Print Screen key
        sScrollLock,        ///< Keyboard Scroll Lock key
        sPause,             ///< Keyboard Pause key
        sInsert,            ///< Keyboard Insert key
        sHome,              ///< Keyboard Home key
        sPageUp,            ///< Keyboard Page Up key
        sDelete,            ///< Keyboard Delete Forward key
        sEnd,               ///< Keyboard End key
        sPageDown,          ///< Keyboard Page Down key
        sRight,             ///< Keyboard Right Arrow key
        sLeft,              ///< Keyboard Left Arrow key
        sDown,              ///< Keyboard Down Arrow key
        sUp,                ///< Keyboard Up Arrow key
        sNumLock,           ///< Keypad Num Lock and Clear key
        sDivide,            ///< Keypad / key
        sMultiply,          ///< Keypad * key
        sMinus,             ///< Keypad - key
        sPlus,              ///< Keypad + key
        sPadEquals,         ///< keypad = key, probably Mac only
        sReturn,            ///< Keypad Enter (return) key
        sDecimal,           ///< Keypad . and Delete key
        sNumpad1,           ///< Keypad 1 and End key
        sNumpad2,           ///< Keypad 2 and Down Arrow key
        sNumpad3,           ///< Keypad 3 and Page Down key
        sNumpad4,           ///< Keypad 4 and Left Arrow key
        sNumpad5,           ///< Keypad 5 key
        sNumpad6,           ///< Keypad 6 and Right Arrow key
        sNumpad7,           ///< Keypad 7 and Home key
        sNumpad8,           ///< Keypad 8 and Up Arrow key
        sNumpad9,           ///< Keypad 9 and Page Up key
        sNumpad0,           ///< Keypad 0 and Insert key
        sReverseSolidus,    ///< Keyboard Non-US \ and | key
        // FIXME what is this one? Might need better name. The doc says:
        // - Typically near the Left-Shift key in AT-102 implementations.
        // - Typical language mappings: Belg:<\> FrCa:«°» Dan:<\> Dutch:]|[ Fren:<> Ger:<|> Ital:<> LatAm:<> Nor:<> Span:<> Swed:<|> Swiss:<\> UK:\| Brazil: \|.
        // What is the difference with "regular" \ and | key?
        sApplication,       ///< Keyboard Application key
        sExecute,           ///< Keyboard Execute key
        sHelp,              ///< Keyboard Help key
        sMenu,              ///< Keyboard Menu key
        sSelect,            ///< Keyboard Select key
        sStop,              ///< Keyboard Stop key
        sAgain,             ///< Keyboard Again key
        sUndo,              ///< Keyboard Undo key
        sCut,               ///< Keyboard Cut key
        sCopy,              ///< Keyboard Copy key
        sPaste,             ///< Keyboard Paste key
        sFind,              ///< Keyboard Find key
        sMute,              ///< Keyboard Mute key
        sVolumeUp,          ///< Keyboard Volume Up key
        sVolumeDown,        ///< Keyboard Volume Down key
        sLControl,          ///< Keyboard Left Control key
        sLShift,            ///< Keyboard Left Shift key
        sLAlt,              ///< Keyboard Left Alt key
        sLSystem,           ///< Keyboard Left System key
        sRControl,          ///< Keyboard Right Control key
        sRShift,            ///< Keyboard Right Shift key
        sRAlt,              ///< Keyboard Right Alt key
        sRSystem,           ///< Keyboard Right System key

        sCodeCount          ///< Keep last -- the total number of scancodes
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
    ///         sf::Keyboard::sUnknown when the key cannot be mapped
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
    /// \warning The result is OS-dependent: for example, sf::Keyboard::sLSystem
    ///          is "Left Meta" on Linux, "Left Windows" on Windows and
    ///          "Left Command" on macOS.
    ///
    /// The current keyboard layout set by the operating system is used to
    /// interpret the scancode: for example, sf::Keyboard::SemiColon is
    /// mapped to ";" for layout and to "é" for others.
    ///
    /// \return The localized description of the code
    ///
    ////////////////////////////////////////////////////////////
    static String localizedRepresentation(Scancode code);

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
