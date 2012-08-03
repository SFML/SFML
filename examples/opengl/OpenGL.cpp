
////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>


////////////////////////////////////////////////////////////
/// Entry point of application
///
/// \return Application exit code
///
////////////////////////////////////////////////////////////
int main()
{
    // Create the main window
    sf::RenderWindow window(sf::VideoMode(800, 600), "SFML OpenGL", sf::Style::Default, sf::ContextSettings(32));
    window.setVerticalSyncEnabled(true);

    // Load a font for drawing some text
    sf::Font font;
    if (!font.loadFromFile("resources/sansation.ttf"))
        return EXIT_FAILURE;

    // Create a sprite for the background
    sf::Texture backgroundTexture;
    if (!backgroundTexture.loadFromFile("resources/background.jpg"))
        return EXIT_FAILURE;
    sf::Sprite background(backgroundTexture);

    // Load an OpenGL texture.
    // We could directly use a sf::Texture as an OpenGL texture (with its Bind() member function),
    // but here we want more control on it (generate mipmaps, ...) so we create a new one from an image
    GLuint texture = 0;
    {
        sf::Image image;
        if (!image.loadFromFile("resources/texture.jpg"))
            return EXIT_FAILURE;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, image.getSize().x, image.getSize().y, GL_RGBA, GL_UNSIGNED_BYTE, image.getPixelsPtr());
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
    while (window.isOpen())
    {
        // Process events
        sf::Event event;
        while (window.pollEvent(event))
        {
            // Close window : exit
            if (event.type == sf::Event::Closed)
                window.close();

            // Escape key : exit
            if ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Escape))
                window.close();

            // Adjust the viewport when the window is resized
            if (event.type == sf::Event::Resized)
                glViewport(0, 0, event.size.width, event.size.height);
        }

        // Draw the background
        window.pushGLStates();
        window.draw(background);
        window.popGLStates();

        // Activate the window before using OpenGL commands.
        // This is useless here because we have only one window which is
        // always the active one, but don't forget it if you use multiple windows
        window.setActive();

        // Clear the depth buffer
        glClear(GL_DEPTH_BUFFER_BIT);

        // We get the position of the mouse cursor, so that we can move the box accordingly
        float x =  sf::Mouse::getPosition(window).x * 200.f / window.getSize().x - 100.f;
        float y = -sf::Mouse::getPosition(window).y * 200.f / window.getSize().y + 100.f;

        // Apply some transformations
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        glTranslatef(x, y, -100.f);
        glRotatef(clock.getElapsedTime().asSeconds() * 50.f, 1.f, 0.f, 0.f);
        glRotatef(clock.getElapsedTime().asSeconds() * 30.f, 0.f, 1.f, 0.f);
        glRotatef(clock.getElapsedTime().asSeconds() * 90.f, 0.f, 0.f, 1.f);

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
        window.pushGLStates();
        sf::Text text("SFML / OpenGL demo", font);
        text.setColor(sf::Color(255, 255, 255, 170));
        text.setPosition(250.f, 450.f);
        window.draw(text);
        window.popGLStates();

        // Finally, display the rendered frame on screen
        window.display();
    }

    // Don't forget to destroy our texture
    glDeleteTextures(1, &texture);

    return EXIT_SUCCESS;
}
