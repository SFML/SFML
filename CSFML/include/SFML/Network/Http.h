////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007-2009 Laurent Gomila (laurent.gom@gmail.com)
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

#ifndef SFML_HTTP_H
#define SFML_HTTP_H

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Config.h>
#include <SFML/Network/Types.h>


////////////////////////////////////////////////////////////
/// Enumerate the available HTTP methods for a request
////////////////////////////////////////////////////////////
enum sfHttpMethod
{
    sfHttpGet,  ///< Request in get mode, standard method to retrieve a page
    sfHttpPost, ///< Request in post mode, usually to send data to a page
    sfHttpHead  ///< Request a page's header only
};


////////////////////////////////////////////////////////////
/// Enumerate all the valid status codes returned in
/// a HTTP response
////////////////////////////////////////////////////////////
enum sfHttpStatus
{
    // 2xx: success
    sfHttpOk        = 200, ///< Most common code returned when operation was successful
    sfHttpCreated   = 201, ///< The resource has successfully been created
    sfHttpAccepted  = 202, ///< The request has been accepted, but will be processed later by the server
    sfHttpNoContent = 204, ///< Sent when the server didn't send any data in return

    // 3xx: redirection
    sfHttpMultipleChoices  = 300, ///< The requested page can be accessed from several locations
    sfHttpMovedPermanently = 301, ///< The requested page has permanently moved to a new location
    sfHttpMovedTemporarily = 302, ///< The requested page has temporarily moved to a new location
    sfHttpNotModified      = 304, ///< For conditionnal requests, means the requested page hasn't changed and doesn't need to be refreshed

    // 4xx: client error
    sfHttpBadRequest   = 400, ///< The server couldn't understand the request (syntax error)
    sfHttpUnauthorized = 401, ///< The requested page needs an authentification to be accessed
    sfHttpForbidden    = 403, ///< The requested page cannot be accessed at all, even with authentification
    sfHttpNotFound     = 404, ///< The requested page doesn't exist

    // 5xx: server error
    sfHttpInternalServerError = 500, ///< The server encountered an unexpected error
    sfHttpNotImplemented      = 501, ///< The server doesn't implement a requested feature
    sfHttpBadGateway          = 502, ///< The gateway server has received an error from the source server
    sfHttpServiceNotAvailable = 503, ///< The server is temporarily unavailable (overloaded, in maintenance, ...)

    // 10xx: SFML custom codes
    sfHttpInvalidResponse  = 1000, ///< Response is not a valid HTTP one
    sfHttpConnectionFailed = 1001  ///< Connection with server failed
};


////////////////////////////////////////////////////////////
/// Construct a new Http request
///
/// \return Pointer to the new Http request
///
////////////////////////////////////////////////////////////
CSFML_API sfHttpRequest* sfHttpRequest_Create(void);

////////////////////////////////////////////////////////////
/// Destroy an existing Http request
///
/// \param httpRequest : Http request to destroy
///
////////////////////////////////////////////////////////////
CSFML_API void sfHttpRequest_Destroy(sfHttpRequest* httpRequest);

////////////////////////////////////////////////////////////
/// Set the value of a field; the field is added if it doesn't exist
///
/// \param httpRequest : Http request to modify
/// \param field :       Name of the field to set (case-insensitive)
/// \param value :       Value of the field
///
////////////////////////////////////////////////////////////
CSFML_API void sfHttpRequest_SetField(sfHttpRequest* httpRequest, const char* field, const char* value);

////////////////////////////////////////////////////////////
/// Set the request method.
/// This parameter is sfHttpGet by default
///
/// \param httpRequest : Http request to modify
/// \param method :      Method to use for the request
///
////////////////////////////////////////////////////////////
CSFML_API void sfHttpRequest_SetMethod(sfHttpRequest* httpRequest, sfHttpMethod method);

////////////////////////////////////////////////////////////
/// Set the target URI of the request.
/// This parameter is "/" by default
///
/// \param httpRequest : Http request to modify
/// \param URI :         URI to request, local to the host
///
////////////////////////////////////////////////////////////
CSFML_API void sfHttpRequest_SetUri(sfHttpRequest* httpRequest, const char* uri);

////////////////////////////////////////////////////////////
/// Set the HTTP version of the request.
/// This parameter is 1.0 by default
///
/// \param httpRequest : Http request to modify
/// \param major :       Major version number
/// \param minor :       Minor version number
///
////////////////////////////////////////////////////////////
CSFML_API void sfHttpRequest_SetHttpVersion(sfHttpRequest* httpRequest, unsigned int major, unsigned int minor);

////////////////////////////////////////////////////////////
/// Set the body of the request. This parameter is optional and
/// makes sense only for POST requests.
/// This parameter is empty by default
///
/// \param httpRequest : Http request to modify
/// \param body :        Content of the request body
///
////////////////////////////////////////////////////////////
CSFML_API void sfHttpRequest_SetBody(sfHttpRequest* httpRequest, const char* body);

////////////////////////////////////////////////////////////
/// Destroy an existing Http response
///
/// \param httpResponse : Http response to destroy
///
////////////////////////////////////////////////////////////
CSFML_API void sfHttpResponse_Destroy(sfHttpResponse* httpResponse);

////////////////////////////////////////////////////////////
/// Get the value of a field; returns NULL if the field doesn't exist
///
/// \param httpResponse : Http response
/// \param field :        Field to get
///
/// \return Value of the field (NULL if it doesn't exist)
///
////////////////////////////////////////////////////////////
CSFML_API const char* sfHttpResponse_GetField(const sfHttpResponse* httpResponse, const char* field);

////////////////////////////////////////////////////////////
/// Get the status of a response
///
/// \param httpResponse : Http response
///
/// \return Status of the response
///
////////////////////////////////////////////////////////////
CSFML_API sfHttpStatus sfHttpResponse_GetStatus(const sfHttpResponse* httpResponse);

////////////////////////////////////////////////////////////
/// Get the major HTTP version of a response
///
/// \param httpResponse : Http response
///
/// \return HTTP major version of the response
///
////////////////////////////////////////////////////////////
CSFML_API unsigned int sfHttpResponse_GetMajorVersion(const sfHttpResponse* httpResponse);

////////////////////////////////////////////////////////////
/// Get the minor HTTP version of a response
///
/// \param httpResponse : Http response
///
/// \return HTTP minor version of the response
///
////////////////////////////////////////////////////////////
CSFML_API unsigned int sfHttpResponse_GetMinorVersion(const sfHttpResponse* httpResponse);

////////////////////////////////////////////////////////////
/// Get the body of the response. The body can contain :
/// - the requested page (for GET requests)
/// - a response from the server (for POST requests)
/// - nothing (for HEAD requests)
/// - an error message (in case of an error)
///
/// \param httpResponse : Http response
///
/// \return Body of the response (empty string if no body)
///
////////////////////////////////////////////////////////////
CSFML_API const char* sfHttpResponse_GetBody(const sfHttpResponse* httpResponse);

////////////////////////////////////////////////////////////
/// Construct a new Http object
///
/// \return Pointer to the new Http
///
////////////////////////////////////////////////////////////
CSFML_API sfHttp* sfHttp_Create(void);

////////////////////////////////////////////////////////////
/// Destroy an existing Http object
///
/// \param Http : Http to destroy
///
////////////////////////////////////////////////////////////
CSFML_API void sfHttp_Destroy(sfHttp* http);

////////////////////////////////////////////////////////////
/// Set the target host of a Http server
///
/// \param http : Http object
/// \param host : Web server to connect to
/// \param port : Port to use for connection (0 to use the standard port of the protocol used)
///
////////////////////////////////////////////////////////////
CSFML_API void sfHttp_SetHost(sfHttp* http, const char* host, unsigned short port);

////////////////////////////////////////////////////////////
/// Send a HTTP request and return the server's response.
/// You must be connected to a host before sending requests.
/// Any missing mandatory header field will be added with an appropriate value.
/// Warning : this function waits for the server's response and may
/// not return instantly; use a thread if you don't want to block your
/// application.
///
/// \param http :    Http object
/// \param request : Request to send
/// \param timeout : Maximum time to wait (0 to use no timeout)
///
/// \return Server's response, or NULL if request is invalid
///
////////////////////////////////////////////////////////////
CSFML_API sfHttpResponse* sfHttp_SendRequest(sfHttp* http, const sfHttpRequest* request, float timeout);


#endif // SFML_HTTP_H
