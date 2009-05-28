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

#ifndef SFML_OPENGL_H
#define SFML_OPENGL_H


////////////////////////////////////////////////////////////
/// Headers
////////////////////////////////////////////////////////////
#include <SFML/Config.h>


////////////////////////////////////////////////////////////
/// This file just includes the OpenGL (GL and GLU) headers,
/// which have actually different paths on each system
////////////////////////////////////////////////////////////
#if defined(CSFML_SYSTEM_WINDOWS)

    #include <windows.h>
    #include <GL/gl.h>
    #include <GL/glu.h>

#elif defined(CSFML_SYSTEM_LINUX) || defined(CSFML_SYSTEM_FREEBSD)

    #include <GL/gl.h>
    #include <GL/glu.h>

#elif defined(CSFML_SYSTEM_MACOS)

    #include <OpenGL/gl.h>
    #include <OpenGL/glu.h>

#endif


#endif // SFML_OPENGL_H
