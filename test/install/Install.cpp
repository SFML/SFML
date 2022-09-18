#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>

// Instantiate some types from each module to test for linker issues. This program is not meant be ran.
int main()
{
    // Audio
    sf::InputSoundFile input_sound_file;
    sf::Listener       listener;
    sf::Music          music;
    sf::Sound          sound;

    // Graphics
    sf::Color        color;
    sf::Font         font;
    sf::RenderWindow render_window;
    sf::Sprite       sprite;
    sf::Vertex       vertex;

    // Network
    sf::Ftp       ftp;
    sf::Http      http;
    sf::Packet    packet;
    sf::UdpSocket udp_socket;

    // System
    sf::Angle           angle;
    sf::FileInputStream file_input_stream;
    sf::String          string;
    sf::Time            time;

    // Window
    sf::Context   context;
    sf::VideoMode video_mode;
    sf::Window    window;
}
