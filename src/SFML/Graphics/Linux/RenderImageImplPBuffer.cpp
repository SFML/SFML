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
#include <SFML/Graphics/Linux/RenderImageImplPBuffer.hpp>
#include <SFML/Graphics/GLCheck.hpp>
#include <SFML/Window/Context.hpp>
#include <SFML/System/Err.hpp>


namespace sf
{
namespace priv
{
////////////////////////////////////////////////////////////
RenderImageImplPBuffer::RenderImageImplPBuffer() :
myPBuffer(0),
myContext(NULL),
myWidth  (0),
myHeight (0)
{
    myDisplay = XOpenDisplay(NULL);
}


////////////////////////////////////////////////////////////
RenderImageImplPBuffer::~RenderImageImplPBuffer()
{
    EnsureGlContext();

    if (myContext)
        glXDestroyContext(myDisplay, myContext);

    if (myPBuffer)
        glXDestroyGLXPbufferSGIX(myDisplay, myPBuffer);

    XCloseDisplay(myDisplay);
}


////////////////////////////////////////////////////////////
bool RenderImageImplPBuffer::IsSupported()
{
    EnsureGlContext();

    // Make sure that GLEW is initialized
    EnsureGlewInit();

    return GLXEW_SGIX_pbuffer && GLXEW_SGIX_fbconfig;
}


////////////////////////////////////////////////////////////
bool RenderImageImplPBuffer::Create(unsigned int width, unsigned int height, unsigned int, bool depthBuffer)
{
    // Store the dimensions
    myWidth  = width;
    myHeight = height;

    // Define the PBuffer attributes
    int visualAttributes[] =
    {
        GLX_RENDER_TYPE,   GLX_RGBA_BIT,
        GLX_DRAWABLE_TYPE, GLX_PBUFFER_BIT,
        GLX_RED_SIZE,      8,
        GLX_GREEN_SIZE,    8,
        GLX_BLUE_SIZE,     8,
        GLX_ALPHA_SIZE,    8,
        GLX_DEPTH_SIZE,    (depthBuffer ? 24 : 0),
        0
    };
    int PBufferAttributes[] =
    {
        GLX_PBUFFER_WIDTH, 	width,
        GLX_PBUFFER_HEIGHT, height,
        0
    };

    // Get the available FB configurations
    int nbConfigs = 0;
    GLXFBConfig* configs = glXChooseFBConfigSGIX(myDisplay, DefaultScreen(myDisplay), visualAttributes, &nbConfigs);
    if (!configs || !nbConfigs)
    {
        Err() << "Impossible to create render image (failed to find a suitable pixel format for PBuffer)" << std::endl;
        return false;
    }

    // Create the P-Buffer
    myPBuffer = glXCreateGLXPbufferSGIX(myDisplay, configs[0], width, height, PBufferAttributes);
    if (!myPBuffer)
    {
        Err() << "Impossible to create render image (failed to create the OpenGL PBuffer)" << std::endl;
        XFree(configs);
        return false;
    }

    // Check the actual size of the P-Buffer
    unsigned int actualWidth, actualHeight;
    glXQueryGLXPbufferSGIX(myDisplay, myPBuffer, GLX_WIDTH_SGIX, &actualWidth);
    glXQueryGLXPbufferSGIX(myDisplay, myPBuffer, GLX_HEIGHT_SGIX, &actualHeight);
    if ((actualWidth != width) || (actualHeight != height))
    {
        Err() << "Impossible to create render image (failed to match the requested size). "
              << "Size: " << actualWidth << "x" << actualHeight << " - "
              << "Requested: " << width << "x" << height
              << std::endl;
        XFree(configs);
        return false;
    }

    // We'll have to share the P-Buffer context with the current context
    GLXDrawable currentDrawable = glXGetCurrentDrawable();
    GLXContext currentContext = glXGetCurrentContext();
    if (currentContext)
        glXMakeCurrent(myDisplay, 0, NULL);

    // Create the context
    XVisualInfo* visual = glXGetVisualFromFBConfig(myDisplay, configs[0]);
    myContext = glXCreateContext(myDisplay, visual, currentContext, true);
    if (!myContext)
    {
        Err() << "Impossible to create render image (failed to create the OpenGL context)" << std::endl;
        XFree(configs);
        XFree(visual);
        return false;
    }
 
    // Restore the previous active context
    if (currentContext)
        glXMakeCurrent(myDisplay, currentDrawable, currentContext);
 
    // Cleanup resources
    XFree(configs);
    XFree(visual);

    return true;
}


////////////////////////////////////////////////////////////
bool RenderImageImplPBuffer::Activate(bool active)
{
    if (active)
    {
        if (myPBuffer && myContext)
        {
            if (glXGetCurrentContext() != myContext)
                return glXMakeCurrent(myDisplay, myPBuffer, myContext) != 0;
            else
                return true;
        }
        else
        {
            return false;
        }
    }
    else
    {
        // To deactivate the P-Buffer's context, we actually activate
        // another one so that we make sure that there is always an
        // active context for subsequent graphics operations
        // @odo fixme
        //return Context::SetReferenceActive();
        return true;
    }
}


////////////////////////////////////////////////////////////
void RenderImageImplPBuffer::UpdateTexture(unsigned int textureId)
{
    GLint previous;
    GLCheck(glGetIntegerv(GL_TEXTURE_BINDING_2D, &previous));

    // Copy the rendered pixels to the image
    GLCheck(glBindTexture(GL_TEXTURE_2D, textureId));
    GLCheck(glCopyTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 0, 0, myWidth, myHeight));

    GLCheck(glBindTexture(GL_TEXTURE_2D, previous));
}

} // namespace priv

} // namespace sf
