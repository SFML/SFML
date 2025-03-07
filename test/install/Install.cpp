#include <SFML/Graphics.hpp>

#include <SFML/Audio.hpp>

#include <SFML/Network.hpp>

// Instantiate some types from each module to test for linker issues. This program is not meant be ran.
int main()
{
    // Audio
    [[maybe_unused]] const sf::SoundBufferRecorder soundBufferRecorder;

    // Graphics
    [[maybe_unused]] const sf::Color          color;
    [[maybe_unused]] const sf::IntRect        rect;
    [[maybe_unused]] const sf::RenderWindow   renderWindow;
    [[maybe_unused]] const sf::RectangleShape rectangleShape;
    [[maybe_unused]] const sf::Vertex         vertex;

    // Network
    [[maybe_unused]] const sf::Ftp       ftp;
    [[maybe_unused]] const sf::Http      http;
    [[maybe_unused]] const sf::Packet    packet;
    [[maybe_unused]] const sf::UdpSocket udpSocket;

    // System
    [[maybe_unused]] const sf::Angle  angle;
    [[maybe_unused]] const sf::String string;
    [[maybe_unused]] const sf::Time   time;

    // Window
    [[maybe_unused]] const sf::Context   context;
    [[maybe_unused]] const sf::VideoMode videoMode;
    [[maybe_unused]] const sf::Window    window;
}
