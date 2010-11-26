////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007-2010 Marco Antognini (antognini.marco@gmail.com), 
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

#warning RenderImageImplPBuffer not yet implemented

namespace sf
{
namespace priv
{
////////////////////////////////////////////////////////////
RenderImageImplPBuffer::RenderImageImplPBuffer()
{
	/* TODO */
}


////////////////////////////////////////////////////////////
RenderImageImplPBuffer::~RenderImageImplPBuffer()
{
  /* TODO */

	// This is to make sure that another valid context is made
  // active after we destroy the P-Buffer's one
	Context::SetReferenceActive();
}


////////////////////////////////////////////////////////////
bool RenderImageImplPBuffer::IsSupported()
{
	/* TODO */
	return false;
}


////////////////////////////////////////////////////////////
bool RenderImageImplPBuffer::Create(unsigned int width, unsigned int height, unsigned int, bool depthBuffer)
{
	// Store the dimensions
	myWidth  = width;
	myHeight = height;

	/* TODO */

  return true;
}


////////////////////////////////////////////////////////////
bool RenderImageImplPBuffer::Activate(bool active)
{
    if (active)
    {
        if (false/*myPBuffer && myContext*/)
        {
            // try to activate
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
        return Context::SetReferenceActive();
    }
}


////////////////////////////////////////////////////////////
void RenderImageImplPBuffer::UpdateTexture(unsigned int textureId)
{
    if (Activate(true))
    {
        GLint previous;
        GLCheck(glGetIntegerv(GL_TEXTURE_BINDING_2D, &previous));

        // Copy the rendered pixels to the image
        GLCheck(glBindTexture(GL_TEXTURE_2D, textureId));
        GLCheck(glCopyTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 0, 0, myWidth, myHeight));

        GLCheck(glBindTexture(GL_TEXTURE_2D, previous));
    }
}

} // namespace priv

} // namespace sf
