#include <iostream>

#include <SFML/OpenGL.hpp>
#include <SFML/Window.hpp>

int main(int, char**) {
    sf::Window window;
    window.create(sf::VideoMode(800, 600), "RawInput", sf::Style::Close);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            else if (event.type == sf::Event::MouseButtonPressed) {
                std::cout << "Mouse button pressed" << std::endl;
            }
            else if (event.type == sf::Event::MouseButtonReleased) {
                std::cout << "Mouse button released" << std::endl;
            }
            else if (event.type == sf::Event::MouseMovedRaw) {
                std::cout << "Mouse moved raw: " << event.mouseMoveRaw.deltaX << ", " << event.mouseMoveRaw.deltaY << std::endl;
            }
        }

        glClear(GL_COLOR_BUFFER_BIT);
        window.display();
    }

    return 0;
}
