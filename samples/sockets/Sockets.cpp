
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
    const unsigned short Port = 2435;

    // TCP or UDP ?
    char Protocol;
    std::cout << "Do you want to use TCP ('t') or UDP ('u') ? ";
    std::cin  >> Protocol;

    // Client or server ?
    char Who;
    std::cout << "Do you want to be a server ('s') or a client ('c') ? ";
    std::cin  >> Who;

    if (Who == 's')
    {
        // Run as a server
        if (Protocol == 't')
            DoServerTCP(Port);
        else
            DoServerUDP(Port);
    }
    else
    {
        // Run as a client
        if (Protocol == 't')
            DoClientTCP(Port);
        else
            DoClientUDP(Port);
    }

    // Wait until the user presses 'enter' key
    std::cout << "Press enter to exit..." << std::endl;
    std::cin.ignore(10000, '\n');
    std::cin.ignore(10000, '\n');

    return EXIT_SUCCESS;
}
