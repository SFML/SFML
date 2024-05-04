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

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Audio/AudioDevice.hpp>
#include <SFML/Audio/MiniaudioUtils.hpp>
#include <SFML/Audio/SoundStream.hpp>

#include <SFML/System/Err.hpp>
#include <SFML/System/Sleep.hpp>

#include <miniaudio.h>

#include <algorithm>
#include <ostream>
#include <vector>

#include <cassert>
#include <cstring>


namespace sf
{
struct SoundStream::Impl
{
    Impl(SoundStream* ownerPtr) : owner(ownerPtr)
    {
        static constexpr ma_data_source_vtable vtable{read, seek, getFormat, getCursor, getLength, setLooping, /* flags */ 0};
        priv::MiniaudioUtils::initializeSound(vtable, dataSourceBase, sound, [this] { initialize(); });
    }

    ~Impl()
    {
        ma_sound_uninit(&sound);
        ma_node_uninit(&effectNode, nullptr);
        ma_data_source_uninit(&dataSourceBase);
    }

    void initialize()
    {
        // Initialize the sound
        auto* engine = priv::AudioDevice::getEngine();

        if (engine == nullptr)
        {
            err() << "Failed to initialize sound: No engine available" << std::endl;
            return;
        }

        ma_sound_config soundConfig;

        soundConfig                      = ma_sound_config_init();
        soundConfig.pDataSource          = this;
        soundConfig.pEndCallbackUserData = this;
        soundConfig.endCallback          = [](void* userData, ma_sound* soundPtr)
        {
            // Seek back to the start of the sound when it finishes playing
            auto& impl     = *static_cast<Impl*>(userData);
            impl.streaming = true;
            impl.status    = Status::Stopped;

            if (const ma_result result = ma_sound_seek_to_pcm_frame(soundPtr, 0); result != MA_SUCCESS)
                err() << "Failed to seek sound to frame 0: " << ma_result_description(result) << std::endl;
        };

        if (const ma_result result = ma_sound_init_ex(engine, &soundConfig, &sound); result != MA_SUCCESS)
        {
            err() << "Failed to initialize sound: " << ma_result_description(result) << std::endl;
            return;
        }

        // Initialize the custom effect node
        effectNodeVTable.onProcess =
            [](ma_node* node, const float** framesIn, ma_uint32* frameCountIn, float** framesOut, ma_uint32* frameCountOut)
        { static_cast<EffectNode*>(node)->impl->processEffect(framesIn, *frameCountIn, framesOut, *frameCountOut); };
        effectNodeVTable.onGetRequiredInputFrameCount = nullptr;
        effectNodeVTable.inputBusCount                = 1;
        effectNodeVTable.outputBusCount               = 1;
        effectNodeVTable.flags = MA_NODE_FLAG_CONTINUOUS_PROCESSING | MA_NODE_FLAG_ALLOW_NULL_INPUT;

        const auto     nodeChannelCount = ma_engine_get_channels(engine);
        ma_node_config nodeConfig       = ma_node_config_init();
        nodeConfig.vtable               = &effectNodeVTable;
        nodeConfig.pInputChannels       = &nodeChannelCount;
        nodeConfig.pOutputChannels      = &nodeChannelCount;

        if (const ma_result result = ma_node_init(ma_engine_get_node_graph(engine), &nodeConfig, nullptr, &effectNode);
            result != MA_SUCCESS)
        {
            err() << "Failed to initialize effect node: " << ma_result_description(result) << std::endl;
            return;
        }

        effectNode.impl         = this;
        effectNode.channelCount = nodeChannelCount;

        // Route the sound through the effect node depending on whether an effect processor is set
        connectEffect(bool{effectProcessor});

        // Because we are providing a custom data source, we have to provide the channel map ourselves
        if (!channelMap.empty())
        {
            soundChannelMap.clear();

            for (const SoundChannel channel : channelMap)
            {
                soundChannelMap.push_back(priv::MiniaudioUtils::soundChannelToMiniaudioChannel(channel));
            }

            sound.engineNode.spatializer.pChannelMapIn = soundChannelMap.data();
        }
        else
        {
            sound.engineNode.spatializer.pChannelMapIn = nullptr;
        }
    }

    void reinitialize()
    {
        priv::MiniaudioUtils::reinitializeSound(sound,
                                                [this]
                                                {
                                                    ma_node_uninit(&effectNode, nullptr);
                                                    initialize();
                                                });
    }

    void processEffect(const float** framesIn, ma_uint32& frameCountIn, float** framesOut, ma_uint32& frameCountOut) const
    {
        // If a processor is set, call it
        if (effectProcessor)
        {
            if (!framesIn)
                frameCountIn = 0;

            effectProcessor(framesIn ? framesIn[0] : nullptr, frameCountIn, framesOut[0], frameCountOut, effectNode.channelCount);
            return;
        }

        // Otherwise just pass the data through 1:1
        if (framesIn == nullptr)
        {
            frameCountIn  = 0;
            frameCountOut = 0;
            return;
        }

        const auto toProcess = std::min(frameCountIn, frameCountOut);
        std::memcpy(framesOut[0], framesIn[0], toProcess * effectNode.channelCount * sizeof(float));
        frameCountIn  = toProcess;
        frameCountOut = toProcess;
    }

    void connectEffect(bool connect)
    {
        auto* engine = priv::AudioDevice::getEngine();

        if (engine == nullptr)
        {
            err() << "Failed to connect effect: No engine available" << std::endl;
            return;
        }

        if (connect)
        {
            // Attach the custom effect node output to our engine endpoint
            if (const ma_result result = ma_node_attach_output_bus(&effectNode, 0, ma_engine_get_endpoint(engine), 0);
                result != MA_SUCCESS)
            {
                err() << "Failed to attach effect node output to endpoint: " << ma_result_description(result) << std::endl;
                return;
            }
        }
        else
        {
            // Detach the custom effect node output from our engine endpoint
            if (const ma_result result = ma_node_detach_output_bus(&effectNode, 0); result != MA_SUCCESS)
            {
                err() << "Failed to detach effect node output from endpoint: " << ma_result_description(result)
                      << std::endl;
                return;
            }
        }

        // Attach the sound output to the custom effect node or the engine endpoint
        if (const ma_result
                result = ma_node_attach_output_bus(&sound, 0, connect ? &effectNode : ma_engine_get_endpoint(engine), 0);
            result != MA_SUCCESS)
        {
            err() << "Failed to attach sound node output to effect node: " << ma_result_description(result) << std::endl;
            return;
        }
    }

    static ma_result read(ma_data_source* dataSource, void* framesOut, ma_uint64 frameCount, ma_uint64* framesRead)
    {
        auto& impl  = *static_cast<Impl*>(dataSource);
        auto* owner = impl.owner;

        // Try to fill our buffer with new samples if the source is still willing to stream data
        if (impl.sampleBuffer.empty() && impl.streaming)
        {
            Chunk chunk;

            impl.streaming = owner->onGetData(chunk);

            if (chunk.samples && chunk.sampleCount)
            {
                impl.sampleBuffer.assign(chunk.samples, chunk.samples + chunk.sampleCount);
                impl.sampleBufferCursor = 0;
            }
        }

        // Push the samples to miniaudio
        if (!impl.sampleBuffer.empty())
        {
            // Determine how many frames we can read
            *framesRead = std::min<ma_uint64>(frameCount,
                                              (impl.sampleBuffer.size() - impl.sampleBufferCursor) / impl.channelCount);

            const auto sampleCount = *framesRead * impl.channelCount;

            // Copy the samples to the output
            std::memcpy(framesOut,
                        impl.sampleBuffer.data() + impl.sampleBufferCursor,
                        static_cast<std::size_t>(sampleCount) * sizeof(impl.sampleBuffer[0]));

            impl.sampleBufferCursor += static_cast<std::size_t>(sampleCount);
            impl.samplesProcessed += sampleCount;

            if (impl.sampleBufferCursor >= impl.sampleBuffer.size())
            {
                impl.sampleBuffer.clear();
                impl.sampleBufferCursor = 0;

                // If we are looping and at the end of the loop, set the cursor back to the beginning of the loop
                if (!impl.streaming && impl.loop)
                {
                    if (const auto seekPositionAfterLoop = owner->onLoop())
                    {
                        impl.streaming        = true;
                        impl.samplesProcessed = *seekPositionAfterLoop;
                    }
                }
            }
        }
        else
        {
            *framesRead = 0;
        }

        return MA_SUCCESS;
    }

    static ma_result seek(ma_data_source* dataSource, ma_uint64 frameIndex)
    {
        auto& impl  = *static_cast<Impl*>(dataSource);
        auto* owner = impl.owner;

        impl.streaming = true;
        impl.sampleBuffer.clear();
        impl.sampleBufferCursor = 0;
        impl.samplesProcessed   = frameIndex * impl.channelCount;

        if (impl.sampleRate != 0)
        {
            owner->onSeek(seconds(static_cast<float>(frameIndex / impl.sampleRate)));
        }
        else
        {
            owner->onSeek(Time::Zero);
        }

        return MA_SUCCESS;
    }

    static ma_result getFormat(ma_data_source* dataSource,
                               ma_format*      format,
                               ma_uint32*      channels,
                               ma_uint32*      sampleRate,
                               ma_channel*,
                               size_t)
    {
        const auto& impl = *static_cast<const Impl*>(dataSource);

        // If we don't have valid values yet, initialize with defaults so sound creation doesn't fail
        *format     = ma_format_s16;
        *channels   = impl.channelCount ? impl.channelCount : 1;
        *sampleRate = impl.sampleRate ? impl.sampleRate : 44100;

        return MA_SUCCESS;
    }

    static ma_result getCursor(ma_data_source* dataSource, ma_uint64* cursor)
    {
        auto& impl = *static_cast<Impl*>(dataSource);
        *cursor    = impl.channelCount ? impl.samplesProcessed / impl.channelCount : 0;

        return MA_SUCCESS;
    }

    static ma_result getLength(ma_data_source*, ma_uint64* length)
    {
        *length = 0;

        return MA_NOT_IMPLEMENTED;
    }

    static ma_result setLooping(ma_data_source* dataSource, ma_bool32 looping)
    {
        static_cast<Impl*>(dataSource)->loop = (looping == MA_TRUE);

        return MA_SUCCESS;
    }

    ////////////////////////////////////////////////////////////
    // Member data
    ////////////////////////////////////////////////////////////
    struct EffectNode
    {
        ma_node_base base{};
        Impl*        impl{};
        ma_uint32    channelCount{};
    };

    ma_data_source_base dataSourceBase{}; //!< The struct that makes this object a miniaudio data source (must be first member)
    SoundStream* const      owner;        //!< Owning SoundStream object
    ma_node_vtable          effectNodeVTable{}; //!< Vtable of the effect node
    EffectNode              effectNode;         //!< The engine node that performs effect processing
    std::vector<ma_channel> soundChannelMap; //!< The map of position in sample frame to sound channel (miniaudio channels)
    ma_sound                sound{};         //!< The sound
    std::vector<std::int16_t> sampleBuffer;            //!< Our temporary sample buffer
    std::size_t               sampleBufferCursor{};    //!< The current read position in the temporary sample buffer
    std::uint64_t             samplesProcessed{};      //!< Number of samples processed since beginning of the stream
    unsigned int              channelCount{};          //!< Number of channels (1 = mono, 2 = stereo, ...)
    unsigned int              sampleRate{};            //!< Frequency (samples / second)
    std::vector<SoundChannel> channelMap{};            //!< The map of position in sample frame to sound channel
    bool                      loop{};                  //!< Loop flag (true to loop, false to play once)
    bool                      streaming{true};         //!< True if we are still streaming samples from the source
    Status                    status{Status::Stopped}; //!< The status
    EffectProcessor           effectProcessor;         //!< The effect processor
};


////////////////////////////////////////////////////////////
SoundStream::SoundStream() : m_impl(std::make_unique<Impl>(this))
{
}


////////////////////////////////////////////////////////////
SoundStream::~SoundStream() = default;


////////////////////////////////////////////////////////////
void SoundStream::initialize(unsigned int channelCount, unsigned int sampleRate, const std::vector<SoundChannel>& channelMap)
{
    m_impl->channelCount     = channelCount;
    m_impl->sampleRate       = sampleRate;
    m_impl->channelMap       = channelMap;
    m_impl->samplesProcessed = 0;

    m_impl->reinitialize();
}


////////////////////////////////////////////////////////////
void SoundStream::play()
{
    if (m_impl->status == Status::Playing)
        setPlayingOffset(Time::Zero);

    if (const ma_result result = ma_sound_start(&m_impl->sound); result != MA_SUCCESS)
    {
        err() << "Failed to start playing sound: " << ma_result_description(result) << std::endl;
    }
    else
    {
        m_impl->status = Status::Playing;
    }
}


////////////////////////////////////////////////////////////
void SoundStream::pause()
{
    if (const ma_result result = ma_sound_stop(&m_impl->sound); result != MA_SUCCESS)
    {
        err() << "Failed to stop playing sound: " << ma_result_description(result) << std::endl;
    }
    else
    {
        if (m_impl->status == Status::Playing)
            m_impl->status = Status::Paused;
    }
}


////////////////////////////////////////////////////////////
void SoundStream::stop()
{
    if (const ma_result result = ma_sound_stop(&m_impl->sound); result != MA_SUCCESS)
    {
        err() << "Failed to stop playing sound: " << ma_result_description(result) << std::endl;
    }
    else
    {
        setPlayingOffset(Time::Zero);
        m_impl->status = Status::Stopped;
    }
}


////////////////////////////////////////////////////////////
unsigned int SoundStream::getChannelCount() const
{
    return m_impl->channelCount;
}


////////////////////////////////////////////////////////////
unsigned int SoundStream::getSampleRate() const
{
    return m_impl->sampleRate;
}


////////////////////////////////////////////////////////////
std::vector<SoundChannel> SoundStream::getChannelMap() const
{
    return m_impl->channelMap;
}


////////////////////////////////////////////////////////////
SoundStream::Status SoundStream::getStatus() const
{
    return m_impl->status;
}


////////////////////////////////////////////////////////////
void SoundStream::setPlayingOffset(Time timeOffset)
{
    if (m_impl->sampleRate == 0)
        return;

    if (m_impl->sound.pDataSource == nullptr || m_impl->sound.engineNode.pEngine == nullptr)
        return;

    const auto frameIndex = priv::MiniaudioUtils::getFrameIndex(m_impl->sound, timeOffset);

    m_impl->streaming = true;
    m_impl->sampleBuffer.clear();
    m_impl->sampleBufferCursor = 0;
    m_impl->samplesProcessed   = frameIndex * m_impl->channelCount;

    onSeek(seconds(static_cast<float>(frameIndex / m_impl->sampleRate)));
}


////////////////////////////////////////////////////////////
Time SoundStream::getPlayingOffset() const
{
    if (m_impl->channelCount == 0 || m_impl->sampleRate == 0)
        return {};

    return priv::MiniaudioUtils::getPlayingOffset(m_impl->sound);
}


////////////////////////////////////////////////////////////
void SoundStream::setLoop(bool loop)
{
    ma_sound_set_looping(&m_impl->sound, loop ? MA_TRUE : MA_FALSE);
}


////////////////////////////////////////////////////////////
bool SoundStream::getLoop() const
{
    return ma_sound_is_looping(&m_impl->sound) == MA_TRUE;
}


////////////////////////////////////////////////////////////
void SoundStream::setEffectProcessor(EffectProcessor effectProcessor)
{
    m_impl->effectProcessor = std::move(effectProcessor);
    m_impl->connectEffect(bool{m_impl->effectProcessor});
}


////////////////////////////////////////////////////////////
std::optional<std::uint64_t> SoundStream::onLoop()
{
    onSeek(Time::Zero);
    return 0;
}


////////////////////////////////////////////////////////////
void* SoundStream::getSound() const
{
    return &m_impl->sound;
}

} // namespace sf
