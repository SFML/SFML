////////////////////////////////////////////////////////////
/// \mainpage
///
/// \section welcome Welcome
/// Welcome to the official SFML documentation. Here you will find a detailed
/// view of all the SFML classes and functions. <br/>
/// If you are looking for tutorials, you can visit the official website
/// at <a href="https://www.sfml-dev.org/">www.sfml-dev.org</a>.
///
/// \section example Short example
/// Here is a short example, to show you how simple it is to use SFML:
//
///     sf::Sprite sprite
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
///         for (sf::Event event; window.pollEvent(event);)
///         {
///             // Close window: exit
///             if (event.type == sf::Event::Closed)
///                 window.close();
///         }
///
///         // Clear screen
///         window.clear();
///
///         // Draw the sprite
///         window.draw(sprite);
///
///         // Draw the string
///         window.draw(text);
///
///         // Update the window
///         window.display();
///     }
///
///     return EXIT_SUCCESS;
/// }
/// \endcode
////////////////////////////////////////////////////////////
