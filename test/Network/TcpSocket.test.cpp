#include <SFML/Network/TcpSocket.hpp>

// Other 1st party headers
#include <SFML/Network/IpAddress.hpp>

#include <catch2/catch_test_macros.hpp>

#include <type_traits>

TEST_CASE("[Network] sf::TcpSocket")
{
    SECTION("Type traits")
    {
        STATIC_CHECK(!std::is_copy_constructible_v<sf::TcpSocket>);
        STATIC_CHECK(!std::is_copy_assignable_v<sf::TcpSocket>);
        STATIC_CHECK(std::is_nothrow_move_constructible_v<sf::TcpSocket>);
        STATIC_CHECK(std::is_nothrow_move_assignable_v<sf::TcpSocket>);
    }

    SECTION("Construction")
    {
        const sf::TcpSocket tcpSocket;
        CHECK(tcpSocket.getLocalPort() == 0);
        CHECK(!tcpSocket.getRemoteAddress().has_value());
        CHECK(tcpSocket.getRemotePort() == 0);
    }
}
