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
#include <SFML/Window/glext/glxext.h>
#include <SFML/Window/glext/glext.h>
#include <SFML/System/Unicode.hpp>
#include <X11/keysym.h>
#include <X11/extensions/Xrandr.h>
#include <iostream>
#include <sstream>
#include <vector>


namespace
{
    ////////////////////////////////////////////////////////////
    /// Filter the events received by windows
    /// (only allow those matching a specific window)
    ////////////////////////////////////////////////////////////
    Bool CheckEvent(::Display*, XEvent* Event, XPointer UserData)
    {
        // Just check if the event matches the window
        return Event->xany.window == reinterpret_cast< ::Window >(UserData);
    }
}

namespace sf
{
namespace priv
{
////////////////////////////////////////////////////////////
// Static member data
////////////////////////////////////////////////////////////
Display*       WindowImplX11::ourDisplay          = NULL;
int            WindowImplX11::ourScreen           = 0;
WindowImplX11* WindowImplX11::ourFullscreenWindow = NULL;
unsigned int   WindowImplX11::ourWindowsCount     = 0;
XIM            WindowImplX11::ourInputMethod      = NULL;
unsigned long  WindowImplX11::ourEventMask        = FocusChangeMask | ButtonPressMask | ButtonReleaseMask | ButtonMotionMask |
                                                    PointerMotionMask | KeyPressMask | KeyReleaseMask | StructureNotifyMask |
                                                    EnterWindowMask | LeaveWindowMask;


////////////////////////////////////////////////////////////
/// Default constructor
/// (creates a dummy window to provide a valid OpenGL context)
////////////////////////////////////////////////////////////
WindowImplX11::WindowImplX11() :
myWindow      (0),
myIsExternal  (false),
myGLContext   (NULL),
myAtomClose   (0),
myOldVideoMode(-1),
myHiddenCursor(0),
myInputContext(NULL),
myKeyRepeat   (true)
{
    // Open the display at first call
    if (!OpenDisplay())
        return;

    // Use small dimensions
    myWidth  = 1;
    myHeight = 1;

    // Create the rendering context
    XVisualInfo Visual;
    WindowSettings Params(0, 0, 0);
    if (!CreateContext(VideoMode(myWidth, myHeight, 32), Visual, Params))
        return;

    // Create a new color map with the chosen visual
    Colormap ColMap = XCreateColormap(ourDisplay, RootWindow(ourDisplay, ourScreen), Visual.visual, AllocNone);

    // Define the window attributes
    XSetWindowAttributes Attributes;
    Attributes.colormap = ColMap;

    // Create a dummy window (disabled and hidden)
    myWindow = XCreateWindow(ourDisplay,
                             RootWindow(ourDisplay, ourScreen),
                             0, 0,
                             myWidth, myHeight,
                             0,
                             Visual.depth,
                             InputOutput,
                             Visual.visual,
                             CWColormap, &Attributes);

    // Don't activate the dummy context by default
}


////////////////////////////////////////////////////////////
/// Create the window implementation from an existing control
////////////////////////////////////////////////////////////
WindowImplX11::WindowImplX11(WindowHandle Handle, WindowSettings& Params) :
myWindow      (0),
myIsExternal  (true),
myGLContext   (NULL),
myAtomClose   (0),
myOldVideoMode(-1),
myHiddenCursor(0),
myInputContext(NULL),
myKeyRepeat   (true)
{
    // Open the display at first call
    if (!OpenDisplay())
        return;

    // Save the window handle
    myWindow = Handle;

    if (myWindow)
    {
        // Get the window size
        XWindowAttributes WindowAttributes;
        if (XGetWindowAttributes(ourDisplay, myWindow, &WindowAttributes) == 0)
        {
            std::cerr << "Failed to get the window attributes" << std::endl;
            return;
        }
        myWidth  = WindowAttributes.width;
        myHeight = WindowAttributes.height;

        // Setup the visual infos to match
        XVisualInfo Template;
        Template.depth     = WindowAttributes.depth;
        Template.visualid  = XVisualIDFromVisual(WindowAttributes.visual);
        unsigned long Mask = VisualDepthMask | VisualIDMask;

        // Create the rendering context
        VideoMode Mode(myWidth, myHeight, VideoMode::GetDesktopMode().BitsPerPixel);
        XVisualInfo Visual;
        if (!CreateContext(Mode, Visual, Params, Template, Mask))
            return;

        // Create a new color map with the chosen visual
        Colormap ColMap = XCreateColormap(ourDisplay, RootWindow(ourDisplay, ourScreen), Visual.visual, AllocNone);
        XSetWindowColormap(ourDisplay, myWindow, ColMap);

        // Make sure the window is listening to all the requiered events
        XSelectInput(ourDisplay, myWindow, ourEventMask & ~ButtonPressMask);

        // Do some common initializations
        Initialize();
    }
}


////////////////////////////////////////////////////////////
/// Create the window implementation
////////////////////////////////////////////////////////////
WindowImplX11::WindowImplX11(VideoMode Mode, const std::string& Title, unsigned long WindowStyle, WindowSettings& Params) :
myWindow      (0),
myIsExternal  (false),
myGLContext   (NULL),
myAtomClose   (0),
myOldVideoMode(-1),
myHiddenCursor(0),
myInputContext(NULL),
myKeyRepeat   (true)
{
    // Open the display at first call
    if (!OpenDisplay())
        return;

    // Compute position and size
    int Left, Top;
    bool Fullscreen = (WindowStyle & Style::Fullscreen) != 0;
    if (!Fullscreen)
    {
        Left = (DisplayWidth(ourDisplay, ourScreen)  - Mode.Width)  / 2;
        Top  = (DisplayHeight(ourDisplay, ourScreen) - Mode.Height) / 2;
    }
    else
    {
        Left = 0;
        Top  = 0;
    }
    int Width  = myWidth  = Mode.Width;
    int Height = myHeight = Mode.Height;

    // Switch to fullscreen if necessary
    if (Fullscreen)
        SwitchToFullscreen(Mode);

    // Create the rendering context
    XVisualInfo Visual;
    if (!CreateContext(Mode, Visual, Params))
        return;

    // Create a new color map with the chosen visual
    Colormap ColMap = XCreateColormap(ourDisplay, RootWindow(ourDisplay, ourScreen), Visual.visual, AllocNone);

    // Define the window attributes
    XSetWindowAttributes Attributes;
    Attributes.event_mask        = ourEventMask;
    Attributes.colormap          = ColMap;
    Attributes.override_redirect = Fullscreen;

    // Create the window
    myWindow = XCreateWindow(ourDisplay,
                             RootWindow(ourDisplay, ourScreen),
                             Left, Top,
                             Width, Height,
                             0,
                             Visual.depth,
                             InputOutput,
                             Visual.visual,
                             CWEventMask | CWColormap | CWOverrideRedirect, &Attributes);
    if (!myWindow)
    {
        std::cerr << "Failed to create window" << std::endl;
        return;
    }

    // Set the window's name
    XStoreName(ourDisplay, myWindow, Title.c_str());

    // Set the window's style (tell the windows manager to change our window's decorations and functions according to the requested style)
    if (!Fullscreen)
    {
        Atom WMHintsAtom = XInternAtom(ourDisplay, "_MOTIF_WM_HINTS", false);
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
    
            WMHints Hints;
            Hints.Flags       = MWM_HINTS_FUNCTIONS | MWM_HINTS_DECORATIONS;
            Hints.Decorations = 0;
            Hints.Functions   = 0;

            if (WindowStyle & Style::Titlebar)
            {
                Hints.Decorations |= MWM_DECOR_BORDER | MWM_DECOR_TITLE | MWM_DECOR_MINIMIZE | MWM_DECOR_MENU;
                Hints.Functions   |= MWM_FUNC_MOVE | MWM_FUNC_MINIMIZE;
            }
            if (WindowStyle & Style::Resize)
            {
                Hints.Decorations |= MWM_DECOR_MAXIMIZE | MWM_DECOR_RESIZEH;
                Hints.Functions   |= MWM_FUNC_MAXIMIZE | MWM_FUNC_RESIZE;
            }
            if (WindowStyle & Style::Close)
            {
                Hints.Decorations |= 0;
                Hints.Functions   |= MWM_FUNC_CLOSE;
            }

            const unsigned char* HintsPtr = reinterpret_cast<const unsigned char*>(&Hints);
            XChangeProperty(ourDisplay, myWindow, WMHintsAtom, WMHintsAtom, 32, PropModeReplace, HintsPtr, 5);
        }

        // This is a hack to force some windows managers to disable resizing
        if (!(WindowStyle & Style::Resize))
        {
            XSizeHints XSizeHints;
            XSizeHints.flags      = PMinSize | PMaxSize;
            XSizeHints.min_width  = XSizeHints.max_width  = Width;
            XSizeHints.min_height = XSizeHints.max_height = Height;
            XSetWMNormalHints(ourDisplay, myWindow, &XSizeHints); 
        }
    }

    // Do some common initializations
    Initialize();

    // In fullscreen mode, we must grab keyboard and mouse inputs
    if (Fullscreen)
    {
        XGrabPointer(ourDisplay, myWindow, true, 0, GrabModeAsync, GrabModeAsync, myWindow, None, CurrentTime);
        XGrabKeyboard(ourDisplay, myWindow, true, GrabModeAsync, GrabModeAsync, CurrentTime);
    }
}


////////////////////////////////////////////////////////////
/// Destructor
////////////////////////////////////////////////////////////
WindowImplX11::~WindowImplX11()
{
    // Cleanup graphical resources
    CleanUp();

    // Destroy the input context
    if (myInputContext)
    {
        XDestroyIC(myInputContext);
    }

    // Destroy the window
    if (myWindow && !myIsExternal)
    {
        XDestroyWindow(ourDisplay, myWindow);
        XFlush(ourDisplay);
    }

    // Close the display
    CloseDisplay();
}


////////////////////////////////////////////////////////////
/// Check if there's an active context on the current thread
////////////////////
////////////////////////////////////////////////////////////
/// Filter the received events
/// (only allow those matching a specific window)
////////////////////////////////////////////////////////////
Bool CheckEvent(::Display*, XEvent* Event, XPointer UserData)
{
    // Just check if the event matches our window
    return Event->xany.window == reinterpret_cast< ::Window >(UserData);
}////////////////////////////////////////
bool WindowImplX11::IsContextActive()
{
    return glXGetCurrentContext() != NULL;
}


////////////////////////////////////////////////////////////
/// /see WindowImpl::Display
////////////////////////////////////////////////////////////
void WindowImplX11::Display()
{
    if (myWindow && myGLContext)
        glXSwapBuffers(ourDisplay, myWindow);
}


////////////////////////////////////////////////////////////
/// /see WindowImpl::ProcessEvents
////////////////////////////////////////////////////////////
void WindowImplX11::ProcessEvents()
{
    // This function implements a workaround to properly discard
    // repeated key events when necessary. The problem is that the
    // system's key events policy doesn't match SFML's one: X server will generate
    // both repeated KeyPress and KeyRelease events when maintaining a key down, while
    // SFML only wants repeated KeyPress events. Thus, we have to:
    // - Discard duplicated KeyRelease events when EnableKeyRepeat is true
    // - Discard both duplicated KeyPress and KeyRelease events when EnableKeyRepeat is false


    // Process any event in the queue matching our window
    XEvent Event;
    while (XCheckIfEvent(ourDisplay, &Event, &CheckEvent, reinterpret_cast<XPointer>(myWindow)))
    {
        // Detect repeated key events
        if ((Event.type == KeyPress) || (Event.type == KeyRelease))
        {
            if (Event.xkey.keycode < 256)
            {
                // To detect if it is a repeated key event, we check the current state of the key.
                // - If the state is "down", KeyReleased events must obviously be discarded.
                // - KeyPress events are a little bit harder to handle: they depend on the EnableKeyRepeat state,
                //   and we need to properly forward the first one.
                char Keys[32];
                XQueryKeymap(ourDisplay, Keys);
                if (Keys[Event.xkey.keycode >> 3] & (1 << (Event.xkey.keycode % 8)))
                {
                    // KeyRelease event + key down = repeated event --> discard
                    if (Event.type == KeyRelease)
                    {
                        myLastKeyReleaseEvent = Event;
                        continue;
                    }

                    // KeyPress event + key repeat disabled + matching KeyRelease event = repeated event --> discard
                    if ((Event.type == KeyPress) && !myKeyRepeat &&
                        (myLastKeyReleaseEvent.xkey.keycode == Event.xkey.keycode) &&
                        (myLastKeyReleaseEvent.xkey.time == Event.xkey.time))
                    {
                        continue;
                    }
                }
            }
        }

        // Process the event
        ProcessEvent(Event);
   }
}


////////////////////////////////////////////////////////////
/// /see WindowImpl::SetActive
////////////////////////////////////////////////////////////
void WindowImplX11::SetActive(bool Active) const
{
    if (Active)
    {
        if (myWindow && myGLContext && (glXGetCurrentContext() != myGLContext))
            glXMakeCurrent(ourDisplay, myWindow, myGLContext);
    }
    else
    {
        if (glXGetCurrentContext() == myGLContext)
            glXMakeCurrent(ourDisplay, None, NULL);
    }
}


////////////////////////////////////////////////////////////
/// /see WindowImpl::UseVerticalSync
////////////////////////////////////////////////////////////
void WindowImplX11::UseVerticalSync(bool Enabled)
{
    const GLubyte* ProcAddress = reinterpret_cast<const GLubyte*>("glXSwapIntervalSGI");
    PFNGLXSWAPINTERVALSGIPROC glXSwapIntervalSGI = reinterpret_cast<PFNGLXSWAPINTERVALSGIPROC>(glXGetProcAddress(ProcAddress));
    if (glXSwapIntervalSGI)
        glXSwapIntervalSGI(Enabled ? 1 : 0);
}


////////////////////////////////////////////////////////////
/// /see WindowImpl::ShowMouseCursor
////////////////////////////////////////////////////////////
void WindowImplX11::ShowMouseCursor(bool Show)
{
    XDefineCursor(ourDisplay, myWindow, Show ? None : myHiddenCursor);
    XFlush(ourDisplay);
}


////////////////////////////////////////////////////////////
/// /see sfWindowImpl::SetCursorPosition
////////////////////////////////////////////////////////////
void WindowImplX11::SetCursorPosition(unsigned int Left, unsigned int Top)
{
    XWarpPointer(ourDisplay, None, myWindow, 0, 0, 0, 0, Left, Top);
    XFlush(ourDisplay);
}


////////////////////////////////////////////////////////////
/// /see sfWindowImpl::SetPosition
////////////////////////////////////////////////////////////
void WindowImplX11::SetPosition(int Left, int Top)
{
    XMoveWindow(ourDisplay, myWindow, Left, Top);
    XFlush(ourDisplay);
}


////////////////////////////////////////////////////////////
/// /see WindowImpl::SetSize
////////////////////////////////////////////////////////////
void WindowImplX11::SetSize(unsigned int Width, unsigned int Height)
{
    XResizeWindow(ourDisplay, myWindow, Width, Height);
    XFlush(ourDisplay);
}


////////////////////////////////////////////////////////////
/// /see sfWindowImpl::Show
////////////////////////////////////////////////////////////
void WindowImplX11::Show(bool State)
{
    if (State)
        XMapWindow(ourDisplay, myWindow);
    else
        XUnmapWindow(ourDisplay, myWindow);

    XFlush(ourDisplay);
}


////////////////////////////////////////////////////////////
/// /see sfWindowImpl::EnableKeyRepeat
////////////////////////////////////////////////////////////
void WindowImplX11::EnableKeyRepeat(bool Enabled)
{
    myKeyRepeat = Enabled;
}


////////////////////////////////////////////////////////////
/// /see WindowImpl::SetIcon
////////////////////////////////////////////////////////////
void WindowImplX11::SetIcon(unsigned int Width, unsigned int Height, const Uint8* Pixels)
{
    // X11 wants BGRA pixels : swap red and blue channels
    // Note : this memory will never be freed, but it seems to cause a bug on exit if I do so
    Uint8* IconPixels = new Uint8[Width * Height * 4];
    for (std::size_t i = 0; i < Width * Height; ++i)
    {
        IconPixels[i * 4 + 0] = Pixels[i * 4 + 2];
        IconPixels[i * 4 + 1] = Pixels[i * 4 + 1];
        IconPixels[i * 4 + 2] = Pixels[i * 4 + 0];
        IconPixels[i * 4 + 3] = Pixels[i * 4 + 3];
    }

    // Create the icon pixmap
    Visual*      DefVisual = DefaultVisual(ourDisplay, ourScreen);
    unsigned int DefDepth  = DefaultDepth(ourDisplay, ourScreen);
    XImage* IconImage = XCreateImage(ourDisplay, DefVisual, DefDepth, ZPixmap, 0, (char*)IconPixels, Width, Height, 32, 0);
    if (!IconImage)
    {
        std::cerr << "Failed to set the window's icon" << std::endl;
        return;
    }
    Pixmap IconPixmap = XCreatePixmap(ourDisplay, RootWindow(ourDisplay, ourScreen), Width, Height, DefDepth);
    XGCValues Values;
    GC IconGC = XCreateGC(ourDisplay, IconPixmap, 0, &Values);
    XPutImage(ourDisplay, IconPixmap, IconGC, IconImage, 0, 0, 0, 0, Width, Height);
    XFreeGC(ourDisplay, IconGC);
    XDestroyImage(IconImage);

    // Create the mask pixmap (must have 1 bit depth)
    std::size_t Pitch = (Width + 7) / 8;
    static std::vector<Uint8> MaskPixels(Pitch * Height, 0);
    for (std::size_t j = 0; j < Height; ++j)
    {
        for (std::size_t i = 0; i < Pitch; ++i)
        {
            for (std::size_t k = 0; k < 8; ++k)
            {
                if (i * 8 + k < Width)
                {
                    Uint8 Opacity = (Pixels[(i * 8 + k + j * Width) * 4 + 3] > 0) ? 1 : 0;
                    MaskPixels[i + j * Pitch] |= (Opacity << k);                    
                }
            }
        }
    }
    Pixmap MaskPixmap = XCreatePixmapFromBitmapData(ourDisplay, myWindow, (char*)&MaskPixels[0], Width, Height, 1, 0, 1);

    // Send our new icon to the window through the WMHints
    XWMHints* Hints = XAllocWMHints();
    Hints->flags       = IconPixmapHint | IconMaskHint;
    Hints->icon_pixmap = IconPixmap;
    Hints->icon_mask   = MaskPixmap;
    XSetWMHints(ourDisplay, myWindow, Hints);
    XFree(Hints);

    XFlush(ourDisplay);
}


////////////////////////////////////////////////////////////
/// Switch to fullscreen mode
////////////////////////////////////////////////////////////
void WindowImplX11::SwitchToFullscreen(const VideoMode& Mode)
{
    // Check if the XRandR extension is present
    int Version;
    if (XQueryExtension(ourDisplay, "RANDR", &Version, &Version, &Version))
    {
        // Get the current configuration
        XRRScreenConfiguration* Config = XRRGetScreenInfo(ourDisplay, RootWindow(ourDisplay, ourScreen));
        if (Config)
        {
            // Get the current rotation
            Rotation CurrentRotation;
            myOldVideoMode = XRRConfigCurrentConfiguration(Config, &CurrentRotation);

            // Get the available screen sizes
            int NbSizes;
            XRRScreenSize* Sizes = XRRConfigSizes(Config, &NbSizes);
            if (Sizes && (NbSizes > 0))
            {
                // Search a matching size
                for (int i = 0; i < NbSizes; ++i)
                {
                    if ((Sizes[i].width == static_cast<int>(Mode.Width)) && (Sizes[i].height == static_cast<int>(Mode.Height)))
                    {
                        // Switch to fullscreen mode
                        XRRSetScreenConfig(ourDisplay, Config, RootWindow(ourDisplay, ourScreen), i, CurrentRotation, CurrentTime);

                        // Set "this" as the current fullscreen window
                        ourFullscreenWindow = this;
                        break;
                    }
                }
            }

            // Free the configuration instance
            XRRFreeScreenConfigInfo(Config);
        }
        else
        {
            // Failed to get the screen configuration
            std::cerr << "Failed to get the current screen configuration for fullscreen mode, switching to windiw mode" << std::endl;
        }
    }
    else
    {
        // XRandr extension is not supported : we cannot use fullscreen mode
        std::cerr << "Fullscreen is not supported, switching to window mode" << std::endl;
    }
}


////////////////////////////////////////////////////////////
/// Create the OpenGL rendering context
////////////////////////////////////////////////////////////
bool WindowImplX11::CreateContext(const VideoMode& Mode, XVisualInfo& ChosenVisual, WindowSettings& Params, XVisualInfo Template, unsigned long Mask)
{
    // Get all the visuals matching the template
    Template.screen = ourScreen;
    int NbVisuals = 0;
    XVisualInfo* Visuals = XGetVisualInfo(ourDisplay, Mask | VisualScreenMask, &Template, &NbVisuals);
    if (!Visuals || (NbVisuals == 0))
    {
        if (Visuals)
            XFree(Visuals);
        std::cerr << "There is no valid visual for the selected screen" << std::endl;
        return false;
    }

    // Find the best visual
    int          BestScore  = 0xFFFF;
    XVisualInfo* BestVisual = NULL;
    while (!BestVisual)
    {
        for (int i = 0; i < NbVisuals; ++i)
        {
            // Get the current visual attributes
            int RGBA, DoubleBuffer, Red, Green, Blue, Alpha, Depth, Stencil, MultiSampling, Samples;
            glXGetConfig(ourDisplay, &Visuals[i], GLX_RGBA,               &RGBA);
            glXGetConfig(ourDisplay, &Visuals[i], GLX_DOUBLEBUFFER,       &DoubleBuffer); 
            glXGetConfig(ourDisplay, &Visuals[i], GLX_RED_SIZE,           &Red);
            glXGetConfig(ourDisplay, &Visuals[i], GLX_GREEN_SIZE,         &Green); 
            glXGetConfig(ourDisplay, &Visuals[i], GLX_BLUE_SIZE,          &Blue); 
            glXGetConfig(ourDisplay, &Visuals[i], GLX_ALPHA_SIZE,         &Alpha); 
            glXGetConfig(ourDisplay, &Visuals[i], GLX_DEPTH_SIZE,         &Depth);        
            glXGetConfig(ourDisplay, &Visuals[i], GLX_STENCIL_SIZE,       &Stencil);
            glXGetConfig(ourDisplay, &Visuals[i], GLX_SAMPLE_BUFFERS_ARB, &MultiSampling);        
            glXGetConfig(ourDisplay, &Visuals[i], GLX_SAMPLES_ARB,        &Samples);

            // First check the mandatory parameters
            if ((RGBA == 0) || (DoubleBuffer == 0))
                continue;

            // Evaluate the current configuration
            int Color = Red + Green + Blue + Alpha;
            int Score = EvaluateConfig(Mode, Params, Color, Depth, Stencil, MultiSampling ? Samples : 0);

            // Keep it if it's better than the current best
            if (Score < BestScore)
            {
                BestScore  = Score;
                BestVisual = &Visuals[i];
            }
        }

        // If no visual has been found, try a lower level of antialiasing
        if (!BestVisual)
        {
            if (Params.AntialiasingLevel > 2)
            {
                std::cerr << "Failed to find a pixel format supporting "
                          << Params.AntialiasingLevel << " antialiasing levels ; trying with 2 levels" << std::endl;
                Params.AntialiasingLevel = 2;
            }
            else if (Params.AntialiasingLevel > 0)
            {
                std::cerr << "Failed to find a pixel format supporting antialiasing ; antialiasing will be disabled" << std::endl;
                Params.AntialiasingLevel = 0;
            }
            else
            {
                std::cerr << "Failed to find a suitable pixel format for the window -- cannot create OpenGL context" << std::endl;
                return false;
            }
        }
    }

    // Create the OpenGL context
    myGLContext = glXCreateContext(ourDisplay, BestVisual, glXGetCurrentContext(), true);
    if (myGLContext == NULL)
    {
        std::cerr << "Failed to create an OpenGL context for this window" << std::endl;
        return false;
    }

    // Update the creation settings from the chosen format
    int Depth, Stencil;
    glXGetConfig(ourDisplay, BestVisual, GLX_DEPTH_SIZE,   &Depth);
    glXGetConfig(ourDisplay, BestVisual, GLX_STENCIL_SIZE, &Stencil);
    Params.DepthBits   = static_cast<unsigned int>(Depth);
    Params.StencilBits = static_cast<unsigned int>(Stencil);

    // Assign the chosen visual, and free the temporary visuals array
    ChosenVisual = *BestVisual;
    XFree(Visuals);

    // Activate the context
    SetActive(true);

    // Enable multisampling if needed
    if (Params.AntialiasingLevel > 0)
        glEnable(GL_MULTISAMPLE_ARB);

    return true;
}


////////////////////////////////////////////////////////////
/// Do some common initializations after the window has been created
////////////////////////////////////////////////////////////
void WindowImplX11::Initialize()
{
    // Make sure the "last key release" is initialized with invalid values
    myLastKeyReleaseEvent.type = -1;

    // Get the atom defining the close event
    myAtomClose = XInternAtom(ourDisplay, "WM_DELETE_WINDOW", false);
    XSetWMProtocols(ourDisplay, myWindow, &myAtomClose, 1);

    // Create the input context
    if (ourInputMethod)
    {
        myInputContext = XCreateIC(ourInputMethod,
                                   XNClientWindow,  myWindow,
                                   XNFocusWindow,   myWindow,
                                   XNInputStyle,    XIMPreeditNothing  | XIMStatusNothing,
                                   NULL);
        
        if (!myInputContext)
            std::cerr << "Failed to create input context for window -- TextEntered event won't be able to return unicode" << std::endl;
    }

    // Show the window
    XMapWindow(ourDisplay, myWindow);
    XFlush(ourDisplay);

    // Create the hiden cursor
    CreateHiddenCursor();

    // Set our context as the current OpenGL context for rendering
    SetActive();

    // Flush the commands queue
    XFlush(ourDisplay);
}


////////////////////////////////////////////////////////////
/// Create a transparent mouse cursor
////////////////////////////////////////////////////////////
void WindowImplX11::CreateHiddenCursor()
{
    // Create the cursor's pixmap (1x1 pixels)
	Pixmap CursorPixmap = XCreatePixmap(ourDisplay, myWindow, 1, 1, 1);
    GC GraphicsContext = XCreateGC(ourDisplay, CursorPixmap, 0, NULL);
    XDrawPoint(ourDisplay, CursorPixmap, GraphicsContext, 0, 0);
    XFreeGC(ourDisplay, GraphicsContext);

    // Create the cursor, using the pixmap as both the shape and the mask of the cursor
	XColor Color;
	Color.flags = DoRed | DoGreen | DoBlue;
	Color.red = Color.blue = Color.green = 0;
    myHiddenCursor = XCreatePixmapCursor(ourDisplay, CursorPixmap, CursorPixmap, &Color, &Color, 0, 0);

    // We don't need the pixmap any longer, free it
	XFreePixmap(ourDisplay, CursorPixmap);
}


////////////////////////////////////////////////////////////
/// Cleanup graphical resources attached to the window
////////////////////////////////////////////////////////////
void WindowImplX11::CleanUp()
{
    // Restore the previous video mode (in case we were running in fullscreen)
    if (ourFullscreenWindow == this)
    {
        // Get current screen info
        XRRScreenConfiguration* Config = XRRGetScreenInfo(ourDisplay, RootWindow(ourDisplay, ourScreen));
        if (Config) 
        {
            // Get the current rotation
            Rotation CurrentRotation;
            XRRConfigCurrentConfiguration(Config, &CurrentRotation);

            // Reset the video mode
            XRRSetScreenConfig(ourDisplay, Config, RootWindow(ourDisplay, ourScreen), myOldVideoMode, CurrentRotation, CurrentTime);

            // Free the configuration instance
            XRRFreeScreenConfigInfo(Config);
        } 

        // Reset the fullscreen window
        ourFullscreenWindow = NULL;
    }

    // Unhide the mouse cursor (in case it was hidden)
    ShowMouseCursor(true);

    // Destroy the OpenGL context
    if (myGLContext)
    {
        glXDestroyContext(ourDisplay, myGLContext);
        myGLContext = NULL;
    }
}


////////////////////////////////////////////////////////////
/// Process an incoming event from the window
////////////////////////////////////////////////////////////
void WindowImplX11::ProcessEvent(XEvent WinEvent)
{
    switch (WinEvent.type)
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

            Event Evt;
            Evt.Type = Event::GainedFocus;
            SendEvent(Evt);
            break;
        }

        // Lost focus event
        case FocusOut :
        {
            // Update the input context
            if (myInputContext)
                XUnsetICFocus(myInputContext);

            Event Evt;
            Evt.Type = Event::LostFocus;
            SendEvent(Evt);
            break;
        }

        // Resize event
        case ConfigureNotify :
        {
            if ((WinEvent.xconfigure.width != static_cast<int>(myWidth)) || (WinEvent.xconfigure.height != static_cast<int>(myHeight)))
            {
                myWidth  = WinEvent.xconfigure.width;
                myHeight = WinEvent.xconfigure.height;

                Event Evt;
                Evt.Type        = Event::Resized;
                Evt.Size.Width  = myWidth;
                Evt.Size.Height = myHeight;
                SendEvent(Evt);
            }
            break;
        }

        // Close event
        case ClientMessage :
        {
            if ((WinEvent.xclient.format == 32) && (WinEvent.xclient.data.l[0]) == static_cast<long>(myAtomClose))  
            {
                Event Evt;
                Evt.Type = Event::Closed;
                SendEvent(Evt);
            }
            break;
        }

        // Key down event
        case KeyPress :
        {
            // Get the keysym of the key that has been pressed
            static XComposeStatus KeyboardStatus;
            char Buffer[32];
            KeySym Sym;
            XLookupString(&WinEvent.xkey, Buffer, sizeof(Buffer), &Sym, &KeyboardStatus);

            // Fill the event parameters
            Event Evt;
            Evt.Type        = Event::KeyPressed;
            Evt.Key.Code    = KeysymToSF(Sym);
            Evt.Key.Alt     = WinEvent.xkey.state & Mod1Mask;
            Evt.Key.Control = WinEvent.xkey.state & ControlMask;
            Evt.Key.Shift   = WinEvent.xkey.state & ShiftMask;
            SendEvent(Evt);

            // Generate a TextEntered event
            if (!XFilterEvent(&WinEvent, None))
            {
                #ifdef X_HAVE_UTF8_STRING
                if (myInputContext)
                {
                    Status ReturnedStatus;
                    Uint8  KeyBuffer[16];
                    int Length = Xutf8LookupString(myInputContext, &WinEvent.xkey, reinterpret_cast<char*>(KeyBuffer), sizeof(KeyBuffer), NULL, &ReturnedStatus);
                    if (Length > 0)
                    {
                        Uint32 Unicode[2]; // just in case, but 1 character should be enough
                        const Uint32* End = Unicode::UTF8ToUTF32(KeyBuffer, KeyBuffer + Length, Unicode);

                        if (End > Unicode)
                        {
                            Event TextEvt;
                            TextEvt.Type         = Event::TextEntered;
                            TextEvt.Text.Unicode = Unicode[0];
                            SendEvent(TextEvt);
                        }
                    }
                }
                else
                #endif
                {
                    static XComposeStatus ComposeStatus;
                    char KeyBuffer[16];
                    if (XLookupString(&WinEvent.xkey, KeyBuffer, sizeof(KeyBuffer), NULL, &ComposeStatus))
                    {
                        Event TextEvt;
                        TextEvt.Type         = Event::TextEntered;
                        TextEvt.Text.Unicode = static_cast<Uint32>(KeyBuffer[0]);
                        SendEvent(TextEvt);
                    }
                }
            }

            break;
        }

        // Key up event
        case KeyRelease :
        {
            // Get the keysym of the key that has been pressed
            char Buffer[32];
            KeySym Sym;
            XLookupString(&WinEvent.xkey, Buffer, 32, &Sym, NULL);

            // Fill the event parameters
            Event Evt;
            Evt.Type        = Event::KeyReleased;
            Evt.Key.Code    = KeysymToSF(Sym);
            Evt.Key.Alt     = WinEvent.xkey.state & Mod1Mask;
            Evt.Key.Control = WinEvent.xkey.state & ControlMask;
            Evt.Key.Shift   = WinEvent.xkey.state & ShiftMask;

            SendEvent(Evt);
            break;
        }

        // Mouse button pressed
        case ButtonPress :
        {
            unsigned int Button = WinEvent.xbutton.button;
            if ((Button == Button1) || (Button == Button2) || (Button == Button3) || (Button == 8) || (Button == 9))
            {
                Event Evt;
                Evt.Type          = Event::MouseButtonPressed;
                Evt.MouseButton.X = WinEvent.xbutton.x;
                Evt.MouseButton.Y = WinEvent.xbutton.y;
                switch (Button)
                {
                    case Button1 : Evt.MouseButton.Button = Mouse::Left;     break;
                    case Button2 : Evt.MouseButton.Button = Mouse::Middle;   break;
                    case Button3 : Evt.MouseButton.Button = Mouse::Right;    break;
                    case 8 :       Evt.MouseButton.Button = Mouse::XButton1; break;
                    case 9 :       Evt.MouseButton.Button = Mouse::XButton2; break;            
                }
                SendEvent(Evt);
            }
            break;
        }

        // Mouse button released
        case ButtonRelease :
        {
            unsigned int Button = WinEvent.xbutton.button;
            if ((Button == Button1) || (Button == Button2) || (Button == Button3) || (Button == 8) || (Button == 9))
            {
                Event Evt;
                Evt.Type          = Event::MouseButtonReleased;
                Evt.MouseButton.X = WinEvent.xbutton.x;
                Evt.MouseButton.Y = WinEvent.xbutton.y;
                switch (Button)
                {
                    case Button1 : Evt.MouseButton.Button = Mouse::Left;     break;
                    case Button2 : Evt.MouseButton.Button = Mouse::Middle;   break;
                    case Button3 : Evt.MouseButton.Button = Mouse::Right;    break;
                    case 8 :       Evt.MouseButton.Button = Mouse::XButton1; break;
                    case 9 :       Evt.MouseButton.Button = Mouse::XButton2; break;            
                }
                SendEvent(Evt);
            }
            else if ((Button == Button4) || (Button == Button5))
            {
                Event Evt;
                Evt.Type             = Event::MouseWheelMoved;
                Evt.MouseWheel.Delta = WinEvent.xbutton.button == Button4 ? 1 : -1;
                SendEvent(Evt);
            }
            break;
        }

        // Mouse moved
        case MotionNotify :
        {
            Event Evt;
            Evt.Type        = Event::MouseMoved;
            Evt.MouseMove.X = WinEvent.xmotion.x;
            Evt.MouseMove.Y = WinEvent.xmotion.y;
            SendEvent(Evt);
            break;
        }

        // Mouse entered
        case EnterNotify :
        {
            Event Evt;
            Evt.Type = Event::MouseEntered;
            SendEvent(Evt);
            break;
        }

        // Mouse left
        case LeaveNotify :
        {
            Event Evt;
            Evt.Type = Event::MouseLeft;
            SendEvent(Evt);
            break;
        }
    }
}


////////////////////////////////////////////////////////////
/// Convert a X11 keysym to SFML key code
////////////////////////////////////////////////////////////
Key::Code WindowImplX11::KeysymToSF(KeySym Sym)
{
    // First convert to uppercase (to avoid dealing with two different keysyms for the same key)
    KeySym Lower, Key;
    XConvertCase(Sym, &Lower, &Key);

    switch (Key)
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


////////////////////////////////////////////////////////////
/// Open the display (if not already done)
////////////////////////////////////////////////////////////
bool WindowImplX11::OpenDisplay(bool AddWindow)
{
    // If no display has been opened yet, open it
    if (ourDisplay == NULL)
    {
        ourDisplay = XOpenDisplay(NULL);
        if (ourDisplay)
        {
            ourScreen = DefaultScreen(ourDisplay);

            // Get the input method (XIM) object
            ourInputMethod = XOpenIM(ourDisplay, NULL, NULL, NULL);
        }
        else
        {
            std::cerr << "Failed to open a connection with the X server" << std::endl;
        }
    }

    // Increase the number of windows
    if (AddWindow)
        ourWindowsCount++;

    return ourDisplay != NULL;
}


////////////////////////////////////////////////////////////
/// Close the display
////////////////////////////////////////////////////////////
void WindowImplX11::CloseDisplay()
{
    // Decrease the number of windows
    ourWindowsCount--;

    // If all windows have been destroyed, then we can close the display
    if (ourWindowsCount == 0)
    {
        // Close the input method object
        if (ourInputMethod)
            XCloseIM(ourInputMethod);

        XCloseDisplay(ourDisplay);
        ourDisplay = NULL;
    }
}

} // namespace priv

} // namespace sf
