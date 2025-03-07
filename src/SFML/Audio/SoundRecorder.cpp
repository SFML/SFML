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

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Audio/SoundRecorder.hpp>

#include <SFML/System/Err.hpp>
#include <SFML/System/Sleep.hpp>

#include <miniaudio.h>

#include <algorithm>
#include <array>
#include <optional>
#include <ostream>

#include <cassert>
#include <cstring>


namespace sf
{
struct SoundRecorder::Impl
{
    explicit Impl(SoundRecorder* ownerPtr) : owner(ownerPtr)
    {
    }

    bool initialize()
    {
        if (!context)
            return false;

        // Find the device by its name
        auto devices = getAvailableDevices();

        const auto iter = std::find_if(devices.begin(),
                                       devices.end(),
                                       [this](const ma_device_info& info) { return info.name == deviceName; });

        if (iter == devices.end())
            return false;

        // (Re-)create the capture device
        if (captureDevice)
        {
            ma_device_uninit(&*captureDevice);
        }
        else
        {
            captureDevice.emplace();
        }

        auto captureDeviceConfig              = ma_device_config_init(ma_device_type_capture);
        captureDeviceConfig.capture.pDeviceID = &iter->id;
        captureDeviceConfig.capture.channels  = channelCount;
        captureDeviceConfig.capture.format    = ma_format_s16;
        captureDeviceConfig.sampleRate        = sampleRate;
        captureDeviceConfig.pUserData         = this;
        captureDeviceConfig.dataCallback = [](ma_device* device, void*, const void* input, std::uint32_t frameCount)
        {
            auto& impl = *static_cast<Impl*>(device->pUserData);

            // Copy the new samples into our temporary buffer
            impl.samples.resize(frameCount * impl.channelCount);
            std::memcpy(impl.samples.data(), input, frameCount * impl.channelCount * sizeof(std::int16_t));

            // Notify the derived class of the availability of new samples
            if (!impl.owner->onProcessSamples(impl.samples.data(), impl.samples.size()))
            {
                // If the derived class wants to stop, stop the capture
                if (const auto result = ma_device_stop(device); result != MA_SUCCESS)
                {
                    err() << "Failed to stop audio capture device: " << ma_result_description(result) << std::endl;
                    return;
                }
            }
        };

        if (const auto result = ma_device_init(&*context, &captureDeviceConfig, &*captureDevice); result != MA_SUCCESS)
        {
            captureDevice.reset();
            err() << "Failed to initialize the audio capture device: " << ma_result_description(result) << std::endl;
            return false;
        }

        return true;
    }

    static std::vector<ma_device_info> getAvailableDevices()
    {
        std::vector<ma_device_info> deviceList;

        // Create the context
        ma_context context;

        const auto contextConfig = ma_context_config_init();

        if (const auto result = ma_context_init(nullptr, 0, &contextConfig, &context); result != MA_SUCCESS)
        {
            err() << "Failed to initialize the audio context: " << ma_result_description(result) << std::endl;
            return deviceList;
        }

        // Enumerate the capture devices
        ma_device_info* deviceInfos = nullptr;
        std::uint32_t   deviceCount = 0;

        if (const auto result = ma_context_get_devices(&context, nullptr, nullptr, &deviceInfos, &deviceCount);
            result != MA_SUCCESS)
        {
            err() << "Failed to get audio capture devices: " << ma_result_description(result) << std::endl;
            ma_context_uninit(&context);
            return deviceList;
        }

        for (auto i = 0u; i < deviceCount; ++i)
            deviceList.push_back(deviceInfos[i]);

        ma_context_uninit(&context);
        return deviceList;
    }

    ////////////////////////////////////////////////////////////
    // Member data
    ////////////////////////////////////////////////////////////
    SoundRecorder* const      owner;                          //!< Owning SoundRecorder object
    std::optional<ma_log>     log;                            //!< The miniaudio log
    std::optional<ma_context> context;                        //!< The miniaudio context
    std::optional<ma_device>  captureDevice;                  //!< The miniaudio capture device
    std::string               deviceName{getDefaultDevice()}; //!< Name of the audio capture device
    unsigned int              channelCount{1};                //!< Number of recording channels
    unsigned int              sampleRate{44100};              //!< Sample rate
    std::vector<std::int16_t> samples;                        //!< Buffer to store captured samples
    std::vector<SoundChannel> channelMap{SoundChannel::Mono}; //!< The map of position in sample frame to sound channel
};


////////////////////////////////////////////////////////////
SoundRecorder::SoundRecorder() : m_impl(std::make_unique<Impl>(this))
{
    // Create the log
    m_impl->log.emplace();

    if (const auto result = ma_log_init(nullptr, &*m_impl->log); result != MA_SUCCESS)
    {
        m_impl->log.reset();
        err() << "Failed to initialize the audio log: " << ma_result_description(result) << std::endl;
        return;
    }

    // Register our logging callback to output any warning/error messages
    if (const auto result = ma_log_register_callback(&*m_impl->log,
                                                     ma_log_callback_init(
                                                         [](void*, std::uint32_t level, const char* message)
                                                         {
                                                             if (level <= MA_LOG_LEVEL_WARNING)
                                                                 err() << "miniaudio " << ma_log_level_to_string(level)
                                                                       << ": " << message << std::flush;
                                                         },
                                                         nullptr));
        result != MA_SUCCESS)
        err() << "Failed to register audio log callback: " << ma_result_description(result) << std::endl;

    // Create the context
    m_impl->context.emplace();

    auto contextConfig                                 = ma_context_config_init();
    contextConfig.pLog                                 = &*m_impl->log;
    std::uint32_t                          deviceCount = 0;
    const auto                             nullBackend = ma_backend_null;
    const std::array<const ma_backend*, 2> backendLists{nullptr, &nullBackend};

    for (const auto* backendList : backendLists)
    {
        // We can set backendCount to 1 since it is ignored when backends is set to nullptr
        if (const auto result = ma_context_init(backendList, 1, &contextConfig, &*m_impl->context); result != MA_SUCCESS)
        {
            m_impl->context.reset();
            err() << "Failed to initialize the audio capture context: " << ma_result_description(result) << std::endl;
            return;
        }

        // Count the capture devices
        if (const auto result = ma_context_get_devices(&*m_impl->context, nullptr, nullptr, nullptr, &deviceCount);
            result != MA_SUCCESS)
        {
            err() << "Failed to get audio capture devices: " << ma_result_description(result) << std::endl;
            return;
        }

        // Check if there are audio capture devices available on the system
        if (deviceCount > 0)
            break;

        // Warn if no devices were found using the default backend list
        if (backendList == nullptr)
            err() << "No audio capture devices available on the system" << std::endl;

        // Clean up the context if we didn't find any devices
        ma_context_uninit(&*m_impl->context);
    }

    // If the NULL audio backend also doesn't provide a device we give up
    if (deviceCount == 0)
    {
        m_impl->context.reset();
        return;
    }

    if (m_impl->context->backend == ma_backend_null)
        err() << "Using NULL audio backend for capture" << std::endl;

    // Create the capture device
    m_impl->initialize();
}


////////////////////////////////////////////////////////////
SoundRecorder::~SoundRecorder()
{
    // This assertion is triggered if the recording is still running while
    // the object is destroyed. It ensures that stop() is called in the
    // destructor of the derived class, which makes sure that the capture
    // device is stopped before the derived object is destroyed. Otherwise a
    // "pure virtual method called" exception is triggered.
    assert(!(m_impl->captureDevice && ma_device_is_started(&*m_impl->captureDevice)) &&
           "You must call stop() in the destructor of your derived class, so that the "
           "capture device is stopped before your object is destroyed.");

    // Destroy the capture device
    if (m_impl->captureDevice)
        ma_device_uninit(&*m_impl->captureDevice);

    // Destroy the context
    if (m_impl->context)
        ma_context_uninit(&*m_impl->context);

    // Destroy the log
    if (m_impl->log)
        ma_log_uninit(&*m_impl->log);
}


////////////////////////////////////////////////////////////
bool SoundRecorder::start(unsigned int sampleRate)
{
    // Check if the device can do audio capture
    if (!isAvailable())
    {
        err() << "Failed to start capture: your system cannot capture audio data (call SoundRecorder::isAvailable to "
                 "check it)"
              << std::endl;
        return false;
    }

    // Store the sample rate and re-initialize if necessary
    if (m_impl->sampleRate != sampleRate)
    {
        m_impl->sampleRate = sampleRate;

        if (!m_impl->initialize())
        {
            err() << "Failed to set audio capture device sample rate to " << sampleRate << std::endl;
            return false;
        }
    }

    // Ensure we have a capture device
    if (!m_impl->captureDevice)
    {
        err() << "Trying to start audio capture, but no device available" << std::endl;
        return false;
    }

    // Check that another capture is not already running
    if (ma_device_is_started(&*m_impl->captureDevice))
    {
        err() << "Trying to start audio capture, but another capture is already running" << std::endl;
        return false;
    }

    // Notify derived class
    if (onStart())
    {
        // Start the capture
        if (const auto result = ma_device_start(&*m_impl->captureDevice); result != MA_SUCCESS)
        {
            err() << "Failed to start audio capture device: " << ma_result_description(result) << std::endl;
            return false;
        }

        return true;
    }

    return false;
}


////////////////////////////////////////////////////////////
void SoundRecorder::stop()
{
    // Stop the capturing device if one is started
    if (m_impl->captureDevice && ma_device_is_started(&*m_impl->captureDevice))
    {
        // Stop the capture
        if (const auto result = ma_device_stop(&*m_impl->captureDevice); result != MA_SUCCESS)
        {
            err() << "Failed to stop audio capture device: " << ma_result_description(result) << std::endl;
            return;
        }

        // Notify derived class
        onStop();
    }
}


////////////////////////////////////////////////////////////
unsigned int SoundRecorder::getSampleRate() const
{
    return m_impl->sampleRate;
}


////////////////////////////////////////////////////////////
std::vector<std::string> SoundRecorder::getAvailableDevices()
{
    // Convert the internal miniaudio device list into a name-only list
    const auto               devices = Impl::getAvailableDevices();
    std::vector<std::string> deviceNameList;
    deviceNameList.reserve(devices.size());

    for (const auto& device : devices)
        deviceNameList.emplace_back(device.name);

    return deviceNameList;
}


////////////////////////////////////////////////////////////
std::string SoundRecorder::getDefaultDevice()
{
    const auto devices = Impl::getAvailableDevices();

    for (const auto& device : devices)
    {
        if (device.isDefault)
            return device.name;
    }

    return "";
}


////////////////////////////////////////////////////////////
bool SoundRecorder::setDevice(const std::string& name)
{
    // Store the device name and re-initialize if necessary
    if (m_impl->deviceName != name)
    {
        m_impl->deviceName = name;

        if (!m_impl->initialize())
            return false;
    }

    return true;
}


////////////////////////////////////////////////////////////
const std::string& SoundRecorder::getDevice() const
{
    return m_impl->deviceName;
}


////////////////////////////////////////////////////////////
void SoundRecorder::setChannelCount(unsigned int channelCount)
{
    // We only bother supporting mono/stereo recording for now
    if (channelCount < 1 || channelCount > 2)
    {
        err() << "Unsupported channel count: " << channelCount
              << " Currently only mono (1) and stereo (2) recording is supported." << std::endl;
        return;
    }

    // Store the channel count and re-initialize if necessary
    if (m_impl->channelCount != channelCount)
    {
        m_impl->channelCount = channelCount;
        m_impl->initialize();

        // We only bother supporting mono/stereo recording for now
        if (channelCount == 1)
        {
            m_impl->channelMap = {SoundChannel::Mono};
        }
        else if (channelCount == 2)
        {
            m_impl->channelMap = {SoundChannel::FrontLeft, SoundChannel::FrontRight};
        }
    }
}


////////////////////////////////////////////////////////////
unsigned int SoundRecorder::getChannelCount() const
{
    return m_impl->channelCount;
}


////////////////////////////////////////////////////////////
const std::vector<SoundChannel>& SoundRecorder::getChannelMap() const
{
    return m_impl->channelMap;
}


////////////////////////////////////////////////////////////
bool SoundRecorder::isAvailable()
{
    // Try to open a device for capture to see if recording is available
    const auto config = ma_device_config_init(ma_device_type_capture);
    ma_device  device;

    if (ma_device_init(nullptr, &config, &device) != MA_SUCCESS)
        return false;

    ma_device_uninit(&device);

    return true;
}


////////////////////////////////////////////////////////////
bool SoundRecorder::onStart()
{
    // Nothing to do
    return true;
}


////////////////////////////////////////////////////////////
void SoundRecorder::onStop()
{
    // Nothing to do
}

} // namespace sf
