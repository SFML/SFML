////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Network.hpp>

#include <iostream>
#include <optional>
#include <string>


namespace
{
////////////////////////////////////////////////////////////
/// Request a URL
///
////////////////////////////////////////////////////////////
void requestUrl(const std::string& url)
{
    // Split the URL up into host and resource parts
    const auto resourcePos = url.find('/', url.find("://") + 3);
    const auto resource    = (resourcePos != std::string::npos) ? url.substr(resourcePos) : std::string("/");
    const auto host        = url.substr(0, resourcePos);
    const auto portPos     = host.find(':', 6);
    const auto port        = (portPos != std::string::npos) ? host.substr(portPos + 1) : std::string("0");

    // Create a new HTTP client
    sf::Http http;

    http.setHost(host.substr(0, portPos), static_cast<unsigned short>(std::stoi(port)));

    // Prepare a request to get the resource
    const sf::Http::Request request(resource);

    // Send the request
    const sf::Http::Response response = http.sendRequest(request);

    // Check the status code and display the result
    const sf::Http::Response::Status status    = response.getStatus();
    const auto                       statusNum = static_cast<int>(status);
    std::cout << "Server responded with HTTP status " << statusNum << '\n' << std::endl;

    // Output body if its content type is text-based and not compressed
    if (response.getField("Content-Type").find("text") == 0)
    {
        if (const auto encoding = response.getField("Content-Encoding"); encoding.empty())
        {
            std::cout << response.getBody() << std::endl;
        }
        else
        {
            std::cout << encoding << " compressed body content, length: " << response.getBody().size() << '\n'
                      << std::endl;
        }
    }

    // Follow redirections
    if (statusNum >= 301 && statusNum <= 308)
    {
        if (auto nextUrl = response.getField("Location"); !nextUrl.empty())
        {
            if ((nextUrl.find("http://") != 0) && (nextUrl.find("https://") != 0))
                nextUrl = host + nextUrl;
            std::cout << "Following redirect to " << nextUrl << '\n' << std::endl;
            requestUrl(nextUrl);
        }
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
    // Ask for the complete webpage URL
    std::string url;
    do
    {
        std::cout << "Type the complete URL of the webpage to request: ";
        std::cin >> url;
    } while ((url.find("http://") != 0) && (url.find("https://") != 0));

    std::cout << "\nRequesting " << url << '\n' << std::endl;

    requestUrl(url);

    // Wait until the user presses 'enter' key
    std::cout << "Press enter to exit..." << std::endl;
    std::cin.ignore(10'000, '\n');
    std::cin.ignore(10'000, '\n');
}
