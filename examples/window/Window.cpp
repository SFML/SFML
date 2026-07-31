////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Window.hpp>

#include <cstdlib>

#define GLAD_GL_IMPLEMENTATION
#include <GL2Utils.hpp>

#ifdef SFML_SYSTEM_IOS
#include <SFML/Main.hpp>
#endif

#include <array>
#include <iostream>

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
    if (!gladLoadGLES2(sf::Context::getFunction))
#else
    if (!gladLoadGL(sf::Context::getFunction))
#endif
    {
        std::cerr << "Failed to load OpenGL entry points" << std::endl;
        return EXIT_FAILURE;
    }

    gl2::Program program;
    if (!program.load(gl2::colorVertexShader, gl2::colorFragmentShader, {{0, "position"}, {1, "color"}}))
        return EXIT_FAILURE;

    program.use();
    const GLint projectionUniform = program.uniform("projection");
    const GLint modelViewUniform  = program.uniform("modelView");

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

    // Configure the viewport (the same size as the window)
    glViewport(0, 0, static_cast<GLsizei>(window.getSize().x), static_cast<GLsizei>(window.getSize().y));

    // Setup a perspective projection
    const GLfloat ratio = static_cast<float>(window.getSize().x) / static_cast<float>(window.getSize().y);
    gl2::setMatrix(projectionUniform, gl2::frustum(-ratio, ratio, -1.f, 1.f, 1.f, 500.f));

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
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), cube.data());
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), cube.data() + 3);

    // Create a clock for measuring the time elapsed
    const sf::Clock clock;
    bool            running = true;

    // Start the game loop
    while (running)
    {
        // Process events
        while (const std::optional event = window.pollEvent())
        {
            // Window closed or escape key pressed: exit
            if (event->is<sf::Event::Closed>() ||
                (event->is<sf::Event::KeyPressed>() &&
                 event->getIf<sf::Event::KeyPressed>()->code == sf::Keyboard::Key::Escape))
            {
                running = false;
            }

            // Resize event: adjust the viewport
            if (const auto* resized = event->getIf<sf::Event::Resized>())
            {
                const auto [width, height] = resized->size;
                glViewport(0, 0, static_cast<GLsizei>(width), static_cast<GLsizei>(height));
                const GLfloat newRatio = static_cast<float>(width) / static_cast<float>(height);
                gl2::setMatrix(projectionUniform, gl2::frustum(-newRatio, newRatio, -1.f, 1.f, 1.f, 500.f));
            }
        }

        if (!running)
            break;

        // Clear the color and depth buffers
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Apply some transformations to rotate the cube
        const float elapsedTime = clock.getElapsedTime().asSeconds();
        gl2::setMatrix(modelViewUniform,
                       gl2::modelView(0.f, 0.f, -200.f, elapsedTime * 50.f, elapsedTime * 30.f, elapsedTime * 90.f));

        // Draw the cube
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // Finally, display the rendered frame on screen
        window.display();
    }

    glUseProgram(0);
    program.reset();
    window.close();
    return EXIT_SUCCESS;
}
