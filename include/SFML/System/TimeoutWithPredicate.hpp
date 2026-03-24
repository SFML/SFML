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
#include <SFML/System/Export.hpp>

#include <SFML/System/Time.hpp>

#include <functional>

namespace sf
{

////////////////////////////////////////////////////////////
/// \brief Utility class providing hybrid functionality
///        of a timeout and a continuation predicate
///
////////////////////////////////////////////////////////////
class SFML_SYSTEM_API TimeoutWithPredicate
{
public:
    ////////////////////////////////////////////////////////////
    /// \brief Constructor
    ///
    /// This constructor constructs a `TimeoutWithPredicate` object
    /// that times out after the given amount of time.
    ///
    /// \param timeout Time to timeout after
    ///
    ////////////////////////////////////////////////////////////
    TimeoutWithPredicate(Time timeout);

    ////////////////////////////////////////////////////////////
    /// \brief Constructor
    ///
    /// This constructor constructs a `TimeoutWithPredicate` object
    /// that times out when the given predicate returns `false`.
    /// The frequency at which predicate is checked is specified
    /// by `period`.
    ///
    /// If an empty predicate is passed, it will be set to a
    /// predicate that returns `false` when called.
    ///
    /// \param predicate Predicate that returns `true` to continue or `false` to timeout
    /// \param period    The period between checks of the predicate
    ///
    ////////////////////////////////////////////////////////////
    TimeoutWithPredicate(std::function<bool()> predicate, Time period = milliseconds(1));

    ////////////////////////////////////////////////////////////
    /// \brief Get the predicate
    ///
    /// \return The predicate
    ///
    ////////////////////////////////////////////////////////////
    [[nodiscard]] const std::function<bool()>& getPredicate() const;

    ////////////////////////////////////////////////////////////
    /// \brief Get the period
    ///
    /// \return The period
    ///
    ////////////////////////////////////////////////////////////
    [[nodiscard]] const Time& getPeriod() const;

private:
    ////////////////////////////////////////////////////////////
    // Member data
    ////////////////////////////////////////////////////////////
    std::function<bool()> m_predicate; //!< The contained predicate
    Time                  m_period;    //!< The contained period
};

} // namespace sf


////////////////////////////////////////////////////////////
/// \class sf::TimeoutWithPredicate
/// \ingroup system
///
/// `sf::TimeoutWithPredicate` is a utility class that
/// provides a hybrid of the functionality of a simple
/// time based timeout and a predicate based timeout.
///
/// Functions taking a timeout parameter that is specified
/// solely by an `sf::Time` cannot be easily interrupted.
/// This becomes a problem when the time that the function
/// call takes to complete depends on factors that are not
/// within the control of the developer beforehand and thus
/// specifying a fixed timeout value is not possible.
/// Networking functions that transfer varying amounts of
/// data are good examples of this.
///
/// In order to maintain backwards compatibility,
/// `sf::TimeoutWithPredicate` objects can be implicitly
/// constructed from a simple `sf::Time` value. In this case
/// they provide the same functionality as if the function
/// would have just taken a `sf::Time` timeout parameter as
/// was the case in the past.
///
/// If the acceptable duration a function call should run
/// for depends on information that is only available at
/// run time, the `sf::TimeoutWithPredicate` object can be
/// constructed from a predicate instead.
///
/// The predicate takes no arguments and must return `true`
/// if the function should continue running or `false` if
/// the function should end as soon as possible with a
/// timeout error.
///
/// Functions that support taking a `sf::TimeoutWithPredicate`
/// internally break their implementation down into discrete
/// phases after which the predicate is re-evalutated. If a
/// phase itself requires calling a blocking function, the
/// timeout value provided to the blocking function is
/// specified by providing a period value when initially
/// constructing the `sf::TimeoutWithPredicate` object.
///
/// The optional `period` parameter is merely a hint to the
/// implementation specifying the frequency at which the
/// predicate should be re-evaluated. Depending on the
/// concrete function that is called and how it is
/// implemented the re-evaluation frequency can be kept as
/// close as possible to the specified `period` value or
/// when it is not possible the `period` value will not be
/// respected at all.
///
/// Any callable object can be passed as the predicate. This
/// includes lambda expressions, `std::function` objects and
/// function pointers.
///
/// The predicate will be evaluated from the execution
/// context of the function call in which the
/// `sf::TimeoutWithPredicate` is passed. If the predicate
/// relies on evaluating data that is set from another
/// execution context e.g. another thread while the function
/// call is still in progress care has to be taken to ensure
/// proper synchronization to avoid data races between
/// multiple threads that simultaneously read and write to
/// the shared data.
///
///
/// Usage example:
/// \code
/// // Simple timeout, implicitly constructed, not interruptible
/// const auto result = blockingFunction(sf::milliseconds(1000));
///
/// ... Check result to see if the function timed out ...
///
/// // Predicate timeout, based on data provided
/// // by the function call itself
/// std::vector<char> buffer;
/// const auto readResult = readDataIntoBuffer(buffer, [&buffer]
/// {
///     // Keep reading data until we have at least 1024 bytes
///     return buffer.size() <= 1024u;
/// });
///
/// ... Check result to see if the function timed out ...
/// // In this case a "timeout" could also mean the
/// // buffer contains at least 1024 bytes of data
///
/// // Predicate timeout, based on data shared by multiple threads
/// // Set evalutation frequency to 100 milliseconds
/// std::vector<char> receivedData;
/// std::mutex mutex;
/// bool applicationRunning = true;
/// std::thread workerThread([&]
/// {
///     const auto result = receiveDataFromNetwork(receivedData, [&]
///     {
///         const std::lock_guard lock(mutex);
///         return applicationRunning;
///     }, sf::milliseconds(100));
///
///     ... Check result to see if the function timed out ...
///     // In this case a "timeout" could also mean
///     // application exit was requested
/// });
///
/// ...
///
/// // The user requested to exit the application
/// {
///     const std::lock_guard lock(mutex);
///     applicationRunning = false;
/// }
/// workerThread.join();
/// \endcode
///
////////////////////////////////////////////////////////////
