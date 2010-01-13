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
#include <SFML/Graphics/GLCheck.hpp>
#include <iostream>


namespace sf
{
////////////////////////////////////////////////////////////
/// Destructor
////////////////////////////////////////////////////////////
RenderTarget::~RenderTarget()
{
    // Nothing to do
}


////////////////////////////////////////////////////////////
/// Clear the entire target with a single color
////////////////////////////////////////////////////////////
void RenderTarget::Clear(const Color& color)
{
    if (Activate(true))
    {
        // Clear the color buffer
        GLCheck(glClearColor(color.r / 255.f, color.g / 255.f, color.b / 255.f, color.a / 255.f));
        GLCheck(glClear(GL_COLOR_BUFFER_BIT));

        // Clear the render queue
        myRenderQueue.Clear();

        Activate(false);
    }
}


////////////////////////////////////////////////////////////
/// Draw something on the window
////////////////////////////////////////////////////////////
void RenderTarget::Draw(const Drawable& object)
{
    // Save the current render states
    myRenderQueue.PushStates();

    // Setup the shader
    myRenderQueue.SetShader(NULL);

    // Setup the viewport
    myRenderQueue.SetViewport(GetViewport(myCurrentView));

    // Let the object draw itself
    object.Draw(*this, myRenderQueue);

    // Restore the previous render states
    myRenderQueue.PopStates();
}


////////////////////////////////////////////////////////////
/// Draw something into the target with a shader
////////////////////////////////////////////////////////////
void RenderTarget::Draw(const Drawable& object, const Shader& shader)
{
    // Save the current render states
    myRenderQueue.PushStates();

    // Setup the shader
    myRenderQueue.SetShader(&shader);

    // Setup the viewport
    myRenderQueue.SetViewport(GetViewport(myCurrentView));

    // Let the object draw itself
    object.Draw(*this, myRenderQueue);

    // Restore the previous render states
    myRenderQueue.PopStates();
}


////////////////////////////////////////////////////////////
/// Make sure that what has been drawn so far is rendered
////////////////////////////////////////////////////////////
void RenderTarget::Flush()
{
    if (Activate(true))
    {
        // Draw the whole render queue
        myRenderQueue.Render();

        Activate(false);
    }
}


////////////////////////////////////////////////////////////
/// Change the current active view
////////////////////////////////////////////////////////////
void RenderTarget::SetView(const View& view)
{
    // Save it
    myCurrentView = view;

    // Send the projection matrix to the render queue
    myRenderQueue.SetProjection(view.GetMatrix());
}


////////////////////////////////////////////////////////////
/// Get the current view
////////////////////////////////////////////////////////////
const View& RenderTarget::GetView() const
{
    return myCurrentView;
}


////////////////////////////////////////////////////////////
/// Get the default view of the window
////////////////////////////////////////////////////////////
const View& RenderTarget::GetDefaultView() const
{
    return myDefaultView;
}


////////////////////////////////////////////////////////////
/// Get the viewport of a view applied to this target
////////////////////////////////////////////////////////////
IntRect RenderTarget::GetViewport(const View& view) const
{
    float width  = static_cast<float>(GetWidth());
    float height = static_cast<float>(GetHeight());
    const FloatRect& viewport = view.GetViewport();

    IntRect rect;
    rect.Left   = static_cast<int>(0.5f + width  * viewport.Left);
    rect.Top    = static_cast<int>(0.5f + height * (1.f - viewport.Bottom));
    rect.Right  = static_cast<int>(0.5f + width  * viewport.Right);
    rect.Bottom = static_cast<int>(0.5f + height * (1.f - viewport.Top));

    return rect;
}


////////////////////////////////////////////////////////////
/// Convert a point in window coordinates into view coordinates
/// This version uses the current view of the window
////////////////////////////////////////////////////////////
sf::Vector2f RenderTarget::ConvertCoords(unsigned int x, unsigned int y) const
{
    return ConvertCoords(x, y, GetView());
}


////////////////////////////////////////////////////////////
/// Convert a point in window coordinates into view coordinates
/// This version uses the given view
////////////////////////////////////////////////////////////
sf::Vector2f RenderTarget::ConvertCoords(unsigned int x, unsigned int y, const View& view) const
{
    // First, convert from viewport coordinates to homogeneous coordinates
    Vector2f coords;
    IntRect viewport = GetViewport(view);
    coords.x = -1.f + 2.f * (static_cast<int>(x) - viewport.Left) / viewport.GetSize().x;
    coords.y =  1.f - 2.f * (static_cast<int>(y) - viewport.Top)  / viewport.GetSize().y;

    // Then transform by the inverse of the view matrix
    return view.GetInverseMatrix().Transform(coords);
}


////////////////////////////////////////////////////////////
/// Called by the derived class when it's ready to be initialized
////////////////////////////////////////////////////////////
void RenderTarget::Initialize()
{
    // Setup the default view
    myDefaultView.Reset(FloatRect(0, 0, static_cast<float>(GetWidth()), static_cast<float>(GetHeight())));
    SetView(myDefaultView);

    // Clear the render queue
    myRenderQueue.Clear();
}

} // namespace sf
