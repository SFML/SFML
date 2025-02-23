////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Audio.hpp>

#include <iostream>

#include <cstdlib>


////////////////////////////////////////////////////////////
/// Entry point of application
///
/// \return Application exit code
///
////////////////////////////////////////////////////////////
int main()
{
    // Check that the device can capture audio
    if (!sf::SoundRecorder::isAvailable())
    {
        std::cout << "Sorry, audio capture is not supported by your system" << std::endl;
        return EXIT_SUCCESS;
    }

    // List the available capture devices
    auto devices = sf::SoundRecorder::getAvailableDevices();

    std::cout << "Available capture devices:\n" << std::endl;

    for (auto i = 0u; i < devices.size(); ++i)
        std::cout << i << ": " << devices[i] << '\n';

    std::cout << std::endl;

    std::size_t deviceIndex = 0;

    // Choose the capture device
    if (devices.size() > 1)
    {
        deviceIndex = devices.size();
        std::cout << "Please choose the capture device to use [0-" << devices.size() - 1 << "]: ";
        do
        {
            std::cin >> deviceIndex;
            std::cin.ignore(10'000, '\n');
        } while (deviceIndex >= devices.size());
    }

    // Choose the sample rate
    unsigned int sampleRate = 0;
    std::cout << "Please choose the sample rate for sound capture (44100 is CD quality): ";
    std::cin >> sampleRate;
    std::cin.ignore(10'000, '\n');

    // Wait for user input...
    std::cout << "Press enter to start recording audio";
    std::cin.ignore(10'000, '\n');

    // Here we'll use an integrated custom recorder, which saves the captured data into a SoundBuffer
    sf::SoundBufferRecorder recorder;

    if (!recorder.setDevice(devices[deviceIndex]))
    {
        std::cerr << "Failed to set the capture device" << std::endl;
        return EXIT_FAILURE;
    }

    // Audio capture is done in a separate thread, so we can block the main thread while it is capturing
    if (!recorder.start(sampleRate))
    {
        std::cerr << "Failed to start recorder" << std::endl;
        return EXIT_FAILURE;
    }

    std::cout << "Recording... press enter to stop";
    std::cin.ignore(10'000, '\n');
    recorder.stop();

    // Get the buffer containing the captured data
    const sf::SoundBuffer& buffer = recorder.getBuffer();

    // Display captured sound information
    std::cout << "Sound information:" << '\n'
              << " " << buffer.getDuration().asSeconds() << " seconds" << '\n'
              << " " << buffer.getSampleRate() << " samples / seconds" << '\n'
              << " " << buffer.getChannelCount() << " channels" << std::endl;

    // Choose what to do with the recorded sound data
    char choice = 0;
    std::cout << "What do you want to do with captured sound (p = play, s = save) ? ";
    std::cin >> choice;
    std::cin.ignore(10'000, '\n');

    if (choice == 's')
    {
        // Choose the filename
        std::string filename;
        std::cout << "Choose the file to create: ";
        std::getline(std::cin, filename);

        // Save the buffer
        if (!buffer.saveToFile(filename))
            std::cerr << "Could not save sound buffer to file" << std::endl;
    }
    else
    {
        // Create a sound instance and play it
        sf::Sound sound(buffer);
        sound.play();

        // Wait until finished
        while (sound.getStatus() == sf::Sound::Status::Playing)
        {
            // Display the playing position
            std::cout << "\rPlaying... " << sound.getPlayingOffset().asSeconds() << " sec        ";
            std::cout << std::flush;

            // Leave some CPU time for other threads
            sf::sleep(sf::milliseconds(100));
        }
    }

    // Finished!
    std::cout << '\n' << "Done!" << std::endl;

    // Wait until the user presses 'enter' key
    std::cout << "Press enter to exit..." << std::endl;
    std::cin.ignore(10'000, '\n');
}
