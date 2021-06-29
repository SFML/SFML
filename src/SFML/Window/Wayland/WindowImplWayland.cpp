////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007-2021 Laurent Gomila (laurent@sfml-dev.org)
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
#include <SFML/Window/Wayland/WindowImplWayland.hpp>
#include <SFML/System/Utf.hpp>
#include <SFML/System/Err.hpp>
#include <SFML/System/Mutex.hpp>
#include <SFML/System/Lock.hpp>
#include <SFML/System/Sleep.hpp>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <libgen.h>
#include <fcntl.h>
#include <algorithm>
#include <vector>
#include <string>
#include <cstring>
#include <wayland-client-protocol.h>
#include <xdg-shell-client-protocol.h>
#include <SFML/Window/EglContext.hpp>
typedef sf::priv::EglContext ContextType;


////////////////////////////////////////////////////////////
// Private data
////////////////////////////////////////////////////////////
namespace
{

}

namespace sf
{
namespace priv
{



void WindowImplWayland::registry_handle_global(
    void * data,
    struct wl_registry *registry,
    uint32_t name,
    char const *interface,
    uint32_t version
) {
    WindowImplWayland *self = static_cast<WindowImplWayland *>(data);
    if (strcmp(interface, "wl_compositor") == 0)
    {
        self->m_compositor = static_cast<struct wl_compositor *>(wl_registry_bind(registry, name, &wl_compositor_interface, 1));
    } else if (strcmp(interface, "xdg_wm_base") == 0)
    {
        self->m_xdg_wm_base = static_cast<struct xdg_wm_base *>(wl_registry_bind(registry, name, &xdg_wm_base_interface, 1));
    } else if (strcmp(interface, "wl_output") == 0)
    {
        self->m_output = static_cast<struct wl_output *>(wl_registry_bind(registry, name, &wl_output_interface, 1));
    }
}

void WindowImplWayland::registry_handle_global_remove(void *data, struct wl_registry *, uint32_t name)
{
}

void WindowImplWayland::handle_xdg_surface_configure(void *data, struct xdg_surface *surface, uint32_t serial)
{
}

void WindowImplWayland::handle_xdg_toplevel_configure(void *data, struct xdg_toplevel *toplevel, int32_t width, int32_t height, struct wl_array *states)
{
    WindowImplWayland *self = static_cast<WindowImplWayland *>(data);
    self->m_size = Vector2u { (unsigned int) width, (unsigned int) height };
}

void WindowImplWayland::handle_xdg_toplevel_close(void *data, struct xdg_toplevel *toplevel)
{

}

////////////////////////////////////////////////////////////
WindowImplWayland::WindowImplWayland(WindowHandle handle)
{
    // Open a connection with the compositor
    m_display = wl_display_connect(NULL);

    struct wl_registry *registry = wl_display_get_registry(m_display);

    const struct wl_registry_listener registry_listener = {
        WindowImplWayland::registry_handle_global,
        WindowImplWayland::registry_handle_global_remove,
    };

    wl_registry_add_listener(registry, &registry_listener, this);

    wl_display_roundtrip(m_display);

    wl_registry_destroy(registry);

    m_surface = wl_compositor_create_surface(m_compositor);

    if (m_xdg_wm_base == NULL)
    {
        err() << "Compositor doesn't support xdg_wm_base" << std::endl;
        std::abort();
    }

    m_xdg_surface = xdg_wm_base_get_xdg_surface(m_xdg_wm_base, m_surface);

    const struct xdg_surface_listener surface_listener = {
        WindowImplWayland::handle_xdg_surface_configure
    };

    xdg_surface_add_listener(m_xdg_surface, &surface_listener, this);

    wl_display_roundtrip(m_display);

    m_xdg_toplevel = xdg_surface_get_toplevel(m_xdg_surface);

    const struct xdg_toplevel_listener toplevel_listener = {
        WindowImplWayland::handle_xdg_toplevel_configure,
        WindowImplWayland::handle_xdg_toplevel_close
    };

    xdg_toplevel_add_listener(m_xdg_toplevel, &toplevel_listener, this);

    wl_display_roundtrip(m_display);

    this->m_visible = true;
}


////////////////////////////////////////////////////////////
WindowImplWayland::WindowImplWayland(VideoMode mode, const String& title, unsigned long style, const ContextSettings& settings)
{

}


////////////////////////////////////////////////////////////
WindowImplWayland::~WindowImplWayland()
{

}


////////////////////////////////////////////////////////////
WindowHandle WindowImplWayland::getSystemHandle() const
{
    return m_display;
}


////////////////////////////////////////////////////////////
void WindowImplWayland::processEvents()
{

}


////////////////////////////////////////////////////////////
Vector2i WindowImplWayland::getPosition() const
{
    // Wayland doesn't supply this information
    return Vector2i(0, 0);
}


////////////////////////////////////////////////////////////
void WindowImplWayland::setPosition(const Vector2i& position)
{
    // Wayland doesn't support this operation
}


////////////////////////////////////////////////////////////
Vector2u WindowImplWayland::getSize() const
{
    return m_size;
}


////////////////////////////////////////////////////////////
void WindowImplWayland::setSize(const Vector2u& size)
{
    xdg_toplevel_set_min_size(m_xdg_toplevel, size.x, size.y);
    xdg_toplevel_set_max_size(m_xdg_toplevel, size.x, size.y);
}


////////////////////////////////////////////////////////////
void WindowImplWayland::setTitle(const String& title)
{
    xdg_toplevel_set_title(m_xdg_toplevel, title.toAnsiString().c_str());
}


////////////////////////////////////////////////////////////
void WindowImplWayland::setIcon(unsigned int width, unsigned int height, const Uint8* pixels)
{
    // Wayland doesn't support this operation
}


////////////////////////////////////////////////////////////
void WindowImplWayland::setVisible(bool visible)
{
    if (visible && !this->m_visible) {

    } else if (!visible && this->m_visible) {
        xdg_toplevel_destroy(this->m_xdg_toplevel);
        xdg_surface_destroy(this->m_xdg_surface);
    }
    this->m_visible = visible;
}


////////////////////////////////////////////////////////////
void WindowImplWayland::setMouseCursorVisible(bool visible)
{

}


////////////////////////////////////////////////////////////
void WindowImplWayland::setMouseCursor(const CursorImpl& cursor)
{

}


////////////////////////////////////////////////////////////
void WindowImplWayland::setMouseCursorGrabbed(bool grabbed)
{

}


////////////////////////////////////////////////////////////
void WindowImplWayland::setKeyRepeatEnabled(bool enabled)
{

}


////////////////////////////////////////////////////////////
void WindowImplWayland::requestFocus()
{

}


////////////////////////////////////////////////////////////
bool WindowImplWayland::hasFocus() const
{
    // Wayland doesn't support this operation
	return true;
}


////////////////////////////////////////////////////////////
void WindowImplWayland::grabFocus()
{

}


////////////////////////////////////////////////////////////
void WindowImplWayland::setVideoMode(const VideoMode& mode)
{
}


////////////////////////////////////////////////////////////
void WindowImplWayland::resetVideoMode()
{

}


////////////////////////////////////////////////////////////
void WindowImplWayland::switchToFullscreen()
{
    xdg_toplevel_set_fullscreen(m_xdg_toplevel, m_output);
}


////////////////////////////////////////////////////////////
void WindowImplWayland::setProtocols()
{

}


////////////////////////////////////////////////////////////
void WindowImplWayland::initialize()
{

}


////////////////////////////////////////////////////////////
void WindowImplWayland::updateLastInputTime(::Time time)
{

}


////////////////////////////////////////////////////////////
void WindowImplWayland::createHiddenCursor()
{

}


////////////////////////////////////////////////////////////
void WindowImplWayland::cleanup()
{

}


////////////////////////////////////////////////////////////
// bool WindowImplWayland::processEvent(XEvent& windowEvent)
// {

// }


////////////////////////////////////////////////////////////
// RROutput WindowImplWayland::getOutputPrimary()
// {

// }


////////////////////////////////////////////////////////////
Vector2i WindowImplWayland::getPrimaryMonitorPosition()
{

}

} // namespace priv

} // namespace sf
