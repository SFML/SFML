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
#include <SFML/Network/Http.hpp>
#include <SFML/Internal.h>


struct sfHttpRequest
{
    sf::Http::Request This;
};


////////////////////////////////////////////////////////////
/// Construct a new Http request
////////////////////////////////////////////////////////////
sfHttpRequest* sfHttpRequest_Create()
{
    return new sfHttpRequest;
}


////////////////////////////////////////////////////////////
/// Destroy an existing Http request
////////////////////////////////////////////////////////////
void sfHttpRequest_Destroy(sfHttpRequest* HttpRequest)
{
    delete HttpRequest;
}


////////////////////////////////////////////////////////////
/// Set the value of a field; the field is added if it doesn't exist
////////////////////////////////////////////////////////////
void sfHttpRequest_SetField(sfHttpRequest* HttpRequest, const char* Field, const char* Value)
{
    CSFML_CHECK(HttpRequest);
    if (Field)
        HttpRequest->This.SetField(Field, Value);
}


////////////////////////////////////////////////////////////
/// Set the request method.
/// This parameter is sfHttpGet by default
////////////////////////////////////////////////////////////
void sfHttpRequest_SetMethod(sfHttpRequest* HttpRequest, sfHttpMethod Method)
{
    CSFML_CALL(HttpRequest, SetMethod(static_cast<sf::Http::Request::Method>(Method)));
}


////////////////////////////////////////////////////////////
/// Set the target URI of the request.
/// This parameter is "/" by default
////////////////////////////////////////////////////////////
void sfHttpRequest_SetURI(sfHttpRequest* HttpRequest, const char* URI)
{
    CSFML_CALL(HttpRequest, SetURI(URI ? URI : ""));
}


////////////////////////////////////////////////////////////
/// Set the HTTP version of the request.
/// This parameter is 1.0 by default
////////////////////////////////////////////////////////////
void sfHttpRequest_SetHttpVersion(sfHttpRequest* HttpRequest, unsigned int Major, unsigned int Minor)
{
    CSFML_CALL(HttpRequest, SetHttpVersion(Major, Minor));
}


////////////////////////////////////////////////////////////
/// Set the body of the request. This parameter is optional and
/// makes sense only for POST requests.
/// This parameter is empty by default
////////////////////////////////////////////////////////////
void sfHttpRequest_SetBody(sfHttpRequest* HttpRequest, const char* Body)
{
    CSFML_CALL(HttpRequest, SetBody(Body ? Body : ""));
}


struct sfHttpResponse
{
    sf::Http::Response This;
};


////////////////////////////////////////////////////////////
/// Destroy an existing Http response
////////////////////////////////////////////////////////////
void sfHttpResponse_Destroy(sfHttpResponse* HttpResponse)
{
    delete HttpResponse;
}


////////////////////////////////////////////////////////////
/// Get the value of a field; returns NULL if the field doesn't exist
////////////////////////////////////////////////////////////
const char* sfHttpResponse_GetField(sfHttpResponse* HttpResponse, const char* Field)
{
    CSFML_CHECK_RETURN(HttpResponse, NULL);
    if (!Field)
        return NULL;

    return HttpResponse->This.GetField(Field).c_str();
}


////////////////////////////////////////////////////////////
/// Get the status of a response
////////////////////////////////////////////////////////////
sfHttpStatus sfHttpResponse_GetStatus(sfHttpResponse* HttpResponse)
{
    CSFML_CHECK_RETURN(HttpResponse, sfHttpInvalidResponse);

    return static_cast<sfHttpStatus>(HttpResponse->This.GetStatus());
}


////////////////////////////////////////////////////////////
/// Get the major HTTP version of a response
////////////////////////////////////////////////////////////
unsigned int sfHttpResponse_GetMajorVersion(sfHttpResponse* HttpResponse)
{
    CSFML_CALL_RETURN(HttpResponse, GetMajorHttpVersion(), 0);
}


////////////////////////////////////////////////////////////
/// Get the minor HTTP version of a response
////////////////////////////////////////////////////////////
unsigned int sfHttpResponse_GetMinorVersion(sfHttpResponse* HttpResponse)
{
    CSFML_CALL_RETURN(HttpResponse, GetMinorHttpVersion(), 0);
}


////////////////////////////////////////////////////////////
/// Get the body of the response. The body can contain :
/// - the requested page (for GET requests)
/// - a response from the server (for POST requests)
/// - nothing (for HEAD requests)
/// - an error message (in case of an error)
////////////////////////////////////////////////////////////
const char* sfHttpResponse_GetBody(sfHttpResponse* HttpResponse)
{
    CSFML_CHECK_RETURN(HttpResponse, NULL);

    return HttpResponse->This.GetBody().c_str();
}


struct sfHttp
{
    sf::Http This;
};


////////////////////////////////////////////////////////////
/// Construct a new Http object
////////////////////////////////////////////////////////////
sfHttp* sfHttp_Create()
{
    return new sfHttp;
}


////////////////////////////////////////////////////////////
/// Destroy an existing Http object
////////////////////////////////////////////////////////////
void sfHttp_Destroy(sfHttp* Http)
{
    delete Http;
}


////////////////////////////////////////////////////////////
/// Set the target host of a Http server
////////////////////////////////////////////////////////////
void sfHttp_SetHost(sfHttp* Http, const char* Host, unsigned short Port)
{
    CSFML_CALL(Http, SetHost(Host ? Host : "", Port));
}


////////////////////////////////////////////////////////////
/// Send a HTTP request and return the server's response.
/// You must be connected to a host before sending requests.
/// Any missing mandatory header field will be added with an appropriate value.
/// Warning : this function waits for the server's response and may
/// not return instantly; use a thread if you don't want to block your
/// application.
////////////////////////////////////////////////////////////
sfHttpResponse* sfHttp_SendRequest(sfHttp* Http, sfHttpRequest* Request, float Timeout)
{
    CSFML_CHECK_RETURN(Http,    NULL);
    CSFML_CHECK_RETURN(Request, NULL);

    sfHttpResponse* Response = new sfHttpResponse;
    Response->This = Http->This.SendRequest(Request->This, Timeout);

    return Response;
}
