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
#include <SFML/Window/WindowStyle.hpp> // important to be included first (conflict with None)
#include <SFML/Window/Linux/WindowImplX11.hpp>
#include <SFML/Window/Linux/GlxContext.hpp>
#include <SFML/Window/Linux/Display.hpp>
#include <SFML/System/Utf.hpp>
#include <SFML/System/Err.hpp>
#include <X11/Xutil.h>
#include <X11/keysym.h>
#include <X11/extensions/Xrandr.h>
#include <unistd.h>
#include <cstring>
#include <sstream>
#include <vector>
#include <string>
#include <iterator>


////////////////////////////////////////////////////////////
// Private data
////////////////////////////////////////////////////////////
namespace
{
    sf::priv::WindowImplX11* fullscreenWindow = NULL;
    unsigned long            eventMask        = FocusChangeMask | ButtonPressMask | ButtonReleaseMask | ButtonMotionMask |
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
m_previousSize(-1, -1)
{
    // Open a connection with the X server
    m_display = OpenDisplay();
    m_screen = DefaultScreen(m_display);

    // Save the window handle
    m_window = handle;

    if (m_window)
    {
        // Make sure the window is listening to all the requiered events
        XSelectInput(m_display, m_window, eventMask & ~ButtonPressMask);

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
m_previousSize(-1, -1)
{
    // Open a connection with the X server
    m_display = OpenDisplay();
    m_screen = DefaultScreen(m_display);
    ::Window root = RootWindow(m_display, m_screen);

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
    XVisualInfo visualInfo = GlxContext::selectBestVisual(m_display, mode.bitsPerPixel, settings);

    // Define the window attributes
    XSetWindowAttributes attributes;
    attributes.override_redirect = fullscreen;
    attributes.event_mask = eventMask;
    attributes.colormap = XCreateColormap(m_display, root, visualInfo.visual, AllocNone);

    // Create the window
    m_window = XCreateWindow(m_display,
                             root,
                             left, top,
                             width, height,
                             0,
                             visualInfo.depth,
                             InputOutput,
                             visualInfo.visual,
                             CWEventMask | CWOverrideRedirect | CWColormap, &attributes);
    if (!m_window)
    {
        err() << "Failed to create window" << std::endl;
        return;
    }

    // Set the window's name
    setTitle(title);

    // Set the window's style (tell the windows manager to change our window's decorations and functions according to the requested style)
    if (!fullscreen)
    {
        Atom WMHintsAtom = XInternAtom(m_display, "_MOTIF_WM_HINTS", false);
        if (WMHintsAtom)
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
                unsigned long flags;
                unsigned long functions;
                unsigned long decorations;
                long          inputMode;
                unsigned long state;
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
            XChangeProperty(m_display, m_window, WMHintsAtom, WMHintsAtom, 32, PropModeReplace, ptr, 5);
        }

        // This is a hack to force some windows managers to disable resizing
        if (!(style & Style::Resize))
        {
            XSizeHints* sizeHints = XAllocSizeHints();
            sizeHints->flags = PMinSize | PMaxSize;
            sizeHints->min_width = sizeHints->max_width  = width;
            sizeHints->min_height = sizeHints->max_height = height;
            XSetWMNormalHints(m_display, m_window, sizeHints); 
            XFree(sizeHints);
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
        XGrabPointer(m_display, m_window, true, 0, GrabModeAsync, GrabModeAsync, m_window, None, CurrentTime);
        XGrabKeyboard(m_display, m_window, true, GrabModeAsync, GrabModeAsync, CurrentTime);
    }
}


////////////////////////////////////////////////////////////
WindowImplX11::~WindowImplX11()
{
    // Cleanup graphical resources
    cleanup();

    // Destroy the cursor
    if (m_hiddenCursor)
        XFreeCursor(m_display, m_hiddenCursor);

    // Destroy the input context
    if (m_inputContext)
        XDestroyIC(m_inputContext);

    // Destroy the window
    if (m_window && !m_isExternal)
    {
        XDestroyWindow(m_display, m_window);
        XFlush(m_display);
    }

    // Close the input method
    if (m_inputMethod)
        XCloseIM(m_inputMethod);

    // Close the connection with the X server
    CloseDisplay(m_display);
}


////////////////////////////////////////////////////////////
WindowHandle WindowImplX11::getSystemHandle() const
{
    return m_window;
}


////////////////////////////////////////////////////////////
void WindowImplX11::processEvents()
{
    XEvent event;
    while (XCheckIfEvent(m_display, &event, &checkEvent, reinterpret_cast<XPointer>(m_window)))
    {
        processEvent(event);
    }
}


////////////////////////////////////////////////////////////
Vector2i WindowImplX11::getPosition() const
{
    ::Window root, child;
    int localX, localY, x, y;
    unsigned int width, height, border, depth;

    XGetGeometry(m_display, m_window, &root, &localX, &localY, &width, &height, &border, &depth);
    XTranslateCoordinates(m_display, m_window, root, localX, localY, &x, &y, &child);

    return Vector2i(x, y);
}


////////////////////////////////////////////////////////////
void WindowImplX11::setPosition(const Vector2i& position)
{
    XMoveWindow(m_display, m_window, position.x, position.y);
    XFlush(m_display);
}


////////////////////////////////////////////////////////////
Vector2u WindowImplX11::getSize() const
{
    XWindowAttributes attributes;
    XGetWindowAttributes(m_display, m_window, &attributes);
    return Vector2u(attributes.width, attributes.height);
}


////////////////////////////////////////////////////////////
void WindowImplX11::setSize(const Vector2u& size)
{
    XResizeWindow(m_display, m_window, size.x, size.y);
    XFlush(m_display);
}


////////////////////////////////////////////////////////////
void WindowImplX11::setTitle(const String& title)
{
    // Bare X11 has no Unicode window title support.
    // There is however an option to tell the window manager your unicode title via hints.
    
    // Convert to UTF-8 encoding.
    std::basic_string<Uint8> utf8Title;
    Utf32::toUtf8(title.begin(), title.end(), std::back_inserter(utf8Title));
    
    // Set the _NET_WM_NAME atom, which specifies a UTF-8 encoded window title.
    Atom wmName = XInternAtom(m_display, "_NET_WM_NAME", False);
    Atom useUtf8 = XInternAtom(m_display, "UTF8_STRING", False);
    XChangeProperty(m_display, m_window, wmName, useUtf8, 8,
                    PropModeReplace, utf8Title.c_str(), utf8Title.size());
    
    // Set the non-Unicode title as a fallback for window managers who don't support _NET_WM_NAME.
    XStoreName(m_display, m_window, title.toAnsiString().c_str());
}


////////////////////////////////////////////////////////////
void WindowImplX11::setIcon(unsigned int width, unsigned int height, const Uint8* pixels)
{
    // X11 wants BGRA pixels : swap red and blue channels
    // Note: this memory will be freed by XDestroyImage
    Uint8* iconPixels = static_cast<Uint8*>(std::malloc(width * height * 4));
    for (std::size_t i = 0; i < width * height; ++i)
    {
        iconPixels[i * 4 + 0] = pixels[i * 4 + 2];
        iconPixels[i * 4 + 1] = pixels[i * 4 + 1];
        iconPixels[i * 4 + 2] = pixels[i * 4 + 0];
        iconPixels[i * 4 + 3] = pixels[i * 4 + 3];
    }

    // Create the icon pixmap
    Visual*      defVisual = DefaultVisual(m_display, m_screen);
    unsigned int defDepth  = DefaultDepth(m_display, m_screen);
    XImage* iconImage = XCreateImage(m_display, defVisual, defDepth, ZPixmap, 0, (char*)iconPixels, width, height, 32, 0);
    if (!iconImage)
    {
        err() << "Failed to set the window's icon" << std::endl;
        return;
    }
    Pixmap iconPixmap = XCreatePixmap(m_display, RootWindow(m_display, m_screen), width, height, defDepth);
    XGCValues values;
    GC iconGC = XCreateGC(m_display, iconPixmap, 0, &values);
    XPutImage(m_display, iconPixmap, iconGC, iconImage, 0, 0, 0, 0, width, height);
    XFreeGC(m_display, iconGC);
    XDestroyImage(iconImage);

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
    Pixmap maskPixmap = XCreatePixmapFromBitmapData(m_display, m_window, (char*)&maskPixels[0], width, height, 1, 0, 1);

    // Send our new icon to the window through the WMHints
    XWMHints* hints = XAllocWMHints();
    hints->flags       = IconPixmapHint | IconMaskHint;
    hints->icon_pixmap = iconPixmap;
    hints->icon_mask   = maskPixmap;
    XSetWMHints(m_display, m_window, hints);
    XFree(hints);

    XFlush(m_display);
}


////////////////////////////////////////////////////////////
void WindowImplX11::setVisible(bool visible)
{
    if (visible)
        XMapWindow(m_display, m_window);
    else
        XUnmapWindow(m_display, m_window);

    XFlush(m_display);
}


////////////////////////////////////////////////////////////
void WindowImplX11::setMouseCursorVisible(bool visible)
{
    XDefineCursor(m_display, m_window, visible ? None : m_hiddenCursor);
    XFlush(m_display);
}


////////////////////////////////////////////////////////////
void WindowImplX11::setKeyRepeatEnabled(bool enabled)
{
    m_keyRepeat = enabled;
}


////////////////////////////////////////////////////////////
void WindowImplX11::switchToFullscreen(const VideoMode& mode)
{
    // Check if the XRandR extension is present
    int version;
    if (XQueryExtension(m_display, "RANDR", &version, &version, &version))
    {
        // Get the current configuration
        XRRScreenConfiguration* config = XRRGetScreenInfo(m_display, RootWindow(m_display, m_screen));
        if (config)
        {
            // Get the current rotation
            Rotation currentRotation;
            m_oldVideoMode = XRRConfigCurrentConfiguration(config, &currentRotation);

            // Get the available screen sizes
            int nbSizes;
            XRRScreenSize* sizes = XRRConfigSizes(config, &nbSizes);
            if (sizes && (nbSizes > 0))
            {
                // Search a matching size
                for (int i = 0; i < nbSizes; ++i)
                {
                    if ((sizes[i].width == static_cast<int>(mode.width)) && (sizes[i].height == static_cast<int>(mode.height)))
                    {
                        // Switch to fullscreen mode
                        XRRSetScreenConfig(m_display, config, RootWindow(m_display, m_screen), i, currentRotation, CurrentTime);

                        // Set "this" as the current fullscreen window
                        fullscreenWindow = this;
                        break;
                    }
                }
            }

            // Free the configuration instance
            XRRFreeScreenConfigInfo(config);
        }
        else
        {
            // Failed to get the screen configuration
            err() << "Failed to get the current screen configuration for fullscreen mode, switching to window mode" << std::endl;
        }
    }
    else
    {
        // XRandr extension is not supported : we cannot use fullscreen mode
        err() << "Fullscreen is not supported, switching to window mode" << std::endl;
    }
}


////////////////////////////////////////////////////////////
void WindowImplX11::initialize()
{
    // Get the atom defining the close event
    m_atomClose = XInternAtom(m_display, "WM_DELETE_WINDOW", false);
    XSetWMProtocols(m_display, m_window, &m_atomClose, 1);

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
    XMapWindow(m_display, m_window);
    XFlush(m_display);

    // Create the hiden cursor
    createHiddenCursor();

    // Flush the commands queue
    XFlush(m_display);
}


////////////////////////////////////////////////////////////
void WindowImplX11::createHiddenCursor()
{
    // Create the cursor's pixmap (1x1 pixels)
    Pixmap cursorPixmap = XCreatePixmap(m_display, m_window, 1, 1, 1);
    GC graphicsContext = XCreateGC(m_display, cursorPixmap, 0, NULL);
    XDrawPoint(m_display, cursorPixmap, graphicsContext, 0, 0);
    XFreeGC(m_display, graphicsContext);

    // Create the cursor, using the pixmap as both the shape and the mask of the cursor
    XColor color;
    color.flags = DoRed | DoGreen | DoBlue;
    color.red = color.blue = color.green = 0;
    m_hiddenCursor = XCreatePixmapCursor(m_display, cursorPixmap, cursorPixmap, &color, &color, 0, 0);

    // We don't need the pixmap any longer, free it
    XFreePixmap(m_display, cursorPixmap);
}


////////////////////////////////////////////////////////////
void WindowImplX11::cleanup()
{
    // Restore the previous video mode (in case we were running in fullscreen)
    if (fullscreenWindow == this)
    {
        // Get current screen info
        XRRScreenConfiguration* config = XRRGetScreenInfo(m_display, RootWindow(m_display, m_screen));
        if (config) 
        {
            // Get the current rotation
            Rotation currentRotation;
            XRRConfigCurrentConfiguration(config, &currentRotation);

            // Reset the video mode
            XRRSetScreenConfig(m_display, config, RootWindow(m_display, m_screen), m_oldVideoMode, currentRotation, CurrentTime);

            // Free the configuration instance
            XRRFreeScreenConfigInfo(config);
        } 

        // Reset the fullscreen window
        fullscreenWindow = NULL;
    }

    // Unhide the mouse cursor (in case it was hidden)
    setMouseCursorVisible(true);
}


////////////////////////////////////////////////////////////
bool WindowImplX11::processEvent(XEvent windowEvent)
{
    // This function implements a workaround to properly discard
    // repeated key events when necessary. The problem is that the
    // system's key events policy doesn't match SFML's one: X server will generate
    // both repeated KeyPress and KeyRelease events when maintaining a key down, while
    // SFML only wants repeated KeyPress events. Thus, we have to:
    // - Discard duplicated KeyRelease events when KeyRepeatEnabled is true
    // - Discard both duplicated KeyPress and KeyRelease events when KeyRepeatEnabled is false

    // Detect repeated key events
    // (code shamelessly taken from SDL)
    if (windowEvent.type == KeyRelease)
    {
        // Check if there's a matching KeyPress event in the queue
        XEvent nextEvent;
        if (XPending(m_display))
        {
            // Grab it but don't remove it from the queue, it still needs to be processed :)
            XPeekEvent(m_display, &nextEvent);
            if (nextEvent.type == KeyPress)
            {
                // Check if it is a duplicated event (same timestamp as the KeyRelease event)
                if ((nextEvent.xkey.keycode == windowEvent.xkey.keycode) &&
                    (nextEvent.xkey.time - windowEvent.xkey.time < 2))
                {
                    // If we don't want repeated events, remove the next KeyPress from the queue
                    if (!m_keyRepeat)
                        XNextEvent(m_display, &nextEvent);

                    // This KeyRelease is a repeated event and we don't want it
                    return false;
                }
            }
        }
    }

    // Convert the X11 event to a sf::Event
    switch (windowEvent.type)
    {
        // Destroy event
        case DestroyNotify :
        {
            // The window is about to be destroyed : we must cleanup resources
            cleanup();
            break;
        }

        // Gain focus event
        case FocusIn :
        {
            // Update the input context
            if (m_inputContext)
                XSetICFocus(m_inputContext);

            Event event;
            event.type = Event::GainedFocus;
            pushEvent(event);
            break;
        }

        // Lost focus event
        case FocusOut :
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
        case ConfigureNotify :
        {
            // ConfigureNotify can be triggered for other reasons, check if the size has acutally changed
            if ((windowEvent.xconfigure.width != m_previousSize.x) || (windowEvent.xconfigure.height != m_previousSize.y))
            {
                Event event;
                event.type        = Event::Resized;
                event.size.width  = windowEvent.xconfigure.width;
                event.size.height = windowEvent.xconfigure.height;
                pushEvent(event);

                m_previousSize.x = windowEvent.xconfigure.width;
                m_previousSize.y = windowEvent.xconfigure.height;
            }
            break;
        }

        // Close event
        case ClientMessage :
        {
            if ((windowEvent.xclient.format == 32) && (windowEvent.xclient.data.l[0]) == static_cast<long>(m_atomClose))  
            {
                Event event;
                event.type = Event::Closed;
                pushEvent(event);
            }
            break;
        }

        // Key down event
        case KeyPress :
        {
            // Get the keysym of the key that has been pressed
            static XComposeStatus keyboard;
            char buffer[32];
            KeySym symbol;
            XLookupString(&windowEvent.xkey, buffer, sizeof(buffer), &symbol, &keyboard);

            // Fill the event parameters
            // TODO: if modifiers are wrong, use XGetModifierMapping to retrieve the actual modifiers mapping
            Event event;
            event.type        = Event::KeyPressed;
            event.key.code    = keysymToSF(symbol);
            event.key.alt     = windowEvent.xkey.state & Mod1Mask;
            event.key.control = windowEvent.xkey.state & ControlMask;
            event.key.shift   = windowEvent.xkey.state & ShiftMask;
            event.key.system  = windowEvent.xkey.state & Mod4Mask;
            pushEvent(event);

            // Generate a TextEntered event
            if (!XFilterEvent(&windowEvent, None))
            {
                #ifdef X_HAVE_UTF8_STRING
                if (m_inputContext)
                {
                    Status status;
                    Uint8  keyBuffer[16];
                    int length = Xutf8LookupString(m_inputContext, &windowEvent.xkey, reinterpret_cast<char*>(keyBuffer), sizeof(keyBuffer), NULL, &status);
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
                    if (XLookupString(&windowEvent.xkey, keyBuffer, sizeof(keyBuffer), NULL, &status))
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
        case KeyRelease :
        {
            // Get the keysym of the key that has been pressed
            char buffer[32];
            KeySym symbol;
            XLookupString(&windowEvent.xkey, buffer, 32, &symbol, NULL);

            // Fill the event parameters
            Event event;
            event.type        = Event::KeyReleased;
            event.key.code    = keysymToSF(symbol);
            event.key.alt     = windowEvent.xkey.state & Mod1Mask;
            event.key.control = windowEvent.xkey.state & ControlMask;
            event.key.shift   = windowEvent.xkey.state & ShiftMask;
            event.key.system  = windowEvent.xkey.state & Mod4Mask;
            pushEvent(event);

            break;
        }

        // Mouse button pressed
        case ButtonPress :
        {
            unsigned int button = windowEvent.xbutton.button;
            if ((button == Button1) || (button == Button2) || (button == Button3) || (button == 8) || (button == 9))
            {
                Event event;
                event.type          = Event::MouseButtonPressed;
                event.mouseButton.x = windowEvent.xbutton.x;
                event.mouseButton.y = windowEvent.xbutton.y;
                switch (button)
                {
                    case Button1 : event.mouseButton.button = Mouse::Left;     break;
                    case Button2 : event.mouseButton.button = Mouse::Middle;   break;
                    case Button3 : event.mouseButton.button = Mouse::Right;    break;
                    case 8 :       event.mouseButton.button = Mouse::XButton1; break;
                    case 9 :       event.mouseButton.button = Mouse::XButton2; break;            
                }
                pushEvent(event);
            }
            break;
        }

        // Mouse button released
        case ButtonRelease :
        {
            unsigned int button = windowEvent.xbutton.button;
            if ((button == Button1) || (button == Button2) || (button == Button3) || (button == 8) || (button == 9))
            {
                Event event;
                event.type          = Event::MouseButtonReleased;
                event.mouseButton.x = windowEvent.xbutton.x;
                event.mouseButton.y = windowEvent.xbutton.y;
                switch (button)
                {
                    case Button1 : event.mouseButton.button = Mouse::Left;     break;
                    case Button2 : event.mouseButton.button = Mouse::Middle;   break;
                    case Button3 : event.mouseButton.button = Mouse::Right;    break;
                    case 8 :       event.mouseButton.button = Mouse::XButton1; break;
                    case 9 :       event.mouseButton.button = Mouse::XButton2; break;            
                }
                pushEvent(event);
            }
            else if ((button == Button4) || (button == Button5))
            {
                Event event;
                event.type             = Event::MouseWheelMoved;
                event.mouseWheel.delta = windowEvent.xbutton.button == Button4 ? 1 : -1;
                event.mouseWheel.x     = windowEvent.xbutton.x;
                event.mouseWheel.y     = windowEvent.xbutton.y;
                pushEvent(event);
            }
            break;
        }

        // Mouse moved
        case MotionNotify :
        {
            Event event;
            event.type        = Event::MouseMoved;
            event.mouseMove.x = windowEvent.xmotion.x;
            event.mouseMove.y = windowEvent.xmotion.y;
            pushEvent(event);
            break;
        }

        // Mouse entered
        case EnterNotify :
        {
            if (windowEvent.xcrossing.mode == NotifyNormal)
            {
                Event event;
                event.type = Event::MouseEntered;
                pushEvent(event);
            }
            break;
        }

        // Mouse left
        case LeaveNotify :
        {
            if (windowEvent.xcrossing.mode == NotifyNormal)
            {
                Event event;
                event.type = Event::MouseLeft;
                pushEvent(event);
            }
            break;
        }
    }

    return true;
}


////////////////////////////////////////////////////////////
Keyboard::Key WindowImplX11::keysymToSF(KeySym symbol)
{
    // First convert to uppercase (to avoid dealing with two different keysyms for the same key)
    KeySym lower, key;
    XConvertCase(symbol, &lower, &key);

    switch (key)
    {
        case XK_Shift_L :      return Keyboard::LShift;
        case XK_Shift_R :      return Keyboard::RShift;
        case XK_Control_L :    return Keyboard::LControl;
        case XK_Control_R :    return Keyboard::RControl;
        case XK_Alt_L :        return Keyboard::LAlt;
        case XK_Alt_R :        return Keyboard::RAlt;
        case XK_Super_L :      return Keyboard::LSystem;
        case XK_Super_R :      return Keyboard::RSystem;
        case XK_Menu :         return Keyboard::Menu;
        case XK_Escape :       return Keyboard::Escape;
        case XK_semicolon :    return Keyboard::SemiColon;
        case XK_slash :        return Keyboard::Slash;
        case XK_equal :        return Keyboard::Equal;
        case XK_minus :        return Keyboard::Dash;
        case XK_bracketleft :  return Keyboard::LBracket;
        case XK_bracketright : return Keyboard::RBracket;
        case XK_comma :        return Keyboard::Comma;
        case XK_period :       return Keyboard::Period;
        case XK_dead_acute :   return Keyboard::Quote;
        case XK_backslash :    return Keyboard::BackSlash;
        case XK_dead_grave :   return Keyboard::Tilde;
        case XK_space :        return Keyboard::Space;
        case XK_Return :       return Keyboard::Return;
        case XK_KP_Enter :     return Keyboard::Return;
        case XK_BackSpace :    return Keyboard::BackSpace;
        case XK_Tab :          return Keyboard::Tab;
        case XK_Prior :        return Keyboard::PageUp;
        case XK_Next :         return Keyboard::PageDown;
        case XK_End :          return Keyboard::End;
        case XK_Home :         return Keyboard::Home;
        case XK_Insert :       return Keyboard::Insert;
        case XK_Delete :       return Keyboard::Delete;
        case XK_KP_Add :       return Keyboard::Add;
        case XK_KP_Subtract :  return Keyboard::Subtract;
        case XK_KP_Multiply :  return Keyboard::Multiply;
        case XK_KP_Divide :    return Keyboard::Divide;
        case XK_Pause :        return Keyboard::Pause;
        case XK_F1 :           return Keyboard::F1;
        case XK_F2 :           return Keyboard::F2;
        case XK_F3 :           return Keyboard::F3;
        case XK_F4 :           return Keyboard::F4;
        case XK_F5 :           return Keyboard::F5;
        case XK_F6 :           return Keyboard::F6;
        case XK_F7 :           return Keyboard::F7;
        case XK_F8 :           return Keyboard::F8;
        case XK_F9 :           return Keyboard::F9;
        case XK_F10 :          return Keyboard::F10;
        case XK_F11 :          return Keyboard::F11;
        case XK_F12 :          return Keyboard::F12;
        case XK_F13 :          return Keyboard::F13;
        case XK_F14 :          return Keyboard::F14;
        case XK_F15 :          return Keyboard::F15;
        case XK_Left :         return Keyboard::Left;
        case XK_Right :        return Keyboard::Right;
        case XK_Up :           return Keyboard::Up;
        case XK_Down :         return Keyboard::Down;
        case XK_KP_0 :         return Keyboard::Numpad0;
        case XK_KP_1 :         return Keyboard::Numpad1;
        case XK_KP_2 :         return Keyboard::Numpad2;
        case XK_KP_3 :         return Keyboard::Numpad3;
        case XK_KP_4 :         return Keyboard::Numpad4;
        case XK_KP_5 :         return Keyboard::Numpad5;
        case XK_KP_6 :         return Keyboard::Numpad6;
        case XK_KP_7 :         return Keyboard::Numpad7;
        case XK_KP_8 :         return Keyboard::Numpad8;
        case XK_KP_9 :         return Keyboard::Numpad9;
        case XK_A :            return Keyboard::A;
        case XK_Z :            return Keyboard::Z;
        case XK_E :            return Keyboard::E;
        case XK_R :            return Keyboard::R;
        case XK_T :            return Keyboard::T;
        case XK_Y :            return Keyboard::Y;
        case XK_U :            return Keyboard::U;
        case XK_I :            return Keyboard::I;
        case XK_O :            return Keyboard::O;
        case XK_P :            return Keyboard::P;
        case XK_Q :            return Keyboard::Q;
        case XK_S :            return Keyboard::S;
        case XK_D :            return Keyboard::D;
        case XK_F :            return Keyboard::F;
        case XK_G :            return Keyboard::G;
        case XK_H :            return Keyboard::H;
        case XK_J :            return Keyboard::J;
        case XK_K :            return Keyboard::K;
        case XK_L :            return Keyboard::L;
        case XK_M :            return Keyboard::M;
        case XK_W :            return Keyboard::W;
        case XK_X :            return Keyboard::X;
        case XK_C :            return Keyboard::C;
        case XK_V :            return Keyboard::V;
        case XK_B :            return Keyboard::B;
        case XK_N :            return Keyboard::N;
        case XK_0 :            return Keyboard::Num0;
        case XK_1 :            return Keyboard::Num1;
        case XK_2 :            return Keyboard::Num2;
        case XK_3 :            return Keyboard::Num3;
        case XK_4 :            return Keyboard::Num4;
        case XK_5 :            return Keyboard::Num5;
        case XK_6 :            return Keyboard::Num6;
        case XK_7 :            return Keyboard::Num7;
        case XK_8 :            return Keyboard::Num8;
        case XK_9 :            return Keyboard::Num9;
    }

    return Keyboard::Unknown;
}

} // namespace priv

} // namespace sf
