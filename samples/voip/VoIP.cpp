
////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <iomanip>
#include <iostream>
#include <cstdlib>


////////////////////////////////////////////////////////////
// Function prototypes
// (I'm too lazy to put them into separate headers...)
////////////////////////////////////////////////////////////
void DoClient(unsigned short Port);
void DoServer(unsigned short Port);


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

    // Client or server ?
    char Who;
    std::cout << "Do you want to be a server ('s') or a client ('c') ? ";
    std::cin  >> Who;

    if (Who == 's')
    {
        // Run as a server
        DoServer(Port);
    }
    else
    {
        // Run as a client
        DoClient(Port);
    }

    // Wait until the user presses 'enter' key
    std::cout << "Press enter to exit..." << std::endl;
    std::cin.ignore(10000, '\n');

    return EXIT_SUCCESS;
}
