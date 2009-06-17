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
    Context Ctx;
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
bool RenderImageImplPBuffer::Create(unsigned int Width, unsigned int Height, unsigned int /*TextureId*/, bool DepthBuffer)
{
    // Store the dimensions
    myWidth = Width;
    myHeight = Height;

    // Define the PBuffer attributes
    int VisualAttributes[] =
    {
        GLX_RENDER_TYPE,   GLX_RGBA_BIT,
        GLX_DRAWABLE_TYPE, GLX_PBUFFER_BIT,
        GLX_RED_SIZE,      8,
        GLX_GREEN_SIZE,    8,
        GLX_BLUE_SIZE,     8,
        GLX_ALPHA_SIZE,    8,
        GLX_DEPTH_SIZE,    (DepthBuffer ? 24 : 0),
        0
    };
    int PBufferAttributes[] =
    {
        GLX_PBUFFER_WIDTH, 	Width,
        GLX_PBUFFER_HEIGHT, Height,
        0
    };

    // Get the available FB configurations
    int NbConfigs = 0;
    GLXFBConfig* Configs = glXChooseFBConfigSGIX(myDisplay, DefaultScreen(myDisplay), VisualAttributes, &NbConfigs);
    if (!Configs || !NbConfigs)
    {
        std::cerr << "Impossible to create render image (failed to find a suitable pixel format for PBuffer)" << std::endl;
        return false;
    }

    // Create the P-Buffer
    myPBuffer = glXCreateGLXPbufferSGIX(myDisplay, Configs[0], Width, Height, PBufferAttributes);
    if (!myPBuffer)
    {
        std::cerr << "Impossible to create render image (failed to create the OpenGL PBuffer)" << std::endl;
        XFree(Configs);
        return false;
    }

    // Check the actual size of the P-Buffer
    unsigned int ActualWidth, ActualHeight;
    glXQueryGLXPbufferSGIX(myDisplay, myPBuffer, GLX_WIDTH_SGIX, &ActualWidth);
    glXQueryGLXPbufferSGIX(myDisplay, myPBuffer, GLX_HEIGHT_SGIX, &ActualHeight);
    if ((ActualWidth != Width) || (ActualHeight != Height))
    {
        std::cerr << "Impossible to create render image (failed to match the requested size). "
                  << "Size: " << ActualWidth << "x" << ActualHeight << " - "
                  << "Requested: " << Width << "x" << Height
                  << std::endl;
        XFree(Configs);
        return false;
    }

    // We'll have to share the P-Buffer context with the current context
    GLXDrawable CurrentDrawable = glXGetCurrentDrawable();
    GLXContext CurrentContext = glXGetCurrentContext();
    if (CurrentContext)
        glXMakeCurrent(myDisplay, NULL, NULL);

    // Create the context
    XVisualInfo* Visual = glXGetVisualFromFBConfig(myDisplay, Configs[0]);
    myContext = glXCreateContext(myDisplay, Visual, CurrentContext, true);
    if (!myContext)
    {
        std::cerr << "Impossible to create render image (failed to create the OpenGL context)" << std::endl;
        XFree(Configs);
        XFree(Visual);
        return false;
    }
 
    // Restore the previous active context
    if (CurrentContext)
        glXMakeCurrent(myDisplay, CurrentDrawable, CurrentContext);
 
    // Cleanup resources
    XFree(Configs);
    XFree(Visual);

    return true;
}


////////////////////////////////////////////////////////////
/// /see RenderImageImpl::Activate
////////////////////////////////////////////////////////////
bool RenderImageImplPBuffer::Activate(bool Active)
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
bool RenderImageImplPBuffer::UpdateTexture(unsigned int TextureId)
{
    // Store the current active context
    GLXDrawable CurrentDrawable = glXGetCurrentDrawable();
    GLXContext CurrentContext = glXGetCurrentContext();

    if (Activate(true))
    {
        // Bind the texture
        GLCheck(glEnable(GL_TEXTURE_2D));
        GLCheck(glBindTexture(GL_TEXTURE_2D, TextureId));

        // Copy the rendered pixels to the image
        GLCheck(glCopyTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 0, 0, myWidth, myHeight));

        // Restore the previous context
        glXMakeCurrent(myDisplay, CurrentDrawable, CurrentContext);

        return true;
    }

    return false;
}

} // namespace priv

} // namespace sf
