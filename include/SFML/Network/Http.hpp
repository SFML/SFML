////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007-2014 Laurent Gomila (laurent.gom@gmail.com)
//
// This software is provided 'as-is', without any express or implied warranty.
// In no event will the authors be held liable for any damages arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it freely,
// subject to the following restrictions:
//
// 1. The origin of this software must not be misrepresented;
//    you must not claim that you wrote the original software.
//    If you use this software in a product, an acknowledgment
//    in the product documentation would be appreciated but is not required.
//
// 2. Altered source versions must be plainly marked as such,
//    and must not be misrepresented as being the original software.
//
// 3. This notice may not be removed or altered from any source distribution.
//
////////////////////////////////////////////////////////////

#ifndef SFML_HTTP_HPP
#define SFML_HTTP_HPP

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Network/Export.hpp>
#include <SFML/Network/IpAddress.hpp>
#include <SFML/Network/TcpSocket.hpp>
#include <SFML/System/NonCopyable.hpp>
#include <SFML/System/Time.hpp>
#include <map>
#include <string>


namespace sf
{
////////////////////////////////////////////////////////////
/// \brief A HTTP client
///
////////////////////////////////////////////////////////////
class SFML_NETWORK_API Http : NonCopyable
{
public :

    ////////////////////////////////////////////////////////////
    /// \brief Declare a HTTP request and response
    ///
    ////////////////////////////////////////////////////////////
    class SFML_NETWORK_API Request;
    class SFML_NETWORK_API Response;

    ////////////////////////////////////////////////////////////
    /// \brief Default constructor
    ///
    ////////////////////////////////////////////////////////////
    Http();

    ////////////////////////////////////////////////////////////
    /// \brief Construct the HTTP client with the target host
    ///
    /// This is equivalent to calling setHost(host, port).
    /// The port has a default value of 0, which means that the
    /// HTTP client will use the right port according to the
    /// protocol used (80 for HTTP). You should leave it like
    /// this unless you really need a port other than the
    /// standard one, or use an unknown protocol.
    ///
    /// \param host Web server to connect to
    /// \param port Port to use for connection
    ///
    ////////////////////////////////////////////////////////////
    Http(const std::string& host, unsigned short port = 0);

    ////////////////////////////////////////////////////////////
    /// \brief Set the target host
    ///
    /// This function just stores the host address and port, it
    /// doesn't actually connect to it until you send a request.
    /// The port has a default value of 0, which means that the
    /// HTTP client will use the right port according to the
    /// protocol used (80 for HTTP). You should leave it like
    /// this unless you really need a port other than the
    /// standard one, or use an unknown protocol.
    ///
    /// \param host Web server to connect to
    /// \param port Port to use for connection
    ///
    ////////////////////////////////////////////////////////////
    void setHost(const std::string& host, unsigned short port = 0);

    ////////////////////////////////////////////////////////////
    /// \brief Send a HTTP request and return the server's response.
    ///
    /// You must have a valid host before sending a request (see setHost).
    /// Any missing mandatory header field in the request will be added
    /// with an appropriate value.
    /// Warning: this function waits for the server's response and may
    /// not return instantly; use a thread if you don't want to block your
    /// application, or use a timeout to limit the time to wait. A value
    /// of Time::Zero means that the client will use the system defaut timeout
    /// (which is usually pretty long).
    ///
    /// \param request Request to send
    /// \param timeout Maximum time to wait
    ///
    /// \return Server's response
    ///
    ////////////////////////////////////////////////////////////
    Response sendRequest(const Request& request, Time timeout = Time::Zero);

private :

    ////////////////////////////////////////////////////////////
    // Member data
    ////////////////////////////////////////////////////////////
    TcpSocket      m_connection; ///< Connection to the host
    IpAddress      m_host;       ///< Web host address
    std::string    m_hostName;   ///< Web host name
    unsigned short m_port;       ///< Port used for connection with host
};

} // namespace sf


#endif // SFML_HTTP_HPP


////////////////////////////////////////////////////////////
/// \class sf::Http
/// \ingroup network
///
/// sf::Http is a very simple HTTP client that allows you
/// to communicate with a web server. You can retrieve
/// web pages, send data to an interactive resource,
/// download a remote file, etc. The HTTPS protocol is
/// not supported.
///
/// The HTTP client is split into 3 classes:
/// \li sf::Http::Request
/// \li sf::Http::Response
/// \li sf::Http
///
/// sf::Http provides a simple function, SendRequest, to send a
/// sf::Http::Request and return the corresponding sf::Http::Response
/// from the server.
///
/// Usage example:
/// \code
/// // Create a new HTTP client
/// sf::Http http;
///
/// // We'll work on http://www.sfml-dev.org
/// http.setHost("http://www.sfml-dev.org");
///
/// // Prepare a request to get the 'features.php' page
/// sf::Http::Request request("features.php");
///
/// // Send the request
/// sf::Http::Response response = http.sendRequest(request);
///
/// // Check the status code and display the result
/// sf::Http::Response::Status status = response.getStatus();
/// if (status == sf::Http::Response::Ok)
/// {
///     std::cout << response.getBody() << std::endl;
/// }
/// else
/// {
///     std::cout << "Error " << status << std::endl;
/// }
/// \endcode
///
////////////////////////////////////////////////////////////
