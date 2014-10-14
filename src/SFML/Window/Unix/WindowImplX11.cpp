////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007-2014 Laurent Gomila (laurent.gom@gmail.com)
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
#include <SFML/System/Utf.hpp>
#include <SFML/System/Err.hpp>
#include <X11/keysym.h>
#include <X11/Xutil.h>
#include <X11/Xatom.h>
#include <xcb/xcb_atom.h>
#include <xcb/xcb_icccm.h>
#include <xcb/xcb_image.h>
#include <xcb/xcb_util.h>
#include <xcb/randr.h>
#include <unistd.h>
#include <libgen.h>
#include <cstring>
#include <sstream>
#include <vector>
#include <string>
#include <iterator>
#include <algorithm>
#include <iostream>

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
    unsigned long                         eventMask        = FocusChangeMask | ButtonPressMask | ButtonReleaseMask | ButtonMotionMask |
                                                             PointerMotionMask | KeyPressMask | KeyReleaseMask | StructureNotifyMask |
                                                             EnterWindowMask | LeaveWindowMask;

    // Filter the events received by windows (only allow those matching a specific window)
    Bool checkEvent(::Display*, XEvent* event, XPointer userData)
    {
        // Just check if the event matches the window
        return event->xany.window == reinterpret_cast< ::Window >(userData);
    }

    // Find the name of the current executable
    void findExecutableName(char* buffer, std::size_t bufferSize)
    {
        //Default fallback name
        const char* executableName = "sfml";
        std::size_t length = readlink("/proc/self/exe", buffer, bufferSize);
        if ((length > 0) && (length < bufferSize))
        {
            // Remove the path to keep the executable name only
            buffer[length] = '\0';
            executableName = basename(buffer);
        }
        std::memmove(buffer, executableName, std::strlen(executableName) + 1);
    }
}


namespace sf
{
namespace priv
{
////////////////////////////////////////////////////////////
WindowImplX11::WindowImplX11(WindowHandle handle) :
m_window      (0),
m_inputMethod (NULL),
m_inputContext(NULL),
m_isExternal  (true),
m_atomClose   (0),
m_oldVideoMode(-1),
m_hiddenCursor(0),
m_keyRepeat   (true),
m_previousSize(-1, -1),
m_useSizeHints(false)
{
    // Open a connection with the X server
    m_display = OpenDisplay();
    XSetEventQueueOwner(m_display, XCBOwnsEventQueue);
    m_connection = XGetXCBConnection(m_display);

    if (!m_connection)
    {
        err() << "Failed cast Display object to an XCB connection object" << std::endl;
        return;
    }

    m_screen = DefaultScreen(m_display);

    // Save the window handle
    m_window = handle;

    if (m_window)
    {
        // Make sure the window is listening to all the required events
        const uint32_t value_list[] = {eventMask};

        xcb_change_window_attributes(m_connection,
                                     m_window,
                                     XCB_CW_EVENT_MASK,
                                     value_list);

        // Do some common initializations
        initialize();
    }
}


////////////////////////////////////////////////////////////
WindowImplX11::WindowImplX11(VideoMode mode, const String& title, unsigned long style, const ContextSettings& settings) :
m_window      (0),
m_inputMethod (NULL),
m_inputContext(NULL),
m_isExternal  (false),
m_atomClose   (0),
m_oldVideoMode(-1),
m_hiddenCursor(0),
m_keyRepeat   (true),
m_previousSize(-1, -1),
m_useSizeHints(false)
{
    // Open a connection with the X server
    m_display = OpenDisplay();
    XSetEventQueueOwner(m_display, XCBOwnsEventQueue);
    m_connection = XGetXCBConnection(m_display);
    if (! m_connection)
    {
        err() << "Failed cast Display object to an XCB connection object" << std::endl;
        return;
    }
    m_screen  = DefaultScreen(m_display);

    // Compute position and size
    int left, top;
    bool fullscreen = (style & Style::Fullscreen) != 0;
    if (!fullscreen)
    {
        left = (DisplayWidth(m_display, m_screen)  - mode.width)  / 2;
        top  = (DisplayHeight(m_display, m_screen) - mode.height) / 2;
    }
    else
    {
        left = 0;
        top  = 0;
    }
    int width  = mode.width;
    int height = mode.height;

    // Switch to fullscreen if necessary
    if (fullscreen)
        switchToFullscreen(mode);

    // Choose the visual according to the context settings
    XVisualInfo visualInfo = ContextType::selectBestVisual(m_display, mode.bitsPerPixel, settings);

    // Define the window attributes
    const uint32_t value_list[] = {fullscreen, eventMask};

    // Create the window
    m_window = xcb_generate_id(m_connection);

    xcb_void_cookie_t cookie = xcb_create_window_checked(
                m_connection,
                XCB_COPY_FROM_PARENT,
                m_window,
                RootWindow(m_display, m_screen),
                left, top,
                width, height,
                0,
                XCB_WINDOW_CLASS_INPUT_OUTPUT,
                XCB_COPY_FROM_PARENT,
                XCB_CW_EVENT_MASK | XCB_CW_OVERRIDE_REDIRECT,
                value_list);

    xcb_generic_error_t* errptr = xcb_request_check(m_connection, cookie);
    bool createWindowFailed = (errptr != NULL);
    free(errptr);

    if (createWindowFailed)
    {
        err() << "Failed to create window" << std::endl;
        return;
    }

    // Set the window's name
    setTitle(title);

    // Set the window's style (tell the windows manager to change our window's decorations and functions according to the requested style)
    if (!fullscreen)
    {
        static const std::string MOTIF_WM_HINTS = "_MOTIF_WM_HINTS";
        xcb_intern_atom_cookie_t hintsAtomRequest = xcb_intern_atom(
            m_connection,
            0,
            MOTIF_WM_HINTS.size(),
            MOTIF_WM_HINTS.c_str()
        );
        xcb_intern_atom_reply_t* hintsAtomReply = xcb_intern_atom_reply(
            m_connection,
            hintsAtomRequest,
            NULL
        );

        if (hintsAtomReply)
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

            const unsigned char* ptr = reinterpret_cast<const unsigned char*>(&hints);
            xcb_change_property(m_connection, XCB_PROP_MODE_REPLACE, m_window,
                                    hintsAtomReply->atom, XA_WM_HINTS, 32, 5, ptr);

            free(hintsAtomReply);
        }

        // This is a hack to force some windows managers to disable resizing
        if (!(style & Style::Resize))
        {
            m_useSizeHints = true;
            xcb_size_hints_t sizeHints;
            sizeHints.flags      = XCB_ICCCM_SIZE_HINT_P_MIN_SIZE | XCB_ICCCM_SIZE_HINT_P_MAX_SIZE;
            sizeHints.min_width  = sizeHints.max_width  = width;
            sizeHints.min_height = sizeHints.max_height = height;
            xcb_icccm_set_wm_normal_hints(m_connection, m_window, &sizeHints);
        }
    }

    // Set the window's WM class (this can be used by window managers)
    char windowClass[512];
    findExecutableName(windowClass, sizeof(windowClass));
    XClassHint* classHint = XAllocClassHint();
    classHint->res_name = windowClass;
    classHint->res_class = windowClass;
    XSetClassHint(m_display, m_window, classHint);
    XFree(classHint);

    // Do some common initializations
    initialize();

    // In fullscreen mode, we must grab keyboard and mouse inputs
    if (fullscreen)
    {
        xcb_grab_pointer(m_connection, True, m_window, 0, XCB_GRAB_MODE_ASYNC, XCB_GRAB_MODE_ASYNC, m_window, XCB_NONE, XCB_CURRENT_TIME);
        xcb_grab_keyboard(m_connection, True, m_window, XCB_CURRENT_TIME, XCB_GRAB_MODE_ASYNC, XCB_GRAB_MODE_ASYNC);
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

    while(event = xcb_poll_for_event(m_connection))
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
    ::Window rootWindow = RootWindow(m_display, m_screen);
    ::Window topLevelWindow = m_window;
    ::Window nextWindow = topLevelWindow;
    xcb_query_tree_cookie_t treeCookie;
    xcb_query_tree_reply_t* treeReply = NULL;

    // Get "top level" window, i.e. the window with the root window as its parent.
    while (nextWindow != rootWindow)
    {
        topLevelWindow = nextWindow;

        treeCookie = xcb_query_tree(m_connection, topLevelWindow);
        treeReply = xcb_query_tree_reply(m_connection, treeCookie, NULL);
        nextWindow = treeReply->parent;
        free(treeReply);
    }

    xcb_get_geometry_cookie_t geometryCookie = xcb_get_geometry(m_connection, topLevelWindow);
    xcb_get_geometry_reply_t* geometryReply = xcb_get_geometry_reply(m_connection, geometryCookie, NULL);
    sf::Vector2i result(geometryReply->x, geometryReply->y);
    free(geometryReply);

    return result;
}


////////////////////////////////////////////////////////////
void WindowImplX11::setPosition(const Vector2i& position)
{
    uint32_t values[] = {position.x, position.y};
    xcb_configure_window(m_connection, m_window,
                         XCB_CONFIG_WINDOW_X | XCB_CONFIG_WINDOW_Y,
                         values);
    xcb_flush(m_connection);
}


////////////////////////////////////////////////////////////
Vector2u WindowImplX11::getSize() const
{
    xcb_get_geometry_reply_t* reply = xcb_get_geometry_reply(m_connection, xcb_get_geometry(m_connection, m_window), NULL);
    Vector2u result(reply->width, reply->height);
    free(reply);

    return result;
}


////////////////////////////////////////////////////////////
void WindowImplX11::setSize(const Vector2u& size)
{
    // If resizing is disable for the window we have to update the size hints (required by some window managers).
    if( m_useSizeHints ) {
        xcb_size_hints_t sizeHints;
        sizeHints.flags      = XCB_ICCCM_SIZE_HINT_P_MIN_SIZE | XCB_ICCCM_SIZE_HINT_P_MAX_SIZE;
        sizeHints.min_width  = sizeHints.max_width  = size.x;
        sizeHints.min_height = sizeHints.max_height = size.y;
        xcb_icccm_set_wm_normal_hints(m_connection, m_window, &sizeHints);
    }

    uint32_t values[] = {size.x, size.y};
    xcb_configure_window(m_connection, m_window,
                         XCB_CONFIG_WINDOW_WIDTH | XCB_CONFIG_WINDOW_HEIGHT,
                         values);
    xcb_flush(m_connection);
}


////////////////////////////////////////////////////////////
void WindowImplX11::setTitle(const String& title)
{
    // XCB takes UTF-8-encoded strings.
    std::basic_string<sf::Uint8> utf8String;
    sf::Utf<32>::toUtf8(
        title.begin(), title.end(),
        std::back_inserter( utf8String )
    );

    xcb_change_property(m_connection, XCB_PROP_MODE_REPLACE, m_window,
                        XA_WM_NAME, XA_STRING,
                        8, utf8String.length(), utf8String.c_str());
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
    uint8_t defDepth  = DefaultDepth(m_display, m_screen);

    xcb_pixmap_t iconPixmap = xcb_generate_id(m_connection);
    xcb_create_pixmap(m_connection, defDepth, iconPixmap, RootWindow(m_display, m_screen),
                      width, height);

    xcb_gcontext_t iconGC = xcb_generate_id(m_connection);
    xcb_create_gc(m_connection, iconGC, iconPixmap, 0, NULL);
    xcb_void_cookie_t cookie = xcb_put_image_checked(
                m_connection, XCB_IMAGE_FORMAT_Z_PIXMAP, iconPixmap, iconGC,
                width, height, 0, 0, 0, defDepth, sizeof(iconPixels), iconPixels);
    xcb_free_gc(m_connection, iconGC);

    xcb_generic_error_t* errptr = xcb_request_check(m_connection, cookie);
    bool setWindowIconFailed = (errptr != NULL);
    free(errptr);

    if (setWindowIconFailed)
    {
        err() << "Failed to set the window's icon: Error code " << (int)errptr->error_code << std::endl;
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
    xcb_pixmap_t maskPixmap = xcb_create_pixmap_from_bitmap_data(m_connection, m_window, (Uint8*)&maskPixels[0], width, height, 1, 0, 1, NULL);

    // Send our new icon to the window through the WMHints
    xcb_icccm_wm_hints_t hints;
    hints.flags       = XCB_ICCCM_WM_HINT_ICON_PIXMAP | XCB_ICCCM_WM_HINT_ICON_MASK;
    hints.icon_pixmap = iconPixmap;
    hints.icon_mask   = maskPixmap;
    xcb_icccm_set_wm_hints(m_connection, m_window, &hints);

    xcb_flush(m_connection);
}


////////////////////////////////////////////////////////////
void WindowImplX11::setVisible(bool visible)
{
    if (visible)
        xcb_map_window(m_connection, m_window);
    else
        xcb_unmap_window(m_connection, m_window);

    xcb_flush(m_connection);
}


////////////////////////////////////////////////////////////
void WindowImplX11::setMouseCursorVisible(bool visible)
{
    const uint32_t values = visible ? XCB_NONE : m_hiddenCursor;
    xcb_change_window_attributes(m_connection, m_window, XCB_CW_CURSOR, &values);
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
    XWindowAttributes attributes;
    if (XGetWindowAttributes(m_display, m_window, &attributes) == 0)
    {
        sf::err() << "Failed to check if window is viewable while requesting focus" << std::endl;
        return; // error getting attribute
    }

    bool windowViewable = (attributes.map_state == IsViewable);
    
    if (sfmlWindowFocused && windowViewable)
    {
        // Another SFML window of this application has the focus and the current window is viewable:
        // steal focus (i.e. bring window to the front and give it input focus)
        XRaiseWindow(m_display, m_window);
        XSetInputFocus(m_display, m_window, RevertToPointerRoot, CurrentTime);
    }
    else
    {
        // Otherwise: display urgency hint (flashing application logo)
        // Ensure WM hints exist, allocate if necessary
        XWMHints* hints = XGetWMHints(m_display, m_window);
        if (hints == NULL)
            hints = XAllocWMHints();
        
        // Add urgency (notification) flag to hints
        hints->flags |= XUrgencyHint;
        XSetWMHints(m_display, m_window, hints);
        XFree(hints);
    }
}


////////////////////////////////////////////////////////////
bool WindowImplX11::hasFocus() const
{
    ::Window focusedWindow = 0;
    int revertToReturn = 0;
    XGetInputFocus(m_display, &focusedWindow, &revertToReturn);

    return m_window == focusedWindow;
}


////////////////////////////////////////////////////////////
void WindowImplX11::switchToFullscreen(const VideoMode& mode)
{
    // Check if the XRandR extension is present
    xcb_query_extension_reply_t* randr_ext =
            xcb_query_extension_reply(m_connection, xcb_query_extension(m_connection, 5, "RANDR"), NULL);
    if (randr_ext->present)
    {
        xcb_screen_t* screen = XCBScreenOfDisplay(m_connection, m_screen);
        // Get the current configuration
        xcb_generic_error_t* errors;
        xcb_randr_get_screen_info_reply_t* config =
                xcb_randr_get_screen_info_reply(m_connection,
                                                xcb_randr_get_screen_info(m_connection, screen->root),
                                                &errors);
        if (! errors)
        {
            // Save the current video mode before we switch to fullscreen
            m_oldVideoMode = config->sizeID;

            // Get the available screen sizes
            xcb_randr_screen_size_t* sizes = xcb_randr_get_screen_info_sizes(config);
            if (sizes && (config->nSizes > 0))
            {
                // Search a matching size
                for (int i = 0; i < config->nSizes; ++i)
                {
                    if ((sizes[i].width == static_cast<int>(mode.width)) && (sizes[i].height == static_cast<int>(mode.height)))
                    {
                        // Switch to fullscreen mode
                        xcb_randr_set_screen_config(m_connection,
                                                    screen->root,
                                                    config->timestamp,
                                                    config->config_timestamp,
                                                    i, config->rotation, config->rate);

                        // Set "this" as the current fullscreen window
                        fullscreenWindow = this;
                        break;
                    }
                }
            }
        }
        else
        {
            // Failed to get the screen configuration
            err() << "Failed to get the current screen configuration for fullscreen mode, switching to window mode" << std::endl;
        }
        free(errors);
        free(config);
    }
    else
    {
        // XRandr extension is not supported: we cannot use fullscreen mode
        err() << "Fullscreen is not supported, switching to window mode" << std::endl;
    }
    free(randr_ext);
}


////////////////////////////////////////////////////////////
void WindowImplX11::initialize()
{
    // Get the atoms for registering the close event
    static const std::string WM_DELETE_WINDOW_NAME = "WM_DELETE_WINDOW";

    xcb_intern_atom_cookie_t deleteWindowAtomRequest = xcb_intern_atom(
        m_connection,
        0,
        WM_DELETE_WINDOW_NAME.size(),
        WM_DELETE_WINDOW_NAME.c_str()
    );
    xcb_intern_atom_reply_t* deleteWindowAtomReply = xcb_intern_atom_reply(
        m_connection,
        deleteWindowAtomRequest,
        NULL
    );

    static const std::string WM_PROTOCOLS_NAME = "WM_PROTOCOLS";

    xcb_intern_atom_cookie_t protocolsAtomRequest = xcb_intern_atom(
        m_connection,
        0,
        WM_PROTOCOLS_NAME.size(),
        WM_PROTOCOLS_NAME.c_str()
    );
    xcb_intern_atom_reply_t* protocolsAtomReply = xcb_intern_atom_reply(
        m_connection,
        protocolsAtomRequest,
        NULL
    );

    if (protocolsAtomReply && deleteWindowAtomReply)
    {
        xcb_icccm_set_wm_protocols(
            m_connection,
            m_window,
            protocolsAtomReply->atom,
            1,
            &deleteWindowAtomReply->atom
        );

        m_atomClose = deleteWindowAtomReply->atom;
    }
    else
    {
        // Should not happen, but better safe than sorry.
        std::cerr << "Failed to request WM_PROTOCOLS/WM_DELETE_WINDOW_NAME atoms." << std::endl;
    }

    free(protocolsAtomReply);
    free(deleteWindowAtomReply);

    // Create the input context
    m_inputMethod = XOpenIM(m_display, NULL, NULL, NULL);
    if (m_inputMethod)
    {
        m_inputContext = XCreateIC(m_inputMethod,
                                   XNClientWindow, m_window,
                                   XNFocusWindow,  m_window,
                                   XNInputStyle,   XIMPreeditNothing  | XIMStatusNothing,
                                   (void*)NULL);
    }
    else
    {
        m_inputContext = NULL;
    }
    if (!m_inputContext)
        err() << "Failed to create input context for window -- TextEntered event won't be able to return unicode" << std::endl;

    // Show the window
    xcb_map_window(m_connection, m_window);
    xcb_flush(m_connection);

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
    m_hiddenCursor = xcb_generate_id(m_connection);
    // Create the cursor's pixmap (1x1 pixels)
    xcb_create_pixmap(m_connection, 1, cursorPixmap, m_window, 1, 1);

    // Create the cursor, using the pixmap as both the shape and the mask of the cursor
    xcb_create_cursor(m_connection, m_hiddenCursor, cursorPixmap, cursorPixmap,
                      0, 0, 0,  // Fore color
                      0, 0, 0,  // Back color
                      0, 0);

    // We don't need the pixmap any longer, free it
    xcb_free_pixmap(m_connection, cursorPixmap);
}


////////////////////////////////////////////////////////////
void WindowImplX11::cleanup()
{
    // Restore the previous video mode (in case we were running in fullscreen)
    if (fullscreenWindow == this)
    {
        // Retrieve the default screen
        xcb_screen_t* screen = XCBScreenOfDisplay(m_connection, m_screen);

        // Get current screen info
        xcb_generic_error_t* errors;
        xcb_randr_get_screen_info_reply_t* config = xcb_randr_get_screen_info_reply(
                    m_connection, xcb_randr_get_screen_info(m_connection, screen->root), &errors);
        if (! errors)
        {
            // Reset the video mode
            xcb_randr_set_screen_config(m_connection,
                                        screen->root,
                                        CurrentTime,
                                        config->config_timestamp,
                                        m_oldVideoMode,
                                        config->rotation, config->rate);

            // Free the configuration instance
            free(config);
        }
        free(errors);

        // Reset the fullscreen window
        fullscreenWindow = NULL;
    }

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
            XWMHints* hints = XGetWMHints(m_display, m_window);
            if (hints != NULL)
            {
                // Remove urgency (notification) flag from hints
                hints->flags &= ~XUrgencyHint;
                XSetWMHints(m_display, m_window, hints);
                XFree(hints);
            }
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
            if ((e->format == 32) && (e->data.data32[0]) == static_cast<long>(m_atomClose))
            {
                Event event;
                event.type = Event::Closed;
                pushEvent(event);
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
            fake_event.xany.window = e->event;
            fake_event.xkey.state = e->state;
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
                    int length = Xutf8LookupString(m_inputContext, &fake_event.xkey, reinterpret_cast<char*>(keyBuffer), sizeof(keyBuffer), NULL, &status);
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
            fake_event.state = e->state;
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
            xcb_button_t button = e->detail;
            if ((button == XCB_BUTTON_INDEX_1) || (button == XCB_BUTTON_INDEX_2)
                    || (button == XCB_BUTTON_INDEX_3) || (button == 8) || (button == 9))
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
            if ((button == XCB_BUTTON_INDEX_1) || (button == XCB_BUTTON_INDEX_2)
                    || (button == XCB_BUTTON_INDEX_3) || (button == 8) || (button == 9))
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
            else if ((button == XCB_BUTTON_INDEX_4) || (button == XCB_BUTTON_INDEX_5))
            {
                Event event;
                event.type             = Event::MouseWheelMoved;
                event.mouseWheel.delta = button == XCB_BUTTON_INDEX_4 ? 1 : -1;
                event.mouseWheel.x     = e->event_x;
                event.mouseWheel.y     = e->event_y;
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
        case ReparentNotify:
        {
            XSync(m_display, True); // Discard remaining events
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
