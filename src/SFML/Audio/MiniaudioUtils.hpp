////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007-2024 Laurent Gomila (laurent@sfml-dev.org)
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
#include <SFML/Audio/SoundChannel.hpp>

#include <miniaudio.h>

#include <functional>


////////////////////////////////////////////////////////////
// Forward declarations
////////////////////////////////////////////////////////////

namespace sf
{
class Time;
}


////////////////////////////////////////////////////////////
// Declaration of 'MiniaudioUtils'
////////////////////////////////////////////////////////////

namespace sf::priv
{
////////////////////////////////////////////////////////////
class MiniaudioUtils
{
private:
    struct SavedSettings;

    [[nodiscard]] static SavedSettings saveSettings(const ma_sound& sound);
    static void                        applySettings(ma_sound& sound, const SavedSettings& savedSettings);

    static void initializeSoundWithDefaultSettings(ma_sound& sound);
    static void initializeDataSource(ma_data_source_base& dataSourceBase, const ma_data_source_vtable& vtable);

public:
    [[nodiscard]] static ma_channel soundChannelToMiniaudioChannel(sf::SoundChannel soundChannel);
    [[nodiscard]] static Time       getPlayingOffset(ma_sound& sound);
    [[nodiscard]] static ma_uint64  getFrameIndex(ma_sound& sound, Time timeOffset);

    static void reinitializeSound(ma_sound& sound, const std::function<void()>& initializeFn);

    static void initializeSound(const ma_data_source_vtable& vtable,
                                ma_data_source_base&         dataSourceBase,
                                ma_sound&                    sound,
                                const std::function<void()>& initializeFn);
};

} // namespace sf::priv
