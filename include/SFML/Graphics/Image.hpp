////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007-2024 Laurent Gomila (laurent@sfml-dev.org)
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
#include <SFML/Graphics/Export.hpp>

#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Rect.hpp>

#include <SFML/System/Vector2.hpp>

#include <filesystem>
#include <optional>
#include <string_view>
#include <vector>

#include <cstddef>
#include <cstdint>


namespace sf
{
class InputStream;

////////////////////////////////////////////////////////////
/// \brief Class for loading, manipulating and saving images
///
////////////////////////////////////////////////////////////
class SFML_GRAPHICS_API Image
{
public:
    ////////////////////////////////////////////////////////////
    /// \brief Default constructor
    ///
    /// Constructs an image with width 0 and height 0.
    ///
    /// \see `resize`
    ///
    ////////////////////////////////////////////////////////////
    Image() = default;

    ////////////////////////////////////////////////////////////
    /// \brief Construct the image and fill it with a unique color
    ///
    /// \param size  Width and height of the image
    /// \param color Fill color
    ///
    ////////////////////////////////////////////////////////////
    explicit Image(Vector2u size, Color color = Color::Black);

    ////////////////////////////////////////////////////////////
    /// \brief Construct the image from an array of pixels
    ///
    /// The pixel array is assumed to contain 32-bits RGBA pixels,
    /// and have the given `size`. If not, this is an undefined behavior.
    /// If `pixels` is `nullptr`, an empty image is created.
    ///
    /// \param size   Width and height of the image
    /// \param pixels Array of pixels to copy to the image
    ///
    ////////////////////////////////////////////////////////////
    Image(Vector2u size, const std::uint8_t* pixels);

    ////////////////////////////////////////////////////////////
    /// \brief Construct the image from a file on disk
    ///
    /// The supported image formats are bmp, png, tga, jpg, gif,
    /// psd, hdr, pic and pnm. Some format options are not supported,
    /// like jpeg with arithmetic coding or ASCII pnm.
    ///
    /// \param filename Path of the image file to load
    ///
    /// \throws sf::Exception if loading was unsuccessful
    ///
    /// \see `loadFromFile`, `loadFromMemory`, `loadFromStream`
    ///
    ////////////////////////////////////////////////////////////
    explicit Image(const std::filesystem::path& filename);

    ////////////////////////////////////////////////////////////
    /// \brief Construct the image from a file in memory
    ///
    /// The supported image formats are bmp, png, tga, jpg, gif,
    /// psd, hdr, pic and pnm. Some format options are not supported,
    /// like jpeg with arithmetic coding or ASCII pnm.
    ///
    /// \param data Pointer to the file data in memory
    /// \param size Size of the data to load, in bytes
    ///
    /// \throws sf::Exception if loading was unsuccessful
    ///
    /// \see `loadFromFile`, `loadFromMemory`, `loadFromStream`
    ///
    ////////////////////////////////////////////////////////////
    Image(const void* data, std::size_t size);

    ////////////////////////////////////////////////////////////
    /// \brief Construct the image from a custom stream
    ///
    /// The supported image formats are bmp, png, tga, jpg, gif,
    /// psd, hdr, pic and pnm. Some format options are not supported,
    /// like jpeg with arithmetic coding or ASCII pnm.
    ///
    /// \param stream Source stream to read from
    ///
    /// \throws sf::Exception if loading was unsuccessful
    ///
    /// \see `loadFromFile`, `loadFromMemory`, `loadFromStream`
    ///
    ////////////////////////////////////////////////////////////
    explicit Image(InputStream& stream);

    ////////////////////////////////////////////////////////////
    /// \brief Resize the image and fill it with a unique color
    ///
    /// \param size  Width and height of the image
    /// \param color Fill color
    ///
    ////////////////////////////////////////////////////////////
    void resize(Vector2u size, Color color = Color::Black);

    ////////////////////////////////////////////////////////////
    /// \brief Resize the image from an array of pixels
    ///
    /// The pixel array is assumed to contain 32-bits RGBA pixels,
    /// and have the given `size`. If not, this is an undefined behavior.
    /// If `pixels` is `nullptr`, an empty image is created.
    ///
    /// \param size   Width and height of the image
    /// \param pixels Array of pixels to copy to the image
    ///
    ////////////////////////////////////////////////////////////
    void resize(Vector2u size, const std::uint8_t* pixels);

    ////////////////////////////////////////////////////////////
    /// \brief Load the image from a file on disk
    ///
    /// The supported image formats are bmp, png, tga, jpg, gif,
    /// psd, hdr, pic and pnm. Some format options are not supported,
    /// like jpeg with arithmetic coding or ASCII pnm.
    /// If this function fails, the image is left unchanged.
    ///
    /// \param filename Path of the image file to load
    ///
    /// \return `true` if loading was successful
    ///
    /// \see `loadFromMemory`, `loadFromStream`, `saveToFile`
    ///
    ////////////////////////////////////////////////////////////
    [[nodiscard]] bool loadFromFile(const std::filesystem::path& filename);

    ////////////////////////////////////////////////////////////
    /// \brief Load the image from a file in memory
    ///
    /// The supported image formats are bmp, png, tga, jpg, gif,
    /// psd, hdr, pic and pnm. Some format options are not supported,
    /// like jpeg with arithmetic coding or ASCII pnm.
    /// If this function fails, the image is left unchanged.
    ///
    /// \param data Pointer to the file data in memory
    /// \param size Size of the data to load, in bytes
    ///
    /// \return `true` if loading was successful
    ///
    /// \see `loadFromFile`, `loadFromStream`, `saveToMemory`
    ///
    ////////////////////////////////////////////////////////////
    [[nodiscard]] bool loadFromMemory(const void* data, std::size_t size);

    ////////////////////////////////////////////////////////////
    /// \brief Load the image from a custom stream
    ///
    /// The supported image formats are bmp, png, tga, jpg, gif,
    /// psd, hdr, pic and pnm. Some format options are not supported,
    /// like jpeg with arithmetic coding or ASCII pnm.
    /// If this function fails, the image is left unchanged.
    ///
    /// \param stream Source stream to read from
    ///
    /// \return `true` if loading was successful
    ///
    /// \see `loadFromFile`, `loadFromMemory`
    ///
    ////////////////////////////////////////////////////////////
    [[nodiscard]] bool loadFromStream(InputStream& stream);

    ////////////////////////////////////////////////////////////
    /// \brief Save the image to a file on disk
    ///
    /// The format of the image is automatically deduced from
    /// the extension. The supported image formats are bmp, png,
    /// tga and jpg. The destination file is overwritten
    /// if it already exists. This function fails if the image is empty.
    ///
    /// \param filename Path of the file to save
    ///
    /// \return `true` if saving was successful
    ///
    /// \see `saveToMemory`, `loadFromFile`
    ///
    ////////////////////////////////////////////////////////////
    [[nodiscard]] bool saveToFile(const std::filesystem::path& filename) const;

    ////////////////////////////////////////////////////////////
    /// \brief Save the image to a buffer in memory
    ///
    /// The format of the image must be specified.
    /// The supported image formats are bmp, png, tga and jpg.
    /// This function fails if the image is empty, or if
    /// the format was invalid.
    ///
    /// \param format Encoding format to use
    ///
    /// \return Buffer with encoded data if saving was successful,
    ///     otherwise `std::nullopt`
    ///
    /// \see `saveToFile`, `loadFromMemory`
    ///
    ////////////////////////////////////////////////////////////
    [[nodiscard]] std::optional<std::vector<std::uint8_t>> saveToMemory(std::string_view format) const;

    ////////////////////////////////////////////////////////////
    /// \brief Return the size (width and height) of the image
    ///
    /// \return Size of the image, in pixels
    ///
    ////////////////////////////////////////////////////////////
    [[nodiscard]] Vector2u getSize() const;

    ////////////////////////////////////////////////////////////
    /// \brief Create a transparency mask from a specified color-key
    ///
    /// This function sets the alpha value of every pixel matching
    /// the given color to `alpha` (0 by default), so that they
    /// become transparent.
    ///
    /// \param color Color to make transparent
    /// \param alpha Alpha value to assign to transparent pixels
    ///
    ////////////////////////////////////////////////////////////
    void createMaskFromColor(Color color, std::uint8_t alpha = 0);

    ////////////////////////////////////////////////////////////
    /// \brief Copy pixels from another image onto this one
    ///
    /// This function does a slow pixel copy and should not be
    /// used intensively. It can be used to prepare a complex
    /// static image from several others, but if you need this
    /// kind of feature in real-time you'd better use `sf::RenderTexture`.
    ///
    /// If `sourceRect` is empty, the whole image is copied.
    /// If `applyAlpha` is set to `true`, alpha blending is
    /// applied from the source pixels to the destination pixels
    /// using the \b over operator. If it is `false`, the source
    /// pixels are copied unchanged with their alpha value.
    ///
    /// See https://en.wikipedia.org/wiki/Alpha_compositing for
    /// details on the \b over operator.
    ///
    /// Note that this function can fail if either image is invalid
    /// (i.e. zero-sized width or height), or if `sourceRect` is
    /// not within the boundaries of the `source` parameter, or
    /// if the destination area is out of the boundaries of this image.
    ///
    /// On failure, the destination image is left unchanged.
    ///
    /// \param source     Source image to copy
    /// \param dest       Coordinates of the destination position
    /// \param sourceRect Sub-rectangle of the source image to copy
    /// \param applyAlpha Should the copy take into account the source transparency?
    ///
    /// \return `true` if the operation was successful, `false` otherwise
    ///
    ////////////////////////////////////////////////////////////
    [[nodiscard]] bool copy(const Image& source, Vector2u dest, const IntRect& sourceRect = {}, bool applyAlpha = false);

    ////////////////////////////////////////////////////////////
    /// \brief Change the color of a pixel
    ///
    /// This function doesn't check the validity of the pixel
    /// coordinates, using out-of-range values will result in
    /// an undefined behavior.
    ///
    /// \param coords Coordinates of pixel to change
    /// \param color  New color of the pixel
    ///
    /// \see `getPixel`
    ///
    ////////////////////////////////////////////////////////////
    void setPixel(Vector2u coords, Color color);

    ////////////////////////////////////////////////////////////
    /// \brief Get the color of a pixel
    ///
    /// This function doesn't check the validity of the pixel
    /// coordinates, using out-of-range values will result in
    /// an undefined behavior.
    ///
    /// \param coords Coordinates of pixel to change
    ///
    /// \return Color of the pixel at given coordinates
    ///
    /// \see `setPixel`
    ///
    ////////////////////////////////////////////////////////////
    [[nodiscard]] Color getPixel(Vector2u coords) const;

    ////////////////////////////////////////////////////////////
    /// \brief Get a read-only pointer to the array of pixels
    ///
    /// The returned value points to an array of RGBA pixels made of
    /// 8 bit integer components. The size of the array is
    /// `width * height * 4 (getSize().x * getSize().y * 4)`.
    /// Warning: the returned pointer may become invalid if you
    /// modify the image, so you should never store it for too long.
    /// If the image is empty, a null pointer is returned.
    ///
    /// \return Read-only pointer to the array of pixels
    ///
    ////////////////////////////////////////////////////////////
    [[nodiscard]] const std::uint8_t* getPixelsPtr() const;

    ////////////////////////////////////////////////////////////
    /// \brief Flip the image horizontally (left <-> right)
    ///
    ////////////////////////////////////////////////////////////
    void flipHorizontally();

    ////////////////////////////////////////////////////////////
    /// \brief Flip the image vertically (top <-> bottom)
    ///
    ////////////////////////////////////////////////////////////
    void flipVertically();

private:
    ////////////////////////////////////////////////////////////
    // Member data
    ////////////////////////////////////////////////////////////
    Vector2u                  m_size;   //!< Image size
    std::vector<std::uint8_t> m_pixels; //!< Pixels of the image
};

} // namespace sf


////////////////////////////////////////////////////////////
/// \class sf::Image
/// \ingroup graphics
///
/// `sf::Image` is an abstraction to manipulate images
/// as bi-dimensional arrays of pixels. The class provides
/// functions to load, read, write and save pixels, as well
/// as many other useful functions.
///
/// `sf::Image` can handle a unique internal representation of
/// pixels, which is RGBA 32 bits. This means that a pixel
/// must be composed of 8 bit red, green, blue and alpha
/// channels -- just like a `sf::Color`.
/// All the functions that return an array of pixels follow
/// this rule, and all parameters that you pass to `sf::Image`
/// functions (such as `loadFromMemory`) must use this
/// representation as well.
///
/// A `sf::Image` can be copied, but it is a heavy resource and
/// if possible you should always use [const] references to
/// pass or return them to avoid useless copies.
///
/// Usage example:
/// \code
/// // Load an image file from a file
/// const sf::Image background("background.jpg");
///
/// // Create a 20x20 image filled with black color
/// sf::Image image({20, 20}, sf::Color::Black);
///
/// // Copy background on image at position (10, 10)
/// if (!image.copy(background, {10, 10}))
///     return -1;
///
/// // Make the top-left pixel transparent
/// sf::Color color = image.getPixel({0, 0});
/// color.a = 0;
/// image.setPixel({0, 0}, color);
///
/// // Save the image to a file
/// if (!image.saveToFile("result.png"))
///     return -1;
/// \endcode
///
/// \see `sf::Texture`
///
////////////////////////////////////////////////////////////
