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
#include <SFML/Graphics/Win32/RenderImageImplPBuffer.hpp>
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
myPBuffer      (NULL),
myDeviceContext(NULL),
myContext      (NULL),
myWidth        (0),
myHeight       (0)
{

}


////////////////////////////////////////////////////////////
/// Destructor
////////////////////////////////////////////////////////////
RenderImageImplPBuffer::~RenderImageImplPBuffer()
{
    if (myContext)
    {
        wglDeleteContext(myContext);
    }

    if (myPBuffer && myDeviceContext)
    {
        wglReleasePbufferDCARB(myPBuffer, myDeviceContext);
        wglDestroyPbufferARB(myPBuffer);
    }

    // This is to make sure that another valid context is made
    // active after we destroy the P-Buffer's one
    Context Ctx;
}


////////////////////////////////////////////////////////////
/// Check whether the system supports FBOs or not
////////////////////////////////////////////////////////////
bool RenderImageImplPBuffer::IsSupported()
{
    // Make sure that GLEW is initialized
    EnsureGlewInit();

    return wglewIsSupported("WGL_ARB_pbuffer") &&
           wglewIsSupported("WGL_ARB_pixel_format");
}


////////////////////////////////////////////////////////////
/// /see RenderImageImpl::Create
////////////////////////////////////////////////////////////
bool RenderImageImplPBuffer::Create(unsigned int Width, unsigned int Height, unsigned int /*TextureId*/, bool DepthBuffer)
{
    // Store the dimensions
    myWidth = Width;
    myHeight = Height;

    // Get the current HDC
    HDC CurrentDeviceContext = wglGetCurrentDC();

    // Define the minimum PBuffer attributes
    int Attributes[] =
    {
        WGL_SUPPORT_OPENGL_ARB,  GL_TRUE,
        WGL_DRAW_TO_PBUFFER_ARB, GL_TRUE,
        WGL_RED_BITS_ARB,        8,
        WGL_GREEN_BITS_ARB,      8,
        WGL_BLUE_BITS_ARB,       8,
        WGL_ALPHA_BITS_ARB,      8,
        WGL_DEPTH_BITS_ARB,      (DepthBuffer ? 24 : 0),
        WGL_DOUBLE_BUFFER_ARB,   GL_FALSE,
        0
    };

    // Select the best pixel format for our attributes
	unsigned int NbFormats = 0;
	int PixelFormat = -1;
	wglChoosePixelFormatARB(CurrentDeviceContext, Attributes, NULL, 1, &PixelFormat, &NbFormats);

    // Make sure that one pixel format has been found
    if (NbFormats == 0)
    {
        std::cerr << "Impossible to create render image (failed to find a suitable pixel format for PBuffer)" << std::endl;
        return false;
    }

    // Create the P-Buffer and its OpenGL context
    myPBuffer       = wglCreatePbufferARB(CurrentDeviceContext, PixelFormat, Width, Height, NULL);
    myDeviceContext = wglGetPbufferDCARB(myPBuffer);
    myContext       = wglCreateContext(myDeviceContext);

    // Check errors
    if (!myPBuffer || !myDeviceContext || !myContext)
    {
        std::cerr << "Impossible to create render image (failed to create PBuffer)" << std::endl;
        return false;
    }

    // Check the actual size of the P-Buffer
    int ActualWidth, ActualHeight;
    wglQueryPbufferARB(myPBuffer, WGL_PBUFFER_WIDTH_ARB, &ActualWidth);
    wglQueryPbufferARB(myPBuffer, WGL_PBUFFER_HEIGHT_ARB, &ActualHeight);
    if ((ActualWidth != static_cast<int>(Width)) || (ActualHeight != static_cast<int>(Height)))
    {
        std::cerr << "Impossible to create render image (failed to match the requested size). "
                  << "Size: " << ActualWidth << "x" << ActualHeight << " - "
                  << "Requested: " << Width << "x" << Height
                  << std::endl;
        return false;
    }

    // Share the P-Buffer context with the current context
    HGLRC CurrentContext = wglGetCurrentContext();
    if (CurrentContext)
    {
        wglMakeCurrent(NULL, NULL);
        wglShareLists(CurrentContext, myContext);
        wglMakeCurrent(CurrentDeviceContext, CurrentContext);
    }

    return true;
}


////////////////////////////////////////////////////////////
/// /see RenderImageImpl::Activate
////////////////////////////////////////////////////////////
bool RenderImageImplPBuffer::Activate(bool Active)
{
    if (Active)
    {
        if (myDeviceContext && myContext && (wglGetCurrentContext() != myContext))
        {
            // Bind the OpenGL context of the P-Buffer
            if (!wglMakeCurrent(myDeviceContext, myContext))
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
    HDC CurrentDeviceContext = wglGetCurrentDC();
    HGLRC CurrentContext = wglGetCurrentContext();

    if (Activate(true))
    {
        // Bind the texture
        GLCheck(glEnable(GL_TEXTURE_2D));
        GLCheck(glBindTexture(GL_TEXTURE_2D, TextureId));

        // Copy the rendered pixels to the image
        GLCheck(glCopyTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 0, 0, myWidth, myHeight));

        // Restore the previous context
        wglMakeCurrent(CurrentDeviceContext, CurrentContext);

        return true;
    }

    return false;
}

} // namespace priv

} // namespace sf
