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
#include <SFML/Network/Ftp.h>
#include <SFML/Network/FtpStruct.h>
#include <SFML/Network/IpAddress.hpp>
#include <SFML/Internal.h>


////////////////////////////////////////////////////////////
/// Destroy an existing Ftp directory response
////////////////////////////////////////////////////////////
void sfFtpListingResponse_Destroy(sfFtpListingResponse* ftpListingResponse)
{
    delete ftpListingResponse;
}


////////////////////////////////////////////////////////////
/// Convenience function to check if the response status code
/// means a success
////////////////////////////////////////////////////////////
sfBool sfFtpListingResponse_IsOk(const sfFtpListingResponse* ftpListingResponse)
{
    CSFML_CALL_RETURN(ftpListingResponse, IsOk(), sfFalse);
}


////////////////////////////////////////////////////////////
/// Get the response status code
////////////////////////////////////////////////////////////
sfFtpStatus sfFtpListingResponse_GetStatus(const sfFtpListingResponse* ftpListingResponse)
{
    CSFML_CHECK_RETURN(ftpListingResponse, sfFtpInvalidResponse);

    return static_cast<sfFtpStatus>(ftpListingResponse->This.GetStatus());
}


////////////////////////////////////////////////////////////
/// Get the full message contained in the response
////////////////////////////////////////////////////////////
const char* sfFtpListingResponse_GetMessage(const sfFtpListingResponse* ftpListingResponse)
{
    CSFML_CHECK_RETURN(ftpListingResponse, NULL);

    return ftpListingResponse->This.GetMessage().c_str();
}


////////////////////////////////////////////////////////////
/// Get the number of filenames in the listing
////////////////////////////////////////////////////////////
size_t sfFtpListingResponse_GetCount(const sfFtpListingResponse* ftpListingResponse)
{
    CSFML_CHECK_RETURN(ftpListingResponse, 0);

    return ftpListingResponse->This.GetFilenames().size();
}


////////////////////////////////////////////////////////////
/// Get the Index-th filename in the directory
////////////////////////////////////////////////////////////
const char* sfFtpListingResponse_GetFilename(const sfFtpListingResponse* ftpListingResponse, size_t index)
{
    CSFML_CHECK_RETURN(ftpListingResponse, NULL);

    return ftpListingResponse->This.GetFilenames()[index].c_str();
}


////////////////////////////////////////////////////////////
/// Destroy an existing Ftp directory response
////////////////////////////////////////////////////////////
void sfFtpDirectoryResponse_Destroy(sfFtpDirectoryResponse* ftpDirectoryResponse)
{
    delete ftpDirectoryResponse;
}


////////////////////////////////////////////////////////////
/// Convenience function to check if the response status code
/// means a success
////////////////////////////////////////////////////////////
sfBool sfFtpDirectoryResponse_IsOk(const sfFtpDirectoryResponse* ftpDirectoryResponse)
{
    CSFML_CALL_RETURN(ftpDirectoryResponse, IsOk(), sfFalse);
}


////////////////////////////////////////////////////////////
/// Get the response status code
////////////////////////////////////////////////////////////
sfFtpStatus sfFtpDirectoryResponse_GetStatus(const sfFtpDirectoryResponse* ftpDirectoryResponse)
{
    CSFML_CHECK_RETURN(ftpDirectoryResponse, sfFtpInvalidResponse);

    return static_cast<sfFtpStatus>(ftpDirectoryResponse->This.GetStatus());
}


////////////////////////////////////////////////////////////
/// Get the full message contained in the response
////////////////////////////////////////////////////////////
const char* sfFtpDirectoryResponse_GetMessage(const sfFtpDirectoryResponse* ftpDirectoryResponse)
{
    CSFML_CHECK_RETURN(ftpDirectoryResponse, NULL);

    return ftpDirectoryResponse->This.GetMessage().c_str();
}


////////////////////////////////////////////////////////////
/// Get the directory returned in the response
////////////////////////////////////////////////////////////
const char* sfFtpDirectoryResponse_GetDirectory(const sfFtpDirectoryResponse* ftpDirectoryResponse)
{
    CSFML_CHECK_RETURN(ftpDirectoryResponse, NULL);

    return ftpDirectoryResponse->This.GetDirectory().c_str();
}


////////////////////////////////////////////////////////////
/// Destroy an existing Ftp response
////////////////////////////////////////////////////////////
void sfFtpResponse_Destroy(sfFtpResponse* ftpResponse)
{
    delete ftpResponse;
}


////////////////////////////////////////////////////////////
/// Convenience function to check if the response status code
/// means a success
////////////////////////////////////////////////////////////
sfBool sfFtpResponse_IsOk(const sfFtpResponse* ftpResponse)
{
    CSFML_CALL_RETURN(ftpResponse, IsOk(), sfFalse);
}


////////////////////////////////////////////////////////////
/// Get the response status code
////////////////////////////////////////////////////////////
sfFtpStatus sfFtpResponse_GetStatus(const sfFtpResponse* ftpResponse)
{
    CSFML_CHECK_RETURN(ftpResponse, sfFtpInvalidResponse);

    return static_cast<sfFtpStatus>(ftpResponse->This.GetStatus());
}


////////////////////////////////////////////////////////////
/// Get the full message contained in the response
////////////////////////////////////////////////////////////
const char* sfFtpResponse_GetMessage(const sfFtpResponse* ftpResponse)
{
    CSFML_CHECK_RETURN(ftpResponse, NULL);

    return ftpResponse->This.GetMessage().c_str();
}


////////////////////////////////////////////////////////////
/// Construct a new Ftp
////////////////////////////////////////////////////////////
sfFtp* sfFtp_Create(void)
{
    return new sfFtp;
}


////////////////////////////////////////////////////////////
/// Destroy an existing Ftp
////////////////////////////////////////////////////////////
void sfFtp_Destroy(sfFtp* ftp)
{
    delete ftp;
}


////////////////////////////////////////////////////////////
/// Connect to the specified FTP server
////////////////////////////////////////////////////////////
sfFtpResponse* sfFtp_Connect(sfFtp* ftp, sfIpAddress server, unsigned short port, float timeout)
{
    CSFML_CHECK_RETURN(ftp, NULL);

    sf::IpAddress SFMLServer(server.Address);

    return new sfFtpResponse(ftp->This.Connect(SFMLServer, port, timeout));
}


////////////////////////////////////////////////////////////
/// Log in using anonymous account
////////////////////////////////////////////////////////////
sfFtpResponse* sfFtp_LoginAnonymous(sfFtp* ftp)
{
    CSFML_CHECK_RETURN(ftp, NULL);

    return new sfFtpResponse(ftp->This.Login());
}


////////////////////////////////////////////////////////////
/// Log in using a username and a password
////////////////////////////////////////////////////////////
sfFtpResponse* sfFtp_Login(sfFtp* ftp, const char* userName, const char* password)
{
    CSFML_CHECK_RETURN(ftp, NULL);

    return new sfFtpResponse(ftp->This.Login(userName ? userName : "", password ? password : ""));
}


////////////////////////////////////////////////////////////
/// Close the connection with FTP server
////////////////////////////////////////////////////////////
sfFtpResponse* sfFtp_Disconnect(sfFtp* ftp)
{
    CSFML_CHECK_RETURN(ftp, NULL);

    return new sfFtpResponse(ftp->This.Disconnect());
}


////////////////////////////////////////////////////////////
/// Send a null command just to prevent from being disconnected
////////////////////////////////////////////////////////////
sfFtpResponse* sfFtp_KeepAlive(sfFtp* ftp)
{
    CSFML_CHECK_RETURN(ftp, NULL);

    return new sfFtpResponse(ftp->This.KeepAlive());
}


////////////////////////////////////////////////////////////
/// Get the current working directory
////////////////////////////////////////////////////////////
sfFtpDirectoryResponse* sfFtp_GetWorkingDirectory(sfFtp* ftp)
{
    CSFML_CHECK_RETURN(ftp, NULL);

    return new sfFtpDirectoryResponse(ftp->This.GetWorkingDirectory());
}


////////////////////////////////////////////////////////////
/// Get the contents of the given directory
/// (subdirectories and files)
////////////////////////////////////////////////////////////
sfFtpListingResponse* sfFtp_GetDirectoryListing(sfFtp* ftp, const char* directory)
{
    CSFML_CHECK_RETURN(ftp, NULL);

    return new sfFtpListingResponse(ftp->This.GetDirectoryListing(directory ? directory : ""));
}


////////////////////////////////////////////////////////////
/// Change the current working directory
////////////////////////////////////////////////////////////
sfFtpResponse* sfFtp_ChangeDirectory(sfFtp* ftp, const char* directory)
{
    CSFML_CHECK_RETURN(ftp, NULL);

    return new sfFtpResponse(ftp->This.ChangeDirectory(directory ? directory : ""));
}


////////////////////////////////////////////////////////////
/// Go to the parent directory of the current one
////////////////////////////////////////////////////////////
sfFtpResponse* sfFtp_ParentDirectory(sfFtp* ftp)
{
    CSFML_CHECK_RETURN(ftp, NULL);

    return new sfFtpResponse(ftp->This.ParentDirectory());
}


////////////////////////////////////////////////////////////
/// Create a new directory
////////////////////////////////////////////////////////////
sfFtpResponse* sfFtp_CreateDirectory(sfFtp* ftp, const char* name)
{
    CSFML_CHECK_RETURN(ftp, NULL);

    return new sfFtpResponse(ftp->This.CreateDirectory(name ? name : ""));
}


////////////////////////////////////////////////////////////
/// Remove an existing directory
////////////////////////////////////////////////////////////
sfFtpResponse* sfFtp_DeleteDirectory(sfFtp* ftp, const char* name)
{
    CSFML_CHECK_RETURN(ftp, NULL);

    return new sfFtpResponse(ftp->This.DeleteDirectory(name ? name : ""));
}


////////////////////////////////////////////////////////////
/// Rename a file
////////////////////////////////////////////////////////////
sfFtpResponse* sfFtp_RenameFile(sfFtp* ftp, const char* file, const char* newName)
{
    CSFML_CHECK_RETURN(ftp, NULL);

    return new sfFtpResponse(ftp->This.RenameFile(file ? file : "", newName ? newName : ""));
}


////////////////////////////////////////////////////////////
/// Remove an existing file
////////////////////////////////////////////////////////////
sfFtpResponse* sfFtp_DeleteFile(sfFtp* ftp, const char* name)
{
    CSFML_CHECK_RETURN(ftp, NULL);

    return new sfFtpResponse(ftp->This.DeleteFile(name ? name : ""));
}


////////////////////////////////////////////////////////////
/// Download a file from the server
////////////////////////////////////////////////////////////
sfFtpResponse* sfFtp_Download(sfFtp* ftp, const char* distantFile, const char* destPath, sfFtpTransferMode mode)
{
    CSFML_CHECK_RETURN(ftp, NULL);

    return new sfFtpResponse(ftp->This.Download(distantFile ? distantFile : "",
                                                destPath ? destPath : "",
                                                static_cast<sf::Ftp::TransferMode>(mode)));
}


////////////////////////////////////////////////////////////
/// Upload a file to the server
////////////////////////////////////////////////////////////
sfFtpResponse* sfFtp_Upload(sfFtp* ftp, const char* localFile, const char* destPath, sfFtpTransferMode mode)
{
    CSFML_CHECK_RETURN(ftp, NULL);

    return new sfFtpResponse(ftp->This.Upload(localFile ? localFile : "",
                                              destPath ? destPath : "",
                                              static_cast<sf::Ftp::TransferMode>(mode)));
}
