
////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Graphics.hpp>
#include <iostream>
#include <cstdlib>

#define GLAD_GL_IMPLEMENTATION
#include <gl.h>

#ifdef SFML_SYSTEM_IOS
#include <SFML/Main.hpp>
#endif

#ifndef GL_SRGB8_ALPHA8
#define GL_SRGB8_ALPHA8 0x8C43
#endif

std::string resourcesDir()
{
#ifdef SFML_SYSTEM_IOS
    return "";
#else
    return "resources/";
#endif
}

////////////////////////////////////////////////////////////
/// Entry point of application
///
/// \return Application exit code
///
////////////////////////////////////////////////////////////
int main()
{
    bool exit = false;
    bool sRgb = false;

    while (!exit)
    {
        // Request a 24-bits depth buffer when creating the window
        sf::ContextSettings contextSettings;
        contextSettings.depthBits = 24;
        contextSettings.sRgbCapable = sRgb;

        // Create the main window
        sf::RenderWindow window(sf::VideoMode(800, 600), "SFML graphics with OpenGL", sf::Style::Default, contextSettings);
        window.setVerticalSyncEnabled(true);

        // Create a sprite for the background
        sf::Texture backgroundTexture;
        backgroundTexture.setSrgb(sRgb);
        if (!backgroundTexture.loadFromFile(resourcesDir() + "background.jpg"))
            return EXIT_FAILURE;
        sf::Sprite background(backgroundTexture);

        // Create some text to draw on top of our OpenGL object
        sf::Font font;
        if (!font.loadFromFile(resourcesDir() + "tuffy.ttf"))
            return EXIT_FAILURE;

        sf::Text text("SFML / OpenGL demo", font);
        sf::Text sRgbInstructions("Press space to toggle sRGB conversion", font);
        sf::Text mipmapInstructions("Press return to toggle mipmapping", font);
        text.setFillColor(sf::Color(255, 255, 255, 170));
        sRgbInstructions.setFillColor(sf::Color(255, 255, 255, 170));
        mipmapInstructions.setFillColor(sf::Color(255, 255, 255, 170));
        text.setPosition({280.f, 450.f});
        sRgbInstructions.setPosition({175.f, 500.f});
        mipmapInstructions.setPosition({200.f, 550.f});

        // Load a texture to apply to our 3D cube
        sf::Texture texture;
        if (!texture.loadFromFile(resourcesDir() + "logo.png"))
            return EXIT_FAILURE;

        // Attempt to generate a mipmap for our cube texture
        // We don't check the return value here since
        // mipmapping is purely optional in this example
        (void) texture.generateMipmap();

        // Make the window the active window for OpenGL calls
        if (!window.setActive(true))
        {
            std::cerr << "Failed to set window to active" << std::endl;
            return EXIT_FAILURE;
        }

        // Load OpenGL or OpenGL ES entry points using glad
#ifdef SFML_OPENGL_ES
        gladLoadGLES1(reinterpret_cast<GLADloadfunc>(sf::Context::getFunction));
#else
        gladLoadGL(reinterpret_cast<GLADloadfunc>(sf::Context::getFunction));
#endif

        // Enable Z-buffer read and write
        glEnable(GL_DEPTH_TEST);
        glDepthMask(GL_TRUE);
#ifdef SFML_OPENGL_ES
        glClearDepthf(1.f);
#else
        glClearDepth(1.f);
#endif

        // Disable lighting
        glDisable(GL_LIGHTING);

        // Configure the viewport (the same size as the window)
        glViewport(0, 0, static_cast<GLsizei>(window.getSize().x), static_cast<GLsizei>(window.getSize().y));

        // Setup a perspective projection
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        GLfloat ratio = static_cast<float>(window.getSize().x) / static_cast<float>(window.getSize().y);
#ifdef SFML_OPENGL_ES
        glFrustumf(-ratio, ratio, -1.f, 1.f, 1.f, 500.f);
#else
        glFrustum(-ratio, ratio, -1.f, 1.f, 1.f, 500.f);
#endif

        // Bind the texture
        glEnable(GL_TEXTURE_2D);
        sf::Texture::bind(&texture);

        // Define a 3D cube (6 faces made of 2 triangles composed by 3 vertices)
        static const GLfloat cube[] =
        {
            // positions    // texture coordinates
            -20, -20, -20,  0, 0,
            -20,  20, -20,  1, 0,
            -20, -20,  20,  0, 1,
            -20, -20,  20,  0, 1,
            -20,  20, -20,  1, 0,
            -20,  20,  20,  1, 1,

             20, -20, -20,  0, 0,
             20,  20, -20,  1, 0,
             20, -20,  20,  0, 1,
             20, -20,  20,  0, 1,
             20,  20, -20,  1, 0,
             20,  20,  20,  1, 1,

            -20, -20, -20,  0, 0,
             20, -20, -20,  1, 0,
            -20, -20,  20,  0, 1,
            -20, -20,  20,  0, 1,
             20, -20, -20,  1, 0,
             20, -20,  20,  1, 1,

            -20,  20, -20,  0, 0,
             20,  20, -20,  1, 0,
            -20,  20,  20,  0, 1,
            -20,  20,  20,  0, 1,
             20,  20, -20,  1, 0,
             20,  20,  20,  1, 1,

            -20, -20, -20,  0, 0,
             20, -20, -20,  1, 0,
            -20,  20, -20,  0, 1,
            -20,  20, -20,  0, 1,
             20, -20, -20,  1, 0,
             20,  20, -20,  1, 1,

            -20, -20,  20,  0, 0,
             20, -20,  20,  1, 0,
            -20,  20,  20,  0, 1,
            -20,  20,  20,  0, 1,
             20, -20,  20,  1, 0,
             20,  20,  20,  1, 1
        };

        // Enable position and texture coordinates vertex components
        glEnableClientState(GL_VERTEX_ARRAY);
        glEnableClientState(GL_TEXTURE_COORD_ARRAY);
        glVertexPointer(3, GL_FLOAT, 5 * sizeof(GLfloat), cube);
        glTexCoordPointer(2, GL_FLOAT, 5 * sizeof(GLfloat), cube + 3);

        // Disable normal and color vertex components
        glDisableClientState(GL_NORMAL_ARRAY);
        glDisableClientState(GL_COLOR_ARRAY);

        // Make the window no longer the active window for OpenGL calls
        if (!window.setActive(false))
        {
            std::cerr << "Failed to set window to inactive" << std::endl;
            return EXIT_FAILURE;
        }

        // Create a clock for measuring the time elapsed
        sf::Clock clock;

        // Flag to track whether mipmapping is currently enabled
        bool mipmapEnabled = true;

        // Start game loop
        while (window.isOpen())
        {
            // Process events
            sf::Event event;
            while (window.pollEvent(event))
            {
                // Close window: exit
                if (event.type == sf::Event::Closed)
                {
                    exit = true;
                    window.close();
                }

                // Escape key: exit
                if ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Escape))
                {
                    exit = true;
                    window.close();
                }

                // Return key: toggle mipmapping
                if ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Enter))
                {
                    if (mipmapEnabled)
                    {
                        // We simply reload the texture to disable mipmapping
                        if (!texture.loadFromFile(resourcesDir() + "logo.png"))
                            return EXIT_FAILURE;

                        mipmapEnabled = false;
                    }
                    else if (texture.generateMipmap())
                    {
                        mipmapEnabled = true;
                    }
                }

                // Space key: toggle sRGB conversion
                if ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Space))
                {
                    sRgb = !sRgb;
                    window.close();
                }

                // Adjust the viewport when the window is resized
                if (event.type == sf::Event::Resized)
                {
                    sf::Vector2u textureSize = backgroundTexture.getSize();

                    // Make the window the active window for OpenGL calls
                    if (!window.setActive(true))
                    {
                        std::cerr << "Failed to set window to active" << std::endl;
                        return EXIT_FAILURE;
                    }

                    glViewport(0, 0, static_cast<GLsizei>(event.size.width), static_cast<GLsizei>(event.size.height));
                    glMatrixMode(GL_PROJECTION);
                    glLoadIdentity();
                    GLfloat newRatio = static_cast<float>(event.size.width) / static_cast<float>(event.size.height);
#ifdef SFML_OPENGL_ES
                    glFrustumf(-newRatio, newRatio, -1.f, 1.f, 1.f, 500.f);
#else
                    glFrustum(-newRatio, newRatio, -1.f, 1.f, 1.f, 500.f);
#endif

                    // Make the window no longer the active window for OpenGL calls
                    if (!window.setActive(false))
                    {
                        std::cerr << "Failed to set window to inactive" << std::endl;
                        return EXIT_FAILURE;
                    }

                    sf::View view;
                    view.setSize(sf::Vector2f(textureSize));
                    view.setCenter(sf::Vector2f(textureSize) / 2.f);
                    window.setView(view);
                }
            }

            // Draw the background
            window.pushGLStates();
            window.draw(background);
            window.popGLStates();

            // Make the window the active window for OpenGL calls
            if (!window.setActive(true))
            {
                // On failure, try re-creating the window, as it is intentionally 
                // closed when changing color space.
                continue;
            }

            // Clear the depth buffer
            glClear(GL_DEPTH_BUFFER_BIT);

            // We get the position of the mouse cursor (or touch), so that we can move the box accordingly
            sf::Vector2i pos;

            #ifdef SFML_SYSTEM_IOS
            pos = sf::Touch::getPosition(0);
            #else
            pos = sf::Mouse::getPosition(window);
            #endif

            float x = static_cast<float>(pos.x) * 200.f / static_cast<float>(window.getSize().x) - 100.f;
            float y = -static_cast<float>(pos.y) * 200.f / static_cast<float>(window.getSize().y) + 100.f;

            // Apply some transformations
            glMatrixMode(GL_MODELVIEW);
            glLoadIdentity();
            glTranslatef(x, y, -100.f);
            glRotatef(clock.getElapsedTime().asSeconds() * 50.f, 1.f, 0.f, 0.f);
            glRotatef(clock.getElapsedTime().asSeconds() * 30.f, 0.f, 1.f, 0.f);
            glRotatef(clock.getElapsedTime().asSeconds() * 90.f, 0.f, 0.f, 1.f);

            // Draw the cube
            glDrawArrays(GL_TRIANGLES, 0, 36);

            // Make the window no longer the active window for OpenGL calls
            if (!window.setActive(false))
            {
                std::cerr << "Failed to set window to inactive" << std::endl;
                return EXIT_FAILURE;
            }

            // Draw some text on top of our OpenGL object
            window.pushGLStates();
            window.draw(text);
            window.draw(sRgbInstructions);
            window.draw(mipmapInstructions);
            window.popGLStates();

            // Finally, display the rendered frame on screen
            window.display();
        }
    }

    return EXIT_SUCCESS;
}
