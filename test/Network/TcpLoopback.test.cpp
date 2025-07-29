#include <SFML/Network/SocketSelector.hpp>
#include <SFML/Network/TcpListener.hpp>
#include <SFML/Network/TcpSocket.hpp>

#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators_all.hpp>

#include <NetworkUtil.hpp>
#include <algorithm>
#include <chrono>
#include <random>
#include <vector>

#include <cstddef>

namespace
{
constexpr auto* commonName  = "SFML TLS Example"; // Part of certificate
constexpr auto* certificate = R"(-----BEGIN CERTIFICATE-----
MIIByzCCAVCgAwIBAgIUMsx0eZyglgkQ2acW53E6f9iIZVAwCgYIKoZIzj0EAwMw
GzEZMBcGA1UEAwwQU0ZNTCBUTFMgRXhhbXBsZTAgFw0yNTA3MjEwODEzMzBaGA8y
MTI1MDYyNzA4MTMzMFowGzEZMBcGA1UEAwwQU0ZNTCBUTFMgRXhhbXBsZTB2MBAG
ByqGSM49AgEGBSuBBAAiA2IABPJtsphFKn6RJXiDb3CvmNxB7CP1HtiC4u635UH5
iEXMeAAn6euyR4sEf9qsJsLcsWSW8GJtebUYAsa7k8c30Kad+wslhC6bL+6eF3E1
nSMnJ7FB4UwaLdkuST/UAeZYQaNTMFEwHQYDVR0OBBYEFCyZH8QqmYP96LUcSt/R
vIEi56L1MB8GA1UdIwQYMBaAFCyZH8QqmYP96LUcSt/RvIEi56L1MA8GA1UdEwEB
/wQFMAMBAf8wCgYIKoZIzj0EAwMDaQAwZgIxAOTxRz+pFAqqj3W1YXTVfgcJHlxf
tBoDnmHl3LfCO4Xfa7xQJwZEbiY89WAQd9/I4wIxAIxEIBZdkHXT9pkfUOAqXt6l
qaE9o2kRuRMav9cgNN9tbmHN+p0QQenlXm5eSV1ISQ==
-----END CERTIFICATE-----)";
constexpr auto* privateKey  = R"(-----BEGIN EC PARAMETERS-----
BgUrgQQAIg==
-----END EC PARAMETERS-----
-----BEGIN EC PRIVATE KEY-----
MIGkAgEBBDBdiTGuOa0X+OI42nRJa1+tOCW6WS6LfBAtMz5DP7Z+pcK16K6ueMJs
MQxucn12NkGgBwYFK4EEACKhZANiAATybbKYRSp+kSV4g29wr5jcQewj9R7YguLu
t+VB+YhFzHgAJ+nrskeLBH/arCbC3LFklvBibXm1GALGu5PHN9CmnfsLJYQumy/u
nhdxNZ0jJyexQeFMGi3ZLkk/1AHmWEE=
-----END EC PRIVATE KEY-----)";

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
