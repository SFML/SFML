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
#include <SFML/Graphics/GraphicsContext.hpp>
#include <SFML/Window/Context.hpp>


namespace
{
    ////////////////////////////////////////////////////////////
    // One time initialization of 3rd party libraries.
    // We use a global function with a static boolean rather
    // than directly a global boolean, to avoid the randomness
    // of global variables initializations across compile units.
    ////////////////////////////////////////////////////////////
    void InitGraphicsLibs()
    {
        static bool InitDone = false;
        if (!InitDone)
        {
            // Initialize GLEW
            glewInit();

            InitDone = true;
        }
    }

}


namespace sf
{
namespace priv
{
////////////////////////////////////////////////////////////
/// Default constructor, activate the global context
/// if no other context is bound to the current thread
////////////////////////////////////////////////////////////
GraphicsContext::GraphicsContext()
{
    // Activate the global context
    if (!Context::IsContextActive())
    {
        Context::GetGlobal().SetActive(true);
        myActivated = true;
    }
    else
    {
        myActivated = false;
    }

    // Make sure third party libraries are initialized
    InitGraphicsLibs();
}


////////////////////////////////////////////////////////////
/// Destructor, deactivate the global context
/// if no other context was previously bound to the current thread
////////////////////////////////////////////////////////////
GraphicsContext::~GraphicsContext()
{
    // Deactivate the global context
    if (myActivated)
        Context::GetGlobal().SetActive(false);
}

} // namespace priv

} // namespace sf
