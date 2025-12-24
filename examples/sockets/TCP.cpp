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
        {
            std::cout << "TLS handshake could not be completed" << std::endl;
            return;
        }
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
