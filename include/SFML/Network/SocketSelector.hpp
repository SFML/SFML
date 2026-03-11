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

#pragma once

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Network/Export.hpp>

#include <SFML/System/Time.hpp>

#include <functional>
#include <memory>


namespace sf
{
class Socket;

////////////////////////////////////////////////////////////
/// \brief Multiplexer that allows to read from multiple sockets
///
////////////////////////////////////////////////////////////
class SFML_NETWORK_API SocketSelector
{
public:
    ////////////////////////////////////////////////////////////
    /// \brief Type of readiness to check for
    ///
    ////////////////////////////////////////////////////////////
    enum class ReadinessType
    {
        Receive,      //!< Check if sockets are ready to be received from
        Send,         //!< Check if sockets are ready to be sent to
        Either,       //!< Check if sockets are ready to be received from or sent to
        Both = Either //!< Alias for Either used in readiness callbacks
    };

    ////////////////////////////////////////////////////////////
    /// \brief Default constructor
    ///
    ////////////////////////////////////////////////////////////
    SocketSelector();

    ////////////////////////////////////////////////////////////
    /// \brief Destructor
    ///
    ////////////////////////////////////////////////////////////
    ~SocketSelector();

    ////////////////////////////////////////////////////////////
    /// \brief Copy constructor
    ///
    /// \param copy Instance to copy
    ///
    ////////////////////////////////////////////////////////////
    SocketSelector(const SocketSelector& copy);

    ////////////////////////////////////////////////////////////
    /// \brief Overload of assignment operator
    ///
    /// \param right Instance to assign
    ///
    /// \return Reference to self
    ///
    ////////////////////////////////////////////////////////////
    SocketSelector& operator=(const SocketSelector& right);

    ////////////////////////////////////////////////////////////
    /// \brief Move constructor
    ///
    ////////////////////////////////////////////////////////////
    SocketSelector(SocketSelector&&) noexcept;

    ////////////////////////////////////////////////////////////
    /// \brief Move assignment
    ///
    ////////////////////////////////////////////////////////////
    SocketSelector& operator=(SocketSelector&&) noexcept;

    ////////////////////////////////////////////////////////////
    /// \brief Add a new socket to the selector
    ///
    /// The type of readiness to wait for can be specified.
    /// Specifying `ReadinessType::Receive` will wait for the
    /// socket to become ready to receive data from, specifying
    /// `ReadinessType::Send` will wait for the socket to become
    /// ready to send data on. Specying `ReadinessType::Either`
    /// will wait for the socket to become either ready to send
    /// or receive data on.
    ///
    /// Adding a socket after it has already been added will just
    /// overwrite the existing readiness type with the new value.
    ///
    /// This function keeps a weak reference to the socket,
    /// so you have to make sure that the socket is not destroyed
    /// while it is stored in the selector.
    /// This function does nothing if the socket is not valid.
    ///
    /// When adding a socket to the selector you can also attach
    /// a callback along with it. The callback is called by
    /// `dispatchReadyCallbacks` when a socket is determined to be
    /// ready after a call to `wait`.
    ///
    /// Using attached callbacks instead of having to individually
    /// call `isReady` on every socket after every call to `wait`
    /// allows for scaling up to a large number of sockets. This
    /// is because the overhead of checking for socket readiness
    /// using `isReady` grows proportionally to the total number
    /// of sockets. When using callbacks calling `isReady` on
    /// every socket is no longer necessary.
    ///
    /// Because a socket can be ready for receiving, sending or
    /// both, the type of readiness is passed to the attached
    /// callback when it is called by `dispatchReadyCallbacks`.
    /// Some systems don't support combined read and write
    /// notifications. On these systems, if a socket is ready
    /// to be both received from and sent to the callback will be
    /// called twice, once with `ReadinessType::Receive` and once
    /// with `ReadinessType::Send`.
    ///
    /// To remove the attached callback of a socket, call `add`
    /// again with an empty function.
    ///
    /// By default, no readiness callback is attached when adding
    /// a socket.
    ///
    /// \param socket        Reference to the socket to add
    /// \param readinessType Type of readiness to wait for
    /// \param readyCallback Ready callback to attach to the socket, pass an empty function to remove the ready callback
    ///
    /// \return `true` if the socket was added successfully, `false` otherwise
    ///
    /// \see `remove`, `clear`
    ///
    ////////////////////////////////////////////////////////////
    bool add(const Socket&                                    socket,
             ReadinessType                                    readinessType = ReadinessType::Receive,
             std::function<void(ReadinessType readinessType)> readyCallback = {});

    ////////////////////////////////////////////////////////////
    /// \brief Remove a socket from the selector
    ///
    /// This function doesn't destroy the socket, it simply
    /// removes the reference that the selector has to it.
    ///
    /// \param socket Reference to the socket to remove
    ///
    /// \return `true` if the socket was removed successfully, `false` otherwise
    ///
    /// \see `add`, `clear`
    ///
    ////////////////////////////////////////////////////////////
    bool remove(const Socket& socket);

    ////////////////////////////////////////////////////////////
    /// \brief Remove all the sockets stored in the selector
    ///
    /// This function doesn't destroy any instance, it simply
    /// removes all the references that the selector has to
    /// external sockets.
    ///
    /// \see `add`, `remove`
    ///
    ////////////////////////////////////////////////////////////
    void clear();

    ////////////////////////////////////////////////////////////
    /// \brief Wait until one or more sockets are ready to receive or send
    ///
    /// This function returns as soon as at least one socket has
    /// some data available to be received or data can be sent,
    /// depending on how the socket was added to this selector.
    /// To know which sockets are ready, use the `isReady` function.
    /// If you use a timeout and no socket is ready before the timeout
    /// is over, the function returns `false`.
    ///
    /// \param timeout Maximum time to wait, (use Time::Zero for infinity)
    ///
    /// \return `true` if there are sockets ready, `false` otherwise
    ///
    /// \see `isReady`, `dispatchReadyCallbacks`
    ///
    ////////////////////////////////////////////////////////////
    [[nodiscard]] bool wait(Time timeout = Time::Zero);

    ////////////////////////////////////////////////////////////
    /// \brief Test a socket to know if it is ready to receive or send data
    ///
    /// This function must be used after a call to `wait`, to know
    /// which sockets are ready to receive or send data. If a socket
    /// is ready, a call to receive or send will never block because
    /// we know that there is data available to read or we can write.
    /// Note that if this function returns `true` for a TcpListener,
    /// this means that it is ready to accept a new connection.
    ///
    /// \param socket        Socket to test
    /// \param readinessType Type of readiness to check for
    ///
    /// \return `true` if the socket is ready to read, `false` otherwise
    ///
    /// \see `wait`
    ///
    ////////////////////////////////////////////////////////////
    [[nodiscard]] bool isReady(const Socket& socket, ReadinessType readinessType = ReadinessType::Receive) const;

    ////////////////////////////////////////////////////////////
    /// \brief Dispatch callbacks of ready sockets
    ///
    /// After calling `wait` returns `true`, at least one socket
    /// is ready to receive or send data. Calling
    /// `dispatchReadyCallbacks` will call the attached ready
    /// callback for every socket that is ready to either receive
    /// or send data. Sockets that don't have a callback attached
    /// can still be individually checked using `isReady`.
    ///
    /// The readiness state of each socket is maintained until
    /// the next call to `wait`. Calling `dispatchReadyCallbacks`
    /// multiple times after a single call to `wait` will run the
    /// exact same callbacks with the exact same passed arguments.
    ///
    /// \see `wait`
    ///
    ////////////////////////////////////////////////////////////
    void dispatchReadyCallbacks();

private:
    struct SocketSelectorImpl;

    ////////////////////////////////////////////////////////////
    // Member data
    ////////////////////////////////////////////////////////////
    std::unique_ptr<SocketSelectorImpl> m_impl; //!< Opaque pointer to the implementation (which requires OS-specific types)
};

} // namespace sf


////////////////////////////////////////////////////////////
/// \class sf::SocketSelector
/// \ingroup network
///
/// Socket selectors provide a way to wait until data can be
/// received or sent on a set of sockets, instead of just one.
/// This is convenient when you have multiple sockets that may
/// possibly receive data, but you don't know which one will
/// be ready first. In particular, it avoids to use a thread
/// for each socket; with selectors, a single thread can handle
/// all the sockets. When sending large amounts of data, the
/// socket send buffer might fill up and you will have to wait
/// for the data to actually be sent over the network connection
/// before attempting to send more data.
///
/// All types of sockets can be used in a selector:
/// \li `sf::TcpListener`
/// \li `sf::TcpSocket`
/// \li `sf::UdpSocket`
///
/// A selector doesn't store its own copies of the sockets
/// (socket classes are not copyable anyway), it simply keeps
/// a reference to the original sockets that you pass to the
/// "add" function. Therefore, you can't use the selector as a
/// socket container, you must store them outside and make sure
/// that they are alive as long as they are used in the selector.
///
/// Using a selector is simple:
/// \li populate the selector with all the sockets that you want to observe
/// \li make it wait until there is data available or data can be sent on any of the sockets
/// \li test each socket to find out which ones are ready for receiving or sending
///
/// If scalability is a concern, using callbacks is also possible:
/// \li add all the sockets that you want to observe to the selector with their own ready callbacks
/// \li make the selector wait until there is data available or data can be sent on any of the sockets
/// \li dispatch the callbacks of the sockets that became ready during the wait
///
/// Usage example:
/// \code
/// // Create a socket to listen to new connections
/// sf::TcpListener listener;
/// if (listener.listen(55001) != sf::Socket::Status::Done)
/// {
///     // Handle error...
/// }
///
/// // Create a list to store the future clients
/// std::vector<sf::TcpSocket> clients;
///
/// // Create a selector
/// sf::SocketSelector selector;
///
/// // Add the listener to the selector
/// selector.add(listener);
///
/// // Endless loop that waits for new connections
/// while (running)
/// {
///     // Make the selector wait for data on any socket
///     if (selector.wait())
///     {
///         // Test the listener
///         if (selector.isReady(listener))
///         {
///             // The listener is ready: there is a pending connection
///             sf::TcpSocket client;
///             if (listener.accept(client) == sf::Socket::Status::Done)
///             {
///                 // Add the new client to the selector so that we will
///                 // be notified when they send something
///                 selector.add(client);
///
///                 // Add the new client to the clients list
///                 clients.push_back(std::move(client));
///             }
///             else
///             {
///                 // Handle error...
///             }
///         }
///         else
///         {
///             // The listener socket is not ready, test all other sockets (the clients)
///             for (sf::TcpSocket& client : clients)
///             {
///                 if (selector.isReady(client))
///                 {
///                     // The client has sent some data, we can receive it
///                     sf::Packet packet;
///                     if (client.receive(packet) == sf::Socket::Status::Done)
///                     {
///                         ...
///                     }
///                 }
///             }
///         }
///     }
/// }
/// \endcode
///
/// Usage example with callbacks:
/// \code
/// // Create a socket to listen to new connections
/// sf::TcpListener listener;
/// if (listener.listen(55001) != sf::Socket::Status::Done)
/// {
///     // Handle error...
/// }
///
/// // Create a list to store the future clients
/// // We use a std::list here because modifications
/// // don't invalidate references to existing elements
/// std::list<sf::TcpSocket> clients;
///
/// // Create a selector
/// sf::SocketSelector selector;
///
/// // Listeners can only become ready to receive so we don't
/// // have to check the readiness type in their callback
/// const auto listenerCallback = [&](sf::SocketSelector::ReadinessType)
/// {
///     // The listener is ready: there is a pending connection
///     sf::TcpSocket newSocket;
///     if (listener.accept(newSocket) == sf::Socket::Status::Done)
///     {
///         // Add the new client to the clients list
///         auto& client = clients.emplace_back(std::move(newSocket));
///
///         const auto clientCallback = [&client](sf::SocketSelector::ReadinessType readinessType)
///         {
///             if (readinessType == sf::SocketSelector::ReadinessType::Receive)
///             {
///                 // The client has sent some data, we can receive it
///                 sf::Packet packet;
///                 if (client.receive(packet) == sf::Socket::Status::Done)
///                 {
///                     ...
///                 }
///             }
///         };
///
///         // Add the new client to the selector with an attached
///         // callback that will be called when they send something
///         selector.add(client, sf::SocketSelector::ReadinessType::Receive, clientCallback);
///     }
///     else
///     {
///         // Handle error...
///     }
/// };
///
/// // Add the listener to the selector with an attached callback
/// selector.add(listener, sf::SocketSelector::ReadinessType::Receive, listenerCallback);
///
/// // Endless loop that waits for new connections and receives client data
/// while (running)
/// {
///     // Make the selector wait for sockets to become ready and dispatch their callbacks
///     if (selector.wait())
///         selector.dispatchReadyCallbacks();
/// }
/// \endcode
///
/// \see `sf::Socket`
///
////////////////////////////////////////////////////////////
