#include <SFML/Network/TcpSocket.hpp>

// Other 1st party headers
#include <SFML/Network/IpAddress.hpp>

#include <catch2/catch_test_macros.hpp>

#include <NetworkUtil.hpp>
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
        CHECK_FALSE(tcpSocket.getRemoteAddress().has_value());
        CHECK(tcpSocket.getRemotePort() == 0);
        CHECK_FALSE(tcpSocket.getCurrentCiphersuiteName().has_value());
    }
}

TEST_CASE("[Network] sf::TcpSocket Connection", runConnectionTests())
{
    SECTION("Connection")
    {
        const auto githubAddress = sf::IpAddress::resolve("github.com");
        REQUIRE(githubAddress.has_value());

        sf::TcpSocket tcpSocket;
        CHECK(tcpSocket.setupTlsServer("", "") == sf::TcpSocket::TlsStatus::NotConnected);
        CHECK(tcpSocket.setupTlsClient("") == sf::TcpSocket::TlsStatus::NotConnected);

        SECTION("Non-TLS")
        {
            CHECK(tcpSocket.connect(*githubAddress, 80, sf::milliseconds(1000)) == sf::TcpSocket::Status::Done);
            CHECK_FALSE(tcpSocket.getCurrentCiphersuiteName().has_value());
        }

        SECTION("TLS")
        {
            CHECK(tcpSocket.connect(*githubAddress, 443, sf::milliseconds(1000)) == sf::TcpSocket::Status::Done);
            CHECK(tcpSocket.setupTlsClient("github.com") == sf::TcpSocket::TlsStatus::HandshakeComplete);

            SECTION("Ciphersuite")
            {
                REQUIRE(tcpSocket.getCurrentCiphersuiteName().has_value());
                CHECK_FALSE(tcpSocket.getCurrentCiphersuiteName()->empty());
            }
        }
    }
}
