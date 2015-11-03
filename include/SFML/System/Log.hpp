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
/// \file
/// \brief Defines the public logger interface
///
////////////////////////////////////////////////////////////

#ifndef SFML_LOG_HPP
#define SFML_LOG_HPP

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/System/NonCopyable.hpp>
#include <string>
#include <sstream>

namespace sf
{
namespace Log
{
////////////////////////////////////////////////////////////
/// \brief Specifies the category of a log entry
///
////////////////////////////////////////////////////////////
struct Level
{
    ////////////////////////////////////////////////////////////
    /// \brief The different supported log entry categories
    /// 
    ////////////////////////////////////////////////////////////
    enum Native
    {
        Verbose,    ///< Very verbose information, which normally isn't necessary
        Debug,      ///< Information assisting debugging
        Info,       ///< Informational message like a system property, etc.
        Warning,    ///< Recoverable failure
        Error,      ///< Failure information
    };

    ////////////////////////////////////////////////////////////
    /// \brief Not useful, but required by stl containers.
    ///
    /// This constructor is equivalent to Level(Native::Verbose)
    ////////////////////////////////////////////////////////////
    inline Level();
    ////////////////////////////////////////////////////////////
    /// \brief Initializes a new instance representing the given
    ///        category
    ///
    /// \param[in] value will be the significance represented by
    ///            the new instance
    ///
    ////////////////////////////////////////////////////////////
    inline Level(Native value);

    ////////////////////////////////////////////////////////////
    /// \brief Equality comparison
    /// 
    /// \param[in] right operand
    ///
    /// \returns true if the left operand is as significant as
    ///          the right operand
    ///
    ////////////////////////////////////////////////////////////
    inline bool operator==(const Level &right) const;
    ////////////////////////////////////////////////////////////
    /// \brief Lesser comparison
    ///
    /// \param[in] right operand  
    ///  
    /// \returns true if the left hand operand is less significant
    ///
    ////////////////////////////////////////////////////////////
    inline bool operator<(const Level &right) const;
    ////////////////////////////////////////////////////////////
    /// \brief Greater comparison
    ///
    /// \param[in] right hand operand 
    ///
    /// \returns true if the left hand operand is more significant
    ///
    ////////////////////////////////////////////////////////////
    inline bool operator>(const Level &right) const;

    ////////////////////////////////////////////////////////////
    /// \brief Returns a string identifier for the represented
    ///        significance
    ///
    /// \returns a pointer to the null terminated string
    ///          associated with the Level represented.
    ///
    ////////////////////////////////////////////////////////////
    inline const char * toString() const;

private:
    Native Value;
};

////////////////////////////////////////////////////////////
/// \brief Represents a log channel with which log entries
///        can be grouped
///
////////////////////////////////////////////////////////////
struct Channel
{
    ////////////////////////////////////////////////////////////
    /// \brief The standard SFML log channels
    ///
    /// \note You are not limited to the channels specified here,
    ///       you can use any other integer to represent your own
    ///       channels, just associate a name with them through
    ///       sf::Log::Manager::registerCustomModuleName
    ///
    ////////////////////////////////////////////////////////////
    enum Native
    {
        Unspecified,
        System,         ///< SFML System module channel \see sfLogSys
        Window,         ///< SFML Window module channel \see sfLogWnd
        Graphics,       ///< SFML Graphics module channel \see sfLogGra
        Audio,          ///< SFML Audio module channel \see sfLogAud
        Network,        ///< SFML Network module channel \see sfLogNet
        Reserved1,
        Reserved2,
    };

    ////////////////////////////////////////////////////////////
    /// \brief Initializes a new instance representing the given
    ///        channel
    ///
    /// \param[in] id identifies the channel to be represented,
    ///            this is either a Native value or a user
    ///            defined one
    ///
    ////////////////////////////////////////////////////////////
    inline Channel(unsigned int id);

    ////////////////////////////////////////////////////////////
    /// \brief Equality comparison
    ///
    /// \param[in] right operand
    ///
    /// \returns true if the left operand represents the same
    ///          channel as the right operand
    ///
    ////////////////////////////////////////////////////////////
    inline bool operator==(const Channel &right) const;
    ////////////////////////////////////////////////////////////
    /// \brief Lesser comparison, not meaningful, but required
    ///        by some containers (e.g. std::map)
    ///
    /// \param[in] right hand operand
    ///
    /// \returns true if the left  operand is represented by a
    ///          lesser value than the right operand
    ///
    ////////////////////////////////////////////////////////////
    inline bool operator<(const Channel &right) const;

    ////////////////////////////////////////////////////////////
    /// \brief Returns a string identifier for the represented
    ///        significance
    ///
    /// \returns a pointer to the null terminated string
    ///          associated with the Channel represented.
    ///
    ////////////////////////////////////////////////////////////
    inline const char * toString() const;

private:
    unsigned int m_id;
};

////////////////////////////////////////////////////////////
/// \brief SFML log adjustment functionality
/// 
/// \see Log.hpp
///
////////////////////////////////////////////////////////////
namespace Manager
{
////////////////////////////////////////////////////////////
/// \brief The signature of an user defined log entry sink
///
/// \param[in] userData will be the value with which
///            setCustomLogEntryProcessor was called
/// \param[in] channel specifies the Channel to which the
///            message to be logged belongs
/// \param[in] level specifies the importance of the log entry
/// \param[in] message is obviously the text to be logged
///
////////////////////////////////////////////////////////////
typedef void(*LogEntryProcessor)(void *userData, Channel channel, Level level, const std::string &message);

////////////////////////////////////////////////////////////
/// \brief Sets the global logger threshold
///
/// \param[in] threshold All log entries which are less
///                      important won't be logged.
///
/// \note Defaults to Level::Debug for debug builds and to
/// Level::Error for release builds.
///
////////////////////////////////////////////////////////////
SFML_SYSTEM_API void setGlobalThreshold(Level threshold);
////////////////////////////////////////////////////////////
/// \brief Sets the logger threshold for a Channel
///
/// \param[in] channel for which the threshold should be set
/// \param[in] threshold All log entries which are less
///                      important won't be logged.
/// 
/// \note All standard channels default to Level::Debug for
/// debug builds and to Level::Error for release builds.
/// Custom channels however default to the global threshold
/// at the time when the first check with isBelowThreshold
/// happens (which is called by sfLog()).
///
////////////////////////////////////////////////////////////
SFML_SYSTEM_API void setModuleThreshold(Channel channel, Level threshold);

////////////////////////////////////////////////////////////
/// \brief Associates a name with a channel
///
/// \param[in] channel for which the name should be set
/// \param[in] name to be associated with the channel
///
/// \post The given name will be returned by Channel::toString()
///       if the Channel instances equals the given one.
///
/// \note The standard SFML channel names can't be overridden.
/// 
////////////////////////////////////////////////////////////
SFML_SYSTEM_API void registerCustomModuleName(Channel channel, const std::string &name);

////////////////////////////////////////////////////////////
/// \brief Installs a different log backend
/// 
/// By default all log entries are piped to std::cerr, with
/// this function however you can format and redirect all
/// log entries however and wherever you want.
///
/// \param[in] processor points to your custom log entry sink
/// \param[in] userData will be passed to every subsequent
///                     call to your custom processor
///
/// \note if processor == NULL the default log entry processor
///       will be used.
///
////////////////////////////////////////////////////////////
SFML_SYSTEM_API void setCustomLogEntryProcessor(LogEntryProcessor processor, void *userData = NULL);
}
}

namespace priv
{
SFML_SYSTEM_API bool isBelowThreshold(sf::Log::Channel channel, sf::Log::Level level);
SFML_SYSTEM_API void processLogEntry(sf::Log::Channel channel, sf::Log::Level level, const std::string &message);
SFML_SYSTEM_API const char * logModuleToString(sf::Log::Channel value);

template< sf::Log::Channel::Native Mdl, sf::Log::Level::Native Lvl >
class Log : sf::NonCopyable
{
public:
    inline Log();
    inline ~Log();

    inline std::ostream & get();

private:
    std::ostringstream m_messageBuffer;
};
}
}

#include <SFML/System/Log.inl>

#ifdef SFML_DISABLE_LOGGING
#define sfLog(channel, level, message)
#else
////////////////////////////////////////////////////////////
/// \brief Adds a log entry of the given category to the
///        specified channel
///
/// \param[in] channel with which the new log entry will be
///                    associated
/// \param[in] level specifies the log category
///
/// \returns a std::ostream reference.
///
/// You may use this and all derived macros like so:
/// \code
/// sfLog(sf::Log::Channel::System, sf::Log::Level::Error,
///     "Something failed; additional formatted data: " << data);
/// \endcode
/// The default log entry processor would output the following to std::cerr:
/// \code
/// SFML SYSTEM ERROR: Something failed; additional formatted data: ostreamed data
/// \endcode
///
/// \note It is recommended to use an appropriate shorthand macro
/// instead of this one.
///
////////////////////////////////////////////////////////////
#define sfLog(channel, level, message) do { if (::sf::priv::isBelowThreshold(::sf::Log::Channel(channel), ::sf::Log::Level(level))) { } \
                                       else ::sf::priv::Log<(channel), (level)>().get() << message; } while (0) 
#endif

#define sfLogErr(channel, message)   sfLog(channel, ::sf::Log::Level::Error, message)
#define sfLogWarn(channel, message)  sfLog(channel, ::sf::Log::Level::Warning, message)
#define sfLogInfo(channel, message)  sfLog(channel, ::sf::Log::Level::Info, message)
#define sfLogDebug(channel, message) sfLog(channel, ::sf::Log::Level::Debug, message)

/// \see sfLog()
#define sfLogSys(level, message) sfLog(::sf::Log::Channel::System, level, message)
#define sfLogSysErr(message)     sfLogSys(::sf::Log::Level::Error, message)
#define sfLogSysWarn(message)    sfLogSys(::sf::Log::Level::Warning, message)
#define sfLogSysInfo(message)    sfLogSys(::sf::Log::Level::Info, message)
#define sfLogSysDebug(message)   sfLogSys(::sf::Log::Level::Debug, message)

/// \see sfLog()
#define sfLogWnd(level, message) sfLog(::sf::Log::Channel::Window, level, message)
#define sfLogWndErr(message)     sfLogWnd(::sf::Log::Level::Error, message)
#define sfLogWndWarn(message)    sfLogWnd(::sf::Log::Level::Warning, message)
#define sfLogWndInfo(message)    sfLogWnd(::sf::Log::Level::Info, message)
#define sfLogWndDebug(message)   sfLogWnd(::sf::Log::Level::Debug, message)

/// \see sfLog()
#define sfLogGra(level, message) sfLog(::sf::Log::Channel::Graphics, level, message)
#define sfLogGraErr(message)     sfLogGra(::sf::Log::Level::Error, message)
#define sfLogGraWarn(message)    sfLogGra(::sf::Log::Level::Warning, message)
#define sfLogGraInfo(message)    sfLogGra(::sf::Log::Level::Info, message)
#define sfLogGraDebug(message)   sfLogGra(::sf::Log::Level::Debug, message)

/// \see sfLog()
#define sfLogNet(level, message) sfLog(::sf::Log::Channel::Network, level, message)
#define sfLogNetErr(message)     sfLogNet(::sf::Log::Level::Error, message)
#define sfLogNetWarn(message)    sfLogNet(::sf::Log::Level::Warning, message)
#define sfLogNetInfo(message)    sfLogNet(::sf::Log::Level::Info, message)
#define sfLogNetDebug(message)   sfLogNet(::sf::Log::Level::Debug, message)

/// \see sfLog()
#define sfLogAud(level, message) sfLog(::sf::Log::Channel::Audio, level, message)
#define sfLogAudErr(message)     sfLogAud(::sf::Log::Level::Error, message)
#define sfLogAudWarn(message)    sfLogAud(::sf::Log::Level::Warning, message)
#define sfLogAudInfo(message)    sfLogAud(::sf::Log::Level::Info, message)
#define sfLogAudDebug(message)   sfLogAud(::sf::Log::Level::Debug, message)

#endif

////////////////////////////////////////////////////////////
/// \file
///
////////////////////////////////////////////////////////////
