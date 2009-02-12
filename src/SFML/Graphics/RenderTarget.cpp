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
#include <SFML/Graphics/GraphicsContext.hpp>
#include <iostream>


namespace sf
{
////////////////////////////////////////////////////////////
/// Default constructor
////////////////////////////////////////////////////////////
RenderTarget::RenderTarget() :
myCurrentView   (&myDefaultView),
myPreserveStates(false),
myIsDrawing     (false)
{

}


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
void RenderTarget::Clear(const Color& FillColor)
{
    if (Activate(true))
    {
        // Clear the frame buffer
        GLCheck(glClearColor(FillColor.r / 255.f, FillColor.g / 255.f, FillColor.b / 255.f, FillColor.a / 255.f));
        GLCheck(glClear(GL_COLOR_BUFFER_BIT));

        Activate(false);
    }
}


////////////////////////////////////////////////////////////
/// Draw something on the window
////////////////////////////////////////////////////////////
void RenderTarget::Draw(const Drawable& Object)
{
    // Check whether we are called from the outside or from a previous call to Draw
    if (!myIsDrawing)
    {
        myIsDrawing = true;

        // Set our target as the current target for rendering
        if (Activate(true))
        {
            // Save the current render states and set the SFML ones
            if (myPreserveStates)
            {
                GLCheck(glMatrixMode(GL_MODELVIEW));  GLCheck(glPushMatrix());
                GLCheck(glMatrixMode(GL_PROJECTION)); GLCheck(glPushMatrix());
                GLCheck(glPushAttrib(GL_COLOR_BUFFER_BIT | GL_CURRENT_BIT   | GL_ENABLE_BIT  |
                                     GL_TEXTURE_BIT      | GL_TRANSFORM_BIT | GL_VIEWPORT_BIT));
                SetRenderStates();
            }

            // Set the window viewport and transform matrices
            GLCheck(glViewport(0, 0, GetWidth(), GetHeight()));
            GLCheck(glMatrixMode(GL_PROJECTION)); GLCheck(glLoadMatrixf(myCurrentView->GetMatrix().Get4x4Elements()));
            GLCheck(glMatrixMode(GL_MODELVIEW));  GLCheck(glLoadIdentity());

            // Let the object draw itself
            Object.Draw(*this);

            // Restore render states
            if (myPreserveStates)
            {
                GLCheck(glMatrixMode(GL_PROJECTION)); GLCheck(glPopMatrix());
                GLCheck(glMatrixMode(GL_MODELVIEW));  GLCheck(glPopMatrix());
                GLCheck(glPopAttrib());
            }

            // Deactivate rendering on this target
            Activate(false);
        }

        myIsDrawing = false;
    }
    else
    {
        // We are already called from a previous Draw : we don't need to set the states again, just draw the object
        Object.Draw(*this);
    }
}


////////////////////////////////////////////////////////////
/// Change the current active view
////////////////////////////////////////////////////////////
void RenderTarget::SetView(const View& NewView)
{
    myCurrentView = &NewView;
}


////////////////////////////////////////////////////////////
/// Get the current view
////////////////////////////////////////////////////////////
const View& RenderTarget::GetView() const
{
    return *myCurrentView;
}


////////////////////////////////////////////////////////////
/// Get the default view of the window for read / write
////////////////////////////////////////////////////////////
View& RenderTarget::GetDefaultView()
{
    return myDefaultView;
}


////////////////////////////////////////////////////////////
/// Tell SFML to preserve external OpenGL states, at the expense of
/// more CPU charge. Use this function if you don't want SFML
/// to mess up your own OpenGL states (if any).
/// Don't enable state preservation if not needed, as it will allow
/// SFML to do internal optimizations and improve performances.
/// This parameter is false by default
////////////////////////////////////////////////////////////
void RenderTarget::PreserveOpenGLStates(bool Preserve)
{
    myPreserveStates = Preserve;
}


////////////////////////////////////////////////////////////
/// Called by the derived class when it's ready to be initialized
////////////////////////////////////////////////////////////
void RenderTarget::Initialize()
{
    // Set the default rendering states
    SetRenderStates();

    // Setup the default view
    myDefaultView.SetFromRect(FloatRect(0, 0, static_cast<float>(GetWidth()), static_cast<float>(GetHeight())));
    SetView(myDefaultView);
}


////////////////////////////////////////////////////////////
/// Set the OpenGL render states needed for the SFML rendering
////////////////////////////////////////////////////////////
void RenderTarget::SetRenderStates()
{
    GLCheck(glDisable(GL_ALPHA_TEST));
    GLCheck(glDisable(GL_DEPTH_TEST));
    GLCheck(glDisable(GL_LIGHTING)); 
}

} // namespace sf
