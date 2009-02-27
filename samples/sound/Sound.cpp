
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
    sf::SoundBuffer Buffer;
    if (!Buffer.LoadFromFile("datas/sound/footsteps.wav"))
        return;

    // Display sound informations
    std::cout << "footsteps.wav :" << std::endl;
    std::cout << " " << Buffer.GetDuration()      << " sec"           << std::endl;
    std::cout << " " << Buffer.GetSampleRate()    << " samples / sec" << std::endl;
    std::cout << " " << Buffer.GetChannelsCount() << " channels"      << std::endl;

    // Create a sound instance and play it
    sf::Sound Sound(Buffer);
    Sound.Play();

    // Loop while the sound is playing
    while (Sound.GetStatus() == sf::Sound::Playing)
    {
        // Leave some CPU time for other processes
        sf::Sleep(0.1f);

        // Display the playing position
        std::cout << "\rPlaying... " << std::fixed << std::setprecision(2) << Sound.GetPlayingOffset() << " sec   ";
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
    sf::Music Music;
    if (!Music.OpenFromFile("datas/sound/lepidoptera.ogg"))
        return;

    // Display music informations
    std::cout << "lepidoptera.ogg :" << std::endl;
    std::cout << " " << Music.GetDuration()      << " sec"           << std::endl;
    std::cout << " " << Music.GetSampleRate()    << " samples / sec" << std::endl;
    std::cout << " " << Music.GetChannelsCount() << " channels"      << std::endl;

    // Play it
    Music.Play();

    // Loop while the music is playing
    while (Music.GetStatus() == sf::Music::Playing)
    {
        // Leave some CPU time for other processes
        sf::Sleep(0.1f);

        // Display the playing position
        std::cout << "\rPlaying... " << std::fixed << std::setprecision(2) << Music.GetPlayingOffset() << " sec   ";
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
