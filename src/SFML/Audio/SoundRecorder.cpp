////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007-2023 Laurent Gomila (laurent@sfml-dev.org)
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
#include <optional>
#include <ostream>

#include <cassert>
#include <cstring>



namespace sf
{
struct SoundRecorder::Impl
{
    Impl(SoundRecorder* owner) : m_owner(owner)
    {
    }

    bool initialize()
    {
        if (!m_context)
            return false;

        // Find the device by its name
        auto devices = getAvailableDevices();

        auto iter = std::find_if(devices.begin(),
                                 devices.end(),
                                 [this](const ma_device_info& info) { return info.name == m_deviceName; });

        if (iter == devices.end())
            return false;

        // (Re-)create the capture device
        if (m_captureDevice)
        {
            ma_device_uninit(&*m_captureDevice);
        }
        else
        {
            m_captureDevice.emplace();
        }

        auto captureDeviceConfig              = ma_device_config_init(ma_device_type_capture);
        captureDeviceConfig.capture.pDeviceID = &iter->id;
        captureDeviceConfig.capture.channels  = m_channelCount;
        captureDeviceConfig.capture.format    = ma_format_s16;
        captureDeviceConfig.sampleRate        = m_sampleRate;
        captureDeviceConfig.pUserData         = this;
        captureDeviceConfig.dataCallback      = [](ma_device* device, void*, const void* input, ma_uint32 frameCount)
        {
            auto& impl = *static_cast<Impl*>(device->pUserData);

            // Copy the new samples into our temporary buffer
            impl.m_samples.resize(frameCount * impl.m_channelCount);
            std::memcpy(impl.m_samples.data(), input, frameCount * impl.m_channelCount * sizeof(std::int16_t));

            // Notify the derived class of the availability of new samples
            if (!impl.m_owner->onProcessSamples(impl.m_samples.data(), impl.m_samples.size()))
            {
                // If the derived class wants to stop, stop the capture
                if (auto result = ma_device_stop(device); result != MA_SUCCESS)
                {
                    err() << "Failed to stop audio capture device: " << ma_result_description(result) << std::endl;
                    return;
                }
            }
        };

        if (auto result = ma_device_init(&*m_context, &captureDeviceConfig, &*m_captureDevice); result != MA_SUCCESS)
        {
            m_captureDevice.reset();
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

        auto contextConfig = ma_context_config_init();

        if (auto result = ma_context_init(nullptr, 0, &contextConfig, &context); result != MA_SUCCESS)
        {
            err() << "Failed to initialize the audio context: " << ma_result_description(result) << std::endl;
            return deviceList;
        }

        // Enumerate the capture devices
        ma_device_info* deviceInfos;
        ma_uint32       deviceCount;

        if (auto result = ma_context_get_devices(&context, nullptr, nullptr, &deviceInfos, &deviceCount);
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
    SoundRecorder* const      m_owner;                          //!< Owning SoundRecorder object
    std::optional<ma_log>     m_log;                            //!< The miniaudio log
    std::optional<ma_context> m_context;                        //!< The miniaudio context
    std::optional<ma_device>  m_captureDevice;                  //!< The miniaudio capture device
    std::string               m_deviceName{getDefaultDevice()}; //!< Name of the audio capture device
    unsigned int              m_channelCount{1};                //!< Number of recording channels
    unsigned int              m_sampleRate{44100};              //!< Sample rate
    std::vector<std::int16_t> m_samples;                        //!< Buffer to store captured samples
};


////////////////////////////////////////////////////////////
SoundRecorder::SoundRecorder() : m_impl(std::make_unique<Impl>(this))
{
    // Create the log
    m_impl->m_log.emplace();

    if (auto result = ma_log_init(nullptr, &*m_impl->m_log); result != MA_SUCCESS)
    {
        m_impl->m_log.reset();
        err() << "Failed to initialize the audio log: " << ma_result_description(result) << std::endl;
        return;
    }

    // Register our logging callback to output any warning/error messages
    if (auto result = ma_log_register_callback(&*m_impl->m_log,
                                               ma_log_callback_init(
                                                   [](void*, ma_uint32 level, const char* message)
                                                   {
                                                       if (level <= MA_LOG_LEVEL_WARNING)
                                                           err() << "miniaudio " << ma_log_level_to_string(level)
                                                                 << ": " << message << std::flush;
                                                   },
                                                   nullptr));
        result != MA_SUCCESS)
        err() << "Failed to register audio log callback: " << ma_result_description(result) << std::endl;

    // Create the context
    m_impl->m_context.emplace();

    auto contextConfig = ma_context_config_init();
    contextConfig.pLog = &*m_impl->m_log;

    if (auto result = ma_context_init(nullptr, 0, &contextConfig, &*m_impl->m_context); result != MA_SUCCESS)
    {
        m_impl->m_context.reset();
        err() << "Failed to initialize the audio context: " << ma_result_description(result) << std::endl;
        return;
    }

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
    assert(!(m_impl->m_captureDevice && ma_device_is_started(&*m_impl->m_captureDevice)) &&
           "You must call stop() in the destructor of your derived class, so that the "
           "capture device is stopped before your object is destroyed.");

    // Destroy the capture device
    if (m_impl->m_captureDevice)
        ma_device_uninit(&*m_impl->m_captureDevice);

    // Destroy the context
    if (m_impl->m_context)
        ma_context_uninit(&*m_impl->m_context);

    // Destroy the log
    if (m_impl->m_log)
        ma_log_uninit(&*m_impl->m_log);
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
    if (m_impl->m_sampleRate != sampleRate)
    {
        m_impl->m_sampleRate = sampleRate;

        if (!m_impl->initialize())
        {
            err() << "Failed to set audio capture device sample rate to " << sampleRate << std::endl;
            return false;
        }
    }

    // Ensure we have a capture device
    if (!m_impl->m_captureDevice)
    {
        err() << "Trying to start audio capture, but no device available" << std::endl;
        return false;
    }

    // Check that another capture is not already running
    if (ma_device_is_started(&*m_impl->m_captureDevice))
    {
        err() << "Trying to start audio capture, but another capture is already running" << std::endl;
        return false;
    }

    // Notify derived class
    if (onStart())
    {
        // Start the capture
        if (auto result = ma_device_start(&*m_impl->m_captureDevice); result != MA_SUCCESS)
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
    if (m_impl->m_captureDevice && ma_device_is_started(&*m_impl->m_captureDevice))
    {
        // Stop the capture
        if (auto result = ma_device_stop(&*m_impl->m_captureDevice); result != MA_SUCCESS)
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
    return m_impl->m_sampleRate;
}


////////////////////////////////////////////////////////////
std::vector<std::string> SoundRecorder::getAvailableDevices()
{
    // Convert the internal miniaudio device list into a name-only list
    std::vector<std::string> deviceNameList;
    const auto               devices = Impl::getAvailableDevices();

    for (const auto& device : devices)
        deviceNameList.push_back(device.name);

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
    if (m_impl->m_deviceName != name)
    {
        m_impl->m_deviceName = name;

        if (!m_impl->initialize())
            return false;
    }

    return true;
}


////////////////////////////////////////////////////////////
const std::string& SoundRecorder::getDevice() const
{
    return m_impl->m_deviceName;
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
    if (m_impl->m_channelCount != channelCount)
    {
        m_impl->m_channelCount = channelCount;
        m_impl->initialize();
    }
}


////////////////////////////////////////////////////////////
unsigned int SoundRecorder::getChannelCount() const
{
    return m_impl->m_channelCount;
}


////////////////////////////////////////////////////////////
std::vector<SoundChannel> SoundRecorder::getChannelMap() const
{
    // We only bother supporting mono/stereo recording for now
    if (m_impl->m_channelCount == 1)
    {
        return {SoundChannel::Mono};
    }
    else if (m_impl->m_channelCount == 2)
    {
        return {SoundChannel::FrontLeft, SoundChannel::FrontRight};
    }
    else
    {
        return {};
    }
}


////////////////////////////////////////////////////////////
bool SoundRecorder::isAvailable()
{
    // Try to open a device for capture to see if recording is available
    auto      config = ma_device_config_init(ma_device_type_capture);
    ma_device device;

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
