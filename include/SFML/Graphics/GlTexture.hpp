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

#ifndef SFML_GLTEXTURE_HPP
#define SFML_GLTEXTURE_HPP

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Graphics/Export.hpp>
#include <SFML/Window/GlResource.hpp>


namespace sf
{

////////////////////////////////////////////////////////////
/// \brief Sampler living on the graphics card using opengl identifier.
/// 
////////////////////////////////////////////////////////////
class SFML_GRAPHICS_API GlTexture : protected GlResource
{

public:

    ////////////////////////////////////////////////////////////
    /// \brief Default constructor
    ///
    ////////////////////////////////////////////////////////////
    GlTexture();

    ////////////////////////////////////////////////////////////
    /// \brief Destructor
    ///
    /// The destructor will clean the OpenGl Texture
    ////////////////////////////////////////////////////////////
    virtual ~GlTexture();


    ////////////////////////////////////////////////////////////
    /// \brief Get the maximum texture size allowed
    ///
    /// This maximum size is defined by the graphics driver.
    /// You can expect a value of 512 pixels for low-end graphics
    /// card, and up to 8192 pixels or more for newer hardware.
    ///
    /// Note: The first call to this function, whether by your
    /// code or SFML will result in a context switch.
    ///
    /// \return Maximum size allowed for textures
    ///
    ////////////////////////////////////////////////////////////
    static unsigned int getMaximumSize();
    
    ////////////////////////////////////////////////////////////
    /// \brief Get a texture size according to hardware support
    ///
    /// This function checks whether the graphics driver supports
    /// non power of two sizes or not, and adjusts the size
    /// accordingly.
    /// The returned size is greater than or equal to the original size.
    ///
    /// \param size size to convert
    ///
    /// \return Valid nearest size (greater than or equal to specified size)
    ///
    ////////////////////////////////////////////////////////////
    static unsigned int getValidSize(unsigned int size);
    
    
    ////////////////////////////////////////////////////////////
    /// \brief Get the OpenGl Texture Identifier
    ///
    /// This function return the OpenGl Texture Identifier of the Object
    /// if the identifier is 0 the GlTexture is not valid
    ///
    /// \return the OpenGl Texture Identifier
    ///
    ////////////////////////////////////////////////////////////
    inline unsigned int getOpenGlId() {
      return m_texture;
    }
    
    ////////////////////////////////////////////////////////////
    /// \brief Bind a texture for rendering
    ///
    /// Must implement the correct OpenGL bind operation (GL_TEXTURE_1D
    /// GL_TEXTURE_2D etc.)
    ///
    ////////////////////////////////////////////////////////////
    virtual void bind() const =0;    

protected:

    ////////////////////////////////////////////////////////////
    // Member data
    ////////////////////////////////////////////////////////////
    unsigned int m_texture;       ///< Internal texture identifier
};

} // namespace sf


#endif // SFML_GLTEXTURE_HPP

////////////////////////////////////////////////////////////
/// \class sf::GlTexture
/// \ingroup graphics
///
/// sf::GlTexture is an abstract class which must be inherited 
/// to enable the compatibility with sf::Shader
///
/// \see sf::Texture sf::Shader
///
////////////////////////////////////////////////////////////
