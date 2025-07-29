////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include "TCP.hpp"

#include <SFML/Network.hpp>

#include <iomanip>
#include <iostream>
#include <optional>
#include <string_view>

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
} // namespace


////////////////////////////////////////////////////////////
/// Launch a server, wait for an incoming connection,
/// send a message and wait for the answer.
///
////////////////////////////////////////////////////////////
void runTcpServer(unsigned short port, bool tls)
{
    // Create a server socket to accept new connections
    sf::TcpListener listener;

    // Listen to the given port for incoming connections
    if (listener.listen(port) != sf::Socket::Status::Done)
        return;
    std::cout << "Server is listening to port " << port << ", waiting for connections... " << std::endl;

    // Wait for a connection
    sf::TcpSocket socket;
    if (listener.accept(socket) != sf::Socket::Status::Done)
        return;
    std::cout << "Client connected: " << socket.getRemoteAddress().value() << std::endl;

    if (tls)
    {
        // Setup TLS
        if (socket.setupTlsServer(certificate, privateKey) != sf::TcpSocket::TlsStatus::HandshakeComplete)
            return;
        std::cout << "TLS set up" << std::endl;

        if (auto ciphersuite = socket.getCurrentCiphersuiteName(); ciphersuite)
            std::cout << "Ciphersuite in use: " << *ciphersuite << std::endl;
    }

    // Send a message to the connected client
    static constexpr std::string_view out = "Hi, I'm the server";
    if (socket.send(out.data(), out.size()) != sf::Socket::Status::Done)
        return;
    std::cout << "Message sent to the client: " << std::quoted(out.data()) << std::endl;

    // Receive a message back from the client
    std::array<char, 128> in{};
    std::size_t           received = 0;
    if (socket.receive(in.data(), in.size(), received) != sf::Socket::Status::Done)
        return;
    std::cout << "Answer received from the client: " << std::quoted(in.data()) << std::endl;
}


////////////////////////////////////////////////////////////
/// Create a client, connect it to a server, display the
/// welcome message and send an answer.
///
////////////////////////////////////////////////////////////
void runTcpClient(unsigned short port, bool tls)
{
    // Ask for the server address
    std::optional<sf::IpAddress> server;
    do
    {
        std::cout << "Type the address or name of the server to connect to: ";
        std::cin >> server;
    } while (!server.has_value());

    // Create a socket for communicating with the server
    sf::TcpSocket socket;

    // Connect to the server
    if (socket.connect(server.value(), port) != sf::Socket::Status::Done)
        return;
    std::cout << "Connected to server " << server.value() << std::endl;

    if (tls)
    {
        // Setup TLS
        if (socket.setupTlsClient(commonName, certificate) != sf::TcpSocket::TlsStatus::HandshakeComplete)
            return;
        std::cout << "TLS set up" << std::endl;

        if (auto ciphersuite = socket.getCurrentCiphersuiteName(); ciphersuite)
            std::cout << "Ciphersuite in use: " << *ciphersuite << std::endl;
    }

    // Receive a message from the server
    std::array<char, 128> in{};
    std::size_t           received = 0;
    if (socket.receive(in.data(), in.size(), received) != sf::Socket::Status::Done)
        return;
    std::cout << "Message received from the server: " << std::quoted(in.data()) << std::endl;

    // Send an answer to the server
    static constexpr std::string_view out = "Hi, I'm a client";
    if (socket.send(out.data(), out.size()) != sf::Socket::Status::Done)
        return;
    std::cout << "Message sent to the server: " << std::quoted(out.data()) << std::endl;
}
