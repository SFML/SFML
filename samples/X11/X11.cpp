
////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Window.hpp>
#include <X11/Xlib.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <iostream>


////////////////////////////////////////////////////////////
/// Initialize OpenGL states into the specified view
///
/// \param Window : Target window to initialize
///
////////////////////////////////////////////////////////////
void Initialize(sf::Window& Window)
{
    // Activate the window
    Window.SetActive();

    // Setup OpenGL states
    // Set color and depth clear value
    glClearDepth(1.f);
    glClearColor(0.f, 0.5f, 0.5f, 0.f);

    // Enable Z-buffer read and write
    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);

    // Setup a perspective projection
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(90.f, 1.f, 1.f, 500.f);
}

////////////////////////////////////////////////////////////
/// Draw the OpenGL scene (a rotating cube) into
/// the specified view
///
/// \param Window :      Target window for rendering
/// \param ElapsedTime : Time elapsed since the last draw
///
////////////////////////////////////////////////////////////
void Draw(sf::Window& Window, float ElapsedTime)
{
    // Activate the window
    Window.SetActive();

    // Clear color and depth buffers
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Apply some transformations
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(0.f, 0.f, -200.f);
    glRotatef(ElapsedTime * 50, 1.f, 0.f, 0.f);
    glRotatef(ElapsedTime * 30, 0.f, 1.f, 0.f);
    glRotatef(ElapsedTime * 90, 0.f, 0.f, 1.f);

    // Draw a cube
    glBegin(GL_QUADS);

        glColor3f(1.f, 1.f, 0.f);
        glVertex3f(-50.f, -50.f, -50.f);
        glVertex3f(-50.f,  50.f, -50.f);
        glVertex3f( 50.f,  50.f, -50.f);
        glVertex3f( 50.f, -50.f, -50.f);

        glColor3f(1.f, 1.f, 0.f);
        glVertex3f(-50.f, -50.f, 50.f);
        glVertex3f(-50.f,  50.f, 50.f);
        glVertex3f( 50.f,  50.f, 50.f);
        glVertex3f( 50.f, -50.f, 50.f);

        glColor3f(0.f, 1.f, 1.f);
        glVertex3f(-50.f, -50.f, -50.f);
        glVertex3f(-50.f,  50.f, -50.f);
        glVertex3f(-50.f,  50.f,  50.f);
        glVertex3f(-50.f, -50.f,  50.f);

        glColor3f(0.f, 1.f, 1.f);
        glVertex3f(50.f, -50.f, -50.f);
        glVertex3f(50.f,  50.f, -50.f);
        glVertex3f(50.f,  50.f,  50.f);
        glVertex3f(50.f, -50.f,  50.f);

        glColor3f(1.f, 0.f, 1.f);
        glVertex3f(-50.f, -50.f,  50.f);
        glVertex3f(-50.f, -50.f, -50.f);
        glVertex3f( 50.f, -50.f, -50.f);
        glVertex3f( 50.f, -50.f,  50.f);

        glColor3f(1.f, 0.f, 1.f);
        glVertex3f(-50.f, 50.f,  50.f);
        glVertex3f(-50.f, 50.f, -50.f);
        glVertex3f( 50.f, 50.f, -50.f);
        glVertex3f( 50.f, 50.f,  50.f);

    glEnd();
}


////////////////////////////////////////////////////////////
/// Entry point of application
///
/// \return Error code
///
////////////////////////////////////////////////////////////
int main()
{
    // Open a connection with the X server
    Display* Disp = XOpenDisplay(NULL);
    if (!Disp)
        return EXIT_FAILURE;

    // Get the default screen
    int Screen = DefaultScreen(Disp);

    // Let's create the main window
    XSetWindowAttributes Attributes;
    Attributes.background_pixel = BlackPixel(Disp, Screen);
    Attributes.event_mask       = KeyPressMask;
    Window Win = XCreateWindow(Disp, RootWindow(Disp, Screen),
                               0, 0, 650, 330, 0,
                               DefaultDepth(Disp, Screen),
                               InputOutput,
                               DefaultVisual(Disp, Screen),
                               CWBackPixel | CWEventMask, &Attributes);
    if (!Win)
        return EXIT_FAILURE;

    // Set the window's name
    XStoreName(Disp, Win, "SFML Window");

    // Let's create the windows which will serve as containers for our SFML views
    Window View1 = XCreateWindow(Disp, Win,
                                 10, 10, 310, 310, 0,
                                 DefaultDepth(Disp, Screen),
                                 InputOutput,
                                 DefaultVisual(Disp, Screen),
                                 0, NULL);
    Window View2 = XCreateWindow(Disp, Win,
                                 330, 10, 310, 310, 0,
                                 DefaultDepth(Disp, Screen),
                                 InputOutput,
                                 DefaultVisual(Disp, Screen),
                                 0, NULL);

    // Show our windows
    XMapWindow(Disp, Win);
    XFlush(Disp);

    // Create our SFML views
    sf::Window SFMLView1(View1);
    sf::Window SFMLView2(View2);

    // Create a clock for measuring elapsed time
    sf::Clock Clock;

    // Initialize our views
    Initialize(SFMLView1);
    Initialize(SFMLView2);

    // Start the event loop
    bool IsRunning = true;
    while (IsRunning)
    {
        while (XPending(Disp))
        {
            // Get the next pending event
            XEvent Event;
            XNextEvent(Disp, &Event);

            // Process it
            switch (Event.type)
            {
                // Any key is pressed : quit
                case KeyPress :
                    IsRunning = false;
                    break;
            }
        }

        // Draw something into our views
        Draw(SFMLView1, Clock.GetElapsedTime());
        Draw(SFMLView2, Clock.GetElapsedTime() * 0.3f);

        // Display the views on screen
        SFMLView1.Display();
        SFMLView2.Display();
    }

    // Close the display
    XCloseDisplay(Disp);

    return EXIT_SUCCESS;
}
