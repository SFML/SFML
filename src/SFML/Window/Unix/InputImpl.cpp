////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007-2015 Laurent Gomila (laurent@sfml-dev.org)
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

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Window/Window.hpp> // important to be included first (conflict with None)
#include <SFML/Window/Unix/InputImpl.hpp>
#include <SFML/Window/Unix/Display.hpp>
#include <SFML/Window/Unix/ScopedXcbPtr.hpp>
#include <SFML/System/Err.hpp>
#include <xcb/xcb_keysyms.h>
#include <X11/Xlib-xcb.h>
#include <X11/keysym.h>
#include <cstdlib>

////////////////////////////////////////////////////////////
// Private data
////////////////////////////////////////////////////////////
namespace
{
    bool mapBuilt = false;

    // We use a simple array instead of a map => constant time lookup
    xcb_keycode_t keycodeMap[sf::Keyboard::KeyCount];

    xcb_keycode_t getKeycode(xcb_key_symbols_t* keySymbols, xcb_keysym_t keysym)
    {
        // keycodes is actually a vector of keycodes
        // Since we only care about the unmodified keycode,
        // we simply return the first one
        sf::priv::ScopedXcbPtr<xcb_keycode_t> keycodes(xcb_key_symbols_get_keycode(keySymbols, keysym));

        if (keycodes)
            return *keycodes.get();

        return 0;
    }

    void buildMap()
    {
        // Open a connection with the X server
        xcb_connection_t* connection = sf::priv::OpenConnection();

        xcb_key_symbols_t* keySymbols = xcb_key_symbols_alloc(connection);

        if (!keySymbols)
        {
            sf::err() << "Failed to allocate key symbols" << std::endl;
            return;
        }

        keycodeMap[sf::Keyboard::A]         = getKeycode(keySymbols, XK_A);
        keycodeMap[sf::Keyboard::B]         = getKeycode(keySymbols, XK_B);
        keycodeMap[sf::Keyboard::C]         = getKeycode(keySymbols, XK_C);
        keycodeMap[sf::Keyboard::D]         = getKeycode(keySymbols, XK_D);
        keycodeMap[sf::Keyboard::E]         = getKeycode(keySymbols, XK_E);
        keycodeMap[sf::Keyboard::F]         = getKeycode(keySymbols, XK_F);
        keycodeMap[sf::Keyboard::G]         = getKeycode(keySymbols, XK_G);
        keycodeMap[sf::Keyboard::H]         = getKeycode(keySymbols, XK_H);
        keycodeMap[sf::Keyboard::I]         = getKeycode(keySymbols, XK_I);
        keycodeMap[sf::Keyboard::J]         = getKeycode(keySymbols, XK_J);
        keycodeMap[sf::Keyboard::K]         = getKeycode(keySymbols, XK_K);
        keycodeMap[sf::Keyboard::L]         = getKeycode(keySymbols, XK_L);
        keycodeMap[sf::Keyboard::M]         = getKeycode(keySymbols, XK_M);
        keycodeMap[sf::Keyboard::N]         = getKeycode(keySymbols, XK_N);
        keycodeMap[sf::Keyboard::O]         = getKeycode(keySymbols, XK_O);
        keycodeMap[sf::Keyboard::P]         = getKeycode(keySymbols, XK_P);
        keycodeMap[sf::Keyboard::Q]         = getKeycode(keySymbols, XK_Q);
        keycodeMap[sf::Keyboard::R]         = getKeycode(keySymbols, XK_R);
        keycodeMap[sf::Keyboard::S]         = getKeycode(keySymbols, XK_S);
        keycodeMap[sf::Keyboard::T]         = getKeycode(keySymbols, XK_T);
        keycodeMap[sf::Keyboard::U]         = getKeycode(keySymbols, XK_U);
        keycodeMap[sf::Keyboard::V]         = getKeycode(keySymbols, XK_V);
        keycodeMap[sf::Keyboard::W]         = getKeycode(keySymbols, XK_W);
        keycodeMap[sf::Keyboard::X]         = getKeycode(keySymbols, XK_X);
        keycodeMap[sf::Keyboard::Y]         = getKeycode(keySymbols, XK_Y);
        keycodeMap[sf::Keyboard::Z]         = getKeycode(keySymbols, XK_Z);
        keycodeMap[sf::Keyboard::Num0]      = getKeycode(keySymbols, XK_0);
        keycodeMap[sf::Keyboard::Num1]      = getKeycode(keySymbols, XK_1);
        keycodeMap[sf::Keyboard::Num2]      = getKeycode(keySymbols, XK_2);
        keycodeMap[sf::Keyboard::Num3]      = getKeycode(keySymbols, XK_3);
        keycodeMap[sf::Keyboard::Num4]      = getKeycode(keySymbols, XK_4);
        keycodeMap[sf::Keyboard::Num5]      = getKeycode(keySymbols, XK_5);
        keycodeMap[sf::Keyboard::Num6]      = getKeycode(keySymbols, XK_6);
        keycodeMap[sf::Keyboard::Num7]      = getKeycode(keySymbols, XK_7);
        keycodeMap[sf::Keyboard::Num8]      = getKeycode(keySymbols, XK_8);
        keycodeMap[sf::Keyboard::Num9]      = getKeycode(keySymbols, XK_9);
        keycodeMap[sf::Keyboard::Escape]    = getKeycode(keySymbols, XK_Escape);
        keycodeMap[sf::Keyboard::LControl]  = getKeycode(keySymbols, XK_Control_L);
        keycodeMap[sf::Keyboard::LShift]    = getKeycode(keySymbols, XK_Shift_L);
        keycodeMap[sf::Keyboard::LAlt]      = getKeycode(keySymbols, XK_Alt_L);
        keycodeMap[sf::Keyboard::LSystem]   = getKeycode(keySymbols, XK_Super_L);
        keycodeMap[sf::Keyboard::RControl]  = getKeycode(keySymbols, XK_Control_R);
        keycodeMap[sf::Keyboard::RShift]    = getKeycode(keySymbols, XK_Shift_R);
        keycodeMap[sf::Keyboard::RAlt]      = getKeycode(keySymbols, XK_Alt_R);
        keycodeMap[sf::Keyboard::RSystem]   = getKeycode(keySymbols, XK_Super_R);
        keycodeMap[sf::Keyboard::Menu]      = getKeycode(keySymbols, XK_Menu);
        keycodeMap[sf::Keyboard::LBracket]  = getKeycode(keySymbols, XK_bracketleft);
        keycodeMap[sf::Keyboard::RBracket]  = getKeycode(keySymbols, XK_bracketright);
        keycodeMap[sf::Keyboard::SemiColon] = getKeycode(keySymbols, XK_semicolon);
        keycodeMap[sf::Keyboard::Comma]     = getKeycode(keySymbols, XK_comma);
        keycodeMap[sf::Keyboard::Period]    = getKeycode(keySymbols, XK_period);
        keycodeMap[sf::Keyboard::Quote]     = getKeycode(keySymbols, XK_apostrophe);
        keycodeMap[sf::Keyboard::Slash]     = getKeycode(keySymbols, XK_slash);
        keycodeMap[sf::Keyboard::BackSlash] = getKeycode(keySymbols, XK_backslash);
        keycodeMap[sf::Keyboard::Tilde]     = getKeycode(keySymbols, XK_grave);
        keycodeMap[sf::Keyboard::Equal]     = getKeycode(keySymbols, XK_equal);
        keycodeMap[sf::Keyboard::Dash]      = getKeycode(keySymbols, XK_minus);
        keycodeMap[sf::Keyboard::Space]     = getKeycode(keySymbols, XK_space);
        keycodeMap[sf::Keyboard::Return]    = getKeycode(keySymbols, XK_Return);
        keycodeMap[sf::Keyboard::BackSpace] = getKeycode(keySymbols, XK_BackSpace);
        keycodeMap[sf::Keyboard::Tab]       = getKeycode(keySymbols, XK_Tab);
        keycodeMap[sf::Keyboard::PageUp]    = getKeycode(keySymbols, XK_Prior);
        keycodeMap[sf::Keyboard::PageDown]  = getKeycode(keySymbols, XK_Next);
        keycodeMap[sf::Keyboard::End]       = getKeycode(keySymbols, XK_End);
        keycodeMap[sf::Keyboard::Home]      = getKeycode(keySymbols, XK_Home);
        keycodeMap[sf::Keyboard::Insert]    = getKeycode(keySymbols, XK_Insert);
        keycodeMap[sf::Keyboard::Delete]    = getKeycode(keySymbols, XK_Delete);
        keycodeMap[sf::Keyboard::Add]       = getKeycode(keySymbols, XK_KP_Add);
        keycodeMap[sf::Keyboard::Subtract]  = getKeycode(keySymbols, XK_KP_Subtract);
        keycodeMap[sf::Keyboard::Multiply]  = getKeycode(keySymbols, XK_KP_Multiply);
        keycodeMap[sf::Keyboard::Divide]    = getKeycode(keySymbols, XK_KP_Divide);
        keycodeMap[sf::Keyboard::Left]      = getKeycode(keySymbols, XK_Left);
        keycodeMap[sf::Keyboard::Right]     = getKeycode(keySymbols, XK_Right);
        keycodeMap[sf::Keyboard::Up]        = getKeycode(keySymbols, XK_Up);
        keycodeMap[sf::Keyboard::Down]      = getKeycode(keySymbols, XK_Down);
        keycodeMap[sf::Keyboard::Numpad0]   = getKeycode(keySymbols, XK_KP_0);
        keycodeMap[sf::Keyboard::Numpad1]   = getKeycode(keySymbols, XK_KP_1);
        keycodeMap[sf::Keyboard::Numpad2]   = getKeycode(keySymbols, XK_KP_2);
        keycodeMap[sf::Keyboard::Numpad3]   = getKeycode(keySymbols, XK_KP_3);
        keycodeMap[sf::Keyboard::Numpad4]   = getKeycode(keySymbols, XK_KP_4);
        keycodeMap[sf::Keyboard::Numpad5]   = getKeycode(keySymbols, XK_KP_5);
        keycodeMap[sf::Keyboard::Numpad6]   = getKeycode(keySymbols, XK_KP_6);
        keycodeMap[sf::Keyboard::Numpad7]   = getKeycode(keySymbols, XK_KP_7);
        keycodeMap[sf::Keyboard::Numpad8]   = getKeycode(keySymbols, XK_KP_8);
        keycodeMap[sf::Keyboard::Numpad9]   = getKeycode(keySymbols, XK_KP_9);
        keycodeMap[sf::Keyboard::F1]        = getKeycode(keySymbols, XK_F1);
        keycodeMap[sf::Keyboard::F2]        = getKeycode(keySymbols, XK_F2);
        keycodeMap[sf::Keyboard::F3]        = getKeycode(keySymbols, XK_F3);
        keycodeMap[sf::Keyboard::F4]        = getKeycode(keySymbols, XK_F4);
        keycodeMap[sf::Keyboard::F5]        = getKeycode(keySymbols, XK_F5);
        keycodeMap[sf::Keyboard::F6]        = getKeycode(keySymbols, XK_F6);
        keycodeMap[sf::Keyboard::F7]        = getKeycode(keySymbols, XK_F7);
        keycodeMap[sf::Keyboard::F8]        = getKeycode(keySymbols, XK_F8);
        keycodeMap[sf::Keyboard::F9]        = getKeycode(keySymbols, XK_F9);
        keycodeMap[sf::Keyboard::F10]       = getKeycode(keySymbols, XK_F10);
        keycodeMap[sf::Keyboard::F11]       = getKeycode(keySymbols, XK_F11);
        keycodeMap[sf::Keyboard::F12]       = getKeycode(keySymbols, XK_F12);
        keycodeMap[sf::Keyboard::F13]       = getKeycode(keySymbols, XK_F13);
        keycodeMap[sf::Keyboard::F14]       = getKeycode(keySymbols, XK_F14);
        keycodeMap[sf::Keyboard::F15]       = getKeycode(keySymbols, XK_F15);
        keycodeMap[sf::Keyboard::Pause]     = getKeycode(keySymbols, XK_Pause);

        xcb_key_symbols_free(keySymbols);

        // Close the connection with the X server
        sf::priv::CloseConnection(connection);

        mapBuilt = true;
    }
}


namespace sf
{
namespace priv
{
////////////////////////////////////////////////////////////
bool InputImpl::isKeyPressed(Keyboard::Key key)
{
    if (!mapBuilt)
        buildMap();

    // Sanity checks
    if (key < 0 || key >= sf::Keyboard::KeyCount)
        return false;

    // Convert to keycode
    xcb_keycode_t keycode = keycodeMap[key];

    ScopedXcbPtr<xcb_generic_error_t> error(NULL);

    // Open a connection with the X server
    xcb_connection_t* connection = OpenConnection();

    // Get the whole keyboard state
    ScopedXcbPtr<xcb_query_keymap_reply_t> keymap(
        xcb_query_keymap_reply(
            connection,
            xcb_query_keymap(connection),
            &error
        )
    );

    // Close the connection with the X server
    CloseConnection(connection);

    if (error)
    {
        err() << "Failed to query keymap" << std::endl;

        return false;
    }

    // Check our keycode
    return (keymap->keys[keycode / 8] & (1 << (keycode % 8))) != 0;
}


////////////////////////////////////////////////////////////
void InputImpl::setVirtualKeyboardVisible(bool /*visible*/)
{
    // Not applicable
}


////////////////////////////////////////////////////////////
bool InputImpl::isMouseButtonPressed(Mouse::Button button)
{
    // Open a connection with the X server
    xcb_connection_t* connection = OpenConnection();

    ScopedXcbPtr<xcb_generic_error_t> error(NULL);

    // Get pointer mask
    ScopedXcbPtr<xcb_query_pointer_reply_t> pointer(
        xcb_query_pointer_reply(
            connection,
            xcb_query_pointer(
                connection,
                XCBDefaultRootWindow(connection)
            ),
            &error
        )
    );

    // Close the connection with the X server
    CloseConnection(connection);

    if (error)
    {
        err() << "Failed to query pointer" << std::endl;

        return false;
    }

    uint16_t buttons = pointer->mask;

    switch (button)
    {
        case Mouse::Left:     return buttons & XCB_BUTTON_MASK_1;
        case Mouse::Right:    return buttons & XCB_BUTTON_MASK_3;
        case Mouse::Middle:   return buttons & XCB_BUTTON_MASK_2;
        case Mouse::XButton1: return false; // not supported by X
        case Mouse::XButton2: return false; // not supported by X
        default:              return false;
    }
}


////////////////////////////////////////////////////////////
Vector2i InputImpl::getMousePosition()
{
    // Open a connection with the X server
    xcb_connection_t* connection = OpenConnection();

    ScopedXcbPtr<xcb_generic_error_t> error(NULL);

    ScopedXcbPtr<xcb_query_pointer_reply_t> pointer(
        xcb_query_pointer_reply(
            connection,
            xcb_query_pointer(
                connection,
                XCBDefaultRootWindow(connection)
            ),
            &error
        )
    );

    // Close the connection with the X server
    CloseConnection(connection);

    if (error)
    {
        err() << "Failed to query pointer" << std::endl;

        return Vector2i(0, 0);
    }

    return Vector2i(pointer->root_x, pointer->root_y);
}


////////////////////////////////////////////////////////////
Vector2i InputImpl::getMousePosition(const Window& relativeTo)
{
    WindowHandle handle = relativeTo.getSystemHandle();
    if (handle)
    {
        // Open a connection with the X server
        xcb_connection_t* connection = OpenConnection();

        ScopedXcbPtr<xcb_generic_error_t> error(NULL);

        ScopedXcbPtr<xcb_query_pointer_reply_t> pointer(
            xcb_query_pointer_reply(
                connection,
                xcb_query_pointer(
                    connection,
                    XCBDefaultRootWindow(connection)
                ),
                &error
            )
        );

        // Close the connection with the X server
        CloseConnection(connection);

        if (error)
        {
            err() << "Failed to query pointer" << std::endl;

            return Vector2i(0, 0);
        }

        return Vector2i(pointer->win_x, pointer->win_y);
    }
    else
    {
        return Vector2i();
    }
}


////////////////////////////////////////////////////////////
void InputImpl::setMousePosition(const Vector2i& position)
{
    // Open a connection with the X server
    xcb_connection_t* connection = OpenConnection();

    ScopedXcbPtr<xcb_generic_error_t> error(xcb_request_check(
        connection,
        xcb_warp_pointer(
            connection,
            None,                             // Source window
            XCBDefaultRootWindow(connection), // Destination window
            0, 0,                             // Source position
            0, 0,                             // Source size
            position.x, position.y            // Destination position
        )
    ));

    if (error)
        err() << "Failed to set mouse position" << std::endl;

    xcb_flush(connection);

    // Close the connection with the X server
    CloseConnection(connection);
}


////////////////////////////////////////////////////////////
void InputImpl::setMousePosition(const Vector2i& position, const Window& relativeTo)
{
    // Open a connection with the X server
    xcb_connection_t* connection = OpenConnection();

    WindowHandle handle = relativeTo.getSystemHandle();
    if (handle)
    {
        ScopedXcbPtr<xcb_generic_error_t> error(xcb_request_check(
            connection,
            xcb_warp_pointer(
                connection,
                None,                  // Source window
                handle,                // Destination window
                0, 0,                  // Source position
                0, 0,                  // Source size
                position.x, position.y // Destination position
            )
        ));

        if (error)
            err() << "Failed to set mouse position" << std::endl;

        xcb_flush(connection);
    }

    // Close the connection with the X server
    CloseConnection(connection);
}


////////////////////////////////////////////////////////////
bool InputImpl::isTouchDown(unsigned int /*finger*/)
{
    // Not applicable
    return false;
}


////////////////////////////////////////////////////////////
Vector2i InputImpl::getTouchPosition(unsigned int /*finger*/)
{
    // Not applicable
    return Vector2i();
}


////////////////////////////////////////////////////////////
Vector2i InputImpl::getTouchPosition(unsigned int /*finger*/, const Window& /*relativeTo*/)
{
    // Not applicable
    return Vector2i();
}

} // namespace priv

} // namespace sf
