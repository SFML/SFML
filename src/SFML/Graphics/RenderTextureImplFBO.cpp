////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007-2018 Laurent Gomila (laurent@sfml-dev.org)
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
#include <SFML/Graphics/RenderTextureImplFBO.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/GLCheck.hpp>
#include <SFML/System/Err.hpp>


namespace sf
{
namespace priv
{
////////////////////////////////////////////////////////////
RenderTextureImplFBO::RenderTextureImplFBO() :
m_context               (NULL),
m_frameBuffer           (0),
m_multisampleFrameBuffer(0),
m_depthStencilBuffer    (0),
m_colorBuffer           (0),
m_width                 (0),
m_height                (0)
{

}


////////////////////////////////////////////////////////////
RenderTextureImplFBO::~RenderTextureImplFBO()
{
    m_context->setActive(true);

    // Destroy the color buffer
    if (m_colorBuffer)
    {
        GLuint colorBuffer = static_cast<GLuint>(m_colorBuffer);
        glCheck(GLEXT_glDeleteRenderbuffers(1, &colorBuffer));
    }

    // Destroy the depth/stencil buffer
    if (m_depthStencilBuffer)
    {
        GLuint depthStencilBuffer = static_cast<GLuint>(m_depthStencilBuffer);
        glCheck(GLEXT_glDeleteRenderbuffers(1, &depthStencilBuffer));
    }

    // Destroy the multisample frame buffer
    if (m_multisampleFrameBuffer)
    {
        GLuint multisampleFrameBuffer = static_cast<GLuint>(m_multisampleFrameBuffer);
        glCheck(GLEXT_glDeleteFramebuffers(1, &multisampleFrameBuffer));
    }

    // Destroy the frame buffer
    if (m_frameBuffer)
    {
        GLuint frameBuffer = static_cast<GLuint>(m_frameBuffer);
        glCheck(GLEXT_glDeleteFramebuffers(1, &frameBuffer));
    }

    // Delete the context
    delete m_context;
}


////////////////////////////////////////////////////////////
bool RenderTextureImplFBO::isAvailable()
{
    TransientContextLock lock;

    // Make sure that extensions are initialized
    priv::ensureExtensionsInit();

    return GLEXT_framebuffer_object != 0;
}


////////////////////////////////////////////////////////////
unsigned int RenderTextureImplFBO::getMaximumAntialiasingLevel()
{
    GLint samples = 0;

#ifndef SFML_OPENGL_ES

    glCheck(glGetIntegerv(GLEXT_GL_MAX_SAMPLES, &samples));

#endif

    return static_cast<unsigned int>(samples);
}


////////////////////////////////////////////////////////////
bool RenderTextureImplFBO::create(unsigned int width, unsigned int height, unsigned int textureId, const ContextSettings& settings)
{
    // Store the dimensions
    m_width = width;
    m_height = height;

    // Disable creation of depth/stencil surfaces in the context
    ContextSettings contextSettings(settings);
    contextSettings.depthBits = 0;
    contextSettings.stencilBits = 0;
    contextSettings.antialiasingLevel = 0;
    contextSettings.sRgbCapable = false;

    // Create the context
    m_context = new Context(contextSettings, 1, 1);

    if (settings.antialiasingLevel && !(GLEXT_framebuffer_multisample && GLEXT_framebuffer_blit))
        return false;

    if (settings.stencilBits && !GLEXT_packed_depth_stencil)
        return false;

#ifndef SFML_OPENGL_ES

    // Check if the requested anti-aliasing level is supported
    if (settings.antialiasingLevel)
    {
        GLint samples = 0;
        glCheck(glGetIntegerv(GLEXT_GL_MAX_SAMPLES, &samples));

        if (settings.antialiasingLevel > static_cast<unsigned int>(samples))
        {
            err() << "Impossible to create render texture (unsupported anti-aliasing level)";
            err() << " Requested: " << settings.antialiasingLevel << " Maximum supported: " << samples << std::endl;
            return false;
        }
    }

#endif

    if (!settings.antialiasingLevel)
    {
        // Create the framebuffer object
        GLuint frameBuffer = 0;
        glCheck(GLEXT_glGenFramebuffers(1, &frameBuffer));
        m_frameBuffer = static_cast<unsigned int>(frameBuffer);
        if (!m_frameBuffer)
        {
            err() << "Impossible to create render texture (failed to create the frame buffer object)" << std::endl;
            return false;
        }
        glCheck(GLEXT_glBindFramebuffer(GLEXT_GL_FRAMEBUFFER, m_frameBuffer));

        // Create the depth/stencil buffer if requested
        if (settings.stencilBits)
        {

#ifndef SFML_OPENGL_ES

            GLuint depthStencil = 0;
            glCheck(GLEXT_glGenRenderbuffers(1, &depthStencil));
            m_depthStencilBuffer = static_cast<unsigned int>(depthStencil);
            if (!m_depthStencilBuffer)
            {
                err() << "Impossible to create render texture (failed to create the attached depth/stencil buffer)" << std::endl;
                return false;
            }
            glCheck(GLEXT_glBindRenderbuffer(GLEXT_GL_RENDERBUFFER, m_depthStencilBuffer));
            glCheck(GLEXT_glRenderbufferStorage(GLEXT_GL_RENDERBUFFER, GLEXT_GL_DEPTH24_STENCIL8, width, height));
            glCheck(GLEXT_glFramebufferRenderbuffer(GLEXT_GL_FRAMEBUFFER, GLEXT_GL_DEPTH_ATTACHMENT, GLEXT_GL_RENDERBUFFER, m_depthStencilBuffer));
            glCheck(GLEXT_glFramebufferRenderbuffer(GLEXT_GL_FRAMEBUFFER, GLEXT_GL_STENCIL_ATTACHMENT, GLEXT_GL_RENDERBUFFER, m_depthStencilBuffer));
#else

            err() << "Impossible to create render texture (failed to create the attached depth/stencil buffer)" << std::endl;
            return false;

#endif // SFML_OPENGL_ES

        }
        else if (settings.depthBits)
        {
            GLuint depthStencil = 0;
            glCheck(GLEXT_glGenRenderbuffers(1, &depthStencil));
            m_depthStencilBuffer = static_cast<unsigned int>(depthStencil);
            if (!m_depthStencilBuffer)
            {
                err() << "Impossible to create render texture (failed to create the attached depth buffer)" << std::endl;
                return false;
            }
            glCheck(GLEXT_glBindRenderbuffer(GLEXT_GL_RENDERBUFFER, m_depthStencilBuffer));
            glCheck(GLEXT_glRenderbufferStorage(GLEXT_GL_RENDERBUFFER, GLEXT_GL_DEPTH_COMPONENT, width, height));
            glCheck(GLEXT_glFramebufferRenderbuffer(GLEXT_GL_FRAMEBUFFER, GLEXT_GL_DEPTH_ATTACHMENT, GLEXT_GL_RENDERBUFFER, m_depthStencilBuffer));
        }

        // Link the texture to the frame buffer
        glCheck(GLEXT_glFramebufferTexture2D(GLEXT_GL_FRAMEBUFFER, GLEXT_GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureId, 0));

        // A final check, just to be sure...
        GLenum status;
        glCheck(status = GLEXT_glCheckFramebufferStatus(GLEXT_GL_FRAMEBUFFER));
        if (status != GLEXT_GL_FRAMEBUFFER_COMPLETE)
        {
            glCheck(GLEXT_glBindFramebuffer(GLEXT_GL_FRAMEBUFFER, 0));
            err() << "Impossible to create render texture (frame buffer incomplete)" << std::endl;
            return false;
        }

        return true;
    }
    else
    {

#ifndef SFML_OPENGL_ES

        // Create the framebuffer object
        GLuint frameBuffer = 0;
        glCheck(GLEXT_glGenFramebuffers(1, &frameBuffer));
        m_frameBuffer = static_cast<unsigned int>(frameBuffer);
        if (!m_frameBuffer)
        {
            err() << "Impossible to create render texture (failed to create the frame buffer object)" << std::endl;
            return false;
        }
        glCheck(GLEXT_glBindFramebuffer(GLEXT_GL_FRAMEBUFFER, m_frameBuffer));

        // Link the texture to the frame buffer
        glCheck(GLEXT_glFramebufferTexture2D(GLEXT_GL_FRAMEBUFFER, GLEXT_GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureId, 0));

        // A final check, just to be sure...
        GLenum status;
        glCheck(status = GLEXT_glCheckFramebufferStatus(GLEXT_GL_FRAMEBUFFER));
        if (status != GLEXT_GL_FRAMEBUFFER_COMPLETE)
        {
            glCheck(GLEXT_glBindFramebuffer(GLEXT_GL_FRAMEBUFFER, 0));
            err() << "Impossible to create render texture (frame buffer incomplete)" << std::endl;
            return false;
        }

        // Create the multisample framebuffer object
        frameBuffer = 0;
        glCheck(GLEXT_glGenFramebuffers(1, &frameBuffer));
        m_multisampleFrameBuffer = static_cast<unsigned int>(frameBuffer);
        if (!m_multisampleFrameBuffer)
        {
            err() << "Impossible to create render texture (failed to create the multisample frame buffer object)" << std::endl;
            return false;
        }
        glCheck(GLEXT_glBindFramebuffer(GLEXT_GL_FRAMEBUFFER, m_multisampleFrameBuffer));

        // Create the multisample color buffer
        GLuint color = 0;
        glCheck(GLEXT_glGenRenderbuffers(1, &color));
        m_colorBuffer = static_cast<unsigned int>(color);
        if (!m_colorBuffer)
        {
            err() << "Impossible to create render texture (failed to create the attached multisample color buffer)" << std::endl;
            return false;
        }
        glCheck(GLEXT_glBindRenderbuffer(GLEXT_GL_RENDERBUFFER, m_colorBuffer));
        glCheck(GLEXT_glRenderbufferStorageMultisample(GLEXT_GL_RENDERBUFFER, settings.antialiasingLevel, GL_RGBA, width, height));
        glCheck(GLEXT_glFramebufferRenderbuffer(GLEXT_GL_FRAMEBUFFER, GLEXT_GL_COLOR_ATTACHMENT0, GLEXT_GL_RENDERBUFFER, m_colorBuffer));

        // Create the multisample depth/stencil buffer if requested
        if (settings.stencilBits)
        {
            GLuint depthStencil = 0;
            glCheck(GLEXT_glGenRenderbuffers(1, &depthStencil));
            m_depthStencilBuffer = static_cast<unsigned int>(depthStencil);
            if (!m_depthStencilBuffer)
            {
                err() << "Impossible to create render texture (failed to create the attached multisample depth/stencil buffer)" << std::endl;
                return false;
            }
            glCheck(GLEXT_glBindRenderbuffer(GLEXT_GL_RENDERBUFFER, m_depthStencilBuffer));
            glCheck(GLEXT_glRenderbufferStorageMultisample(GLEXT_GL_RENDERBUFFER, settings.antialiasingLevel, GLEXT_GL_DEPTH24_STENCIL8, width, height));
            glCheck(GLEXT_glFramebufferRenderbuffer(GLEXT_GL_FRAMEBUFFER, GLEXT_GL_DEPTH_ATTACHMENT, GLEXT_GL_RENDERBUFFER, m_depthStencilBuffer));
            glCheck(GLEXT_glFramebufferRenderbuffer(GLEXT_GL_FRAMEBUFFER, GLEXT_GL_STENCIL_ATTACHMENT, GLEXT_GL_RENDERBUFFER, m_depthStencilBuffer));
        }
        else if (settings.depthBits)
        {
            GLuint depthStencil = 0;
            glCheck(GLEXT_glGenRenderbuffers(1, &depthStencil));
            m_depthStencilBuffer = static_cast<unsigned int>(depthStencil);
            if (!m_depthStencilBuffer)
            {
                err() << "Impossible to create render texture (failed to create the attached multisample depth buffer)" << std::endl;
                return false;
            }
            glCheck(GLEXT_glBindRenderbuffer(GLEXT_GL_RENDERBUFFER, m_depthStencilBuffer));
            glCheck(GLEXT_glRenderbufferStorageMultisample(GLEXT_GL_RENDERBUFFER, settings.antialiasingLevel, GLEXT_GL_DEPTH_COMPONENT, width, height));
            glCheck(GLEXT_glFramebufferRenderbuffer(GLEXT_GL_FRAMEBUFFER, GLEXT_GL_DEPTH_ATTACHMENT, GLEXT_GL_RENDERBUFFER, m_depthStencilBuffer));
        }

        // A final check, just to be sure...
        glCheck(status = GLEXT_glCheckFramebufferStatus(GLEXT_GL_FRAMEBUFFER));
        if (status != GLEXT_GL_FRAMEBUFFER_COMPLETE)
        {
            glCheck(GLEXT_glBindFramebuffer(GLEXT_GL_FRAMEBUFFER, 0));
            err() << "Impossible to create render texture (multisample frame buffer incomplete)" << std::endl;
            return false;
        }

        return true;

#else

        err() << "Impossible to create render texture (failed to create the multisample frame buffer object)" << std::endl;
        return false;

#endif // SFML_OPENGL_ES

    }
}


////////////////////////////////////////////////////////////
bool RenderTextureImplFBO::activate(bool active)
{
    return m_context->setActive(active);
}


////////////////////////////////////////////////////////////
void RenderTextureImplFBO::updateTexture(unsigned int)
{

#ifndef SFML_OPENGL_ES

    if (m_multisampleFrameBuffer)
    {
        glCheck(GLEXT_glBindFramebuffer(GLEXT_GL_DRAW_FRAMEBUFFER, m_frameBuffer));
        glCheck(GLEXT_glBlitFramebuffer(0, 0, m_width, m_height, 0, 0, m_width, m_height, GL_COLOR_BUFFER_BIT, GL_NEAREST));
        glCheck(GLEXT_glBindFramebuffer(GLEXT_GL_DRAW_FRAMEBUFFER, m_multisampleFrameBuffer));
    }

#endif // SFML_OPENGL_ES

    glCheck(glFlush());
}

} // namespace priv

} // namespace sf
