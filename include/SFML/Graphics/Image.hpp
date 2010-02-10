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

#ifndef SFML_IMAGE_HPP
#define SFML_IMAGE_HPP

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/System/Resource.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <string>
#include <vector>


namespace sf
{
class RenderWindow;

////////////////////////////////////////////////////////////
/// Image is the low-level class for loading and
/// manipulating images
////////////////////////////////////////////////////////////
class SFML_API Image : public Resource<Image>
{
public :

    ////////////////////////////////////////////////////////////
    /// Default constructor
    ///
    ////////////////////////////////////////////////////////////
    Image();

    ////////////////////////////////////////////////////////////
    /// Copy constructor
    ///
    /// \param Copy : instance to copy
    ///
    ////////////////////////////////////////////////////////////
    Image(const Image& Copy);

    ////////////////////////////////////////////////////////////
    /// Construct an empty image
    ///
    /// \param Width :  Image width
    /// \param Height : Image height
    /// \param Col :    Image color (black by default)
    ///
    ////////////////////////////////////////////////////////////
    Image(unsigned int Width, unsigned int Height, const Color& Col = Color(0, 0, 0, 255));

    ////////////////////////////////////////////////////////////
    /// Construct the image from pixels in memory
    ///
    /// \param Width :  Image width
    /// \param Height : Image height
    /// \param Data :   Pointer to the pixels in memory (assumed format is RGBA)
    ///
    ////////////////////////////////////////////////////////////
    Image(unsigned int Width, unsigned int Height, const Uint8* Data);

    ////////////////////////////////////////////////////////////
    /// Destructor
    ///
    ////////////////////////////////////////////////////////////
    ~Image();

    ////////////////////////////////////////////////////////////
    /// Load the image from a file
    ///
    /// \param Filename : Path of the image file to load
    ///
    /// \return True if loading was successful
    ///
    ////////////////////////////////////////////////////////////
    bool LoadFromFile(const std::string& Filename);

    ////////////////////////////////////////////////////////////
    /// Load the image from a file in memory
    ///
    /// \param Data :        Pointer to the file data in memory
    /// \param SizeInBytes : Size of the data to load, in bytes
    ///
    /// \return True if loading was successful
    ///
    ////////////////////////////////////////////////////////////
    bool LoadFromMemory(const char* Data, std::size_t SizeInBytes);

    ////////////////////////////////////////////////////////////
    /// Load the image directly from an array of pixels
    ///
    /// \param Width :  Image width
    /// \param Height : Image height
    /// \param Data :   Pointer to the pixels in memory (assumed format is RGBA)
    ///
    /// \return True if loading was successful
    ///
    ////////////////////////////////////////////////////////////
    bool LoadFromPixels(unsigned int Width, unsigned int Height, const Uint8* Data);

    ////////////////////////////////////////////////////////////
    /// Save the content of the image to a file
    ///
    /// \param Filename : Path of the file to save (overwritten if already exist)
    ///
    /// \return True if saving was successful
    ///
    ////////////////////////////////////////////////////////////
    bool SaveToFile(const std::string& Filename) const;

    ////////////////////////////////////////////////////////////
    /// Create an empty image
    ///
    /// \param Width :  Image width
    /// \param Height : Image height
    /// \param Col :    Image color (black by default)
    ///
    /// \return True if creation was successful
    ///
    ////////////////////////////////////////////////////////////
    bool Create(unsigned int Width, unsigned int Height, Color Col = Color(0, 0, 0, 255));

    ////////////////////////////////////////////////////////////
    /// Create transparency mask from a specified colorkey
    ///
    /// \param ColorKey : Color to become transparent
    /// \param Alpha :    Alpha value to use for transparent pixels (0 by default)
    ///
    ////////////////////////////////////////////////////////////
    void CreateMaskFromColor(Color ColorKey, Uint8 Alpha = 0);

    ////////////////////////////////////////////////////////////
    /// Copy pixels from another image onto this one.
    /// This function does a slow pixel copy and should only
    /// be used at initialization time
    ///
    /// \param Source :     Source image to copy
    /// \param DestX :      X coordinate of the destination position
    /// \param DestY :      Y coordinate of the destination position
    /// \param SourceRect : Sub-rectangle of the source image to copy (empty by default - entire image)
    /// \param ApplyAlpha : Should the copy take in account the source transparency? (false by default)
    ///
    ////////////////////////////////////////////////////////////
    void Copy(const Image& Source, unsigned int DestX, unsigned int DestY, const IntRect& SourceRect = IntRect(0, 0, 0, 0), bool ApplyAlpha = false);

    ////////////////////////////////////////////////////////////
    /// Create the image from the current contents of the
    /// given window
    ///
    /// \param Window :     Window to capture
    /// \param SourceRect : Sub-rectangle of the screen to copy (empty by default - entire image)
    ///
    /// \return True if copy was successful
    ///
    ////////////////////////////////////////////////////////////
    bool CopyScreen(RenderWindow& Window, const IntRect& SourceRect = IntRect(0, 0, 0, 0));

    ////////////////////////////////////////////////////////////
    /// Change the color of a pixel
    ///
    /// \param X :   X coordinate of pixel in the image
    /// \param Y :   Y coordinate of pixel in the image
    /// \param Col : New color for pixel (X, Y)
    ///
    ////////////////////////////////////////////////////////////
    void SetPixel(unsigned int X, unsigned int Y, const Color& Col);

    ////////////////////////////////////////////////////////////
    /// Get a pixel from the image
    ///
    /// \param X : X coordinate of pixel in the image
    /// \param Y : Y coordinate of pixel in the image
    ///
    /// \return Color of pixel (X, Y)
    ///
    ////////////////////////////////////////////////////////////
    const Color& GetPixel(unsigned int X, unsigned int Y) const;

    ////////////////////////////////////////////////////////////
    /// Get a read-only pointer to the array of pixels (RGBA 8 bits integers components)
    /// Array size is GetWidth() x GetHeight() x 4
    /// This pointer becomes invalid if you reload or resize the image
    ///
    /// \return Const pointer to the array of pixels
    ///
    ////////////////////////////////////////////////////////////
    const Uint8* GetPixelsPtr() const;

    ////////////////////////////////////////////////////////////
    /// Bind the image for rendering
    ///
    ////////////////////////////////////////////////////////////
    void Bind() const;

    ////////////////////////////////////////////////////////////
    /// Enable or disable image smooth filter.
    /// This parameter is enabled by default
    ///
    /// \param Smooth : True to enable smoothing filter, false to disable it
    ///
    ////////////////////////////////////////////////////////////
    void SetSmooth(bool Smooth);

    ////////////////////////////////////////////////////////////
    /// Return the width of the image
    ///
    /// \return Width in pixels
    ///
    ////////////////////////////////////////////////////////////
    unsigned int GetWidth() const;

    ////////////////////////////////////////////////////////////
    /// Return the height of the image
    ///
    /// \return Height in pixels
    ///
    ////////////////////////////////////////////////////////////
    unsigned int GetHeight() const;

    ////////////////////////////////////////////////////////////
    /// Tells whether the smooth filtering is enabled or not
    ///
    /// \return True if image smoothing is enabled
    ///
    ////////////////////////////////////////////////////////////
    bool IsSmooth() const;

    ////////////////////////////////////////////////////////////
    /// Convert a subrect expressed in pixels, into float
    /// texture coordinates
    ///
    /// \param Rect : Sub-rectangle of image to convert
    ///
    /// \return Texture coordinates corresponding to the sub-rectangle
    ///
    ////////////////////////////////////////////////////////////
    FloatRect GetTexCoords(const IntRect& Rect) const;

    ////////////////////////////////////////////////////////////
    /// Get a valid texture size according to hardware support
    ///
    /// \param Size : Size to convert
    ///
    /// \return Valid nearest size (greater than or equal to specified size)
    ///
    ////////////////////////////////////////////////////////////
    static unsigned int GetValidTextureSize(unsigned int Size);

    ////////////////////////////////////////////////////////////
    /// Assignment operator
    ///
    /// \param Other : instance to assign
    ///
    /// \return Reference to the image
    ///
    ////////////////////////////////////////////////////////////
    Image& operator =(const Image& Other);

private :

    ////////////////////////////////////////////////////////////
    /// Create the OpenGL texture
    ///
    /// \return True if texture has been successfully created
    ///
    ////////////////////////////////////////////////////////////
    bool CreateTexture();

    ////////////////////////////////////////////////////////////
    /// Make sure the texture in video memory is updated with the
    /// array of pixels
    ////////////////////////////////////////////////////////////
    void EnsureTextureUpdate() const;

    ////////////////////////////////////////////////////////////
    /// Make sure the array of pixels is updated with the
    /// texture in video memory
    ////////////////////////////////////////////////////////////
    void EnsureArrayUpdate() const;

    ////////////////////////////////////////////////////////////
    /// Reset the image attributes
    ///
    ////////////////////////////////////////////////////////////
    void Reset();

    ////////////////////////////////////////////////////////////
    /// Destroy the OpenGL texture
    ///
    ////////////////////////////////////////////////////////////
    void DestroyTexture();

    ////////////////////////////////////////////////////////////
    // Member data
    ////////////////////////////////////////////////////////////
    unsigned int               myWidth;             ///< Image width
    unsigned int               myHeight;            ///< Image Height
    unsigned int               myTextureWidth;      ///< Actual texture width (can be greater than image width because of padding)
    unsigned int               myTextureHeight;     ///< Actual texture height (can be greater than image height because of padding)
    unsigned int               myTexture;           ///< Internal texture identifier
    bool                       myIsSmooth;          ///< Status of the smooth filter
    mutable std::vector<Color> myPixels;            ///< Pixels of the image
    mutable bool               myNeedTextureUpdate; ///< Status of synchronization between pixels in central memory and the internal texture un video memory
    mutable bool               myNeedArrayUpdate;   ///< Status of synchronization between pixels in central memory and the internal texture un video memory
};

} // namespace sf


#endif // SFML_IMAGE_HPP
