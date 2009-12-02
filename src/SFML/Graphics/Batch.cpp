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
#include <SFML/Graphics/Batch.hpp>
#include <SFML/Graphics/Image.hpp>
#include <SFML/Graphics/Shader.hpp>
#include <SFML/Graphics/GeometryRenderer.hpp>
#include <SFML/Graphics/GLCheck.hpp>


namespace sf
{
namespace priv
{
////////////////////////////////////////////////////////////
Batch::Batch(const Image* texture, const Shader* shader, Blend::Mode blendMode, const IntRect& viewport) :
myTexture  (texture),
myShader   (shader),
myBlendMode(blendMode),
myViewport (viewport),
myStart    (0),
myCount    (0)
{
}


////////////////////////////////////////////////////////////
bool Batch::Matches(const Image* texture, const Shader* shader, Blend::Mode blendMode, const IntRect& viewport) const
{
    return myTexture         == texture         &&
           myShader          == shader          &&
           myBlendMode       == blendMode       &&
           myViewport.Left   == viewport.Left   &&
           myViewport.Top    == viewport.Top    &&
           myViewport.Right  == viewport.Right  &&
           myViewport.Bottom == viewport.Bottom;
}


////////////////////////////////////////////////////////////
void Batch::Begin(Uint32 index)
{
    myStart = index;
}


////////////////////////////////////////////////////////////
void Batch::End(Uint32 index)
{
    myCount = index - myStart;
}


////////////////////////////////////////////////////////////
void Batch::Render(GeometryRenderer& renderer) const
{
    // Set the viewport
    GLCheck(glViewport(myViewport.Left, myViewport.Top, myViewport.GetSize().x, myViewport.GetSize().y));

    // Set the blending mode
    if (myBlendMode == Blend::None)
    {
        GLCheck(glDisable(GL_BLEND));
    }
    else
    {
        GLCheck(glEnable(GL_BLEND));

        switch (myBlendMode)
        {
            // Alpha blending
            // glBlendFuncSeparateEXT is used when available to avoid an incorrect alpha value when the target
            // is a RenderImage -- in this case the alpha value must be written directly to the target buffer
            default :
            case Blend::Alpha :
                if (GLEW_EXT_blend_func_separate)
                    GLCheck(glBlendFuncSeparateEXT(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE_MINUS_SRC_ALPHA));
                else
                    GLCheck(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
                break;

            // Additive blending
            case Blend::Add :
                GLCheck(glBlendFunc(GL_SRC_ALPHA, GL_ONE));
                break;

            // Multiplicative blending
            case Blend::Multiply :
                GLCheck(glBlendFunc(GL_DST_COLOR, GL_ZERO));
                break;
        }
    }

    // Bind the texture
    if (myTexture)
        myTexture->Bind();
    else
        GLCheck(glDisable(GL_TEXTURE_2D));

    // Bind the pixel shader
    if (myShader)
        myShader->Bind();

    // Render the triangles
    renderer.RenderTriangles(myStart, myCount);

    // Disable the pixel shader
    if (myShader)
        myShader->Unbind();
}

} // namespace priv

} // namespace sf
