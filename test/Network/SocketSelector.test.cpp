#include <SFML/Network/SocketSelector.hpp>

// Other 1st party headers
#include <SFML/Network/TcpListener.hpp>
#include <SFML/Network/TcpSocket.hpp>
#include <SFML/Network/UdpSocket.hpp>

#include <catch2/catch_test_macros.hpp>

#include <NetworkUtil.hpp>
#include <algorithm>
#include <list>
#include <memory>
#include <random>
#include <type_traits>

#include <cstring>

TEST_CASE("[Network] sf::SocketSelector")
{
    SECTION("Type traits")
    {
        STATIC_CHECK(std::is_copy_constructible_v<sf::SocketSelector>);
        STATIC_CHECK(std::is_copy_assignable_v<sf::SocketSelector>);
        STATIC_CHECK(std::is_nothrow_move_constructible_v<sf::SocketSelector>);
        STATIC_CHECK(std::is_nothrow_move_assignable_v<sf::SocketSelector>);
    }

    SECTION("Construction")
    {
        const sf::UdpSocket      socket;
        const sf::SocketSelector socketSelector;
        CHECK(!socketSelector.isReady(socket));
    }
}

TEST_CASE("[Network] sf::SocketSelector Many Connections", runLoopbackTests())
{
    // Setting connection count to 1024 / 2 + 1 should force this test to fail if the select()-based backend is used
    static constexpr std::size_t connectionCount = 1024 / 2 + 1;
    static constexpr auto connectionBacklogMax   = 64; // Try to stay under the typical system listen queue size of 128
    static constexpr std::size_t randomDataSize  = 32 * 1500; // 32x typical MTU size

    std::mt19937 gen(std::random_device{}());
    // NOLINTNEXTLINE(cppcoreguidelines-avoid-c-arrays,modernize-avoid-c-arrays)
    static const auto randomData = std::make_unique<std::uint8_t[]>(randomDataSize);
    std::generate(randomData.get(), randomData.get() + randomDataSize, [&] { return static_cast<std::uint8_t>(gen()); });

    // NOTE:
    // In this test, we purposely perform only a single receive, send or accept operation
    // per readiness notification. Usually when waiting for readiness of non-blocking
    // sockets, the receive buffer, send buffer or connection backlog should be fully
    // drained before waiting for readiness again in order to improve performance.
    // Here, it is more important for us to test the level-triggering behaviour of
    // sf::SocketSelector. This means that as long as buffers are not completely drained
    // the wait() should keep returning immediately until they are empty. This is the
    // opposite of edge-triggering, in which we would only get a single readiness notification
    // and MUST drain the ready buffers in order to be able to be notified of new edges.
    // Edge-triggering is therefore much harder to use correctly and can result in deadlocks.
    // Currently sf::SocketSelector does not support edge-triggered notifications.

    sf::SocketSelector socketSelector;
    sf::TcpListener    listener;

    REQUIRE(listener.listen(sf::Socket::AnyPort) == sf::Socket::Status::Done);
    const auto port = listener.getLocalPort();
    REQUIRE_FALSE(port == 0);

    struct ConnectionState
    {
        sf::TcpSocket socket;
        // NOLINTNEXTLINE(cppcoreguidelines-avoid-c-arrays,modernize-avoid-c-arrays)
        std::unique_ptr<std::uint8_t[]> data        = std::make_unique<std::uint8_t[]>(randomDataSize);
        std::size_t                     transferred = 0;
    };

    auto connectionBacklogCount = 0;

    // Set up incoming connections
    std::list<ConnectionState> incomingConnections;
    std::size_t                connectionIncomingDoneCount = 0;

    const auto listenerCallback = [&](sf::SocketSelector::ReadinessType listenerReadinessType)
    {
        REQUIRE(listenerReadinessType == sf::SocketSelector::ReadinessType::Receive);
        auto& state = incomingConnections.emplace_back();
        REQUIRE(listener.accept(state.socket) == sf::Socket::Status::Done);
        state.socket.setBlocking(false);
        REQUIRE(connectionBacklogCount > 0);
        --connectionBacklogCount;

        const auto incomingCallback = [&](sf::SocketSelector::ReadinessType readinessType)
        {
            REQUIRE(readinessType == sf::SocketSelector::ReadinessType::Receive);
            std::size_t received = 0;
            const auto  result   = state.socket.receive(state.data.get() + state.transferred,
                                                     randomDataSize - state.transferred,
                                                     received);
            state.transferred += received;
            REQUIRE(state.transferred <= randomDataSize);
            if (result == sf::Socket::Status::NotReady)
                return;
            if (result == sf::Socket::Status::Disconnected)
            {
                state.socket.disconnect();
                ++connectionIncomingDoneCount;
                REQUIRE(socketSelector.remove(state.socket) == true);
                return;
            }
            REQUIRE(result == sf::Socket::Status::Done);
        };

        REQUIRE(socketSelector.add(state.socket, sf::SocketSelector::ReadinessType::Receive, incomingCallback) == true);
    };

    REQUIRE(socketSelector.add(listener, sf::SocketSelector::ReadinessType::Receive, listenerCallback) == true);

    // Set up outgoing connections
    std::list<ConnectionState> outgoingConnections(connectionCount);
    std::size_t                connectionOutgoingDoneCount = 0;

    std::list<ConnectionState*> pendingOutgoingConnections;
    for (auto& outgoingConnection : outgoingConnections)
        pendingOutgoingConnections.emplace_back(&outgoingConnection);

    // Connect outgoing connections
    while (incomingConnections.size() < connectionCount)
    {
        while (!pendingOutgoingConnections.empty() && (connectionBacklogCount < connectionBacklogMax))
        {
            auto& state = *pendingOutgoingConnections.front();
            pendingOutgoingConnections.pop_front();
            ++connectionBacklogCount;

            state.socket.setBlocking(false);
            const auto result = state.socket.connect(sf::IpAddress::LocalHost, port);
            REQUIRE_FALSE(result == sf::Socket::Status::Disconnected);
            REQUIRE_FALSE(result == sf::Socket::Status::Error);
            REQUIRE_FALSE(result == sf::Socket::Status::Partial);

            const auto outgoingConnectedCallback = [&](sf::SocketSelector::ReadinessType readinessType)
            {
                REQUIRE(readinessType == sf::SocketSelector::ReadinessType::Send);
                REQUIRE(state.socket.getRemoteAddress().has_value() == true);
                REQUIRE(socketSelector.remove(state.socket) == true);
            };

            REQUIRE(socketSelector.add(state.socket, sf::SocketSelector::ReadinessType::Send, outgoingConnectedCallback) ==
                    true);
        }

        if (!socketSelector.wait(sf::milliseconds(5000)))
            break;
        socketSelector.dispatchReadyCallbacks();
    }

    REQUIRE(incomingConnections.size() == connectionCount);

    // At this point all connections are established

    // Transfer data
    for (auto& state : outgoingConnections)
    {
        const auto outgoingDataCallback = [&](sf::SocketSelector::ReadinessType readinessType)
        {
            REQUIRE(readinessType == sf::SocketSelector::ReadinessType::Send);
            std::size_t sent = 0;
            const auto result = state.socket.send(randomData.get() + state.transferred, randomDataSize - state.transferred, sent);
            state.transferred += sent;
            REQUIRE_FALSE(result == sf::Socket::Status::Disconnected);
            REQUIRE_FALSE(result == sf::Socket::Status::Error);
            if (state.transferred == randomDataSize)
            {
                state.socket.disconnect();
                ++connectionOutgoingDoneCount;
                REQUIRE(socketSelector.remove(state.socket) == true);
            }
        };

        REQUIRE(socketSelector.add(state.socket, sf::SocketSelector::ReadinessType::Send, outgoingDataCallback) == true);
    }

    // Wait until all data transfers are complete
    while ((connectionOutgoingDoneCount < connectionCount) || (connectionIncomingDoneCount < connectionCount))
    {
        if (!socketSelector.wait(sf::milliseconds(5000)))
            break;
        socketSelector.dispatchReadyCallbacks();
    }

    REQUIRE(connectionOutgoingDoneCount == connectionCount);
    REQUIRE(connectionIncomingDoneCount == connectionCount);

    // Verify all data was transferred correctly
    for (const auto& state : incomingConnections)
        CHECK(std::memcmp(state.data.get(), randomData.get(), randomDataSize) == 0);
}
