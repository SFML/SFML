[![SFML logo](https://www.sfml-dev.org/images/logo.png)](https://www.sfml-dev.org)

# SFML — Simple and Fast Multimedia Library, raw input implementation

This repository includes a raw input implementation for Linux and Windows ([hornta's work](https://github.com/hornta/SFML-2.1-RAWINPUT)). It would be nice if somebody could work out an implementation for OSX.

# Installation
Refer to the official installation instructions.

# Usage
```c++
#include <SFML/Graphics.hpp>
#include <iostream>

int main()
{
        sf::RenderWindow window(sf::VideoMode(400, 400), "Test");
        window.setRawMouseEnabled(true);

        while (window.isOpen())
        {
                sf::Event event;
                while (window.pollEvent(event))
                {
                        if (event.type == sf::Event::Closed)
                        {
                                window.close();
                        }
                        else if (event.type == sf::Event::MouseMotion)
                        {
                                std::cout << "Motion: x = ";
                                std::cout << event.mouseMotion.x;
                                std::cout << ", y = ";
                                std::cout << event.mouseMotion.y;
                                std::cout << std::endl;
                        }
                }

                window.clear();
                window.display();
        }

        return EXIT_SUCCESS;
}
```
