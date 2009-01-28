
////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Network.hpp>
#include <iostream>


////////////////////////////////////////////////////////////
/// Create a client and send a message to a running server
///
////////////////////////////////////////////////////////////
void DoClientUDP(unsigned short Port)
{
    // Ask for server address
    sf::IPAddress ServerAddress;
    do
    {
        std::cout << "Type address or name of the server to send the message to : ";
        std::cin  >> ServerAddress;
    }
    while (!ServerAddress.IsValid());

    // Create a UDP socket for communicating with server
    sf::SocketUDP Client;

    // Send a message to the server
    char Message[] = "Hi, I'm a client !";
    if (Client.Send(Message, sizeof(Message), ServerAddress, Port) != sf::Socket::Done)
        return;
    std::cout << "Message sent to server : \"" << Message << "\"" << std::endl;

    // Close the socket when we're done
    Client.Close();
}


////////////////////////////////////////////////////////////
/// Launch a server and wait for incoming messages
///
////////////////////////////////////////////////////////////
void DoServerUDP(unsigned short Port)
{
    // Create a UDP socket for communicating with clients
    sf::SocketUDP Server;

    // Bind it to the specified port
    if (!Server.Bind(Port))
        return;

    // Receive a message from anyone
    sf::IPAddress ClientAddress;
    unsigned short ClientPort;
    char Message[128];
    std::size_t Received;
    if (Server.Receive(Message, sizeof(Message), Received, ClientAddress, ClientPort) != sf::Socket::Done)
        return;

    // Display it
    std::cout << "Message received from " << ClientAddress << " on port " << ClientPort
              << ": \"" << Message << "\"" << std::endl;

    // Close the socket when we're done
    Server.Close();
}
