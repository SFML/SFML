////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Graphics.hpp>

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
        sf::err() << "Sorry, audio capture is not supported by your system" << std::endl;
        return EXIT_SUCCESS;
    }

    const auto devices = sf::SoundRecorder::getAvailableDevices();
    if (devices.empty())
    {
        sf::err() << "No sound recording devices available" << std::endl;
        return EXIT_SUCCESS;
    }

    sf::RenderWindow window(sf::VideoMode({800, 600}), "Sound capture example");
    const sf::Font   font("resources/tuffy.ttf");

    // List the available capture devices and display text for each
    std::vector<sf::Text> deviceTexts(devices.size(), {font});
    for (auto i = 0u; i < devices.size(); ++i)
    {
        deviceTexts[i].setString(devices[i]);
        deviceTexts[i].setPosition(
            {10, (static_cast<float>(deviceTexts[i].getCharacterSize()) + 10.f) * static_cast<float>(i)});
    }

    const auto lastDeviceTextBounds = deviceTexts.back().getGlobalBounds();
    const auto buttonYOffset        = lastDeviceTextBounds.position.y + lastDeviceTextBounds.size.y * 2;

    // An indicator to show current device
    sf::RectangleShape deviceIndicator;
    deviceIndicator.setFillColor(sf::Color::Transparent);
    deviceIndicator.setOutlineColor(sf::Color::White);
    deviceIndicator.setOutlineThickness(3);

    // A red circle "record" button
    sf::CircleShape recordButton(100);
    recordButton.setFillColor(sf::Color::Red);
    recordButton.setPosition({100, buttonYOffset});
    recordButton.setOutlineColor(sf::Color::Transparent);
    recordButton.setOutlineThickness(5);

    // A green triangle "play" button
    sf::CircleShape playButton(100, 3);
    playButton.setFillColor(sf::Color::Transparent);
    playButton.setPosition({static_cast<float>(window.getSize().x) - 100.f, buttonYOffset});
    playButton.setRotation(sf::degrees(90));
    playButton.setOutlineThickness(5);
    playButton.setOutlineColor(sf::Color::Transparent);

    // And text to show status
    sf::Text statusText(font);
    statusText.setPosition({10, recordButton.getGlobalBounds().position.y + recordButton.getGlobalBounds().size.y + 10});

    // Here we'll use an integrated custom recorder, which saves the captured data into a SoundBuffer
    sf::SoundBufferRecorder recorder;
    sf::Sound               sound(recorder.getBuffer());

    auto setCurrentDevice = [&](auto index)
    {
        assert(index < devices.size());
        if (!recorder.setDevice(devices[index]))
        {
            sf::err() << "Unable to set device to " << devices[index];
        }
        const auto bounds = deviceTexts[index].getGlobalBounds();
        deviceIndicator.setSize(bounds.size);
        deviceIndicator.setPosition(bounds.position);
    };
    setCurrentDevice(0u);

    bool recording      = false;
    auto startRecording = [&]
    {
        assert(!recording);
        if (!recorder.start())
        {
            sf::err() << "Failed to start recording" << std::endl;
        }
        else
        {
            recording = true;
            playButton.setFillColor(sf::Color::Transparent);
            recordButton.setOutlineColor(sf::Color::White);
        }
        statusText.setString("Recording...");
    };

    auto stopRecording = [&]
    {
        assert(recording);
        recorder.stop();
        playButton.setFillColor(sf::Color::Green);
        recordButton.setOutlineColor(sf::Color::Transparent);
        recording          = false;
        const auto& buffer = recorder.getBuffer();
        sound.setBuffer(buffer);
        statusText.setString("Recorded: " + std::to_string(buffer.getDuration().asSeconds()) + " seconds");
    };

    bool playing       = false;
    auto startPlayback = [&]
    {
        sound.play();
        recordButton.setFillColor(sf::Color::Transparent);
        playButton.setOutlineColor(sf::Color::White);
        playing = true;
    };

    auto stopPlayback = [&]
    {
        sound.stop();
        recordButton.setFillColor(sf::Color::Red);
        playButton.setOutlineColor(sf::Color::Transparent);
        playing = false;
    };

    while (window.isOpen())
    {
        while (const auto event = window.pollEvent())
        {
            if (const auto* clickEvent = event->getIf<sf::Event::MouseButtonPressed>();
                clickEvent && clickEvent->button == sf::Mouse::Button::Left)
            {
                // Check if a device is selected
                for (auto i = 0u; i < devices.size(); ++i)
                {
                    if (deviceTexts[i].getGlobalBounds().contains(sf::Vector2f{clickEvent->position}))
                    {
                        setCurrentDevice(i);
                    }
                }

                // Check for recording button press
                if ((sf::Vector2f{clickEvent->position} - recordButton.getGlobalBounds().getCenter()).length() <
                    recordButton.getRadius())
                {
                    if (!recording)
                    {
                        startRecording();
                    }
                    else
                    {
                        stopRecording();
                    }
                }

                // Check for play button press
                if (playButton.getGlobalBounds().contains(sf::Vector2f{clickEvent->position}))
                {
                    if (!playing)
                    {
                        startPlayback();
                    }
                    else
                    {
                        stopPlayback();
                    }
                }
            }
        }

        // Check if sound finished playing
        if (playing && sound.getStatus() == sf::Sound::Status::Stopped)
        {
            stopPlayback();
        }

        window.clear();

        window.draw(deviceIndicator);
        for (const auto& text : deviceTexts)
        {
            window.draw(text);
        }
        window.draw(playButton);
        window.draw(recordButton);
        window.draw(statusText);
        window.display();
    }
}
