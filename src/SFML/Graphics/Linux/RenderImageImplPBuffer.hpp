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

#ifndef SFML_RENDERIMAGEIMPLPBUFFER_HPP
#define SFML_RENDERIMAGEIMPLPBUFFER_HPP

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Graphics/RenderImageImpl.hpp>
#include <SFML/Graphics/GLEW/glew.h>
#include <SFML/Graphics/GLEW/glxew.h>
#include <X11/Xlib.h>


namespace sf
{
namespace priv
{
////////////////////////////////////////////////////////////
/// Specialization of RenderImageImpl using GLX P-Buffers
////////////////////////////////////////////////////////////
class RenderImageImplPBuffer : public RenderImageImpl
{
public :

    ////////////////////////////////////////////////////////////
    /// Default constructor
    ///
    ////////////////////////////////////////////////////////////
    RenderImageImplPBuffer();

    ////////////////////////////////////////////////////////////
    /// Destructor
    ///
    ////////////////////////////////////////////////////////////
    ~RenderImageImplPBuffer();

    ////////////////////////////////////////////////////////////
    /// Check whether the system supports P-Buffer or not
    ///
    /// \return True if P-Buffer render images are supported
    ///
    ////////////////////////////////////////////////////////////
    static bool IsSupported();

private :

    ////////////////////////////////////////////////////////////
    /// /see RenderImageImpl::Create
    ///
    ////////////////////////////////////////////////////////////
    virtual bool Create(unsigned int Width, unsigned int Height, unsigned int TextureId, bool DepthBuffer);

    ////////////////////////////////////////////////////////////
    /// /see RenderImageImpl::Activate
    ///
    ////////////////////////////////////////////////////////////
    virtual bool Activate(bool Active);

    ////////////////////////////////////////////////////////////
    /// /see RenderImageImpl::UpdateTexture
    ///
    ////////////////////////////////////////////////////////////
    virtual bool UpdateTexture(unsigned TextureId);

    ////////////////////////////////////////////////////////////
    // Member data
    ////////////////////////////////////////////////////////////
    ::Display*   myDisplay; ///< Connection to the X display
    GLXPbuffer   myPBuffer; ///< P-Buffer
    GLXContext   myContext; ///< Associated OpenGL context
    unsigned int myWidth;   ///< Width of the P-Buffer
    unsigned int myHeight;  ///< Height of the P-Buffer
};

} // namespace priv

} // namespace sf


#endif // SFML_RENDERIMAGEIMPLPBUFFER_HPP
