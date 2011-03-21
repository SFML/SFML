////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007-2011 Marco Antognini (antognini.marco@gmail.com), 
//                         Laurent Gomila (laurent.gom@gmail.com), 
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
#include <SFML/Window/GlResource.hpp>
#include <GL/glew.h>
#include <AGL/agl.h>

namespace sf
{
namespace priv
{
////////////////////////////////////////////////////////////
/// \brief Specialization of RenderImageImpl using AGL P-Buffers
///
////////////////////////////////////////////////////////////
class RenderImageImplPBuffer : public RenderImageImpl, GlResource
{
public :

    ////////////////////////////////////////////////////////////
    /// \brief Default constructor
    ///
    ////////////////////////////////////////////////////////////
    RenderImageImplPBuffer();

    ////////////////////////////////////////////////////////////
    /// \brief Destructor
    ///
    ////////////////////////////////////////////////////////////
    ~RenderImageImplPBuffer();

    ////////////////////////////////////////////////////////////
    /// \brief Check whether the system supports P-Buffer or not
    ///
    /// \return True if P-Buffer render images are supported
    ///
    ////////////////////////////////////////////////////////////
    static bool IsSupported();

private :

    ////////////////////////////////////////////////////////////
    /// \brief Create the render image implementation
    ///
    /// \param width       Width of the image to render to
    /// \param height      Height of the image to render to
    /// \param textureId   OpenGL texture identifier of the target image
    /// \param depthBuffer Is a depth buffer requested?
    ///
    /// \return True if creation has been successful
    ///
    ////////////////////////////////////////////////////////////
    virtual bool Create(unsigned int width, unsigned int height, unsigned int textureId, bool depthBuffer);

    ////////////////////////////////////////////////////////////
    /// \brief Activate or deactivate the render image for rendering
    ///
    /// \param active True to activate, false to deactivate
    ///
    /// \return True on success, false on failure
    ///
    ////////////////////////////////////////////////////////////
    virtual bool Activate(bool active);

    ////////////////////////////////////////////////////////////
    /// \brief Update the pixels of the target texture
    ///
    /// \param textureId OpenGL identifier of the target texture
    ///
    ////////////////////////////////////////////////////////////
    virtual void UpdateTexture(unsigned textureId);

    ////////////////////////////////////////////////////////////
    // Member data
    ////////////////////////////////////////////////////////////
    AGLPbuffer   myPBuffer; ///< P-Buffer
    AGLContext   myContext; ///< Associated OpenGL context
    unsigned int myWidth;   ///< Width of the P-Buffer
    unsigned int myHeight;  ///< Height of the P-Buffer
};

} // namespace priv

} // namespace sf


#endif // SFML_RENDERIMAGEIMPLPBUFFER_HPP
