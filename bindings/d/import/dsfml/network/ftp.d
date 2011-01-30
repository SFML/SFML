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

module dsfml.network.ftp;

import dsfml.system.common;
import dsfml.system.stringutil;

import dsfml.network.ipaddress;


/**
*	Enumeration of transfer mode
*/
enum FtpTransferMode
{
	BINARY, ///< Binary mode (file is transfered as a sequence of bytes)
	ASCII,  ///< Text mode using ASCII encoding
	EBCDIC  ///< Text mode using EBCDIC encoding
}

/**
*	Enumerate all the valid status codes returned in
*	a FTP response
*/
enum FtpStatus
{
	// 1xx: the requested action is being initiated,
	// expect another reply before proceeding with a new command
	RESTARTMARKERREPLY		  = 110, ///< Restart marker reply
	SERVICEREADYSOON			= 120, ///< Service ready in N minutes
	DATACONNECTIONALREADYOPENED = 125, ///< Data connection already opened, transfer starting
	OPENINGDATACONNECTION		= 150, ///< File status ok, about to open data connection

	// 2xx: the requested action has been successfully completed
	OK					= 200, ///< Command ok
	POINTLESSCOMMAND	  = 202, ///< Command not implemented
	SYSTEMSTATUS		  = 211, ///< System status, or system help reply
	DIRECTORYSTATUS		= 212, ///< Directory status
	FILESTATUS			= 213, ///< File status
	HELPMESSAGE			= 214, ///< Help message
	SYSTEMTYPE			= 215, ///< NAME system type, where NAME is an official system name from the list in the Assigned Numbers document
	SERVICEREADY		  = 220, ///< Service ready for new user
	CLOSINGCONNECTION	 = 221, ///< Service closing control connection
	DATACONNECTIONOPENED  = 225, ///< Data connection open, no transfer in progress
	CLOSINGDATACONNECTION = 226, ///< Closing data connection, requested file action successful
	ENTERINGPASSIVEMODE	= 227, ///< Entering passive mode
	LOGGEDIN			  = 230, ///< User logged in, proceed. Logged out if appropriate
	FILEACTIONOK		  = 250, ///< Requested file action ok
	DIRECTORYOK			= 257, ///< PATHNAME created

	// 3xx: the command has been accepted, but the requested action
	// is dormant, pending receipt of further information
	NEEDPASSWORD		= 331, ///< User name ok, need password
	NEEDACCOUNTTOLOGIN = 332, ///< Need account for login
	NEEDINFORMATION	= 350, ///< Requested file action pending further information

	// 4xx: the command was not accepted and the requested action did not take place,
	// but the error condition is temporary and the action may be requested again
	SERVICEUNAVAILABLE		= 421, ///< Service not available, closing control connection
	DATACONNECTIONUNAVAILABLE = 425, ///< Can't open data connection
	TRANSFERABORTED			= 426, ///< Connection closed, transfer aborted
	FILEACTIONABORTED		 = 450, ///< Requested file action not taken
	LOCALERROR				= 451, ///< Requested action aborted, local error in processing
	INSUFFICIENTSTORAGESPACE  = 452, ///< Requested action not taken; insufficient storage space in system, file unavailable

	// 5xx: the command was not accepted and
	// the requested action did not take place
	COMMANDUNKNOWN		  = 500, ///< Syntax error, command unrecognized
	PARAMETERSUNKNOWN		= 501, ///< Syntax error in parameters or arguments
	COMMANDNOTIMPLEMENTED	= 502, ///< Command not implemented
	BADCOMMANDSEQUENCE	  = 503, ///< Bad sequence of commands
	PARAMETERNOTIMPLEMENTED = 504, ///< Command not implemented for that parameter
	NOTLOGGEDIN			 = 530, ///< Not logged in
	NEEDACCOUNTTOSTORE	  = 532, ///< Need account for storing files
	FILEUNAVAILABLE		 = 550, ///< Requested action not taken, file unavailable
	PAGETYPEUNKNOWN		 = 551, ///< Requested action aborted, page type unknown
	NOTENOUGHMEMORY		 = 552, ///< Requested file action aborted, exceeded storage allocation
	FILENAMENOTALLOWED	  = 553, ///< Requested action not taken, file name not allowed

	// 10xx: SFML custom codes
	INVALIDRESPONSE  = 1000, ///< Response is not a valid FTP one
	CONNECTIONFAILED = 1001, ///< Connection with server failed
	CONNECTIONCLOSED = 1002, ///< Connection with server closed
	INVALIDFILE	  = 1003  ///< Invalid file to upload / download
}


/**
*	This class provides methods for manipulating the FTP protocol (described in RFC 959).
*	It provides easy access and transfers to remote directories and files on a FTP server.
*/
class Ftp : DSFMLObject
{
	/**
	*	This class wraps a FTP response, which is basically :
	*		- a status code
	*		- a message
	*/
	static class FtpResponse : DSFMLObject
	{
		override void dispose()
		{
			sfFtpResponse_Destroy(m_ptr);
		}
	
		/**
		*	Convenience function to check if the response status code
		*	means a success
		*
		*	Returns:
		*		True if status is success (code < 400)
		*/
		bool isOk()
		{
			return cast(bool)sfFtpResponse_IsOk(m_ptr);
		}
	
		/**
		*	Get the response status code
		*
		*	Returns:
		*		Status code
		*/
		FtpStatus getStatus()
		{
			return sfFtpResponse_GetStatus(m_ptr);
		}
	
		/**
		*	Get the full message contained in the response
		*
		*	Returns: 
		*		The response message	 
		*/		
		string getMessage()
		{
			return fromStringz(sfFtpResponse_GetMessage(m_ptr));
		}
	
	private:
		this(SFMLClass ptr)
		{
			super(ptr);
		}
	// External ================================================================
		
		extern (C)
		{
			typedef void function(SFMLClass) pf_sfFtpResponse_Destroy;
			typedef int function(SFMLClass) pf_sfFtpResponse_IsOk;
			typedef FtpStatus function(SFMLClass) pf_sfFtpResponse_GetStatus;
			typedef ichar* function(SFMLClass) pf_sfFtpResponse_GetMessage;
			
			static pf_sfFtpResponse_Destroy sfFtpResponse_Destroy;
			static pf_sfFtpResponse_IsOk sfFtpResponse_IsOk;
			static pf_sfFtpResponse_GetStatus sfFtpResponse_GetStatus;
			static pf_sfFtpResponse_GetMessage sfFtpResponse_GetMessage;
		}
		static this()
		{
			DllLoader dll = DllLoader.load("csfml-network-2");
			
			sfFtpResponse_Destroy = cast(pf_sfFtpResponse_Destroy)dll.getSymbol("sfFtpResponse_Destroy");
			sfFtpResponse_IsOk = cast(pf_sfFtpResponse_IsOk)dll.getSymbol("sfFtpResponse_IsOk");
			sfFtpResponse_GetStatus = cast(pf_sfFtpResponse_GetStatus)dll.getSymbol("sfFtpResponse_GetStatus");
			sfFtpResponse_GetMessage = cast(pf_sfFtpResponse_GetMessage)dll.getSymbol("sfFtpResponse_GetMessage");
		}
	}

	/**
	*	Specialization of FTP response returning a directory
	*/
	static class FtpDirectoryResponse : FtpResponse
	{
		override void dispose()
		{
			sfFtpDirectoryResponse_Destroy(m_ptr);
		}
	
		/**
		*	Get the directory returned in the response.
		*	
		*	Returns:
		*		Directory name						
		*/				
		string getDirectory()
		{
			return fromStringz(sfFtpDirectoryResponse_GetDirectory(m_ptr));
		}
	
	private:
		this(SFMLClass ptr)
		{
			super(ptr);
		}
	// External ================================================================
		
		extern (C)
		{
			typedef void function(SFMLClass) pf_sfFtpDirectoryResponse_Destroy;
			typedef ichar* function(SFMLClass) pf_sfFtpDirectoryResponse_GetDirectory;
			
			static pf_sfFtpDirectoryResponse_Destroy sfFtpDirectoryResponse_Destroy;
			static pf_sfFtpDirectoryResponse_GetDirectory sfFtpDirectoryResponse_GetDirectory;
		}
		static this()
		{
			DllLoader dll = DllLoader.load("csfml-network-2");
			
			sfFtpDirectoryResponse_Destroy = cast(pf_sfFtpDirectoryResponse_Destroy)dll.getSymbol("sfFtpDirectoryResponse_Destroy");
			sfFtpDirectoryResponse_GetDirectory = cast(pf_sfFtpDirectoryResponse_GetDirectory)dll.getSymbol("sfFtpDirectoryResponse_GetDirectory");
		}

	}

	/**
	*	Specialization of FTP response returning a filename listing.
	*/ 
	static class FtpListingResponse : FtpResponse
	{
		override void dispose()
		{
			sfFtpListingResponse_Destroy(m_ptr);
		}
	
		/**
		*	Get the number of files in the listing
		*	
		*	Returns:
		*		Total number of files		
		*/
		size_t getCount()
		{
			return sfFtpListingResponse_GetCount(m_ptr);
		}
	
		/**
		*	Get the index-th filename in the directory
		*	
		*	Params:
		*		index = Index of the filename to get
		*		
		*	Returns:
		*		Filename												
		*/				
		string opIndex(size_t index)
		{
			return fromStringz(sfFtpListingResponse_GetFilename(m_ptr, index));
		}
	
		/**
		*	Foreach implementation 
		*/				
		int opApply(int delegate(string) dg)
		{
			size_t count = getCount();
			int result;
			
			for(int i = 0; i < count; i++)
			{
				result = dg(this[i]);
				if (result)
					break;
			}
		
			return result;
		}
	
	private:
		this(SFMLClass ptr)
		{
			super(ptr);
		}
	
	// External ================================================================
		extern (C)
		{
			typedef void function(SFMLClass) pf_sfFtpListingResponse_Destroy;
			typedef size_t function(SFMLClass) pf_sfFtpListingResponse_GetCount;
			typedef ichar* function(SFMLClass, size_t) pf_sfFtpListingResponse_GetFilename;
			
			static pf_sfFtpListingResponse_Destroy sfFtpListingResponse_Destroy;
			static pf_sfFtpListingResponse_GetCount sfFtpListingResponse_GetCount;
			static pf_sfFtpListingResponse_GetFilename sfFtpListingResponse_GetFilename;
		}
		static this()
		{
			DllLoader dll = DllLoader.load("csfml-network-2");
			
			sfFtpListingResponse_Destroy = cast(pf_sfFtpListingResponse_Destroy)dll.getSymbol("sfFtpListingResponse_Destroy");
			sfFtpListingResponse_GetCount = cast(pf_sfFtpListingResponse_GetCount)dll.getSymbol("sfFtpListingResponse_GetCount");
			sfFtpListingResponse_GetFilename = cast(pf_sfFtpListingResponse_GetFilename)dll.getSymbol("sfFtpListingResponse_GetFilename");
		}
	}
	
	/**
	*	Default constructor
	*/		
	this()
	{
		super(sfFtp_Create());
	}
	
	override void dispose()
	{
		sfFtp_Destroy(m_ptr);
	}

	/**
	*	Connect to the specified FTP server
	*
	*	Params:
	*		server = FTP server to connect to
	*		port = Port used for connection (21 by default, standard FTP port)
	*		timeout = Maximum time to wait, in seconds (0 by default, means no timeout)
	*
	*	Returns:
	*		Server response to the request
	*/	
	FtpResponse connect(IPAddress server, ushort port = 21, float timeout = 0.f)
	{
		return new FtpResponse(sfFtp_Connect(m_ptr, server, port, timeout));
	}
	
	/**
	*	Log in using anonymous account
	*
	*	Returns:
	*		Server response to the request
	*/
	FtpResponse login()
	{
		return new FtpResponse(sfFtp_LoginAnonymous(m_ptr));
	}

	/**
	*	Log in using a username and a password
	*	
	*	Params:
	*		username = User name
	*		password = password
	*		
	*	Returns:
	*		Server response to the request							
	*/		
	FtpResponse login(string username, string password)
	{
		return new FtpResponse(sfFtp_Login(m_ptr, toStringz(username), toStringz(password)));
	}
	
	/**
	*	Close the connection with FTP server
	*	
	*	Returns:
	*		Server response to the request			
	*/		
	FtpResponse disconnect()
	{
		return new FtpResponse(sfFtp_Disconnect(m_ptr));
	}

	/**
	*	Send a null command to prevent from being disconnected.
	*	
	*	Returns:
	*		Server response to the request			
	*/		
	FtpResponse keepAlive()
	{
		return new FtpResponse(sfFtp_KeepAlive(m_ptr));
	}

	/**
	*	Get the current working directory
	*	
	*	Returns:
	*		Server response to the request			
	*/		
	FtpDirectoryResponse getWorkingDirectory()
	{
		return new FtpDirectoryResponse(sfFtp_GetWorkingDirectory(m_ptr));
	}

	/**
	*	Get the content of the given directory (subdirectories and files).
	*	
	*	Params:
	*		directory = directory to list (null by default, the current one)
	*		
	*	Returns:
	*		Server response to the request						
	*/		
	FtpListingResponse getDirectoryListing(string directory = null)
	{
		return new FtpListingResponse(sfFtp_GetDirectoryListing(m_ptr, toStringz(directory)));
	}

	/**
	*	Change the current working directory
	*	
	*	Params:
	*		directory = New directory, relative to the current one.
	*		
	*	Returns:
	*		Server response to the request						
	*/		
	FtpResponse changeDirectory(string directory)
	{
		return new FtpResponse(sfFtp_ChangeDirectory(m_ptr, toStringz(directory)));
	}

	/**
	*	Go to the parent directory of the current one
	*	
	*	Returns:
	*		Server response to the request			
	*/		
	FtpResponse parentDirectory()
	{
		return new FtpResponse(sfFtp_ParentDirectory(m_ptr));
	}

	/**
	*	Create a new directory
	*	
	*	Params:
	*		name = name of the directory to create
	*		
	*	Returns:
	*		Server response to the request						
	*/		
	FtpResponse createDirectory(string name)
	{
		return new FtpResponse(sfFtp_CreateDirectory(m_ptr, toStringz(name)));
	}

	/**
	*	remove an existing directory
	*	
	*	Params:
	*		name = name of the directory to remove
	*		
	*	Returns:
	*		Server response to the request						
	*/		
	FtpResponse deleteDirectory(string name)
	{
		return new FtpResponse(sfFtp_DeleteDirectory(m_ptr, toStringz(name)));
	}

	/**
	*	Rename a file
	*	
	*	Params:
	*		name = file to rename
	*		newName = new name
	*		
	*	Returns:
	*		Server response to the request							
	*/		
	FtpResponse renameFile(string name, string newName)
	{
		return new FtpResponse(sfFtp_RenameFile(m_ptr, toStringz(name), toStringz(newName)));
	}

	/**
	*	Remove an existing file
	*	
	*	Params:
	*		name = file to remove
	*		
	*	Returns:
	*		Server response to the request						
	*/		
	FtpResponse deleteFile(string name)
	{
		return new FtpResponse(sfFtp_DeleteFile(m_ptr, toStringz(name)));
	}

	/**
	*	Download a file from the server
	*	
	*	Params:
	*		distantFile = Path of the distant file to download
	*		destFile = Where to put the file on the local computer
	*		mode = transfer mode (binary by default)
	*		
	*	Returns:
	*		Server response to the request					 
	*/		
	FtpResponse download(string distantFile, string destFile, FtpTransferMode mode = FtpTransferMode.BINARY)
	{
		return new FtpResponse(sfFtp_Download(m_ptr, toStringz(distantFile), toStringz(destFile), mode));
	}

	/**
	*	Upload a file to the server
	*	
	*	Params:
	*		localFile = Path of the local file to upload
	*		destPath = Where to put the file on the server
	*		mode = transfer mode (binary by default)
	*	Returns:
	*		Server response to the request					 
	*/
	FtpResponse upload(string localFile, string destFile, FtpTransferMode mode = FtpTransferMode.BINARY)
	{
		return new FtpResponse(sfFtp_Upload(m_ptr, toStringz(localFile), toStringz(destFile), mode));
	}

private:

// External ====================================================================
	
	extern (C)
	{
		typedef SFMLClass function() pf_sfFtp_Create;
		typedef void function(SFMLClass) pf_sfFtp_Destroy;
		typedef SFMLClass function(SFMLClass, IPAddress, ushort, float) pf_sfFtp_Connect;
		typedef SFMLClass function(SFMLClass) pf_sfFtp_LoginAnonymous;
		typedef SFMLClass function(SFMLClass, cchar*, cchar*) pf_sfFtp_Login;
		typedef SFMLClass function(SFMLClass) pf_sfFtp_Disconnect;
		typedef SFMLClass function(SFMLClass) pf_sfFtp_KeepAlive;
		typedef SFMLClass function(SFMLClass) pf_sfFtp_GetWorkingDirectory;
		typedef SFMLClass function(SFMLClass, cchar*) pf_sfFtp_GetDirectoryListing;
		typedef SFMLClass function(SFMLClass, cchar*) pf_sfFtp_ChangeDirectory;
		typedef SFMLClass function(SFMLClass) pf_sfFtp_ParentDirectory;
		typedef SFMLClass function(SFMLClass, cchar*) pf_sfFtp_CreateDirectory;
		typedef SFMLClass function(SFMLClass, cchar*) pf_sfFtp_DeleteDirectory;
		typedef SFMLClass function(SFMLClass, cchar*, cchar*) pf_sfFtp_RenameFile;
		typedef SFMLClass function(SFMLClass, cchar*) pf_sfFtp_DeleteFile;
		typedef SFMLClass function(SFMLClass, cchar*, cchar*, FtpTransferMode) pf_sfFtp_Download;
		typedef SFMLClass function(SFMLClass, cchar*, cchar*, FtpTransferMode) pf_sfFtp_Upload;
		
		static pf_sfFtp_Create sfFtp_Create;
		static pf_sfFtp_Destroy sfFtp_Destroy;
		static pf_sfFtp_Connect sfFtp_Connect;
		static pf_sfFtp_LoginAnonymous sfFtp_LoginAnonymous;
		static pf_sfFtp_Login sfFtp_Login;
		static pf_sfFtp_Disconnect sfFtp_Disconnect;
		static pf_sfFtp_KeepAlive sfFtp_KeepAlive;
		static pf_sfFtp_GetWorkingDirectory sfFtp_GetWorkingDirectory;
		static pf_sfFtp_GetDirectoryListing sfFtp_GetDirectoryListing;
		static pf_sfFtp_ChangeDirectory sfFtp_ChangeDirectory;
		static pf_sfFtp_ParentDirectory sfFtp_ParentDirectory;
		static pf_sfFtp_CreateDirectory sfFtp_CreateDirectory;
		static pf_sfFtp_DeleteDirectory sfFtp_DeleteDirectory;
		static pf_sfFtp_RenameFile sfFtp_RenameFile;
		static pf_sfFtp_DeleteFile sfFtp_DeleteFile;
		static pf_sfFtp_Download sfFtp_Download;
		static pf_sfFtp_Upload sfFtp_Upload;
	}
	static this()
	{
	debug
		DllLoader dll = DllLoader.load("csfml-network-d-2");
	else
		DllLoader dll = DllLoader.load("csfml-network-2");
		
		sfFtp_Create = cast(pf_sfFtp_Create)dll.getSymbol("sfFtp_Create");
		sfFtp_Destroy = cast(pf_sfFtp_Destroy)dll.getSymbol("sfFtp_Destroy");
		sfFtp_Connect = cast(pf_sfFtp_Connect)dll.getSymbol("sfFtp_Connect");
		sfFtp_LoginAnonymous = cast(pf_sfFtp_LoginAnonymous)dll.getSymbol("sfFtp_LoginAnonymous");
		sfFtp_Login = cast(pf_sfFtp_Login)dll.getSymbol("sfFtp_Login");
		sfFtp_Disconnect = cast(pf_sfFtp_Disconnect)dll.getSymbol("sfFtp_Disconnect");
		sfFtp_KeepAlive = cast(pf_sfFtp_KeepAlive)dll.getSymbol("sfFtp_KeepAlive");
		sfFtp_GetWorkingDirectory = cast(pf_sfFtp_GetWorkingDirectory)dll.getSymbol("sfFtp_GetWorkingDirectory");
		sfFtp_GetDirectoryListing = cast(pf_sfFtp_GetDirectoryListing)dll.getSymbol("sfFtp_GetDirectoryListing");
		sfFtp_ChangeDirectory = cast(pf_sfFtp_ChangeDirectory)dll.getSymbol("sfFtp_ChangeDirectory");
		sfFtp_ParentDirectory = cast(pf_sfFtp_ParentDirectory)dll.getSymbol("sfFtp_ParentDirectory");
		sfFtp_sfFtp_CreateDirectoryDirectory = cast(pf_sfFtp_CreateDirectory)dll.getSymbol("sfFtp_CreateDirectory");
		sfFtp_DeleteDirectory = cast(pf_sfFtp_DeleteDirectory)dll.getSymbol("sfFtp_DeleteDirectory");
		sfFtp_RenameFile = cast(pf_sfFtp_RenameFile)dll.getSymbol("sfFtp_RenameFile");
		sfFtp_DeleteFile = cast(pf_sfFtp_DeleteFile)dll.getSymbol("sfFtp_DeleteFile");
		sfFtp_Download = cast(pf_sfFtp_Download)dll.getSymbol("sfFtp_Download");
		sfFtp_Upload = cast(pf_sfFtp_Upload)dll.getSymbol("sfFtp_Upload");	
	}	
}		
