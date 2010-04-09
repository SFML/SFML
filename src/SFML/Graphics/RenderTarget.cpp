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
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <iostream>

#ifdef _MSC_VER
    #pragma warning(disable : 4355) // "'this' : used in base member initializer list"
#endif


namespace sf
{
////////////////////////////////////////////////////////////
RenderTarget::RenderTarget() :
myRenderer      (*this),
myStatesSaved   (false),
myViewHasChanged(false)
{

}


////////////////////////////////////////////////////////////
RenderTarget::~RenderTarget()
{
    // Nothing to do
}


////////////////////////////////////////////////////////////
void RenderTarget::Clear(const Color& color)
{
    if (Activate(true))
        myRenderer.Clear(color);
}


////////////////////////////////////////////////////////////
void RenderTarget::Draw(const Drawable& object)
{
    if (Activate(true))
    {
        // Update the projection matrix and viewport if the current view has changed
        // Note: we do the changes here and not directly in SetView in order to gather
        // rendering commands, which is safer in multithreaded environments
        if (myViewHasChanged)
        {
            myRenderer.SetProjection(myCurrentView.GetMatrix());
            myRenderer.SetViewport(GetViewport(myCurrentView));
            myViewHasChanged = false;
        }

        // Save the current render states
        myRenderer.PushStates();

        // Setup the shader
        myRenderer.SetShader(NULL);

        // Let the object draw itself
        object.Draw(*this, myRenderer);

        // Restore the previous render states
        myRenderer.PopStates();
    }
}


////////////////////////////////////////////////////////////
void RenderTarget::Draw(const Drawable& object, const Shader& shader)
{
    if (Activate(true))
    {
        // Update the projection matrix and viewport if the current view has changed
        // Note: we do the changes here and not directly in SetView in order to gather
        // rendering commands, which is safer in multithreaded environments
        if (myViewHasChanged)
        {
            myRenderer.SetProjection(myCurrentView.GetMatrix());
            myRenderer.SetViewport(GetViewport(myCurrentView));
            myViewHasChanged = false;
        }

        // Save the current render states
        myRenderer.PushStates();

        // Setup the shader
        myRenderer.SetShader(&shader);

        // Let the object draw itself
        object.Draw(*this, myRenderer);

        // Restore the previous render states
        myRenderer.PopStates();
    }
}


////////////////////////////////////////////////////////////
void RenderTarget::SetView(const View& view)
{
    // Save it for later use
    myCurrentView = view;
    myViewHasChanged = true;
}


////////////////////////////////////////////////////////////
const View& RenderTarget::GetView() const
{
    return myCurrentView;
}


////////////////////////////////////////////////////////////
const View& RenderTarget::GetDefaultView() const
{
    return myDefaultView;
}


////////////////////////////////////////////////////////////
IntRect RenderTarget::GetViewport(const View& view) const
{
    float width  = static_cast<float>(GetWidth());
    float height = static_cast<float>(GetHeight());
    const FloatRect& viewport = view.GetViewport();

    return IntRect(static_cast<int>(0.5f + width  * viewport.Left),
                   static_cast<int>(0.5f + height * viewport.Top),
                   static_cast<int>(width  * viewport.Width),
                   static_cast<int>(height * viewport.Height));
}


////////////////////////////////////////////////////////////
Vector2f RenderTarget::ConvertCoords(unsigned int x, unsigned int y) const
{
    return ConvertCoords(x, y, GetView());
}


////////////////////////////////////////////////////////////
Vector2f RenderTarget::ConvertCoords(unsigned int x, unsigned int y, const View& view) const
{
    // First, convert from viewport coordinates to homogeneous coordinates
    Vector2f coords;
    IntRect viewport = GetViewport(view);
    coords.x = -1.f + 2.f * (static_cast<int>(x) - viewport.Left) / viewport.Width;
    coords.y = 1.f  - 2.f * (static_cast<int>(y) - viewport.Top)  / viewport.Height;

    // Then transform by the inverse of the view matrix
    return view.GetInverseMatrix().Transform(coords);
}


////////////////////////////////////////////////////////////
void RenderTarget::SaveGLStates()
{
    if (Activate(true))
    {
        myRenderer.SaveGLStates();
        myStatesSaved = true;

        // Restore the render states and the current view, for SFML rendering
        myRenderer.Initialize();
        SetView(GetView());
    }
}


////////////////////////////////////////////////////////////
void RenderTarget::RestoreGLStates()
{
    if (myStatesSaved)
    {
        if (Activate(true))
        {
            myRenderer.RestoreGLStates();
            myStatesSaved = false;
        }
    }
}


////////////////////////////////////////////////////////////
void RenderTarget::Initialize()
{
    // Setup the default view
    myDefaultView.Reset(FloatRect(0, 0, static_cast<float>(GetWidth()), static_cast<float>(GetHeight())));
    SetView(myDefaultView);

    // Initialize the renderer
    if (Activate(true))
        myRenderer.Initialize();
}

} // namespace sf
