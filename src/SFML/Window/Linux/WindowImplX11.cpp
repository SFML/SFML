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
#include <SFML/Window/WindowStyle.hpp> // important to be included first (conflict with None)
#include <SFML/Window/Linux/WindowImplX11.hpp>
#include <SFML/System/Utf.hpp>
#include <SFML/System/Err.hpp>
#include <X11/Xutil.h>
#include <X11/keysym.h>
#include <X11/extensions/Xrandr.h>
#include <sstream>
#include <vector>


////////////////////////////////////////////////////////////
// Private data
////////////////////////////////////////////////////////////
namespace
{
    sf::priv::WindowImplX11* fullscreenWindow = NULL;
    unsigned long            eventMask        = FocusChangeMask | ButtonPressMask | ButtonReleaseMask | ButtonMotionMask |
                                                PointerMotionMask | KeyPressMask | KeyReleaseMask | StructureNotifyMask |
                                                EnterWindowMask | LeaveWindowMask;

    /// Filter the events received by windows
    /// (only allow those matching a specific window)
    Bool CheckEvent(::Display*, XEvent* event, XPointer userData)
    {
        // Just check if the event matches the window
        return event->xany.window == reinterpret_cast< ::Window >(userData);
    }
}


namespace sf
{
namespace priv
{
////////////////////////////////////////////////////////////
WindowImplX11::WindowImplX11(WindowHandle handle) :
myWindow      (0),
myInputMethod (NULL),
myInputContext(NULL),
myIsExternal  (true),
myAtomClose   (0),
myOldVideoMode(-1),
myHiddenCursor(0),
myKeyRepeat   (true)
{
    // Open a connection with the X server
    myDisplay = XOpenDisplay(NULL);
    myScreen  = DefaultScreen(myDisplay);

    // Save the window handle
    myWindow = handle;

    if (myWindow)
    {
        // Get the window size
        XWindowAttributes windowAttributes;
        if (XGetWindowAttributes(myDisplay, myWindow, &windowAttributes) == 0)
        {
            Err() << "Failed to get the window attributes" << std::endl;
            return;
        }
        myWidth  = windowAttributes.width;
        myHeight = windowAttributes.height;

        // Make sure the window is listening to all the requiered events
        XSelectInput(myDisplay, myWindow, eventMask & ~ButtonPressMask);

        // Do some common initializations
        Initialize();
    }
}


////////////////////////////////////////////////////////////
WindowImplX11::WindowImplX11(VideoMode mode, const std::string& title, unsigned long style) :
myWindow      (0),
myInputMethod (NULL),
myInputContext(NULL),
myIsExternal  (false),
myAtomClose   (0),
myOldVideoMode(-1),
myHiddenCursor(0),
myKeyRepeat   (true)
{
    // Open a connection with the X server
    myDisplay = XOpenDisplay(NULL);
    myScreen  = DefaultScreen(myDisplay);

    // Compute position and size
    int left, top;
    bool fullscreen = (style & Style::Fullscreen) != 0;
    if (!fullscreen)
    {
        left = (DisplayWidth(myDisplay, myScreen)  - mode.Width)  / 2;
        top  = (DisplayHeight(myDisplay, myScreen) - mode.Height) / 2;
    }
    else
    {
        left = 0;
        top  = 0;
    }
    int width  = myWidth  = mode.Width;
    int height = myHeight = mode.Height;

    // Switch to fullscreen if necessary
    if (fullscreen)
        SwitchToFullscreen(mode);

    // Define the window attributes
    XSetWindowAttributes attributes;
    attributes.event_mask        = eventMask;
    attributes.override_redirect = fullscreen;

    // Create the window
    myWindow = XCreateWindow(myDisplay,
                             RootWindow(myDisplay, myScreen),
                             left, top,
                             width, height,
                             0,
                             DefaultDepth(myDisplay, myScreen),
                             InputOutput,
                             DefaultVisual(myDisplay, myScreen),
                             CWEventMask | CWOverrideRedirect, &attributes);
    if (!myWindow)
    {
        Err() << "Failed to create window" << std::endl;
        return;
    }

    // Set the window's name
    SetTitle(title);

    // Set the window's style (tell the windows manager to change our window's decorations and functions according to the requested style)
    if (!fullscreen)
    {
        Atom WMHintsAtom = XInternAtom(myDisplay, "_MOTIF_WM_HINTS", false);
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
                unsigned long Flags;
                unsigned long Functions;
                unsigned long Decorations;
                long          InputMode;
                unsigned long State;
            };
    
            WMHints hints;
            hints.Flags       = MWM_HINTS_FUNCTIONS | MWM_HINTS_DECORATIONS;
            hints.Decorations = 0;
            hints.Functions   = 0;

            if (style & Style::Titlebar)
            {
                hints.Decorations |= MWM_DECOR_BORDER | MWM_DECOR_TITLE | MWM_DECOR_MINIMIZE | MWM_DECOR_MENU;
                hints.Functions   |= MWM_FUNC_MOVE | MWM_FUNC_MINIMIZE;
            }
            if (style & Style::Resize)
            {
                hints.Decorations |= MWM_DECOR_MAXIMIZE | MWM_DECOR_RESIZEH;
                hints.Functions   |= MWM_FUNC_MAXIMIZE | MWM_FUNC_RESIZE;
            }
            if (style & Style::Close)
            {
                hints.Decorations |= 0;
                hints.Functions   |= MWM_FUNC_CLOSE;
            }

            const unsigned char* ptr = reinterpret_cast<const unsigned char*>(&hints);
            XChangeProperty(myDisplay, myWindow, WMHintsAtom, WMHintsAtom, 32, PropModeReplace, ptr, 5);
        }

        // This is a hack to force some windows managers to disable resizing
        if (!(style & Style::Resize))
        {
            XSizeHints sizeHints;
            sizeHints.flags      = PMinSize | PMaxSize;
            sizeHints.min_width  = sizeHints.max_width  = width;
            sizeHints.min_height = sizeHints.max_height = height;
            XSetWMNormalHints(myDisplay, myWindow, &sizeHints); 
        }
    }

    // Do some common initializations
    Initialize();

    // In fullscreen mode, we must grab keyboard and mouse inputs
    if (fullscreen)
    {
        XGrabPointer(myDisplay, myWindow, true, 0, GrabModeAsync, GrabModeAsync, myWindow, None, CurrentTime);
        XGrabKeyboard(myDisplay, myWindow, true, GrabModeAsync, GrabModeAsync, CurrentTime);
    }
}


////////////////////////////////////////////////////////////
WindowImplX11::~WindowImplX11()
{
    // Cleanup graphical resources
    CleanUp();

    // Destroy the cursor
    if (myHiddenCursor)
        XFreeCursor(myDisplay, myHiddenCursor);

    // Destroy the input context
    if (myInputContext)
        XDestroyIC(myInputContext);

    // Destroy the window
    if (myWindow && !myIsExternal)
    {
        XDestroyWindow(myDisplay, myWindow);
        XFlush(myDisplay);
    }

    // Close the input method
    if (myInputMethod)
        XCloseIM(myInputMethod);

    // Close the connection with the X server
    XCloseDisplay(myDisplay);
}


////////////////////////////////////////////////////////////
::Display* WindowImplX11::GetDisplay() const
{
    return myDisplay;
}


////////////////////////////////////////////////////////////
WindowHandle WindowImplX11::GetSystemHandle() const
{
    return myWindow;
}


////////////////////////////////////////////////////////////
void WindowImplX11::ProcessEvents(bool block)
{
    if (block)
    {
        // Blocking -- wait and process events in the event queue until a valid event is found
        XEvent event;
        do
        {
            XIfEvent(myDisplay, &event, &CheckEvent, reinterpret_cast<XPointer>(myWindow));
        }
        while (!ProcessEvent(event));
    }
    else
    {
        // Non-blocking -- process all events in the event queue
        XEvent event;
        while (XCheckIfEvent(myDisplay, &event, &CheckEvent, reinterpret_cast<XPointer>(myWindow)))
        {
            ProcessEvent(event);
        }
    }
}


////////////////////////////////////////////////////////////
void WindowImplX11::ShowMouseCursor(bool show)
{
    XDefineCursor(myDisplay, myWindow, show ? None : myHiddenCursor);
    XFlush(myDisplay);
}


////////////////////////////////////////////////////////////
void WindowImplX11::SetCursorPosition(unsigned int x, unsigned int y)
{
    XWarpPointer(myDisplay, None, myWindow, 0, 0, 0, 0, x, y);
    XFlush(myDisplay);
}


////////////////////////////////////////////////////////////
void WindowImplX11::SetPosition(int x, int y)
{
    XMoveWindow(myDisplay, myWindow, x, y);
    XFlush(myDisplay);
}


////////////////////////////////////////////////////////////
void WindowImplX11::SetSize(unsigned int width, unsigned int height)
{
    XResizeWindow(myDisplay, myWindow, width, height);
    XFlush(myDisplay);
}


////////////////////////////////////////////////////////////
void WindowImplX11::SetTitle(const std::string& title)
{
    XStoreName(myDisplay, myWindow, title.c_str());
}


////////////////////////////////////////////////////////////
void WindowImplX11::Show(bool show)
{
    if (show)
        XMapWindow(myDisplay, myWindow);
    else
        XUnmapWindow(myDisplay, myWindow);

    XFlush(myDisplay);
}


////////////////////////////////////////////////////////////
void WindowImplX11::EnableKeyRepeat(bool enabled)
{
    myKeyRepeat = enabled;
}


////////////////////////////////////////////////////////////
void WindowImplX11::SetIcon(unsigned int width, unsigned int height, const Uint8* pixels)
{
    // X11 wants BGRA pixels : swap red and blue channels
    // Note : this memory will never be freed, but it seems to cause a bug on exit if I do so
    Uint8* iconPixels = new Uint8[width * height * 4];
    for (std::size_t i = 0; i < width * height; ++i)
    {
        iconPixels[i * 4 + 0] = pixels[i * 4 + 2];
        iconPixels[i * 4 + 1] = pixels[i * 4 + 1];
        iconPixels[i * 4 + 2] = pixels[i * 4 + 0];
        iconPixels[i * 4 + 3] = pixels[i * 4 + 3];
    }

    // Create the icon pixmap
    Visual*      defVisual = DefaultVisual(myDisplay, myScreen);
    unsigned int defDepth  = DefaultDepth(myDisplay, myScreen);
    XImage* iconImage = XCreateImage(myDisplay, defVisual, defDepth, ZPixmap, 0, (char*)iconPixels, width, height, 32, 0);
    if (!iconImage)
    {
        Err() << "Failed to set the window's icon" << std::endl;
        return;
    }
    Pixmap iconPixmap = XCreatePixmap(myDisplay, RootWindow(myDisplay, myScreen), width, height, defDepth);
    XGCValues values;
    GC iconGC = XCreateGC(myDisplay, iconPixmap, 0, &values);
    XPutImage(myDisplay, iconPixmap, iconGC, iconImage, 0, 0, 0, 0, width, height);
    XFreeGC(myDisplay, iconGC);
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
    Pixmap maskPixmap = XCreatePixmapFromBitmapData(myDisplay, myWindow, (char*)&maskPixels[0], width, height, 1, 0, 1);

    // Send our new icon to the window through the WMHints
    XWMHints* hints = XAllocWMHints();
    hints->flags       = IconPixmapHint | IconMaskHint;
    hints->icon_pixmap = iconPixmap;
    hints->icon_mask   = maskPixmap;
    XSetWMHints(myDisplay, myWindow, hints);
    XFree(hints);

    XFlush(myDisplay);
}


////////////////////////////////////////////////////////////
void WindowImplX11::SwitchToFullscreen(const VideoMode& mode)
{
    // Check if the XRandR extension is present
    int version;
    if (XQueryExtension(myDisplay, "RANDR", &version, &version, &version))
    {
        // Get the current configuration
        XRRScreenConfiguration* config = XRRGetScreenInfo(myDisplay, RootWindow(myDisplay, myScreen));
        if (config)
        {
            // Get the current rotation
            Rotation currentRotation;
            myOldVideoMode = XRRConfigCurrentConfiguration(config, &currentRotation);

            // Get the available screen sizes
            int nbSizes;
            XRRScreenSize* sizes = XRRConfigSizes(config, &nbSizes);
            if (sizes && (nbSizes > 0))
            {
                // Search a matching size
                for (int i = 0; i < nbSizes; ++i)
                {
                    if ((sizes[i].width == static_cast<int>(mode.Width)) && (sizes[i].height == static_cast<int>(mode.Height)))
                    {
                        // Switch to fullscreen mode
                        XRRSetScreenConfig(myDisplay, config, RootWindow(myDisplay, myScreen), i, currentRotation, CurrentTime);

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
            Err() << "Failed to get the current screen configuration for fullscreen mode, switching to window mode" << std::endl;
        }
    }
    else
    {
        // XRandr extension is not supported : we cannot use fullscreen mode
        Err() << "Fullscreen is not supported, switching to window mode" << std::endl;
    }
}


////////////////////////////////////////////////////////////
void WindowImplX11::Initialize()
{
    // Make sure the "last key release" is initialized with invalid values
    myLastKeyReleaseEvent.type = -1;

    // Get the atom defining the close event
    myAtomClose = XInternAtom(myDisplay, "WM_DELETE_WINDOW", false);
    XSetWMProtocols(myDisplay, myWindow, &myAtomClose, 1);

    // Create the input context
    myInputMethod = XOpenIM(myDisplay, NULL, NULL, NULL);
    if (myInputMethod)
    {
        myInputContext = XCreateIC(myInputMethod,
                                   XNClientWindow, myWindow,
                                   XNFocusWindow,  myWindow,
                                   XNInputStyle,   XIMPreeditNothing  | XIMStatusNothing,
                                   NULL);
    }
    else
    {
        myInputContext = NULL;
    }
    if (!myInputContext)
        Err() << "Failed to create input context for window -- TextEntered event won't be able to return unicode" << std::endl;

    // Show the window
    XMapWindow(myDisplay, myWindow);
    XFlush(myDisplay);

    // Create the hiden cursor
    CreateHiddenCursor();

    // Flush the commands queue
    XFlush(myDisplay);
}


////////////////////////////////////////////////////////////
void WindowImplX11::CreateHiddenCursor()
{
    // Create the cursor's pixmap (1x1 pixels)
    Pixmap cursorPixmap = XCreatePixmap(myDisplay, myWindow, 1, 1, 1);
    GC graphicsContext = XCreateGC(myDisplay, cursorPixmap, 0, NULL);
    XDrawPoint(myDisplay, cursorPixmap, graphicsContext, 0, 0);
    XFreeGC(myDisplay, graphicsContext);

    // Create the cursor, using the pixmap as both the shape and the mask of the cursor
    XColor color;
    color.flags = DoRed | DoGreen | DoBlue;
    color.red = color.blue = color.green = 0;
    myHiddenCursor = XCreatePixmapCursor(myDisplay, cursorPixmap, cursorPixmap, &color, &color, 0, 0);

    // We don't need the pixmap any longer, free it
    XFreePixmap(myDisplay, cursorPixmap);
}


////////////////////////////////////////////////////////////
void WindowImplX11::CleanUp()
{
    // Restore the previous video mode (in case we were running in fullscreen)
    if (fullscreenWindow == this)
    {
        // Get current screen info
        XRRScreenConfiguration* config = XRRGetScreenInfo(myDisplay, RootWindow(myDisplay, myScreen));
        if (config) 
        {
            // Get the current rotation
            Rotation currentRotation;
            XRRConfigCurrentConfiguration(config, &currentRotation);

            // Reset the video mode
            XRRSetScreenConfig(myDisplay, config, RootWindow(myDisplay, myScreen), myOldVideoMode, currentRotation, CurrentTime);

            // Free the configuration instance
            XRRFreeScreenConfigInfo(config);
        } 

        // Reset the fullscreen window
        fullscreenWindow = NULL;
    }

    // Unhide the mouse cursor (in case it was hidden)
    ShowMouseCursor(true);
}


////////////////////////////////////////////////////////////
bool WindowImplX11::ProcessEvent(XEvent windowEvent)
{
    // This function implements a workaround to properly discard
    // repeated key events when necessary. The problem is that the
    // system's key events policy doesn't match SFML's one: X server will generate
    // both repeated KeyPress and KeyRelease events when maintaining a key down, while
    // SFML only wants repeated KeyPress events. Thus, we have to:
    // - Discard duplicated KeyRelease events when EnableKeyRepeat is true
    // - Discard both duplicated KeyPress and KeyRelease events when EnableKeyRepeat is false

    // Detect repeated key events
    if ((windowEvent.type == KeyPress) || (windowEvent.type == KeyRelease))
    {
        if (windowEvent.xkey.keycode < 256)
        {
            // To detect if it is a repeated key event, we check the current state of the key.
            // - If the state is "down", KeyReleased events must obviously be discarded.
            // - KeyPress events are a little bit harder to handle: they depend on the EnableKeyRepeat state,
            //   and we need to properly forward the first one.
            char keys[32];
            XQueryKeymap(myDisplay, keys);
            if (keys[windowEvent.xkey.keycode >> 3] & (1 << (windowEvent.xkey.keycode % 8)))
            {
                // KeyRelease event + key down = repeated event --> discard
                if (windowEvent.type == KeyRelease)
                {
                    myLastKeyReleaseEvent = windowEvent;
                    return false;
                }

                // KeyPress event + key repeat disabled + matching KeyRelease event = repeated event --> discard
                if ((windowEvent.type == KeyPress) && !myKeyRepeat &&
                    (myLastKeyReleaseEvent.xkey.keycode == windowEvent.xkey.keycode) &&
                    (myLastKeyReleaseEvent.xkey.time == windowEvent.xkey.time))
                {
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
            CleanUp();
            break;
        }

        // Gain focus event
        case FocusIn :
        {
            // Update the input context
            if (myInputContext)
                XSetICFocus(myInputContext);

            Event event;
            event.Type = Event::GainedFocus;
            PushEvent(event);
            break;
        }

        // Lost focus event
        case FocusOut :
        {
            // Update the input context
            if (myInputContext)
                XUnsetICFocus(myInputContext);

            Event event;
            event.Type = Event::LostFocus;
            PushEvent(event);
            break;
        }

        // Resize event
        case ConfigureNotify :
        {
            if ((windowEvent.xconfigure.width != static_cast<int>(myWidth)) || (windowEvent.xconfigure.height != static_cast<int>(myHeight)))
            {
                myWidth  = windowEvent.xconfigure.width;
                myHeight = windowEvent.xconfigure.height;

                Event event;
                event.Type        = Event::Resized;
                event.Size.Width  = myWidth;
                event.Size.Height = myHeight;
                PushEvent(event);
            }
            break;
        }

        // Close event
        case ClientMessage :
        {
            if ((windowEvent.xclient.format == 32) && (windowEvent.xclient.data.l[0]) == static_cast<long>(myAtomClose))  
            {
                Event event;
                event.Type = Event::Closed;
                PushEvent(event);
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
            Event event;
            event.Type        = Event::KeyPressed;
            event.Key.Code    = KeysymToSF(symbol);
            event.Key.Alt     = windowEvent.xkey.state & Mod1Mask;
            event.Key.Control = windowEvent.xkey.state & ControlMask;
            event.Key.Shift   = windowEvent.xkey.state & ShiftMask;
            PushEvent(event);

            // Generate a TextEntered event
            if (!XFilterEvent(&windowEvent, None))
            {
                #ifdef X_HAVE_UTF8_STRING
                if (myInputContext)
                {
                    Status status;
                    Uint8  keyBuffer[16];
                    int length = Xutf8LookupString(myInputContext, &windowEvent.xkey, reinterpret_cast<char*>(keyBuffer), sizeof(keyBuffer), NULL, &status);
                    if (length > 0)
                    {
                        Uint32 unicode = 0;
                        Utf8::Decode(keyBuffer, keyBuffer + length, unicode, 0);
                        if (unicode != 0)
                        {
                            Event textEvent;
                            textEvent.Type         = Event::TextEntered;
                            textEvent.Text.Unicode = unicode;
                            PushEvent(textEvent);
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
                        textEvent.Type         = Event::TextEntered;
                        textEvent.Text.Unicode = static_cast<Uint32>(keyBuffer[0]);
                        PushEvent(textEvent);
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
            event.Type        = Event::KeyReleased;
            event.Key.Code    = KeysymToSF(symbol);
            event.Key.Alt     = windowEvent.xkey.state & Mod1Mask;
            event.Key.Control = windowEvent.xkey.state & ControlMask;
            event.Key.Shift   = windowEvent.xkey.state & ShiftMask;
            PushEvent(event);

            break;
        }

        // Mouse button pressed
        case ButtonPress :
        {
            unsigned int button = windowEvent.xbutton.button;
            if ((button == Button1) || (button == Button2) || (button == Button3) || (button == 8) || (button == 9))
            {
                Event event;
                event.Type          = Event::MouseButtonPressed;
                event.MouseButton.X = windowEvent.xbutton.x;
                event.MouseButton.Y = windowEvent.xbutton.y;
                switch (button)
                {
                    case Button1 : event.MouseButton.Button = Mouse::Left;     break;
                    case Button2 : event.MouseButton.Button = Mouse::Middle;   break;
                    case Button3 : event.MouseButton.Button = Mouse::Right;    break;
                    case 8 :       event.MouseButton.Button = Mouse::XButton1; break;
                    case 9 :       event.MouseButton.Button = Mouse::XButton2; break;            
                }
                PushEvent(event);
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
                event.Type          = Event::MouseButtonReleased;
                event.MouseButton.X = windowEvent.xbutton.x;
                event.MouseButton.Y = windowEvent.xbutton.y;
                switch (button)
                {
                    case Button1 : event.MouseButton.Button = Mouse::Left;     break;
                    case Button2 : event.MouseButton.Button = Mouse::Middle;   break;
                    case Button3 : event.MouseButton.Button = Mouse::Right;    break;
                    case 8 :       event.MouseButton.Button = Mouse::XButton1; break;
                    case 9 :       event.MouseButton.Button = Mouse::XButton2; break;            
                }
                PushEvent(event);
            }
            else if ((button == Button4) || (button == Button5))
            {
                Event event;
                event.Type             = Event::MouseWheelMoved;
                event.MouseWheel.Delta = windowEvent.xbutton.button == Button4 ? 1 : -1;
                event.MouseWheel.X     = windowEvent.xbutton.x;
                event.MouseWheel.Y     = windowEvent.xbutton.y;
                PushEvent(event);
            }
            break;
        }

        // Mouse moved
        case MotionNotify :
        {
            Event event;
            event.Type        = Event::MouseMoved;
            event.MouseMove.X = windowEvent.xmotion.x;
            event.MouseMove.Y = windowEvent.xmotion.y;
            PushEvent(event);
            break;
        }

        // Mouse entered
        case EnterNotify :
        {
            Event event;
            event.Type = Event::MouseEntered;
            PushEvent(event);
            break;
        }

        // Mouse left
        case LeaveNotify :
        {
            Event event;
            event.Type = Event::MouseLeft;
            PushEvent(event);
            break;
        }
    }

    return true;
}


////////////////////////////////////////////////////////////
Key::Code WindowImplX11::KeysymToSF(KeySym symbol)
{
    // First convert to uppercase (to avoid dealing with two different keysyms for the same key)
    KeySym lower, key;
    XConvertCase(symbol, &lower, &key);

    switch (key)
    {
        case XK_Shift_L :      return Key::LShift;
        case XK_Shift_R :      return Key::RShift;
        case XK_Control_L :    return Key::LControl;
        case XK_Control_R :    return Key::RControl;
        case XK_Alt_L :        return Key::LAlt;
        case XK_Alt_R :        return Key::RAlt;
        case XK_Super_L :      return Key::LSystem;
        case XK_Super_R :      return Key::RSystem;
        case XK_Menu :         return Key::Menu;
        case XK_Escape :       return Key::Escape;
        case XK_semicolon :    return Key::SemiColon;
        case XK_slash :        return Key::Slash;
        case XK_equal :        return Key::Equal;
        case XK_minus :        return Key::Dash;
        case XK_bracketleft :  return Key::LBracket;
        case XK_bracketright : return Key::RBracket;
        case XK_comma :        return Key::Comma;
        case XK_period :       return Key::Period;
        case XK_dead_acute :   return Key::Quote;
        case XK_backslash :    return Key::BackSlash;
        case XK_dead_grave :   return Key::Tilde;
        case XK_space :        return Key::Space;
        case XK_Return :       return Key::Return;
        case XK_KP_Enter :     return Key::Return;
        case XK_BackSpace :    return Key::Back;
        case XK_Tab :          return Key::Tab;
        case XK_Prior :        return Key::PageUp;
        case XK_Next :         return Key::PageDown;
        case XK_End :          return Key::End;
        case XK_Home :         return Key::Home;
        case XK_Insert :       return Key::Insert;
        case XK_Delete :       return Key::Delete;
        case XK_KP_Add :       return Key::Add;
        case XK_KP_Subtract :  return Key::Subtract;
        case XK_KP_Multiply :  return Key::Multiply;
        case XK_KP_Divide :    return Key::Divide;
        case XK_Pause :        return Key::Pause;
        case XK_F1 :           return Key::F1;
        case XK_F2 :           return Key::F2;
        case XK_F3 :           return Key::F3;
        case XK_F4 :           return Key::F4;
        case XK_F5 :           return Key::F5;
        case XK_F6 :           return Key::F6;
        case XK_F7 :           return Key::F7;
        case XK_F8 :           return Key::F8;
        case XK_F9 :           return Key::F9;
        case XK_F10 :          return Key::F10;
        case XK_F11 :          return Key::F11;
        case XK_F12 :          return Key::F12;
        case XK_F13 :          return Key::F13;
        case XK_F14 :          return Key::F14;
        case XK_F15 :          return Key::F15;
        case XK_Left :         return Key::Left;
        case XK_Right :        return Key::Right;
        case XK_Up :           return Key::Up;
        case XK_Down :         return Key::Down;
        case XK_KP_0 :         return Key::Numpad0;
        case XK_KP_1 :         return Key::Numpad1;
        case XK_KP_2 :         return Key::Numpad2;
        case XK_KP_3 :         return Key::Numpad3;
        case XK_KP_4 :         return Key::Numpad4;
        case XK_KP_5 :         return Key::Numpad5;
        case XK_KP_6 :         return Key::Numpad6;
        case XK_KP_7 :         return Key::Numpad7;
        case XK_KP_8 :         return Key::Numpad8;
        case XK_KP_9 :         return Key::Numpad9;
        case XK_A :            return Key::A;
        case XK_Z :            return Key::Z;
        case XK_E :            return Key::E;
        case XK_R :            return Key::R;
        case XK_T :            return Key::T;
        case XK_Y :            return Key::Y;
        case XK_U :            return Key::U;
        case XK_I :            return Key::I;
        case XK_O :            return Key::O;
        case XK_P :            return Key::P;
        case XK_Q :            return Key::Q;
        case XK_S :            return Key::S;
        case XK_D :            return Key::D;
        case XK_F :            return Key::F;
        case XK_G :            return Key::G;
        case XK_H :            return Key::H;
        case XK_J :            return Key::J;
        case XK_K :            return Key::K;
        case XK_L :            return Key::L;
        case XK_M :            return Key::M;
        case XK_W :            return Key::W;
        case XK_X :            return Key::X;
        case XK_C :            return Key::C;
        case XK_V :            return Key::V;
        case XK_B :            return Key::B;
        case XK_N :            return Key::N;
        case XK_0 :            return Key::Num0;
        case XK_1 :            return Key::Num1;
        case XK_2 :            return Key::Num2;
        case XK_3 :            return Key::Num3;
        case XK_4 :            return Key::Num4;
        case XK_5 :            return Key::Num5;
        case XK_6 :            return Key::Num6;
        case XK_7 :            return Key::Num7;
        case XK_8 :            return Key::Num8;
        case XK_9 :            return Key::Num9;
    }

    return Key::Code(0);
}

} // namespace priv

} // namespace sf
