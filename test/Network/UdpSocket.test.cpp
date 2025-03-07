#include <SFML/Network/UdpSocket.hpp>

#include <catch2/catch_test_macros.hpp>

#include <type_traits>

TEST_CASE("[Network] sf::UdpSocket")
{
    SECTION("Type traits")
    {
        STATIC_CHECK(!std::is_copy_constructible_v<sf::UdpSocket>);
        STATIC_CHECK(!std::is_copy_assignable_v<sf::UdpSocket>);
        STATIC_CHECK(std::is_nothrow_move_constructible_v<sf::UdpSocket>);
        STATIC_CHECK(std::is_nothrow_move_assignable_v<sf::UdpSocket>);
    }

    SECTION("Constants")
    {
        STATIC_CHECK(sf::UdpSocket::MaxDatagramSize == 65507);
    }

    SECTION("Construction")
    {
        const sf::UdpSocket udpSocket;
        CHECK(udpSocket.getLocalPort() == 0);
    }

    SECTION("bind()/unbind()")
    {
        sf::UdpSocket udpSocket;
        CHECK(udpSocket.bind(sf::Socket::AnyPort, sf::IpAddress::Broadcast) == sf::Socket::Status::Error);
        CHECK(udpSocket.bind(sf::Socket::AnyPort) == sf::Socket::Status::Done);
        CHECK(udpSocket.getLocalPort() != 0);

        udpSocket.unbind();
        CHECK(udpSocket.getLocalPort() == 0);
    }
}
