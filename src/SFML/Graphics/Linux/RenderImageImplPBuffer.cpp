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
#include <iostream>


namespace sf
{
namespace priv
{
////////////////////////////////////////////////////////////
/// Default constructor
////////////////////////////////////////////////////////////
RenderImageImplPBuffer::RenderImageImplPBuffer() :
myPBuffer(NULL),
myContext(NULL),
myWidth  (0),
myHeight (0)
{
    myDisplay = XOpenDisplay(NULL);
}


////////////////////////////////////////////////////////////
/// Destructor
////////////////////////////////////////////////////////////
RenderImageImplPBuffer::~RenderImageImplPBuffer()
{
    if (myContext)
        glXDestroyContext(myDisplay, myContext);

    if (myPBuffer)
        glXDestroyGLXPbufferSGIX(myDisplay, myPBuffer);

    XCloseDisplay(myDisplay);

    // This is to make sure that another valid context is made
    // active after we destroy the P-Buffer's one
    Context context;
}


////////////////////////////////////////////////////////////
/// Check whether the system supports P-Buffer or not
////////////////////////////////////////////////////////////
bool RenderImageImplPBuffer::IsSupported()
{
    // Make sure that GLEW is initialized
    EnsureGlewInit();

    return false; //glxewIsSupported("GLX_SGIX_pbuffer");
}


////////////////////////////////////////////////////////////
/// /see RenderImageImpl::Create
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
        std::cerr << "Impossible to create render image (failed to find a suitable pixel format for PBuffer)" << std::endl;
        return false;
    }

    // Create the P-Buffer
    myPBuffer = glXCreateGLXPbufferSGIX(myDisplay, configs[0], width, height, PBufferAttributes);
    if (!myPBuffer)
    {
        std::cerr << "Impossible to create render image (failed to create the OpenGL PBuffer)" << std::endl;
        XFree(configs);
        return false;
    }

    // Check the actual size of the P-Buffer
    unsigned int actualWidth, actualHeight;
    glXQueryGLXPbufferSGIX(myDisplay, myPBuffer, GLX_WIDTH_SGIX, &actualWidth);
    glXQueryGLXPbufferSGIX(myDisplay, myPBuffer, GLX_HEIGHT_SGIX, &actualHeight);
    if ((actualWidth != width) || (actualHeight != height))
    {
        std::cerr << "Impossible to create render image (failed to match the requested size). "
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
        glXMakeCurrent(myDisplay, NULL, NULL);

    // Create the context
    XVisualInfo* Visual = glXGetVisualFromFBConfig(myDisplay, configs[0]);
    myContext = glXCreateContext(myDisplay, visual, currentContext, true);
    if (!myContext)
    {
        std::cerr << "Impossible to create render image (failed to create the OpenGL context)" << std::endl;
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
/// /see RenderImageImpl::Activate
////////////////////////////////////////////////////////////
bool RenderImageImplPBuffer::Activate(bool active)
{
    if (Active)
    {
        if (myPBuffer && myContext && (glXGetCurrentContext() != myContext))
        {
            // Bind the OpenGL context of the P-Buffer
            if (!glXMakeCurrent(myDisplay, myPBuffer, myContext))
            {
                std::cout << "Failed to activate render image" << std::endl;
                return false;
            }
        }
    }
    else
    {
        // We don't actually unbind the P-Buffer, for performances reasons
    }

    return true;
}


////////////////////////////////////////////////////////////
/// /see RenderImageImpl::UpdateTexture
////////////////////////////////////////////////////////////
bool RenderImageImplPBuffer::UpdateTexture(unsigned int textureId)
{
    // Store the current active context
    GLXDrawable currentDrawable = glXGetCurrentDrawable();
    GLXContext currentContext = glXGetCurrentContext();

    if (Activate(true))
    {
        // Bind the texture
        GLCheck(glEnable(GL_TEXTURE_2D));
        GLCheck(glBindTexture(GL_TEXTURE_2D, textureId));

        // Copy the rendered pixels to the image
        GLCheck(glCopyTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 0, 0, myWidth, myHeight));

        // Restore the previous context
        glXMakeCurrent(myDisplay, currentDrawable, currentContext);

        return true;
    }

    return false;
}

} // namespace priv

} // namespace sf
