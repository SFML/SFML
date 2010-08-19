////////////////////////////////////////////////////////////
/// \mainpage
///
/// \section welcome Welcome
/// Welcome to the official SFML documentation. Here you will find a detailed
/// view of all the SFML <a href="./annotated.htm">classes</a>, as well as source
/// <a href="./files.htm">files</a>. <br>
/// If you are looking for tutorials, you can visit the official website
/// at <a href="http://www.sfml-dev.org/tutorials/">www.sfml-dev.org</a>.
///
/// \section example Short example
/// Here is a short example, to show you how simple it is to use SFML :
///
/// \code
///
/// #include <SFML/Audio.hpp>
/// #include <SFML/Graphics.hpp>
/// 
/// int main()
/// {
///     // Create the main window
///     sf::RenderWindow window(sf::VideoMode(800, 600), "SFML window");
/// 
///     // Load a sprite to display
///     sf::Image image;
///     if (!image.LoadFromFile("cute_image.jpg"))
///         return EXIT_FAILURE;
///     sf::Sprite sprite(image);
/// 
///     // Create a graphical text to display
///     sf::Font font;
///     if (!font.LoadFromFile("arial.ttf"))
///         return EXIT_FAILURE;
///     sf::Text text("Hello SFML", font, 50);
/// 
///     // Load a music to play
///     sf::Music music;
///     if (!music.OpenFromFile("nice_music.ogg"))
///         return EXIT_FAILURE;
///
///     // Play the music
///     music.Play();
/// 
///     // Start the game loop
///     while (window.IsOpened())
///     {
///         // Process events
///         sf::Event event;
///         while (window.GetEvent(event))
///         {
///             // Close window : exit
///             if (event.Type == sf::Event::Closed)
///                 window.Close();
///         }
/// 
///         // Clear screen
///         window.Clear();
/// 
///         // Draw the sprite
///         window.Draw(sprite);
/// 
///         // Draw the string
///         window.Draw(text);
/// 
///         // Update the window
///         window.Display();
///     }
/// 
///     return EXIT_SUCCESS;
/// }
/// \endcode
////////////////////////////////////////////////////////////
