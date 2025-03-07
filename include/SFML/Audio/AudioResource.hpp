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

#include <memory>


namespace sf
{
////////////////////////////////////////////////////////////
/// \brief Base class for classes that require an audio device
///
////////////////////////////////////////////////////////////
class SFML_AUDIO_API AudioResource
{
public:
    ////////////////////////////////////////////////////////////
    /// \brief Copy constructor
    ///
    ////////////////////////////////////////////////////////////
    AudioResource(const AudioResource&) = default;

    ////////////////////////////////////////////////////////////
    /// \brief  Copy assignment
    ///
    ////////////////////////////////////////////////////////////
    AudioResource& operator=(const AudioResource&) = default;

    ////////////////////////////////////////////////////////////
    /// \brief Move constructor
    ///
    ////////////////////////////////////////////////////////////
    AudioResource(AudioResource&&) noexcept = default;

    ////////////////////////////////////////////////////////////
    /// \brief Move assignment
    ///
    ////////////////////////////////////////////////////////////
    AudioResource& operator=(AudioResource&&) noexcept = default;

protected:
    ////////////////////////////////////////////////////////////
    /// \brief Default constructor
    ///
    ////////////////////////////////////////////////////////////
    AudioResource();

private:
    ////////////////////////////////////////////////////////////
    // Member data
    ////////////////////////////////////////////////////////////
    std::shared_ptr<void> m_device; //!< Sound device
};

} // namespace sf


////////////////////////////////////////////////////////////
/// \class sf::AudioResource
/// \ingroup audio
///
/// This class is for internal use only, it must be the base
/// of every class that requires a valid audio device in
/// order to work.
///
////////////////////////////////////////////////////////////
