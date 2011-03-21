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
#include <SFML/System/Err.hpp>


namespace sf
{
namespace priv
{
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
RenderImageImplPBuffer::~RenderImageImplPBuffer()
{
    EnsureGlContext();

    if (myContext)
        wglDeleteContext(myContext);

    if (myPBuffer && myDeviceContext)
    {
        wglReleasePbufferDCARB(myPBuffer, myDeviceContext);
        wglDestroyPbufferARB(myPBuffer);
    }
}


////////////////////////////////////////////////////////////
bool RenderImageImplPBuffer::IsSupported()
{
    EnsureGlContext();

    // Make sure that GLEW is initialized
    priv::EnsureGlewInit();

    return WGLEW_ARB_pbuffer && WGLEW_ARB_pixel_format;
}


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
        Err() << "Impossible to create render image (failed to find a suitable pixel format for PBuffer)" << std::endl;
        return false;
    }

    // Create the P-Buffer and its OpenGL context
    myPBuffer       = wglCreatePbufferARB(currentDC, pixelFormat, width, height, NULL);
    myDeviceContext = wglGetPbufferDCARB(myPBuffer);
    myContext       = wglCreateContext(myDeviceContext);

    // Check errors
    if (!myPBuffer || !myDeviceContext || !myContext)
    {
        Err() << "Impossible to create render image (failed to create PBuffer)" << std::endl;
        return false;
    }

    // Check the actual size of the P-Buffer
    int actualWidth, actualHeight;
    wglQueryPbufferARB(myPBuffer, WGL_PBUFFER_WIDTH_ARB, &actualWidth);
    wglQueryPbufferARB(myPBuffer, WGL_PBUFFER_HEIGHT_ARB, &actualHeight);
    if ((actualWidth != static_cast<int>(width)) || (actualHeight != static_cast<int>(height)))
    {
        Err() << "Impossible to create render image (failed to match the requested size). "
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
bool RenderImageImplPBuffer::Activate(bool active)
{
    if (active)
    {
        if (myDeviceContext && myContext)
        {
            if (wglGetCurrentContext() != myContext)
                return wglMakeCurrent(myDeviceContext, myContext) != 0;
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
        // @todo fix
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
