////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include "Client.hpp"
#include "Server.hpp"

#include <iostream>


////////////////////////////////////////////////////////////
/// Entry point of application
///
/// \return Application exit code
///
////////////////////////////////////////////////////////////
int main()
{
    // Choose a random port for opening sockets (ports < 1024 are reserved)
    const unsigned short port = 2435;

    // Client or server ?
    char who = 0;
    std::cout << "Do you want to be a server ('s') or a client ('c')? ";
    std::cin >> who;

    if (who == 's')
    {
        // Run as a server
        doServer(port);
    }
    else
    {
        // Run as a client
        doClient(port);
    }

    // Wait until the user presses 'enter' key
    std::cout << "Press enter to exit..." << std::endl;
    std::cin.ignore(10'000, '\n');
}
