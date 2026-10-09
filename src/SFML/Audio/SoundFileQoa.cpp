////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007-2026 Laurent Gomila (laurent@sfml-dev.org)
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
#include <SFML/Audio/SoundChannel.hpp>
#include <SFML/Audio/SoundFileQoa.hpp>

#include <array>
#include <limits>
#include <type_traits>
#include <vector>

#include <cassert>
#include <cmath>
#include <cstddef>
#include <cstdint>


namespace
{

constexpr std::uint8_t  maxChannels   = 8;
constexpr std::uint32_t maxSampleRate = 0xFFFFFF;
constexpr std::uint32_t minSampleRate = 1;

constexpr std::array<std::array<std::int32_t, 8>, 16> dequantTab = {
    std::array{1, -1, 3, -3, 5, -5, 7, -7},
    {5, -5, 18, -18, 32, -32, 49, -49},
    {16, -16, 53, -53, 95, -95, 147, -147},
    {34, -34, 113, -113, 203, -203, 315, -315},
    {63, -63, 210, -210, 378, -378, 588, -588},
    {104, -104, 345, -345, 621, -621, 966, -966},
    {158, -158, 528, -528, 950, -950, 1477, -1477},
    {228, -228, 760, -760, 1368, -1368, 2128, -2128},
    {316, -316, 1053, -1053, 1895, -1895, 2947, -2947},
    {422, -422, 1405, -1405, 2529, -2529, 3934, -3934},
    {548, -548, 1828, -1828, 3290, -3290, 5117, -5117},
    {696, -696, 2320, -2320, 4176, -4176, 6496, -6496},
    {868, -868, 2893, -2893, 5207, -5207, 8099, -8099},
    {1064, -1064, 3548, -3548, 6386, -6386, 9933, -9933},
    {1286, -1286, 4288, -4288, 7718, -7718, 12005, -12005},
    {1536, -1536, 5120, -5120, 9216, -9216, 14336, -14336},
};

const std::array<std::vector<sf::SoundChannel>, maxChannels + 1> channelMaps = {
    std::vector<sf::SoundChannel>{},
    {
        sf::SoundChannel::Mono,
    },
    {
        sf::SoundChannel::FrontLeft,
        sf::SoundChannel::FrontRight,
    },
    {
        sf::SoundChannel::FrontLeft,
        sf::SoundChannel::FrontRight,
        sf::SoundChannel::FrontCenter,
    },
    {
        sf::SoundChannel::FrontLeft,
        sf::SoundChannel::FrontRight,
        sf::SoundChannel::BackLeft,
        sf::SoundChannel::BackRight,
    },
    {
        sf::SoundChannel::FrontLeft,
        sf::SoundChannel::FrontRight,
        sf::SoundChannel::FrontCenter,
        sf::SoundChannel::BackLeft,
        sf::SoundChannel::BackRight,
    },
    {
        sf::SoundChannel::FrontLeft,
        sf::SoundChannel::FrontRight,
        sf::SoundChannel::FrontCenter,
        sf::SoundChannel::LowFrequencyEffects,
        sf::SoundChannel::BackLeft,
        sf::SoundChannel::BackRight,
    },
    {
        sf::SoundChannel::FrontLeft,
        sf::SoundChannel::FrontRight,
        sf::SoundChannel::FrontCenter,
        sf::SoundChannel::LowFrequencyEffects,
        sf::SoundChannel::BackCenter,
        sf::SoundChannel::SideLeft,
        sf::SoundChannel::SideRight,
    },
    {
        sf::SoundChannel::FrontLeft,
        sf::SoundChannel::FrontRight,
        sf::SoundChannel::FrontCenter,
        sf::SoundChannel::LowFrequencyEffects,
        sf::SoundChannel::BackLeft,
        sf::SoundChannel::BackRight,
        sf::SoundChannel::SideLeft,
        sf::SoundChannel::SideRight,
    },
};

template <typename Out, typename Num>
constexpr Out clampInt(Num value)
{
    static_assert(std::is_signed_v<Out> == std::is_signed_v<Num>);
    constexpr auto min = std::numeric_limits<Out>::min();
    constexpr auto max = std::numeric_limits<Out>::max();
    return value < min ? min : value > max ? max : static_cast<Out>(value);
}

} // namespace

namespace sf::priv::qoaFile
{
bool isSampleRateValid(std::uint32_t sampleRate)
{
    return sampleRate >= minSampleRate && sampleRate <= maxSampleRate;
}

bool isChannelCountValid(std::uint8_t channelCount)
{
    return channelCount <= maxChannels;
}

std::int32_t dequantTable(std::uint8_t quantizedScaleFactor, std::uint8_t quantizedResidual)
{
    assert(quantizedScaleFactor < dequantTab.size() && quantizedResidual < dequantTab[0].size());
    return dequantTab[quantizedScaleFactor][quantizedResidual];
}

std::vector<SoundChannel> getChannelMap(std::uint8_t channelCount)
{
    assert(channelCount <= maxChannels);
    return channelMaps[channelCount];
}

std::uint16_t getFrameSizeByte(std::uint8_t channelCount, std::uint16_t slicesPerChannel)
{
    const auto result = static_cast<std::uint64_t>(frameHeaderSizeByte::value) +
                        channelCount * (static_cast<std::uint64_t>(slicesPerChannel) * sliceSizeByte::value +
                                        lmsStatePerChannelSizeByte::value);
    assert(result <= std::numeric_limits<std::uint16_t>::max() && "Out of bounds calculation");
    return static_cast<std::uint16_t>(result);
}

std::int16_t calculateSample(std::int32_t dequantizedResidual, std::int32_t predictedSample)
{
    return clampInt<std::int16_t>(dequantizedResidual + predictedSample);
}

QoaSlice::QoaSlice() : QoaSlice(0){};

QoaSlice::QoaSlice(std::uint64_t bits) : m_bits{bits}
{
}

QoaSlice& QoaSlice::operator=(std::uint64_t bits)
{
    m_bits = bits;
    return *this;
}

std::uint64_t QoaSlice::raw() const
{
    return m_bits;
}

std::uint8_t QoaSlice::sfQuant() const
{
    return static_cast<std::uint8_t>(m_bits >> 60);
}

std::uint8_t QoaSlice::qr0x(std::uint8_t x) const
{
    assert(x < 20);
    return static_cast<std::uint8_t>((m_bits >> (3 * (19 - x))) & 7);
}

std::int32_t LmsState::predictSample(std::uint8_t channel) const
{
    const auto&  channelState    = channels[channel];
    std::int64_t predictedSample = 0;
    for (std::size_t i = 0; i < channelState.history.size(); ++i)
    {
        predictedSample += static_cast<std::int32_t>(channelState.history[i]) *
                           static_cast<std::int32_t>(channelState.weights[i]);
    }
    // We must right shift by 13, but that is implementation defined
    predictedSample = static_cast<std::int64_t>(std::floor(static_cast<double>(predictedSample) / (1 << 13)));
    return clampInt<std::int32_t>(predictedSample);
}

void LmsState::updateState(std::uint8_t channel, std::int32_t dequantizedResidual, std::int16_t sample)
{
    auto& channelState = channels[channel];
    // We must right shift by 4, but that is implementation defined
    const auto delta = static_cast<std::int32_t>(std::floor(dequantizedResidual / 16.0));
    for (std::size_t i = 0; i < channelState.history.size(); ++i)
        channelState.weights[i] += static_cast<std::int16_t>(channelState.history[i] < 0 ? -delta : delta);
    for (std::size_t i = 0; i < channelState.history.size() - 1; ++i)
        channelState.history[i] = channelState.history[i + 1];
    channelState.history[channelState.history.size() - 1] = sample;
}

} // namespace sf::priv::qoaFile