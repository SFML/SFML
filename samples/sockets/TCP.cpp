
////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Network.hpp>
#include <iostream>


////////////////////////////////////////////////////////////
/// Create a client and connect it to a running server
///
////////////////////////////////////////////////////////////
void DoClientTCP(unsigned short Port)
{
    // Ask for server address
    sf::IPAddress ServerAddress;
    do
    {
        std::cout << "Type address or name of the server to connect to : ";
        std::cin  >> ServerAddress;
    }
    while (!ServerAddress.IsValid());

    // Create a TCP socket for communicating with server
    sf::SocketTCP Client;

    // Connect to the specified server
    if (Client.Connect(Port, ServerAddress) != sf::Socket::Done)
        return;
    std::cout << "Connected to server " << ServerAddress << std::endl;

    // Receive a message from the client
    char Message[128];
    std::size_t Received;
    if (Client.Receive(Message, sizeof(Message), Received) != sf::Socket::Done)
        return;

    // Show it
    std::cout << "Message received from server : \"" << Message << "\"" << std::endl;

    // Define a message to send back to the server
    char ToSend[] = "Hi, I'm a client !";

    // Send the message
    if (Client.Send(ToSend, sizeof(ToSend)) != sf::Socket::Done)
        return;
    std::cout << "Message sent to server : \"" << ToSend << "\"" << std::endl;

    // Close the socket when we're done
    Client.Close();
}


////////////////////////////////////////////////////////////
/// Launch a server and wait for incoming connections
///
////////////////////////////////////////////////////////////
void DoServerTCP(unsigned short Port)
{
    // Create a TCP socket for communicating with clients
    sf::SocketTCP Server;

    // Listen to a port for incoming connections
    if (!Server.Listen(Port))
        return;
    std::cout << "Server is listening to port " << Port << ", waiting for connections... " << std::endl;

    // Wait for a connection
    sf::IPAddress ClientAddress;
    sf::SocketTCP Client;
    if (Server.Accept(Client, &ClientAddress) != sf::Socket::Done)
        return;
    std::cout << "Client connected : " << ClientAddress << std::endl;

    // Send a message to the client
    char ToSend[] = "Hi, I'm the server";
    if (Client.Send(ToSend, sizeof(ToSend)) != sf::Socket::Done)
        return;
    std::cout << "Message sent to the client : \"" << ToSend << "\"" << std::endl;

    // Receive a message back from the client
    char Message[128];
    std::size_t Received;
    if (Client.Receive(Message, sizeof(Message), Received) != sf::Socket::Done)
        return;

    // Show the message
    std::cout << "Message received from the client : \"" << Message << "\"" << std::endl;

    // Close the sockets when we're done
    Client.Close();
    Server.Close();
}
