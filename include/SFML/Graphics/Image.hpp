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
#include <SFML/Window/GlResource.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <string>
#include <vector>


namespace sf
{
class Renderer;
class RenderImage;
class RenderWindow;

////////////////////////////////////////////////////////////
/// \brief Class for loading, manipulating and saving images
///
////////////////////////////////////////////////////////////
class SFML_API Image : public Resource<Image>, GlResource
{
public :

    ////////////////////////////////////////////////////////////
    /// \brief Default constructor
    ///
    /// Creates an empty image.
    ///
    ////////////////////////////////////////////////////////////
    Image();

    ////////////////////////////////////////////////////////////
    /// \brief Copy constructor
    ///
    /// \param copy instance to copy
    ///
    ////////////////////////////////////////////////////////////
    Image(const Image& copy);

    ////////////////////////////////////////////////////////////
    /// \brief Destructor
    ///
    ////////////////////////////////////////////////////////////
    ~Image();

    ////////////////////////////////////////////////////////////
    /// \brief Load the image from a file on disk
    ///
    /// The supported image formats are bmp, png, tga, jpg, gif,
    /// psd, hdr and pic. Some format options are not supported,
    /// like progressive jpeg.
    /// The maximum size for an image depends on the graphics
    /// driver and can be retrieve with the GetMaximumSize function.
    /// If this function fails, the image is left unchanged.
    ///
    /// \param filename Path of the image file to load
    ///
    /// \return True if loading was successful
    ///
    /// \see LoadFromMemory, LoadFromPixels, SaveToFile
    ///
    ////////////////////////////////////////////////////////////
    bool LoadFromFile(const std::string& filename);

    ////////////////////////////////////////////////////////////
    /// \brief Load the image from a file in memory
    ///
    /// The supported image formats are bmp, png, tga, jpg, gif,
    /// psd, hdr and pic. Some format options are not supported,
    /// like progressive jpeg.
    /// The maximum size for an image depends on the graphics
    /// driver and can be retrieve with the GetMaximumSize function.
    /// If this function fails, the image is left unchanged.
    ///
    /// \param data Pointer to the file data in memory
    /// \param size Size of the data to load, in bytes
    ///
    /// \return True if loading was successful
    ///
    /// \see LoadFromFile, LoadFromPixels, SaveToFile
    ///
    ////////////////////////////////////////////////////////////
    bool LoadFromMemory(const void* data, std::size_t size);

    ////////////////////////////////////////////////////////////
    /// \brief Load the image from an array of pixels
    ///
    /// The \a pixels argument must point to an array of 32 bits
    /// RGBA pixels. In other words, the pixel array must have
    /// this memory layout: [r0 g0 b0 a0 r1 g1 b1 a1 r2...]
    /// If this function fails, the image is left unchanged.
    ///
    /// \param width  Width of the image
    /// \param height Height of the image
    /// \param pixels Pointer to the pixels in memory
    ///
    /// \return True if loading was successful
    ///
    /// \see LoadFromFile, LoadFromMemory, SaveToFile
    ///
    ////////////////////////////////////////////////////////////
    bool LoadFromPixels(unsigned int width, unsigned int height, const Uint8* pixels);

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
    /// \return True if saving was successful
    ///
    /// \see LoadFromFile, LoadFromMemory, LoadFromPixels
    ///
    ////////////////////////////////////////////////////////////
    bool SaveToFile(const std::string& filename) const;

    ////////////////////////////////////////////////////////////
    /// \brief Create the image and fill it with a unique color
    ///
    /// If this function fails, the image is left unchanged.
    ///
    /// \param width  Width of the image
    /// \param height Height of the image
    /// \param color  Fill color
    ///
    /// \return True if creation was successful
    ///
    ////////////////////////////////////////////////////////////
    bool Create(unsigned int width, unsigned int height, const Color& color = Color(0, 0, 0));

    ////////////////////////////////////////////////////////////
    /// \brief Create a transparency mask from a specified colorkey
    ///
    /// This function sets the alpha value of every pixel matching
    /// the given color to \a alpha (0 by default), so that they
    /// become transparent.
    ///
    /// \param color Color to make transparent
    /// \param alpha Alpha value to assign to transparent pixels
    ///
    ////////////////////////////////////////////////////////////
    void CreateMaskFromColor(const Color& color, Uint8 alpha = 0);

    ////////////////////////////////////////////////////////////
    /// \brief Copy pixels from another image onto this one
    ///
    /// This function does a slow pixel copy and should only
    /// be used at initialization time. It can be used to prepare
    /// a complex static image from several others, but if you
    /// need this kind of feature in real-time you'd better use
    /// sf::RenderImage.
    /// If \a sourceRect is empty, the whole image is copied.
    /// If \a applyAlpha is set to true, the transparency of
    /// source pixels is applied. If it is false, the pixels are
    /// copied unchanged with their alpha value.
    ///
    /// \param source     Source image to copy
    /// \param destX      X coordinate of the destination position
    /// \param destY      Y coordinate of the destination position
    /// \param sourceRect Sub-rectangle of the source image to copy
    /// \param applyAlpha Should the copy take in account the source transparency?
    ///
    ////////////////////////////////////////////////////////////
    void Copy(const Image& source, unsigned int destX, unsigned int destY, const IntRect& sourceRect = IntRect(0, 0, 0, 0), bool applyAlpha = false);

    ////////////////////////////////////////////////////////////
    /// \brief Copy the contents of a window to the image
    ///
    /// If \a sourceRect is empty, the whole window is copied.
    /// Warning: this is a slow operation, if you need to draw
    /// dynamic contents to an image then use sf::RenderImage.
    /// If this function fails, the image is left unchanged.
    ///
    /// \param window     Window to capture
    /// \param sourceRect Sub-rectangle of the screen to copy
    ///
    /// \return True if copy was successful
    ///
    ////////////////////////////////////////////////////////////
    bool CopyScreen(RenderWindow& window, const IntRect& sourceRect = IntRect(0, 0, 0, 0));

    ////////////////////////////////////////////////////////////
    /// \brief Change the color of a pixel
    ///
    /// This function doesn't check the validity of the pixel
    /// coordinates, using out-of-range values will result in
    /// an undefined behaviour.
    ///
    /// \param x      X coordinate of pixel to change
    /// \param y      Y coordinate of pixel to change
    /// \param color  New color of the pixel
    ///
    /// \see GetPixel
    ///
    ////////////////////////////////////////////////////////////
    void SetPixel(unsigned int x, unsigned int y, const Color& color);

    ////////////////////////////////////////////////////////////
    /// \brief Get the color of a pixel
    ///
    /// This function doesn't check the validity of the pixel
    /// coordinates, using out-of-range values will result in
    /// an undefined behaviour.
    ///
    /// \param x X coordinate of pixel to get
    /// \param y Y coordinate of pixel to get
    ///
    /// \return Color of the pixel at coordinates (x, y)
    ///
    ////////////////////////////////////////////////////////////
    Color GetPixel(unsigned int x, unsigned int y) const;

    ////////////////////////////////////////////////////////////
    /// \brief Get a read-only pointer to the array of pixels
    ///
    /// The returned value points to an array of RGBA pixels made of
    /// 8 bits integers components. The size of the array is
    /// GetWidth() * GetHeight() * 4.
    /// Warning: the returned pointer may become invalid if you
    /// modify the image, so you should never store it for too long.
    /// If the image is empty, a null pointer is returned.
    ///
    /// \return Const pointer to the array of pixels
    ///
    ////////////////////////////////////////////////////////////
    const Uint8* GetPixelsPtr() const;

    ////////////////////////////////////////////////////////////
    /// \brief Update the whole image from an array of pixels
    ///
    /// The \a pixels array is assumed to have the same size as
    /// the image, and to store RGBA 32 bits pixels.
    /// See the other overload of this function to update only
    /// a sub-rectangle of the image.
    /// This function does nothing if \a pixels is null.
    ///
    /// \param pixels Array of pixels to write to the image
    ///
    ////////////////////////////////////////////////////////////
    void UpdatePixels(const Uint8* pixels);

    ////////////////////////////////////////////////////////////
    /// \brief Update a sub-rectangle of the image from an array of pixels
    ///
    /// The \a pixels array is assumed to store RGBA 32 bits pixels.
    /// Warning: for performances reasons, this function doesn't
    /// perform any check; thus you're responsible of ensuring that
    /// \a rectangle does not exceed the image size, and that
    /// \a pixels contains enough elements.
    /// This function does nothing if \a pixels is null.
    /// See the other overload of this function to update the
    /// whole image.
    ///
    /// \param rectangle Sub-rectangle of the image to update
    /// \param pixels    Array of pixels to write to the image
    ///
    ////////////////////////////////////////////////////////////
    void UpdatePixels(const Uint8* pixels, const IntRect& rectangle);

    ////////////////////////////////////////////////////////////
    /// \brief Activate the image for rendering
    ///
    /// This function is mainly used internally by the SFML
    /// render system. However it can be useful when
    /// using sf::Image together with OpenGL code (it calls
    /// glBindTexture).
    ///
    ////////////////////////////////////////////////////////////
    void Bind() const;

    ////////////////////////////////////////////////////////////
    /// \brief Enable or disable the smooth filter
    ///
    /// When the filter is activated, the image appears smoother
    /// so that pixels are less noticeable. However if you want
    /// the image to look exactly the same as its source file, you
    /// should disable it.
    /// The smooth filter is enabled by default.
    ///
    /// \param smooth True to enable smoothing, false to disable it
    ///
    /// \see IsSmooth
    ///
    ////////////////////////////////////////////////////////////
    void SetSmooth(bool smooth);

    ////////////////////////////////////////////////////////////
    /// \brief Tell whether the smooth filter is enabled or not
    ///
    /// \return True if smoothing is enabled, false if it is disabled
    ///
    /// \see SetSmooth
    ///
    ////////////////////////////////////////////////////////////
    bool IsSmooth() const;

    ////////////////////////////////////////////////////////////
    /// \brief Return the width of the image
    ///
    /// \return Width in pixels
    ///
    /// \see GetHeight
    ///
    ////////////////////////////////////////////////////////////
    unsigned int GetWidth() const;

    ////////////////////////////////////////////////////////////
    /// \brief Return the height of the image
    ///
    /// \return Height in pixels
    ///
    /// \see GetWidth
    ///
    ////////////////////////////////////////////////////////////
    unsigned int GetHeight() const;

    ////////////////////////////////////////////////////////////
    /// \brief Convert a rectangle of pixels into texture coordinates
    ///
    /// This function is used by code that needs to map the image
    /// to some OpenGL geometry. It converts the source rectangle,
    /// expressed in pixels, to float coordinates in the range [0, 1].
    ///
    /// \param rectangle Rectangle to convert
    ///
    /// \return Texture coordinates corresponding to \a rectangle
    ///
    ////////////////////////////////////////////////////////////
    FloatRect GetTexCoords(const IntRect& rectangle) const;

    ////////////////////////////////////////////////////////////
    /// \brief Get the maximum image size allowed
    ///
    /// This maximum size is defined by the graphics driver.
    /// You can expect a value of 512 pixels for low-end graphics
    /// card, and up to 8192 pixels for newer hardware.
    ///
    /// \return Maximum size allowed for images, in pixels
    ///
    ////////////////////////////////////////////////////////////
    static unsigned int GetMaximumSize();

    ////////////////////////////////////////////////////////////
    /// \brief Overload of assignment operator
    ///
    /// \param right Instance to assign
    ///
    /// \return Reference to self
    ///
    ////////////////////////////////////////////////////////////
    Image& operator =(const Image& right);

private :

    friend class Renderer;
    friend class RenderImage;

    ////////////////////////////////////////////////////////////
    /// \brief Get a valid image size according to hardware support
    ///
    /// This function checks whether the graphics driver supports
    /// non power of two sizes or not, and adjusts the size
    /// accordingly.
    /// The returned size is greater than or equal to the original size.
    ///
    /// \param Size size to convert
    ///
    /// \return Valid nearest size (greater than or equal to specified size)
    ///
    ////////////////////////////////////////////////////////////
    static unsigned int GetValidSize(unsigned int size);

    ////////////////////////////////////////////////////////////
    /// \brief Create the OpenGL texture
    ///
    /// If this function fails, the image's members are left unchanged.
    ///
    /// \param width Texture width
    /// \param height Texture height
    ///
    /// \return True if texture was successfully created, false if it failed
    ///
    ////////////////////////////////////////////////////////////
    bool CreateTexture(unsigned int width, unsigned int height);

    ////////////////////////////////////////////////////////////
    /// \brief Make sure that the texture in video memory is
    ///        synchronized with the pixels cache
    ///
    ////////////////////////////////////////////////////////////
    void EnsureTextureUpdate() const;

    ////////////////////////////////////////////////////////////
    /// \brief Make sure the pixels cache is synchronized with
    ///        the texture in video memory
    ///
    ////////////////////////////////////////////////////////////
    void EnsureArrayUpdate() const;

    ////////////////////////////////////////////////////////////
    /// \brief Make sure that the image is ready to be used
    ///
    ////////////////////////////////////////////////////////////
    void Use() const;

    ////////////////////////////////////////////////////////////
    // Types
    ////////////////////////////////////////////////////////////
    typedef std::vector<Uint8> ColorArray; ///< Array of colors

    ////////////////////////////////////////////////////////////
    // Member data
    ////////////////////////////////////////////////////////////
    unsigned int       myWidth;          ///< Image width
    unsigned int       myHeight;         ///< Image Height
    unsigned int       myTextureWidth;   ///< Actual texture width (can be greater than image width because of padding)
    unsigned int       myTextureHeight;  ///< Actual texture height (can be greater than image height because of padding)
    unsigned int       myTexture;        ///< Internal texture identifier
    bool               myIsSmooth;       ///< Status of the smooth filter
    mutable ColorArray myPixels;         ///< Pixels of the image
    mutable bool       myTextureUpdated; ///< Status of synchronization between pixels in central memory and the internal texture un video memory
    mutable bool       myArrayUpdated;   ///< Status of synchronization between pixels in central memory and the internal texture un video memory
    mutable bool       myPixelsFlipped;  ///< To work around the inconsistency in Y orientation
};

} // namespace sf


#endif // SFML_IMAGE_HPP


////////////////////////////////////////////////////////////
/// \class sf::Image
/// \ingroup graphics
///
/// sf::Image is an abstraction to manipulate images
/// as bidimensional arrays of pixels. The class provides
/// functions to load, read, write and save pixels, as well
/// as many other useful functions to...
///
/// sf::Image can handle a unique internal representation of
/// pixels, which is RGBA 32 bits. This means that a pixel
/// must be composed of 8 bits red, green, blue and alpha
/// channels -- just like a sf::Color.
/// All the functions that return an array of pixels follow
/// this rule, and all parameters that you pass to sf::Image
/// functions (such as LoadFromPixels or UpdatePixels) must
/// use this representation as well.
///
/// A sf::Image can be copied, but it is a heavy resource and
/// if possible you should always use [const] references to
/// pass or return them to avoid useless copies.
///
/// Usage example:
/// \code
/// // Load an image file
/// sf::Image background;
/// if (!background.LoadFromFile("background.jpg"))
///     return -1;
///
/// // Create a 20x20 image filled with black color
/// sf::Image image;
/// if (!image.Create(20, 20, sf::Color::Black))
///     return -1;
///
/// // Copy image1 on image2 at position (10, 10)
/// image.Copy(background, 10, 10);
///
/// // Make the top-left pixel transparent
/// sf::Color color = image.GetPixel(0, 0);
/// color.a = 0;
/// image.SetPixel(0, 0, color);
///
/// // Save the image to a file
/// if (!image.SaveToFile("result.png"))
///     return -1;
/// \endcode
///
/// \see sf::Sprite
///
////////////////////////////////////////////////////////////
