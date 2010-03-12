
////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Network.hpp>
#include <iostream>


////////////////////////////////////////////////////////////
/// Create a client and send a message to a running server
///
////////////////////////////////////////////////////////////
void DoClientUDP(unsigned short port)
{
    // Ask for server address
    sf::IpAddress serverAddress;
    do
    {
        std::cout << "Type address or name of the server to send the message to : ";
        std::cin  >> serverAddress;
    }
    while (!serverAddress.IsValid());

    // Create a UDP socket for communicating with server
    sf::SocketUDP client;

    // Send a message to the server
    char message[] = "Hi, I'm a client !";
    if (client.Send(message, sizeof(message), serverAddress, port) != sf::Socket::Done)
        return;
    std::cout << "Message sent to server : \"" << message << "\"" << std::endl;

    // Close the socket when we're done
    client.Close();
}


////////////////////////////////////////////////////////////
/// Launch a server and wait for incoming messages
///
////////////////////////////////////////////////////////////
void DoServerUDP(unsigned short port)
{
    // Create a UDP socket for communicating with clients
    sf::SocketUDP server;

    // Bind it to the specified port
    if (!server.Bind(port))
        return;

    // Receive a message from anyone
    sf::IpAddress clientAddress;
    unsigned short clientPort;
    char message[128];
    std::size_t received;
    if (server.Receive(message, sizeof(message), received, clientAddress, clientPort) != sf::Socket::Done)
        return;

    // Display it
    std::cout << "Message received from " << clientAddress << " on port " << clientPort
              << ": \"" << message << "\"" << std::endl;

    // Close the socket when we're done
    server.Close();
}
