////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007-2012 Marco Antognini (antognini.marco@gmail.com), 
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
#include <SFML/Window/OSX/WindowImplCocoa.hpp>
#include <SFML/System/Err.hpp>
#include <SFML/System/String.hpp>

#import <SFML/Window/OSX/SFWindowController.h>
#import <SFML/Window/OSX/SFViewController.h>
#import <SFML/Window/OSX/cpp_objc_conversion.h>
#import <SFML/Window/OSX/AutoreleasePoolWrapper.h>
#import <SFML/Window/OSX/SFApplication.h>

namespace sf
{
namespace priv
{

#pragma mark
#pragma mark WindowImplCocoa's ctor/dtor

////////////////////////////////////////////////////////////
WindowImplCocoa::WindowImplCocoa(WindowHandle handle)
: m_showCursor(true)
{
    // Ask for a pool.
    retainPool();
    
    // Treat the handle as it real type
    id nsHandle = (id)handle;
    if ([nsHandle isKindOfClass:[NSWindow class]]) {
        
        // We have a window.
        m_delegate = [[SFWindowController alloc] initWithWindow:nsHandle];
    
    } else if ([nsHandle isKindOfClass:[NSView class]]) {
        
        // We have a view.
        m_delegate = [[SFViewController alloc] initWithView:nsHandle];
        
    } else {
        
        sf::err()
            << "Cannot import this Window Handle because it is neither "
            << "a <NSWindow*> nor <NSView*> object "
            << "(or any of their subclasses). You gave a <" 
            << [[nsHandle className] UTF8String]
            << "> object."
            << std::endl;
        return;            
        
    }
     
    [m_delegate setRequesterTo:this];
}
    
    
////////////////////////////////////////////////////////////
WindowImplCocoa::WindowImplCocoa(VideoMode mode,
                                 const String& title,
                                 unsigned long style,
                                 const ContextSettings& /*settings*/)
: m_showCursor(true)
{
    // Transform the app process.
    setUpProcess();
    
    // Ask for a pool.
    retainPool();
    
    m_delegate = [[SFWindowController alloc] initWithMode:mode andStyle:style];
    [m_delegate changeTitle:sfStringToNSString(title)];
    [m_delegate setRequesterTo:this];
}

    
////////////////////////////////////////////////////////////
WindowImplCocoa::~WindowImplCocoa()
{
    [m_delegate closeWindow];
    
    [m_delegate release];
    
    releasePool();
    
    drainPool(); // Make sure everything was freed
    // This solve some issue when sf::Window::Create is called for the
    // second time (nothing was render until the function was called again)
}
    
    
////////////////////////////////////////////////////////////
void WindowImplCocoa::applyContext(NSOpenGLContextRef context) const
{
    [m_delegate applyContext:context];
}


////////////////////////////////////////////////////////////
void WindowImplCocoa::setUpProcess(void)
{
    static bool isTheProcessSetAsApplication = false;
    
    if (!isTheProcessSetAsApplication) {
        // Do it only once !
        isTheProcessSetAsApplication = true;
        
        // Set the process as a normal application so it can get focus.
        ProcessSerialNumber psn;
        if (!GetCurrentProcess(&psn)) {
            TransformProcessType(&psn, kProcessTransformToForegroundApplication);
            SetFrontProcess(&psn);
        }
        
        // Tell the application to stop bouncing in the Dock.
        [[SFApplication sharedApplication] finishLaunching];
        // NOTE : This last call won't harm anything even if SFML window was
        // created with an external handle.
    }
}


#pragma mark
#pragma mark WindowImplCocoa's window-event methods
    
    
////////////////////////////////////////////////////////////
void WindowImplCocoa::windowClosed(void)
{            
    Event event;
    event.type = Event::Closed;
    
    pushEvent(event);
}
    
    
////////////////////////////////////////////////////////////
void WindowImplCocoa::windowResized(unsigned int width, unsigned int height)
{
    Event event;
    event.type = Event::Resized;
    event.size.width  = width;
    event.size.height = height;
    
    pushEvent(event);
}

    
////////////////////////////////////////////////////////////
void WindowImplCocoa::windowLostFocus(void)
{
    if (!m_showCursor) {
        [m_delegate showMouseCursor]; // Make sur the cursor is visible
    }
    
    Event event;
    event.type = Event::LostFocus;
    
    pushEvent(event);
}

    
////////////////////////////////////////////////////////////
void WindowImplCocoa::windowGainedFocus(void)
{
    if (!m_showCursor) {
        [m_delegate hideMouseCursor]; // Restore user's setting
    }
    
    Event event;
    event.type = Event::GainedFocus;
    
    pushEvent(event);
}
    
#pragma mark
#pragma mark WindowImplCocoa's mouse-event methods
    
    
////////////////////////////////////////////////////////////
void WindowImplCocoa::mouseDownAt(Mouse::Button button, int x, int y)
{
    Event event;
    event.type = Event::MouseButtonPressed;
    event.mouseButton.button = button;
    event.mouseButton.x = x;
    event.mouseButton.y = y;
    
    pushEvent(event);
}

    
////////////////////////////////////////////////////////////
void WindowImplCocoa::mouseUpAt(Mouse::Button button, int x, int y)
{
    Event event;
    event.type = Event::MouseButtonReleased;
    event.mouseButton.button = button;
    event.mouseButton.x = x;
    event.mouseButton.y = y;
    
    pushEvent(event);
}


////////////////////////////////////////////////////////////
void WindowImplCocoa::mouseMovedAt(int x, int y)
{
    Event event;
    event.type = Event::MouseMoved;
    event.mouseMove.x = x;
    event.mouseMove.y = y;
    
    pushEvent(event);
}

////////////////////////////////////////////////////////////
void WindowImplCocoa::mouseWheelScrolledAt(float delta, int x, int y)
{
    Event event;
    event.type = Event::MouseWheelMoved;
    event.mouseWheel.delta = delta;
    event.mouseWheel.x = x;
    event.mouseWheel.y = y;
    
    pushEvent(event);
}

////////////////////////////////////////////////////////////
void WindowImplCocoa::mouseMovedIn(void)
{
    if (!m_showCursor) {
        [m_delegate hideMouseCursor]; // Restore user's setting
    }
    
    Event event;
    event.type = Event::MouseEntered;
    
    pushEvent(event);
}
    
////////////////////////////////////////////////////////////
void WindowImplCocoa::mouseMovedOut(void)
{
    if (!m_showCursor) {
        [m_delegate showMouseCursor]; // Make sur the cursor is visible
    }
    
    Event event;
    event.type = Event::MouseLeft;
    
    pushEvent(event);
}
    
    
#pragma mark
#pragma mark WindowImplCocoa's key-event methods
    
    
////////////////////////////////////////////////////////////
void WindowImplCocoa::keyDown(Event::KeyEvent key)
{
    Event event;
    event.type = Event::KeyPressed;
    event.key = key;
    
    pushEvent(event);
}

    
////////////////////////////////////////////////////////////
void WindowImplCocoa::keyUp(Event::KeyEvent key)
{
    Event event;
    event.type = Event::KeyReleased;
    event.key = key;
    
    pushEvent(event);
}


////////////////////////////////////////////////////////////
void WindowImplCocoa::textEntered(unichar charcode)
{
    Event event;
    event.type = Event::TextEntered;
    event.text.unicode = charcode;
    
    pushEvent(event);
}

    
#pragma mark
#pragma mark WindowImplCocoa's event-related methods

////////////////////////////////////////////////////////////
void WindowImplCocoa::processEvents()
{
    [m_delegate processEvent];
}
    
#pragma mark
#pragma mark WindowImplCocoa's private methods

////////////////////////////////////////////////////////////
WindowHandle WindowImplCocoa::getSystemHandle() const
{
    return [m_delegate getSystemHandle];
}

    
////////////////////////////////////////////////////////////
Vector2i WindowImplCocoa::getPosition() const
{
    NSPoint pos = [m_delegate position];
    return Vector2i(pos.x, pos.y);
}


////////////////////////////////////////////////////////////
void WindowImplCocoa::setPosition(const Vector2i& position)
{
    [m_delegate setWindowPositionToX:position.x Y:position.y];
}


////////////////////////////////////////////////////////////
Vector2u WindowImplCocoa::getSize() const
{
    NSSize size = [m_delegate size];
    return Vector2u(size.width, size.height);
}

    
////////////////////////////////////////////////////////////
void WindowImplCocoa::setSize(const Vector2u& size)
{    
    [m_delegate resizeTo:size.x by:size.y];
}

    
////////////////////////////////////////////////////////////
void WindowImplCocoa::setTitle(const String& title)
{
    [m_delegate changeTitle:sfStringToNSString(title)];
}

    
////////////////////////////////////////////////////////////
void WindowImplCocoa::setIcon(unsigned int width, unsigned int height, const Uint8* pixels)
{
    [m_delegate setIconTo:width by:height with:pixels];
}

    
////////////////////////////////////////////////////////////
void WindowImplCocoa::setVisible(bool visible)
{
    if (visible) {
        [m_delegate showWindow];
    } else {
        [m_delegate hideWindow];
    }
}

    
////////////////////////////////////////////////////////////
void WindowImplCocoa::setMouseCursorVisible(bool visible)
{
    m_showCursor = visible;
    
    if (m_showCursor) {
        [m_delegate showMouseCursor];
    } else {
        [m_delegate hideMouseCursor];
    }
}

    
////////////////////////////////////////////////////////////
void WindowImplCocoa::setKeyRepeatEnabled(bool enabled)
{
    if (enabled) {
        [m_delegate enableKeyRepeat];
    } else {
        [m_delegate disableKeyRepeat];
    }
}

    
} // namespace priv
    
} // namespace sf
