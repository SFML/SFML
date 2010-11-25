

#ifndef SFML_RENDERIMAGEIMPLPBUFFER_HPP
#define SFML_RENDERIMAGEIMPLPBUFFER_HPP

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Graphics/RenderImageImpl.hpp>


namespace sf
{
namespace priv
{
////////////////////////////////////////////////////////////
/// \brief Specialization of RenderImageImpl using ****** P-Buffers
///
////////////////////////////////////////////////////////////
class RenderImageImplPBuffer : public RenderImageImpl
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
    unsigned int myWidth;   ///< Width of the P-Buffer
    unsigned int myHeight;  ///< Height of the P-Buffer
};

} // namespace priv

} // namespace sf


#endif // SFML_RENDERIMAGEIMPLPBUFFER_HPP
