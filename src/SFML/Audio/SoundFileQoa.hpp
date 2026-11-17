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

#pragma once

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Audio/SoundChannel.hpp>

#include <array>
#include <type_traits>
#include <vector>

#include <cstdint>

namespace sf::priv::qoaFile
{
using magicBytes                  = std::integral_constant<std::uint32_t, 0x716F6166>; // "qoaf"
using lmsStatePerChannelSizeByte  = std::integral_constant<std::uint16_t, 16>;
using frameHeaderSizeByte         = std::integral_constant<std::uint16_t, 8>;
using fileHeaderSizeByte          = std::integral_constant<std::uint16_t, 8>;
using sliceSizeByte               = std::integral_constant<std::uint16_t, 8>;
using maxSlicesPerChannelPerFrame = std::integral_constant<std::uint16_t, 256>;
using samplesPerSlice             = std::integral_constant<std::uint8_t, 20>;

[[nodiscard]] bool                      isChannelCountValid(std::uint8_t channelCount);
[[nodiscard]] bool                      isSampleRateValid(std::uint32_t sampleRate);
[[nodiscard]] std::int32_t              dequantTable(std::uint8_t quantizedScaleFactor, std::uint8_t quantizedResidual);
[[nodiscard]] std::vector<SoundChannel> getChannelMap(std::uint8_t channelCount);
[[nodiscard]] std::uint16_t             getFrameSizeByte(std::uint8_t  channelCount,
                                                         std::uint16_t slicesPerChannel = maxSlicesPerChannelPerFrame::value);

template <typename SizeType>
[[nodiscard]] SizeType samplesToSlices(SizeType samples)
{
    static_assert(std::is_unsigned_v<SizeType>);
    return static_cast<SizeType>((samples + samplesPerSlice::value - 1) / samplesPerSlice::value);
}

[[nodiscard]] std::int16_t calculateSample(std::int32_t dequantizedResidual, std::int32_t predictedSample);

class QoaSlice
{
public:
    QoaSlice();
    explicit QoaSlice(std::uint64_t bits);
    QoaSlice&                   operator=(std::uint64_t bits);
    [[nodiscard]] std::uint64_t raw() const;
    [[nodiscard]] std::uint8_t  sfQuant() const;
    [[nodiscard]] std::uint8_t  qr0x(std::uint8_t x) const;

private:
    std::uint64_t m_bits;
};

struct LmsState
{
    [[nodiscard]] std ::int32_t predictSample(std::uint8_t channel) const;
    void updateState(std::uint8_t channel, std::int32_t dequantizedResidual, std::int16_t sample);

    struct PerChannelState
    {
        std::array<std::int16_t, 4> history;
        std::array<std::int16_t, 4> weights;
    };
    std::vector<PerChannelState> channels;
};

} // namespace sf::priv::qoaFile
