////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007-2023 Laurent Gomila (laurent@sfml-dev.org)
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
#include <SFML/System/Mutex.hpp>
#include <SFML/System/Lock.hpp>
#include <SFML/System/Err.hpp>
#include <utility>
#include <set>


namespace
{
    // Set to track all active FBO mappings
    // This is used to free active FBOs while their owning
    // RenderTextureImplFBO is still alive
    std::set<std::map<sf::Uint64, unsigned int>*> frameBuffers;

    // Set to track all stale FBOs
    // This is used to free stale FBOs after their owning
    // RenderTextureImplFBO has already been destroyed
    // An FBO cannot be destroyed until it's containing context
    // becomes active, so the destruction of the RenderTextureImplFBO
    // has to be decoupled from the destruction of the FBOs themselves
    std::set<std::pair<sf::Uint64, unsigned int> > staleFrameBuffers;

    // Mutex to protect both active and stale frame buffer sets
    sf::Mutex mutex;

    // This function is called either when a RenderTextureImplFBO is
    // destroyed or via contextDestroyCallback when context destruction
    // might trigger deletion of its contained stale FBOs
    void destroyStaleFBOs()
    {
        sf::Uint64 contextId = sf::Context::getActiveContextId();

        for (std::set<std::pair<sf::Uint64, unsigned int> >::iterator iter = staleFrameBuffers.begin(); iter != staleFrameBuffers.end();)
        {
            if (iter->first == contextId)
            {
                GLuint frameBuffer = static_cast<GLuint>(iter->second);
                glCheck(GLEXT_glDeleteFramebuffers(1, &frameBuffer));

                staleFrameBuffers.erase(iter++);
            }
            else
            {
                ++iter;
            }
        }
    }

    // Callback that is called every time a context is destroyed
    void contextDestroyCallback(void* /*arg*/)
    {
        sf::Lock lock(mutex);

        sf::Uint64 contextId = sf::Context::getActiveContextId();

        // Destroy active frame buffer objects
        for (std::set<std::map<sf::Uint64, unsigned int>*>::iterator frameBuffersIter = frameBuffers.begin(); frameBuffersIter != frameBuffers.end(); ++frameBuffersIter)
        {
            for (std::map<sf::Uint64, unsigned int>::iterator iter = (*frameBuffersIter)->begin(); iter != (*frameBuffersIter)->end(); ++iter)
            {
                if (iter->first == contextId)
                {
                    GLuint frameBuffer = iter->second;
                    glCheck(GLEXT_glDeleteFramebuffers(1, &frameBuffer));

                    // Erase the entry from the RenderTextureImplFBO's map
                    (*frameBuffersIter)->erase(iter);

                    break;
                }
            }
        }

        // Destroy stale frame buffer objects
        destroyStaleFBOs();
    }
}


namespace sf
{
namespace priv
{
////////////////////////////////////////////////////////////
RenderTextureImplFBO::RenderTextureImplFBO() :
m_depthStencilBuffer(0),
m_colorBuffer       (0),
m_width             (0),
m_height            (0),
m_context           (NULL),
m_textureId         (0),
m_multisample       (false),
m_stencil           (false),
m_sRgb              (false)
{
    Lock lock(mutex);

    // Register the context destruction callback
    registerContextDestroyCallback(contextDestroyCallback, 0);

    // Insert the new frame buffer mapping into the set of all active mappings
    frameBuffers.insert(&m_frameBuffers);
    frameBuffers.insert(&m_multisampleFrameBuffers);
}


////////////////////////////////////////////////////////////
RenderTextureImplFBO::~RenderTextureImplFBO()
{
    TransientContextLock contextLock;

    Lock lock(mutex);

    // Remove the frame buffer mapping from the set of all active mappings
    frameBuffers.erase(&m_frameBuffers);
    frameBuffers.erase(&m_multisampleFrameBuffers);

    // Destroy the color buffer
    if (m_colorBuffer)
    {
        GLuint colorBuffer = m_colorBuffer;
        glCheck(GLEXT_glDeleteRenderbuffers(1, &colorBuffer));
    }

    // Destroy the depth/stencil buffer
    if (m_depthStencilBuffer)
    {
        GLuint depthStencilBuffer = m_depthStencilBuffer;
        glCheck(GLEXT_glDeleteRenderbuffers(1, &depthStencilBuffer));
    }

    // Move all frame buffer objects to stale set
    for (std::map<Uint64, unsigned int>::iterator iter = m_frameBuffers.begin(); iter != m_frameBuffers.end(); ++iter)
        staleFrameBuffers.insert(std::make_pair(iter->first, iter->second));

    for (std::map<Uint64, unsigned int>::iterator iter = m_multisampleFrameBuffers.begin(); iter != m_multisampleFrameBuffers.end(); ++iter)
        staleFrameBuffers.insert(std::make_pair(iter->first, iter->second));

    // Clean up FBOs
    destroyStaleFBOs();

    // Delete the backup context if we had to create one
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
    TransientContextLock lock;

    GLint samples = 0;

#ifndef SFML_OPENGL_ES

    glCheck(glGetIntegerv(GLEXT_GL_MAX_SAMPLES, &samples));

#endif

    return static_cast<unsigned int>(samples);
}


////////////////////////////////////////////////////////////
void RenderTextureImplFBO::unbind()
{
    glCheck(GLEXT_glBindFramebuffer(GLEXT_GL_FRAMEBUFFER, 0));
}


////////////////////////////////////////////////////////////
bool RenderTextureImplFBO::create(unsigned int width, unsigned int height, unsigned int textureId, const ContextSettings& settings)
{
    // Store the dimensions
    m_width = width;
    m_height = height;

    {
        TransientContextLock lock;

        // Make sure that extensions are initialized
        priv::ensureExtensionsInit();

        if (settings.antialiasingLevel && !(GLEXT_framebuffer_multisample && GLEXT_framebuffer_blit))
            return false;

        if (settings.stencilBits && !GLEXT_packed_depth_stencil)
            return false;

        m_sRgb = settings.sRgbCapable && GL_EXT_texture_sRGB;

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
            // Create the depth/stencil buffer if requested
            if (settings.stencilBits)
            {

#ifndef SFML_OPENGL_ES

                GLuint depthStencil = 0;
                glCheck(GLEXT_glGenRenderbuffers(1, &depthStencil));
                m_depthStencilBuffer = depthStencil;
                if (!m_depthStencilBuffer)
                {
                    err() << "Impossible to create render texture (failed to create the attached depth/stencil buffer)" << std::endl;
                    return false;
                }
                glCheck(GLEXT_glBindRenderbuffer(GLEXT_GL_RENDERBUFFER, m_depthStencilBuffer));
                glCheck(GLEXT_glRenderbufferStorage(GLEXT_GL_RENDERBUFFER, GLEXT_GL_DEPTH24_STENCIL8, static_cast<GLsizei>(width), static_cast<GLsizei>(height)));

#else

                err() << "Impossible to create render texture (failed to create the attached depth/stencil buffer)" << std::endl;
                return false;

#endif // SFML_OPENGL_ES

                m_stencil = true;

            }
            else if (settings.depthBits)
            {
                GLuint depthStencil = 0;
                glCheck(GLEXT_glGenRenderbuffers(1, &depthStencil));
                m_depthStencilBuffer = depthStencil;
                if (!m_depthStencilBuffer)
                {
                    err() << "Impossible to create render texture (failed to create the attached depth buffer)" << std::endl;
                    return false;
                }
                glCheck(GLEXT_glBindRenderbuffer(GLEXT_GL_RENDERBUFFER, m_depthStencilBuffer));
                glCheck(GLEXT_glRenderbufferStorage(GLEXT_GL_RENDERBUFFER, GLEXT_GL_DEPTH_COMPONENT, static_cast<GLsizei>(width), static_cast<GLsizei>(height)));
            }
        }
        else
        {

#ifndef SFML_OPENGL_ES

            // Create the multisample color buffer
            GLuint color = 0;
            glCheck(GLEXT_glGenRenderbuffers(1, &color));
            m_colorBuffer = color;
            if (!m_colorBuffer)
            {
                err() << "Impossible to create render texture (failed to create the attached multisample color buffer)" << std::endl;
                return false;
            }
            glCheck(GLEXT_glBindRenderbuffer(GLEXT_GL_RENDERBUFFER, m_colorBuffer));
            glCheck(GLEXT_glRenderbufferStorageMultisample(GLEXT_GL_RENDERBUFFER, static_cast<GLsizei>(settings.antialiasingLevel), m_sRgb ? GL_SRGB8_ALPHA8_EXT : GL_RGBA, static_cast<GLsizei>(width), static_cast<GLsizei>(height)));

            // Create the multisample depth/stencil buffer if requested
            if (settings.stencilBits)
            {
                GLuint depthStencil = 0;
                glCheck(GLEXT_glGenRenderbuffers(1, &depthStencil));
                m_depthStencilBuffer = depthStencil;
                if (!m_depthStencilBuffer)
                {
                    err() << "Impossible to create render texture (failed to create the attached multisample depth/stencil buffer)" << std::endl;
                    return false;
                }
                glCheck(GLEXT_glBindRenderbuffer(GLEXT_GL_RENDERBUFFER, m_depthStencilBuffer));
                glCheck(GLEXT_glRenderbufferStorageMultisample(GLEXT_GL_RENDERBUFFER, static_cast<GLsizei>(settings.antialiasingLevel), GLEXT_GL_DEPTH24_STENCIL8, static_cast<GLsizei>(width), static_cast<GLsizei>(height)));

                m_stencil = true;
            }
            else if (settings.depthBits)
            {
                GLuint depthStencil = 0;
                glCheck(GLEXT_glGenRenderbuffers(1, &depthStencil));
                m_depthStencilBuffer = depthStencil;
                if (!m_depthStencilBuffer)
                {
                    err() << "Impossible to create render texture (failed to create the attached multisample depth buffer)" << std::endl;
                    return false;
                }
                glCheck(GLEXT_glBindRenderbuffer(GLEXT_GL_RENDERBUFFER, m_depthStencilBuffer));
                glCheck(GLEXT_glRenderbufferStorageMultisample(GLEXT_GL_RENDERBUFFER, static_cast<GLsizei>(settings.antialiasingLevel), GLEXT_GL_DEPTH_COMPONENT, static_cast<GLsizei>(width), static_cast<GLsizei>(height)));
            }

#else

            err() << "Impossible to create render texture (failed to create the multisample render buffers)" << std::endl;
            return false;

#endif // SFML_OPENGL_ES

            m_multisample = true;

        }
    }

    // Save our texture ID in order to be able to attach it to an FBO at any time
    m_textureId = textureId;

    // We can't create an FBO now if there is no active context
    if (!Context::getActiveContextId())
        return true;

#ifndef SFML_OPENGL_ES

    // Save the current bindings so we can restore them after we are done
    GLint readFramebuffer = 0;
    GLint drawFramebuffer = 0;

    glCheck(glGetIntegerv(GLEXT_GL_READ_FRAMEBUFFER_BINDING, &readFramebuffer));
    glCheck(glGetIntegerv(GLEXT_GL_DRAW_FRAMEBUFFER_BINDING, &drawFramebuffer));

    if (createFrameBuffer())
    {
        // Restore previously bound framebuffers
        glCheck(GLEXT_glBindFramebuffer(GLEXT_GL_READ_FRAMEBUFFER, static_cast<GLuint>(readFramebuffer)));
        glCheck(GLEXT_glBindFramebuffer(GLEXT_GL_DRAW_FRAMEBUFFER, static_cast<GLuint>(drawFramebuffer)));

        return true;
    }

#else

    // Save the current binding so we can restore them after we are done
    GLint frameBuffer = 0;

    glCheck(glGetIntegerv(GLEXT_GL_FRAMEBUFFER_BINDING, &frameBuffer));

    if (createFrameBuffer())
    {
        // Restore previously bound framebuffer
        glCheck(GLEXT_glBindFramebuffer(GLEXT_GL_FRAMEBUFFER, frameBuffer));

        return true;
    }

#endif

    return false;
}


////////////////////////////////////////////////////////////
bool RenderTextureImplFBO::createFrameBuffer()
{
    // Create the framebuffer object
    GLuint frameBuffer = 0;
    glCheck(GLEXT_glGenFramebuffers(1, &frameBuffer));

    if (!frameBuffer)
    {
        err() << "Impossible to create render texture (failed to create the frame buffer object)" << std::endl;
        return false;
    }
    glCheck(GLEXT_glBindFramebuffer(GLEXT_GL_FRAMEBUFFER, frameBuffer));

    // Link the depth/stencil renderbuffer to the frame buffer
    if (!m_multisample && m_depthStencilBuffer)
    {
        glCheck(GLEXT_glFramebufferRenderbuffer(GLEXT_GL_FRAMEBUFFER, GLEXT_GL_DEPTH_ATTACHMENT, GLEXT_GL_RENDERBUFFER, m_depthStencilBuffer));

#ifndef SFML_OPENGL_ES

        if (m_stencil)
        {
            glCheck(GLEXT_glFramebufferRenderbuffer(GLEXT_GL_FRAMEBUFFER, GLEXT_GL_STENCIL_ATTACHMENT, GLEXT_GL_RENDERBUFFER, m_depthStencilBuffer));
        }

#endif

    }

    // Link the texture to the frame buffer
    glCheck(GLEXT_glFramebufferTexture2D(GLEXT_GL_FRAMEBUFFER, GLEXT_GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_textureId, 0));

    // A final check, just to be sure...
    GLenum status;
    glCheck(status = GLEXT_glCheckFramebufferStatus(GLEXT_GL_FRAMEBUFFER));
    if (status != GLEXT_GL_FRAMEBUFFER_COMPLETE)
    {
        glCheck(GLEXT_glBindFramebuffer(GLEXT_GL_FRAMEBUFFER, 0));
        glCheck(GLEXT_glDeleteFramebuffers(1, &frameBuffer));
        err() << "Impossible to create render texture (failed to link the target texture to the frame buffer)" << std::endl;
        return false;
    }

    {
        Lock lock(mutex);

        // Insert the FBO into our map
        m_frameBuffers.insert(std::make_pair(Context::getActiveContextId(), frameBuffer));
    }

#ifndef SFML_OPENGL_ES

    if (m_multisample)
    {
        // Create the multisample framebuffer object
        GLuint multisampleFrameBuffer = 0;
        glCheck(GLEXT_glGenFramebuffers(1, &multisampleFrameBuffer));

        if (!multisampleFrameBuffer)
        {
            err() << "Impossible to create render texture (failed to create the multisample frame buffer object)" << std::endl;
            return false;
        }
        glCheck(GLEXT_glBindFramebuffer(GLEXT_GL_FRAMEBUFFER, multisampleFrameBuffer));

        // Link the multisample color buffer to the frame buffer
        glCheck(GLEXT_glBindRenderbuffer(GLEXT_GL_RENDERBUFFER, m_colorBuffer));
        glCheck(GLEXT_glFramebufferRenderbuffer(GLEXT_GL_FRAMEBUFFER, GLEXT_GL_COLOR_ATTACHMENT0, GLEXT_GL_RENDERBUFFER, m_colorBuffer));

        // Link the depth/stencil renderbuffer to the frame buffer
        if (m_depthStencilBuffer)
        {
            glCheck(GLEXT_glFramebufferRenderbuffer(GLEXT_GL_FRAMEBUFFER, GLEXT_GL_DEPTH_ATTACHMENT, GLEXT_GL_RENDERBUFFER, m_depthStencilBuffer));

            if (m_stencil)
            {
                glCheck(GLEXT_glFramebufferRenderbuffer(GLEXT_GL_FRAMEBUFFER, GLEXT_GL_STENCIL_ATTACHMENT, GLEXT_GL_RENDERBUFFER, m_depthStencilBuffer));
            }
        }

        // A final check, just to be sure...
        glCheck(status = GLEXT_glCheckFramebufferStatus(GLEXT_GL_FRAMEBUFFER));
        if (status != GLEXT_GL_FRAMEBUFFER_COMPLETE)
        {
            glCheck(GLEXT_glBindFramebuffer(GLEXT_GL_FRAMEBUFFER, 0));
            glCheck(GLEXT_glDeleteFramebuffers(1, &multisampleFrameBuffer));
            err() << "Impossible to create render texture (failed to link the render buffers to the multisample frame buffer)" << std::endl;
            return false;
        }

        {
            Lock lock(mutex);

            // Insert the FBO into our map
            m_multisampleFrameBuffers.insert(std::make_pair(Context::getActiveContextId(), multisampleFrameBuffer));
        }
    }

#endif

    return true;
}


////////////////////////////////////////////////////////////
bool RenderTextureImplFBO::activate(bool active)
{
    // Unbind the FBO if requested
    if (!active)
    {
        glCheck(GLEXT_glBindFramebuffer(GLEXT_GL_FRAMEBUFFER, 0));
        return true;
    }

    Uint64 contextId = Context::getActiveContextId();

    // In the odd case we have to activate and there is no active
    // context yet, we have to create one
    if (!contextId)
    {
        if (!m_context)
            m_context = new Context;

        m_context->setActive(true);

        contextId = Context::getActiveContextId();

        if (!contextId)
        {
            err() << "Impossible to activate render texture (failed to create backup context)" << std::endl;

            return false;
        }
    }

    // Lookup the FBO corresponding to the currently active context
    // If none is found, there is no FBO corresponding to the
    // currently active context so we will have to create a new FBO
    {
        Lock lock(mutex);

        std::map<Uint64, unsigned int>::iterator iter;
        
        if (m_multisample)
        {
            iter = m_multisampleFrameBuffers.find(contextId);

            if (iter != m_multisampleFrameBuffers.end())
            {
                glCheck(GLEXT_glBindFramebuffer(GLEXT_GL_FRAMEBUFFER, iter->second));

                return true;
            }
        }
        else
        {
            iter = m_frameBuffers.find(contextId);

            if (iter != m_frameBuffers.end())
            {
                glCheck(GLEXT_glBindFramebuffer(GLEXT_GL_FRAMEBUFFER, iter->second));

                return true;
            }
        }
    }

    return createFrameBuffer();
}


////////////////////////////////////////////////////////////
bool RenderTextureImplFBO::isSrgb() const
{
    return m_sRgb;
}


////////////////////////////////////////////////////////////
void RenderTextureImplFBO::updateTexture(unsigned int)
{
    // If multisampling is enabled, we need to resolve by blitting
    // from our FBO with multisample renderbuffer attachments
    // to our FBO to which our target texture is attached

#ifndef SFML_OPENGL_ES

    // In case of multisampling, make sure both FBOs
    // are already available within the current context
    if (m_multisample && m_width && m_height && activate(true))
    {
        Uint64 contextId = Context::getActiveContextId();

        Lock lock(mutex);

        std::map<Uint64, unsigned int>::iterator iter = m_frameBuffers.find(contextId);
        std::map<Uint64, unsigned int>::iterator multisampleIter = m_multisampleFrameBuffers.find(contextId);

        if ((iter != m_frameBuffers.end()) && (multisampleIter != m_multisampleFrameBuffers.end()))
        {
            // Set up the blit target (draw framebuffer) and blit (from the read framebuffer, our multisample FBO)
            glCheck(GLEXT_glBindFramebuffer(GLEXT_GL_DRAW_FRAMEBUFFER, iter->second));
            glCheck(GLEXT_glBlitFramebuffer(0, 0, static_cast<GLint>(m_width), static_cast<GLint>(m_height), 0, 0, static_cast<GLint>(m_width), static_cast<GLint>(m_height), GL_COLOR_BUFFER_BIT, GL_NEAREST));
            glCheck(GLEXT_glBindFramebuffer(GLEXT_GL_DRAW_FRAMEBUFFER, multisampleIter->second));
        }
    }

#endif // SFML_OPENGL_ES

}

} // namespace priv

} // namespace sf
