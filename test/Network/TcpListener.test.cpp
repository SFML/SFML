#include <SFML/Network/TcpListener.hpp>

// Other 1st party headers
#include <SFML/Network/TcpSocket.hpp>

#include <catch2/catch_test_macros.hpp>

#include <type_traits>

TEST_CASE("[Network] sf::TcpListener")
{
    SECTION("Type traits")
    {
        STATIC_CHECK(!std::is_copy_constructible_v<sf::TcpListener>);
        STATIC_CHECK(!std::is_copy_assignable_v<sf::TcpListener>);
        STATIC_CHECK(std::is_nothrow_move_constructible_v<sf::TcpListener>);
        STATIC_CHECK(std::is_nothrow_move_assignable_v<sf::TcpListener>);
    }

    SECTION("Construction")
    {
        const sf::TcpListener tcpListener;
        CHECK(tcpListener.getLocalPort() == 0);
    }

    SECTION("listen()")
    {
        sf::TcpListener tcpListener;

        SECTION("Valid")
        {
            CHECK(tcpListener.listen(0) == sf::Socket::Status::Done);
            CHECK(tcpListener.getLocalPort() != 0);
        }

        SECTION("Invalid")
        {
            CHECK(tcpListener.listen(0, sf::IpAddress::Broadcast) == sf::Socket::Status::Error);
            CHECK(tcpListener.getLocalPort() == 0);
        }
    }

    SECTION("close()")
    {
        sf::TcpListener tcpListener;
        CHECK(tcpListener.listen(0) == sf::Socket::Status::Done);
        CHECK(tcpListener.getLocalPort() != 0);
        tcpListener.close();
        CHECK(tcpListener.getLocalPort() == 0);
    }

    SECTION("accept()")
    {
        sf::TcpListener tcpListener;
        sf::TcpSocket   tcpSocket;
        CHECK(tcpListener.accept(tcpSocket) == sf::Socket::Status::Error);
    }
}
