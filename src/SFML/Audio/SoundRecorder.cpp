////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007-2009 Laurent Gomila (laurent.gom@gmail.com)
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


////////////////////////////////////////////////////////////
// Private data
////////////////////////////////////////////////////////////
namespace
{
    ALCdevice* captureDevice = NULL;
}

namespace sf
{
////////////////////////////////////////////////////////////
SoundRecorder::SoundRecorder() :
myThread     (&SoundRecorder::Record, this),
mySampleRate (0),
myIsCapturing(false)
{
    priv::EnsureALInit();
}


////////////////////////////////////////////////////////////
SoundRecorder::~SoundRecorder()
{
    // Nothing to do
}


////////////////////////////////////////////////////////////
void SoundRecorder::Start(unsigned int sampleRate)
{
    // Check if the device can do audio capture
    if (!IsAvailable())
    {
        Err() << "Failed to start capture : your system cannot capture audio data (call SoundRecorder::IsAvailable to check it)" << std::endl;
        return;
    }

    // Check that another capture is not already running
    if (captureDevice)
    {
        Err() << "Trying to start audio capture, but another capture is already running" << std::endl;
        return;
    }

    // Open the capture device for capturing 16 bits mono samples
    captureDevice = alcCaptureOpenDevice(NULL, sampleRate, AL_FORMAT_MONO16, sampleRate);
    if (!captureDevice)
    {
        Err() << "Failed to open the audio capture device" << std::endl;
        return;
    }

    // Clear the array of samples
    mySamples.clear();

    // Store the sample rate
    mySampleRate = sampleRate;

    // Notify derived class
    if (OnStart())
    {
        // Start the capture
        alcCaptureStart(captureDevice);

        // Start the capture in a new thread, to avoid blocking the main thread
        myIsCapturing = true;
        myThread.Launch();
    }
}


////////////////////////////////////////////////////////////
void SoundRecorder::Stop()
{
    // Stop the capturing thread
    myIsCapturing = false;
    myThread.Wait();
}


////////////////////////////////////////////////////////////
unsigned int SoundRecorder::GetSampleRate() const
{
    return mySampleRate;
}


////////////////////////////////////////////////////////////
bool SoundRecorder::IsAvailable()
{
    return (priv::AudioDevice::IsExtensionSupported("ALC_EXT_CAPTURE") != AL_FALSE) ||
           (priv::AudioDevice::IsExtensionSupported("ALC_EXT_capture") != AL_FALSE); // "bug" in Mac OS X 10.5 and 10.6
}


////////////////////////////////////////////////////////////
bool SoundRecorder::OnStart()
{
    // Nothing to do
    return true;
}


////////////////////////////////////////////////////////////
void SoundRecorder::OnStop()
{
    // Nothing to do
}


////////////////////////////////////////////////////////////
void SoundRecorder::Record()
{
    while (myIsCapturing)
    {
        // Process available samples
        ProcessCapturedSamples();

        // Don't bother the CPU while waiting for more captured data
        Sleep(0.1f);
    }

    // Capture is finished : clean up everything
    CleanUp();

    // Notify derived class
    OnStop();
}


////////////////////////////////////////////////////////////
void SoundRecorder::ProcessCapturedSamples()
{
    // Get the number of samples available
    ALCint samplesAvailable;
    alcGetIntegerv(captureDevice, ALC_CAPTURE_SAMPLES, 1, &samplesAvailable);

    if (samplesAvailable > 0)
    {
        // Get the recorded samples
        mySamples.resize(samplesAvailable);
        alcCaptureSamples(captureDevice, &mySamples[0], samplesAvailable);

        // Forward them to the derived class
        if (!OnProcessSamples(&mySamples[0], mySamples.size()))
        {
            // The user wants to stop the capture
            myIsCapturing = false;
        }
    }
}


////////////////////////////////////////////////////////////
void SoundRecorder::CleanUp()
{
    // Stop the capture
    alcCaptureStop(captureDevice);

    // Get the samples left in the buffer
    ProcessCapturedSamples();

    // Close the device
    alcCaptureCloseDevice(captureDevice);
    captureDevice = NULL;
}

} // namespace sf
