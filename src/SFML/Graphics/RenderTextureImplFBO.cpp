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
myFrameBuffer(0),
myDepthBuffer(0)
{

}


////////////////////////////////////////////////////////////
RenderTextureImplFBO::~RenderTextureImplFBO()
{
    EnsureGlContext();

    // Destroy the depth buffer
    if (myDepthBuffer)
    {
        GLuint depthBuffer = static_cast<GLuint>(myDepthBuffer);
        GLCheck(glDeleteFramebuffersEXT(1, &depthBuffer));
    }

    // Destroy the frame buffer
    if (myFrameBuffer)
    {
        GLuint frameBuffer = static_cast<GLuint>(myFrameBuffer);
        GLCheck(glDeleteFramebuffersEXT(1, &frameBuffer));
    }

    // Delete the context
    delete myContext;
}


////////////////////////////////////////////////////////////
bool RenderTextureImplFBO::IsAvailable()
{
    EnsureGlContext();

    // Make sure that GLEW is initialized
    priv::EnsureGlewInit();

    return GLEW_EXT_framebuffer_object != 0;
}


////////////////////////////////////////////////////////////
bool RenderTextureImplFBO::Create(unsigned int width, unsigned int height, unsigned int textureId, bool depthBuffer)
{
    // Create the context
    myContext = new Context;

    // Create the framebuffer object
    GLuint frameBuffer = 0;
    GLCheck(glGenFramebuffersEXT(1, &frameBuffer));
    myFrameBuffer = static_cast<unsigned int>(frameBuffer);
    if (!myFrameBuffer)
    {
        Err() << "Impossible to create render texture (failed to create the frame buffer object)" << std::endl;
        return false;
    }
    GLCheck(glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, myFrameBuffer));

    // Create the depth buffer if requested
    if (depthBuffer)
    {
        GLuint depth = 0;
        GLCheck(glGenRenderbuffersEXT(1, &depth));
        myDepthBuffer = static_cast<unsigned int>(depth);
        if (!myDepthBuffer)
        {
            Err() << "Impossible to create render texture (failed to create the attached depth buffer)" << std::endl;
            return false;
        }
        GLCheck(glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, myDepthBuffer));
        GLCheck(glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, GL_DEPTH_COMPONENT, width, height));
        GLCheck(glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT, myDepthBuffer));
    }

    // Link the texture to the frame buffer
    GLCheck(glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, textureId, 0));

    // A final check, just to be sure...
    if (glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT) != GL_FRAMEBUFFER_COMPLETE_EXT)
    {
        GLCheck(glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0));
        Err() << "Impossible to create render texture (failed to link the target texture to the frame buffer)" << std::endl;
        return false;
    }

    return true;
}


////////////////////////////////////////////////////////////
bool RenderTextureImplFBO::Activate(bool active)
{
    return myContext->SetActive(active);
}

////////////////////////////////////////////////////////////
void RenderTextureImplFBO::UpdateTexture(unsigned int)
{
    glFlush();
}

} // namespace priv

} // namespace sf
