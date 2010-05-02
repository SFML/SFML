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
#include <SFML/Network/IpAddress.h>
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
/// \param ftpListingResponse : Ftp listing response to destroy
///
////////////////////////////////////////////////////////////
CSFML_API void sfFtpListingResponse_Destroy(sfFtpListingResponse* ftpListingResponse);

////////////////////////////////////////////////////////////
/// Convenience function to check if the response status code
/// means a success
///
/// \param ftpListingResponse : Ftp listing response
///
/// \return sfTrue if status is success (code < 400)
///
////////////////////////////////////////////////////////////
CSFML_API sfBool sfFtpListingResponse_IsOk(const sfFtpListingResponse* ftpListingResponse);

////////////////////////////////////////////////////////////
/// Get the response status code
///
/// \param ftpListingResponse : Ftp listing response
///
/// \return Status code
///
////////////////////////////////////////////////////////////
CSFML_API sfFtpStatus sfFtpListingResponse_GetStatus(const sfFtpListingResponse* ftpListingResponse);

////////////////////////////////////////////////////////////
/// Get the full message contained in the response
///
/// \param ftpListingResponse : Ftp listing response
///
/// \return The response message
///
////////////////////////////////////////////////////////////
CSFML_API const char* sfFtpListingResponse_GetMessage(const sfFtpListingResponse* ftpListingResponse);

////////////////////////////////////////////////////////////
/// Get the number of filenames in the listing
///
/// \param ftpListingResponse : Ftp listing response
///
/// \return Total number of filenames
///
////////////////////////////////////////////////////////////
CSFML_API size_t sfFtpListingResponse_GetCount(const sfFtpListingResponse* ftpListingResponse);

////////////////////////////////////////////////////////////
/// Get the Index-th filename in the directory
///
/// \param ftpListingResponse : Ftp listing response
/// \param index :              Index of the filename to get
///
/// \return Index-th filename
///
////////////////////////////////////////////////////////////
CSFML_API const char* sfFtpListingResponse_GetFilename(const sfFtpListingResponse* ftpListingResponse, size_t index);

////////////////////////////////////////////////////////////
/// Destroy an existing Ftp directory response
///
/// \param ftpDirectoryResponse : Ftp directory response to destroy
///
////////////////////////////////////////////////////////////
CSFML_API void sfFtpDirectoryResponse_Destroy(sfFtpDirectoryResponse* ftpDirectoryResponse);

////////////////////////////////////////////////////////////
/// Convenience function to check if the response status code
/// means a success
///
/// \param ftpDirectoryResponse : Ftp directory response
///
/// \return sfTrue if status is success (code < 400)
///
////////////////////////////////////////////////////////////
CSFML_API sfBool sfFtpDirectoryResponse_IsOk(const sfFtpDirectoryResponse* ftpDirectoryResponse);

////////////////////////////////////////////////////////////
/// Get the response status code
///
/// \param ftpDirectoryResponse : Ftp directory response
///
/// \return Status code
///
////////////////////////////////////////////////////////////
CSFML_API sfFtpStatus sfFtpDirectoryResponse_GetStatus(const sfFtpDirectoryResponse* ftpDirectoryResponse);

////////////////////////////////////////////////////////////
/// Get the full message contained in the response
///
/// \param ftpDirectoryResponse : Ftp directory response
///
/// \return The response message
///
////////////////////////////////////////////////////////////
CSFML_API const char* sfFtpDirectoryResponse_GetMessage(const sfFtpDirectoryResponse* ftpDirectoryResponse);

////////////////////////////////////////////////////////////
/// Get the directory returned in the response
///
/// \param ftpDirectoryResponse : Ftp directory response
///
/// \return Directory name
///
////////////////////////////////////////////////////////////
CSFML_API const char* sfFtpDirectoryResponse_GetDirectory(const sfFtpDirectoryResponse* ftpDirectoryResponse);


////////////////////////////////////////////////////////////
/// Destroy an existing Ftp response
///
/// \param ftpResponse : Ftp response to destroy
///
////////////////////////////////////////////////////////////
CSFML_API void sfFtpResponse_Destroy(sfFtpResponse* ftpResponse);

////////////////////////////////////////////////////////////
/// Convenience function to check if the response status code
/// means a success
///
/// \param ftpResponse : Ftp response
///
/// \return sfTrue if status is success (code < 400)
///
////////////////////////////////////////////////////////////
CSFML_API sfBool sfFtpResponse_IsOk(const sfFtpResponse* ftpResponse);

////////////////////////////////////////////////////////////
/// Get the response status code
///
/// \param ftpResponse : Ftp response
///
/// \return Status code
///
////////////////////////////////////////////////////////////
CSFML_API sfFtpStatus sfFtpResponse_GetStatus(const sfFtpResponse* ftpResponse);

////////////////////////////////////////////////////////////
/// Get the full message contained in the response
///
/// \param ftpResponse : Ftp response
///
/// \return The response message
///
////////////////////////////////////////////////////////////
CSFML_API const char* sfFtpResponse_GetMessage(const sfFtpResponse* ftpResponse);

////////////////////////////////////////////////////////////
/// Construct a new Ftp
///
/// \return Pointer to the new Ftp
///
////////////////////////////////////////////////////////////
CSFML_API sfFtp* sfFtp_Create(void);

////////////////////////////////////////////////////////////
/// Destroy an existing Ftp
///
/// \param ftp : Ftp to destroy
///
////////////////////////////////////////////////////////////
CSFML_API void sfFtp_Destroy(sfFtp* ftp);

////////////////////////////////////////////////////////////
/// Connect to the specified FTP server
///
/// \param ftp :     Ftp instance
/// \param server :  FTP server to connect to
/// \param port :    Port used for connection (21 by default, standard FTP port)
/// \param timeout : Maximum time to wait (0 to use no timeout)
///
/// \return Server response to the request
///
////////////////////////////////////////////////////////////
CSFML_API sfFtpResponse* sfFtp_Connect(sfFtp* ftp, sfIpAddress server, unsigned short port, float timeout);

////////////////////////////////////////////////////////////
/// Log in using anonymous account
///
/// \param ftp : Ftp instance
///
/// \return Server response to the request
///
////////////////////////////////////////////////////////////
CSFML_API sfFtpResponse* sfFtp_LoginAnonymous(sfFtp* ftp);

////////////////////////////////////////////////////////////
/// Log in using a username and a password
///
/// \param ftp :      Ftp instance
/// \param userName : User name
/// \param password : Password
///
/// \return Server response to the request
///
////////////////////////////////////////////////////////////
CSFML_API sfFtpResponse* sfFtp_Login(sfFtp* ftp, const char* userName, const char* password);

////////////////////////////////////////////////////////////
/// Close the connection with FTP server
///
/// \param ftp : Ftp instance
///
/// \return Server response to the request
///
////////////////////////////////////////////////////////////
CSFML_API sfFtpResponse* sfFtp_Disconnect(sfFtp* ftp);

////////////////////////////////////////////////////////////
/// Send a null command just to prevent from being disconnected
///
/// \param ftp : Ftp instance
///
/// \return Server response to the request
///
////////////////////////////////////////////////////////////
CSFML_API sfFtpResponse* sfFtp_KeepAlive(sfFtp* ftp);

////////////////////////////////////////////////////////////
/// Get the current working directory
///
/// \param ftp : Ftp instance
///
/// \return Server response to the request
///
////////////////////////////////////////////////////////////
CSFML_API sfFtpDirectoryResponse* sfFtp_GetWorkingDirectory(sfFtp* ftp);

////////////////////////////////////////////////////////////
/// Get the contents of the given directory
/// (subdirectories and files)
///
/// \param ftp :       Ftp instance
/// \param directory : Directory to list ("" by default, the current one)
///
/// \return Server response to the request
///
////////////////////////////////////////////////////////////
CSFML_API sfFtpListingResponse* sfFtp_GetDirectoryListing(sfFtp* ftp, const char* directory);

////////////////////////////////////////////////////////////
/// Change the current working directory
///
/// \param ftp :       Ftp instance
/// \param directory : New directory, relative to the current one
///
/// \return Server response to the request
///
////////////////////////////////////////////////////////////
CSFML_API sfFtpResponse* sfFtp_ChangeDirectory(sfFtp* ftp, const char* directory);

////////////////////////////////////////////////////////////
/// Go to the parent directory of the current one
///
/// \param ftp : Ftp instance
///
/// \return Server response to the request
///
////////////////////////////////////////////////////////////
CSFML_API sfFtpResponse* sfFtp_ParentDirectory(sfFtp* ftp);

////////////////////////////////////////////////////////////
/// Create a new directory
///
/// \param ftp :  Ftp instance
/// \param name : Name of the directory to create
///
/// \return Server response to the request
///
////////////////////////////////////////////////////////////
CSFML_API sfFtpResponse* sfFtp_CreateDirectory(sfFtp* ftp, const char* name);

////////////////////////////////////////////////////////////
/// Remove an existing directory
///
/// \param ftp :  Ftp instance
/// \param name : Name of the directory to remove
///
/// \return Server response to the request
///
////////////////////////////////////////////////////////////
CSFML_API sfFtpResponse* sfFtp_DeleteDirectory(sfFtp* ftp, const char* name);

////////////////////////////////////////////////////////////
/// Rename a file
///
/// \param ftp :     Ftp instance
/// \param file :    File to rename
/// \param newName : New name
///
/// \return Server response to the request
///
////////////////////////////////////////////////////////////
CSFML_API sfFtpResponse* sfFtp_RenameFile(sfFtp* ftp, const char* file, const char* newName);

////////////////////////////////////////////////////////////
/// Remove an existing file
///
/// \param ftp :  Ftp instance
/// \param name : File to remove
///
/// \return Server response to the request
///
////////////////////////////////////////////////////////////
CSFML_API sfFtpResponse* sfFtp_DeleteFile(sfFtp* ftp, const char* name);

////////////////////////////////////////////////////////////
/// Download a file from the server
///
/// \param ftp :         Ftp instance
/// \param distantFile : Path of the distant file to download
/// \param destPath :    Where to put to file on the local computer
/// \param mode :        Transfer mode (binary by default)
///
/// \return Server response to the request
///
////////////////////////////////////////////////////////////
CSFML_API sfFtpResponse* sfFtp_Download(sfFtp* ftp, const char* distantFile, const char* destPath, sfFtpTransferMode mode);

////////////////////////////////////////////////////////////
/// Upload a file to the server
///
/// \param ftp :       Ftp instance
/// \param localFile : Path of the local file to upload
/// \param destPath :  Where to put to file on the server
/// \param mode :      Transfer mode (binary by default)
///
/// \return Server response to the request
///
////////////////////////////////////////////////////////////
CSFML_API sfFtpResponse* sfFtp_Upload(sfFtp* ftp, const char* localFile, const char* destPath, sfFtpTransferMode mode);


#endif // SFML_FTP_H
