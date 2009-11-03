
////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Audio.hpp>
#include <iomanip>
#include <iostream>


////////////////////////////////////////////////////////////
/// Entry point of application
///
/// \return Application exit code
///
////////////////////////////////////////////////////////////
int main()
{
    // Check that the device can capture audio
    if (sf::SoundRecorder::IsAvailable() == false)
    {
        std::cout << "Sorry, audio capture is not supported by your system" << std::endl;
        return EXIT_SUCCESS;
    }

    // Choose the sample rate
    unsigned int sampleRate;
    std::cout << "Please choose the sample rate for sound capture (44100 is CD quality) : ";
    std::cin  >> sampleRate;
    std::cin.ignore(10000, '\n');

    // Wait for user input...
    std::cout << "Press enter to start recording audio";
    std::cin.ignore(10000, '\n');

    // Here we'll use an integrated custom recorder, which saves the captured data into a SoundBuffer
    sf::SoundBufferRecorder recorder;

    // Audio capture is done in a separate thread, so we can block the main thread while it is capturing
    recorder.Start(sampleRate);
    std::cout << "Recording... press enter to stop";
    std::cin.ignore(10000, '\n');
    recorder.Stop();

    // Get the buffer containing the captured data
    const sf::SoundBuffer& buffer = recorder.GetBuffer();

    // Display captured sound informations
    std::cout << "Sound information :" << std::endl;
    std::cout << " " << buffer.GetDuration()      << " seconds"           << std::endl;
    std::cout << " " << buffer.GetSampleRate()    << " samples / seconds" << std::endl;
    std::cout << " " << buffer.GetChannelsCount() << " channels"          << std::endl;

    // Choose what to do with the recorded sound data
    char choice;
    std::cout << "What do you want to do with captured sound (p = play, s = save) ? ";
    std::cin  >> choice;
    std::cin.ignore(10000, '\n');

    if (choice == 's')
    {
        // Choose the filename
        std::string filename;
        std::cout << "Choose the file to create : ";
        std::getline(std::cin, filename);

        // Save the buffer
        buffer.SaveToFile(filename);
    }
    else
    {
        // Create a sound instance and play it
        sf::Sound sound(buffer);
        sound.Play();

        // Wait until finished
        while (sound.GetStatus() == sf::Sound::Playing)
        {
            // Display the playing position
            std::cout << "\rPlaying... " << std::fixed << std::setprecision(2) << sound.GetPlayingOffset() << " sec";

            // Leave some CPU time for other threads
            sf::Sleep(0.1f);
        }
    }

    // Finished !
    std::cout << std::endl << "Done !" << std::endl;

    // Wait until the user presses 'enter' key
    std::cout << "Press enter to exit..." << std::endl;
    std::cin.ignore(10000, '\n');

    return EXIT_SUCCESS;
}
