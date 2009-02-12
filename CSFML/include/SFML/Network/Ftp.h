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

#ifndef SFML_FTP_H
#define SFML_FTP_H

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Config.h>
#include <SFML/Network/IPAddress.h>
#include <SFML/Network/Types.h>


////////////////////////////////////////////////////////////
/// Enumerate all the FTP file transfer modes
////////////////////////////////////////////////////////////
enum sfFtpTransferMode
{
    sfFtpBinary, ///< Binary mode (file is transfered as a sequence of bytes)
    sfFtpAscii,  ///< Text mode using ASCII encoding
    sfFtpEbcdic  ///< Text mode using EBCDIC encoding
};


////////////////////////////////////////////////////////////
/// Enumerate all the valid status codes returned in
/// a FTP response
////////////////////////////////////////////////////////////
enum sfFtpStatus
{
    // 1xx: the requested action is being initiated,
    // expect another reply before proceeding with a new command
    sfFtpRestartMarkerReply          = 110, ///< Restart marker reply
    sfFtpServiceReadySoon            = 120, ///< Service ready in N minutes
    sfFtpDataConnectionAlreadyOpened = 125, ///< Data connection already opened, transfer starting
    sfFtpOpeningDataConnection       = 150, ///< File status ok, about to open data connection

    // 2xx: the requested action has been successfully completed
    sfFtpOk                    = 200, ///< Command ok
    sfFtpPointlessCommand      = 202, ///< Command not implemented
    sfFtpSystemStatus          = 211, ///< System status, or system help reply
    sfFtpDirectoryStatus       = 212, ///< Directory status
    sfFtpFileStatus            = 213, ///< File status
    sfFtpHelpMessage           = 214, ///< Help message
    sfFtpSystemType            = 215, ///< NAME system type, where NAME is an official system name from the list in the Assigned Numbers document
    sfFtpServiceReady          = 220, ///< Service ready for new user
    sfFtpClosingConnection     = 221, ///< Service closing control connection
    sfFtpDataConnectionOpened  = 225, ///< Data connection open, no transfer in progress
    sfFtpClosingDataConnection = 226, ///< Closing data connection, requested file action successful
    sfFtpEnteringPassiveMode   = 227, ///< Entering passive mode
    sfFtpLoggedIn              = 230, ///< User logged in, proceed. Logged out if appropriate
    sfFtpFileActionOk          = 250, ///< Requested file action ok
    sfFtpDirectoryOk           = 257, ///< PATHNAME created

    // 3xx: the command has been accepted, but the requested action
    // is dormant, pending receipt of further information
    sfFtpNeedPassword       = 331, ///< User name ok, need password
    sfFtpNeedAccountToLogIn = 332, ///< Need account for login
    sfFtpNeedInformation    = 350, ///< Requested file action pending further information

    // 4xx: the command was not accepted and the requested action did not take place,
    // but the error condition is temporary and the action may be requested again
    sfFtpServiceUnavailable        = 421, ///< Service not available, closing control connection
    sfFtpDataConnectionUnavailable = 425, ///< Can't open data connection
    sfFtpTransferAborted           = 426, ///< Connection closed, transfer aborted
    sfFtpFileActionAborted         = 450, ///< Requested file action not taken
    sfFtpLocalError                = 451, ///< Requested action aborted, local error in processing
    sfFtpInsufficientStorageSpace  = 452, ///< Requested action not taken; insufficient storage space in system, file unavailable

    // 5xx: the command was not accepted and
    // the requested action did not take place
    sfFtpCommandUnknown          = 500, ///< Syntax error, command unrecognized
    sfFtpParametersUnknown       = 501, ///< Syntax error in parameters or arguments
    sfFtpCommandNotImplemented   = 502, ///< Command not implemented
    sfFtpBadCommandSequence      = 503, ///< Bad sequence of commands
    sfFtpParameterNotImplemented = 504, ///< Command not implemented for that parameter
    sfFtpNotLoggedIn             = 530, ///< Not logged in
    sfFtpNeedAccountToStore      = 532, ///< Need account for storing files
    sfFtpFileUnavailable         = 550, ///< Requested action not taken, file unavailable
    sfFtpPageTypeUnknown         = 551, ///< Requested action aborted, page type unknown
    sfFtpNotEnoughMemory         = 552, ///< Requested file action aborted, exceeded storage allocation
    sfFtpFilenameNotAllowed      = 553, ///< Requested action not taken, file name not allowed

    // 10xx: SFML custom codes
    sfFtpInvalidResponse  = 1000, ///< Response is not a valid FTP one
    sfFtpConnectionFailed = 1001, ///< Connection with server failed
    sfFtpConnectionClosed = 1002, ///< Connection with server closed
    sfFtpInvalidFile      = 1003  ///< Invalid file to upload / download
};


////////////////////////////////////////////////////////////
/// Destroy an existing Ftp listing response
///
/// \param FtpResponse : Ftp listing response to destroy
///
////////////////////////////////////////////////////////////
CSFML_API void sfFtpListingResponse_Destroy(sfFtpListingResponse* FtpListingResponse);

////////////////////////////////////////////////////////////
/// Convenience function to check if the response status code
/// means a success
///
/// \param FtpListingResponse : Ftp listing response
///
/// \return sfTrue if status is success (code < 400)
///
////////////////////////////////////////////////////////////
CSFML_API sfBool sfFtpListingResponse_IsOk(sfFtpListingResponse* FtpListingResponse);

////////////////////////////////////////////////////////////
/// Get the response status code
///
/// \param FtpListingResponse : Ftp listing response
///
/// \return Status code
///
////////////////////////////////////////////////////////////
CSFML_API sfFtpStatus sfFtpListingResponse_GetStatus(sfFtpListingResponse* FtpListingResponse);

////////////////////////////////////////////////////////////
/// Get the full message contained in the response
///
/// \param FtpListingResponse : Ftp listing response
///
/// \return The response message
///
////////////////////////////////////////////////////////////
CSFML_API const char* sfFtpListingResponse_GetMessage(sfFtpListingResponse* FtpListingResponse);

////////////////////////////////////////////////////////////
/// Get the number of filenames in the listing
///
/// \param FtpListingResponse : Ftp listing response
///
/// \return Total number of filenames
///
////////////////////////////////////////////////////////////
CSFML_API size_t sfFtpListingResponse_GetCount(sfFtpListingResponse* FtpListingResponse);

////////////////////////////////////////////////////////////
/// Get the Index-th filename in the directory
///
/// \param FtpListingResponse : Ftp listing response
/// \param Index :              Index of the filename to get
///
/// \return Index-th filename
///
////////////////////////////////////////////////////////////
CSFML_API const char* sfFtpListingResponse_GetFilename(sfFtpListingResponse* FtpListingResponse, size_t Index);

////////////////////////////////////////////////////////////
/// Destroy an existing Ftp directory response
///
/// \param FtpDirectoryResponse : Ftp directory response to destroy
///
////////////////////////////////////////////////////////////
CSFML_API void sfFtpDirectoryResponse_Destroy(sfFtpDirectoryResponse* FtpDirectoryResponse);

////////////////////////////////////////////////////////////
/// Convenience function to check if the response status code
/// means a success
///
/// \param FtpDirectoryResponse : Ftp directory response
///
/// \return sfTrue if status is success (code < 400)
///
////////////////////////////////////////////////////////////
CSFML_API sfBool sfFtpDirectoryResponse_IsOk(sfFtpDirectoryResponse* FtpDirectoryResponse);

////////////////////////////////////////////////////////////
/// Get the response status code
///
/// \param FtpDirectoryResponse : Ftp directory response
///
/// \return Status code
///
////////////////////////////////////////////////////////////
CSFML_API sfFtpStatus sfFtpDirectoryResponse_GetStatus(sfFtpDirectoryResponse* FtpDirectoryResponse);

////////////////////////////////////////////////////////////
/// Get the full message contained in the response
///
/// \param FtpDirectoryResponse : Ftp directory response
///
/// \return The response message
///
////////////////////////////////////////////////////////////
CSFML_API const char* sfFtpDirectoryResponse_GetMessage(sfFtpDirectoryResponse* FtpDirectoryResponse);

////////////////////////////////////////////////////////////
/// Get the directory returned in the response
///
/// \param FtpDirectoryResponse : Ftp directory response
///
/// \return Directory name
///
////////////////////////////////////////////////////////////
CSFML_API const char* sfFtpDirectoryResponse_GetDirectory(sfFtpDirectoryResponse* FtpDirectoryResponse);


////////////////////////////////////////////////////////////
/// Destroy an existing Ftp response
///
/// \param FtpResponse : Ftp response to destroy
///
////////////////////////////////////////////////////////////
CSFML_API void sfFtpResponse_Destroy(sfFtpResponse* FtpResponse);

////////////////////////////////////////////////////////////
/// Convenience function to check if the response status code
/// means a success
///
/// \param FtpResponse : Ftp response
///
/// \return sfTrue if status is success (code < 400)
///
////////////////////////////////////////////////////////////
CSFML_API sfBool sfFtpResponse_IsOk(sfFtpResponse* FtpResponse);

////////////////////////////////////////////////////////////
/// Get the response status code
///
/// \param FtpResponse : Ftp response
///
/// \return Status code
///
////////////////////////////////////////////////////////////
CSFML_API sfFtpStatus sfFtpResponse_GetStatus(sfFtpResponse* FtpResponse);

////////////////////////////////////////////////////////////
/// Get the full message contained in the response
///
/// \param FtpResponse : Ftp response
///
/// \return The response message
///
////////////////////////////////////////////////////////////
CSFML_API const char* sfFtpResponse_GetMessage(sfFtpResponse* FtpResponse);

////////////////////////////////////////////////////////////
/// Construct a new Ftp
///
/// \return Pointer to the new Ftp
///
////////////////////////////////////////////////////////////
CSFML_API sfFtp* sfFtp_Create();

////////////////////////////////////////////////////////////
/// Destroy an existing Ftp
///
/// \param Ftp : Ftp to destroy
///
////////////////////////////////////////////////////////////
CSFML_API void sfFtp_Destroy(sfFtp* Ftp);

////////////////////////////////////////////////////////////
/// Connect to the specified FTP server
///
/// \param Ftp :     Ftp instance
/// \param Server :  FTP server to connect to
/// \param Port :    Port used for connection (21 by default, standard FTP port)
/// \param Timeout : Maximum time to wait (0 to use no timeout)
///
/// \return Server response to the request
///
////////////////////////////////////////////////////////////
CSFML_API sfFtpResponse* sfFtp_Connect(sfFtp* Ftp, sfIPAddress Server, unsigned short Port, float Timeout);

////////////////////////////////////////////////////////////
/// Log in using anonymous account
///
/// \param Ftp : Ftp instance
///
/// \return Server response to the request
///
////////////////////////////////////////////////////////////
CSFML_API sfFtpResponse* sfFtp_LoginAnonymous(sfFtp* Ftp);

////////////////////////////////////////////////////////////
/// Log in using a username and a password
///
/// \param Ftp :      Ftp instance
/// \param UserName : User name
/// \param Password : Password
///
/// \return Server response to the request
///
////////////////////////////////////////////////////////////
CSFML_API sfFtpResponse* sfFtp_Login(sfFtp* Ftp, const char* UserName, const char* Password);

////////////////////////////////////////////////////////////
/// Close the connection with FTP server
///
/// \param Ftp : Ftp instance
///
/// \return Server response to the request
///
////////////////////////////////////////////////////////////
CSFML_API sfFtpResponse* sfFtp_Disconnect(sfFtp* Ftp);

////////////////////////////////////////////////////////////
/// Send a null command just to prevent from being disconnected
///
/// \param Ftp : Ftp instance
///
/// \return Server response to the request
///
////////////////////////////////////////////////////////////
CSFML_API sfFtpResponse* sfFtp_KeepAlive(sfFtp* Ftp);

////////////////////////////////////////////////////////////
/// Get the current working directory
///
/// \param Ftp : Ftp instance
///
/// \return Server response to the request
///
////////////////////////////////////////////////////////////
CSFML_API sfFtpDirectoryResponse* sfFtp_GetWorkingDirectory(sfFtp* Ftp);

////////////////////////////////////////////////////////////
/// Get the contents of the given directory
/// (subdirectories and files)
///
/// \param Ftp :       Ftp instance
/// \param Directory : Directory to list ("" by default, the current one)
///
/// \return Server response to the request
///
////////////////////////////////////////////////////////////
CSFML_API sfFtpListingResponse* sfFtp_GetDirectoryListing(sfFtp* Ftp, const char* Directory);

////////////////////////////////////////////////////////////
/// Change the current working directory
///
/// \param Ftp :       Ftp instance
/// \param Directory : New directory, relative to the current one
///
/// \return Server response to the request
///
////////////////////////////////////////////////////////////
CSFML_API sfFtpResponse* sfFtp_ChangeDirectory(sfFtp* Ftp, const char* Directory);

////////////////////////////////////////////////////////////
/// Go to the parent directory of the current one
///
/// \param Ftp : Ftp instance
///
/// \return Server response to the request
///
////////////////////////////////////////////////////////////
CSFML_API sfFtpResponse* sfFtp_ParentDirectory(sfFtp* Ftp);

////////////////////////////////////////////////////////////
/// Create a new directory
///
/// \param Ftp :  Ftp instance
/// \param Name : Name of the directory to create
///
/// \return Server response to the request
///
////////////////////////////////////////////////////////////
CSFML_API sfFtpResponse* sfFtp_MakeDirectory(sfFtp* Ftp, const char* Name);

////////////////////////////////////////////////////////////
/// Remove an existing directory
///
/// \param Ftp :  Ftp instance
/// \param Name : Name of the directory to remove
///
/// \return Server response to the request
///
////////////////////////////////////////////////////////////
CSFML_API sfFtpResponse* sfFtp_DeleteDirectory(sfFtp* Ftp, const char* Name);

////////////////////////////////////////////////////////////
/// Rename a file
///
/// \param Ftp :     Ftp instance
/// \param File :    File to rename
/// \param NewName : New name
///
/// \return Server response to the request
///
////////////////////////////////////////////////////////////
CSFML_API sfFtpResponse* sfFtp_RenameFile(sfFtp* Ftp, const char* File, const char* NewName);

////////////////////////////////////////////////////////////
/// Remove an existing file
///
/// \param Ftp :  Ftp instance
/// \param Name : File to remove
///
/// \return Server response to the request
///
////////////////////////////////////////////////////////////
CSFML_API sfFtpResponse* sfFtp_DeleteFile(sfFtp* Ftp, const char* Name);

////////////////////////////////////////////////////////////
/// Download a file from the server
///
/// \param Ftp :         Ftp instance
/// \param DistantFile : Path of the distant file to download
/// \param DestPath :    Where to put to file on the local computer
/// \param Mode :        Transfer mode (binary by default)
///
/// \return Server response to the request
///
////////////////////////////////////////////////////////////
CSFML_API sfFtpResponse* sfFtp_Download(sfFtp* Ftp, const char* DistantFile, const char* DestPath, sfFtpTransferMode Mode);

////////////////////////////////////////////////////////////
/// Upload a file to the server
///
/// \param Ftp :       Ftp instance
/// \param LocalFile : Path of the local file to upload
/// \param DestPath :  Where to put to file on the server
/// \param Mode :      Transfer mode (binary by default)
///
/// \return Server response to the request
///
////////////////////////////////////////////////////////////
CSFML_API sfFtpResponse* sfFtp_Upload(sfFtp* Ftp, const char* LocalFile, const char* DestPath, sfFtpTransferMode Mode);


#endif // SFML_FTP_H
