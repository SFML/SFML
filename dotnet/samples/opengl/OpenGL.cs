using System;
using SFML;
using SFML.Graphics;
using SFML.Window;
using Tao.OpenGl;

namespace sample_opengl
{
    static class Program
    {
        /// <summary>
        /// The main entry point for the application.
        /// </summary>
        static void Main()
        {
            // Create main window
            RenderWindow App = new RenderWindow(new VideoMode(800, 600), "SFML.Net OpenGL");
            App.PreserveOpenGLStates(true);

            // Setup event handlers
            App.Closed     += new EventHandler(OnClosed);
            App.KeyPressed += new EventHandler<KeyEventArgs>(OnKeyPressed);
            App.Resized    += new EventHandler<SizeEventArgs>(OnResized);

            // Create a sprite for the background
            Image BackgroundImage = new Image("datas/opengl/background.jpg");
            Sprite Background = new Sprite(BackgroundImage);

            // Create a text to display
            String2D Text = new String2D("This is a rotating cube");
            Text.Position = new Vector2(250.0F, 300.0F);
            Text.Color = new Color(128, 0, 128);

            // Load an OpenGL texture.
            // We could directly use a sf::Image as an OpenGL texture (with its Bind() member function),
            // but here we want more control on it (generate mipmaps, ...) so we create a new one
            int Texture = 0;
            using (Image TempImage = new Image("datas/opengl/texture.jpg"))
            {
                Gl.glGenTextures(1, out Texture);
                Gl.glBindTexture(Gl.GL_TEXTURE_2D, Texture);
                Glu.gluBuild2DMipmaps(Gl.GL_TEXTURE_2D, Gl.GL_RGBA, (int)TempImage.Width, (int)TempImage.Height, Gl.GL_RGBA, Gl.GL_UNSIGNED_BYTE, TempImage.Pixels);
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
            Gl.glBindTexture(Gl.GL_TEXTURE_2D, Texture);
            Gl.glColor4f(1.0F, 1.0F, 1.0F, 1.0F);

            float Time = 0.0F;

            // Start game loop
            while (App.IsOpened())
            {
                // Process events
                App.DispatchEvents();

                // Clear the window
                App.Clear();

                // Draw background
                App.Draw(Background);

                // Clear depth buffer
                Gl.glClear(Gl.GL_DEPTH_BUFFER_BIT);

                // Apply some transformations
                Time += App.GetFrameTime();
                Gl.glMatrixMode(Gl.GL_MODELVIEW);
                Gl.glLoadIdentity();
                Gl.glTranslatef(0.0F, 0.0F, -200.0F);
                Gl.glRotatef(Time * 50, 1.0F, 0.0F, 0.0F);
                Gl.glRotatef(Time * 30, 0.0F, 1.0F, 0.0F);
                Gl.glRotatef(Time * 90, 0.0F, 0.0F, 1.0F);

                // Draw a cube
                Gl.glBegin(Gl.GL_QUADS);

                    Gl.glTexCoord2f(0, 0); Gl.glVertex3f(-50.0F, -50.0F, -50.0F);
                    Gl.glTexCoord2f(0, 1); Gl.glVertex3f(-50.0F,  50.0F, -50.0F);
                    Gl.glTexCoord2f(1, 1); Gl.glVertex3f( 50.0F,  50.0F, -50.0F);
                    Gl.glTexCoord2f(1, 0); Gl.glVertex3f( 50.0F, -50.0F, -50.0F);

                    Gl.glTexCoord2f(0, 0); Gl.glVertex3f(-50.0F, -50.0F, 50.0F);
                    Gl.glTexCoord2f(0, 1); Gl.glVertex3f(-50.0F,  50.0F, 50.0F);
                    Gl.glTexCoord2f(1, 1); Gl.glVertex3f( 50.0F,  50.0F, 50.0F);
                    Gl.glTexCoord2f(1, 0); Gl.glVertex3f( 50.0F, -50.0F, 50.0F);

                    Gl.glTexCoord2f(0, 0); Gl.glVertex3f(-50.0F, -50.0F, -50.0F);
                    Gl.glTexCoord2f(0, 1); Gl.glVertex3f(-50.0F,  50.0F, -50.0F);
                    Gl.glTexCoord2f(1, 1); Gl.glVertex3f(-50.0F,  50.0F,  50.0F);
                    Gl.glTexCoord2f(1, 0); Gl.glVertex3f(-50.0F, -50.0F,  50.0F);

                    Gl.glTexCoord2f(0, 0); Gl.glVertex3f(50.0F, -50.0F, -50.0F);
                    Gl.glTexCoord2f(0, 1); Gl.glVertex3f(50.0F,  50.0F, -50.0F);
                    Gl.glTexCoord2f(1, 1); Gl.glVertex3f(50.0F,  50.0F,  50.0F);
                    Gl.glTexCoord2f(1, 0); Gl.glVertex3f(50.0F, -50.0F,  50.0F);

                    Gl.glTexCoord2f(0, 1); Gl.glVertex3f(-50.0F, -50.0F,  50.0F);
                    Gl.glTexCoord2f(0, 0); Gl.glVertex3f(-50.0F, -50.0F, -50.0F);
                    Gl.glTexCoord2f(1, 0); Gl.glVertex3f( 50.0F, -50.0F, -50.0F);
                    Gl.glTexCoord2f(1, 1); Gl.glVertex3f( 50.0F, -50.0F,  50.0F);

                    Gl.glTexCoord2f(0, 1); Gl.glVertex3f(-50.0F, 50.0F,  50.0F);
                    Gl.glTexCoord2f(0, 0); Gl.glVertex3f(-50.0F, 50.0F, -50.0F);
                    Gl.glTexCoord2f(1, 0); Gl.glVertex3f( 50.0F, 50.0F, -50.0F);
                    Gl.glTexCoord2f(1, 1); Gl.glVertex3f( 50.0F, 50.0F,  50.0F);

                Gl.glEnd();

                // Draw some text on top of our OpenGL object
                App.Draw(Text);

                // Finally, display the rendered frame on screen
                App.Display();
            }

            // Don't forget to destroy our texture
            Gl.glDeleteTextures(1, ref Texture);
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
