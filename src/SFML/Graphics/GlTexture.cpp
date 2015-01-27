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
#include <SFML/Graphics/GlTexture.hpp>
#include <SFML/Graphics/GLCheck.hpp>
#include <SFML/Window/Context.hpp>
#include <SFML/System/Mutex.hpp>
#include <SFML/System/Lock.hpp>
#include <SFML/System/Err.hpp>
#include <cassert>
#include <cstring>


namespace
{
    sf::Mutex mutex;

    unsigned int checkMaximumTextureSize()
    {
        // Create a temporary context in case the user queries
        // the size before a GlResource is created, thus
        // initializing the shared context
        sf::Context context;

        GLint size;
        glCheck(glGetIntegerv(GL_MAX_TEXTURE_SIZE, &size));

        return static_cast<unsigned int>(size);
    }
}


namespace sf
{
////////////////////////////////////////////////////////////
GlTexture::GlTexture() :
m_texture      (0)
{

}

////////////////////////////////////////////////////////////
GlTexture::~GlTexture()
{
    // Destroy the OpenGL texture
    if (m_texture)
    {
        ensureGlContext();

        GLuint texture = static_cast<GLuint>(m_texture);
        glCheck(glDeleteTextures(1, &texture));
    }
}

////////////////////////////////////////////////////////////
unsigned int GlTexture::getMaximumSize()
{
    // TODO: Remove this lock when it becomes unnecessary in C++11
    Lock lock(mutex);

    static unsigned int size = checkMaximumTextureSize();

    return size;
}

////////////////////////////////////////////////////////////
unsigned int GlTexture::getValidSize(unsigned int size)
{
    ensureGlContext();

    // Make sure that extensions are initialized
    priv::ensureExtensionsInit();

    if (GLEXT_texture_non_power_of_two)
    {
        // If hardware supports NPOT textures, then just return the unmodified size
        return size;
    }
    else
    {
        // If hardware doesn't support NPOT textures, we calculate the nearest power of two
        unsigned int powerOfTwo = 1;
        while (powerOfTwo < size)
            powerOfTwo *= 2;

        return powerOfTwo;
    }
}

} // namespace sf
