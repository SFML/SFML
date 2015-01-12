////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007-2014 Laurent Gomila (laurent.gom@gmail.com)
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
#include <SFML/Graphics/GLExtensions.hpp>
#include <SFML/System/Err.hpp>


namespace sf
{
namespace priv
{
////////////////////////////////////////////////////////////
void ensureExtensionsInit()
{
#if !defined(SFML_OPENGL_ES)
    static bool initialized = false;
    if (!initialized)
    {
        int loaded = sfogl_LoadFunctions();

        if (!sfogl_IsVersionGEQ(1, 2))
        {
            err() << "sfml-graphics requires support for OpenGL 1.2 or greater" << std::endl;
            err() << "Ensure that hardware acceleration is enabled if available" << std::endl;
            return;
        }

        if (loaded == sfogl_LOAD_FAILED)
        {
            err() << "Failed to initialize OpenGL 1.2 entry points, ";
            err() << "number of functions that failed to load: " << loaded - sfogl_LOAD_SUCCEEDED << std::endl;
            return;
        }

        bool missing = false;

        if (sfogl_ext_EXT_blend_minmax == sfogl_LOAD_FAILED)
        {
            err() << "Required extension EXT_blend_minmax unavailable" << std::endl;
            missing = true;
        }

        if (sfogl_ext_EXT_blend_subtract == sfogl_LOAD_FAILED)
        {
            err() << "Required extension EXT_blend_subtract unavailable" << std::endl;
            missing = true;
        }

        if (!missing)
        {
            initialized = true;
        }
    }
#endif
}

} // namespace priv

} // namespace sf
