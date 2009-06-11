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

#ifndef SFML_RENDERIMAGEIMPL_HPP
#define SFML_RENDERIMAGEIMPL_HPP

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/System/NonCopyable.hpp>


namespace sf
{
namespace priv
{
////////////////////////////////////////////////////////////
/// Abstract base class for render-image implementations
////////////////////////////////////////////////////////////
class RenderImageImpl : NonCopyable
{
public :

    ////////////////////////////////////////////////////////////
    /// Destructor
    ///
    ////////////////////////////////////////////////////////////
    virtual ~RenderImageImpl();

    ////////////////////////////////////////////////////////////
    /// Create the render image
    ///
    /// \param Width :       Width of the image to render to
    /// \param Height :      Height of the image to render to
    /// \param TextureId :   OpenGL texture identifier of the target image
    /// \param DepthBuffer : Do you want a depth buffer attached ?
    ///
    /// \return True if creation has been successful
    ///
    ////////////////////////////////////////////////////////////
    virtual bool Create(unsigned int Width, unsigned int Height, unsigned int TextureId, bool DepthBuffer) = 0;

    ////////////////////////////////////////////////////////////
    /// Activate / deactivate the render image for rendering
    ///
    /// \param Active : True to activate, false to deactivate
    ///
    ////////////////////////////////////////////////////////////
    virtual bool Activate(bool Active) = 0;

    ////////////////////////////////////////////////////////////
    /// Update the pixels of the target texture
    ///
    /// \param TextureId : OpenGL identifier of the target texture
    ///
    /// \return True if the new pixels are flipped vertically
    ///
    ////////////////////////////////////////////////////////////
    virtual bool UpdateTexture(unsigned int TextureId) = 0;
};

} // namespace priv

} // namespace sf


#endif // SFML_RENDERIMAGEIMPL_HPP
