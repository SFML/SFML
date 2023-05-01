#include <SFML/Graphics.hpp>

#include <SFML/Audio.hpp>

#include <SFML/Network.hpp>

// Instantiate some types from each module to test for linker issues. This program is not meant be ran.
int main()
{
    // Audio
    [[maybe_unused]] sf::InputSoundFile      inputSoundFile;
    [[maybe_unused]] sf::SoundBufferRecorder soundBufferRecorder;
    [[maybe_unused]] sf::Music               music;
    [[maybe_unused]] sf::Sound               sound;

    // Graphics
    [[maybe_unused]] sf::Color        color;
    [[maybe_unused]] sf::Font         font;
    [[maybe_unused]] sf::RenderWindow renderWindow;
    [[maybe_unused]] sf::Sprite       sprite;
    [[maybe_unused]] sf::Vertex       vertex;

    // Network
    [[maybe_unused]] sf::Ftp       ftp;
    [[maybe_unused]] sf::Http      http;
    [[maybe_unused]] sf::Packet    packet;
    [[maybe_unused]] sf::UdpSocket udpSocket;

    // System
    [[maybe_unused]] sf::Angle           angle;
    [[maybe_unused]] sf::FileInputStream fileInputStream;
    [[maybe_unused]] sf::String          string;
    [[maybe_unused]] sf::Time            time;

    // Window
    [[maybe_unused]] sf::Context   context;
    [[maybe_unused]] sf::VideoMode videoMode;
    [[maybe_unused]] sf::Window    window;
}
