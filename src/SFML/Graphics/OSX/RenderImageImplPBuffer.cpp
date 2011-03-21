////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007-2011 Marco Antognini (antognini.marco@gmail.com), 
//                         Laurent Gomila (laurent.gom@gmail.com), 
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
#include <SFML/Graphics/OSX/RenderImageImplPBuffer.hpp>
#include <SFML/Graphics/GLCheck.hpp>
#include <SFML/Window/Context.hpp>
#include <SFML/System/Err.hpp>

namespace sf
{
namespace priv
{
////////////////////////////////////////////////////////////
RenderImageImplPBuffer::RenderImageImplPBuffer() :
    myPBuffer(NULL),
    myContext(NULL),
    myWidth  (0),
    myHeight (0) 
{
    /* Nothing else */
}


////////////////////////////////////////////////////////////
RenderImageImplPBuffer::~RenderImageImplPBuffer()
{
    if (myPBuffer && aglDestroyPBuffer(myPBuffer) == GL_FALSE) {
        sf::Err() 
        << "An error occurs while destroying the PBuffer in "
        << __PRETTY_FUNCTION__
        << ". The error code is "
        << aglGetError()
        << std::endl;
    }
    
    if (myContext && aglDestroyContext(myContext) == GL_FALSE) {
        sf::Err() 
        << "An error occurs while destroying the context in "
        << __PRETTY_FUNCTION__
        << ". The error code is "
        << aglGetError()
        << std::endl;
    }
}


////////////////////////////////////////////////////////////
bool RenderImageImplPBuffer::IsSupported()
{
    EnsureGlContext();

    const GLubyte* strExt = glGetString(GL_EXTENSIONS);
    GLboolean isSupported = gluCheckExtension((const GLubyte*)"GL_APPLE_pixel_buffer", strExt);
    
    return isSupported;
}


////////////////////////////////////////////////////////////
bool RenderImageImplPBuffer::Create(unsigned int width, unsigned int height, unsigned int, bool depthBuffer)
{
    // Store the dimensions
    myWidth  = width;
    myHeight = height;

    // Create the pixel format.
    GLint attribs[] = {
        AGL_RGBA,
        AGL_RED_SIZE,      8,
        AGL_GREEN_SIZE,    8,
        AGL_BLUE_SIZE,     8,
        AGL_ALPHA_SIZE,    8,
        AGL_DEPTH_SIZE,    (depthBuffer ? 24 : 0),
        0
    };
    
    AGLPixelFormat pf = aglChoosePixelFormat(NULL, 0, attribs);
    if (!pf) {
        sf::Err()
        << "Couldn't create the pixel format for the PBuffer."
        << std::endl;
        
        return false;
    }
    
    // Create the context.
    myContext = aglCreateContext(pf, NULL);
    if (!myContext) {
        sf::Err()
        << "Couldn't create the context for the PBuffer. (Error : "
        << aglGetError()
        << ")"
        << std::endl;
        
        return false;
    }
    
    // Create the PBuffer.
    GLboolean status = aglCreatePBuffer(myWidth, myHeight, GL_TEXTURE_RECTANGLE_EXT, GL_RGBA, 0, &myPBuffer);
    if (status == GL_FALSE) {
        sf::Err()
        << "Couldn't create the PBuffer. (Error : "
        << aglGetError()
        << ")"
        << std::endl;
        
        return false;
    }
    
    // Set up the PBuffer with the context.
    GLint screen = aglGetVirtualScreen(myContext);
    if (screen == -1) {
        sf::Err()
        << "Couldn't get the virtual screen of the context used with the PBuffer. (Error : "
        << aglGetError()
        << ")"
        << std::endl;
        
        return false;
    }
    
    status = aglSetPBuffer(myContext, myPBuffer, 0, 0, screen);
    if (status == GL_FALSE) {
        sf::Err()
        << "Couldn't set up the PBuffer with the context. (Error : "
        << aglGetError()
        << ")"
        << std::endl;
        
        return false;
    }
    
    return true;
}


////////////////////////////////////////////////////////////
bool RenderImageImplPBuffer::Activate(bool active)
{
    if (active)
    {
        if (!myContext || !myPBuffer) // Not created yet.
            return false;
        
        if (aglGetCurrentContext() == myContext)
            return true;
        else
            return aglSetCurrentContext(myContext);
    }
    else
    {
        // To deactivate the P-Buffer's context, we actually activate
        // another one so that we make sure that there is always an
        // active context for subsequent graphics operations
        // @todo fixme
        // return Context::SetReferenceActive();
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
