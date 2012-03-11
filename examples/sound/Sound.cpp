
////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Audio.hpp>
#include <iomanip>
#include <iostream>


////////////////////////////////////////////////////////////
/// Play a sound
///
////////////////////////////////////////////////////////////
void playSound()
{
    // Load a sound buffer from a wav file
    sf::SoundBuffer buffer;
    if (!buffer.loadFromFile("resources/canary.wav"))
        return;

    // Display sound informations
    std::cout << "canary.wav :" << std::endl;
    std::cout << " " << buffer.getDuration().asSeconds() << " seconds"       << std::endl;
    std::cout << " " << buffer.getSampleRate()           << " samples / sec" << std::endl;
    std::cout << " " << buffer.getChannelCount()         << " channels"      << std::endl;

    // Create a sound instance and play it
    sf::Sound sound(buffer);
    sound.play();

    // Loop while the sound is playing
    while (sound.getStatus() == sf::Sound::Playing)
    {
        // Leave some CPU time for other processes
        sf::sleep(sf::milliseconds(100));

        // Display the playing position
        std::cout << "\rPlaying... " << std::fixed << std::setprecision(2) << sound.getPlayingOffset().asSeconds() << " sec   ";
        std::cout << std::flush;
    }
    std::cout << std::endl << std::endl;
}


////////////////////////////////////////////////////////////
/// Play a music
///
////////////////////////////////////////////////////////////
void playMusic()
{
    // Load an ogg music file
    sf::Music music;
    if (!music.openFromFile("resources/orchestral.ogg"))
        return;

    // Display music informations
    std::cout << "orchestral.ogg :" << std::endl;
    std::cout << " " << music.getDuration().asSeconds() << " seconds"       << std::endl;
    std::cout << " " << music.getSampleRate()           << " samples / sec" << std::endl;
    std::cout << " " << music.getChannelCount()         << " channels"      << std::endl;

    // Play it
    music.play();

    // Loop while the music is playing
    while (music.getStatus() == sf::Music::Playing)
    {
        // Leave some CPU time for other processes
        sf::sleep(sf::milliseconds(100));

        // Display the playing position
        std::cout << "\rPlaying... " << std::fixed << std::setprecision(2) << music.getPlayingOffset().asSeconds() << " sec   ";
        std::cout << std::flush;
    }
    std::cout << std::endl;
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

    // Play a music
    playMusic();

    // Wait until the user presses 'enter' key
    std::cout << "Press enter to exit..." << std::endl;
    std::cin.ignore(10000, '\n');

    return EXIT_SUCCESS;
}
