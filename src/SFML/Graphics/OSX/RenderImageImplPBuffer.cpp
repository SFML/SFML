
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
