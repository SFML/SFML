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
    Context context;
}


////////////////////////////////////////////////////////////
/// Check whether the system supports P-Buffer or not
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
bool RenderImageImplPBuffer::Create(unsigned int width, unsigned int height, unsigned int, bool depthBuffer)
{
    // Store the dimensions
    myWidth = width;
    myHeight = height;

    // Get the current HDC
    HDC currentDC = wglGetCurrentDC();

    // Define the minimum PBuffer attributes
    int attributes[] =
    {
        WGL_SUPPORT_OPENGL_ARB,  GL_TRUE,
        WGL_DRAW_TO_PBUFFER_ARB, GL_TRUE,
        WGL_RED_BITS_ARB,        8,
        WGL_GREEN_BITS_ARB,      8,
        WGL_BLUE_BITS_ARB,       8,
        WGL_ALPHA_BITS_ARB,      8,
        WGL_DEPTH_BITS_ARB,      (depthBuffer ? 24 : 0),
        WGL_DOUBLE_BUFFER_ARB,   GL_FALSE,
        0
    };

    // Select the best pixel format for our attributes
	unsigned int nbFormats = 0;
	int pixelFormat = -1;
	wglChoosePixelFormatARB(currentDC, attributes, NULL, 1, &pixelFormat, &nbFormats);

    // Make sure that one pixel format has been found
    if (nbFormats == 0)
    {
        std::cerr << "Impossible to create render image (failed to find a suitable pixel format for PBuffer)" << std::endl;
        return false;
    }

    // Create the P-Buffer and its OpenGL context
    myPBuffer       = wglCreatePbufferARB(currentDC, pixelFormat, width, height, NULL);
    myDeviceContext = wglGetPbufferDCARB(myPBuffer);
    myContext       = wglCreateContext(myDeviceContext);

    // Check errors
    if (!myPBuffer || !myDeviceContext || !myContext)
    {
        std::cerr << "Impossible to create render image (failed to create PBuffer)" << std::endl;
        return false;
    }

    // Check the actual size of the P-Buffer
    int actualWidth, actualHeight;
    wglQueryPbufferARB(myPBuffer, WGL_PBUFFER_WIDTH_ARB, &actualWidth);
    wglQueryPbufferARB(myPBuffer, WGL_PBUFFER_HEIGHT_ARB, &actualHeight);
    if ((actualWidth != static_cast<int>(width)) || (actualHeight != static_cast<int>(height)))
    {
        std::cerr << "Impossible to create render image (failed to match the requested size). "
                  << "Size: " << actualWidth << "x" << actualHeight << " - "
                  << "Requested: " << width << "x" << height
                  << std::endl;
        return false;
    }

    // Share the P-Buffer context with the current context
    HGLRC currentContext = wglGetCurrentContext();
    if (currentContext)
    {
        wglMakeCurrent(NULL, NULL);
        wglShareLists(currentContext, myContext);
        wglMakeCurrent(currentDC, currentContext);
    }

    return true;
}


////////////////////////////////////////////////////////////
/// /see RenderImageImpl::Activate
////////////////////////////////////////////////////////////
bool RenderImageImplPBuffer::Activate(bool active)
{
    if (active)
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
bool RenderImageImplPBuffer::UpdateTexture(unsigned int textureId)
{
    // Store the current active context
    HDC currentDC = wglGetCurrentDC();
    HGLRC currentContext = wglGetCurrentContext();

    if (Activate(true))
    {
        // Bind the texture
        GLCheck(glEnable(GL_TEXTURE_2D));
        GLCheck(glBindTexture(GL_TEXTURE_2D, textureId));

        // Copy the rendered pixels to the image
        GLCheck(glCopyTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 0, 0, myWidth, myHeight));

        // Restore the previous context
        wglMakeCurrent(currentDC, currentContext);

        return true;
    }

    return false;
}

} // namespace priv

} // namespace sf
