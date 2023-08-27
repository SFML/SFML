#include <SFML/Network/SocketSelector.hpp>

// Other 1st party headers
#include <SFML/Network/UdpSocket.hpp>

#include <catch2/catch_test_macros.hpp>

#include <type_traits>

TEST_CASE("[Network] sf::SocketSelector")
{
    SECTION("Type traits")
    {
        STATIC_CHECK(std::is_copy_constructible_v<sf::SocketSelector>);
        STATIC_CHECK(std::is_copy_assignable_v<sf::SocketSelector>);
        STATIC_CHECK(std::is_nothrow_move_constructible_v<sf::SocketSelector>);
        STATIC_CHECK(std::is_nothrow_move_assignable_v<sf::SocketSelector>);
    }

    sf::UdpSocket socket;

    SECTION("Construction")
    {
        const sf::SocketSelector socketSelector;
        CHECK(!socketSelector.isReady(socket));
    }
}
