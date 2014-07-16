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

#ifndef SFML_RESPONSE_HPP
#define SFML_RESPONSE_HPP

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Network/Http.hpp>

namespace sf
{
    ////////////////////////////////////////////////////////////
    /// \brief Define a HTTP response
    ///
    ////////////////////////////////////////////////////////////
    class SFML_NETWORK_API Http::Response
    {
    public :

        ////////////////////////////////////////////////////////////
        /// \brief Enumerate all the valid status codes for a response
        ///
        ////////////////////////////////////////////////////////////
        enum Status
        {
            // 2xx: success
            Ok             = 200, ///< Most common code returned when operation was successful
            Created        = 201, ///< The resource has successfully been created
            Accepted       = 202, ///< The request has been accepted, but will be processed later by the server
            NoContent      = 204, ///< The server didn't send any data in return
            ResetContent   = 205, ///< The server informs the client that it should clear the view (form) that caused the request to be sent
            PartialContent = 206, ///< The server has sent a part of the resource, as a response to a partial GET request

            // 3xx: redirection
            MultipleChoices  = 300, ///< The requested page can be accessed from several locations
            MovedPermanently = 301, ///< The requested page has permanently moved to a new location
            MovedTemporarily = 302, ///< The requested page has temporarily moved to a new location
            NotModified      = 304, ///< For conditionnal requests, means the requested page hasn't changed and doesn't need to be refreshed

            // 4xx: client error
            BadRequest          = 400, ///< The server couldn't understand the request (syntax error)
            Unauthorized        = 401, ///< The requested page needs an authentification to be accessed
            Forbidden           = 403, ///< The requested page cannot be accessed at all, even with authentification
            NotFound            = 404, ///< The requested page doesn't exist
            RangeNotSatisfiable = 407, ///< The server can't satisfy the partial GET request (with a "Range" header field)

            // 5xx: server error
            InternalServerError = 500, ///< The server encountered an unexpected error
            NotImplemented      = 501, ///< The server doesn't implement a requested feature
            BadGateway          = 502, ///< The gateway server has received an error from the source server
            ServiceNotAvailable = 503, ///< The server is temporarily unavailable (overloaded, in maintenance, ...)
            GatewayTimeout      = 504, ///< The gateway server couldn't receive a response from the source server
            VersionNotSupported = 505, ///< The server doesn't support the requested HTTP version

            // 10xx: SFML custom codes
            InvalidResponse  = 1000, ///< Response is not a valid HTTP one
            ConnectionFailed = 1001  ///< Connection with server failed
        };

        ////////////////////////////////////////////////////////////
        /// \brief Default constructor
        ///
        /// Constructs an empty response.
        ///
        ////////////////////////////////////////////////////////////
        Response();

        ////////////////////////////////////////////////////////////
        /// \brief Get the value of a field
        ///
        /// If the field \a field is not found in the response header,
        /// the empty string is returned. This function uses
        /// case-insensitive comparisons.
        ///
        /// \param field Name of the field to get
        ///
        /// \return Value of the field, or empty string if not found
        ///
        ////////////////////////////////////////////////////////////
        const std::string& getField(const std::string& field) const;

        ////////////////////////////////////////////////////////////
        /// \brief Get the response status code
        ///
        /// The status code should be the first thing to be checked
        /// after receiving a response, it defines whether it is a
        /// success, a failure or anything else (see the Status
        /// enumeration).
        ///
        /// \return Status code of the response
        ///
        ////////////////////////////////////////////////////////////
        Status getStatus() const;

        ////////////////////////////////////////////////////////////
        /// \brief Get the major HTTP version number of the response
        ///
        /// \return Major HTTP version number
        ///
        /// \see getMinorHttpVersion
        ///
        ////////////////////////////////////////////////////////////
        unsigned int getMajorHttpVersion() const;

        ////////////////////////////////////////////////////////////
        /// \brief Get the minor HTTP version number of the response
        ///
        /// \return Minor HTTP version number
        ///
        /// \see getMajorHttpVersion
        ///
        ////////////////////////////////////////////////////////////
        unsigned int getMinorHttpVersion() const;

        ////////////////////////////////////////////////////////////
        /// \brief Get the body of the response
        ///
        /// The body of a response may contain:
        /// \li the requested page (for GET requests)
        /// \li a response from the server (for POST requests)
        /// \li nothing (for HEAD requests)
        /// \li an error message (in case of an error)
        ///
        /// \return The response body
        ///
        ////////////////////////////////////////////////////////////
        const std::string& getBody() const;

    private :

        friend class Http;

        ////////////////////////////////////////////////////////////
        /// \brief Construct the header from a response string
        ///
        /// This function is used by Http to build the response
        /// of a request.
        ///
        /// \param data Content of the response to parse
        ///
        ////////////////////////////////////////////////////////////
        void parse(const std::string& data);


        ////////////////////////////////////////////////////////////
        /// \brief Read values passed in the answer header
        ///
        /// This function is used by Http to extract values passed
        /// in the response.
        ///
        /// \param in String stream containing the header values
        ///
        ////////////////////////////////////////////////////////////
        void parseFields(std::istream &in);

        ////////////////////////////////////////////////////////////
        // Types
        ////////////////////////////////////////////////////////////
        typedef std::map<std::string, std::string> FieldTable;

        ////////////////////////////////////////////////////////////
        // Member data
        ////////////////////////////////////////////////////////////
        FieldTable   m_fields;       ///< Fields of the header
        Status       m_status;       ///< Status code
        unsigned int m_majorVersion; ///< Major HTTP version
        unsigned int m_minorVersion; ///< Minor HTTP version
        std::string  m_body;         ///< Body of the response
    };
}

////////////////////////////////////////////////////////////
/// \class sf::Http::Response
/// \ingroup network
///
/// sf::Http::Response parse the response from the web server
/// and provides getters to read them. The response contains:
/// \li a status code
/// \li header fields (that may be answers to the ones that you requested)
/// \li a body, which contains the contents of the requested resource
///
////////////////////////////////////////////////////////////
