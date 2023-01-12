////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007-2023 Laurent Gomila (laurent@sfml-dev.org)
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

#pragma once

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Config.hpp>

#include <SFML/System/Vector2.hpp>

#include <filesystem>
#include <string>
#include <vector>


namespace sf
{
class InputStream;

namespace priv
{
////////////////////////////////////////////////////////////
/// \brief Load/save image files
///
////////////////////////////////////////////////////////////
class ImageLoader
{
public:
    ////////////////////////////////////////////////////////////
    /// \brief Deleted copy constructor
    ///
    ////////////////////////////////////////////////////////////
    ImageLoader(const ImageLoader&) = delete;

    ////////////////////////////////////////////////////////////
    /// \brief Deleted copy assignment
    ///
    ////////////////////////////////////////////////////////////
    ImageLoader& operator=(const ImageLoader&) = delete;

    ////////////////////////////////////////////////////////////
    /// \brief Get the unique instance of the class
    ///
    /// \return Reference to the ImageLoader instance
    ///
    ////////////////////////////////////////////////////////////
    static ImageLoader& getInstance();

    ////////////////////////////////////////////////////////////
    /// \brief Load an image from a file on disk
    ///
    /// \param filename Path of image file to load
    /// \param pixels   Array of pixels to fill with loaded image
    /// \param size     Size of loaded image, in pixels
    ///
    /// \return True if loading was successful
    ///
    ////////////////////////////////////////////////////////////
    bool loadImageFromFile(const std::filesystem::path& filename, std::vector<std::uint8_t>& pixels, Vector2u& size);

    ////////////////////////////////////////////////////////////
    /// \brief Load an image from a file in memory
    ///
    /// \param data     Pointer to the file data in memory
    /// \param dataSize Size of the data to load, in bytes
    /// \param pixels   Array of pixels to fill with loaded image
    /// \param size     Size of loaded image, in pixels
    ///
    /// \return True if loading was successful
    ///
    ////////////////////////////////////////////////////////////
    bool loadImageFromMemory(const void* data, std::size_t dataSize, std::vector<std::uint8_t>& pixels, Vector2u& size);

    ////////////////////////////////////////////////////////////
    /// \brief Load an image from a custom stream
    ///
    /// \param stream Source stream to read from
    /// \param pixels Array of pixels to fill with loaded image
    /// \param size   Size of loaded image, in pixels
    ///
    /// \return True if loading was successful
    ///
    ////////////////////////////////////////////////////////////
    bool loadImageFromStream(InputStream& stream, std::vector<std::uint8_t>& pixels, Vector2u& size);

    ////////////////////////////////////////////////////////////
    /// \brief Save an array of pixels as an image file
    ///
    /// \param filename Path of image file to save
    /// \param pixels   Array of pixels to save to image
    /// \param size     Size of image to save, in pixels
    ///
    /// \return True if saving was successful
    ///
    ////////////////////////////////////////////////////////////
    bool saveImageToFile(const std::filesystem::path& filename, const std::vector<std::uint8_t>& pixels, const Vector2u& size);

    ////////////////////////////////////////////////////////////
    /// \brief Save an array of pixels as an encoded image buffer
    ///
    /// \param format   Must be "bmp", "png", "tga" or "jpg"/"jpeg".
    /// \param output   Buffer to fill with encoded data
    /// \param pixels   Array of pixels to save to image
    /// \param size     Size of image to save, in pixels
    ///
    /// \return True if saving was successful
    ///
    ////////////////////////////////////////////////////////////
    bool saveImageToMemory(const std::string&               format,
                           std::vector<std::uint8_t>&       output,
                           const std::vector<std::uint8_t>& pixels,
                           const Vector2u&                  size);

private:
    ////////////////////////////////////////////////////////////
    /// \brief Default constructor
    ///
    ////////////////////////////////////////////////////////////
    ImageLoader();
};

} // namespace priv

} // namespace sf
