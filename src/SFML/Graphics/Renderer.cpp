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
#include <SFML/Graphics/Renderer.hpp>
#include <SFML/Graphics/Image.hpp>
#include <SFML/Graphics/Shader.hpp>
#include <SFML/Graphics/GLCheck.hpp>


////////////////////////////////////////////////////////////
// Private data
////////////////////////////////////////////////////////////
namespace
{
    // Fast float to int conversion
    inline sf::Int32 Round(double value)
    {
        // Use a union rather than reinterpret_cast, because it doesn't break strict-aliasing
        // rules and results in a correct behaviour when compiling in optimized mode
        union DoubleToInt
        {
            double d; 
            sf::Int32 i[2];
        };

        DoubleToInt u;
        u.d = value + 6755399441055744.0;

        #if defined(SFML_ENDIAN_LITTLE)
            return u.i[0];
        #else
            return u.i[1];
        #endif
    } 
}


namespace sf
{
////////////////////////////////////////////////////////////
Renderer::Renderer() :
myTextureIsValid  (false),
myShaderIsValid   (false),
myBlendModeIsValid(false),
myViewportIsValid (false)
{
    myStates = &myStatesStack[0];
}


////////////////////////////////////////////////////////////
Renderer::~Renderer()
{
}


////////////////////////////////////////////////////////////
void Renderer::Initialize()
{
    // Default render states
    GLCheck(glDisable(GL_LIGHTING));
    GLCheck(glDisable(GL_DEPTH_TEST));
    GLCheck(glEnable(GL_ALPHA_TEST));
    GLCheck(glAlphaFunc(GL_GREATER, 0));

    // Default transform matrices
    GLCheck(glMatrixMode(GL_MODELVIEW));
    GLCheck(glLoadIdentity());
    GLCheck(glMatrixMode(GL_PROJECTION));
    GLCheck(glLoadIdentity());

    // Invalidate the cached SFML states
    myTextureIsValid   = false;
    myShaderIsValid    = false;
    myBlendModeIsValid = false;
    myViewportIsValid  = false;
}


////////////////////////////////////////////////////////////
void Renderer::SaveGLStates()
{
    // Save render states
    GLCheck(glPushAttrib(GL_ALL_ATTRIB_BITS));
    //GLCheck(glPushClientAttrib(GL_CLIENT_ALL_ATTRIB_BITS));

    // Save matrices
    GLCheck(glMatrixMode(GL_MODELVIEW));
    GLCheck(glPushMatrix());
    GLCheck(glMatrixMode(GL_PROJECTION));
    GLCheck(glPushMatrix());
}


////////////////////////////////////////////////////////////
void Renderer::RestoreGLStates()
{
    // Restore render states
    GLCheck(glPopAttrib());
    //GLCheck(glPopClientAttrib());

    // Restore matrices
    GLCheck(glMatrixMode(GL_PROJECTION));
    GLCheck(glPopMatrix());
    GLCheck(glMatrixMode(GL_MODELVIEW));
    GLCheck(glPopMatrix());
}


////////////////////////////////////////////////////////////
void Renderer::Clear(const Color& color)
{
    GLCheck(glClearColor(color.r / 255.f, color.g / 255.f, color.b / 255.f, color.a / 255.f));
    GLCheck(glClear(GL_COLOR_BUFFER_BIT));
}


////////////////////////////////////////////////////////////
void Renderer::PushStates()
{
    myStates++;
    *myStates = *(myStates - 1);
}


////////////////////////////////////////////////////////////
void Renderer::PopStates()
{
    myStates--;
}


////////////////////////////////////////////////////////////
void Renderer::SetModelView(const Matrix3& matrix)
{
    myStates->modelView = matrix;
}


////////////////////////////////////////////////////////////
void Renderer::ApplyModelView(const Matrix3& matrix)
{
    myStates->modelView *= matrix;
}


////////////////////////////////////////////////////////////
void Renderer::SetProjection(const Matrix3& matrix)
{
    myProjection = matrix;
}


////////////////////////////////////////////////////////////
void Renderer::ApplyProjection(const Matrix3& matrix)
{
    myProjection *= matrix;
}


////////////////////////////////////////////////////////////
void Renderer::SetColor(const Color& color)
{
    myStates->r = color.r / 255.f;
    myStates->g = color.g / 255.f;
    myStates->b = color.b / 255.f;
    myStates->a = color.a / 255.f;
}


////////////////////////////////////////////////////////////
void Renderer::ApplyColor(const Color& color)
{
    myStates->r *= color.r / 255.f;
    myStates->g *= color.g / 255.f;
    myStates->b *= color.b / 255.f;
    myStates->a *= color.a / 255.f;
}


////////////////////////////////////////////////////////////
void Renderer::SetViewport(const IntRect& viewport)
{
    if ((viewport.Left != myViewport.Left) || (viewport.Right  != myViewport.Right)  ||
        (viewport.Top  != myViewport.Top)  || (viewport.Bottom != myViewport.Bottom) ||
        !myViewportIsValid)
    {
        // Apply the new viewport
        GLCheck(glViewport(viewport.Left, viewport.Top, viewport.GetSize().x, viewport.GetSize().y));

        // Store it
        myViewport = viewport;
        myViewportIsValid = true;

        // Store the half-size of the viewport for later computations
        myViewportSize.x = myViewport.GetSize().x / 2.f;
        myViewportSize.y = myViewport.GetSize().y / 2.f;
    }
}


////////////////////////////////////////////////////////////
void Renderer::SetBlendMode(Blend::Mode mode)
{
    if ((mode != myBlendMode) || !myBlendModeIsValid)
    {
        // Apply the new blending mode
        if (mode == Blend::None)
        {
            GLCheck(glDisable(GL_BLEND));
        }
        else
        {
            GLCheck(glEnable(GL_BLEND));

            switch (mode)
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

        // Store it
        myBlendMode = mode;
        myBlendModeIsValid = true;
    }
}


////////////////////////////////////////////////////////////
void Renderer::SetTexture(const Image* texture)
{
    if ((texture != myTexture) || !myTextureIsValid)
    {
        // Apply the new texture
        if (texture)
            texture->Bind();
        else
            GLCheck(glDisable(GL_TEXTURE_2D));

        // Store it
        myTexture = texture;
        myTextureIsValid = true;
    }
}


////////////////////////////////////////////////////////////
void Renderer::SetShader(const Shader* shader)
{
    if ((shader != myShader) || !myShaderIsValid)
    {
        // Apply the new shader
        if (shader)
            shader->Bind();
        else if (myShaderIsValid && myShader)
            myShader->Unbind();

        // Store it
        myShader = shader;
        myShaderIsValid = false;
    }
}


////////////////////////////////////////////////////////////
void Renderer::Begin(PrimitiveType type)
{
    // Update the combined transform matrix
    myTransform = myProjection * myStates->modelView;

    // Begin rendering
    switch (type)
    {
        case TriangleList :  glBegin(GL_TRIANGLES);      break;
        case TriangleStrip : glBegin(GL_TRIANGLE_STRIP); break;
        case TriangleFan :   glBegin(GL_TRIANGLE_FAN);   break;
        case QuadList :      glBegin(GL_QUADS);          break;
        default: break;
    }
}


////////////////////////////////////////////////////////////
void Renderer::End()
{
    // End rendering
    glEnd();
}


////////////////////////////////////////////////////////////
void Renderer::AddVertex(float x, float y)
{
    ProcessVertex(x, y, 0.f, 0.f, 1.f, 1.f, 1.f, 1.f);
}


////////////////////////////////////////////////////////////
void Renderer::AddVertex(float x, float y, float u, float v)
{
    ProcessVertex(x, y, u, v, 1.f, 1.f, 1.f, 1.f);
}


////////////////////////////////////////////////////////////
void Renderer::AddVertex(float x, float y, const Color& color)
{
    ProcessVertex(x, y, 0.f, 0.f, color.r / 255.f, color.g / 255.f, color.b / 255.f, color.a / 255.f);
}


////////////////////////////////////////////////////////////
void Renderer::AddVertex(float x, float y, float u, float v, const Color& color)
{
    ProcessVertex(x, y, u, v, color.r / 255.f, color.g / 255.f, color.b / 255.f, color.a / 255.f);
}


////////////////////////////////////////////////////////////
void Renderer::ProcessVertex(float x, float y, float u, float v, float r, float g, float b, float a)
{
    // Transform the vertex position by the current model-view-projection matrix
    Vector2f position = myTransform.Transform(Vector2f(x, y));

    // Round the vertex position so that it matches the
    // viewport's pixels, and thus avoid rendering artifacts
    // @todo remove and find a better solution :)
    position.x = Round((position.x + 1.f) * myViewportSize.x) / myViewportSize.x - 1.f;
    position.y = Round((position.y + 1.f) * myViewportSize.y) / myViewportSize.y - 1.f;

    // Modulate the vertex color with the current global color
    r *= myStates->r;
    g *= myStates->g;
    b *= myStates->b;
    a *= myStates->a;

    // Render the vertex
    glColor4f(r, g, b, a);
    glTexCoord2f(u, v);
    glVertex2f(position.x, position.y);
}

} // namespace sf
