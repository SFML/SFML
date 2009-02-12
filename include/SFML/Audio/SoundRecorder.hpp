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

#ifndef SFML_SOUNDRECORDER_HPP
#define SFML_SOUNDRECORDER_HPP

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/System/Thread.hpp>
#include <vector>


namespace sf
{
////////////////////////////////////////////////////////////
/// SoundRecorder is an interface for capturing sound data,
/// it is meant to be used as a base class
////////////////////////////////////////////////////////////
class SFML_API SoundRecorder : private Thread
{
public :

    ////////////////////////////////////////////////////////////
    /// Virtual destructor
    ///
    ////////////////////////////////////////////////////////////
    virtual ~SoundRecorder();

    ////////////////////////////////////////////////////////////
    /// Start the capture.
    /// Warning : only one capture can happen at the same time
    ///
    /// \param SampleRate : Sound frequency (the more samples, the higher the quality)
    ///                    (44100 by default = CD quality)
    ///
    ////////////////////////////////////////////////////////////
    void Start(unsigned int SampleRate = 44100);

    ////////////////////////////////////////////////////////////
    /// Stop the capture
    ///
    ////////////////////////////////////////////////////////////
    void Stop();

    ////////////////////////////////////////////////////////////
    /// Get the sample rate
    ///
    /// \return Frequency, in samples per second
    ///
    ////////////////////////////////////////////////////////////
    unsigned int GetSampleRate() const;

    ////////////////////////////////////////////////////////////
    /// Tell if the system supports sound capture.
    /// If not, this class won't be usable
    ///
    /// \return True if audio capture is supported
    ///
    ////////////////////////////////////////////////////////////
    static bool CanCapture();

protected :

    ////////////////////////////////////////////////////////////
    /// Default constructor
    ///
    ////////////////////////////////////////////////////////////
    SoundRecorder();

private :

    ////////////////////////////////////////////////////////////
    /// Start recording audio data
    ///
    /// \return False to abort recording audio data, true to start
    ///
    ////////////////////////////////////////////////////////////
    virtual bool OnStart();

    ////////////////////////////////////////////////////////////
    /// Process a new chunk of recorded samples
    ///
    /// \param Samples :      Pointer to the new chunk of recorded samples
    /// \param SamplesCount : Number of samples pointed by Samples
    ///
    /// \return False to stop recording audio data, true to continue
    ///
    ////////////////////////////////////////////////////////////
    virtual bool OnProcessSamples(const Int16* Samples, std::size_t SamplesCount) = 0;

    ////////////////////////////////////////////////////////////
    /// Stop recording audio data
    ///
    ////////////////////////////////////////////////////////////
    virtual void OnStop();

    ////////////////////////////////////////////////////////////
    /// /see Thread::Run
    ///
    ////////////////////////////////////////////////////////////
    virtual void Run();

    ////////////////////////////////////////////////////////////
    /// Get the available captured samples and process them
    ///
    ////////////////////////////////////////////////////////////
    void ProcessCapturedSamples();

    ////////////////////////////////////////////////////////////
    /// Clean up the recorder internal resources
    ///
    ////////////////////////////////////////////////////////////
    void CleanUp();

    ////////////////////////////////////////////////////////////
    // Member data
    ////////////////////////////////////////////////////////////
    std::vector<Int16> mySamples;     ///< Buffer to store captured samples
    unsigned int       mySampleRate;  ///< Sample rate
    bool               myIsCapturing; ///< Capturing state
};

} // namespace sf


#endif // SFML_SOUNDRECORDER_HPP
