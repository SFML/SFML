
////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Audio.hpp>
#include <iostream>
#include <string>

////////////////////////////////////////////////////////////
/// Play a sound
///
////////////////////////////////////////////////////////////
void playSound(const sf::SoundEffect& effect)
{
    // Load a sound buffer from a wav file
    sf::SoundBuffer buffer;
    if (!buffer.loadFromFile("resources/killdeer.wav"))
        return;

    // Display sound informations
    std::cout << "killdeer.wav:" << std::endl;
    std::cout << " " << buffer.getDuration().asSeconds() << " seconds"       << std::endl;
    std::cout << " " << buffer.getSampleRate()           << " samples / sec" << std::endl;
    std::cout << " " << buffer.getChannelCount()         << " channels"      << std::endl;

    // Create a sound instance and play it
    sf::Sound sound(buffer);

    for (int i = 0; i < 2; ++i)
    {
        sound.play();

        // Loop while the sound is playing
        while (sound.getStatus() == sf::Sound::Playing)
        {
            // Leave some CPU time for other processes
            sf::sleep(sf::milliseconds(100));

            // Display the playing position
            std::cout << "\rPlaying... " << sound.getPlayingOffset().asSeconds() << " sec        ";
            std::cout << std::flush;
        }

        // apply the effect
        sound.setEffect(&effect);
    }
    std::cout << std::endl << std::endl;
}


////////////////////////////////////////////////////////////
/// Play a music
///
////////////////////////////////////////////////////////////
void playMusic(const std::string& filename, const sf::SoundEffect& effect)
{
    // Load an ogg music file
    sf::Music music;
    if (!music.openFromFile("resources/" + filename))
        return;

    // Display music informations
    std::cout << filename << ":" << std::endl;
    std::cout << " " << music.getDuration().asSeconds() << " seconds"       << std::endl;
    std::cout << " " << music.getSampleRate()           << " samples / sec" << std::endl;
    std::cout << " " << music.getChannelCount()         << " channels"      << std::endl;

    // Play it
    for (int i = 0; i < 2; ++i)
    {
        music.play();

        // Loop while the music is playing
        while (music.getStatus() == sf::Music::Playing)
        {
            // Leave some CPU time for other processes
            sf::sleep(sf::milliseconds(100));

            // Display the playing position
            std::cout << "\rPlaying... " << music.getPlayingOffset().asSeconds() << " sec        ";
            std::cout << std::flush;
        }
        music.setPlayingOffset(sf::seconds(0.f));

        // Apply the effect the second time
        music.setEffect(&effect);
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
    // Create a reverb effect to assign to the sound
    sf::ReverbEffect reverbEffect;
    reverbEffect.setDecayTime(20.f);

    // Play a sound
    playSound(reverbEffect);

    // Create a chorus effect
    sf::ChorusEffect chorusEffect;
    chorusEffect.setFeedback(-0.69f);

    // Play music from an ogg file
    playMusic("doodle_pop.ogg", chorusEffect);

    // Play music from a flac file
    reverbEffect.setDecayTime(5.f);
    reverbEffect.setDiffusion(0.1f);
    playMusic("ding.flac", reverbEffect);

    // Play music from a mp3 file
    playMusic("ding.mp3");

    // Wait until the user presses 'enter' key
    std::cout << "Press enter to exit..." << std::endl;
    std::cin.ignore(10000, '\n');

    return EXIT_SUCCESS;
}
