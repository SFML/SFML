/*
*	DSFML - SFML Library wrapper for the D programming language.
*	Copyright (C) 2008 Julien Dagorn (sirjulio13@gmail.com)
*	Copyright (C) 2010 Andreas Hollandt
*
*	This software is provided 'as-is', without any express or
*	implied warranty. In no event will the authors be held
*	liable for any damages arising from the use of this software.
*
*	Permission is granted to anyone to use this software for any purpose,
*	including commercial applications, and to alter it and redistribute
*	it freely, subject to the following restrictions:
*
*	1.  The origin of this software must not be misrepresented;
*		you must not claim that you wrote the original software.
*		If you use this software in a product, an acknowledgment
*		in the product documentation would be appreciated but
*		is not required.
*
*	2.  Altered source versions must be plainly marked as such,
*		and must not be misrepresented as being the original software.
*
*	3.  This notice may not be removed or altered from any
*		source distribution.
*/

module dsfml.network.http;

import dsfml.system.stringutil;
import dsfml.system.common;

/**
*	HTTP methods enumeration  
*/
enum HttpMethod
{
	GET,  ///< Request in get mode, standard method to retrieve a page
	POST, ///< Request in post mode, usually to send data to a page
	HEAD  ///< Request a page's header only
}

/**
*	HTTP response status code	 
*/
enum HttpStatus
{
	// 2xx: success
	OK		= 200, ///< Most common code returned when operation was successful
	CREATED	= 201, ///< The resource has successfully been created
	ACCEPTED  = 202, ///< The request has been accepted, but will be processed later by the server
	NOCONTENT = 204, ///< Sent when the server didn't send any data in return

	// 3xx: redirection
	MULTIPLECHOICES  = 300, ///< The requested page can be accessed from several locations
	MOVEDPERMANENTLY = 301, ///< The requested page has permanently moved to a new location
	MOVEDTEMPORARILY = 302, ///< The requested page has temporarily moved to a new location
	NOTMODIFIED	  = 304, ///< For conditionnal requests, means the requested page hasn't changed and doesn't need to be refreshed

	// 4xx: client error
	BADREQUEST	= 400, ///< The server couldn't understand the request (syntax error)
	UNAUTHORIZED = 401, ///< The requested page needs an authentification to be accessed
	FORBIDDEN	= 403, ///< The requested page cannot be accessed at all, even with authentification
	NOTFOUND	 = 404, ///< The requested page doesn't exist

	// 5xx: server error
	INTERNALSERVERERROR = 500, ///< The server encountered an unexpected error
	NOTIMPLEMENTED	  = 501, ///< The server doesn't implement a requested feature
	BADGATEWAY		  = 502, ///< The gateway server has received an error from the source server
	SERVICENOTAVAILABLE = 503, ///< The server is temporarily unavailable (overloaded, in maintenance, ...)

	// 10xx: SFML custom codes
	INVALIDRESPONSE  = 1000, ///< Response is not a valid HTTP one
	CONNECTIONFAILED = 1001  ///< Connection with server failed
}

/**
*	This class provides methods for manipulating the HTTP protocol (described in
*	RFC 1945).
*	It can connect to a website, get files, send requests
*/ 
class Http : DSFMLObject
{
	/**
	*	Wrapper for a http request, which is basically :
	*		- a header with a method, a target URI and a set of field/value pairs	
	*		- an optional body (for POST requests)		  
	*/ 
	static class Response : DSFMLObject
	{
		override void dispose()
		{
			sfHttpResponse_Destroy(m_ptr);
		}
		
		/**
		*	Get the value of a field
		*	
		*	Params:
		*		field = Name of the field to get (case-insensitive)		
		*	Returns:
		*		Value of the field, or enpty string if not found		
		*/
		string getField(string field)
		{
			return fromStringz(sfHttpResponse_GetField(m_ptr, toStringz(field)));
		}
		
		/**
		*	Get the header status code
		*	
		*	Returns:
		*		header status code		
		*/		
		HttpStatus getStatus()
		{
			return sfHttpResponse_GetStatus(m_ptr);
		}
	
		/**
		*	Get the major HTTP version number of the response
		*	
		*	Returns:
		*		Major version number						
		*/
		uint getMajorHTTPVersion()
		{
			return sfHttpResponse_GetMajorVersion(m_ptr);
		}
		
		/**
		*	Get the minor HTTP version number of the response
		*	
		*	Returns:
		*		Minor version number		
		*/
		uint getMinorHTTPVersion()
		{
			return sfHttpResponse_GetMinorVersion(m_ptr);
		}
		
		/**
		*	Get the body of the response. The body can contain :
		*		- the requested page (for GET requests)
		*		- a response from the server (for POST requests)
		*		- nothing (for HEAD requests)
		*		- an error message (in case of an error)
		*		
		*	Returns:
		*		the response body						
		*/
		string getBody()
		{
			return fromStringz(sfHttpResponse_GetBody(m_ptr));
		}
			 
	private:
		this(SFMLClass ptr)
		{
			super(ptr);
		}
	// External ================================================================
		extern (C)
		{
			typedef void function(SFMLClass) pf_sfHttpResponse_Destroy; 
			typedef ichar* function(SFMLClass, cchar*) pf_sfHttpResponse_GetField;
			typedef HttpStatus function(SFMLClass) pf_sfHttpResponse_GetStatus; 
			typedef uint function(SFMLClass) pf_sfHttpResponse_GetMajorVersion;
			typedef uint function(SFMLClass) pf_sfHttpResponse_GetMinorVersion;
			typedef ichar* function(SFMLClass) pf_sfHttpResponse_GetBody;

			static pf_sfHttpResponse_Destroy sfHttpResponse_Destroy; 
			static pf_sfHttpResponse_GetField sfHttpResponse_GetField;
			static pf_sfHttpResponse_GetStatus sfHttpResponse_GetStatus; 
			static pf_sfHttpResponse_GetMajorVersion sfHttpResponse_GetMajorVersion;
			static pf_sfHttpResponse_GetMinorVersion sfHttpResponse_GetMinorVersion;
			static pf_sfHttpResponse_GetBody sfHttpResponse_GetBody;
		}
	
		static this()
		{
			DllLoader dll = DllLoader.load("csfml-network-2");
			
			sfHttpResponse_Destroy = cast(pf_sfHttpResponse_Destroy)dll.getSymbol("sfHttpResponse_Destroy"); 
			sfHttpResponse_GetField = cast(pf_sfHttpResponse_GetField)dll.getSymbol("sfHttpResponse_GetField");
			sfHttpResponse_GetStatus = cast(pf_sfHttpResponse_GetStatus)dll.getSymbol("sfHttpResponse_GetStatus"); 
			sfHttpResponse_GetMajorVersion = cast(pf_sfHttpResponse_GetMajorVersion)dll.getSymbol("sfHttpResponse_GetMajorVersion");
			sfHttpResponse_GetMinorVersion = cast(pf_sfHttpResponse_GetMinorVersion)dll.getSymbol("sfHttpResponse_GetMinorVersion");
			sfHttpResponse_GetBody = cast(pf_sfHttpResponse_GetBody)dll.getSymbol("sfHttpResponse_GetBody");
		}
	}

	/**
	*	Wrapper for a HTTP response which is basically :
	*		- a header with a status code and a set of field/value pairs
	*		- a body (the content of the requested resource)					
	*/ 
	static class Request : DSFMLObject
	{
		/**
		*	Constructor
		*	
		*	Params:
		*		requestMethod = Method to use for the request (Get by default)
		*		uri = Target URI ("/" by default -- index page)
		*		requestBody = Content of the request's body (empty by default)					  
		*/
		this(HttpMethod requestMethod = HttpMethod.GET, string uri = "/", string requestBody = "")
		{
			super(sfHttpRequest_Create());
			sfHttpRequest_SetMethod(m_ptr, requestMethod);
			sfHttpRequest_SetUri(m_ptr, toStringz(uri));
			sfHttpRequest_SetBody(m_ptr, toStringz(requestBody));
		}
		
		/**
		*	Set the value of a field. Field is created if it doesn't exists.																						 
		*	
		*	Params:
		*		field = name of the field to set (case-insensitive)
		*		value = value of the field
		*/
		void setField(string field, string value)
		{
			sfHttpRequest_SetField(m_ptr, toStringz(field), toStringz(value));
		}
	
		/**
		*	Set the request method.
		*	
		*	Params:
		*		requestMethod = Method to use for the request.		
		*/
		void setMethod(HttpMethod requestMethod)
		{
			sfHttpRequest_SetMethod(m_ptr, requestMethod);
		}
	
		/**
		*	Set the target URI of the request.
		*	
		*	Params:
		*		uri = URI to request, local to the host.
		*	Returns:
		*/
		void setUri(string uri)
		{
			sfHttpRequest_SetUri(m_ptr, toStringz(uri));
		}
	
		/**
		*	Set the HTTP version of the request.
		*	
		*	Params:
		*		major = Major version number
		*		minor = Minor version number				
		*/
		void setHttpVersion(uint major, uint minor)
		{
			sfHttpRequest_SetHttpVersion(m_ptr, major, minor);
		}
		
		/**
		*	Set the body of the request. This parameter is optionnal and make sense
		*	only for POST requests.		
		*	
		*	Params:
		*		requestBody = Content of the request body.		
		*/
		void setBody(string requestBody)
		{
			sfHttpRequest_SetBody(m_ptr, toStringz(requestBody));
		}
	
	private:
	
	// External ================================================================
		extern (C)
		{
			typedef SFMLClass function() pf_sfHttpRequest_Create;
			typedef void function(SFMLClass) pf_sfHttpRequest_Destroy;
			typedef void function(SFMLClass, cchar*, cchar*) pf_sfHttpRequest_SetField;
			typedef void function(SFMLClass, HttpMethod) pf_sfHttpRequest_SetMethod;
			typedef void function(SFMLClass, cchar*) pf_sfHttpRequest_SetUri;
			typedef void function(SFMLClass, uint, uint) pf_sfHttpRequest_SetHttpVersion;
			typedef void function(SFMLClass, cchar*) pf_sfHttpRequest_SetBody;
			
			static pf_sfHttpRequest_Create sfHttpRequest_Create;
			static pf_sfHttpRequest_Destroy sfHttpRequest_Destroy;
			static pf_sfHttpRequest_SetField sfHttpRequest_SetField;
			static pf_sfHttpRequest_SetMethod sfHttpRequest_SetMethod;
			static pf_sfHttpRequest_SetUri sfHttpRequest_SetUri;
			static pf_sfHttpRequest_SetHttpVersion sfHttpRequest_SetHttpVersion;
			static pf_sfHttpRequest_SetBody sfHttpRequest_SetBody;
		}
	
		static this()
		{
			DllLoader dll = DllLoader.load("csfml-network-2");
			
			sfHttpRequest_Create = cast(pf_sfHttpRequest_Create)dll.getSymbol("sfHttpRequest_Create");
			sfHttpRequest_Destroy = cast(pf_sfHttpRequest_Destroy)dll.getSymbol("sfHttpRequest_Destroy");
			sfHttpRequest_SetField = cast(pf_sfHttpRequest_SetField)dll.getSymbol("sfHttpRequest_SetField");
			sfHttpRequest_SetMethod = cast(pf_sfHttpRequest_SetMethod)dll.getSymbol("sfHttpRequest_SetMethod");
			sfHttpRequest_SetUri = cast(pf_sfHttpRequest_SetUri)dll.getSymbol("sfHttpRequest_SetUri");
			sfHttpRequest_SetHttpVersion = cast(pf_sfHttpRequest_SetHttpVersion)dll.getSymbol("sfHttpRequest_SetHttpVersion");
			sfHttpRequest_SetBody = cast(pf_sfHttpRequest_SetBody)dll.getSymbol("sfHttpRequest_SetBody");
		}
	}
	
	/**
	*	Constructor
	*/
	this()
	{
		super(sfHttp_Create());
	}

	/**
	*	Constructor
	*	
	*	Params:
	*		host = Web server to connect to
	*		port = port to use for connection (0 by default -- use the standard port of the protocol)		
	*/
	this(string host, ushort port = 0)
	{
		super(sfHttp_Create());
		sfHttp_SetHost(m_ptr, toStringz(host), port);
	}

	override void dispose()
	{
		sfHttp_Destroy(m_ptr);
	}
	
	/**
	*	Set the target host.
	*	
	*	Params:
	*		host = Web server to connect to
	*		port = port to use for connection (0 by default -- use the standard port of the protocol)					
	*/
	void setHost(string host, ushort port = 0)
	{
		sfHttp_SetHost(m_ptr, toStringz(host), port);
	}
	
	/**
	*	Send a HTTP request and return the server's response.
	*	You must be connected to a host before sending requests.
	*	Any missing mandatory header field will be added with an appropriate value.
	*		
	*	Warning : this function waits for the server's response and may
	*	not return instantly; use a thread if you don't want to block your
	*	application.
	*
	*	Params:
	*		req = Request to send
	*
	*	Returns:
	*		Server's response
	*/

	Response sendRequest(Request req)
	{
		return new Response( sfHttp_SendRequest(m_ptr, req.nativePointer) ); 
	} 
	  
private:

// External ====================================================================

	extern (C)
	{
		typedef SFMLClass function() pf_sfHttp_Create;
		typedef void function(SFMLClass) pf_sfHttp_Destroy;
		typedef void function(SFMLClass, cchar*, ushort) pf_sfHttp_SetHost;
		typedef SFMLClass function(SFMLClass, SFMLClass) pf_sfHttp_SendRequest;
		
		static pf_sfHttp_Create sfHttp_Create;
		static pf_sfHttp_Destroy sfHttp_Destroy;
		static pf_sfHttp_SetHost sfHttp_SetHost; 
		static pf_sfHttp_SendRequest sfHttp_SendRequest;
	}

	static this()
	{
	debug
		DllLoader dll = DllLoader.load("csfml-network-d-2");
	else
		DllLoader dll = DllLoader.load("csfml-network-2");
		
		sfHttp_Create = cast(pf_sfHttp_Create)dll.getSymbol("sfHttp_Create");
		sfHttp_Destroy = cast(pf_sfHttp_Destroy)dll.getSymbol("sfHttp_Destroy");
		sfHttp_SetHost = cast(pf_sfHttp_SetHost)dll.getSymbol("sfHttp_SetHost"); 
		sfHttp_SendRequest = cast(pf_sfHttp_SendRequest)dll.getSymbol("sfHttp_SendRequest");
	}
}
