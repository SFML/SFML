////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Audio.hpp>

#include <algorithm>
#include <condition_variable>
#include <iostream>
#include <mutex>
#include <thread>

#include <cctype>


////////////////////////////////////////////////////////////
/// Entry point of application
///
/// \return Application exit code
///
////////////////////////////////////////////////////////////
int main()
{
    // List the available playback devices
    auto devices = sf::PlaybackDevice::getAvailableDevices();

    if (devices.empty())
    {
        std::cout << "No playback devices available\n";
        return EXIT_FAILURE;
    }

    std::cout << "Available playback devices:\n\n";

    for (auto i = 0u; i < devices.size(); ++i)
        std::cout << i << ": " << devices[i] << '\n';

    std::cout << '\n';

    // Output the default device name if one is available
    const auto defaultDevice = sf::PlaybackDevice::getDefaultDevice();

    if (defaultDevice)
        std::cout << "Default device: " << *defaultDevice << '\n';

    std::cout << std::endl;

    std::size_t deviceIndex = 0;

    // Choose the playback device
    if (devices.size() > 1)
    {
        deviceIndex = devices.size();
        std::cout << "Please choose the playback device to use [0-" << devices.size() - 1 << "]: ";
        do
        {
            std::cin >> deviceIndex;
            std::cin.ignore(10'000, '\n');
        } while (deviceIndex >= devices.size());
    }

    // Check if we can and whether the user wants to enable automatic stream routing
    bool automaticStreamRouting = false;

    if (devices[deviceIndex] == defaultDevice)
    {
        char answer{};
        std::cout << "Activate automatic stream routing? (Y/N): ";
        do
        {
            std::cin >> answer;
            std::cin.ignore(10'000, '\n');
        } while (answer != 'Y' && answer != 'y' && answer != 'N' && answer != 'n');
        automaticStreamRouting = (answer == 'Y') || (answer == 'y');
    }

    // Check if we should manually reroute to the default device or the null device
    bool rerouteToNull = false;

    if (!automaticStreamRouting)
    {
        char answer{};
        std::cout << "On device loss, reroute to default or null device? (D/N): ";
        do
        {
            std::cin >> answer;
            std::cin.ignore(10'000, '\n');
        } while (answer != 'D' && answer != 'd' && answer != 'N' && answer != 'n');
        rerouteToNull = (answer == 'N') || (answer == 'n');
    }

    // Save the selected device name for later
    const auto& selectedDevice = devices[deviceIndex];

    // Set the playback device to the user selection
    if (!(automaticStreamRouting ? sf::PlaybackDevice::setDeviceToDefault() : sf::PlaybackDevice::setDevice(selectedDevice)))
    {
        std::cout << "Failed to set the playback device\n";
        return EXIT_FAILURE;
    }

    // We use this struct to pass information between the notification callback and our main thread
    struct
    {
        std::mutex              mutex;
        std::condition_variable cv;
        bool                    rerouteRequired{};
    } rerouteData;

    // We set our notification callback here
    // Note: This callback is called from the audio backend
    // Other than saving some rudimentary information out of the callback
    // not much else can be done in it that would be safe under all circumstances
    sf::PlaybackDevice::setNotificationCallback(
        [automaticStreamRouting, &rerouteData](sf::PlaybackDevice::Notification notification)
        {
            switch (notification)
            {
                case sf::PlaybackDevice::Notification::DeviceStarted:
                    std::cout << "Device started" << std::endl;
                    break;
                case sf::PlaybackDevice::Notification::DeviceStopped:
                {
                    std::cout << "Device stopped" << std::endl;

                    // If automatic stream routing is enabled the
                    // operating system will do all the work
                    if (automaticStreamRouting)
                        break;

                    // If not we need to do the work ourselves
                    // Notify main thread we have to manually reroute
                    const std::lock_guard lock(rerouteData.mutex);
                    rerouteData.rerouteRequired = true;
                    rerouteData.cv.notify_all();

                    break;
                }
                case sf::PlaybackDevice::Notification::DeviceRerouted:
                    std::cout << "Device rerouted" << std::endl;
                    break;
                case sf::PlaybackDevice::Notification::DeviceInterruptionBegan:
                    std::cout << "Device interruption began" << std::endl;
                    break;
                case sf::PlaybackDevice::Notification::DeviceInterruptionEnded:
                    std::cout << "Device interruption ended" << std::endl;
                    break;
                case sf::PlaybackDevice::Notification::DeviceUnlocked:
                    std::cout << "Device unlocked" << std::endl;
                    break;
            }
        });

    // Play music from an ogg file
    sf::Music music("resources/doodle_pop.ogg");
    music.setLooping(true);
    music.play();

    const auto outputDeviceInfo = []
    {
        if (const auto device = sf::PlaybackDevice::getDevice())
            std::cout << "Playback device set to: " << *device << std::endl;

        if (sf::PlaybackDevice::isDefaultDevice())
            std::cout << "Playback device is default device" << std::endl;
    };

    outputDeviceInfo();

    // Main thread loop
    while (true)
    {
        // Sleep until manual reroute is required
        {
            std::unique_lock lock(rerouteData.mutex);

            rerouteData.cv.wait_for(lock, std::chrono::hours(1000), [&]() { return rerouteData.rerouteRequired; });

            if (!rerouteData.rerouteRequired)
                continue;
        }

        // Manual reroute required

        // First check if the selected device is still available
        const auto selectedDeviceAvailable = [&]
        {
            const auto availableDevices = sf::PlaybackDevice::getAvailableDevices();
            return std::find(availableDevices.begin(), availableDevices.end(), selectedDevice) != availableDevices.end();
        };

        if (selectedDeviceAvailable())
        {
            // Device is still available, start it again
            if (!sf::PlaybackDevice::setDevice(selectedDevice))
            {
                std::cout << "Failed to set the playback device\n";
                return EXIT_FAILURE;
            }

            const std::lock_guard lock(rerouteData.mutex);
            rerouteData.rerouteRequired = false;

            continue;
        }

        // Selected device is no longer available

        // Temporarily route to the default/null device to keep sounds playing
        std::cout << "Temporarily rerouting to " << (rerouteToNull ? "null device" : "default device") << std::endl;

        if (!(rerouteToNull ? sf::PlaybackDevice::setDeviceToNull() : sf::PlaybackDevice::setDeviceToDefault()))
        {
            std::cout << "Failed to set the playback device\n";
            return EXIT_FAILURE;
        }

        outputDeviceInfo();

        // We currently don't have a way to be notified when new devices are made available
        // Thus we have to periodically check for available devices and switch back
        // to the device the user selected when it shows up in the list again
        while (true)
        {
            if (selectedDeviceAvailable())
            {
                // Device is available again, select it
                std::cout << "Device " << selectedDevice << " available again\n";
                std::cout << "Rerouting to " << selectedDevice << std::endl;

                if (!sf::PlaybackDevice::setDevice(selectedDevice))
                {
                    std::cout << "Failed to set the playback device\n";
                    return EXIT_FAILURE;
                }

                outputDeviceInfo();

                break;
            }

            // Try again in 100 milliseconds
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }

        const std::lock_guard lock(rerouteData.mutex);
        rerouteData.rerouteRequired = false;
    }
}
