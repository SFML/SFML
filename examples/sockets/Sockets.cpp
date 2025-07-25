////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include "TCP.hpp"
#include "UDP.hpp"

#include <iostream>


////////////////////////////////////////////////////////////
/// Entry point of application
///
/// \return Application exit code
///
////////////////////////////////////////////////////////////
int main()
{
    // Choose an arbitrary port for opening sockets
    const unsigned short port = 50001;

    // TCP, UDP or connected UDP ?
    char protocol = 0;
    std::cout << "Do you want to use TCP (t) or UDP (u)? ";
    std::cin >> protocol;

    // Client or server ?
    char who = 0;
    std::cout << "Do you want to be a server (s) or a client (c)? ";
    std::cin >> who;

    if (protocol == 't')
    {
        // Enable TLS ?
        char tls = 0;
        std::cout << "Do you want to enable TLS (y) or not (n)? ";
        std::cin >> tls;

        // Test the TCP protocol
        if (who == 's')
            runTcpServer(port, tls == 'y');
        else
            runTcpClient(port, tls == 'y');
    }
    else
    {
        // Test the unconnected UDP protocol
        if (who == 's')
            runUdpServer(port);
        else
            runUdpClient(port);
    }

    // Wait until the user presses 'enter' key
    std::cout << "Press enter to exit..." << std::endl;
    std::cin.ignore(10'000, '\n');
    std::cin.ignore(10'000, '\n');
}
