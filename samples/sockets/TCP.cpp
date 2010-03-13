
////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Network.hpp>
#include <iostream>


////////////////////////////////////////////////////////////
/// Create a client and connect it to a running server
///
////////////////////////////////////////////////////////////
void DoClientTCP(unsigned short port)
{
    // Ask for server address
    sf::IpAddress serverAddress;
    do
    {
        std::cout << "Type address or name of the server to connect to : ";
        std::cin  >> serverAddress;
    }
    while (serverAddress == sf::IpAddress::None);

    // Create a TCP socket for communicating with server
    sf::SocketTCP client;

    // Connect to the specified server
    if (client.Connect(port, serverAddress) != sf::Socket::Done)
        return;
    std::cout << "Connected to server " << serverAddress << std::endl;

    // Receive a message from the client
    char message[128];
    std::size_t received;
    if (client.Receive(message, sizeof(message), received) != sf::Socket::Done)
        return;

    // Show it
    std::cout << "Message received from server : \"" << message << "\"" << std::endl;

    // Define a message to send back to the server
    char toSend[] = "Hi, I'm a client !";

    // Send the message
    if (client.Send(toSend, sizeof(toSend)) != sf::Socket::Done)
        return;
    std::cout << "Message sent to server : \"" << toSend << "\"" << std::endl;

    // Close the socket when we're done
    client.Close();
}


////////////////////////////////////////////////////////////
/// Launch a server and wait for incoming connections
///
////////////////////////////////////////////////////////////
void DoServerTCP(unsigned short port)
{
    // Create a TCP socket for communicating with clients
    sf::SocketTCP server;

    // Listen to a port for incoming connections
    if (!server.Listen(port))
        return;
    std::cout << "Server is listening to port " << port << ", waiting for connections... " << std::endl;

    // Wait for a connection
    sf::IpAddress clientAddress;
    sf::SocketTCP client;
    if (server.Accept(client, &clientAddress) != sf::Socket::Done)
        return;
    std::cout << "Client connected : " << clientAddress << std::endl;

    // Send a message to the client
    char toSend[] = "Hi, I'm the server";
    if (client.Send(toSend, sizeof(toSend)) != sf::Socket::Done)
        return;
    std::cout << "Message sent to the client : \"" << toSend << "\"" << std::endl;

    // Receive a message back from the client
    char message[128];
    std::size_t received;
    if (client.Receive(message, sizeof(message), received) != sf::Socket::Done)
        return;

    // Show the message
    std::cout << "Message received from the client : \"" << message << "\"" << std::endl;

    // Close the sockets when we're done
    client.Close();
    server.Close();
}
