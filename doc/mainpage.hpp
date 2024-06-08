////////////////////////////////////////////////////////////
/// \mainpage SFML Documentation
///
/// \section welcome Welcome
/// Welcome to the official SFML documentation. Here you will find a detailed
/// view of all the SFML classes and functions. <br/>
/// If you are looking for tutorials, you can visit the official website
/// at <a href="https://www.sfml-dev.org/">www.sfml-dev.org</a>.
///
/// \section example Short example
/// Here is a short example, to show you how simple it is to use SFML:
///
/// \code
///
/// #include <SFML/Audio.hpp>
/// #include <SFML/Graphics.hpp>
///
/// int main()
/// {
///     // Create the main window
///     sf::RenderWindow window(sf::VideoMode({800, 600}), "SFML window");
///
///     // Load a texture to display
///     const auto texture = sf::Texture::loadFromFile("cute_image.jpg").value();
///
///     // Create geometry for the sprite that will display the texture
///     sf::SpriteGeometry geometry(texture.getRect());
///
///     // Create a graphical text to display
///     const auto font = sf::Font::loadFromFile("arial.ttf").value();
///     sf::Text text(font, "Hello SFML", 50);
///
///     // Load a music to play
///     sf::Music music;
///     if (!music.openFromFile("nice_music.ogg"))
///         return EXIT_FAILURE;
///
///     // Play the music
///     music.play();
///
///     // Start the game loop
///     while (window.isOpen())
///     {
///         // Process events
///         while (const auto event = window.pollEvent())
///         {
///             // Close window: exit
///             if (event.is<sf::Event::Closed>())
///                 window.close();
///         }
///
///         // Clear screen
///         window.clear();
///
///         // Create and draw the sprite
///         window.draw(sf::Sprite(geometry, texture));
///
///         // Draw the string
///         window.draw(text);
///
///         // Update the window
///         window.display();
///     }
/// }
/// \endcode
////////////////////////////////////////////////////////////
