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

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Network/Http.h>
#include <SFML/Network/HttpStruct.h>
#include <SFML/Internal.h>


////////////////////////////////////////////////////////////
/// Construct a new Http request
////////////////////////////////////////////////////////////
sfHttpRequest* sfHttpRequest_Create(void)
{
    return new sfHttpRequest;
}


////////////////////////////////////////////////////////////
/// Destroy an existing Http request
////////////////////////////////////////////////////////////
void sfHttpRequest_Destroy(sfHttpRequest* httpRequest)
{
    delete httpRequest;
}


////////////////////////////////////////////////////////////
/// Set the value of a field; the field is added if it doesn't exist
////////////////////////////////////////////////////////////
void sfHttpRequest_SetField(sfHttpRequest* httpRequest, const char* field, const char* value)
{
    CSFML_CHECK(httpRequest);
    if (field)
        httpRequest->This.SetField(field, value);
}


////////////////////////////////////////////////////////////
/// Set the request method.
/// This parameter is sfHttpGet by default
////////////////////////////////////////////////////////////
void sfHttpRequest_SetMethod(sfHttpRequest* httpRequest, sfHttpMethod method)
{
    CSFML_CALL(httpRequest, SetMethod(static_cast<sf::Http::Request::Method>(method)));
}


////////////////////////////////////////////////////////////
/// Set the target URI of the request.
/// This parameter is "/" by default
////////////////////////////////////////////////////////////
void sfHttpRequest_SetUri(sfHttpRequest* httpRequest, const char* uri)
{
    CSFML_CALL(httpRequest, SetUri(uri ? uri : ""));
}


////////////////////////////////////////////////////////////
/// Set the HTTP version of the request.
/// This parameter is 1.0 by default
////////////////////////////////////////////////////////////
void sfHttpRequest_SetHttpVersion(sfHttpRequest* httpRequest, unsigned int major, unsigned int minor)
{
    CSFML_CALL(httpRequest, SetHttpVersion(major, minor));
}


////////////////////////////////////////////////////////////
/// Set the body of the request. This parameter is optional and
/// makes sense only for POST requests.
/// This parameter is empty by default
////////////////////////////////////////////////////////////
void sfHttpRequest_SetBody(sfHttpRequest* httpRequest, const char* body)
{
    CSFML_CALL(httpRequest, SetBody(body ? body : ""));
}


////////////////////////////////////////////////////////////
/// Destroy an existing Http response
////////////////////////////////////////////////////////////
void sfHttpResponse_Destroy(sfHttpResponse* httpResponse)
{
    delete httpResponse;
}


////////////////////////////////////////////////////////////
/// Get the value of a field; returns NULL if the field doesn't exist
////////////////////////////////////////////////////////////
const char* sfHttpResponse_GetField(const sfHttpResponse* httpResponse, const char* field)
{
    CSFML_CHECK_RETURN(httpResponse, NULL);
    if (!field)
        return NULL;

    return httpResponse->This.GetField(field).c_str();
}


////////////////////////////////////////////////////////////
/// Get the status of a response
////////////////////////////////////////////////////////////
sfHttpStatus sfHttpResponse_GetStatus(const sfHttpResponse* httpResponse)
{
    CSFML_CHECK_RETURN(httpResponse, sfHttpInvalidResponse);

    return static_cast<sfHttpStatus>(httpResponse->This.GetStatus());
}


////////////////////////////////////////////////////////////
/// Get the major HTTP version of a response
////////////////////////////////////////////////////////////
unsigned int sfHttpResponse_GetMajorVersion(const sfHttpResponse* httpResponse)
{
    CSFML_CALL_RETURN(httpResponse, GetMajorHttpVersion(), 0);
}


////////////////////////////////////////////////////////////
/// Get the minor HTTP version of a response
////////////////////////////////////////////////////////////
unsigned int sfHttpResponse_GetMinorVersion(const sfHttpResponse* httpResponse)
{
    CSFML_CALL_RETURN(httpResponse, GetMinorHttpVersion(), 0);
}


////////////////////////////////////////////////////////////
/// Get the body of the response. The body can contain :
/// - the requested page (for GET requests)
/// - a response from the server (for POST requests)
/// - nothing (for HEAD requests)
/// - an error message (in case of an error)
////////////////////////////////////////////////////////////
const char* sfHttpResponse_GetBody(const sfHttpResponse* httpResponse)
{
    CSFML_CHECK_RETURN(httpResponse, NULL);

    return httpResponse->This.GetBody().c_str();
}


////////////////////////////////////////////////////////////
/// Construct a new Http object
////////////////////////////////////////////////////////////
sfHttp* sfHttp_Create(void)
{
    return new sfHttp;
}


////////////////////////////////////////////////////////////
/// Destroy an existing Http object
////////////////////////////////////////////////////////////
void sfHttp_Destroy(sfHttp* http)
{
    delete http;
}


////////////////////////////////////////////////////////////
/// Set the target host of a Http server
////////////////////////////////////////////////////////////
void sfHttp_SetHost(sfHttp* http, const char* host, unsigned short port)
{
    CSFML_CALL(http, SetHost(host ? host : "", port));
}


////////////////////////////////////////////////////////////
/// Send a HTTP request and return the server's response.
/// You must be connected to a host before sending requests.
/// Any missing mandatory header field will be added with an appropriate value.
/// Warning : this function waits for the server's response and may
/// not return instantly; use a thread if you don't want to block your
/// application.
////////////////////////////////////////////////////////////
sfHttpResponse* sfHttp_SendRequest(sfHttp* http, const sfHttpRequest* request, float timeout)
{
    CSFML_CHECK_RETURN(http,    NULL);
    CSFML_CHECK_RETURN(request, NULL);

    sfHttpResponse* response = new sfHttpResponse;
    response->This = http->This.SendRequest(request->This, timeout);

    return response;
}
