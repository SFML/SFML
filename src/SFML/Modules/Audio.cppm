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
// Module preamble
////////////////////////////////////////////////////////////

module;

#include <SFML/Audio.hpp>

export module sfml.audio;

export import sfml.system;

export namespace sf {
    using sf::InputSoundFile;
    namespace Listener {
        using sf::Listener::Cone;

        using sf::Listener::setGlobalVolume;
        using sf::Listener::getGlobalVolume;
        using sf::Listener::setPosition;
        using sf::Listener::getPosition;
        using sf::Listener::setDirection;
        using sf::Listener::getDirection;
        using sf::Listener::setVelocity;
        using sf::Listener::getVelocity;
        using sf::Listener::setCone;
        using sf::Listener::getCone;
        using sf::Listener::setUpVector;
        using sf::Listener::getUpVector;
    }
    using sf::Music;
    using sf::OutputSoundFile;
    namespace PlaybackDevice {
        using sf::PlaybackDevice::getAvailableDevices;
        using sf::PlaybackDevice::getDefaultDevice;
        using sf::PlaybackDevice::setDevice;
        using sf::PlaybackDevice::getDevice;
    }
    using sf::Sound;
    using sf::SoundBuffer;
    using sf::SoundBufferRecorder;
    using sf::SoundChannel;
    using sf::SoundFileFactory;
    using sf::SoundFileReader;
    using sf::SoundFileWriter;
    using sf::SoundRecorder;
    using sf::SoundSource;
    using sf::SoundStream;
}

////////////////////////////////////////////////////////////
/// \defgroup audio Audio module
///
/// Sounds, streaming (musics or custom sources), recording,
/// spatialization.
///
////////////////////////////////////////////////////////////
