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

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Window/Unix/Display.hpp>

#include <SFML/System/Err.hpp>

#include <X11/keysym.h>

#include <mutex>
#include <ostream>
#include <unordered_map>

#include <cassert>
#include <clocale>
#include <cstdlib>


namespace
{
// A nested named namespace is used here to allow unity builds of SFML.
namespace UnixDisplayImpl
{
std::weak_ptr<Display> weakSharedDisplay;
std::recursive_mutex   mutex;
} // namespace UnixDisplayImpl
} // namespace


namespace sf::priv
{
////////////////////////////////////////////////////////////
std::shared_ptr<Display> openDisplay()
{
    const std::lock_guard lock(UnixDisplayImpl::mutex);

    auto sharedDisplay = UnixDisplayImpl::weakSharedDisplay.lock();
    if (!sharedDisplay)
    {
        sharedDisplay.reset(XOpenDisplay(nullptr), XCloseDisplay);
        UnixDisplayImpl::weakSharedDisplay = sharedDisplay;

        // Opening display failed: The best we can do at the moment is to output a meaningful error message
        // and cause an abnormal program termination
        if (!sharedDisplay)
        {
            err() << "Failed to open X11 display; make sure the DISPLAY environment variable is set correctly" << std::endl;
            std::abort();
        }
    }

    return sharedDisplay;
}


////////////////////////////////////////////////////////////
std::shared_ptr<_XIM> openXim()
{
    const std::lock_guard lock(UnixDisplayImpl::mutex);

    assert(!UnixDisplayImpl::weakSharedDisplay.expired() &&
           "Display is not initialized. Call priv::openDisplay() to initialize it.");

    static std::weak_ptr<_XIM> xim;

    auto sharedXIM = xim.lock();
    if (!sharedXIM)
    {
        // Create a new XIM instance

        // We need the default (environment) locale and X locale for opening
        // the IM and properly receiving text
        // First save the previous ones (this might be able to be written more elegantly?)
        const char*       p = nullptr;
        const std::string prevLoc((p = std::setlocale(LC_ALL, nullptr)) ? p : "");
        const std::string prevXLoc((p = XSetLocaleModifiers(nullptr)) ? p : "");

        // Set the locales from environment
        std::setlocale(LC_ALL, "");
        XSetLocaleModifiers("");

        // Create the input context
        const auto closeIM = [](XIM im)
        {
            if (im)
                XCloseIM(im);
        };
        sharedXIM.reset(XOpenIM(UnixDisplayImpl::weakSharedDisplay.lock().get(), nullptr, nullptr, nullptr), closeIM);
        xim = sharedXIM;

        // Restore the previous locale
        if (!prevLoc.empty())
            std::setlocale(LC_ALL, prevLoc.c_str());

        if (!prevXLoc.empty())
            XSetLocaleModifiers(prevXLoc.c_str());
    }

    return sharedXIM;
}


////////////////////////////////////////////////////////////
Atom getAtom(const std::string& name, bool onlyIfExists)
{
    static std::unordered_map<std::string, Atom> atoms;

    if (const auto it = atoms.find(name); it != atoms.end())
        return it->second;

    const auto display = openDisplay();
    const Atom atom    = XInternAtom(display.get(), name.c_str(), onlyIfExists ? True : False);
    if (atom)
        atoms[name] = atom;

    return atom;
}

} // namespace sf::priv
