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

namespace sf
{
namespace Log
{
inline Level::Level()
    : Value(Verbose)
{
}
inline Level::Level(Native value)
    : Value(value)
{
}

inline bool Level::operator==(const Level &right) const
{
    return Value == right.Value;
}
inline bool Level::operator<(const Level &right) const
{
    return Value < right.Value;
}
inline bool Level::operator>(const Level &right) const
{
    return Value > right.Value;
}
inline bool operator!=(const Level &left, const Level &right)
{
    return !(left == right);
}

inline const char * Level::toString() const
{
    switch (Value)
    {
    case Verbose:
        return "verbose";
    case Debug:
        return "debug";
    case Info:
        return "info";
    case Warning:
        return "WARNING";
    case Error:
        return "ERROR";
    default:
        // should we throw an exception?
        return "INVALID";
    }
}


inline Channel::Channel(unsigned int id)
    : m_id(id)
{
}

inline bool Channel::operator==(const Channel &right) const
{
    return m_id == right.m_id;
}
inline bool Channel::operator<(const Channel &right) const
{
    return m_id < right.m_id;
}
inline bool operator!=(const Channel &left, const Channel &right)
{
    return !(left == right);
}

inline const char * Channel::toString() const
{
    switch (m_id)
    {
    case Unspecified:
        return "UNKNOWN";
    case System:
        return "SYSTEM";
    case Window:
        return "WINDOW";
    case Graphics:
        return "GRAPHICS";
    case Audio:
        return "AUDIO";
    case Network:
        return "NETWORK";
    case Reserved1:
    case Reserved2:
        // should we throw an exception?
        return "RESERVED";
    default:
        return priv::logModuleToString(*this);
    }
}
}


namespace priv
{
template< sf::Log::Channel::Native Mdl, sf::Log::Level::Native Lvl >
inline Log<Mdl, Lvl>::Log()
    : m_messageBuffer()
{
}

template<sf::Log::Channel::Native Mdl, sf::Log::Level::Native Lvl >
inline Log<Mdl, Lvl>::~Log()
{
    std::string message = m_messageBuffer.str();
    processLogEntry(sf::Log::Channel(Mdl), sf::Log::Level(Lvl), message);
}

template< sf::Log::Channel::Native Mdl, sf::Log::Level::Native Lvl >
inline std::ostream & Log<Mdl, Lvl>::get()
{
    return m_messageBuffer;
}
}
}
