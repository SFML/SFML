////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Network.hpp>

#include <atomic>
#include <chrono>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <optional>
#include <string>

#include <csignal>


namespace
{
// Flag to track if the user requested to interrupt the operation via SIGINT
std::atomic_bool interrupt{false};

// Check an SFTP result and print the error message if necessary
bool checkResult(const sf::Sftp::Result& result)
{
    if (result.isOk())
        return true;
    if ((result.getValue() == sf::Sftp::Result::Value::Timeout) && interrupt)
    {
        std::cout << "Interrupted" << std::endl;
    }
    else
    {
        std::cout << "Operation failed: " << result.getMessage() << std::endl;
    }
    return false;
}

// Stream output operator for filesystem permissions
std::ostream& operator<<(std::ostream& stream, std::filesystem::perms permissions)
{
    const auto print = [&](char c, std::filesystem::perms permission)
    { stream << (((permissions & permission) == std::filesystem::perms::none) ? '-' : c); };
    print('r', std::filesystem::perms::owner_read);
    print('w', std::filesystem::perms::owner_write);
    print('x', std::filesystem::perms::owner_exec);
    print('r', std::filesystem::perms::group_read);
    print('w', std::filesystem::perms::group_write);
    print('x', std::filesystem::perms::group_exec);
    print('r', std::filesystem::perms::others_read);
    print('w', std::filesystem::perms::others_write);
    print('x', std::filesystem::perms::others_exec);
    return stream;
}

// Stream output operator for 64-bit unsigned integers
std::ostream& operator<<(std::ostream& stream, const std::optional<std::uint64_t>& value)
{
    if (value.has_value())
        return stream << std::setw(10) << std::setfill(' ') << *value << std::resetiosflags(stream.flags());
    return stream << "          ";
}

// Stream output operator for filesystem file types
std::ostream& operator<<(std::ostream& stream, const std::optional<std::filesystem::file_type>& type)
{
    switch (type.value_or(std::filesystem::file_type::unknown))
    {
        case std::filesystem::file_type::block:
            return stream << 'b';
        case std::filesystem::file_type::character:
            return stream << 'c';
        case std::filesystem::file_type::directory:
            return stream << 'd';
        case std::filesystem::file_type::fifo:
            return stream << 'f';
        case std::filesystem::file_type::regular:
            return stream << 'r';
        case std::filesystem::file_type::socket:
            return stream << 's';
        case std::filesystem::file_type::symlink:
            return stream << 'l';
        default:
            return stream << '?';
    }
}
} // namespace


////////////////////////////////////////////////////////////
/// Entry point of application
///
/// \return Application exit code
///
////////////////////////////////////////////////////////////
int main()
{
    // Install SIGINT handler to allow the user to interrupt SFTP operations
    std::signal(SIGINT, [](int) { interrupt = true; });

    // Timeout predicate to check whether the ongoing operation has to be interrupted, poll at 1ms
    sf::Sftp::TimeoutPredicate timeout([]() { return !interrupt; }, sf::milliseconds(1));

    // Choose the server address
    std::optional<sf::IpAddress> address;
    do
    {
        std::cout << "Enter the SFTP server address: ";
        std::cin >> address;
    } while (!address.has_value());

    // Choose the server port
    std::optional<unsigned short> port;
    do
    {
        try
        {
            std::cout << "Enter the SFTP server port (empty to use default 22): ";
            std::string line;
            std::cin.ignore(10'000, '\n');
            std::getline(std::cin, line);
            port = static_cast<unsigned short>(line.empty() ? 22ul : std::stoul(line));
        } catch (const std::exception&)
        {
            continue;
        }
    } while (!port.has_value());

    // Connect to the server
    sf::Sftp server;
    if (const auto result = server.connect(address.value(), port.value(), timeout); !checkResult(result))
        return EXIT_FAILURE;
    std::cout << "Connecting to SFTP server at " << address->toString() << " successful" << std::endl;

    // Print session information
    const auto sessionInfo = server.getSessionInfo();

    if (!sessionInfo.has_value())
    {
        std::cout << "Failed to retrieve session information" << std::endl;
        return EXIT_FAILURE;
    }

    std::cout << "\nHost key SHA256: " << std::hex << std::setw(2) << std::setfill('0');
    for (const auto b : sessionInfo->hostKey.sha256)
        std::cout << static_cast<int>(b);
    std::cout << std::resetiosflags(std::cout.flags()) << std::endl;
    std::cout << "Host key algorithm: " << sessionInfo->hostKeyAlgorithm << std::endl;
    std::cout << "Key exchange algorithm: " << sessionInfo->keyExchangeAlgorithm << std::endl;
    std::cout << "Encryption algorithm: " << sessionInfo->serverToClientEncryptionAlgorithm << std::endl;
    std::cout << "MAC algorithm: " << sessionInfo->serverToClientMacAlgorithm << std::endl;
    std::cout << "Compression: " << sessionInfo->serverToClientCompressionAlgorithm << '\n' << std::endl;

    const auto returnValue = [&]()
    {
        // Ask user if they trust the server
        {
            char answer{};
            std::cout << "Continue logging into this server? (Y/N): ";
            do
            {
                std::cin >> answer;
                std::cin.ignore(10'000, '\n');
            } while (answer != 'Y' && answer != 'y' && answer != 'N' && answer != 'n');
            if ((answer == 'N') || (answer == 'n'))
                return 0;
        }

        // Ask for user name and password
        {
            std::string user;
            std::string password;
            std::cout << "User name: ";
            std::cin >> user;
            std::cout << "Password: ";
            std::cin >> password;

            // Login to the server
            interrupt = false;
            if (const auto result = server.login(user, password, timeout); !checkResult(result))
                return EXIT_FAILURE;
            std::cout << "Logging in to SFTP server successful" << std::endl;
        }

        // Main menu
        while (true)
        {
            // Reset interrupt flag
            interrupt = false;

            // Print the current server directory
            if (const auto result = server.getWorkingDirectory(timeout); checkResult(result))
                std::cout << "\nCurrent directory is " << result.getPath() << std::endl;

            // Main FTP menu
            std::cout << '\n'
                      << "Choose an action:\n"
                      << "1. Print contents of a directory\n"
                      << "2. Create directory\n"
                      << "3. Delete directory\n"
                      << "4. Rename file\n"
                      << "5. Remove file\n"
                      << "6. Download file\n"
                      << "7. Upload file\n"
                      << "8. Disconnect\n"
                      << std::endl;

            std::cout << "Your choice: ";
            int choice = 0;
            std::cin >> choice;
            std::cout << std::endl;

            // Reset interrupt flag again for the requested operation
            interrupt = false;

            switch (choice)
            {
                default:
                {
                    // Wrong choice
                    std::cout << "Invalid choice!" << std::endl;
                    std::cin.clear();
                    std::cin.ignore(10'000, '\n');
                    break;
                }

                case 1:
                {
                    // Print the contents of a directory
                    std::string path;
                    std::cout << "Remote path of the directory whose contents to print: ";
                    std::cin >> path;
                    if (const auto result = server.getDirectoryListing(path, timeout); checkResult(result))
                    {
                        for (const auto& entry : result.getListing())
                            std::cout << entry.type << ' ' << entry.permissions.value_or(std::filesystem::perms{})
                                      << ' ' << entry.userId << ' ' << entry.groupId << ' ' << entry.size << ' '
                                      << entry.path.generic_string() << '\n';
                        std::cout.flush();
                    }
                    break;
                }

                case 2:
                {
                    // Create a new directory
                    std::string path;
                    std::cout << "Remote path of the directory to create: ";
                    std::cin >> path;
                    if (const auto result = server.createDirectory(path,
                                                                   (std::filesystem::perms::owner_all |
                                                                    std::filesystem::perms::group_read |
                                                                    std::filesystem::perms::group_exec |
                                                                    std::filesystem::perms::others_read |
                                                                    std::filesystem::perms::others_exec),
                                                                   timeout);
                        checkResult(result))
                        std::cout << "Creating directory successful" << std::endl;
                    break;
                }

                case 3:
                {
                    // Remove an existing directory
                    std::string path;
                    std::cout << "Remote name of the directory to remove: ";
                    std::cin >> path;
                    if (const auto result = server.deleteDirectory(path, timeout); checkResult(result))
                        std::cout << "Deleting directory successful" << std::endl;
                    break;
                }

                case 4:
                {
                    // Rename/Move a file/directory
                    std::string source;
                    std::string destination;
                    std::cout << "Remote path of the file/directory to rename/move: ";
                    std::cin >> source;
                    std::cout << "New path: ";
                    std::cin >> destination;
                    if (const auto result = server.rename(source, destination, false, timeout); checkResult(result))
                        std::cout << "Rename/move successful" << std::endl;
                    break;
                }

                case 5:
                {
                    // Remove an existing file
                    std::string path;
                    std::cout << "Remote path of the file to remove: ";
                    std::cin >> path;
                    if (const auto result = server.deleteFile(path, timeout); checkResult(result))
                        std::cout << "Deleting file successful" << std::endl;
                    break;
                }

                case 6:
                {
                    // Download a file from server
                    std::string remotePath;
                    std::string localPath;
                    std::cout << "Remote path of the file to download: ";
                    std::cin.ignore(10'000, '\n');
                    std::cin >> remotePath;
                    std::cout << "Local path to save the file to: ";
                    std::cin >> localPath;

                    const auto fileAttributes = server.getAttributes(remotePath, true, timeout);
                    if (!checkResult(fileAttributes) || !fileAttributes.getAttributes().size.has_value())
                        break;
                    const auto fileSize = *fileAttributes.getAttributes().size;
                    std::cout << "Remote file size: " << fileSize << " bytes\n" << std::endl;

                    std::ofstream ofs;
                    std::size_t   totalDownloaded{};
                    interrupt         = false;
                    const auto start  = std::chrono::steady_clock::now();
                    const auto result = server.download(
                        remotePath,
                        [&](const void* data, std::size_t size)
                        {
                            if (!ofs.is_open())
                            {
                                ofs.open(localPath, std::ios::binary);
                                if (!ofs)
                                {
                                    std::cout << "Failed to open local file for writing" << std::endl;
                                    return false;
                                }
                            }

                            if (!ofs.write(static_cast<const char*>(data), static_cast<std::streamsize>(size)))
                            {
                                std::cout << "Failed to write to local file" << std::endl;
                                return false;
                            }

                            totalDownloaded += size;
                            std::cout << "\rDownloaded " << totalDownloaded << " bytes" << std::flush;

                            return !interrupt;
                        },
                        0,
                        timeout);
                    std::cout << '\n' << std::endl;
                    if (checkResult(result))
                    {
                        if (interrupt)
                        {
                            std::cout << "Download interrupted" << std::endl;
                        }
                        else
                        {
                            std::cout << "Download completed successfully ("
                                      << static_cast<double>(fileSize) /
                                             static_cast<double>(std::chrono::duration_cast<std::chrono::microseconds>(
                                                                     std::chrono::steady_clock::now() - start)
                                                                     .count())
                                      << " MB/s)" << std::endl;
                        }
                    }
                    break;
                }

                case 7:
                {
                    // Upload a file to server
                    std::string remotePath;
                    std::string localPath;
                    std::cout << "Remote path of the file to upload: ";
                    std::cin.ignore(10'000, '\n');
                    std::getline(std::cin, remotePath);
                    std::cout << "Local path to read the file from: ";
                    std::getline(std::cin, localPath);

                    std::ifstream ifs(localPath, std::ios::binary);
                    if (!ifs)
                    {
                        std::cout << "Failed to open local file for reading" << std::endl;
                        break;
                    }
                    const auto fileSize = std::filesystem::file_size(localPath);
                    std::cout << "Local file size: " << fileSize << " bytes\n" << std::endl;

                    std::size_t totalUploaded{};
                    const auto  start  = std::chrono::steady_clock::now();
                    const auto  result = server.upload(
                        remotePath,
                        [&](void* data, std::size_t& size)
                        {
                            ifs.read(static_cast<char*>(data), static_cast<std::streamsize>(size));
                            size = static_cast<std::size_t>(ifs.gcount());
                            totalUploaded += size;
                            std::cout << "\rUploaded " << totalUploaded << " bytes" << std::flush;

                            if (ifs.eof())
                                return false;

                            if (ifs.fail())
                            {
                                std::cout << "Failed to read from local file" << std::endl;
                                return false;
                            }

                            return !interrupt;
                        },
                        (std::filesystem::perms::owner_read | std::filesystem::perms::owner_write |
                         std::filesystem::perms::group_read | std::filesystem::perms::others_read),
                        true,
                        false,
                        0,
                        timeout);
                    std::cout << '\n' << std::endl;
                    if (checkResult(result))
                    {
                        if (interrupt)
                        {
                            std::cout << "Upload interrupted" << std::endl;
                        }
                        else
                        {
                            std::cout << "Upload completed successfully ("
                                      << static_cast<double>(fileSize) /
                                             static_cast<double>(std::chrono::duration_cast<std::chrono::microseconds>(
                                                                     std::chrono::steady_clock::now() - start)
                                                                     .count())
                                      << " MB/s)" << std::endl;
                        }
                    }
                    break;
                }

                case 8:
                {
                    // Disconnect
                    return 0;
                }
            }
        }
    }();

    // Disconnect from the server
    std::cout << "Disconnecting from server..." << std::endl;
    if (const auto result = server.disconnect(timeout); checkResult(result))
        std::cout << "Disconnected from server successfully" << std::endl;

    // Wait until the user presses 'enter' key
    std::cout << "Press enter to exit..." << std::endl;
    std::cin.ignore(10'000, '\n');
    std::cin.ignore(10'000, '\n');

    return returnValue;
}
