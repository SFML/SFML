
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
void PlaySound()
{
    // Load a sound buffer from a wav file
    sf::SoundBuffer buffer;
    if (!buffer.LoadFromFile("resources/canary.wav"))
        return;

    // Display sound informations
    std::cout << "canary.wav :" << std::endl;
    std::cout << " " << buffer.GetDuration()      << " sec"           << std::endl;
    std::cout << " " << buffer.GetSampleRate()    << " samples / sec" << std::endl;
    std::cout << " " << buffer.GetChannelsCount() << " channels"      << std::endl;

    // Create a sound instance and play it
    sf::Sound sound(buffer);
    sound.Play();

    // Loop while the sound is playing
    while (sound.GetStatus() == sf::Sound::Playing)
    {
        // Leave some CPU time for other processes
        sf::Sleep(0.1f);

        // Display the playing position
        std::cout << "\rPlaying... " << std::fixed << std::setprecision(2) << sound.GetPlayingOffset() << " sec   ";
    }
    std::cout << std::endl << std::endl;
}


////////////////////////////////////////////////////////////
/// Play a music
///
////////////////////////////////////////////////////////////
void PlayMusic()
{
    // Load an ogg music file
    sf::Music music;
    if (!music.OpenFromFile("resources/orchestral.ogg"))
        return;

    // Display music informations
    std::cout << "orchestral.ogg :" << std::endl;
    std::cout << " " << music.GetDuration()      << " sec"           << std::endl;
    std::cout << " " << music.GetSampleRate()    << " samples / sec" << std::endl;
    std::cout << " " << music.GetChannelsCount() << " channels"      << std::endl;

    // Play it
    music.Play();

    // Loop while the music is playing
    while (music.GetStatus() == sf::Music::Playing)
    {
        // Leave some CPU time for other processes
        sf::Sleep(0.1f);

        // Display the playing position
        std::cout << "\rPlaying... " << std::fixed << std::setprecision(2) << music.GetPlayingOffset() << " sec   ";
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
    PlaySound();

    // Play a music
    PlayMusic();

    // Wait until the user presses 'enter' key
    std::cout << "Press enter to exit..." << std::endl;
    std::cin.ignore(10000, '\n');

    return EXIT_SUCCESS;
}
