#include <SFML/Network/SocketSelector.hpp>
#include <SFML/Network/TcpListener.hpp>
#include <SFML/Network/TcpSocket.hpp>

#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators_all.hpp>

#include <NetworkUtil.hpp>
#include <algorithm>
#include <chrono>
#include <random>
#include <string_view>
#include <vector>

#include <cstddef>

namespace
{
constexpr std::string_view commonName = "SFML TLS Example"; // Part of certificate

// Generated with: openssl ecparam -out key.pem -name secp384r1 -genkey
constexpr std::string_view privateKey = R"(-----BEGIN EC PARAMETERS-----
BgUrgQQAIg==
-----END EC PARAMETERS-----
-----BEGIN EC PRIVATE KEY-----
MIGkAgEBBDD4ybj384qP07KjAqKoU4XwVyRH/Sy2kMYrtCr0SU/R0sCSxSPNwAXW
8Z5Z5CV/uDugBwYFK4EEACKhZANiAATpYJyfUQMS5YUOS8yaf2svRqebVAyqZcNh
lUKEP2OPwacLVul1OlW9QYJh4VFvtGvKyNEd1YCpBFUyASfeev8ZNBaLpMb20e3S
ztTSrGCF1rJynnEoGJrIh3trvRAKD0E=
-----END EC PRIVATE KEY-----)";

// Generated with: openssl req -new -key key.pem -x509 -nodes -days 36500 -sha384 -out cert.pem
// Country Name (2 letter code) [AU]:.
// State or Province Name (full name) [Some-State]:.
// Locality Name (eg, city) []:.
// Organization Name (eg, company) [Internet Widgits Pty Ltd]:.
// Organizational Unit Name (eg, section) []:.
// Common Name (e.g. server FQDN or YOUR name) []:SFML TLS Example
// Email Address []:.
constexpr std::string_view certificate = R"(-----BEGIN CERTIFICATE-----
MIIByTCCAVCgAwIBAgIUKT3iSj7kJlvzxEGvfK1/yAYzRPcwCgYIKoZIzj0EAwMw
GzEZMBcGA1UEAwwQU0ZNTCBUTFMgRXhhbXBsZTAgFw0yNTA4MjMxMjI4MDJaGA8y
MTI1MDczMDEyMjgwMlowGzEZMBcGA1UEAwwQU0ZNTCBUTFMgRXhhbXBsZTB2MBAG
ByqGSM49AgEGBSuBBAAiA2IABOlgnJ9RAxLlhQ5LzJp/ay9Gp5tUDKplw2GVQoQ/
Y4/BpwtW6XU6Vb1BgmHhUW+0a8rI0R3VgKkEVTIBJ956/xk0FoukxvbR7dLO1NKs
YIXWsnKecSgYmsiHe2u9EAoPQaNTMFEwHQYDVR0OBBYEFImks65Kr96a8vcyjMgT
WJHWyPpQMB8GA1UdIwQYMBaAFImks65Kr96a8vcyjMgTWJHWyPpQMA8GA1UdEwEB
/wQFMAMBAf8wCgYIKoZIzj0EAwMDZwAwZAIwEwYg6elb6OPPkWaYdMAk4u05pngs
jN4bnBb1ChFgSeEnQmfOA1yOeAd0zUMiK5pGAjAdauGlH5+eE6939umuH6p3VMNc
RZlDlROT9eBnJ76WeMDiPMz+7E/oUdvGCAhuZb0=
-----END CERTIFICATE-----)";

std::random_device            rd;
std::mt19937                  rng(rd());
std::uniform_int_distribution dist;
} // namespace

TEST_CASE("[Network] sf::Tcp Loopback", runLoopbackTests())
{
    std::vector<std::byte> testData(1024 * 1024);
    std::generate(testData.begin(), testData.end(), [] { return static_cast<std::byte>(dist(rng)); });
    const auto*            sendEnd = testData.data() + testData.size();
    std::vector<std::byte> buffer(testData.size());
    const auto*            recvEnd = buffer.data() + buffer.size();

    sf::TcpListener tcpListener;
    tcpListener.setBlocking(false);
    REQUIRE(tcpListener.listen(sf::Socket::AnyPort) == sf::TcpListener::Status::Done);

    const auto localPort = tcpListener.getLocalPort();
    CHECK_FALSE(localPort == 0);

    SECTION("Non-TLS")
    {
        sf::TcpSocket serverSocket;
        sf::TcpSocket clientSocket;
        clientSocket.setBlocking(false);
        CHECK(clientSocket.connect(sf::IpAddress(127, 0, 0, 1), localPort, sf::milliseconds(10000)) ==
              sf::TcpSocket::Status::NotReady);

        auto start = std::chrono::steady_clock::now();

        while (true)
        {
            const auto result = tcpListener.accept(serverSocket);

            REQUIRE(((result == sf::TcpListener::Status::NotReady) || (result == sf::TcpListener::Status::Done)));

            if (result == sf::TcpListener::Status::Done)
                break;

            REQUIRE(std::chrono::steady_clock::now() - start < std::chrono::milliseconds(10000));
        }

        serverSocket.setBlocking(false);
        CHECK_FALSE(clientSocket.getRemoteAddress() == std::nullopt);
        CHECK_FALSE(serverSocket.getRemoteAddress() == std::nullopt);
        CHECK_FALSE(clientSocket.getLocalPort() == 0);
        CHECK_FALSE(serverSocket.getLocalPort() == 0);
        CHECK_FALSE(clientSocket.getRemotePort() == 0);
        CHECK_FALSE(serverSocket.getRemotePort() == 0);

        CHECK(serverSocket.getCurrentCiphersuiteName() == std::nullopt);
        CHECK(clientSocket.getCurrentCiphersuiteName() == std::nullopt);

        const auto* sendPtr = testData.data();
        auto*       recvPtr = buffer.data();

        start = std::chrono::steady_clock::now();

        while (true)
        {
            if (sendPtr != sendEnd)
            {
                std::size_t sent{};
                const auto  status = serverSocket.send(sendPtr, static_cast<std::size_t>(sendEnd - sendPtr), sent);
                REQUIRE_FALSE(status == sf::TcpSocket::Status::Error);
                REQUIRE_FALSE(status == sf::TcpSocket::Status::Disconnected);
                sendPtr += sent;
            }
            else if (serverSocket.getRemoteAddress())
            {
                serverSocket.disconnect();
            }

            {
                std::size_t received{};
                const auto status = clientSocket.receive(recvPtr, static_cast<std::size_t>(recvEnd - recvPtr), received);
                REQUIRE_FALSE(status == sf::TcpSocket::Status::Error);
                if (received > 0)
                    REQUIRE_FALSE(status == sf::TcpSocket::Status::Disconnected);
                recvPtr += received;

                if (status == sf::TcpSocket::Status::Disconnected)
                {
                    clientSocket.disconnect();
                    break;
                }
            }

            REQUIRE(std::chrono::steady_clock::now() - start < std::chrono::milliseconds(10000));
        }

        CHECK(std::equal(buffer.begin(), buffer.end(), testData.begin()));
    }

    SECTION("TLS")
    {
        sf::TcpSocket serverSocket;
        sf::TcpSocket clientSocket;
        clientSocket.setBlocking(false);
        REQUIRE(clientSocket.connect(sf::IpAddress(127, 0, 0, 1), localPort, sf::milliseconds(10000)) ==
                sf::TcpSocket::Status::NotReady);

        auto start = std::chrono::steady_clock::now();

        while (true)
        {
            const auto result = tcpListener.accept(serverSocket);

            REQUIRE(((result == sf::TcpListener::Status::NotReady) || (result == sf::TcpListener::Status::Done)));

            if (result == sf::TcpListener::Status::Done)
                break;

            REQUIRE(std::chrono::steady_clock::now() - start < std::chrono::milliseconds(10000));
        }

        serverSocket.setBlocking(false);
        CHECK_FALSE(clientSocket.getRemoteAddress() == std::nullopt);
        CHECK_FALSE(serverSocket.getRemoteAddress() == std::nullopt);
        CHECK_FALSE(clientSocket.getLocalPort() == 0);
        CHECK_FALSE(serverSocket.getLocalPort() == 0);
        CHECK_FALSE(clientSocket.getRemotePort() == 0);
        CHECK_FALSE(serverSocket.getRemotePort() == 0);

        const auto* sendPtr = testData.data();
        auto*       recvPtr = buffer.data();

        start = std::chrono::steady_clock::now();

        while (true)
        {
            const auto serverStatus = serverSocket.setupTlsServer(certificate, privateKey);

            REQUIRE_FALSE(serverStatus == sf::TcpSocket::TlsStatus::Error);
            REQUIRE_FALSE(serverStatus == sf::TcpSocket::TlsStatus::NotConnected);

            const auto clientStatus = clientSocket.setupTlsClient(commonName, certificate);

            REQUIRE_FALSE(clientStatus == sf::TcpSocket::TlsStatus::Error);
            REQUIRE_FALSE(clientStatus == sf::TcpSocket::TlsStatus::NotConnected);

            if ((serverStatus == sf::TcpSocket::TlsStatus::HandshakeComplete) &&
                (clientStatus == sf::TcpSocket::TlsStatus::HandshakeComplete))
                break;

            REQUIRE(std::chrono::steady_clock::now() - start < std::chrono::milliseconds(10000));
        }

        CHECK_FALSE(serverSocket.getCurrentCiphersuiteName() == std::nullopt);
        CHECK_FALSE(clientSocket.getCurrentCiphersuiteName() == std::nullopt);
        CHECK(serverSocket.getCurrentCiphersuiteName() == clientSocket.getCurrentCiphersuiteName());

        start = std::chrono::steady_clock::now();

        while (true)
        {
            if (sendPtr != sendEnd)
            {
                std::size_t sent{};
                const auto  status = serverSocket.send(sendPtr, static_cast<std::size_t>(sendEnd - sendPtr), sent);
                REQUIRE_FALSE(status == sf::TcpSocket::Status::Error);
                REQUIRE_FALSE(status == sf::TcpSocket::Status::Disconnected);
                sendPtr += sent;
            }
            else if (serverSocket.getRemoteAddress())
            {
                serverSocket.disconnect();
            }

            {
                std::size_t received{};
                const auto status = clientSocket.receive(recvPtr, static_cast<std::size_t>(recvEnd - recvPtr), received);
                REQUIRE_FALSE(status == sf::TcpSocket::Status::Error);
                if (received > 0)
                    REQUIRE_FALSE(status == sf::TcpSocket::Status::Disconnected);
                recvPtr += received;

                if (status == sf::TcpSocket::Status::Disconnected)
                {
                    clientSocket.disconnect();
                    break;
                }
            }

            REQUIRE(std::chrono::steady_clock::now() - start < std::chrono::milliseconds(10000));
        }

        CHECK(std::equal(buffer.begin(), buffer.end(), testData.begin()));
    }
}
