using System;
using SFML;
using SFML.Graphics;
using SFML.Window;
using Tao.OpenGl;

namespace opengl
{
    static class Program
    {
        /// <summary>
        /// The main entry point for the application.
        /// </summary>
        static void Main()
        {
            string path = System.IO.Directory.GetCurrentDirectory();

            // Create main window
            RenderWindow window = new RenderWindow(new VideoMode(800, 600), "SFML.Net OpenGL");

            // Setup event handlers
            window.Closed     += new EventHandler(OnClosed);
            window.KeyPressed += new EventHandler<KeyEventArgs>(OnKeyPressed);
            window.Resized    += new EventHandler<SizeEventArgs>(OnResized);

            // Create a sprite for the background
            Image backgroundImage = new Image("resources/background.jpg");
            Sprite background = new Sprite(backgroundImage);

            // Create a text to display
            Text text = new Text("SFML / OpenGL demo");
            text.Position = new Vector2(250.0F, 450.0F);
            text.Color = new Color(255, 255, 255, 170);

            // Load an OpenGL texture.
            // We could directly use a sf::Image as an OpenGL texture (with its Bind() member function),
            // but here we want more control on it (generate mipmaps, ...) so we create a new one
            int texture = 0;
            using (Image image = new Image("resources/texture.jpg"))
            {
                Gl.glGenTextures(1, out texture);
                Gl.glBindTexture(Gl.GL_TEXTURE_2D, texture);
                Glu.gluBuild2DMipmaps(Gl.GL_TEXTURE_2D, Gl.GL_RGBA, (int)image.Width, (int)image.Height, Gl.GL_RGBA, Gl.GL_UNSIGNED_BYTE, image.Pixels);
                Gl.glTexParameteri(Gl.GL_TEXTURE_2D, Gl.GL_TEXTURE_MAG_FILTER, Gl.GL_LINEAR);
                Gl.glTexParameteri(Gl.GL_TEXTURE_2D, Gl.GL_TEXTURE_MIN_FILTER, Gl.GL_LINEAR_MIPMAP_LINEAR);
            }

            // Enable Z-buffer read and write
            Gl.glEnable(Gl.GL_DEPTH_TEST);
            Gl.glDepthMask(Gl.GL_TRUE);
            Gl.glClearDepth(1.0F);

            // Setup a perspective projection
            Gl.glMatrixMode(Gl.GL_PROJECTION);
            Gl.glLoadIdentity();
            Glu.gluPerspective(90.0F, 1.0F, 1.0F, 500.0F);

            // Bind our texture
            Gl.glEnable(Gl.GL_TEXTURE_2D);
            Gl.glBindTexture(Gl.GL_TEXTURE_2D, texture);
            Gl.glColor4f(1.0F, 1.0F, 1.0F, 1.0F);

            float time = 0.0F;

            // Start game loop
            while (window.IsOpened())
            {
                // Process events
                window.DispatchEvents();

                // Clear the window
                window.Clear();

                // Draw background
                window.SaveGLStates();
                window.Draw(background);
                window.RestoreGLStates();

                // Activate the window before using OpenGL commands.
                // This is useless here because we have only one window which is
                // always the active one, but don't forget it if you use multiple windows
                window.SetActive();

                // Clear depth buffer
                Gl.glClear(Gl.GL_DEPTH_BUFFER_BIT);

                // We get the position of the mouse cursor, so that we can move the box accordingly
                float x =  window.Input.GetMouseX() * 200.0F / window.Width  - 100.0F;
                float y = -window.Input.GetMouseY() * 200.0F / window.Height + 100.0F;

                // Apply some transformations
                time += window.GetFrameTime();
                Gl.glMatrixMode(Gl.GL_MODELVIEW);
                Gl.glLoadIdentity();
                Gl.glTranslatef(x, y, -100.0F);
                Gl.glRotatef(time * 50, 1.0F, 0.0F, 0.0F);
                Gl.glRotatef(time * 30, 0.0F, 1.0F, 0.0F);
                Gl.glRotatef(time * 90, 0.0F, 0.0F, 1.0F);

                // Draw a cube
                float size = 20.0F;
                Gl.glBegin(Gl.GL_QUADS);

                    Gl.glTexCoord2f(0, 0); Gl.glVertex3f(-size, -size, -size);
                    Gl.glTexCoord2f(0, 1); Gl.glVertex3f(-size,  size, -size);
                    Gl.glTexCoord2f(1, 1); Gl.glVertex3f( size,  size, -size);
                    Gl.glTexCoord2f(1, 0); Gl.glVertex3f( size, -size, -size);

                    Gl.glTexCoord2f(0, 0); Gl.glVertex3f(-size, -size, size);
                    Gl.glTexCoord2f(0, 1); Gl.glVertex3f(-size,  size, size);
                    Gl.glTexCoord2f(1, 1); Gl.glVertex3f( size,  size, size);
                    Gl.glTexCoord2f(1, 0); Gl.glVertex3f( size, -size, size);

                    Gl.glTexCoord2f(0, 0); Gl.glVertex3f(-size, -size, -size);
                    Gl.glTexCoord2f(0, 1); Gl.glVertex3f(-size,  size, -size);
                    Gl.glTexCoord2f(1, 1); Gl.glVertex3f(-size,  size,  size);
                    Gl.glTexCoord2f(1, 0); Gl.glVertex3f(-size, -size,  size);

                    Gl.glTexCoord2f(0, 0); Gl.glVertex3f(size, -size, -size);
                    Gl.glTexCoord2f(0, 1); Gl.glVertex3f(size,  size, -size);
                    Gl.glTexCoord2f(1, 1); Gl.glVertex3f(size,  size,  size);
                    Gl.glTexCoord2f(1, 0); Gl.glVertex3f(size, -size,  size);

                    Gl.glTexCoord2f(0, 1); Gl.glVertex3f(-size, -size,  size);
                    Gl.glTexCoord2f(0, 0); Gl.glVertex3f(-size, -size, -size);
                    Gl.glTexCoord2f(1, 0); Gl.glVertex3f( size, -size, -size);
                    Gl.glTexCoord2f(1, 1); Gl.glVertex3f( size, -size,  size);

                    Gl.glTexCoord2f(0, 1); Gl.glVertex3f(-size, size,  size);
                    Gl.glTexCoord2f(0, 0); Gl.glVertex3f(-size, size, -size);
                    Gl.glTexCoord2f(1, 0); Gl.glVertex3f( size, size, -size);
                    Gl.glTexCoord2f(1, 1); Gl.glVertex3f( size, size,  size);

                Gl.glEnd();

                // Draw some text on top of our OpenGL object
                window.SaveGLStates();
                window.Draw(text);
                window.RestoreGLStates();

                // Finally, display the rendered frame on screen
                window.Display();
            }

            // Don't forget to destroy our texture
            Gl.glDeleteTextures(1, ref texture);
        }

        /// <summary>
        /// Function called when the window is closed
        /// </summary>
        static void OnClosed(object sender, EventArgs e)
        {
            RenderWindow window = (RenderWindow)sender;
            window.Close();
        }

        /// <summary>
        /// Function called when a key is pressed
        /// </summary>
        static void OnKeyPressed(object sender, KeyEventArgs e)
        {
            RenderWindow window = (RenderWindow)sender;
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
