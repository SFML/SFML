
////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Network.hpp>
#include <fstream>
#include <iostream>


////////////////////////////////////////////////////////////
/// Print a FTP response into a standard output stream
///
////////////////////////////////////////////////////////////
std::ostream& operator <<(std::ostream& Stream, const sf::Ftp::Response& Response)
{
    return Stream << Response.GetStatus() << Response.GetMessage();
}


////////////////////////////////////////////////////////////
/// Entry point of application
///
/// \return Application exit code
///
////////////////////////////////////////////////////////////
int main()
{
    // Choose the server address
    sf::IPAddress Address;
    do
    {
        std::cout << "Enter the FTP server address : ";
        std::cin  >> Address;
    }
    while (!Address.IsValid());

    // Connect to the server
    sf::Ftp Server;
    sf::Ftp::Response ConnectResponse = Server.Connect(Address);
    std::cout << ConnectResponse << std::endl;
    if (!ConnectResponse.IsOk())
        return EXIT_FAILURE;

    // Ask for user name and password
    std::string UserName, Password;
    std::cout << "User name : ";
    std::cin  >> UserName;
    std::cout << "Password : ";
    std::cin  >> Password;

    // Login to the server
    sf::Ftp::Response LoginResponse = Server.Login(UserName, Password);
    std::cout << LoginResponse << std::endl;
    if (!LoginResponse.IsOk())
        return EXIT_FAILURE;

    // Main menu
    int Choice = 0;
    do
    {
        // Main FTP menu
        std::cout << std::endl;
        std::cout << "Choose an action:"                      << std::endl;
        std::cout << "1. Print working directory"             << std::endl;
        std::cout << "2. Print contents of working directory" << std::endl;
        std::cout << "3. Change directory"                    << std::endl;
        std::cout << "4. Create directory"                    << std::endl;
        std::cout << "5. Delete directory"                    << std::endl;
        std::cout << "6. Rename file"                         << std::endl;
        std::cout << "7. Remove file"                         << std::endl;
        std::cout << "8. Download file"                       << std::endl;
        std::cout << "9. Upload file"                         << std::endl;
        std::cout << "0. Disconnect"                          << std::endl;
        std::cout << std::endl;

        std::cout << "Your choice: ";
        std::cin  >> Choice;
        std::cout << std::endl;

        switch (Choice)
        {
            default :
            {
                // Wrong choice
                std::cout << "Invalid choice!" << std::endl;
                std::cin.clear();
                std::cin.ignore(10000, '\n');
                break;
            }

            case 1 :
            {
                // Print current server directory
                sf::Ftp::DirectoryResponse Response = Server.GetWorkingDirectory();
                std::cout << Response << std::endl;
                std::cout << "Current directory is " << Response.GetDirectory() << std::endl;
                break;
            }

            case 2 :
            {
                // Print content of current server directory
                sf::Ftp::ListingResponse Response = Server.GetDirectoryListing();
                std::cout << Response << std::endl;
                for (std::size_t i = 0; i < Response.GetCount(); ++i)
                    std::cout << Response.GetFilename(i) << std::endl;
                break;
            }

            case 3 :
            {
                // Change the current directory
                std::string Directory;
                std::cout << "Choose a directory: ";
                std::cin  >> Directory;
                std::cout << Server.ChangeDirectory(Directory) << std::endl;
                break;
            }

            case 4 :
            {
                // Create a new directory
                std::string Directory;
                std::cout << "Name of the directory to create: ";
                std::cin  >> Directory;
                std::cout << Server.MakeDirectory(Directory) << std::endl;
                break;
            }

            case 5 :
            {
                // Remove an existing directory
                std::string Directory;
                std::cout << "Name of the directory to remove: ";
                std::cin  >> Directory;
                std::cout << Server.DeleteDirectory(Directory) << std::endl;
                break;
            }

            case 6 :
            {
                // Rename a file
                std::string SrcFilename, DstFilename;
                std::cout << "Name of the file to rename: ";
                std::cin  >> SrcFilename;
                std::cout << "New name: ";
                std::cin  >> DstFilename;
                std::cout << Server.RenameFile(SrcFilename, DstFilename) << std::endl;
                break;
            }

            case 7 :
            {
                // Remove an existing directory
                std::string Filename;
                std::cout << "Name of the file to remove: ";
                std::cin  >> Filename;
                std::cout << Server.DeleteFile(Filename) << std::endl;
                break;
            }

            case 8 :
            {
                // Download a file from server
                std::string Filename, Directory;
                std::cout << "Path of the file to download (relative to current directory): ";
                std::cin  >> Filename;
                std::cout << "Directory to download the file to: ";
                std::cin  >> Directory;
                std::cout << Server.Download(Filename, Directory) << std::endl;
                break;
            }

            case 9 :
            {
                // Upload a file to server
                std::string Filename, Directory;
                std::cout << "Path of the file to upload (absolute or relative to working directory): ";
                std::cin  >> Filename;
                std::cout << "Directory to upload the file to (relative to current directory): ";
                std::cin  >> Directory;
                std::cout << Server.Upload(Filename, Directory) << std::endl;
                break;
            }

            case 0 :
            {
                // Disconnect
                break;
            }
        }

    } while (Choice != 0);

    // Disconnect from the server
    std::cout << "Disconnecting from server..." << std::endl;
    std::cout << Server.Disconnect() << std::endl;

    // Wait until the user presses 'enter' key
    std::cout << "Press enter to exit..." << std::endl;
    std::cin.ignore(10000, '\n');
    std::cin.ignore(10000, '\n');

    return EXIT_SUCCESS;
}
