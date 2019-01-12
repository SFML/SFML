////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007-2019 Laurent Gomila (laurent@sfml-dev.org)
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
#include <SFML/System/Mutex.hpp>
#include <SFML/System/Lock.hpp>
#include <SFML/Window/Unix/Display.hpp>
#include <X11/keysym.h>
#include <cassert>
#include <cstdlib>
#include <map>


namespace
{
    // The shared display and its reference counter
    Display* sharedDisplay = NULL;
    unsigned int referenceCount = 0;
    sf::Mutex mutex;

    typedef std::map<std::string, Atom> AtomMap;
    AtomMap atoms;
}

namespace sf
{
namespace priv
{
////////////////////////////////////////////////////////////
Display* OpenDisplay()
{
    Lock lock(mutex);

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
void CloseDisplay(Display* display)
{
    Lock lock(mutex);

    assert(display == sharedDisplay);

    referenceCount--;
    if (referenceCount == 0)
        XCloseDisplay(display);
}


////////////////////////////////////////////////////////////
Atom getAtom(const std::string& name, bool onlyIfExists)
{
    AtomMap::const_iterator iter = atoms.find(name);

    if (iter != atoms.end())
        return iter->second;

    Display* display = OpenDisplay();

    Atom atom = XInternAtom(display, name.c_str(), onlyIfExists ? True : False);

    CloseDisplay(display);

    atoms[name] = atom;

    return atom;
}

} // namespace priv

} // namespace sf
