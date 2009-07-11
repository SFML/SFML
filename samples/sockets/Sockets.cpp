
////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <iostream>
#include <cstdlib>


////////////////////////////////////////////////////////////
// Function prototypes
// (I'm too lazy to put them into separate headers...)
////////////////////////////////////////////////////////////
void DoClientTCP(unsigned short Port);
void DoClientUDP(unsigned short Port);
void DoServerTCP(unsigned short Port);
void DoServerUDP(unsigned short Port);


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

    // TCP or UDP ?
    char protocol;
    std::cout << "Do you want to use TCP ('t') or UDP ('u') ? ";
    std::cin  >> protocol;

    // Client or server ?
    char who;
    std::cout << "Do you want to be a server ('s') or a client ('c') ? ";
    std::cin  >> who;

    if (who == 's')
    {
        // Run as a server
        if (protocol == 't')
            DoServerTCP(port);
        else
            DoServerUDP(port);
    }
    else
    {
        // Run as a client
        if (protocol == 't')
            DoClientTCP(port);
        else
            DoClientUDP(port);
    }

    // Wait until the user presses 'enter' key
    std::cout << "Press enter to exit..." << std::endl;
    std::cin.ignore(10000, '\n');
    std::cin.ignore(10000, '\n');

    return EXIT_SUCCESS;
}
