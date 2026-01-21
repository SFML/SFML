////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007-2025 Marco Antognini (antognini.marco@gmail.com),
//                         Laurent Gomila (laurent@sfml-dev.org)
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

#pragma once

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Window/Event.hpp>
#include <SFML/Window/WindowEnums.hpp>
#include <SFML/Window/WindowImpl.hpp>

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"

////////////////////////////////////////////////////////////
/// Predefine OBJ-C classes
////////////////////////////////////////////////////////////
#ifdef __OBJC__

#import <SFML/Window/macOS/WindowImplDelegateProtocol.h>
using WindowImplDelegateRef = id<WindowImplDelegateProtocol, NSObject>;

@class NSOpenGLContext;
using NSOpenGLContextRef = NSOpenGLContext*;

#else // If C++

using unichar = unsigned short; // See NSString.h

using WindowImplDelegateRef = void*;
using NSOpenGLContextRef    = void*;

#endif

namespace sf
{
class String;

namespace priv
{
////////////////////////////////////////////////////////////
class WindowImplCocoa : public WindowImpl
{
public:
    ////////////////////////////////////////////////////////////
    WindowImplCocoa(WindowHandle handle);

    ////////////////////////////////////////////////////////////
    WindowImplCocoa(VideoMode mode, const String& title, std::uint32_t style, State state, const ContextSettings& settings);

    ////////////////////////////////////////////////////////////
    ~WindowImplCocoa() override;

    ////////////////////////////////////////////////////////////
    /// \brief Window Closed Event - called by the cocoa window object
    ///
    /// Send the event to SFML WindowImpl class.
    ///
    ////////////////////////////////////////////////////////////
    void windowClosed();

    ////////////////////////////////////////////////////////////
    /// \brief Window Resized Event - called by the cocoa window object
    ///
    /// Send the event to SFML WindowImpl class.
    ///
    /// \param size new width and height
    ///
    ////////////////////////////////////////////////////////////
    void windowResized(Vector2u size);

    ////////////////////////////////////////////////////////////
    /// \brief Window Lost Focus Event - called by the cocoa window object
    ///
    /// Send the event to SFML WindowImpl class.
    ///
    ////////////////////////////////////////////////////////////
    void windowFocusLost();

    ////////////////////////////////////////////////////////////
    /// \brief Window Get Focus Event - called by the cocoa window object
    ///
    /// Send the event to SFML WindowImpl class.
    ///
    ////////////////////////////////////////////////////////////
    void windowFocusGained();

    ////////////////////////////////////////////////////////////
    /// \brief Mouse Down Event - called by the cocoa view object
    ///
    /// Send the event to SFML WindowImpl class.
    ///
    /// \param button   active button
    /// \param position mouse x and y position
    ///
    ////////////////////////////////////////////////////////////
    void mouseDownAt(Mouse::Button button, Vector2i position);

    ////////////////////////////////////////////////////////////
    /// \brief Mouse Up Event - called by the cocoa view object
    ///
    /// Send the event to SFML WindowImpl class.
    ///
    /// \param button   active button
    /// \param position mouse x and y position
    ///
    ////////////////////////////////////////////////////////////
    void mouseUpAt(Mouse::Button button, Vector2i position);

    ////////////////////////////////////////////////////////////
    /// \brief Mouse Moved Event - called by the cocoa view object
    ///
    /// Send the event to SFML WindowImpl class.
    ///
    /// \param position mouse x and y position
    ///
    ////////////////////////////////////////////////////////////
    void mouseMovedAt(Vector2i position);

    ////////////////////////////////////////////////////////////
    /// \brief Mouse Wheel Scrolled Event - called by the cocoa view object
    ///
    /// Send the event to SFML WindowImpl class.
    ///
    /// \param deltaX   horizontal scrolling delta
    /// \param deltaY   vertical scrolling delta
    /// \param position mouse x and y position
    ///
    ////////////////////////////////////////////////////////////
    void mouseWheelScrolledAt(float deltaX, float deltaY, Vector2i position);

    ////////////////////////////////////////////////////////////
    /// \brief Mouse In Event - called by the cocoa view object
    ///
    /// Send the event to SFML WindowImpl class.
    ///
    ////////////////////////////////////////////////////////////
    void mouseMovedIn();

    ////////////////////////////////////////////////////////////
    /// \brief Mouse Out Event - called by the cocoa view object
    ///
    /// Send the event to SFML WindowImpl class.
    ///
    ////////////////////////////////////////////////////////////
    void mouseMovedOut();

    ////////////////////////////////////////////////////////////
    /// \brief Key Down Event - called by the cocoa view object
    ///
    /// Send the event to SFML WindowImpl class.
    ///
    /// \param event active key
    ///
    ////////////////////////////////////////////////////////////
    void keyDown(Event::KeyPressed event);

    ////////////////////////////////////////////////////////////
    /// \brief Key Up Event - called by the cocoa view object
    ///
    /// Send the event to SFML WindowImpl class.
    ///
    /// \param event active key
    ///
    ////////////////////////////////////////////////////////////
    void keyUp(Event::KeyReleased event);

    ////////////////////////////////////////////////////////////
    /// \brief Text Entred Event - called by the cocoa view object
    ///
    /// Send the event to SFML WindowImpl class.
    ///
    /// \param charcode Unicode input
    ///
    ////////////////////////////////////////////////////////////
    void textEntered(unichar charcode);

    ////////////////////////////////////////////////////////////
    /// \brief Apply the context to the view
    ///
    /// Called by the SFML context object to finalize its creation.
    ///
    /// \param context The context to bind to the window
    ///
    ////////////////////////////////////////////////////////////
    void applyContext(NSOpenGLContextRef context) const;

    ////////////////////////////////////////////////////////////
    /// \brief Change the type of the current process
    ///
    /// The type of the process is changed to become a full GUI app.
    /// Also ensure NSApp is constructed.
    ///
    ////////////////////////////////////////////////////////////
    static void setUpProcess();

    ////////////////////////////////////////////////////////////
    [[nodiscard]] WindowHandle getNativeHandle() const override;

    ////////////////////////////////////////////////////////////
    [[nodiscard]] Vector2i getPosition() const override;

    ////////////////////////////////////////////////////////////
    void setPosition(Vector2i position) override;

    ////////////////////////////////////////////////////////////
    [[nodiscard]] Vector2u getSize() const override;

    ////////////////////////////////////////////////////////////
    void setSize(Vector2u size) override;

    ////////////////////////////////////////////////////////////
    void setMinimumSize(const std::optional<Vector2u>& minimumSize) override;

    ////////////////////////////////////////////////////////////
    void setMaximumSize(const std::optional<Vector2u>& maximumSize) override;

    ////////////////////////////////////////////////////////////
    void setTitle(const String& title) override;

    ////////////////////////////////////////////////////////////
    void setIcon(Vector2u size, const std::uint8_t* pixels) override;

    ////////////////////////////////////////////////////////////
    void setVisible(bool visible) override;

    ////////////////////////////////////////////////////////////
    void setMouseCursorVisible(bool visible) override;

    ////////////////////////////////////////////////////////////
    void setMouseCursorGrabbed(bool grabbed) override;

    ////////////////////////////////////////////////////////////
    void setMouseCursor(const CursorImpl& cursor) override;

    ////////////////////////////////////////////////////////////
    void setKeyRepeatEnabled(bool enabled) override;

    ////////////////////////////////////////////////////////////
    void requestFocus() override;

    ////////////////////////////////////////////////////////////
    [[nodiscard]] bool hasFocus() const override;

protected:
    ////////////////////////////////////////////////////////////
    void processEvents() override;

private:
    ////////////////////////////////////////////////////////////
    // Member data
    ////////////////////////////////////////////////////////////
    WindowImplDelegateRef m_delegate;         ///< Implementation in Obj-C.
    bool                  m_showCursor{true}; ///< Is the cursor displayed or hidden?
};

} // namespace priv

} // namespace sf

#pragma GCC diagnostic pop
