using System;
using SFML;
using SFML.Window;
using Tao.OpenGl;

namespace window
{
    static class Program
    {
        /// <summary>
        /// The main entry point for the application.
        /// </summary>
        static void Main()
        {
            // Create the main window
            Window window = new Window(new VideoMode(640, 480, 32), "SFML.Net Window");

            // Setup event handlers
            window.Closed     += new EventHandler(OnClosed);
            window.KeyPressed += new EventHandler<KeyEventArgs>(OnKeyPressed);
            window.Resized    += new EventHandler<SizeEventArgs>(OnResized);

            // Set the color and depth clear values
            Gl.glClearDepth(1.0F);
            Gl.glClearColor(0.0F, 0.0F, 0.0F, 0.0F);

            // Enable Z-buffer read and write
            Gl.glEnable(Gl.GL_DEPTH_TEST);
            Gl.glDepthMask(Gl.GL_TRUE);

            // Setup a perspective projection
            Gl.glMatrixMode(Gl.GL_PROJECTION);
            Gl.glLoadIdentity();
            Glu.gluPerspective(90.0F, 1.0F, 1.0F, 500.0F);

            float time = 0.0F;

            // Start the game loop
            while (window.IsOpened())
            {
                // Process events
                window.DispatchEvents();

                // Activate the window before using OpenGL commands.
                // This is useless here because we have only one window which is
                // always the active one, but don't forget it if you use multiple windows
                window.SetActive();

                // Clear color and depth buffer
                Gl.glClear(Gl.GL_COLOR_BUFFER_BIT | Gl.GL_DEPTH_BUFFER_BIT);

                // Apply some transformations
                time += window.GetFrameTime();
                Gl.glMatrixMode(Gl.GL_MODELVIEW);
                Gl.glLoadIdentity();
                Gl.glTranslatef(0.0F, 0.0F, -200.0F);
                Gl.glRotatef(time * 50, 1.0F, 0.0F, 0.0F);
                Gl.glRotatef(time * 30, 0.0F, 1.0F, 0.0F);
                Gl.glRotatef(time * 90, 0.0F, 0.0F, 1.0F);

                // Draw a cube
                Gl.glBegin(Gl.GL_QUADS);

                    Gl.glColor3f(1.0F, 0.0F, 0.0F);
                    Gl.glVertex3f(-50.0F, -50.0F, -50.0F);
                    Gl.glVertex3f(-50.0F,  50.0F, -50.0F);
                    Gl.glVertex3f( 50.0F,  50.0F, -50.0F);
                    Gl.glVertex3f( 50.0F, -50.0F, -50.0F);

                    Gl.glColor3f(1.0F, 0.0F, 0.0F);
                    Gl.glVertex3f(-50.0F, -50.0F, 50.0F);
                    Gl.glVertex3f(-50.0F,  50.0F, 50.0F);
                    Gl.glVertex3f( 50.0F,  50.0F, 50.0F);
                    Gl.glVertex3f( 50.0F, -50.0F, 50.0F);

                    Gl.glColor3f(0.0F, 1.0F, 0.0F);
                    Gl.glVertex3f(-50.0F, -50.0F, -50.0F);
                    Gl.glVertex3f(-50.0F,  50.0F, -50.0F);
                    Gl.glVertex3f(-50.0F,  50.0F,  50.0F);
                    Gl.glVertex3f(-50.0F, -50.0F,  50.0F);

                    Gl.glColor3f(0.0F, 1.0F, 0.0F);
                    Gl.glVertex3f(50.0F, -50.0F, -50.0F);
                    Gl.glVertex3f(50.0F,  50.0F, -50.0F);
                    Gl.glVertex3f(50.0F,  50.0F,  50.0F);
                    Gl.glVertex3f(50.0F, -50.0F,  50.0F);

                    Gl.glColor3f(0.0F, 0.0F, 1.0F);
                    Gl.glVertex3f(-50.0F, -50.0F,  50.0F);
                    Gl.glVertex3f(-50.0F, -50.0F, -50.0F);
                    Gl.glVertex3f( 50.0F, -50.0F, -50.0F);
                    Gl.glVertex3f( 50.0F, -50.0F,  50.0F);

                    Gl.glColor3f(0.0F, 0.0F, 1.0F);
                    Gl.glVertex3f(-50.0F, 50.0F,  50.0F);
                    Gl.glVertex3f(-50.0F, 50.0F, -50.0F);
                    Gl.glVertex3f( 50.0F, 50.0F, -50.0F);
                    Gl.glVertex3f( 50.0F, 50.0F,  50.0F);

                Gl.glEnd();

                // Finally, display the rendered frame on screen
                window.Display();
            }
        }

        /// <summary>
        /// Function called when the window is closed
        /// </summary>
        static void OnClosed(object sender, EventArgs e)
        {
            Window window = (Window)sender;
            window.Close();
        }

        /// <summary>
        /// Function called when a key is pressed
        /// </summary>
        static void OnKeyPressed(object sender, KeyEventArgs e)
        {
            Window window = (Window)sender;
            if (e.Code == KeyCode.Escape)
                window.Close();
        }

        /// <summary>
        /// Function called when the window is resized
        /// </summary>
        static void OnResized(object sender, SizeEventArgs e)
        {
            Gl.glViewport(0, 0, (int)e.Width, (int)e.Height);
        }
    }
}
