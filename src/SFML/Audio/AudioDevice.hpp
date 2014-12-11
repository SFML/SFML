////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007-2014 Laurent Gomila (laurent.gom@gmail.com)
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

#ifndef SFML_AUDIODEVICE_HPP
#define SFML_AUDIODEVICE_HPP

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <set>
#include <string>


namespace sf
{
namespace priv
{
////////////////////////////////////////////////////////////
/// \brief High-level wrapper around the audio API, it manages
///        the creation and destruction of the audio device and
///        context and stores the device capabilities
///
////////////////////////////////////////////////////////////
class AudioDevice
{
public:

    ////////////////////////////////////////////////////////////
    /// \brief Default constructor
    ///
    ////////////////////////////////////////////////////////////
    AudioDevice();

    ////////////////////////////////////////////////////////////
    /// \brief Destructor
    ///
    ////////////////////////////////////////////////////////////
    ~AudioDevice();

    ////////////////////////////////////////////////////////////
    /// \brief Check if an OpenAL extension is supported
    ///
    /// This functions automatically finds whether it
    /// is an AL or ALC extension, and calls the corresponding
    /// function.
    ///
    /// \param extension Name of the extension to test
    ///
    /// \return True if the extension is supported, false if not
    ///
    ////////////////////////////////////////////////////////////
    static bool isExtensionSupported(const std::string& extension);

    ////////////////////////////////////////////////////////////
    /// \brief Get the OpenAL format that matches the given number of channels
    ///
    /// \param channelCount Number of channels
    ///
    /// \return Corresponding format
    ///
    ////////////////////////////////////////////////////////////
    static int getFormatFromChannelCount(unsigned int channelCount);
};

} // namespace priv

} // namespace sf


#endif // SFML_AUDIODEVICE_HPP
