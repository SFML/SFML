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

#ifndef SFML_IMAGELOADER_HPP
#define SFML_IMAGELOADER_HPP

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Graphics/Color.hpp>
#include <SFML/System/NonCopyable.hpp>
#include <string>
#include <vector>


namespace sf
{
namespace priv
{
////////////////////////////////////////////////////////////
/// ImageLoader load and save images from files ;
/// Supported formats are : bmp, dds, jpg, png, tga, psd
////////////////////////////////////////////////////////////
class ImageLoader : NonCopyable
{
public :

    ////////////////////////////////////////////////////////////
    /// Get the unique instance of the class
    ///
    /// \return Reference to the ImageLoader instance
    ///
    ////////////////////////////////////////////////////////////
    static ImageLoader& GetInstance();

    ////////////////////////////////////////////////////////////
    /// Load pixels from an image file
    ///
    /// \param filename : Path of image file to load
    /// \param pixels :   Array of pixels to fill with loaded image
    /// \param width :    Width of loaded image, in pixels
    /// \param height :   Height of loaded image, in pixels
    ///
    /// \return True if loading was successful
    ///
    ////////////////////////////////////////////////////////////
    bool LoadImageFromFile(const std::string& filename, std::vector<Color>& pixels, unsigned int& width, unsigned int& height);

    ////////////////////////////////////////////////////////////
    /// Load pixels from an image file in memory
    ///
    /// \param data :        Pointer to the file data in memory
    /// \param sizeInBytes : Size of the data to load, in bytes
    /// \param pixels :      Array of pixels to fill with loaded image
    /// \param width :       Width of loaded image, in pixels
    /// \param height :      Height of loaded image, in pixels
    ///
    /// \return True if loading was successful
    ///
    ////////////////////////////////////////////////////////////
    bool LoadImageFromMemory(const void* data, std::size_t sizeInBytes, std::vector<Color>& pixels, unsigned int& width, unsigned int& height);

    ////////////////////////////////////////////////////////////
    /// Save pixels to an image file
    ///
    /// \param filename : Path of image file to save
    /// \param pixels :   Array of pixels to save to image
    /// \param width :    Width of image to save, in pixels
    /// \param height :   Height of image to save, in pixels
    ///
    /// \return True if saving was successful
    ///
    ////////////////////////////////////////////////////////////
    bool SaveImageToFile(const std::string& filename, const std::vector<Color>& pixels, unsigned int width, unsigned int height);

private :

    ////////////////////////////////////////////////////////////
    /// Default constructor
    ///
    ////////////////////////////////////////////////////////////
    ImageLoader();

    ////////////////////////////////////////////////////////////
    /// Destructor
    ///
    ////////////////////////////////////////////////////////////
    ~ImageLoader();

    ////////////////////////////////////////////////////////////
    /// Save a JPG image file
    ///
    /// \param filename : Path of image file to save
    /// \param pixels :   Array of pixels to save to image
    /// \param width :    Width of image to save, in pixels
    /// \param height :   Height of image to save, in pixels
    ///
    /// \return True if saving was successful
    ///
    ////////////////////////////////////////////////////////////
    bool WriteJpg(const std::string& filename, const std::vector<Color>& pixels, unsigned int width, unsigned int height);

    ////////////////////////////////////////////////////////////
    /// Save a PNG image file
    ///
    /// \param filename : Path of image file to save
    /// \param pixels :   Array of pixels to save to image
    /// \param width :    Width of image to save, in pixels
    /// \param height :   Height of image to save, in pixels
    ///
    /// \return True if saving was successful
    ///
    ////////////////////////////////////////////////////////////
    bool WritePng(const std::string& filename, const std::vector<Color>& pixels, unsigned int width, unsigned int height);
};

} // namespace priv

} // namespace sf


#endif // SFML_IMAGELOADER_HPP
