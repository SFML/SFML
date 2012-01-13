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
#include <SFML/Graphics/RenderTexture.hpp>
#include <SFML/Graphics/RenderTextureImplFBO.hpp>
#include <SFML/Graphics/RenderTextureImplDefault.hpp>
#include <SFML/System/Err.hpp>


namespace sf
{
////////////////////////////////////////////////////////////
RenderTexture::RenderTexture() :
myImpl(NULL)
{

}


////////////////////////////////////////////////////////////
RenderTexture::~RenderTexture()
{
    delete myImpl;
}


////////////////////////////////////////////////////////////
bool RenderTexture::Create(unsigned int width, unsigned int height, bool depthBuffer)
{
    // Create the texture
    if (!myTexture.Create(width, height))
    {
        Err() << "Impossible to create render texture (failed to create the target texture)" << std::endl;
        return false;
    }

    // We disable smoothing by default for render textures
    SetSmooth(false);

    // Create the implementation
    delete myImpl;
    if (priv::RenderTextureImplFBO::IsAvailable())
    {
        // Use frame-buffer object (FBO)
        myImpl = new priv::RenderTextureImplFBO;
    }
    else
    {
        // Use default implementation
        myImpl = new priv::RenderTextureImplDefault;
    }

    // Initialize the render texture
    if (!myImpl->Create(width, height, myTexture.myTexture, depthBuffer))
        return false;

    // We can now initialize the render target part
    RenderTarget::Initialize();

    return true;
}


////////////////////////////////////////////////////////////
void RenderTexture::SetSmooth(bool smooth)
{
    myTexture.SetSmooth(smooth);
}


////////////////////////////////////////////////////////////
bool RenderTexture::IsSmooth() const
{
    return myTexture.IsSmooth();
}


////////////////////////////////////////////////////////////
bool RenderTexture::SetActive(bool active)
{
    return myImpl && myImpl->Activate(active);
}


////////////////////////////////////////////////////////////
void RenderTexture::Display()
{
    // Update the target texture
    if (SetActive(true))
    {
        myImpl->UpdateTexture(myTexture.myTexture);
        myTexture.myPixelsFlipped = true;
    }
}


////////////////////////////////////////////////////////////
unsigned int RenderTexture::GetWidth() const
{
    return myTexture.GetWidth();
}


////////////////////////////////////////////////////////////
unsigned int RenderTexture::GetHeight() const
{
    return myTexture.GetHeight();
}


////////////////////////////////////////////////////////////
const Texture& RenderTexture::GetTexture() const
{
    return myTexture;
}


////////////////////////////////////////////////////////////
bool RenderTexture::Activate(bool active)
{
    return SetActive(active);
}

} // namespace sf
