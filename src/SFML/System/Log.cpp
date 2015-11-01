////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007-2015 Laurent Gomila (laurent@sfml-dev.org)
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
#include <SFML/System/NonCopyable.hpp>
#include <SFML/System/Log.hpp>

#include <map>
#include <iostream>

namespace
{
void defaultLogEntryProcessor(void *, sf::Log::Channel channel, sf::Log::Level level, const std::string &message)
{
    size_t nextNL = message.find('\n');
    if (nextNL == std::string::npos)
        std::cerr << "SFML " << channel.toString() << " " << level.toString() << ": " << message << std::endl;
    else
    {
        std::string prefix = std::string("SFML ") + channel.toString() + " " + level.toString() + ": ";
        size_t start = 0;
        do 
        {
            std::cerr << prefix << message.substr(start, ++nextNL);
            start = nextNL;
            nextNL = message.find('\n', start);
        } while (nextNL != std::string::npos);
        std::cerr << prefix << message.substr(start) << std::endl;
    }
    
}

typedef std::map<sf::Log::Channel, sf::Log::Level> ChannelThresholdMap;
typedef std::map<sf::Log::Channel, std::string> ChannelNameMap;

struct LogBackend
{
    LogBackend()
        : processor(&defaultLogEntryProcessor)
#ifdef _DEBUG
        , globalThreshold(sf::Log::Level::Debug)
#else
        , globalThreshold(sf::Log::Level::Error)
#endif
        , userData(NULL)
    {
        channelThresholds[sf::Log::Channel::Unspecified]
            = channelThresholds[sf::Log::Channel::System]
            = channelThresholds[sf::Log::Channel::Window]
            = channelThresholds[sf::Log::Channel::Graphics]
            = channelThresholds[sf::Log::Channel::Audio]
            = channelThresholds[sf::Log::Channel::Network]
            = globalThreshold;

    }

    ChannelThresholdMap channelThresholds;
    ChannelNameMap customChannelNames;
    sf::Log::Manager::LogEntryProcessor processor;
    void *userData;
    sf::Log::Level globalThreshold;
} logBackend;
}

SFML_SYSTEM_API void sf::priv::processLogEntry(sf::Log::Channel channel, sf::Log::Level level, const std::string &message)
{
    logBackend.processor(logBackend.userData, channel, level, message);
}

SFML_SYSTEM_API const char * sf::priv::logModuleToString(sf::Log::Channel value)
{
    ChannelNameMap::const_iterator cIt = logBackend.customChannelNames.find(value);
    return cIt == logBackend.customChannelNames.cend() ? "UNSPECIFIED" : cIt->second.c_str();
}

SFML_SYSTEM_API bool sf::priv::isBelowThreshold(sf::Log::Channel channel, sf::Log::Level level)
{
    ChannelThresholdMap::const_iterator cIt = logBackend.channelThresholds.insert(std::make_pair(channel, logBackend.globalThreshold)).first;
    return level < logBackend.globalThreshold || level < cIt->second;
}

SFML_SYSTEM_API void sf::Log::Manager::setGlobalThreshold(sf::Log::Level threshold)
{
    logBackend.globalThreshold = threshold;
}

SFML_SYSTEM_API void sf::Log::Manager::setModuleThreshold(sf::Log::Channel channel, sf::Log::Level threshold)
{
    logBackend.channelThresholds[channel] = threshold;
}

SFML_SYSTEM_API void sf::Log::Manager::registerCustomModuleName(sf::Log::Channel channel, const std::string &name)
{
    logBackend.customChannelNames[channel] = name;
}

SFML_SYSTEM_API void sf::Log::Manager::setCustomLogEntryProcessor(sf::Log::Manager::LogEntryProcessor processor, void *userData /*= NULL*/)
{
    if (processor)
    {
        logBackend.processor = processor;
        logBackend.userData = userData;
    }
    else
    {
        logBackend.processor = &defaultLogEntryProcessor;
        logBackend.userData = NULL;
    }
}
