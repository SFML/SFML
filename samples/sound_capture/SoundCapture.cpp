
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
    if (sf::SoundRecorder::CanCapture() == false)
    {
        std::cout << "Sorry, audio capture is not supported by your system" << std::endl;
        return EXIT_SUCCESS;
    }

    // Choose the sample rate
    unsigned int SampleRate;
    std::cout << "Please choose the sample rate for sound capture (44100 is CD quality) : ";
    std::cin  >> SampleRate;
    std::cin.ignore(10000, '\n');

    // Wait for user input...
    std::cout << "Press enter to start recording audio";
    std::cin.ignore(10000, '\n');

    // Here we'll use an integrated custom recorder, which saves the captured data into a SoundBuffer
    sf::SoundBufferRecorder Recorder;

    // Audio capture is done in a separate thread, so we can block the main thread while it is capturing
    Recorder.Start(SampleRate);
    std::cout << "Recording... press enter to stop";
    std::cin.ignore(10000, '\n');
    Recorder.Stop();

    // Get the buffer containing the captured data
    const sf::SoundBuffer& Buffer = Recorder.GetBuffer();

    // Display captured sound informations
    std::cout << "Sound information :" << std::endl;
    std::cout << " " << Buffer.GetDuration()      << " seconds"           << std::endl;
    std::cout << " " << Buffer.GetSampleRate()    << " samples / seconds" << std::endl;
    std::cout << " " << Buffer.GetChannelsCount() << " channels"          << std::endl;

    // Choose what to do with the recorded sound data
    char Choice;
    std::cout << "What do you want to do with captured sound (p = play, s = save) ? ";
    std::cin  >> Choice;
    std::cin.ignore(10000, '\n');

    if (Choice == 's')
    {
        // Choose the filename
        std::string Filename;
        std::cout << "Choose the file to create : ";
        std::getline(std::cin, Filename);

        // Save the buffer
        Buffer.SaveToFile(Filename);
    }
    else
    {
        // Create a sound instance and play it
        sf::Sound Sound(Buffer);
        Sound.Play();

        // Wait until finished
        while (Sound.GetStatus() == sf::Sound::Playing)
        {
            // Display the playing position
            std::cout << "\rPlaying... " << std::fixed << std::setprecision(2) << Sound.GetPlayingOffset() << " sec";

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
