////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007-2009 Laurent Gomila (laurent.gom@gmail.com)
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
#include <SFML/Window/Linux/DisplayRef.hpp>
#include <iostream>


////////////////////////////////////////////////////////////
// Private data
////////////////////////////////////////////////////////////
namespace
{
    ::Display* theDisplay     = NULL;
    XIM        theInputMethod = NULL;
    int        refCount       = 0;
}


namespace sf
{
namespace priv
{
////////////////////////////////////////////////////////////
/// Default constructor
////////////////////////////////////////////////////////////
DisplayRef::DisplayRef()
{
    // If the display hasn't been opened yet, open it
    if (theDisplay == NULL)
    {
        theDisplay = XOpenDisplay(NULL);
        if (theDisplay)
        {
            // Create the input method object
            theInputMethod = XOpenIM(theDisplay, NULL, NULL, NULL);
        }
        else
        {
            std::cerr << "Failed to open a connection with the X server" << std::endl;
        }
    }

    // Increase the number of references
    refCount++;
}


////////////////////////////////////////////////////////////
/// Copy constructor
////////////////////////////////////////////////////////////
DisplayRef::DisplayRef(const DisplayRef&)
{
    // Increase the number of references
    refCount++;
}


////////////////////////////////////////////////////////////
/// Destructor
////////////////////////////////////////////////////////////
DisplayRef::~DisplayRef()
{
    // Decrease the number of references
    refCount--;

    // If all references have been destroyed, we can close the display
    if (refCount == 0)
    {
        // Destroy the input method object
        if (theInputMethod)
            XCloseIM(theInputMethod);

        // Close the display
        XCloseDisplay(theDisplay);
    }
}


////////////////////////////////////////////////////////////
/// Get the current display
////////////////////////////////////////////////////////////
::Display* DisplayRef::GetDisplay() const
{
    return theDisplay;
}


////////////////////////////////////////////////////////////
/// Get the input method associated to the display
////////////////////////////////////////////////////////////
XIM DisplayRef::GetInputMethod() const
{
    return theInputMethod;
}

} // namespace priv

} // namespace sf
