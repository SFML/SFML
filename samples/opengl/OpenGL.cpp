
////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Graphics.hpp>
#include <iostream>


////////////////////////////////////////////////////////////
/// Entry point of application
///
/// \return Application exit code
///
////////////////////////////////////////////////////////////
int main()
{
    // Create main window
    sf::RenderWindow App(sf::VideoMode(800, 600), "SFML OpenGL");
    App.PreserveOpenGLStates(true);

    // Create a sprite for the background
    sf::Image BackgroundImage;
    if (!BackgroundImage.LoadFromFile("datas/opengl/background.jpg"))
        return EXIT_FAILURE;
    sf::Sprite Background(BackgroundImage);

    // Load an OpenGL texture.
    // We could directly use a sf::Image as an OpenGL texture (with its Bind() member function),
    // but here we want more control on it (generate mipmaps, ...) so we create a new one
    GLuint Texture = 0;
    {
        sf::Image Image;
        if (!Image.LoadFromFile("datas/opengl/texture.jpg"))
            return EXIT_FAILURE;
        glGenTextures(1, &Texture);
        glBindTexture(GL_TEXTURE_2D, Texture);
        gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, Image.GetWidth(), Image.GetHeight(), GL_RGBA, GL_UNSIGNED_BYTE, Image.GetPixelsPtr());
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    }

    // Enable Z-buffer read and write
    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);
    glClearDepth(1.f);

    // Setup a perspective projection
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(90.f, 1.f, 1.f, 500.f);

    // Bind our texture
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, Texture);
    glColor4f(1.f, 1.f, 1.f, 1.f);

    // Create a clock for measuring the time elapsed
    sf::Clock Clock;

    // Start game loop
    while (App.IsOpened())
    {
        // Process events
        sf::Event Event;
        while (App.GetEvent(Event))
        {
            // Close window : exit
            if (Event.Type == sf::Event::Closed)
                App.Close();

            // Escape key : exit
            if ((Event.Type == sf::Event::KeyPressed) && (Event.Key.Code == sf::Key::Escape))
                App.Close();

            // Adjust the viewport when the window is resized
            if (Event.Type == sf::Event::Resized)
                glViewport(0, 0, Event.Size.Width, Event.Size.Height);
         }

        // Draw background
        App.Draw(Background);

        // Clear depth buffer
        glClear(GL_DEPTH_BUFFER_BIT);

        // Apply some transformations
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        glTranslatef(0.f, 0.f, -200.f);
        glRotatef(Clock.GetElapsedTime() * 50, 1.f, 0.f, 0.f);
        glRotatef(Clock.GetElapsedTime() * 30, 0.f, 1.f, 0.f);
        glRotatef(Clock.GetElapsedTime() * 90, 0.f, 0.f, 1.f);

        // Draw a cube
        glBegin(GL_QUADS);

            glTexCoord2f(0, 0); glVertex3f(-50.f, -50.f, -50.f);
            glTexCoord2f(0, 1); glVertex3f(-50.f,  50.f, -50.f);
            glTexCoord2f(1, 1); glVertex3f( 50.f,  50.f, -50.f);
            glTexCoord2f(1, 0); glVertex3f( 50.f, -50.f, -50.f);

            glTexCoord2f(0, 0); glVertex3f(-50.f, -50.f, 50.f);
            glTexCoord2f(0, 1); glVertex3f(-50.f,  50.f, 50.f);
            glTexCoord2f(1, 1); glVertex3f( 50.f,  50.f, 50.f);
            glTexCoord2f(1, 0); glVertex3f( 50.f, -50.f, 50.f);

            glTexCoord2f(0, 0); glVertex3f(-50.f, -50.f, -50.f);
            glTexCoord2f(0, 1); glVertex3f(-50.f,  50.f, -50.f);
            glTexCoord2f(1, 1); glVertex3f(-50.f,  50.f,  50.f);
            glTexCoord2f(1, 0); glVertex3f(-50.f, -50.f,  50.f);

            glTexCoord2f(0, 0); glVertex3f(50.f, -50.f, -50.f);
            glTexCoord2f(0, 1); glVertex3f(50.f,  50.f, -50.f);
            glTexCoord2f(1, 1); glVertex3f(50.f,  50.f,  50.f);
            glTexCoord2f(1, 0); glVertex3f(50.f, -50.f,  50.f);

            glTexCoord2f(0, 1); glVertex3f(-50.f, -50.f,  50.f);
            glTexCoord2f(0, 0); glVertex3f(-50.f, -50.f, -50.f);
            glTexCoord2f(1, 0); glVertex3f( 50.f, -50.f, -50.f);
            glTexCoord2f(1, 1); glVertex3f( 50.f, -50.f,  50.f);

            glTexCoord2f(0, 1); glVertex3f(-50.f, 50.f,  50.f);
            glTexCoord2f(0, 0); glVertex3f(-50.f, 50.f, -50.f);
            glTexCoord2f(1, 0); glVertex3f( 50.f, 50.f, -50.f);
            glTexCoord2f(1, 1); glVertex3f( 50.f, 50.f,  50.f);

        glEnd();

        // Draw some text on top of our OpenGL object
        sf::String Text("This is a rotating cube");
        Text.SetPosition(250.f, 300.f);
        Text.SetColor(sf::Color(128, 0, 128));
        App.Draw(Text);

        // Finally, display the rendered frame on screen
        App.Display();
    }

    // Don't forget to destroy our texture
    glDeleteTextures(1, &Texture);

    return EXIT_SUCCESS;
}
