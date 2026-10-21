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
#include <SFML/Audio/SoundFileWriterQoa.hpp>

#include <SFML/System/Err.hpp>
#include <SFML/System/Utils.hpp>

#include <algorithm>
#include <array>
#include <filesystem>
#include <fstream>
#include <ios>
#include <limits>
#include <optional>
#include <string_view>
#include <type_traits>
#include <vector>

#include <cassert>
#include <cstddef>
#include <cstdint>

namespace
{
template <std::size_t ByteCount, typename Iter>
Iter writeBigEndianUnsignedInt(std::uint64_t value, Iter iter)
{
    static_assert(ByteCount > 0);
    static_assert(std::is_same_v<std::uint8_t, std::decay_t<decltype(*iter)>>);
    if constexpr (ByteCount < sizeof(value))
        assert((value >> (ByteCount * 8)) == 0 && "Cannot represent value with specified number of bytes");
    auto       currentByteIter = iter + ByteCount - 1;
    const auto result          = currentByteIter + 1;
    for (std::size_t i = 0; i < ByteCount; ++i)
    {
        *currentByteIter = static_cast<std::uint8_t>(value);
        value >>= 8;
        --currentByteIter;
    }
    return result;
}

template <typename Num, typename Iter>
inline Iter writeBigEndianSignedInt(Num value, Iter begin)
{
    static_assert(std::is_signed_v<Num>);
    return writeBigEndianUnsignedInt<sizeof(Num)>(static_cast<std::make_unsigned_t<Num>>(value), begin);
}

sf::priv::qoaFile::LmsState createInitialLmsState(std::uint8_t numChannels)
{
    sf::priv::qoaFile::LmsState result;
    for (std::uint8_t i = 0; i < numChannels; ++i)
        result.channels.push_back({{0, 0, 0, 0}, {0, 0, -1, 2}});
    return result;
}

template <typename Iter>
Iter writeLmsState(Iter begin, const sf::priv::qoaFile::LmsState& lms)
{
    auto iter = begin;
    for (const auto& channel : lms.channels)
    {
        for (const auto i : channel.history)
            iter = writeBigEndianSignedInt(i, iter);
        for (const auto i : channel.weights)
            iter = writeBigEndianSignedInt(i, iter);
    }
    return iter;
}

constexpr std::array<std::int32_t, 16> reciprocalTable =
    {65536, 9363, 3121, 1457, 781, 475, 311, 216, 156, 117, 90, 71, 57, 47, 39, 32};

std::int32_t roundingDivide(std::int32_t val, std::uint8_t quantizedScaleFactor)
{
    assert(quantizedScaleFactor < reciprocalTable.size());
    const auto reciprocal = reciprocalTable[quantizedScaleFactor];
    const auto unrounded  = (val * reciprocal + (1 << 15)) >> 16;
    return unrounded + ((val > 0) - (val < 0)) - ((unrounded > 0) - (unrounded < 0));
}

constexpr std::array<std::uint8_t, 17> quantizedTable = {7, 7, 7, 5, 5, 3, 3, 1, 0, 0, 2, 2, 4, 4, 6, 6, 6};

} // namespace

namespace sf::priv
{
////////////////////////////////////////////////////////////
bool SoundFileWriterQoa::check(const std::filesystem::path& filename)
{
    return toLower(filename.extension().string()) == ".qoa";
}

////////////////////////////////////////////////////////////
SoundFileWriterQoa::~SoundFileWriterQoa()
{
    m_file.close();
}

////////////////////////////////////////////////////////////
bool SoundFileWriterQoa::open(const std::filesystem::path&     filename,
                              unsigned int                     sampleRate,
                              unsigned int                     channelCount,
                              const std::vector<SoundChannel>& channelMap)
{
    if (channelCount > std::numeric_limits<std::uint8_t>::max() ||
        !qoaFile::isChannelCountValid(static_cast<std::uint8_t>(channelCount)))
    {
        err() << "Unsupported channel count when writing QOA file" << std::endl;
        return false;
    }

    if (!qoaFile::isSampleRateValid(sampleRate))
    {
        err() << "Unsupported sample rate when writing QOA file" << std::endl;
        return false;
    }

    const auto targetChannelMap = qoaFile::getChannelMap(static_cast<std::uint8_t>(channelCount));
    if (!std::is_permutation(channelMap.begin(), channelMap.end(), targetChannelMap.begin()))
    {
        err() << "Unsupported channel when writing QOA file" << std::endl;
        return false;
    }

    // Build the remap table
    assert(targetChannelMap.size() == channelCount && channelMap.size() == channelCount);
    m_remapTable.resize(channelCount);
    for (std::size_t i = 0; i < channelCount; ++i)
        m_remapTable[i] = static_cast<std::uint8_t>(
            std::find(targetChannelMap.begin(), targetChannelMap.end(), channelMap[i]) - targetChannelMap.begin());

    m_file.open(filename, std::ios_base::binary);
    if (!m_file)
    {
        err() << "Failed to open QOA file for writing\n" << formatDebugPathInfo(filename) << std::endl;
        return false;
    }

    assert(channelCount <= std::numeric_limits<std::uint8_t>::max() && "Should have been validated before");
    m_frameSharedData = FrameSharedData{
        static_cast<std::uint8_t>(channelCount),
        static_cast<std::uint32_t>(sampleRate),
    };
    return true;
}

////////////////////////////////////////////////////////////
void SoundFileWriterQoa::write(const std::int16_t* samples, std::uint64_t count)
{
    assert(m_frameSharedData.has_value() && m_file.good() && "Must open a stream successfully first");
    const auto numChannels = static_cast<std::uint32_t>(m_frameSharedData->numChannels);
    if (count % numChannels != 0)
    {
        err() << "Cannot write to partial channels when writing QOA file" << std::endl;
        return;
    }

    const auto samplesPerChannel = count / numChannels;
    if (samplesPerChannel > std::numeric_limits<std::uint32_t>::max())
    {
        err() << "Number of samples out of supported range when writing QOA file" << std::endl;
        return;
    }

    std::vector<std::uint8_t> buffer;
    if (const auto error = seekAndWriteHeader(static_cast<std::uint32_t>(samplesPerChannel), buffer))
    {
        err() << "Failed to write QOA file header: " << *error << std::endl;
        return;
    }

    std::uint32_t writtenSamples     = 0;
    const auto    maxSamplesPerFrame = static_cast<std::uint16_t>(
        qoaFile::samplesPerSlice::value * qoaFile::maxSlicesPerChannelPerFrame::value * numChannels);
    auto lmsState = createInitialLmsState(m_frameSharedData->numChannels);
    while (writtenSamples < count)
    {
        const auto thisFrameSamples = static_cast<std::uint16_t>(
            std::min<std::uint64_t>(maxSamplesPerFrame, count - writtenSamples));
        if (const auto error = writeFrame(samples, thisFrameSamples, lmsState, buffer))
        {
            err() << "Failed to write QOA frame: " << *error << std::endl;
            return;
        }

        samples += thisFrameSamples;
        writtenSamples += thisFrameSamples;
    }
}

////////////////////////////////////////////////////////////
std::optional<std::string_view> SoundFileWriterQoa::seekAndWriteHeader(std::uint32_t              samplesPerChannel,
                                                                       std::vector<std::uint8_t>& reusedContainer)
{
    m_file.seekp(0);
    if (m_file.fail())
        return "Cannot seek to start of file";

    reusedContainer.resize(qoaFile::fileHeaderSizeByte::value);
    auto iter = writeBigEndianUnsignedInt<4>(qoaFile::magicBytes::value, reusedContainer.begin());
    writeBigEndianUnsignedInt<4>(samplesPerChannel, iter);

    m_file.write(reinterpret_cast<const char*>(reusedContainer.data()),
                 static_cast<std::streamsize>(reusedContainer.size()));
    if (m_file.fail() || m_file.bad())
        return "Unknown error while writing header";
    return std::nullopt;
}

////////////////////////////////////////////////////////////
std::optional<std::string_view> SoundFileWriterQoa::writeFrame(
    const std::int16_t*        samples,
    std::uint16_t              count,
    qoaFile::LmsState&         lmsState,
    std::vector<std::uint8_t>& reusedContainer)
{
    const auto numChannels = m_frameSharedData->numChannels;
    assert(count <= static_cast<std::uint32_t>(numChannels) * qoaFile::maxSlicesPerChannelPerFrame::value *
                        qoaFile::samplesPerSlice::value &&
           "Too many samples in a single frame");
    const auto samplesPerChannel = static_cast<std::uint16_t>(count / numChannels);
    const auto maybeHeaderError  = m_frameSharedData->writeFrameHeader(samplesPerChannel, m_file, reusedContainer);
    if (maybeHeaderError)
        return maybeHeaderError;

    const auto frameBodySizeByte = qoaFile::getFrameSizeByte(m_frameSharedData->numChannels) -
                                   qoaFile::frameHeaderSizeByte::value;

    reusedContainer.resize(static_cast<std::size_t>(frameBodySizeByte));
    auto currentIter = reusedContainer.begin();
    currentIter      = writeLmsState(currentIter, lmsState);

    for (std::uint8_t unmappedChannel = 0; unmappedChannel < numChannels; ++unmappedChannel)
    {
        const auto    mappedChannel            = m_remapTable[unmappedChannel];
        std::uint16_t encodedSamples           = 0;
        const auto*   channelSamples           = samples + mappedChannel;
        auto          prevQuantizedScaleFactor = 0;

        while (encodedSamples < samplesPerChannel)
        {
            const auto thisSliceSamples = static_cast<std::uint8_t>(
                std::min<std::uint32_t>(qoaFile::samplesPerSlice::value, samplesPerChannel - encodedSamples));
            auto bestRank = std::numeric_limits<std::uint64_t>::max();

            qoaFile::QoaSlice bestSlice;
            qoaFile::LmsState bestLms;
            std::uint8_t      bestQuantizedScaleFactor = 0;

            for (std::uint8_t scaleFactorIndex = 0; scaleFactorIndex < 16; ++scaleFactorIndex)
            {
                // There is a strong correlation between scale factors of neighboring slices,
                // so we start testing from the previous scale factor as an optimization.
                const std::uint8_t quantizedScaleFactor = (scaleFactorIndex + prevQuantizedScaleFactor) & 0b1111;
                auto               prevLmsState         = lmsState;
                std::uint64_t      currentRank          = 0;
                qoaFile::QoaSlice  slice{quantizedScaleFactor};
                for (std::uint16_t sampleIndex = encodedSamples; sampleIndex < encodedSamples + thisSliceSamples;
                     ++sampleIndex)
                {
                    const auto sample              = *(channelSamples + sampleIndex * numChannels);
                    const auto predictedSample     = prevLmsState.predictSample(mappedChannel);
                    const auto residual            = sample - predictedSample;
                    const auto unclampedScaled     = roundingDivide(residual, quantizedScaleFactor);
                    const auto clampedScaled       = std::clamp<std::int32_t>(unclampedScaled, -8, 8);
                    const auto quantizedResidual   = quantizedTable[static_cast<std::uint32_t>(clampedScaled + 8)];
                    const auto dequantizedResidual = qoaFile::dequantTable(quantizedScaleFactor, quantizedResidual);
                    const auto reconstructedSample = qoaFile::calculateSample(dequantizedResidual, predictedSample);

                    // If the weights have grown too large, we introduce a penalty
                    // here. This prevents pops/clicks in certain problem cases.
                    const auto& channelLms    = prevLmsState.channels[mappedChannel];
                    const auto  weightPenalty = std::max<std::int32_t>(((channelLms.weights[0] * channelLms.weights[0] +
                                                                        channelLms.weights[1] * channelLms.weights[1] +
                                                                        channelLms.weights[2] * channelLms.weights[2] +
                                                                        channelLms.weights[3] * channelLms.weights[3]) >>
                                                                       18) -
                                                                          0x8FF,
                                                                      0);
                    const auto  error         = static_cast<std::int64_t>(sample) - reconstructedSample;
                    currentRank += static_cast<std::uint64_t>(error * error + weightPenalty * weightPenalty);
                    if (currentRank > bestRank)
                        break;
                    prevLmsState.updateState(mappedChannel, dequantizedResidual, reconstructedSample);
                    slice = (slice.raw() << 3) | quantizedResidual;
                }

                if (currentRank < bestRank)
                {
                    bestRank                 = currentRank;
                    bestSlice                = slice;
                    bestLms                  = prevLmsState;
                    bestQuantizedScaleFactor = quantizedScaleFactor;
                }
            }

            prevQuantizedScaleFactor = bestQuantizedScaleFactor;
            lmsState                 = std::move(bestLms);
            bestSlice                = (bestSlice.raw()) << ((qoaFile::samplesPerSlice::value - thisSliceSamples) * 3);
            currentIter              = writeBigEndianUnsignedInt<8>(bestSlice.raw(), currentIter);
            encodedSamples += thisSliceSamples;
        }
    }
    m_file.write(reinterpret_cast<const char*>(reusedContainer.data()),
                 static_cast<std::streamsize>(reusedContainer.size()));
    if (m_file.fail() || m_file.bad())
        return "Unknown error while writing frame content";
    return std::nullopt;
}

std::optional<std::string_view> SoundFileWriterQoa::FrameSharedData::writeFrameHeader(
    std::uint16_t              samplesPerChannel,
    std::ofstream&             stream,
    std::vector<std::uint8_t>& reusedContainer) const
{
    reusedContainer.resize(qoaFile::frameHeaderSizeByte::value);
    auto currentIter            = writeBigEndianUnsignedInt<1>(numChannels, reusedContainer.begin());
    currentIter                 = writeBigEndianUnsignedInt<3>(sampleRate, currentIter);
    currentIter                 = writeBigEndianUnsignedInt<2>(samplesPerChannel, currentIter);
    const auto slicesPerChannel = qoaFile::samplesToSlices(samplesPerChannel);
    currentIter = writeBigEndianUnsignedInt<2>(qoaFile::getFrameSizeByte(numChannels, slicesPerChannel), currentIter);
    assert(currentIter == reusedContainer.end());
    stream.write(reinterpret_cast<const char*>(reusedContainer.data()),
                 static_cast<std::streamsize>(reusedContainer.size()));
    if (stream.fail() || stream.bad())
        return "Unknown error while writing frame header";
    return std::nullopt;
}

} // namespace sf::priv
