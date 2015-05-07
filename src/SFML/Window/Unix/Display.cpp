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
#include <SFML/System/Err.hpp>
#include <SFML/Window/Unix/Display.hpp>
#include <SFML/Window/Unix/ScopedXcbPtr.hpp>
#include <X11/keysym.h>
#include <cassert>
#include <cstdlib>
#include <algorithm>
#include <map>


namespace
{
    // The shared display and its reference counter
    Display* sharedDisplay = NULL;
    unsigned int referenceCount = 0;

    typedef std::map<std::string, xcb_atom_t> AtomMap;
    AtomMap atoms;

    bool mapBuilt = false;
    xcb_keycode_t firstKeycode = 255;
    xcb_keycode_t lastKeycode  = 0;

    // We use a simple array instead of a map => constant time lookup
    // xcb_keycode_t can only contain 256 distinct values
    xcb_keysym_t keysymMap[256];

    xcb_keysym_t keysymToLower(xcb_keysym_t keysym)
    {
        switch(keysym >> 8)
        {
            // Latin 1
            case 0:
            {
                if ((keysym >= XK_A) && (keysym <= XK_Z))
                    return keysym + (XK_a - XK_A);
                else if ((keysym >= XK_Agrave) && (keysym <= XK_Odiaeresis))
                    return keysym + (XK_agrave - XK_Agrave);
                else if ((keysym >= XK_Ooblique) && (keysym <= XK_Thorn))
                    return keysym + (XK_oslash - XK_Ooblique);
                break;
            }

            // Latin 2
            case 1:
            {
                if (keysym == XK_Aogonek)
                    return XK_aogonek;
                else if (keysym >= XK_Lstroke && keysym <= XK_Sacute)
                    return keysym + (XK_lstroke - XK_Lstroke);
                else if (keysym >= XK_Scaron && keysym <= XK_Zacute)
                    return keysym + (XK_scaron - XK_Scaron);
                else if (keysym >= XK_Zcaron && keysym <= XK_Zabovedot)
                    return keysym + (XK_zcaron - XK_Zcaron);
                else if (keysym >= XK_Racute && keysym <= XK_Tcedilla)
                    return keysym + (XK_racute - XK_Racute);
                break;
            }

            // Latin 3
            case 2:
            {
                if (keysym >= XK_Hstroke && keysym <= XK_Hcircumflex)
                    return keysym + (XK_hstroke - XK_Hstroke);
                else if (keysym >= XK_Gbreve && keysym <= XK_Jcircumflex)
                    return keysym + (XK_gbreve - XK_Gbreve);
                else if (keysym >= XK_Cabovedot && keysym <= XK_Scircumflex)
                    return keysym + (XK_cabovedot - XK_Cabovedot);
                break;
            }

            // Latin 4
            case 3:
            {
                if (keysym >= XK_Rcedilla && keysym <= XK_Tslash)
                    return keysym + (XK_rcedilla - XK_Rcedilla);
                else if (keysym == XK_ENG)
                    return XK_eng;
                else if (keysym >= XK_Amacron && keysym <= XK_Umacron)
                    return keysym + (XK_amacron - XK_Amacron);
                break;
            }

            // Cyrillic
            case 6:
            {
                if (keysym >= XK_Serbian_DJE && keysym <= XK_Serbian_DZE)
                    return keysym - (XK_Serbian_DJE - XK_Serbian_dje);
                else if (keysym >= XK_Cyrillic_YU && keysym <= XK_Cyrillic_HARDSIGN)
                    return keysym - (XK_Cyrillic_YU - XK_Cyrillic_yu);
                break;
            }

            // Greek
            case 7:
            {
                if (keysym >= XK_Greek_ALPHAaccent && keysym <= XK_Greek_OMEGAaccent)
                    return keysym + (XK_Greek_alphaaccent - XK_Greek_ALPHAaccent);
                else if (keysym >= XK_Greek_ALPHA && keysym <= XK_Greek_OMEGA)
                    return keysym + (XK_Greek_alpha - XK_Greek_ALPHA);
                break;
            }

            // Armenian
            case 0x14:
            {
                if (keysym >= XK_Armenian_AYB && keysym <= XK_Armenian_fe) {
                    return (keysym | 1);
                }
                break;
            }

            default:
            {
                break;
            }
        }

        return keysym;
    }

    void buildMap()
    {
        // Open a connection with the X server
        xcb_connection_t* connection = sf::priv::OpenConnection();

        firstKeycode = xcb_get_setup(connection)->min_keycode;
        lastKeycode = xcb_get_setup(connection)->max_keycode;

        sf::priv::ScopedXcbPtr<xcb_generic_error_t> error(NULL);

        sf::priv::ScopedXcbPtr<xcb_get_keyboard_mapping_reply_t> keyboardMapping(xcb_get_keyboard_mapping_reply(
            connection,
            xcb_get_keyboard_mapping(
                connection,
                firstKeycode,
                lastKeycode - firstKeycode + 1
            ),
            &error
        ));

        sf::priv::CloseConnection(connection);

        if (error || !keyboardMapping)
        {
            sf::err() << "Failed to get keyboard mapping" << std::endl;
            return;
        }

        uint8_t keysymsPerKeycode = keyboardMapping->keysyms_per_keycode;

        if (!keysymsPerKeycode)
        {
            sf::err() << "Error: No keysyms per keycode" << std::endl;
            return;
        }

        const xcb_keysym_t* keysyms = xcb_get_keyboard_mapping_keysyms(keyboardMapping.get());

        if (!keysyms)
        {
            sf::err() << "Failed to get keyboard mapping keysyms" << std::endl;
            return;
        }

        xcb_keycode_t range = lastKeycode - firstKeycode + 1;

        std::fill(keysymMap, keysymMap + 256, XK_VoidSymbol);

        for (xcb_keycode_t i = firstKeycode; ; ++i)
        {
            const xcb_keysym_t* keysym = &keysyms[(i - firstKeycode) * keysymsPerKeycode];

            if ((keysymsPerKeycode == 1) || (keysym[1] == XCB_NO_SYMBOL))
            {
                keysymMap[i] = keysymToLower(keysym[0]);

                if (i == lastKeycode)
                    break;

                continue;
            }

            keysymMap[i] = keysym[0];

            if (i == lastKeycode)
                break;
        }

        mapBuilt = true;
    }
}

namespace sf
{
namespace priv
{
////////////////////////////////////////////////////////////
Display* OpenDisplay()
{
    if (referenceCount == 0)
    {
        sharedDisplay = XOpenDisplay(NULL);

        // Opening display failed: The best we can do at the moment is to output a meaningful error message
        // and cause an abnormal program termination
        if (!sharedDisplay)
        {
            err() << "Failed to open X11 display; make sure the DISPLAY environment variable is set correctly" << std::endl;
            std::abort();
        }
    }

    referenceCount++;
    return sharedDisplay;
}


////////////////////////////////////////////////////////////
xcb_connection_t* OpenConnection()
{
    return XGetXCBConnection(OpenDisplay());
}


////////////////////////////////////////////////////////////
void CloseDisplay(Display* display)
{
    assert(display == sharedDisplay);

    referenceCount--;
    if (referenceCount == 0)
        XCloseDisplay(display);
}


////////////////////////////////////////////////////////////
void CloseConnection(xcb_connection_t* connection)
{
    assert(connection == XGetXCBConnection(sharedDisplay));
    return CloseDisplay(sharedDisplay);
}


////////////////////////////////////////////////////////////
xcb_screen_t* XCBScreenOfDisplay(xcb_connection_t* connection, int screen_nbr)
{
    xcb_screen_iterator_t iter = xcb_setup_roots_iterator(xcb_get_setup(connection));

    for (; iter.rem; --screen_nbr, xcb_screen_next (&iter))
    {
        if (screen_nbr == 0)
            return iter.data;
    }

    return NULL;
}


////////////////////////////////////////////////////////////
xcb_screen_t* XCBDefaultScreen(xcb_connection_t* connection)
{
    assert(connection == XGetXCBConnection(sharedDisplay));
    return XCBScreenOfDisplay(connection, XDefaultScreen(sharedDisplay));
}


////////////////////////////////////////////////////////////
xcb_window_t XCBDefaultRootWindow(xcb_connection_t* connection)
{
    assert(connection == XGetXCBConnection(sharedDisplay));
    xcb_screen_t* screen = XCBScreenOfDisplay(connection, XDefaultScreen(sharedDisplay));
    if (screen)
        return screen->root;
    return 0;
}


////////////////////////////////////////////////////////////
xcb_atom_t getAtom(const std::string& name, bool onlyIfExists)
{
    AtomMap::const_iterator iter = atoms.find(name);

    if (iter != atoms.end())
        return iter->second;

    ScopedXcbPtr<xcb_generic_error_t> error(NULL);

    xcb_connection_t* connection = OpenConnection();

    ScopedXcbPtr<xcb_intern_atom_reply_t> reply(xcb_intern_atom_reply(
        connection,
        xcb_intern_atom(
            connection,
            onlyIfExists,
            name.size(),
            name.c_str()
        ),
        &error
    ));

    CloseConnection(connection);

    if (error || !reply)
    {
        err() << "Failed to get " << name << " atom." << std::endl;
        return XCB_ATOM_NONE;
    }

    atoms[name] = reply->atom;

    return reply->atom;
}


////////////////////////////////////////////////////////////
const xcb_keysym_t* getKeysymMap()
{
    if (!mapBuilt)
        buildMap();

    return keysymMap;
}

} // namespace priv

} // namespace sf
