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

#ifndef SFML_REQUEST_HPP
#define SFML_REQUEST_HPP

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Network/Http.hpp>

namespace sf
{
    ////////////////////////////////////////////////////////////
    /// \brief Define a HTTP request
    ///
    ////////////////////////////////////////////////////////////
    class SFML_NETWORK_API Request
    {
    public :

        ////////////////////////////////////////////////////////////
        /// \brief Enumerate the available HTTP methods for a request
        ///
        ////////////////////////////////////////////////////////////
        enum Method
        {
            Get,   ///< Request in get mode, standard method to retrieve a page
            Post,  ///< Request in post mode, usually to send data to a page
            Head,  ///< Request a page's header only
            Put,   ///< Request in put mode, useful for a REST API
            Delete ///< Request in delete mode, useful for a REST API
        };

        ////////////////////////////////////////////////////////////
        /// \brief Default constructor
        ///
        /// This constructor creates a GET request, with the root
        /// URI ("/") and an empty body.
        ///
        /// \param uri    Target URI
        /// \param method Method to use for the request
        /// \param body   Content of the request's body
        ///
        ////////////////////////////////////////////////////////////
        Request(const std::string& uri = "/", Method method = Get, const std::string& body = "");

        ////////////////////////////////////////////////////////////
        /// \brief Set the value of a field
        ///
        /// The field is created if it doesn't exist. The name of
        /// the field is case insensitive.
        /// By default, a request doesn't contain any field (but the
        /// mandatory fields are added later by the HTTP client when
        /// sending the request).
        ///
        /// \param field Name of the field to set
        /// \param value Value of the field
        ///
        ////////////////////////////////////////////////////////////
        void setField(const std::string& field, const std::string& value);

        ////////////////////////////////////////////////////////////
        /// \brief Set the request method
        ///
        /// See the Method enumeration for a complete list of all
        /// the availale methods.
        /// The method is Http::Request::Get by default.
        ///
        /// \param method Method to use for the request
        ///
        ////////////////////////////////////////////////////////////
        void setMethod(Method method);

        ////////////////////////////////////////////////////////////
        /// \brief Set the requested URI
        ///
        /// The URI is the resource (usually a web page or a file)
        /// that you want to get or post.
        /// The URI is "/" (the root page) by default.
        ///
        /// \param uri URI to request, relative to the host
        ///
        ////////////////////////////////////////////////////////////
        void setUri(const std::string& uri);

        ////////////////////////////////////////////////////////////
        /// \brief Set the HTTP version for the request
        ///
        /// The HTTP version is 1.0 by default.
        ///
        /// \param major Major HTTP version number
        /// \param minor Minor HTTP version number
        ///
        ////////////////////////////////////////////////////////////
        void setHttpVersion(unsigned int major, unsigned int minor);

        ////////////////////////////////////////////////////////////
        /// \brief Set the body of the request
        ///
        /// The body of a request is optional and only makes sense
        /// for POST requests. It is ignored for all other methods.
        /// The body is empty by default.
        ///
        /// \param body Content of the body
        ///
        ////////////////////////////////////////////////////////////
        void setBody(const std::string& body);

    private :

        friend class Http;

        ////////////////////////////////////////////////////////////
        /// \brief Prepare the final request to send to the server
        ///
        /// This is used internally by Http before sending the
        /// request to the web server.
        ///
        /// \return String containing the request, ready to be sent
        ///
        ////////////////////////////////////////////////////////////
        std::string prepare() const;

        ////////////////////////////////////////////////////////////
        /// \brief Check if the request defines a field
        ///
        /// This function uses case-insensitive comparisons.
        ///
        /// \param field Name of the field to test
        ///
        /// \return True if the field exists, false otherwise
        ///
        ////////////////////////////////////////////////////////////
        bool hasField(const std::string& field) const;

        ////////////////////////////////////////////////////////////
        // Types
        ////////////////////////////////////////////////////////////
        typedef std::map<std::string, std::string> FieldTable;

        ////////////////////////////////////////////////////////////
        // Member data
        ////////////////////////////////////////////////////////////
        FieldTable   m_fields;       ///< Fields of the header associated to their value
        Method       m_method;       ///< Method to use for the request
        std::string  m_uri;          ///< Target URI of the request
        unsigned int m_majorVersion; ///< Major HTTP version
        unsigned int m_minorVersion; ///< Minor HTTP version
        std::string  m_body;         ///< Body of the request
    };
}

////////////////////////////////////////////////////////////
/// \class sf::Http::Request
/// \ingroup network
///
/// sf::Http::Request builds the request that will be
/// sent to the server. A request is made of:
/// \li a method (what you want to do)
/// \li a target URI (usually the name of the web page or file)
/// \li one or more header fields (options that you can pass to the server)
/// \li an optional body (for POST requests)
///
////////////////////////////////////////////////////////////
