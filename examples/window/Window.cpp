////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Window.hpp>

#include <cstdlib>

#define GLAD_GL_IMPLEMENTATION
#include <gl.h>

#ifdef SFML_SYSTEM_IOS
#include <SFML/Main.hpp>
#endif

#include <array>
#include <iostream>

#include <cstdlib>

////////////////////////////////////////////////////////////
/// Entry point of application
///
/// \return Application exit code
///
////////////////////////////////////////////////////////////
int main()
{
    // Request a 24-bits depth buffer when creating the window
    sf::ContextSettings contextSettings;
    contextSettings.depthBits = 24;

    // Create the main window
    sf::Window window(sf::VideoMode({640, 480}), "SFML window with OpenGL", sf::Style::Default, sf::State::Windowed, contextSettings);

    // Make it the active window for OpenGL calls
    if (!window.setActive())
    {
        std::cerr << "Failed to set the window as active" << std::endl;
        return EXIT_FAILURE;
    }

    // Load OpenGL or OpenGL ES entry points using glad
#ifdef SFML_OPENGL_ES
    gladLoadGLES1(sf::Context::getFunction);
#else
    gladLoadGL(sf::Context::getFunction);
#endif

    // Set the color and depth clear values
#ifdef SFML_OPENGL_ES
    glClearDepthf(1.f);
#else
    glClearDepth(1.f);
#endif
    glClearColor(0.f, 0.f, 0.f, 1.f);

    // Enable Z-buffer read and write
    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);

    // Disable lighting and texturing
    glDisable(GL_LIGHTING);
    glDisable(GL_TEXTURE_2D);

    // Configure the viewport (the same size as the window)
    glViewport(0, 0, static_cast<GLsizei>(window.getSize().x), static_cast<GLsizei>(window.getSize().y));

    // Setup a perspective projection
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    const GLfloat ratio = static_cast<float>(window.getSize().x) / static_cast<float>(window.getSize().y);
#ifdef SFML_OPENGL_ES
    glFrustumf(-ratio, ratio, -1.f, 1.f, 1.f, 500.f);
#else
    glFrustum(-ratio, ratio, -1.f, 1.f, 1.f, 500.f);
#endif

    // Define a 3D cube (6 faces made of 2 triangles composed by 3 vertices)
    // clang-format off
    constexpr std::array<GLfloat, 252> cube =
    {
        // positions    // colors (r, g, b, a)
        -50, -50, -50,  0, 0, 1, 1,
        -50,  50, -50,  0, 0, 1, 1,
        -50, -50,  50,  0, 0, 1, 1,
        -50, -50,  50,  0, 0, 1, 1,
        -50,  50, -50,  0, 0, 1, 1,
        -50,  50,  50,  0, 0, 1, 1,

         50, -50, -50,  0, 1, 0, 1,
         50,  50, -50,  0, 1, 0, 1,
         50, -50,  50,  0, 1, 0, 1,
         50, -50,  50,  0, 1, 0, 1,
         50,  50, -50,  0, 1, 0, 1,
         50,  50,  50,  0, 1, 0, 1,

        -50, -50, -50,  1, 0, 0, 1,
         50, -50, -50,  1, 0, 0, 1,
        -50, -50,  50,  1, 0, 0, 1,
        -50, -50,  50,  1, 0, 0, 1,
         50, -50, -50,  1, 0, 0, 1,
         50, -50,  50,  1, 0, 0, 1,

        -50,  50, -50,  0, 1, 1, 1,
         50,  50, -50,  0, 1, 1, 1,
        -50,  50,  50,  0, 1, 1, 1,
        -50,  50,  50,  0, 1, 1, 1,
         50,  50, -50,  0, 1, 1, 1,
         50,  50,  50,  0, 1, 1, 1,

        -50, -50, -50,  1, 0, 1, 1,
         50, -50, -50,  1, 0, 1, 1,
        -50,  50, -50,  1, 0, 1, 1,
        -50,  50, -50,  1, 0, 1, 1,
         50, -50, -50,  1, 0, 1, 1,
         50,  50, -50,  1, 0, 1, 1,

        -50, -50,  50,  1, 1, 0, 1,
         50, -50,  50,  1, 1, 0, 1,
        -50,  50,  50,  1, 1, 0, 1,
        -50,  50,  50,  1, 1, 0, 1,
         50, -50,  50,  1, 1, 0, 1,
         50,  50,  50,  1, 1, 0, 1,
    };
    // clang-format on

    // Enable position and color vertex components
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);
    glVertexPointer(3, GL_FLOAT, 7 * sizeof(GLfloat), cube.data());
    glColorPointer(4, GL_FLOAT, 7 * sizeof(GLfloat), cube.data() + 3);

    // Disable normal and texture coordinates vertex components
    glDisableClientState(GL_NORMAL_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);

    // Create a clock for measuring the time elapsed
    const sf::Clock clock;

    // Start the game loop
    while (window.isOpen())
    {
        // Process events
        for (sf::Event event; window.pollEvent(event);)
        {
            // Close window: exit
            if (event.type == sf::Event::Closed)
                window.close();

            // Escape key: exit
            if ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Key::Escape))
                window.close();

            // Resize event: adjust the viewport
            if (event.type == sf::Event::Resized)
            {
                glViewport(0, 0, static_cast<GLsizei>(event.size.width), static_cast<GLsizei>(event.size.height));
                glMatrixMode(GL_PROJECTION);
                glLoadIdentity();
                const GLfloat newRatio = static_cast<float>(event.size.width) / static_cast<float>(event.size.height);
#ifdef SFML_OPENGL_ES
                glFrustumf(-newRatio, newRatio, -1.f, 1.f, 1.f, 500.f);
#else
                glFrustum(-newRatio, newRatio, -1.f, 1.f, 1.f, 500.f);
#endif
            }
        }

        // Clear the color and depth buffers
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Apply some transformations to rotate the cube
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        glTranslatef(0.f, 0.f, -200.f);
        glRotatef(clock.getElapsedTime().asSeconds() * 50, 1.f, 0.f, 0.f);
        glRotatef(clock.getElapsedTime().asSeconds() * 30, 0.f, 1.f, 0.f);
        glRotatef(clock.getElapsedTime().asSeconds() * 90, 0.f, 0.f, 1.f);

        // Draw the cube
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // Finally, display the rendered frame on screen
        window.display();
    }

    return EXIT_SUCCESS;
}
