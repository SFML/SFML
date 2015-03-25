////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007-2015 Laurent Gomila (laurent@sfml-dev.org)
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
#include <SFML/Window/WindowStyle.hpp> // important to be included first (conflict with None)
#include <SFML/Window/Unix/WindowImplX11.hpp>
#include <SFML/Window/Unix/Display.hpp>
#include <SFML/Window/Unix/ScopedXcbPtr.hpp>
#include <SFML/System/Utf.hpp>
#include <SFML/System/Err.hpp>
#include <xcb/xcb_icccm.h>
#include <xcb/xcb_image.h>
#include <xcb/randr.h>
#include <unistd.h>
#include <algorithm>
#include <vector>
#include <string>
#include <cstring>

#ifdef SFML_OPENGL_ES
    #include <SFML/Window/EglContext.hpp>
    typedef sf::priv::EglContext ContextType;
#else
    #include <SFML/Window/Unix/GlxContext.hpp>
    typedef sf::priv::GlxContext ContextType;
#endif

////////////////////////////////////////////////////////////
// Private data
////////////////////////////////////////////////////////////
namespace
{
    sf::priv::WindowImplX11*              fullscreenWindow = NULL;
    std::vector<sf::priv::WindowImplX11*> allWindows;
    unsigned long                         eventMask = XCB_EVENT_MASK_FOCUS_CHANGE | XCB_EVENT_MASK_BUTTON_PRESS |
                                                      XCB_EVENT_MASK_BUTTON_RELEASE | XCB_EVENT_MASK_BUTTON_MOTION |
                                                      XCB_EVENT_MASK_POINTER_MOTION | XCB_EVENT_MASK_KEY_PRESS |
                                                      XCB_EVENT_MASK_KEY_RELEASE | XCB_EVENT_MASK_STRUCTURE_NOTIFY |
                                                      XCB_EVENT_MASK_ENTER_WINDOW | XCB_EVENT_MASK_LEAVE_WINDOW;

    // Find the name of the current executable
    std::string findExecutableName()
    {
        // We use /proc/self/cmdline to get the command line
        // the user used to invoke this instance of the application
        int file = ::open("/proc/self/cmdline", O_RDONLY | O_NONBLOCK);

        if (file < 0)
            return "sfml";

        std::vector<char> buffer(256, 0);
        std::size_t offset = 0;
        ssize_t result = 0;

        while ((result = read(file, &buffer[offset], 256)) > 0)
        {
            buffer.resize(buffer.size() + result, 0);
            offset += result;
        }

        ::close(file);

        if (offset)
        {
            buffer[offset] = 0;

            // Remove the path to keep the executable name only
            return basename(&buffer[0]);
        }

        // Default fallback name
        return "sfml";
    }

    // Check if Extended Window Manager Hints are supported
    bool ewmhSupported()
    {
        static bool checked = false;
        static bool ewmhSupported = false;

        if (checked)
            return ewmhSupported;

        checked = true;

        xcb_connection_t* connection = sf::priv::OpenConnection();
        xcb_ewmh_connection_t ewmhConnection;

        sf::priv::ScopedXcbPtr<xcb_generic_error_t> error(NULL);

        uint8_t result = xcb_ewmh_init_atoms_replies(
            &ewmhConnection,
            xcb_ewmh_init_atoms(
                connection,
                &ewmhConnection
            ),
            &error
        );

        if (!result || error)
        {
            xcb_ewmh_connection_wipe(&ewmhConnection);
            sf::priv::CloseConnection(connection);
            return false;
        }

        xcb_window_t rootWindow;

        result = xcb_ewmh_get_supporting_wm_check_reply(
            &ewmhConnection,
            xcb_ewmh_get_supporting_wm_check(
                &ewmhConnection,
                sf::priv::XCBDefaultRootWindow(connection)
            ),
            &rootWindow,
            &error
        );

        if (!result || error)
        {
            xcb_ewmh_connection_wipe(&ewmhConnection);
            sf::priv::CloseConnection(connection);
            return false;
        }

        xcb_window_t childWindow;

        result = xcb_ewmh_get_supporting_wm_check_reply(
            &ewmhConnection,
            xcb_ewmh_get_supporting_wm_check(
                &ewmhConnection,
                rootWindow
            ),
            &childWindow,
            &error
        );

        xcb_ewmh_connection_wipe(&ewmhConnection);
        sf::priv::CloseConnection(connection);

        if (!result || error)
            return false;

        // Conforming window managers should return the same window for both queries
        if (rootWindow != childWindow)
            return false;

        ewmhSupported = true;
        return true;
    }
}


namespace sf
{
namespace priv
{
////////////////////////////////////////////////////////////
WindowImplX11::WindowImplX11(WindowHandle handle) :
m_window         (0),
m_inputMethod    (NULL),
m_inputContext   (NULL),
m_isExternal     (true),
m_atomWmProtocols(0),
m_atomClose      (0),
m_oldVideoMode   (-1),
m_hiddenCursor   (0),
m_keyRepeat      (true),
m_previousSize   (-1, -1),
m_useSizeHints   (false),
m_fullscreen     (false)
{
    // Open a connection with the X server
    m_display = OpenDisplay();
    m_connection = XGetXCBConnection(m_display);

    if (!m_connection)
    {
        err() << "Failed cast Display object to an XCB connection object" << std::endl;
        return;
    }

    // Make sure to check for EWMH support before we do anything
    if (ewmhSupported())
    {
        ScopedXcbPtr<xcb_generic_error_t> error(NULL);

        uint8_t result = xcb_ewmh_init_atoms_replies(
            &m_ewmhConnection,
            xcb_ewmh_init_atoms(
                m_connection,
                &m_ewmhConnection
            ),
            &error
        );

        if (!result || error)
            err() << "Failed to initialize EWMH atoms" << std::endl;
    }

    m_screen = XCBDefaultScreen(m_connection);
    XSetEventQueueOwner(m_display, XCBOwnsEventQueue);

    // Save the window handle
    m_window = handle;

    if (m_window)
    {
        // Make sure the window is listening to all the required events
        const uint32_t value_list[] = {static_cast<uint32_t>(eventMask)};

        xcb_change_window_attributes(
            m_connection,
            m_window,
            XCB_CW_EVENT_MASK,
            value_list
        );

        // Set the WM protocols
        setProtocols();

        // Do some common initializations
        initialize();
    }
}


////////////////////////////////////////////////////////////
WindowImplX11::WindowImplX11(VideoMode mode, const String& title, unsigned long style, const ContextSettings& settings) :
m_window         (0),
m_inputMethod    (NULL),
m_inputContext   (NULL),
m_isExternal     (false),
m_atomWmProtocols(0),
m_atomClose      (0),
m_oldVideoMode   (-1),
m_hiddenCursor   (0),
m_keyRepeat      (true),
m_previousSize   (-1, -1),
m_useSizeHints   (false),
m_fullscreen     ((style & Style::Fullscreen) != 0)
{
    // Open a connection with the X server
    m_display = OpenDisplay();
    m_connection = XGetXCBConnection(m_display);

    if (!m_connection)
    {
        err() << "Failed cast Display object to an XCB connection object" << std::endl;
        return;
    }

    // Make sure to check for EWMH support before we do anything
    if (ewmhSupported())
    {
        ScopedXcbPtr<xcb_generic_error_t> error(NULL);

        uint8_t result = xcb_ewmh_init_atoms_replies(
            &m_ewmhConnection,
            xcb_ewmh_init_atoms(
                m_connection,
                &m_ewmhConnection
            ),
            &error
        );

        if (!result || error)
            err() << "Failed to initialize EWMH atoms" << std::endl;
    }

    m_screen = XCBDefaultScreen(m_connection);
    XSetEventQueueOwner(m_display, XCBOwnsEventQueue);

    // Compute position and size
    int left = m_fullscreen ? 0 : (m_screen->width_in_pixels  - mode.width) / 2;
    int top = m_fullscreen ? 0 : (m_screen->height_in_pixels - mode.height) / 2;
    int width  = mode.width;
    int height = mode.height;

    // Choose the visual according to the context settings
    XVisualInfo visualInfo = ContextType::selectBestVisual(m_display, mode.bitsPerPixel, settings);

    // Define the window attributes
    xcb_colormap_t colormap = xcb_generate_id(m_connection);
    xcb_create_colormap(m_connection, XCB_COLORMAP_ALLOC_NONE, colormap, m_screen->root, visualInfo.visualid);
    const uint32_t value_list[] = {m_fullscreen && !ewmhSupported(), static_cast<uint32_t>(eventMask), colormap};

    // Create the window
    m_window = xcb_generate_id(m_connection);

    ScopedXcbPtr<xcb_generic_error_t> errptr(xcb_request_check(
        m_connection,
        xcb_create_window_checked(
            m_connection,
            static_cast<uint8_t>(visualInfo.depth),
            m_window,
            m_screen->root,
            left, top,
            width, height,
            0,
            XCB_WINDOW_CLASS_INPUT_OUTPUT,
            visualInfo.visualid,
            XCB_CW_EVENT_MASK | XCB_CW_OVERRIDE_REDIRECT | XCB_CW_COLORMAP,
            value_list
        )
    ));

    if (errptr)
    {
        err() << "Failed to create window" << std::endl;
        return;
    }

    // Set the WM protocols
    setProtocols();

    // Set the WM initial state to the normal state
    xcb_icccm_wm_hints_t hints;
    std::memset(&hints, 0, sizeof(hints));
    xcb_icccm_wm_hints_set_normal(&hints);
    xcb_icccm_set_wm_hints(m_connection, m_window, &hints);

    xcb_size_hints_t sizeHints;
    std::memset(&sizeHints, 0, sizeof(sizeHints));

    // Set the window's style (tell the window manager to change our window's decorations and functions according to the requested style)
    if (!m_fullscreen)
    {
        // Set the Motif WM hints
        setMotifHints(style);

        // This is a hack to force some windows managers to disable resizing
        if (!(style & Style::Resize))
        {
            m_useSizeHints = true;
            xcb_icccm_size_hints_set_min_size(&sizeHints, width, height);
            xcb_icccm_size_hints_set_max_size(&sizeHints, width, height);
        }
    }

    // Set the WM hints of the normal state
    xcb_icccm_set_wm_normal_hints(m_connection, m_window, &sizeHints);

    // Set the window's WM class (this can be used by window managers)
    // The WM_CLASS property actually consists of 2 parts,
    // the instance name and the class name both of which should be
    // null terminated strings.
    // The instance name should be something unique to this invokation
    // of the application but is rarely if ever used these days.
    // For simplicity, we retrieve it via the base executable name.
    // The class name identifies a class of windows that
    // "are of the same type". We simply use the initial window name as
    // the class name.
    std::string windowClass = findExecutableName();
    windowClass += '\0'; // Important to separate instance from class
    windowClass += title.toAnsiString();

    // We add 1 to the size of the string to include the null at the end
    xcb_icccm_set_wm_class_checked(m_connection, m_window, windowClass.size() + 1, windowClass.c_str());

    // Set the window's name
    setTitle(title);

    // Do some common initializations
    initialize();

    // Set fullscreen video mode and switch to fullscreen if necessary
    if (m_fullscreen)
    {
        setPosition(Vector2i(0, 0));
        setVideoMode(mode);
        switchToFullscreen();
    }
}


////////////////////////////////////////////////////////////
WindowImplX11::~WindowImplX11()
{
    // Cleanup graphical resources
    cleanup();

    // Destroy the cursor
    if (m_hiddenCursor)
        xcb_free_cursor(m_connection, m_hiddenCursor);

    // Destroy the input context
    if (m_inputContext)
        XDestroyIC(m_inputContext);

    // Destroy the window
    if (m_window && !m_isExternal)
    {
        xcb_destroy_window(m_connection, m_window);
        xcb_flush(m_connection);
    }

    // Close the input method
    if (m_inputMethod)
        XCloseIM(m_inputMethod);

    // Clean up the EWMH connection
    xcb_ewmh_connection_wipe(&m_ewmhConnection);

    // Close the connection with the X server
    CloseDisplay(m_display);

    // Remove this window from the global list of windows (required for focus request)
    allWindows.erase(std::find(allWindows.begin(), allWindows.end(), this));
}


////////////////////////////////////////////////////////////
WindowHandle WindowImplX11::getSystemHandle() const
{
    return m_window;
}


////////////////////////////////////////////////////////////
void WindowImplX11::processEvents()
{
    // Key repeat workaround: If key repeat is enabled, XCB will spawn two
    // events for each repeat interval: key release and key press. Both have
    // the same timestamp and key code. We are holding back the release event
    // to check for the matching key press event and if so, discard the release
    // event.

    xcb_generic_event_t* event = NULL;
    xcb_key_release_event_t* lastKeyReleaseEvent = NULL;
    uint8_t eventType = 0;

    while((event = xcb_poll_for_event(m_connection)))
    {
        eventType = event->response_type & ~0x80;

        // Key was pressed and one has been released prior to that.
        if (eventType == XCB_KEY_PRESS && lastKeyReleaseEvent)
        {
            // If the key press event matches the held back key release event,
            // then we have a key repeat and discard the held back release
            // event.
            if (lastKeyReleaseEvent->time == reinterpret_cast<xcb_key_press_event_t*>(event)->time &&
                lastKeyReleaseEvent->detail == reinterpret_cast<xcb_key_press_event_t*>(event)->detail)
            {
                free(lastKeyReleaseEvent);
                lastKeyReleaseEvent = NULL;
            }
        }

        // If there's still a key release event held back, process it now.
        if (lastKeyReleaseEvent)
        {
            processEvent(reinterpret_cast<xcb_generic_event_t*>(lastKeyReleaseEvent));
            free(lastKeyReleaseEvent);
            lastKeyReleaseEvent = NULL;
        }

        if (eventType == XCB_KEY_RELEASE)
        {
            // Remember this key release event.
            lastKeyReleaseEvent = reinterpret_cast<xcb_key_release_event_t*>(event);
            event = NULL; // For safety reasons.
        }
        else
        {
            processEvent(event);
            free(event);
        }
    }

    // Process any held back release event.
    if (lastKeyReleaseEvent)
    {
        processEvent(reinterpret_cast<xcb_generic_event_t*>(lastKeyReleaseEvent));
        free(lastKeyReleaseEvent);
        lastKeyReleaseEvent = NULL;
    }
}


////////////////////////////////////////////////////////////
Vector2i WindowImplX11::getPosition() const
{
    ::Window topLevelWindow = m_window;
    ::Window nextWindow = topLevelWindow;

    ScopedXcbPtr<xcb_generic_error_t> error(NULL);

    // Get "top level" window, i.e. the window with the root window as its parent.
    while (nextWindow != m_screen->root)
    {
        topLevelWindow = nextWindow;

        ScopedXcbPtr<xcb_query_tree_reply_t> treeReply(xcb_query_tree_reply(
            m_connection,
            xcb_query_tree(
                m_connection,
                topLevelWindow
            ),
            &error
        ));

        if (error)
        {
            err() << "Failed to get window position (query_tree)" << std::endl;
            return Vector2i(0, 0);
        }

        nextWindow = treeReply->parent;
    }

    ScopedXcbPtr<xcb_get_geometry_reply_t> geometryReply(xcb_get_geometry_reply(
        m_connection,
        xcb_get_geometry(
            m_connection,
            topLevelWindow
        ),
        &error
    ));

    if (error)
    {
        err() << "Failed to get window position (get_geometry)" << std::endl;
        return Vector2i(0, 0);
    }

    return Vector2i(geometryReply->x, geometryReply->y);
}


////////////////////////////////////////////////////////////
void WindowImplX11::setPosition(const Vector2i& position)
{
    uint32_t values[] = {static_cast<uint32_t>(position.x), static_cast<uint32_t>(position.y)};
    xcb_configure_window(m_connection, m_window,
                         XCB_CONFIG_WINDOW_X | XCB_CONFIG_WINDOW_Y,
                         values);
    xcb_flush(m_connection);
}


////////////////////////////////////////////////////////////
Vector2u WindowImplX11::getSize() const
{
    ScopedXcbPtr<xcb_get_geometry_reply_t> reply(xcb_get_geometry_reply(m_connection, xcb_get_geometry(m_connection, m_window), NULL));

    return Vector2u(reply->width, reply->height);
}


////////////////////////////////////////////////////////////
void WindowImplX11::setSize(const Vector2u& size)
{
    // If resizing is disable for the window we have to update the size hints (required by some window managers).
    if( m_useSizeHints ) {
        xcb_size_hints_t sizeHints;
        std::memset(&sizeHints, 0, sizeof(sizeHints));
        xcb_icccm_size_hints_set_min_size(&sizeHints, size.x, size.y);
        xcb_icccm_size_hints_set_max_size(&sizeHints, size.x, size.y);

        ScopedXcbPtr<xcb_generic_error_t> configureWindowError(xcb_request_check(
            m_connection,
            xcb_icccm_set_wm_normal_hints(
                m_connection,
                m_window,
                &sizeHints
            )
        ));

        if (configureWindowError)
            err() << "Failed to set window size hints" << std::endl;
    }

    uint32_t values[] = {size.x, size.y};

    ScopedXcbPtr<xcb_generic_error_t> configureWindowError(xcb_request_check(
        m_connection,
        xcb_configure_window(
            m_connection,
            m_window,
            XCB_CONFIG_WINDOW_WIDTH | XCB_CONFIG_WINDOW_HEIGHT,
            values
        )
    ));

    if (configureWindowError)
        err() << "Failed to set window size" << std::endl;

    xcb_flush(m_connection);
}


////////////////////////////////////////////////////////////
void WindowImplX11::setTitle(const String& title)
{
    // XCB takes UTF-8-encoded strings.
    std::string utf8String;
    Utf<32>::toUtf8(title.begin(), title.end(), std::back_inserter(utf8String));

    ScopedXcbPtr<xcb_generic_error_t> wmNameError(xcb_request_check(
        m_connection,
        xcb_icccm_set_wm_name_checked(
            m_connection,
            m_window,
            XCB_ATOM_STRING,
            sizeof(std::basic_string<Uint8>::value_type) * 8,
            utf8String.length(),
            utf8String.c_str()
        )
    ));

    if (wmNameError)
        err() << "Failed to set window title" << std::endl;

    ScopedXcbPtr<xcb_generic_error_t> wmIconNameError(xcb_request_check(
        m_connection,
        xcb_icccm_set_wm_icon_name_checked(
            m_connection,
            m_window,
            XCB_ATOM_STRING,
            sizeof(std::basic_string<Uint8>::value_type) * 8,
            utf8String.length(),
            utf8String.c_str()
        )
    ));

    if (wmIconNameError)
        err() << "Failed to set WM_ICON_NAME property" << std::endl;

    if (ewmhSupported())
    {
        ScopedXcbPtr<xcb_generic_error_t> ewmhNameError(xcb_request_check(
            m_connection,
            xcb_ewmh_set_wm_name_checked(
                &m_ewmhConnection,
                m_window,
                utf8String.length(),
                utf8String.c_str()
            )
        ));

        if (ewmhNameError)
            err() << "Failed to set _NET_WM_NAME property" << std::endl;

        ScopedXcbPtr<xcb_generic_error_t> ewmhIconNameError(xcb_request_check(
            m_connection,
            xcb_ewmh_set_wm_icon_name_checked(
                &m_ewmhConnection,
                m_window,
                utf8String.length(),
                utf8String.c_str()
            )
        ));

        if (ewmhIconNameError)
            err() << "Failed to set _NET_WM_ICON_NAME property" << std::endl;
    }

    xcb_flush(m_connection);
}


////////////////////////////////////////////////////////////
void WindowImplX11::setIcon(unsigned int width, unsigned int height, const Uint8* pixels)
{
    // X11 wants BGRA pixels: swap red and blue channels
    Uint8 iconPixels[width * height * 4];
    for (std::size_t i = 0; i < width * height; ++i)
    {
        iconPixels[i * 4 + 0] = pixels[i * 4 + 2];
        iconPixels[i * 4 + 1] = pixels[i * 4 + 1];
        iconPixels[i * 4 + 2] = pixels[i * 4 + 0];
        iconPixels[i * 4 + 3] = pixels[i * 4 + 3];
    }

    // Create the icon pixmap
    xcb_pixmap_t iconPixmap = xcb_generate_id(m_connection);

    ScopedXcbPtr<xcb_generic_error_t> createPixmapError(xcb_request_check(
        m_connection,
        xcb_create_pixmap_checked(
            m_connection,
            m_screen->root_depth,
            iconPixmap,
            m_screen->root,
            width,
            height
        )
    ));

    if (createPixmapError)
    {
        err() << "Failed to set the window's icon (create_pixmap): ";
        err() << "Error code " << static_cast<int>(createPixmapError->error_code) << std::endl;
        return;
    }

    xcb_gcontext_t iconGC = xcb_generate_id(m_connection);

    ScopedXcbPtr<xcb_generic_error_t> createGcError(xcb_request_check(
        m_connection,
        xcb_create_gc(
            m_connection,
            iconGC,
            iconPixmap,
            0,
            NULL
        )
    ));

    if (createGcError)
    {
        err() << "Failed to set the window's icon (create_gc): ";
        err() << "Error code " << static_cast<int>(createGcError->error_code) << std::endl;
        return;
    }

    ScopedXcbPtr<xcb_generic_error_t> putImageError(xcb_request_check(
        m_connection,
        xcb_put_image_checked(
            m_connection,
            XCB_IMAGE_FORMAT_Z_PIXMAP,
            iconPixmap,
            iconGC,
            width,
            height,
            0,
            0,
            0,
            m_screen->root_depth,
            sizeof(iconPixels),
            iconPixels
        )
    ));

    ScopedXcbPtr<xcb_generic_error_t> freeGcError(xcb_request_check(
        m_connection,
        xcb_free_gc(
            m_connection,
            iconGC
        )
    ));

    if (freeGcError)
    {
        err() << "Failed to free icon GC: ";
        err() << "Error code " << static_cast<int>(freeGcError->error_code) << std::endl;
    }

    if (putImageError)
    {
        err() << "Failed to set the window's icon (put_image): ";
        err() << "Error code " << static_cast<int>(putImageError->error_code) << std::endl;
        return;
    }

    // Create the mask pixmap (must have 1 bit depth)
    std::size_t pitch = (width + 7) / 8;
    static std::vector<Uint8> maskPixels(pitch * height, 0);
    for (std::size_t j = 0; j < height; ++j)
    {
        for (std::size_t i = 0; i < pitch; ++i)
        {
            for (std::size_t k = 0; k < 8; ++k)
            {
                if (i * 8 + k < width)
                {
                    Uint8 opacity = (pixels[(i * 8 + k + j * width) * 4 + 3] > 0) ? 1 : 0;
                    maskPixels[i + j * pitch] |= (opacity << k);
                }
            }
        }
    }

    xcb_pixmap_t maskPixmap = xcb_create_pixmap_from_bitmap_data(
        m_connection,
        m_window,
        reinterpret_cast<uint8_t*>(&maskPixels[0]),
        width,
        height,
        1,
        0,
        1,
        NULL
    );

    // Send our new icon to the window through the WMHints
    xcb_icccm_wm_hints_t hints;
    std::memset(&hints, 0, sizeof(hints));
    xcb_icccm_wm_hints_set_icon_pixmap(&hints, iconPixmap);
    xcb_icccm_wm_hints_set_icon_mask(&hints, maskPixmap);

    ScopedXcbPtr<xcb_generic_error_t> setWmHintsError(xcb_request_check(
        m_connection,
        xcb_icccm_set_wm_hints(
            m_connection,
            m_window,
            &hints
        )
    ));

    if (setWmHintsError)
    {
        err() << "Failed to set the window's icon (icccm_set_wm_hints): ";
        err() << "Error code " << static_cast<int>(setWmHintsError->error_code) << std::endl;
    }

    xcb_flush(m_connection);

    ScopedXcbPtr<xcb_generic_error_t> freePixmapError(xcb_request_check(
        m_connection,
        xcb_free_pixmap_checked(
            m_connection,
            iconPixmap
        )
    ));

    if (freePixmapError)
    {
        err() << "Failed to free icon pixmap: ";
        err() << "Error code " << static_cast<int>(freePixmapError->error_code) << std::endl;
    }
}


////////////////////////////////////////////////////////////
void WindowImplX11::setVisible(bool visible)
{
    if (visible)
    {
        ScopedXcbPtr<xcb_generic_error_t> error(xcb_request_check(
            m_connection,
            xcb_map_window(
                m_connection,
                m_window
            )
        ));

        if (error)
            err() << "Failed to change window visibility" << std::endl;
    }
    else
    {
        ScopedXcbPtr<xcb_generic_error_t> error(xcb_request_check(
            m_connection,
            xcb_unmap_window(
                m_connection,
                m_window
            )
        ));

        if (error)
            err() << "Failed to change window visibility" << std::endl;
    }

    xcb_flush(m_connection);
}


////////////////////////////////////////////////////////////
void WindowImplX11::setMouseCursorVisible(bool visible)
{
    const uint32_t values = visible ? XCB_NONE : m_hiddenCursor;

    ScopedXcbPtr<xcb_generic_error_t> error(xcb_request_check(
        m_connection,
        xcb_change_window_attributes(
            m_connection,
            m_window,
            XCB_CW_CURSOR,
            &values
        )
    ));

    if (error)
        err() << "Failed to change mouse cursor visibility" << std::endl;

    xcb_flush(m_connection);
}


////////////////////////////////////////////////////////////
void WindowImplX11::setKeyRepeatEnabled(bool enabled)
{
    m_keyRepeat = enabled;
}


////////////////////////////////////////////////////////////
void WindowImplX11::requestFocus()
{
    // Focus is only stolen among SFML windows, not between applications
    // Check the global list of windows to find out whether an SFML window has the focus
    // Note: can't handle console and other non-SFML windows belonging to the application.
    bool sfmlWindowFocused = false;
    for (std::vector<WindowImplX11*>::iterator itr = allWindows.begin(); itr != allWindows.end(); ++itr)
    {
        if ((*itr)->hasFocus())
        {
            sfmlWindowFocused = true;
            break;
        }
    }

    // Check if window is viewable (not on other desktop, ...)
    // TODO: Check also if minimized
    ScopedXcbPtr<xcb_get_window_attributes_reply_t> attributes(xcb_get_window_attributes_reply(
        m_connection,
        xcb_get_window_attributes(
            m_connection,
            m_window
        ),
        NULL
    ));

    if (!attributes)
    {
        err() << "Failed to check if window is viewable while requesting focus" << std::endl;
        return; // error getting attribute
    }

    bool windowViewable = (attributes->map_state == XCB_MAP_STATE_VIEWABLE);

    if (sfmlWindowFocused && windowViewable)
    {
        // Another SFML window of this application has the focus and the current window is viewable:
        // steal focus (i.e. bring window to the front and give it input focus)
        grabFocus();
    }
    else
    {
        // Get current WM hints.
        xcb_icccm_wm_hints_t hints;
        std::memset(&hints, 0, sizeof(hints));

        xcb_icccm_get_wm_hints_reply(
            m_connection,
            xcb_icccm_get_wm_hints_unchecked(
                m_connection,
                m_window
            ),
            &hints,
            NULL
        );

        // Even if no hints were returned, we can simply set the proper flags we need and go on. This is
        // different from Xlib where XAllocWMHints() has to be called.
        xcb_icccm_wm_hints_set_urgency(&hints);
        xcb_icccm_set_wm_hints_checked(m_connection, m_window, &hints);
    }
}


////////////////////////////////////////////////////////////
bool WindowImplX11::hasFocus() const
{
    ScopedXcbPtr<xcb_generic_error_t> error(NULL);

    ScopedXcbPtr<xcb_get_input_focus_reply_t> reply(xcb_get_input_focus_reply(
        m_connection,
        xcb_get_input_focus_unchecked(
            m_connection
        ),
        &error
    ));

    if (error)
        err() << "Failed to check if window has focus" << std::endl;

    return (reply->focus == m_window);
}


////////////////////////////////////////////////////////////
void WindowImplX11::grabFocus()
{
    if (ewmhSupported())
    {
        ScopedXcbPtr<xcb_generic_error_t> activeWindowError(xcb_request_check(
            m_connection,
            xcb_ewmh_request_change_active_window(
                &m_ewmhConnection,
                XDefaultScreen(m_display),
                m_window,
                XCB_EWMH_CLIENT_SOURCE_TYPE_NORMAL,
                XCB_CURRENT_TIME,
                None
            )
        ));

        if (activeWindowError)
        {
            err() << "Failed to change active window (request_change_active_window)" << std::endl;
            return;
        }
    }
    else
    {
        ScopedXcbPtr<xcb_generic_error_t> setInputFocusError(xcb_request_check(
            m_connection,
            xcb_set_input_focus(
                m_connection,
                XCB_INPUT_FOCUS_POINTER_ROOT,
                m_window,
                XCB_CURRENT_TIME
            )
        ));

        if (setInputFocusError)
        {
            err() << "Failed to change active window (set_input_focus)" << std::endl;
            return;
        }

        const uint32_t values[] = {XCB_STACK_MODE_ABOVE};

        ScopedXcbPtr<xcb_generic_error_t> configureWindowError(xcb_request_check(
            m_connection,
            xcb_configure_window(
                m_connection,
                m_window,
                XCB_CONFIG_WINDOW_STACK_MODE,
                values
            )
        ));

        if (configureWindowError)
            err() << "Failed to change active window (configure_window)" << std::endl;
    }
}


////////////////////////////////////////////////////////////
void WindowImplX11::setVideoMode(const VideoMode& mode)
{
    // Skip mode switching if the new mode is equal to the desktop mode
    if (mode == VideoMode::getDesktopMode())
        return;

    ScopedXcbPtr<xcb_generic_error_t> error(NULL);

    // Check if the RandR extension is present
    static const std::string RANDR = "RANDR";
    ScopedXcbPtr<xcb_query_extension_reply_t> randr_ext(xcb_query_extension_reply(
        m_connection,
        xcb_query_extension(
            m_connection,
            RANDR.size(),
            RANDR.c_str()
        ),
        &error
    ));

    if (error || !randr_ext->present)
    {
        // RandR extension is not supported: we cannot use fullscreen mode
        err() << "Fullscreen is not supported, switching to window mode" << std::endl;
        return;
    }

    // Load RandR and check its version
    ScopedXcbPtr<xcb_randr_query_version_reply_t> randrVersion(xcb_randr_query_version_reply(
        m_connection,
        xcb_randr_query_version(
            m_connection,
            1,
            1
        ),
        &error
    ));

    if (error)
    {
        err() << "Failed to load RandR, switching to window mode" << std::endl;
        return;
    }

    // Get the current configuration
    ScopedXcbPtr<xcb_randr_get_screen_info_reply_t> config(xcb_randr_get_screen_info_reply(
        m_connection,
        xcb_randr_get_screen_info(
            m_connection,
            m_screen->root
        ),
        &error
    ));

    if (error)
    {
        // Failed to get the screen configuration
        err() << "Failed to get the current screen configuration for fullscreen mode, switching to window mode" << std::endl;
        return;
    }

    // Save the current video mode before we switch to fullscreen
    m_oldVideoMode = config->sizeID;

    // Get the available screen sizes
    xcb_randr_screen_size_t* sizes = xcb_randr_get_screen_info_sizes(config.get());

    if (!sizes || !config->nSizes)
    {
        err() << "Failed to get the fullscreen sizes, switching to window mode" << std::endl;
        return;
    }

    // Search for a matching size
    for (int i = 0; i < config->nSizes; ++i)
    {
        if (config->rotation == XCB_RANDR_ROTATION_ROTATE_90 ||
            config->rotation == XCB_RANDR_ROTATION_ROTATE_270)
            std::swap(sizes[i].height, sizes[i].width);

        if ((sizes[i].width  == static_cast<int>(mode.width)) &&
            (sizes[i].height == static_cast<int>(mode.height)))
        {
            // Switch to fullscreen mode
            ScopedXcbPtr<xcb_randr_set_screen_config_reply_t> setScreenConfig(xcb_randr_set_screen_config_reply(
                m_connection,
                xcb_randr_set_screen_config(
                    m_connection,
                    m_screen->root,
                    XCB_CURRENT_TIME,
                    config->config_timestamp,
                    i,
                    config->rotation,
                    0//config->rate
                ),
                &error
            ));

            if (error)
                err() << "Failed to set new screen configuration" << std::endl;

            // Set "this" as the current fullscreen window
            fullscreenWindow = this;
            return;
        }
    }

    err() << "Failed to find matching fullscreen size, switching to window mode" << std::endl;
}


////////////////////////////////////////////////////////////
void WindowImplX11::resetVideoMode()
{
    if (fullscreenWindow == this)
    {
        // Get current screen info
        ScopedXcbPtr<xcb_generic_error_t> error(NULL);
        ScopedXcbPtr<xcb_randr_get_screen_info_reply_t> config(xcb_randr_get_screen_info_reply(
            m_connection,
            xcb_randr_get_screen_info(
            m_connection,
            m_screen->root
            ),
            &error
        ));

        if (!error)
        {
            // Reset the video mode
            ScopedXcbPtr<xcb_randr_set_screen_config_reply_t> setScreenConfig(xcb_randr_set_screen_config_reply(
                m_connection,
                xcb_randr_set_screen_config(
                    m_connection,
                    m_screen->root,
                    CurrentTime,
                    config->config_timestamp,
                    m_oldVideoMode,
                    config->rotation,
                    config->rate
                ),
                &error
            ));

            if (error)
                err() << "Failed to reset old screen configuration" << std::endl;
        }

        // Reset the fullscreen window
        fullscreenWindow = NULL;
    }
}


////////////////////////////////////////////////////////////
void WindowImplX11::switchToFullscreen()
{
    grabFocus();

    if (ewmhSupported())
    {
        // Create atom for _NET_WM_BYPASS_COMPOSITOR.
        static const std::string netWmStateBypassCompositor = "_NET_WM_BYPASS_COMPOSITOR";
        ScopedXcbPtr<xcb_intern_atom_reply_t> compReply(xcb_intern_atom_reply(
            m_connection,
            xcb_intern_atom(
                m_connection,
                False,
                netWmStateBypassCompositor.size(),
                netWmStateBypassCompositor.c_str()
            ),
            0
        ));

        static const Uint32 bypassCompositor = 1;

        // Disable compositor.
        ScopedXcbPtr<xcb_generic_error_t> compositorError(xcb_request_check(
            m_connection,
            xcb_change_property_checked(
                m_connection,
                XCB_PROP_MODE_REPLACE,
                m_window,
                compReply->atom,
                XCB_ATOM_CARDINAL,
                32,
                1,
                &bypassCompositor
            )
        ));

        // Not being able to bypass the compositor is not a fatal error
        if (compositorError)
            err() << "xcb_change_property failed, unable to set _NET_WM_BYPASS_COMPOSITOR" << std::endl;

        ScopedXcbPtr<xcb_generic_error_t> wmStateError(xcb_request_check(
            m_connection,
            xcb_ewmh_request_change_wm_state(
                &m_ewmhConnection,
                XDefaultScreen(m_display),
                m_window,
                XCB_EWMH_WM_STATE_ADD,
                m_ewmhConnection._NET_WM_STATE_FULLSCREEN,
                0,
                XCB_EWMH_CLIENT_SOURCE_TYPE_NORMAL
            )
        ));

        if (wmStateError)
        {
            err() << "Setting fullscreen failed, could not change WM state" << std::endl;
            return;
        }
    }
}


////////////////////////////////////////////////////////////
void WindowImplX11::setProtocols()
{
    ScopedXcbPtr<xcb_generic_error_t> error(NULL);

    // Get the atom for setting the window protocols we support
    static const std::string WM_PROTOCOLS_NAME = "WM_PROTOCOLS";
    ScopedXcbPtr<xcb_intern_atom_reply_t> protocolsAtomReply(xcb_intern_atom_reply(
        m_connection,
        xcb_intern_atom(
            m_connection,
            0,
            WM_PROTOCOLS_NAME.size(),
            WM_PROTOCOLS_NAME.c_str()
        ),
        &error
    ));

    if (error || !protocolsAtomReply)
    {
        err() << "Failed to request WM_PROTOCOLS atom." << std::endl;
        return;
    }

    m_atomWmProtocols = protocolsAtomReply->atom;

    std::vector<xcb_atom_t> atoms;

    // Get the atom for registering the close event
    static const std::string WM_DELETE_WINDOW_NAME = "WM_DELETE_WINDOW";
    ScopedXcbPtr<xcb_intern_atom_reply_t> deleteWindowAtomReply(xcb_intern_atom_reply(
        m_connection,
        xcb_intern_atom(
            m_connection,
            0,
            WM_DELETE_WINDOW_NAME.size(),
            WM_DELETE_WINDOW_NAME.c_str()
        ),
        &error
    ));

    if (!error && deleteWindowAtomReply)
    {
        atoms.push_back(deleteWindowAtomReply->atom);
        m_atomClose = deleteWindowAtomReply->atom;
    }
    else
    {
        err() << "Failed to request WM_DELETE_WINDOW atom." << std::endl;
    }

    if (ewmhSupported() && m_ewmhConnection._NET_WM_PING && m_ewmhConnection._NET_WM_PID)
    {
        ScopedXcbPtr<xcb_generic_error_t> error(xcb_request_check(
            m_connection,
            xcb_ewmh_set_wm_pid(
                &m_ewmhConnection,
                m_window,
                getpid()
            )
        ));

        if (!error)
            atoms.push_back(m_ewmhConnection._NET_WM_PING);
    }

    if (!atoms.empty())
    {
        ScopedXcbPtr<xcb_generic_error_t> error(xcb_request_check(
            m_connection,
            xcb_icccm_set_wm_protocols(
                m_connection,
                m_window,
                protocolsAtomReply->atom,
                atoms.size(),
                &atoms[0]
            )
        ));

        if (error)
            err() << "Failed to set window protocols" << std::endl;
    }
    else
    {
        err() << "Didn't set any window protocols" << std::endl;
    }
}


////////////////////////////////////////////////////////////
void WindowImplX11::setMotifHints(unsigned long style)
{
    ScopedXcbPtr<xcb_generic_error_t> error(NULL);

    static const std::string MOTIF_WM_HINTS = "_MOTIF_WM_HINTS";
    ScopedXcbPtr<xcb_intern_atom_reply_t> hintsAtomReply(xcb_intern_atom_reply(
        m_connection,
        xcb_intern_atom(
            m_connection,
            0,
            MOTIF_WM_HINTS.size(),
            MOTIF_WM_HINTS.c_str()
        ),
        &error
    ));

    if (!error && hintsAtomReply)
    {
        static const unsigned long MWM_HINTS_FUNCTIONS   = 1 << 0;
        static const unsigned long MWM_HINTS_DECORATIONS = 1 << 1;

        //static const unsigned long MWM_DECOR_ALL         = 1 << 0;
        static const unsigned long MWM_DECOR_BORDER      = 1 << 1;
        static const unsigned long MWM_DECOR_RESIZEH     = 1 << 2;
        static const unsigned long MWM_DECOR_TITLE       = 1 << 3;
        static const unsigned long MWM_DECOR_MENU        = 1 << 4;
        static const unsigned long MWM_DECOR_MINIMIZE    = 1 << 5;
        static const unsigned long MWM_DECOR_MAXIMIZE    = 1 << 6;

        //static const unsigned long MWM_FUNC_ALL          = 1 << 0;
        static const unsigned long MWM_FUNC_RESIZE       = 1 << 1;
        static const unsigned long MWM_FUNC_MOVE         = 1 << 2;
        static const unsigned long MWM_FUNC_MINIMIZE     = 1 << 3;
        static const unsigned long MWM_FUNC_MAXIMIZE     = 1 << 4;
        static const unsigned long MWM_FUNC_CLOSE        = 1 << 5;

        struct WMHints
        {
            uint32_t flags;
            uint32_t functions;
            uint32_t decorations;
            int32_t  inputMode;
            uint32_t state;
        };

        WMHints hints;
        hints.flags       = MWM_HINTS_FUNCTIONS | MWM_HINTS_DECORATIONS;
        hints.decorations = 0;
        hints.functions   = 0;

        if (style & Style::Titlebar)
        {
            hints.decorations |= MWM_DECOR_BORDER | MWM_DECOR_TITLE | MWM_DECOR_MINIMIZE | MWM_DECOR_MENU;
            hints.functions   |= MWM_FUNC_MOVE | MWM_FUNC_MINIMIZE;
        }
        if (style & Style::Resize)
        {
            hints.decorations |= MWM_DECOR_MAXIMIZE | MWM_DECOR_RESIZEH;
            hints.functions   |= MWM_FUNC_MAXIMIZE | MWM_FUNC_RESIZE;
        }
        if (style & Style::Close)
        {
            hints.decorations |= 0;
            hints.functions   |= MWM_FUNC_CLOSE;
        }

        ScopedXcbPtr<xcb_generic_error_t> propertyError(xcb_request_check(
            m_connection,
            xcb_change_property_checked(
                m_connection,
                XCB_PROP_MODE_REPLACE,
                m_window,
                hintsAtomReply->atom,
                hintsAtomReply->atom,
                sizeof(hints.flags) * 8,
                sizeof(hints) / sizeof(hints.flags),
                reinterpret_cast<const unsigned char*>(&hints)
            )
        ));

        if (propertyError)
            err() << "xcb_change_property failed, could not set window hints" << std::endl;
    }
    else
    {
        err() << "Failed to request _MOTIF_WM_HINTS atom." << std::endl;
    }
}


////////////////////////////////////////////////////////////
void WindowImplX11::initialize()
{
    // Create the input context
    m_inputMethod = XOpenIM(m_display, NULL, NULL, NULL);

    if (m_inputMethod)
    {
        m_inputContext = XCreateIC(
            m_inputMethod,
            XNClientWindow,
            m_window,
            XNFocusWindow,
            m_window,
            XNInputStyle,
            XIMPreeditNothing | XIMStatusNothing,
            reinterpret_cast<void*>(NULL)
        );
    }
    else
    {
        m_inputContext = NULL;
    }

    if (!m_inputContext)
        err() << "Failed to create input context for window -- TextEntered event won't be able to return unicode" << std::endl;

    // Show the window
    setVisible(true);

    // Raise the window and grab input focus
    grabFocus();

    // Create the hidden cursor
    createHiddenCursor();

    // Flush the commands queue
    xcb_flush(m_connection);

    // Add this window to the global list of windows (required for focus request)
    allWindows.push_back(this);
}


////////////////////////////////////////////////////////////
void WindowImplX11::createHiddenCursor()
{
    xcb_pixmap_t cursorPixmap = xcb_generate_id(m_connection);

    // Create the cursor's pixmap (1x1 pixels)
    ScopedXcbPtr<xcb_generic_error_t> createPixmapError(xcb_request_check(
        m_connection,
        xcb_create_pixmap(
            m_connection,
            1,
            cursorPixmap,
            m_window,
            1,
            1
        )
    ));

    if (createPixmapError)
    {
        err() << "Failed to create pixmap for hidden cursor" << std::endl;
        return;
    }

    m_hiddenCursor = xcb_generate_id(m_connection);

    // Create the cursor, using the pixmap as both the shape and the mask of the cursor
    ScopedXcbPtr<xcb_generic_error_t> createCursorError(xcb_request_check(
        m_connection,
        xcb_create_cursor(
            m_connection,
            m_hiddenCursor,
            cursorPixmap,
            cursorPixmap,
            0, 0, 0, // Foreground RGB color
            0, 0, 0, // Background RGB color
            0,       // X
            0        // Y
        )
    ));

    if (createCursorError)
        err() << "Failed to create hidden cursor" << std::endl;

    // We don't need the pixmap any longer, free it
    ScopedXcbPtr<xcb_generic_error_t> freePixmapError(xcb_request_check(
        m_connection,
        xcb_free_pixmap(
            m_connection,
            cursorPixmap
        )
    ));

    if (freePixmapError)
        err() << "Failed to free pixmap for hidden cursor" << std::endl;
}


////////////////////////////////////////////////////////////
void WindowImplX11::cleanup()
{
    // Restore the previous video mode (in case we were running in fullscreen)
    resetVideoMode();

    // Unhide the mouse cursor (in case it was hidden)
    setMouseCursorVisible(true);
}


////////////////////////////////////////////////////////////
bool WindowImplX11::processEvent(xcb_generic_event_t* windowEvent)
{
    // Convert the X11 event to a sf::Event
    switch (windowEvent->response_type & ~0x80)
    {
        // Destroy event
        case XCB_DESTROY_NOTIFY:
        {
            // The window is about to be destroyed: we must cleanup resources
            cleanup();
            break;
        }

        // Gain focus event
        case XCB_FOCUS_IN:
        {
            // Update the input context
            if (m_inputContext)
                XSetICFocus(m_inputContext);

            Event event;
            event.type = Event::GainedFocus;
            pushEvent(event);

            // If the window has been previously marked urgent (notification) as a result of a focus request, undo that
            xcb_icccm_wm_hints_t hints;

            ScopedXcbPtr<xcb_generic_error_t> error(NULL);

            uint8_t result = xcb_icccm_get_wm_hints_reply(
                m_connection,
                xcb_icccm_get_wm_hints_unchecked(
                    m_connection,
                    m_window
                ),
                &hints,
                &error
            );

            if (!result || error)
            {
                err() << "Failed to get WM hints in XCB_FOCUS_IN" << std::endl;
                break;
            }

            // Remove urgency (notification) flag from hints
            hints.flags &= ~XCB_ICCCM_WM_HINT_X_URGENCY;

            ScopedXcbPtr<xcb_generic_error_t> setWmHintsError(xcb_request_check(
                m_connection,
                xcb_icccm_set_wm_hints_checked(
                    m_connection,
                    m_window,
                    &hints
                )
            ));

            if (setWmHintsError)
                err() << "Failed to set WM hints in XCB_FOCUS_IN" << std::endl;

            break;
        }

        // Lost focus event
        case XCB_FOCUS_OUT:
        {
            // Update the input context
            if (m_inputContext)
                XUnsetICFocus(m_inputContext);

            Event event;
            event.type = Event::LostFocus;
            pushEvent(event);
            break;
        }

        // Resize event
        case XCB_CONFIGURE_NOTIFY:
        {
            xcb_configure_notify_event_t* e = reinterpret_cast<xcb_configure_notify_event_t*>(windowEvent);
            Event event;
            event.type        = Event::Resized;
            event.size.width  = e->width;
            event.size.height = e->height;
            pushEvent(event);
            break;
        }

        // Close event
        case XCB_CLIENT_MESSAGE:
        {
            xcb_client_message_event_t* e = reinterpret_cast<xcb_client_message_event_t*>(windowEvent);

            // Handle window manager protocol messages we support
            if (e->type == m_atomWmProtocols)
            {
                if ((e->format == 32) && (e->data.data32[0]) == static_cast<long>(m_atomClose))
                {
                    // Handle the WM_DELETE_WINDOW message
                    Event event;
                    event.type = Event::Closed;
                    pushEvent(event);
                }
                else if (ewmhSupported() && (e->format == 32) && (e->data.data32[0]) == static_cast<long>(m_ewmhConnection._NET_WM_PING))
                {
                    // Handle the _NET_WM_PING message, send pong back to WM to show that we are responsive
                    e->window = XCBDefaultRootWindow(m_connection);

                    ScopedXcbPtr<xcb_generic_error_t> pongError(xcb_request_check(
                        m_connection,
                        xcb_send_event_checked(
                            m_connection,
                            0,
                            XCBDefaultRootWindow(m_connection),
                            XCB_EVENT_MASK_SUBSTRUCTURE_NOTIFY | XCB_EVENT_MASK_SUBSTRUCTURE_REDIRECT,
                            reinterpret_cast<char*>(e)
                        )
                    ));

                    if (pongError)
                        err() << "Could not send pong event back to WM" << std::endl;
                }
            }
            break;
        }

        // Key down event
        case XCB_KEY_PRESS:
        {
            xcb_key_press_event_t* e = reinterpret_cast<xcb_key_press_event_t*>(windowEvent);

            // Get the keysym of the key that has been pressed
            static XComposeStatus keyboard;
            char buffer[32];
            KeySym symbol;

            // There is no xcb equivalent of XLookupString, so the xcb event
            // has to be converted to an XEvent
            XEvent fake_event;
            fake_event.type = KeyPress;
            fake_event.xany.display = m_display;
            fake_event.xany.window  = e->event;
            fake_event.xkey.state   = e->state;
            fake_event.xkey.keycode = e->detail;

            XLookupString(&fake_event.xkey, buffer, sizeof(buffer), &symbol, &keyboard);

            // Fill the event parameters
            // TODO: if modifiers are wrong, use XGetModifierMapping to retrieve the actual modifiers mapping
            Event event;
            event.type        = Event::KeyPressed;
            event.key.code    = keysymToSF(symbol);
            event.key.alt     = e->state & XCB_MOD_MASK_1;
            event.key.control = e->state & XCB_MOD_MASK_CONTROL;
            event.key.shift   = e->state & XCB_MOD_MASK_SHIFT;
            event.key.system  = e->state & XCB_MOD_MASK_4;
            pushEvent(event);

            // Generate a TextEntered event
            if (!XFilterEvent(&fake_event, None))
            {
                #ifdef X_HAVE_UTF8_STRING
                if (m_inputContext)
                {
                    Status status;
                    Uint8  keyBuffer[16];

                    int length = Xutf8LookupString(
                        m_inputContext,
                        &fake_event.xkey,
                        reinterpret_cast<char*>(keyBuffer),
                        sizeof(keyBuffer),
                        NULL,
                        &status
                    );

                    if (length > 0)
                    {
                        Uint32 unicode = 0;
                        Utf8::decode(keyBuffer, keyBuffer + length, unicode, 0);
                        if (unicode != 0)
                        {
                            Event textEvent;
                            textEvent.type         = Event::TextEntered;
                            textEvent.text.unicode = unicode;
                            pushEvent(textEvent);
                        }
                    }
                }
                else
                #endif
                {
                    static XComposeStatus status;
                    char keyBuffer[16];
                    if (XLookupString(&fake_event.xkey, keyBuffer, sizeof(keyBuffer), NULL, &status))
                    {
                        Event textEvent;
                        textEvent.type         = Event::TextEntered;
                        textEvent.text.unicode = static_cast<Uint32>(keyBuffer[0]);
                        pushEvent(textEvent);
                    }
                }
            }

            break;
        }

        // Key up event
        case XCB_KEY_RELEASE:
        {
            xcb_key_release_event_t* e = reinterpret_cast<xcb_key_release_event_t*>(windowEvent);

            // Get the keysym of the key that has been pressed
            char buffer[32];
            KeySym symbol;

            // There is no xcb equivalent of XLookupString, so the xcb event
            // has to be converted to an XEvent
            XKeyEvent fake_event;
            fake_event.display = m_display;
            fake_event.state   = e->state;
            fake_event.keycode = e->detail;
            XLookupString(&fake_event, buffer, 32, &symbol, NULL);

            // Fill the event parameters
            Event event;
            event.type        = Event::KeyReleased;
            event.key.code    = keysymToSF(symbol);
            event.key.alt     = e->state & XCB_MOD_MASK_1;
            event.key.control = e->state & XCB_MOD_MASK_CONTROL;
            event.key.shift   = e->state & XCB_MOD_MASK_SHIFT;
            event.key.system  = e->state & XCB_MOD_MASK_4;
            pushEvent(event);

            break;
        }

        // Mouse button pressed
        case XCB_BUTTON_PRESS:
        {
            xcb_button_press_event_t* e = reinterpret_cast<xcb_button_press_event_t*>(windowEvent);

            // XXX: Why button 8 and 9?
            // Because 4 and 5 are the vertical wheel and 6 and 7 are horizontal wheel ;)
            xcb_button_t button = e->detail;
            if ((button == XCB_BUTTON_INDEX_1) ||
                (button == XCB_BUTTON_INDEX_2) ||
                (button == XCB_BUTTON_INDEX_3) ||
                (button == 8) ||
                (button == 9))
            {
                Event event;
                event.type          = Event::MouseButtonPressed;
                event.mouseButton.x = e->event_x;
                event.mouseButton.y = e->event_y;
                switch(button)
                {
                case XCB_BUTTON_INDEX_1: event.mouseButton.button = Mouse::Left;     break;
                case XCB_BUTTON_INDEX_2: event.mouseButton.button = Mouse::Middle;   break;
                case XCB_BUTTON_INDEX_3: event.mouseButton.button = Mouse::Right;    break;
                case 8:                  event.mouseButton.button = Mouse::XButton1; break;
                case 9:                  event.mouseButton.button = Mouse::XButton2; break;
                }
                pushEvent(event);
            }
            break;
        }

        // Mouse button released
        case XCB_BUTTON_RELEASE:
        {
            xcb_button_release_event_t* e = reinterpret_cast<xcb_button_press_event_t*>(windowEvent);

            xcb_button_t button = e->detail;
            if ((button == XCB_BUTTON_INDEX_1) ||
                (button == XCB_BUTTON_INDEX_2) ||
                (button == XCB_BUTTON_INDEX_3) ||
                (button == 8) ||
                (button == 9))
            {
                Event event;
                event.type          = Event::MouseButtonReleased;
                event.mouseButton.x = e->event_x;
                event.mouseButton.y = e->event_y;
                switch(button)
                {
                case XCB_BUTTON_INDEX_1: event.mouseButton.button = Mouse::Left;     break;
                case XCB_BUTTON_INDEX_2: event.mouseButton.button = Mouse::Middle;   break;
                case XCB_BUTTON_INDEX_3: event.mouseButton.button = Mouse::Right;    break;
                case 8:                  event.mouseButton.button = Mouse::XButton1; break;
                case 9:                  event.mouseButton.button = Mouse::XButton2; break;
                }
                pushEvent(event);
            }
            else if ((button == XCB_BUTTON_INDEX_4) || (button == XCB_BUTTON_INDEX_5))
            {
                Event event;

                event.type             = Event::MouseWheelMoved;
                event.mouseWheel.delta = button == XCB_BUTTON_INDEX_4 ? 1 : -1;
                event.mouseWheel.x     = e->event_x;
                event.mouseWheel.y     = e->event_y;
                pushEvent(event);

                event.type                     = Event::MouseWheelVerticalMoved;
                event.mouseWheelVertical.delta = button == XCB_BUTTON_INDEX_4 ? 1 : -1;
                event.mouseWheelVertical.x     = e->event_x;
                event.mouseWheelVertical.y     = e->event_y;
                pushEvent(event);
            }
            else if ((button == 6) || (button == 7))
            {
                Event event;
                event.type                       = Event::MouseWheelHorizontalMoved;
                event.mouseWheelHorizontal.delta = button == 6 ? 1 : -1;
                event.mouseWheelHorizontal.x     = e->event_x;
                event.mouseWheelHorizontal.y     = e->event_y;
                pushEvent(event);
            }
            break;
        }

        // Mouse moved
        case XCB_MOTION_NOTIFY:
        {
            xcb_motion_notify_event_t* e = reinterpret_cast<xcb_motion_notify_event_t*>(windowEvent);
            Event event;
            event.type        = Event::MouseMoved;
            event.mouseMove.x = e->event_x;
            event.mouseMove.y = e->event_y;
            pushEvent(event);
            break;
        }

        // Mouse entered
        case XCB_ENTER_NOTIFY:
        {
            xcb_enter_notify_event_t* enterNotifyEvent = reinterpret_cast<xcb_enter_notify_event_t*>(windowEvent);

            if (enterNotifyEvent->mode == NotifyNormal)
            {
                Event event;
                event.type = Event::MouseEntered;
                pushEvent(event);
            }
            break;
        }

        // Mouse left
        case XCB_LEAVE_NOTIFY:
        {
            xcb_leave_notify_event_t* leaveNotifyEvent = reinterpret_cast<xcb_leave_notify_event_t*>(windowEvent);

            if (leaveNotifyEvent->mode == NotifyNormal)
            {
                Event event;
                event.type = Event::MouseLeft;
                pushEvent(event);
            }
            break;
        }

        // Parent window changed
        case XCB_REPARENT_NOTIFY:
        {
            // Catch reparent events to properly apply fullscreen on
            // some "strange" window managers (like Awesome) which
            // seem to make use of temporary parents during mapping
            if (m_fullscreen)
                switchToFullscreen();

            xcb_flush(m_connection); // Discard remaining events
            break;
        }
    }

    return true;
}


////////////////////////////////////////////////////////////
Keyboard::Key WindowImplX11::keysymToSF(xcb_keysym_t symbol)
{
    // First convert to uppercase (to avoid dealing with two different keysyms for the same key)
    KeySym lower, key;
    XConvertCase(symbol, &lower, &key);

    switch (key)
    {
        case XK_Shift_L:      return Keyboard::LShift;
        case XK_Shift_R:      return Keyboard::RShift;
        case XK_Control_L:    return Keyboard::LControl;
        case XK_Control_R:    return Keyboard::RControl;
        case XK_Alt_L:        return Keyboard::LAlt;
        case XK_Alt_R:        return Keyboard::RAlt;
        case XK_Super_L:      return Keyboard::LSystem;
        case XK_Super_R:      return Keyboard::RSystem;
        case XK_Menu:         return Keyboard::Menu;
        case XK_Escape:       return Keyboard::Escape;
        case XK_semicolon:    return Keyboard::SemiColon;
        case XK_slash:        return Keyboard::Slash;
        case XK_equal:        return Keyboard::Equal;
        case XK_minus:        return Keyboard::Dash;
        case XK_bracketleft:  return Keyboard::LBracket;
        case XK_bracketright: return Keyboard::RBracket;
        case XK_comma:        return Keyboard::Comma;
        case XK_period:       return Keyboard::Period;
        case XK_dead_acute:   return Keyboard::Quote;
        case XK_backslash:    return Keyboard::BackSlash;
        case XK_dead_grave:   return Keyboard::Tilde;
        case XK_space:        return Keyboard::Space;
        case XK_Return:       return Keyboard::Return;
        case XK_KP_Enter:     return Keyboard::Return;
        case XK_BackSpace:    return Keyboard::BackSpace;
        case XK_Tab:          return Keyboard::Tab;
        case XK_Prior:        return Keyboard::PageUp;
        case XK_Next:         return Keyboard::PageDown;
        case XK_End:          return Keyboard::End;
        case XK_Home:         return Keyboard::Home;
        case XK_Insert:       return Keyboard::Insert;
        case XK_Delete:       return Keyboard::Delete;
        case XK_KP_Add:       return Keyboard::Add;
        case XK_KP_Subtract:  return Keyboard::Subtract;
        case XK_KP_Multiply:  return Keyboard::Multiply;
        case XK_KP_Divide:    return Keyboard::Divide;
        case XK_Pause:        return Keyboard::Pause;
        case XK_F1:           return Keyboard::F1;
        case XK_F2:           return Keyboard::F2;
        case XK_F3:           return Keyboard::F3;
        case XK_F4:           return Keyboard::F4;
        case XK_F5:           return Keyboard::F5;
        case XK_F6:           return Keyboard::F6;
        case XK_F7:           return Keyboard::F7;
        case XK_F8:           return Keyboard::F8;
        case XK_F9:           return Keyboard::F9;
        case XK_F10:          return Keyboard::F10;
        case XK_F11:          return Keyboard::F11;
        case XK_F12:          return Keyboard::F12;
        case XK_F13:          return Keyboard::F13;
        case XK_F14:          return Keyboard::F14;
        case XK_F15:          return Keyboard::F15;
        case XK_Left:         return Keyboard::Left;
        case XK_Right:        return Keyboard::Right;
        case XK_Up:           return Keyboard::Up;
        case XK_Down:         return Keyboard::Down;
        case XK_KP_0:         return Keyboard::Numpad0;
        case XK_KP_1:         return Keyboard::Numpad1;
        case XK_KP_2:         return Keyboard::Numpad2;
        case XK_KP_3:         return Keyboard::Numpad3;
        case XK_KP_4:         return Keyboard::Numpad4;
        case XK_KP_5:         return Keyboard::Numpad5;
        case XK_KP_6:         return Keyboard::Numpad6;
        case XK_KP_7:         return Keyboard::Numpad7;
        case XK_KP_8:         return Keyboard::Numpad8;
        case XK_KP_9:         return Keyboard::Numpad9;
        case XK_A:            return Keyboard::A;
        case XK_Z:            return Keyboard::Z;
        case XK_E:            return Keyboard::E;
        case XK_R:            return Keyboard::R;
        case XK_T:            return Keyboard::T;
        case XK_Y:            return Keyboard::Y;
        case XK_U:            return Keyboard::U;
        case XK_I:            return Keyboard::I;
        case XK_O:            return Keyboard::O;
        case XK_P:            return Keyboard::P;
        case XK_Q:            return Keyboard::Q;
        case XK_S:            return Keyboard::S;
        case XK_D:            return Keyboard::D;
        case XK_F:            return Keyboard::F;
        case XK_G:            return Keyboard::G;
        case XK_H:            return Keyboard::H;
        case XK_J:            return Keyboard::J;
        case XK_K:            return Keyboard::K;
        case XK_L:            return Keyboard::L;
        case XK_M:            return Keyboard::M;
        case XK_W:            return Keyboard::W;
        case XK_X:            return Keyboard::X;
        case XK_C:            return Keyboard::C;
        case XK_V:            return Keyboard::V;
        case XK_B:            return Keyboard::B;
        case XK_N:            return Keyboard::N;
        case XK_0:            return Keyboard::Num0;
        case XK_1:            return Keyboard::Num1;
        case XK_2:            return Keyboard::Num2;
        case XK_3:            return Keyboard::Num3;
        case XK_4:            return Keyboard::Num4;
        case XK_5:            return Keyboard::Num5;
        case XK_6:            return Keyboard::Num6;
        case XK_7:            return Keyboard::Num7;
        case XK_8:            return Keyboard::Num8;
        case XK_9:            return Keyboard::Num9;
    }

    return Keyboard::Unknown;
}

} // namespace priv

} // namespace sf
