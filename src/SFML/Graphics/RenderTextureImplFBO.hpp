////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007-2021 Laurent Gomila (laurent@sfml-dev.org)
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

#ifndef SFML_RENDERTEXTUREIMPLFBO_HPP
#define SFML_RENDERTEXTUREIMPLFBO_HPP

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Graphics/RenderTextureImpl.hpp>
#include <SFML/Window/GlResource.hpp>
#include <unordered_map>
#include <memory>


namespace sf
{
class Context;


namespace priv
{
////////////////////////////////////////////////////////////
/// \brief Specialization of RenderTextureImpl using the
///        FrameBuffer Object OpenGL extension
///
////////////////////////////////////////////////////////////
class RenderTextureImplFBO : public RenderTextureImpl, GlResource
{
public:

    ////////////////////////////////////////////////////////////
    /// \brief Default constructor
    ///
    ////////////////////////////////////////////////////////////
    RenderTextureImplFBO();

    ////////////////////////////////////////////////////////////
    /// \brief Destructor
    ///
    ////////////////////////////////////////////////////////////
    ~RenderTextureImplFBO() override;

    ////////////////////////////////////////////////////////////
    /// \brief Check whether the system supports FBOs or not
    ///
    /// \return True if FBO render textures are supported
    ///
    ////////////////////////////////////////////////////////////
    static bool isAvailable();

    ////////////////////////////////////////////////////////////
    /// \brief Get the maximum anti-aliasing level supported by the system
    ///
    /// \return The maximum anti-aliasing level supported by the system
    ///
    ////////////////////////////////////////////////////////////
    static unsigned int getMaximumAntialiasingLevel();

    ////////////////////////////////////////////////////////////
    /// \brief Unbind the currently bound FBO
    ///
    ////////////////////////////////////////////////////////////
    static void unbind();

private:

    ////////////////////////////////////////////////////////////
    /// \brief Create the render texture implementation
    ///
    /// \param width      Width of the texture to render to
    /// \param height     Height of the texture to render to
    /// \param textureId  OpenGL identifier of the target texture
    /// \param settings   Context settings to create render-texture with
    ///
    /// \return True if creation has been successful
    ///
    ////////////////////////////////////////////////////////////
    bool create(unsigned int width, unsigned int height, unsigned int textureId, const ContextSettings& settings) override;

    ////////////////////////////////////////////////////////////
    /// \brief Create an FBO in the current context
    ///
    /// \return True if creation has been successful
    ///
    ////////////////////////////////////////////////////////////
    bool createFrameBuffer();

    ////////////////////////////////////////////////////////////
    /// \brief Activate or deactivate the render texture for rendering
    ///
    /// \param active True to activate, false to deactivate
    ///
    /// \return True on success, false on failure
    ///
    ////////////////////////////////////////////////////////////
    bool activate(bool active) override;

    ////////////////////////////////////////////////////////////
    /// \brief Tell if the render-texture will use sRGB encoding when drawing on it
    ///
    /// You can request sRGB encoding for a render-texture
    /// by having the sRgbCapable flag set for the context parameter of create() method
    ///
    /// \return True if the render-texture use sRGB encoding, false otherwise
    ///
    ////////////////////////////////////////////////////////////
    bool isSrgb() const override;

    ////////////////////////////////////////////////////////////
    /// \brief Update the pixels of the target texture
    ///
    /// \param textureId OpenGL identifier of the target texture
    ///
    ////////////////////////////////////////////////////////////
    void updateTexture(unsigned textureId) override;

    ////////////////////////////////////////////////////////////
    // Member data
    ////////////////////////////////////////////////////////////
    std::unordered_map<Uint64, unsigned int> m_frameBuffers;            //!< OpenGL frame buffer objects per context
    std::unordered_map<Uint64, unsigned int> m_multisampleFrameBuffers; //!< Optional per-context OpenGL frame buffer objects with multisample attachments
    unsigned int                             m_depthStencilBuffer;      //!< Optional depth/stencil buffer attached to the frame buffer
    unsigned int                             m_colorBuffer;             //!< Optional multisample color buffer attached to the frame buffer
    unsigned int                             m_width;                   //!< Width of the attachments
    unsigned int                             m_height;                  //!< Height of the attachments
    std::unique_ptr<Context>                 m_context;                 //!< Backup OpenGL context, used when none already exist
    unsigned int                             m_textureId;               //!< The ID of the texture to attach to the FBO
    bool                                     m_multisample;             //!< Whether we have to create a multisample frame buffer as well
    bool                                     m_stencil;                 //!< Whether we have stencil attachment
    bool                                     m_sRgb;                    //!< Whether we need to encode drawn pixels into sRGB color space
};

} // namespace priv

} // namespace sf


#endif // SFML_RENDERTEXTUREIMPLFBO_HPP
