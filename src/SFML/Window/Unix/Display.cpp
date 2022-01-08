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

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/System/Err.hpp>
#include <SFML/Window/Unix/Display.hpp>
#include <X11/keysym.h>
#include <mutex>
#include <unordered_map>
#include <cassert>
#include <cstdlib>


namespace
{
    // The shared display and its reference counter
    Display* sharedDisplay = nullptr;
    unsigned int referenceCount = 0;
    XIM sharedXIM = NULL;
    unsigned int referenceCountXIM = 0;
    std::recursive_mutex mutex;

    using AtomMap = std::unordered_map<std::string, Atom>;
    AtomMap atoms;
}

namespace sf
{
namespace priv
{
////////////////////////////////////////////////////////////
Display* OpenDisplay()
{
    std::scoped_lock lock(mutex);

    if (referenceCount == 0)
    {
        sharedDisplay = XOpenDisplay(nullptr);

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
void CloseDisplay(Display* display)
{
    std::scoped_lock lock(mutex);

    assert(display == sharedDisplay);

    referenceCount--;
    if (referenceCount == 0)
        XCloseDisplay(display);
}

////////////////////////////////////////////////////////////
XIM OpenXIM()
{
    std::scoped_lock lock(mutex);

    assert(sharedDisplay != nullptr);

    if (referenceCountXIM == 0)
    {
        // Create a new XIM instance

        // We need the default (environment) locale and X locale for opening
        // the IM and properly receiving text
        // First save the previous ones (this might be able to be written more elegantly?)
        const char* p;
        std::string prevLoc((p = setlocale(LC_ALL, nullptr)) ? p : "");
        std::string prevXLoc((p = XSetLocaleModifiers(nullptr)) ? p : "");

        // Set the locales from environment
        setlocale(LC_ALL, "");
        XSetLocaleModifiers("");

        // Create the input context
        sharedXIM = XOpenIM(sharedDisplay, nullptr, nullptr, nullptr);

        // Restore the previous locale
        if (prevLoc.length() != 0)
            setlocale(LC_ALL, prevLoc.c_str());

        if (prevXLoc.length() != 0)
            XSetLocaleModifiers(prevXLoc.c_str());
    }

    referenceCountXIM++;

    return sharedXIM;
}

////////////////////////////////////////////////////////////
void CloseXIM(XIM xim)
{
    std::scoped_lock lock(mutex);

    assert(xim == sharedXIM);

    referenceCountXIM--;

    if ((referenceCountXIM == 0) && (xim != nullptr))
        XCloseIM(xim);
}

////////////////////////////////////////////////////////////
Atom getAtom(const std::string& name, bool onlyIfExists)
{
    if (auto it = atoms.find(name); it != atoms.end())
        return it->second;

    Display* display = OpenDisplay();

    Atom atom = XInternAtom(display, name.c_str(), onlyIfExists ? True : False);

    CloseDisplay(display);

    atoms[name] = atom;

    return atom;
}

} // namespace priv

} // namespace sf
