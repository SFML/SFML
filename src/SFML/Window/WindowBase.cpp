////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007-2022 Laurent Gomila (laurent@sfml-dev.org)
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
#include <SFML/Window/WindowBase.hpp>
#include <SFML/Window/ContextSettings.hpp>
#include <SFML/Window/WindowImpl.hpp>
#include <SFML/System/Err.hpp>


namespace
{
    // A nested named namespace is used here to allow unity builds of SFML.
    namespace WindowsBaseImpl
    {
        const sf::WindowBase* fullscreenWindow = nullptr;
    }
}


namespace sf
{
////////////////////////////////////////////////////////////
WindowBase::WindowBase() :
m_impl(),
m_size(0, 0)
{

}


////////////////////////////////////////////////////////////
WindowBase::WindowBase(VideoMode mode, const String& title, Uint32 style) :
m_impl(),
m_size(0, 0)
{
    WindowBase::create(mode, title, style);
}


////////////////////////////////////////////////////////////
WindowBase::WindowBase(WindowHandle handle) :
m_impl(),
m_size(0, 0)
{
    WindowBase::create(handle);
}


////////////////////////////////////////////////////////////
WindowBase::~WindowBase()
{
    close();
}


////////////////////////////////////////////////////////////
void WindowBase::create(VideoMode mode, const String& title, Uint32 style)
{
    // Destroy the previous window implementation
    close();

    // Fullscreen style requires some tests
    if (style & Style::Fullscreen)
    {
        // Make sure there's not already a fullscreen window (only one is allowed)
        if (getFullscreenWindow())
        {
            err() << "Creating two fullscreen windows is not allowed, switching to windowed mode" << std::endl;
            style &= ~static_cast<Uint32>(Style::Fullscreen);
        }
        else
        {
            // Make sure that the chosen video mode is compatible
            if (!mode.isValid())
            {
                err() << "The requested video mode is not available, switching to a valid mode" << std::endl;
                mode = VideoMode::getFullscreenModes()[0];
            }

            // Update the fullscreen window
            setFullscreenWindow(this);
        }
    }

    // Check validity of style according to the underlying platform
    #if defined(SFML_SYSTEM_IOS) || defined(SFML_SYSTEM_ANDROID)
        if (style & Style::Fullscreen)
            style &= ~static_cast<Uint32>(Style::Titlebar);
        else
            style |= Style::Titlebar;
    #else
        if ((style & Style::Close) || (style & Style::Resize))
            style |= Style::Titlebar;
    #endif

    // Recreate the window implementation
    m_impl = priv::WindowImpl::create(mode, title, style, ContextSettings(0, 0, 0, 0, 0, 0xFFFFFFFF, false));

    // Perform common initializations
    initialize();
}


////////////////////////////////////////////////////////////
void WindowBase::create(WindowHandle handle)
{
    // Destroy the previous window implementation
    close();

    // Recreate the window implementation
    m_impl = priv::WindowImpl::create(handle);

    // Perform common initializations
    initialize();
}


////////////////////////////////////////////////////////////
void WindowBase::close()
{
    // Delete the window implementation
    m_impl.reset();

    // Update the fullscreen window
    if (this == getFullscreenWindow())
        setFullscreenWindow(nullptr);
}


////////////////////////////////////////////////////////////
bool WindowBase::isOpen() const
{
    return m_impl != nullptr;
}


////////////////////////////////////////////////////////////
bool WindowBase::pollEvent(Event& event)
{
    if (m_impl && m_impl->popEvent(event, false))
    {
        return filterEvent(event);
    }
    else
    {
        return false;
    }
}


////////////////////////////////////////////////////////////
bool WindowBase::waitEvent(Event& event)
{
    if (m_impl && m_impl->popEvent(event, true))
    {
        return filterEvent(event);
    }
    else
    {
        return false;
    }
}


////////////////////////////////////////////////////////////
Vector2i WindowBase::getPosition() const
{
    return m_impl ? m_impl->getPosition() : Vector2i();
}


////////////////////////////////////////////////////////////
void WindowBase::setPosition(const Vector2i& position)
{
    if (m_impl)
        m_impl->setPosition(position);
}


////////////////////////////////////////////////////////////
Vector2u WindowBase::getSize() const
{
    return m_size;
}


////////////////////////////////////////////////////////////
void WindowBase::setSize(const Vector2u& size)
{
    if (m_impl)
    {
        m_impl->setSize(size);

        // Cache the new size
        m_size.x = size.x;
        m_size.y = size.y;

        // Notify the derived class
        onResize();
    }
}


////////////////////////////////////////////////////////////
void WindowBase::setTitle(const String& title)
{
    if (m_impl)
        m_impl->setTitle(title);
}


////////////////////////////////////////////////////////////
void WindowBase::setIcon(unsigned int width, unsigned int height, const Uint8* pixels)
{
    if (m_impl)
        m_impl->setIcon(width, height, pixels);
}


////////////////////////////////////////////////////////////
void WindowBase::setVisible(bool visible)
{
    if (m_impl)
        m_impl->setVisible(visible);
}


////////////////////////////////////////////////////////////
void WindowBase::setMouseCursorVisible(bool visible)
{
    if (m_impl)
        m_impl->setMouseCursorVisible(visible);
}


////////////////////////////////////////////////////////////
void WindowBase::setMouseCursorGrabbed(bool grabbed)
{
    if (m_impl)
        m_impl->setMouseCursorGrabbed(grabbed);
}


////////////////////////////////////////////////////////////
void WindowBase::setMouseCursor(const Cursor& cursor)
{
    if (m_impl)
        m_impl->setMouseCursor(cursor.getImpl());
}


////////////////////////////////////////////////////////////
void WindowBase::setKeyRepeatEnabled(bool enabled)
{
    if (m_impl)
        m_impl->setKeyRepeatEnabled(enabled);
}


////////////////////////////////////////////////////////////
void WindowBase::setJoystickThreshold(float threshold)
{
    if (m_impl)
        m_impl->setJoystickThreshold(threshold);
}


////////////////////////////////////////////////////////////
void WindowBase::requestFocus()
{
    if (m_impl)
        m_impl->requestFocus();
}


////////////////////////////////////////////////////////////
bool WindowBase::hasFocus() const
{
    return m_impl && m_impl->hasFocus();
}


////////////////////////////////////////////////////////////
WindowHandle WindowBase::getSystemHandle() const
{
    return m_impl ? m_impl->getSystemHandle() : WindowHandle{};
}


////////////////////////////////////////////////////////////
bool WindowBase::createVulkanSurface(const VkInstance& instance, VkSurfaceKHR& surface, const VkAllocationCallbacks* allocator)
{
    return m_impl ? m_impl->createVulkanSurface(instance, surface, allocator) : false;
}


////////////////////////////////////////////////////////////
void WindowBase::onCreate()
{
    // Nothing by default
}


////////////////////////////////////////////////////////////
void WindowBase::onResize()
{
    // Nothing by default
}


////////////////////////////////////////////////////////////
bool WindowBase::filterEvent(const Event& event)
{
    // Notify resize events to the derived class
    if (event.type == Event::Resized)
    {
        // Cache the new size
        m_size.x = event.size.width;
        m_size.y = event.size.height;

        // Notify the derived class
        onResize();
    }

    return true;
}


////////////////////////////////////////////////////////////
void WindowBase::initialize()
{
    // Setup default behaviors (to get a consistent behavior across different implementations)
    setVisible(true);
    setMouseCursorVisible(true);
    setKeyRepeatEnabled(true);

    // Get and cache the initial size of the window
    m_size = m_impl->getSize();

    // Notify the derived class
    onCreate();
}


////////////////////////////////////////////////////////////
const WindowBase* WindowBase::getFullscreenWindow()
{
    return WindowsBaseImpl::fullscreenWindow;
}


////////////////////////////////////////////////////////////
void WindowBase::setFullscreenWindow(const WindowBase* window)
{
    WindowsBaseImpl::fullscreenWindow = window;
}

} // namespace sf
