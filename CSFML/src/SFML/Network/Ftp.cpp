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
#include <SFML/Network/Ftp.hpp>
#include <SFML/Network/IPAddress.hpp>
#include <SFML/Internal.h>


struct sfFtpListingResponse
{
    sfFtpListingResponse(const sf::Ftp::ListingResponse& Response) : This(Response) {}

    sf::Ftp::ListingResponse This;
};


////////////////////////////////////////////////////////////
/// Destroy an existing Ftp directory response
////////////////////////////////////////////////////////////
void sfFtpListingResponse_Destroy(sfFtpListingResponse* FtpListingResponse)
{
    delete FtpListingResponse;
}

////////////////////////////////////////////////////////////
/// Convenience function to check if the response status code
/// means a success
////////////////////////////////////////////////////////////
sfBool sfFtpListingResponse_IsOk(sfFtpListingResponse* FtpListingResponse)
{
    CSFML_CALL_RETURN(FtpListingResponse, IsOk(), sfFalse);
}

////////////////////////////////////////////////////////////
/// Get the response status code
////////////////////////////////////////////////////////////
sfFtpStatus sfFtpListingResponse_GetStatus(sfFtpListingResponse* FtpListingResponse)
{
    CSFML_CHECK_RETURN(FtpListingResponse, sfFtpInvalidResponse);

    return static_cast<sfFtpStatus>(FtpListingResponse->This.GetStatus());
}

////////////////////////////////////////////////////////////
/// Get the full message contained in the response
////////////////////////////////////////////////////////////
const char* sfFtpListingResponse_GetMessage(sfFtpListingResponse* FtpListingResponse)
{
    CSFML_CHECK_RETURN(FtpListingResponse, NULL);

    return FtpListingResponse->This.GetMessage().c_str();
}

////////////////////////////////////////////////////////////
/// Get the number of filenames in the listing
////////////////////////////////////////////////////////////
size_t sfFtpListingResponse_GetCount(sfFtpListingResponse* FtpListingResponse)
{
    CSFML_CALL_RETURN(FtpListingResponse, GetCount(), 0);
}

////////////////////////////////////////////////////////////
/// Get the Index-th filename in the directory
////////////////////////////////////////////////////////////
const char* sfFtpListingResponse_GetFilename(sfFtpListingResponse* FtpListingResponse, size_t Index)
{
    CSFML_CHECK_RETURN(FtpListingResponse, NULL);

    return FtpListingResponse->This.GetFilename(Index).c_str();
}


struct sfFtpDirectoryResponse
{
    sfFtpDirectoryResponse(const sf::Ftp::DirectoryResponse& Response) : This(Response) {}

    sf::Ftp::DirectoryResponse This;
};


////////////////////////////////////////////////////////////
/// Destroy an existing Ftp directory response
////////////////////////////////////////////////////////////
void sfFtpDirectoryResponse_Destroy(sfFtpDirectoryResponse* FtpDirectoryResponse)
{
    delete FtpDirectoryResponse;
}

////////////////////////////////////////////////////////////
/// Convenience function to check if the response status code
/// means a success
////////////////////////////////////////////////////////////
sfBool sfFtpDirectoryResponse_IsOk(sfFtpDirectoryResponse* FtpDirectoryResponse)
{
    CSFML_CALL_RETURN(FtpDirectoryResponse, IsOk(), sfFalse);
}

////////////////////////////////////////////////////////////
/// Get the response status code
////////////////////////////////////////////////////////////
sfFtpStatus sfFtpDirectoryResponse_GetStatus(sfFtpDirectoryResponse* FtpDirectoryResponse)
{
    CSFML_CHECK_RETURN(FtpDirectoryResponse, sfFtpInvalidResponse);

    return static_cast<sfFtpStatus>(FtpDirectoryResponse->This.GetStatus());
}

////////////////////////////////////////////////////////////
/// Get the full message contained in the response
////////////////////////////////////////////////////////////
const char* sfFtpDirectoryResponse_GetMessage(sfFtpDirectoryResponse* FtpDirectoryResponse)
{
    CSFML_CHECK_RETURN(FtpDirectoryResponse, NULL);

    return FtpDirectoryResponse->This.GetMessage().c_str();
}

////////////////////////////////////////////////////////////
/// Get the directory returned in the response
////////////////////////////////////////////////////////////
const char* sfFtpDirectoryResponse_GetDirectory(sfFtpDirectoryResponse* FtpDirectoryResponse)
{
    CSFML_CHECK_RETURN(FtpDirectoryResponse, NULL);

    return FtpDirectoryResponse->This.GetDirectory().c_str();
}


struct sfFtpResponse
{
    sfFtpResponse(const sf::Ftp::Response& Response) : This(Response) {}

    sf::Ftp::Response This;
};


////////////////////////////////////////////////////////////
/// Destroy an existing Ftp response
////////////////////////////////////////////////////////////
void sfFtpResponse_Destroy(sfFtpResponse* FtpResponse)
{
    delete FtpResponse;
}


////////////////////////////////////////////////////////////
/// Convenience function to check if the response status code
/// means a success
////////////////////////////////////////////////////////////
sfBool sfFtpResponse_IsOk(sfFtpResponse* FtpResponse)
{
    CSFML_CALL_RETURN(FtpResponse, IsOk(), sfFalse);
}


////////////////////////////////////////////////////////////
/// Get the response status code
////////////////////////////////////////////////////////////
sfFtpStatus sfFtpResponse_GetStatus(sfFtpResponse* FtpResponse)
{
    CSFML_CHECK_RETURN(FtpResponse, sfFtpInvalidResponse);

    return static_cast<sfFtpStatus>(FtpResponse->This.GetStatus());
}


////////////////////////////////////////////////////////////
/// Get the full message contained in the response
////////////////////////////////////////////////////////////
const char* sfFtpResponse_GetMessage(sfFtpResponse* FtpResponse)
{
    CSFML_CHECK_RETURN(FtpResponse, NULL);

    return FtpResponse->This.GetMessage().c_str();
}


struct sfFtp
{
    sf::Ftp This;
};


////////////////////////////////////////////////////////////
/// Construct a new Ftp
////////////////////////////////////////////////////////////
sfFtp* sfFtp_Create()
{
    return new sfFtp;
}


////////////////////////////////////////////////////////////
/// Destroy an existing Ftp
////////////////////////////////////////////////////////////
void sfFtp_Destroy(sfFtp* Ftp)
{
    delete Ftp;
}


////////////////////////////////////////////////////////////
/// Connect to the specified FTP server
////////////////////////////////////////////////////////////
sfFtpResponse* sfFtp_Connect(sfFtp* Ftp, sfIPAddress Server, unsigned short Port, float Timeout)
{
    CSFML_CHECK_RETURN(Ftp, NULL);

    sf::IPAddress SFMLServer(Server.Address);

    return new sfFtpResponse(Ftp->This.Connect(SFMLServer, Port, Timeout));
}


////////////////////////////////////////////////////////////
/// Log in using anonymous account
////////////////////////////////////////////////////////////
sfFtpResponse* sfFtp_LoginAnonymous(sfFtp* Ftp)
{
    CSFML_CHECK_RETURN(Ftp, NULL);

    return new sfFtpResponse(Ftp->This.Login());
}


////////////////////////////////////////////////////////////
/// Log in using a username and a password
////////////////////////////////////////////////////////////
sfFtpResponse* sfFtp_Login(sfFtp* Ftp, const char* UserName, const char* Password)
{
    CSFML_CHECK_RETURN(Ftp, NULL);

    return new sfFtpResponse(Ftp->This.Login(UserName ? UserName : "", Password ? Password : ""));
}


////////////////////////////////////////////////////////////
/// Close the connection with FTP server
////////////////////////////////////////////////////////////
sfFtpResponse* sfFtp_Disconnect(sfFtp* Ftp)
{
    CSFML_CHECK_RETURN(Ftp, NULL);

    return new sfFtpResponse(Ftp->This.Disconnect());
}


////////////////////////////////////////////////////////////
/// Send a null command just to prevent from being disconnected
////////////////////////////////////////////////////////////
sfFtpResponse* sfFtp_KeepAlive(sfFtp* Ftp)
{
    CSFML_CHECK_RETURN(Ftp, NULL);

    return new sfFtpResponse(Ftp->This.KeepAlive());
}


////////////////////////////////////////////////////////////
/// Get the current working directory
////////////////////////////////////////////////////////////
sfFtpDirectoryResponse* sfFtp_GetWorkingDirectory(sfFtp* Ftp)
{
    CSFML_CHECK_RETURN(Ftp, NULL);

    return new sfFtpDirectoryResponse(Ftp->This.GetWorkingDirectory());
}


////////////////////////////////////////////////////////////
/// Get the contents of the given directory
/// (subdirectories and files)
////////////////////////////////////////////////////////////
sfFtpListingResponse* sfFtp_GetDirectoryListing(sfFtp* Ftp, const char* Directory)
{
    CSFML_CHECK_RETURN(Ftp, NULL);

    return new sfFtpListingResponse(Ftp->This.GetDirectoryListing(Directory ? Directory : ""));
}


////////////////////////////////////////////////////////////
/// Change the current working directory
////////////////////////////////////////////////////////////
sfFtpResponse* sfFtp_ChangeDirectory(sfFtp* Ftp, const char* Directory)
{
    CSFML_CHECK_RETURN(Ftp, NULL);

    return new sfFtpResponse(Ftp->This.ChangeDirectory(Directory ? Directory : ""));
}


////////////////////////////////////////////////////////////
/// Go to the parent directory of the current one
////////////////////////////////////////////////////////////
sfFtpResponse* sfFtp_ParentDirectory(sfFtp* Ftp)
{
    CSFML_CHECK_RETURN(Ftp, NULL);

    return new sfFtpResponse(Ftp->This.ParentDirectory());
}


////////////////////////////////////////////////////////////
/// Create a new directory
////////////////////////////////////////////////////////////
sfFtpResponse* sfFtp_MakeDirectory(sfFtp* Ftp, const char* Name)
{
    CSFML_CHECK_RETURN(Ftp, NULL);

    return new sfFtpResponse(Ftp->This.MakeDirectory(Name ? Name : ""));
}


////////////////////////////////////////////////////////////
/// Remove an existing directory
////////////////////////////////////////////////////////////
sfFtpResponse* sfFtp_DeleteDirectory(sfFtp* Ftp, const char* Name)
{
    CSFML_CHECK_RETURN(Ftp, NULL);

    return new sfFtpResponse(Ftp->This.DeleteDirectory(Name ? Name : ""));
}


////////////////////////////////////////////////////////////
/// Rename a file
////////////////////////////////////////////////////////////
sfFtpResponse* sfFtp_RenameFile(sfFtp* Ftp, const char* File, const char* NewName)
{
    CSFML_CHECK_RETURN(Ftp, NULL);

    return new sfFtpResponse(Ftp->This.RenameFile(File ? File : "", NewName ? NewName : ""));
}


////////////////////////////////////////////////////////////
/// Remove an existing file
////////////////////////////////////////////////////////////
sfFtpResponse* sfFtp_DeleteFile(sfFtp* Ftp, const char* Name)
{
    CSFML_CHECK_RETURN(Ftp, NULL);

    return new sfFtpResponse(Ftp->This.DeleteFile(Name ? Name : ""));
}


////////////////////////////////////////////////////////////
/// Download a file from the server
////////////////////////////////////////////////////////////
sfFtpResponse* sfFtp_Download(sfFtp* Ftp, const char* DistantFile, const char* DestPath, sfFtpTransferMode Mode)
{
    CSFML_CHECK_RETURN(Ftp, NULL);

    return new sfFtpResponse(Ftp->This.Download(DistantFile ? DistantFile : "",
                                                DestPath ? DestPath : "",
                                                static_cast<sf::Ftp::TransferMode>(Mode)));
}


////////////////////////////////////////////////////////////
/// Upload a file to the server
////////////////////////////////////////////////////////////
sfFtpResponse* sfFtp_Upload(sfFtp* Ftp, const char* LocalFile, const char* DestPath, sfFtpTransferMode Mode)
{
    CSFML_CHECK_RETURN(Ftp, NULL);

    return new sfFtpResponse(Ftp->This.Upload(LocalFile ? LocalFile : "",
                                              DestPath ? DestPath : "",
                                              static_cast<sf::Ftp::TransferMode>(Mode)));
}
