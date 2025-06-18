////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007-2025 Laurent Gomila (laurent@sfml-dev.org)
//
// This software is provided 'as-is', without any express or implied warranty.
// In no event will the authors be held liable for any damages arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it freely,
// subject to the following restrictions:
//
// 1. The origin of this software must not be misrepresented;
//    you must not claim that you wrote the original software.
//    If you use this software in a product, an acknowledgment
//    in the product documentation would be appreciated but is not required.
//
// 2. Altered source versions must be plainly marked as such,
//    and must not be misrepresented as being the original software.
//
// 3. This notice may not be removed or altered from any source distribution.
//
////////////////////////////////////////////////////////////

#pragma once

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Audio/Export.hpp>

#include <functional>
#include <optional>
#include <string>
#include <vector>

#include <cstdint>


namespace sf::PlaybackDevice
{
////////////////////////////////////////////////////////////
/// \brief Get a list of the names of all available audio playback devices
///
/// This function returns a vector of strings containing
/// the names of all available audio playback devices.
///
/// If the operating system reports multiple devices with
/// the same name, a number will be appended to the name
/// of all subsequent devices to distinguish them from each
/// other. This guarantees that every entry returned by this
/// function will represent a unique device.
///
/// For example, if the operating system reports multiple
/// devices with the name "Sound Card", the entries returned
/// would be:
///   - Sound Card
///   - Sound Card 2
///   - Sound Card 3
///   - ...
///
/// The default device, if one is marked as such, will be
/// placed at the beginning of the vector.
///
/// If no devices are available, this function will return
/// an empty vector.
///
/// \return A vector of strings containing the device names or an empty vector if no devices are available
///
////////////////////////////////////////////////////////////
[[nodiscard]] SFML_AUDIO_API std::vector<std::string> getAvailableDevices();

////////////////////////////////////////////////////////////
/// \brief Get the name of the default audio playback device
///
/// This function returns the name of the default audio
/// playback device. If none is available, `std::nullopt`
/// is returned.
///
/// Note that depending on when this function is called, the
/// default device reported by the operating system might
/// change e.g. when a USB audio device is plugged into or
/// unplugged from the system.
///
/// \return The name of the default audio playback device
///
////////////////////////////////////////////////////////////
[[nodiscard]] SFML_AUDIO_API std::optional<std::string> getDefaultDevice();

////////////////////////////////////////////////////////////
/// \brief Set the audio playback device
///
/// This function sets the audio playback device to the device
/// with the given `name`. It can be called on the fly (i.e:
/// while sounds are playing).
///
/// If there are sounds playing when the audio playback
/// device is switched, the sounds will continue playing
/// uninterrupted on the new audio playback device.
///
/// \param name The name of the audio playback device
///
/// \return `true`, if it was able to set the requested device
///
/// \see `getAvailableDevices`, `getDefaultDevice`, `setDeviceToDefault`, `setDeviceToNull`
///
////////////////////////////////////////////////////////////
[[nodiscard]] SFML_AUDIO_API bool setDevice(const std::string& name);

////////////////////////////////////////////////////////////
/// \brief Set the audio playback device to the default
///
/// This function sets the audio playback device to the
/// default device. It can be called on the fly (i.e:
/// while sounds are playing).
///
/// If there are sounds playing when the audio playback
/// device is switched, the sounds will continue playing
/// uninterrupted on the new audio playback device.
///
/// When certain backends are used, using the default device
/// will enable automatic stream routing. When automatic
/// stream routing is enabled, audio data is automatically
/// sent to whichever physical audio device is currently
/// marked as the default on the system. If the default device
/// changes due to e.g. a device being added to or removed
/// from the system or the user marking another device as the
/// default, automatic stream routing will seamlessly reroute
/// the audio data to the new default device without any
/// manual intervention.
///
/// Automatic stream routing is currently supported when using
/// the WASAPI or DirectSound backend on Windows or the
/// Core Audio backend on macOS and iOS.
///
/// Depending on the order in which hardware devices are
/// initialized e.g. after resuming from sleep, the default
/// device might change one or more times in rapid succession
/// before it reverts back to the state in which it was
/// before the system went to sleep.
///
/// \return `true`, if it was able to set the audio playback device to the default device
///
/// \see `getAvailableDevices`, `getDefaultDevice`, `setDevice`, `setDeviceToNull`
///
////////////////////////////////////////////////////////////
[[nodiscard]] SFML_AUDIO_API bool setDeviceToDefault();

////////////////////////////////////////////////////////////
/// \brief Set the audio playback device to the null device
///
/// This function sets the audio playback device to the
/// null device. It can be called on the fly (i.e:
/// while sounds are playing).
///
/// If there are sounds playing when the audio playback
/// device is switched, the sounds will continue playing
/// uninterrupted on the new audio playback device.
///
/// Audio data routed to the null device will be discarded
/// by the backend. This can be used to keep sounds playing
/// without having them actually output on a physical
/// audio playback device.
///
/// \return `true`, if it was able to set the audio playback device to the null device
///
/// \see `getAvailableDevices`, `getDefaultDevice`, `setDevice`, `setDeviceToDefault`
///
////////////////////////////////////////////////////////////
[[nodiscard]] SFML_AUDIO_API bool setDeviceToNull();

////////////////////////////////////////////////////////////
/// \brief Get the name of the current audio playback device
///
/// \return The name of the current audio playback device or `std::nullopt` if there is none
///
////////////////////////////////////////////////////////////
[[nodiscard]] SFML_AUDIO_API std::optional<std::string> getDevice();

////////////////////////////////////////////////////////////
/// \brief Get the sample rate of the current audio playback device
///
/// \return The sample rate of the current audio playback device or `std::nullopt` if there is none
///
////////////////////////////////////////////////////////////
[[nodiscard]] SFML_AUDIO_API std::optional<std::uint32_t> getDeviceSampleRate();

////////////////////////////////////////////////////////////
/// \brief Check if the current playback device is the default device
///
/// This function will return `false` if there is no
/// current playback device.
///
/// \return `true`, if the current playback device is the default device
///
////////////////////////////////////////////////////////////
[[nodiscard]] SFML_AUDIO_API bool isDefaultDevice();

////////////////////////////////////////////////////////////
/// \brief Enumeration of the playback device notifications
///
////////////////////////////////////////////////////////////
enum class Notification
{
    DeviceStarted,  //!< Playback device has been started
    DeviceStopped,  //!< Playback device has been stopped
    DeviceRerouted, //!< Playback device has been rerouted (Generated on platforms that support automatic stream routing)
    DeviceInterruptionBegan, //!< Playback device interruption has begun (Generated on Apple mobile platforms)
    DeviceInterruptionEnded, //!< Playback device interruption has ended (Generated on Apple mobile platforms)
    DeviceUnlocked           //!< Playback device has been unlocked (Generated by Emscripten/WebAudio)
};

////////////////////////////////////////////////////////////
/// \brief Callable that is called to notify of changes to the playback device state
///
////////////////////////////////////////////////////////////
using NotificationCallback = std::function<void(Notification notification)>;

////////////////////////////////////////////////////////////
/// \brief Set a callback that should be called to notify of changes to the playback device state
///
/// Warning: Do not attempt to alter the device state from
/// within this callback. This includes changing the device
/// and even creating/destroying sound objects since that
/// could indirectly cause the playback device to be
/// created/destroyed. Also do not attempt to set the
/// notification callback from within this callback. Doing
/// so will result in a deadlock.
///
/// When receiving a notification via this callback, store
/// the information somewhere and react on it from another
/// thread e.g. the main thread within the application.
///
/// \param callback The callback that should be called to notify of changes to the playback device state
///
////////////////////////////////////////////////////////////
SFML_AUDIO_API void setNotificationCallback(NotificationCallback callback);

} // namespace sf::PlaybackDevice
