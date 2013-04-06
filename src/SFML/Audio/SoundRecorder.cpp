////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007-2013 Laurent Gomila (laurent.gom@gmail.com)
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
#include <SFML/Audio/AudioDevice.hpp>
#include <SFML/Audio/ALCheck.hpp>
#include <SFML/System/Sleep.hpp>
#include <SFML/System/Err.hpp>

#ifdef _MSC_VER
    #pragma warning(disable : 4355) // 'this' used in base member initializer list
#endif


namespace
{
    ALCdevice* captureDevice = NULL;
}

namespace sf
{
////////////////////////////////////////////////////////////
SoundRecorder::SoundRecorder() :
m_thread     (&SoundRecorder::record, this),
m_sampleRate (0),
m_isCapturing(false)
{
    priv::ensureALInit();
}


////////////////////////////////////////////////////////////
SoundRecorder::~SoundRecorder()
{
    // Nothing to do
}


////////////////////////////////////////////////////////////
void SoundRecorder::start(unsigned int sampleRate)
{
    // Check if the device can do audio capture
    if (!isAvailable())
    {
        err() << "Failed to start capture : your system cannot capture audio data (call SoundRecorder::IsAvailable to check it)" << std::endl;
        return;
    }

    // Check that another capture is not already running
    if (captureDevice)
    {
        err() << "Trying to start audio capture, but another capture is already running" << std::endl;
        return;
    }

    // Open the capture device for capturing 16 bits mono samples
    captureDevice = alcCaptureOpenDevice(NULL, sampleRate, AL_FORMAT_MONO16, sampleRate);
    if (!captureDevice)
    {
        err() << "Failed to open the audio capture device" << std::endl;
        return;
    }

    // Clear the array of samples
    m_samples.clear();

    // Store the sample rate
    m_sampleRate = sampleRate;

    // Notify derived class
    if (onStart())
    {
        // Start the capture
        alcCaptureStart(captureDevice);

        // Start the capture in a new thread, to avoid blocking the main thread
        m_isCapturing = true;
        m_thread.launch();
    }
}


////////////////////////////////////////////////////////////
void SoundRecorder::stop()
{
    // Stop the capturing thread
    m_isCapturing = false;
    m_thread.wait();
}


////////////////////////////////////////////////////////////
unsigned int SoundRecorder::getSampleRate() const
{
    return m_sampleRate;
}


////////////////////////////////////////////////////////////
bool SoundRecorder::isAvailable()
{
    return (priv::AudioDevice::isExtensionSupported("ALC_EXT_CAPTURE") != AL_FALSE) ||
           (priv::AudioDevice::isExtensionSupported("ALC_EXT_capture") != AL_FALSE); // "bug" in Mac OS X 10.5 and 10.6
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


////////////////////////////////////////////////////////////
void SoundRecorder::record()
{
    while (m_isCapturing)
    {
        // Process available samples
        processCapturedSamples();

        // Don't bother the CPU while waiting for more captured data
        sleep(milliseconds(100));
    }

    // Capture is finished : clean up everything
    cleanup();

    // Notify derived class
    onStop();
}


////////////////////////////////////////////////////////////
void SoundRecorder::processCapturedSamples()
{
    // Get the number of samples available
    ALCint samplesAvailable;
    alcGetIntegerv(captureDevice, ALC_CAPTURE_SAMPLES, 1, &samplesAvailable);

    if (samplesAvailable > 0)
    {
        // Get the recorded samples
        m_samples.resize(samplesAvailable);
        alcCaptureSamples(captureDevice, &m_samples[0], samplesAvailable);

        // Forward them to the derived class
        if (!onProcessSamples(&m_samples[0], m_samples.size()))
        {
            // The user wants to stop the capture
            m_isCapturing = false;
        }
    }
}


////////////////////////////////////////////////////////////
void SoundRecorder::cleanup()
{
    // Stop the capture
    alcCaptureStop(captureDevice);

    // Get the samples left in the buffer
    processCapturedSamples();

    // Close the device
    alcCaptureCloseDevice(captureDevice);
    captureDevice = NULL;
}

} // namespace sf
