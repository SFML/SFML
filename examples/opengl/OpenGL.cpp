
////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>
#include <iostream>

////////////////////////////////////////////////////////////
/// Entry point of application
///
/// \return Application exit code
///
////////////////////////////////////////////////////////////
int main()
{
    // Create the main window
    sf::RenderWindow window(sf::VideoMode(800, 600), "SFML OpenGL");

    // Create a sprite for the background
    sf::Image backgroundImage;
    if (!backgroundImage.LoadFromFile("resources/background.jpg"))
        return EXIT_FAILURE;
    sf::Sprite background(backgroundImage);

    // Load an OpenGL texture.
    // We could directly use a sf::Image as an OpenGL texture (with its Bind() member function),
    // but here we want more control on it (generate mipmaps, ...) so we create a new one from the image pixels
    GLuint texture = 0;
    {
        sf::Image image;
        if (!image.LoadFromFile("resources/texture.jpg"))
            return EXIT_FAILURE;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, image.GetWidth(), image.GetHeight(), GL_RGBA, GL_UNSIGNED_BYTE, image.GetPixelsPtr());
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
    glBindTexture(GL_TEXTURE_2D, texture);
    glColor4f(1.f, 1.f, 1.f, 1.f);

    // Create a clock for measuring the time elapsed
    sf::Clock clock;

    // Start game loop
    while (window.IsOpened())
    {
        // Process events
        sf::Event event;
        while (window.GetEvent(event))
        {
            // Close window : exit
            if (event.Type == sf::Event::Closed)
                window.Close();

            // Escape key : exit
            if ((event.Type == sf::Event::KeyPressed) && (event.Key.Code == sf::Key::Escape))
                window.Close();

            // Adjust the viewport when the window is resized
            if (event.Type == sf::Event::Resized)
                glViewport(0, 0, event.Size.Width, event.Size.Height);
        }

        // Draw the background
        window.SaveGLStates();
        window.Draw(background);
        window.RestoreGLStates();

        // Activate the window before using OpenGL commands.
        // This is useless here because we have only one window which is
        // always the active one, but don't forget it if you use multiple windows
        window.SetActive();

        // Clear the depth buffer
        glClear(GL_DEPTH_BUFFER_BIT);

        // We get the position of the mouse cursor, so that we can move the box accordingly
        float x =  window.GetInput().GetMouseX() * 200.f / window.GetWidth()  - 100.f;
        float y = -window.GetInput().GetMouseY() * 200.f / window.GetHeight() + 100.f;

        // Apply some transformations
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        glTranslatef(x, y, -100.f);
        glRotatef(clock.GetElapsedTime() * 50, 1.f, 0.f, 0.f);
        glRotatef(clock.GetElapsedTime() * 30, 0.f, 1.f, 0.f);
        glRotatef(clock.GetElapsedTime() * 90, 0.f, 0.f, 1.f);

        // Draw a cube
        float size = 20.f;
        glBegin(GL_QUADS);

            glTexCoord2f(0, 0); glVertex3f(-size, -size, -size);
            glTexCoord2f(0, 1); glVertex3f(-size,  size, -size);
            glTexCoord2f(1, 1); glVertex3f( size,  size, -size);
            glTexCoord2f(1, 0); glVertex3f( size, -size, -size);

            glTexCoord2f(0, 0); glVertex3f(-size, -size, size);
            glTexCoord2f(0, 1); glVertex3f(-size,  size, size);
            glTexCoord2f(1, 1); glVertex3f( size,  size, size);
            glTexCoord2f(1, 0); glVertex3f( size, -size, size);

            glTexCoord2f(0, 0); glVertex3f(-size, -size, -size);
            glTexCoord2f(0, 1); glVertex3f(-size,  size, -size);
            glTexCoord2f(1, 1); glVertex3f(-size,  size,  size);
            glTexCoord2f(1, 0); glVertex3f(-size, -size,  size);

            glTexCoord2f(0, 0); glVertex3f(size, -size, -size);
            glTexCoord2f(0, 1); glVertex3f(size,  size, -size);
            glTexCoord2f(1, 1); glVertex3f(size,  size,  size);
            glTexCoord2f(1, 0); glVertex3f(size, -size,  size);

            glTexCoord2f(0, 1); glVertex3f(-size, -size,  size);
            glTexCoord2f(0, 0); glVertex3f(-size, -size, -size);
            glTexCoord2f(1, 0); glVertex3f( size, -size, -size);
            glTexCoord2f(1, 1); glVertex3f( size, -size,  size);

            glTexCoord2f(0, 1); glVertex3f(-size, size,  size);
            glTexCoord2f(0, 0); glVertex3f(-size, size, -size);
            glTexCoord2f(1, 0); glVertex3f( size, size, -size);
            glTexCoord2f(1, 1); glVertex3f( size, size,  size);

        glEnd();

        // Draw some text on top of our OpenGL object
        window.SaveGLStates();
        sf::Text text("SFML / OpenGL demo");
        text.SetPosition(250.f, 450.f);
        text.SetColor(sf::Color(255, 255, 255, 170));
        window.Draw(text);
        window.RestoreGLStates();

        // Finally, display the rendered frame on screen
        window.Display();
    }

    // Don't forget to destroy our texture
    glDeleteTextures(1, &texture);

    return EXIT_SUCCESS;
}
