
////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Audio.hpp>
#include <iostream>
#include <string>

using namespace std::chrono_literals;

////////////////////////////////////////////////////////////
/// Play a sound
///
////////////////////////////////////////////////////////////
void playSound()
{
    // Load a sound buffer from a wav file
    sf::SoundBuffer buffer;
    if (!buffer.loadFromFile("resources/killdeer.wav"))
        return;

    // Display sound informations
    std::cout << "killdeer.wav:" << std::endl;
    std::cout << " " << buffer.getDuration().count() << " seconds"       << std::endl;
    std::cout << " " << buffer.getSampleRate()       << " samples / sec" << std::endl;
    std::cout << " " << buffer.getChannelCount()     << " channels"      << std::endl;

    // Create a sound instance and play it
    sf::Sound sound(buffer);
    sound.play();

    // Loop while the sound is playing
    while (sound.getStatus() == sf::Sound::Playing)
    {
        // Leave some CPU time for other processes
        sf::sleep_for(100ms);

        // Display the playing position
        std::cout << "\rPlaying... " << sound.getPlayingOffset().count() << " sec        ";
        std::cout << std::flush;
    }
    std::cout << std::endl << std::endl;
}


////////////////////////////////////////////////////////////
/// Play a music
///
////////////////////////////////////////////////////////////
void playMusic(const std::string& filename)
{
    // Load an ogg music file
    sf::Music music;
    if (!music.openFromFile("resources/" + filename))
        return;

    // Display music informations
    std::cout << filename << ":" << std::endl;
    std::cout << " " << music.getDuration().count() << " seconds"       << std::endl;
    std::cout << " " << music.getSampleRate()       << " samples / sec" << std::endl;
    std::cout << " " << music.getChannelCount()     << " channels"      << std::endl;

    // Play it
    music.play();

    // Loop while the music is playing
    while (music.getStatus() == sf::Music::Playing)
    {
        // Leave some CPU time for other processes
        sf::sleep_for(100ms);

        // Display the playing position
        std::cout << "\rPlaying... " << music.getPlayingOffset().count() << " sec        ";
        std::cout << std::flush;
    }
    std::cout << std::endl << std::endl;
}


////////////////////////////////////////////////////////////
/// Entry point of application
///
/// \return Application exit code
///
////////////////////////////////////////////////////////////
int main()
{
    // Play a sound
    playSound();

    // Play music from an ogg file
    playMusic("doodle_pop.ogg");

    // Play music from a flac file
    playMusic("ding.flac");

    // Wait until the user presses 'enter' key
    std::cout << "Press enter to exit..." << std::endl;
    std::cin.ignore(10000, '\n');

    return EXIT_SUCCESS;
}
