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
    ::Display* TheDisplay     = NULL;
    XIM        TheInputMethod = NULL;
    int        RefCount       = 0;
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
    if (TheDisplay == NULL)
    {
        TheDisplay = XOpenDisplay(NULL);
        if (TheDisplay)
        {
            // Create the input method object
            TheInputMethod = XOpenIM(TheDisplay, NULL, NULL, NULL);
        }
        else
        {
            std::cerr << "Failed to open a connection with the X server" << std::endl;
        }
    }

    // Increase the number of references
    RefCount++;
}


////////////////////////////////////////////////////////////
/// Copy constructor
////////////////////////////////////////////////////////////
DisplayRef::DisplayRef(const DisplayRef& Copy)
{
    // Increase the number of references
    RefCount++;
}


////////////////////////////////////////////////////////////
/// Destructor
////////////////////////////////////////////////////////////
DisplayRef::~DisplayRef()
{
    // Decrease the number of references
    RefCount--;

    // If all references have been destroyed, we can close the display
    if (RefCount == 0)
    {
        // Destroy the input method object
        if (TheInputMethod)
            XCloseIM(TheInputMethod);

        // Close the display
        XCloseDisplay(TheDisplay);
    }
}


////////////////////////////////////////////////////////////
/// Get the current display
////////////////////////////////////////////////////////////
::Display* DisplayRef::GetDisplay() const
{
    return TheDisplay;
}


////////////////////////////////////////////////////////////
/// Get the input method associated to the display
////////////////////////////////////////////////////////////
XIM DisplayRef::GetInputMethod() const
{
    return TheInputMethod;
}

} // namespace priv

} // namespace sf
