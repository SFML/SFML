////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007-2013 Laurent Gomila (laurent.gom@gmail.com)
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
#include <SFML/Window/iOS/WindowImplUIKit.hpp>
#include <SFML/Window/iOS/SFView.hpp>
#include <SFML/Window/iOS/SFViewController.hpp>
#include <SFML/Window/iOS/SFAppDelegate.hpp>
#include <SFML/Window/WindowStyle.hpp>
#include <SFML/System/Err.hpp>
#include <UIKit/UIKit.h>


namespace sf
{
namespace priv
{
////////////////////////////////////////////////////////////
WindowImplUIKit::WindowImplUIKit(WindowHandle handle)
{
    // Not implemented
}


////////////////////////////////////////////////////////////
WindowImplUIKit::WindowImplUIKit(VideoMode mode,
                                 const String& title,
                                 unsigned long style,
                                 const ContextSettings& /*settings*/)
{
    // Apply the fullscreen flag
    [UIApplication sharedApplication].statusBarHidden = !(style & Style::Titlebar) || (style & Style::Fullscreen);

    // Set the orientation according to the requested size
    if (mode.width > mode.height)
        [[UIApplication sharedApplication] setStatusBarOrientation:UIInterfaceOrientationLandscapeLeft];
    else
        [[UIApplication sharedApplication] setStatusBarOrientation:UIInterfaceOrientationPortrait];

    // Create the window
    CGRect frame = [UIScreen mainScreen].bounds; // Ignore user size, it wouldn't make sense to use something else
    m_window = [[UIWindow alloc] initWithFrame:frame];
    m_hasFocus = true;

    // Assign it to the application delegate
    [SFAppDelegate getInstance].sfWindow = this;

    // Create the view
    m_view = [[SFView alloc] initWithFrame:frame];
    [m_view resignFirstResponder];

    // Create the view controller
    m_viewController = [SFViewController alloc];
    m_viewController.view = m_view;
    m_viewController.orientationCanChange = style & Style::Resize;
    m_window.rootViewController = m_viewController;

    // Make it the current window
    [m_window makeKeyAndVisible];
}


////////////////////////////////////////////////////////////
WindowImplUIKit::~WindowImplUIKit()
{
}


////////////////////////////////////////////////////////////
void WindowImplUIKit::processEvents()
{
    while (CFRunLoopRunInMode(kCFRunLoopDefaultMode, 0.0001, true) == kCFRunLoopRunHandledSource)
        ;
}


////////////////////////////////////////////////////////////
WindowHandle WindowImplUIKit::getSystemHandle() const
{
    return (__bridge WindowHandle)m_window;
}


////////////////////////////////////////////////////////////
Vector2i WindowImplUIKit::getPosition() const
{
    return Vector2i(m_window.frame.origin.x, m_window.frame.origin.y);
}


////////////////////////////////////////////////////////////
void WindowImplUIKit::setPosition(const Vector2i& position)
{
}


////////////////////////////////////////////////////////////
Vector2u WindowImplUIKit::getSize() const
{
    return Vector2u(m_window.frame.size.width, m_window.frame.size.height);
}


////////////////////////////////////////////////////////////
void WindowImplUIKit::setSize(const Vector2u& size)
{
    // @todo ...

    // Set the orientation according to the requested size
    if (size.x > size.y)
        [[UIApplication sharedApplication] setStatusBarOrientation:UIInterfaceOrientationLandscapeLeft];
    else
        [[UIApplication sharedApplication] setStatusBarOrientation:UIInterfaceOrientationPortrait];
}


////////////////////////////////////////////////////////////
void WindowImplUIKit::setTitle(const String& title)
{
    // Not applicable
}


////////////////////////////////////////////////////////////
void WindowImplUIKit::setIcon(unsigned int width, unsigned int height, const Uint8* pixels)
{
    // Not applicable
}


////////////////////////////////////////////////////////////
void WindowImplUIKit::setVisible(bool visible)
{
    // Not applicable
}


////////////////////////////////////////////////////////////
void WindowImplUIKit::setMouseCursorVisible(bool visible)
{
    // Not applicable
}


////////////////////////////////////////////////////////////
void WindowImplUIKit::setKeyRepeatEnabled(bool enabled)
{
    // Not applicable
}


////////////////////////////////////////////////////////////
void WindowImplUIKit::requestFocus()
{
    // To implement
}


////////////////////////////////////////////////////////////
bool WindowImplUIKit::hasFocus() const
{
    return m_hasFocus;
}


////////////////////////////////////////////////////////////
void WindowImplUIKit::forwardEvent(Event event)
{
    if (event.type == Event::GainedFocus)
        m_hasFocus = true;
    else if (event.type == Event::LostFocus)
        m_hasFocus = false;

    pushEvent(event);
}


////////////////////////////////////////////////////////////
SFView* WindowImplUIKit::getGlView() const
{
    return m_view;
}


////////////////////////////////////////////////////////////
void WindowImplUIKit::setVirtualKeyboardVisible(bool visible)
{
    if (visible)
        [m_view becomeFirstResponder];
    else
        [m_view resignFirstResponder];
}

} // namespace priv

} // namespace sf
