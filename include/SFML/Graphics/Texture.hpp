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

#ifndef SFML_TEXTURE_HPP
#define SFML_TEXTURE_HPP

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Graphics/Image.hpp>
#include <SFML/Window/GlResource.hpp>


namespace sf
{
class Window;
class Renderer;
class RenderTexture;
class InputStream;

////////////////////////////////////////////////////////////
/// \brief Image living on the graphics card that can be used for drawing
///
////////////////////////////////////////////////////////////
class SFML_API Texture : GlResource
{
public :

    ////////////////////////////////////////////////////////////
    /// \brief Default constructor
    ///
    /// Creates an empty texture.
    ///
    ////////////////////////////////////////////////////////////
    Texture();

    ////////////////////////////////////////////////////////////
    /// \brief Copy constructor
    ///
    /// \param copy instance to copy
    ///
    ////////////////////////////////////////////////////////////
    Texture(const Texture& copy);

    ////////////////////////////////////////////////////////////
    /// \brief Destructor
    ///
    ////////////////////////////////////////////////////////////
    ~Texture();

    ////////////////////////////////////////////////////////////
    /// \brief Create the texture
    ///
    /// If this function fails, the texture is left unchanged.
    ///
    /// \param width  Width of the texture
    /// \param height Height of the texture
    ///
    /// \return True if creation was successful
    ///
    ////////////////////////////////////////////////////////////
    bool Create(unsigned int width, unsigned int height);

    ////////////////////////////////////////////////////////////
    /// \brief Load the texture from a file on disk
    ///
    /// This function is a shortcut for the following code:
    /// \code
    /// sf::Image image;
    /// image.LoadFromFile(filename);
    /// texture.LoadFromImage(image, area);
    /// \endcode
    ///
    /// The \a area argument can be used to load only a sub-rectangle
    /// of the whole image. If you want the entire image then leave
    /// the default value (which is an empty IntRect).
    /// If the \a area rectangle crosses the bounds of the image, it
    /// is adjusted to fit the image size. 
    ///
    /// The maximum size for a texture depends on the graphics
    /// driver and can be retrieved with the GetMaximumSize function.
    ///
    /// If this function fails, the texture is left unchanged.
    ///
    /// \param filename Path of the image file to load
    /// \param area     Area of the image to load
    ///
    /// \return True if loading was successful
    ///
    /// \see LoadFromMemory, LoadFromStream, LoadFromImage
    ///
    ////////////////////////////////////////////////////////////
    bool LoadFromFile(const std::string& filename, const IntRect& area = IntRect());

    ////////////////////////////////////////////////////////////
    /// \brief Load the texture from a file in memory
    ///
    /// This function is a shortcut for the following code:
    /// \code
    /// sf::Image image;
    /// image.LoadFromMemory(data, size);
    /// texture.LoadFromImage(image, area);
    /// \endcode
    ///
    /// The \a area argument can be used to load only a sub-rectangle
    /// of the whole image. If you want the entire image then leave
    /// the default value (which is an empty IntRect).
    /// If the \a area rectangle crosses the bounds of the image, it
    /// is adjusted to fit the image size. 
    ///
    /// The maximum size for a texture depends on the graphics
    /// driver and can be retrieved with the GetMaximumSize function.
    ///
    /// If this function fails, the texture is left unchanged.
    ///
    /// \param data Pointer to the file data in memory
    /// \param size Size of the data to load, in bytes
    /// \param area Area of the image to load
    ///
    /// \return True if loading was successful
    ///
    /// \see LoadFromFile, LoadFromStream, LoadFromImage
    ///
    ////////////////////////////////////////////////////////////
    bool LoadFromMemory(const void* data, std::size_t size, const IntRect& area = IntRect());

    ////////////////////////////////////////////////////////////
    /// \brief Load the texture from a file in memory
    ///
    /// This function is a shortcut for the following code:
    /// \code
    /// sf::Image image;
    /// image.LoadFromStream(stream);
    /// texture.LoadFromImage(image, area);
    /// \endcode
    ///
    /// The \a area argument can be used to load only a sub-rectangle
    /// of the whole image. If you want the entire image then leave
    /// the default value (which is an empty IntRect).
    /// If the \a area rectangle crosses the bounds of the image, it
    /// is adjusted to fit the image size. 
    ///
    /// The maximum size for a texture depends on the graphics
    /// driver and can be retrieved with the GetMaximumSize function.
    ///
    /// If this function fails, the texture is left unchanged.
    ///
    /// \param stream Source stream to read from
    /// \param area   Area of the image to load
    ///
    /// \return True if loading was successful
    ///
    /// \see LoadFromFile, LoadFromMemory, LoadFromImage
    ///
    ////////////////////////////////////////////////////////////
    bool LoadFromStream(sf::InputStream& stream, const IntRect& area = IntRect());

    ////////////////////////////////////////////////////////////
    /// \brief Load the texture from an image
    ///
    /// The \a area argument can be used to load only a sub-rectangle
    /// of the whole image. If you want the entire image then leave
    /// the default value (which is an empty IntRect).
    /// If the \a area rectangle crosses the bounds of the image, it
    /// is adjusted to fit the image size. 
    ///
    /// The maximum size for a texture depends on the graphics
    /// driver and can be retrieved with the GetMaximumSize function.
    ///
    /// If this function fails, the texture is left unchanged.
    ///
    /// \param image Image to load into the texture
    /// \param area  Area of the image to load
    ///
    /// \return True if loading was successful
    ///
    /// \see LoadFromFile, LoadFromMemory
    ///
    ////////////////////////////////////////////////////////////
    bool LoadFromImage(const Image& image, const IntRect& area = IntRect());

    ////////////////////////////////////////////////////////////
    /// \brief Return the width of the texture
    ///
    /// \return Width in pixels
    ///
    /// \see GetHeight
    ///
    ////////////////////////////////////////////////////////////
    unsigned int GetWidth() const;

    ////////////////////////////////////////////////////////////
    /// \brief Return the height of the texture
    ///
    /// \return Height in pixels
    ///
    /// \see GetWidth
    ///
    ////////////////////////////////////////////////////////////
    unsigned int GetHeight() const;

    ////////////////////////////////////////////////////////////
    /// \brief Copy the texture pixels to an image
    ///
    /// This function performs a slow operation that downloads
    /// the texture's pixels from the graphics card and copies
    /// them to a new image, potentially applying transformations
    /// to pixels if necessary (texture may be padded or flipped).
    ///
    /// \return Image containing the texture's pixels
    ///
    /// \see LoadFromImage
    ///
    ////////////////////////////////////////////////////////////
    Image CopyToImage() const;

    ////////////////////////////////////////////////////////////
    /// \brief Update the whole texture from an array of pixels
    ///
    /// The \a pixels array is assumed to have the same size as
    /// the \a area rectangle, and to contain 32-bits RGBA pixels.
    ///
    /// No additional check is performed on the size of the pixel
    /// array, passing invalid arguments will lead to an undefined
    /// behaviour.
    ///
    /// This function does nothing if \a pixels is null or if the
    /// texture was not previously created.
    ///
    /// \param pixels Array of pixels to copy to the texture
    ///
    ////////////////////////////////////////////////////////////
    void Update(const Uint8* pixels);

    ////////////////////////////////////////////////////////////
    /// \brief Update a part of the texture from an array of pixels
    ///
    /// The size of the \a pixels array must match the \a width and
    /// \a height arguments, and it must contain 32-bits RGBA pixels.
    ///
    /// No additional check is performed on the size of the pixel
    /// array or the bounds of the area to update, passing invalid
    /// arguments will lead to an undefined behaviour.
    ///
    /// This function does nothing if \a pixels is null or if the
    /// texture was not previously created.
    ///
    /// \param pixels Array of pixels to copy to the texture
    /// \param width  Width of the pixel region contained in \a pixels
    /// \param height Height of the pixel region contained in \a pixels
    /// \param x      X offset in the texture where to copy the source pixels
    /// \param y      Y offset in the texture where to copy the source pixels
    ///
    ////////////////////////////////////////////////////////////
    void Update(const Uint8* pixels, unsigned int width, unsigned int height, unsigned int x, unsigned int y);

    ////////////////////////////////////////////////////////////
    /// \brief Update the texture from an image
    ///
    /// Although the source image can be smaller than the texture,
    /// this function is usually used for updating the whole texture.
    /// The other overload, which has (x, y) additional arguments,
    /// is more convenient for updating a sub-area of the texture.
    ///
    /// No additional check is performed on the size of the image,
    /// passing an image bigger than the texture will lead to an
    /// undefined behaviour.
    ///
    /// This function does nothing if the texture was not
    /// previously created.
    ///
    /// \param image Image to copy to the texture
    ///
    ////////////////////////////////////////////////////////////
    void Update(const Image& image);

    ////////////////////////////////////////////////////////////
    /// \brief Update a part of the texture from an image
    ///
    /// No additional check is performed on the size of the image,
    /// passing an invalid combination of image size and offset
    /// will lead to an undefined behaviour.
    ///
    /// This function does nothing if the texture was not
    /// previously created.
    ///
    /// \param image Image to copy to the texture
    /// \param x     X offset in the texture where to copy the source image
    /// \param y     Y offset in the texture where to copy the source image
    ///
    ////////////////////////////////////////////////////////////
    void Update(const Image& image, unsigned int x, unsigned int y);

    ////////////////////////////////////////////////////////////
    /// \brief Update the texture from the contents of a window
    ///
    /// Although the source window can be smaller than the texture,
    /// this function is usually used for updating the whole texture.
    /// The other overload, which has (x, y) additional arguments,
    /// is more convenient for updating a sub-area of the texture.
    ///
    /// No additional check is performed on the size of the window,
    /// passing a window bigger than the texture will lead to an
    /// undefined behaviour.
    ///
    /// This function does nothing if either the texture or the window
    /// was not previously created.
    ///
    /// \param window Window to copy to the texture
    ///
    ////////////////////////////////////////////////////////////
    void Update(const Window& window);

    ////////////////////////////////////////////////////////////
    /// \brief Update a part of the texture from the contents of a window
    ///
    /// No additional check is performed on the size of the window,
    /// passing an invalid combination of window size and offset
    /// will lead to an undefined behaviour.
    ///
    /// This function does nothing if either the texture or the window
    /// was not previously created.
    ///
    /// \param window Window to copy to the texture
    /// \param x      X offset in the texture where to copy the source window
    /// \param y      Y offset in the texture where to copy the source window
    ///
    ////////////////////////////////////////////////////////////
    void Update(const Window& window, unsigned int x, unsigned int y);

    ////////////////////////////////////////////////////////////
    /// \brief Activate the texture for rendering
    ///
    /// This function is mainly used internally by the SFML
    /// render system. However it can be useful when
    /// using sf::Texture together with OpenGL code (this function
    /// is equivalent to glBindTexture).
    ///
    ////////////////////////////////////////////////////////////
    void Bind() const;

    ////////////////////////////////////////////////////////////
    /// \brief Enable or disable the smooth filter
    ///
    /// When the filter is activated, the texture appears smoother
    /// so that pixels are less noticeable. However if you want
    /// the texture to look exactly the same as its source file,
    /// you should leave it disabled.
    /// The smooth filter is disabled by default.
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
    /// \brief Convert a rectangle of pixels into texture coordinates
    ///
    /// This function is used by code that needs to map the texture
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
    /// \brief Overload of assignment operator
    ///
    /// \param right Instance to assign
    ///
    /// \return Reference to self
    ///
    ////////////////////////////////////////////////////////////
    Texture& operator =(const Texture& right);

    ////////////////////////////////////////////////////////////
    /// \brief Get the maximum texture size allowed
    ///
    /// This maximum size is defined by the graphics driver.
    /// You can expect a value of 512 pixels for low-end graphics
    /// card, and up to 8192 pixels or more for newer hardware.
    ///
    /// \return Maximum size allowed for textures, in pixels
    ///
    ////////////////////////////////////////////////////////////
    static unsigned int GetMaximumSize();

private :

    friend class Renderer;
    friend class RenderTexture;

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
    // Member data
    ////////////////////////////////////////////////////////////
    unsigned int myWidth;         ///< Image width
    unsigned int myHeight;        ///< Image Height
    unsigned int myTextureWidth;  ///< Actual texture width (can be greater than image width because of padding)
    unsigned int myTextureHeight; ///< Actual texture height (can be greater than image height because of padding)
    unsigned int myTexture;       ///< Internal texture identifier
    bool         myIsSmooth;      ///< Status of the smooth filter
    mutable bool myPixelsFlipped; ///< To work around the inconsistency in Y orientation
};

} // namespace sf


#endif // SFML_TEXTURE_HPP

////////////////////////////////////////////////////////////
/// \class sf::Texture
/// \ingroup graphics
///
/// sf::Texture stores pixels that can be drawn, with a sprite
/// for example. A texture lives in the graphics card memory,
/// therefore it is very fast to draw a texture to a render target,
/// or copy a render target to a texture (the graphics card can
/// access both directly).
///
/// Being stored in the graphics card memory has some drawbacks.
/// A texture cannot be manipulated as freely as a sf::Image,
/// you need to prepare the pixels first and then upload them
/// to the texture in a single operation (see Texture::Update).
///
/// sf::Texture makes it easy to convert from/to sf::Image, but
/// keep in mind that these calls require transfers between
/// the graphics card and the central memory, therefore they are
/// slow operations.
///
/// A texture can be loaded from an image, but also directly
/// from a file/memory/stream. The necessary shortcuts are defined
/// so that you don't need an image first for the most common cases.
/// However, if you want to perform some modifications on the pixels
/// before creating the final texture, you can load your file to a
/// sf::Image, do whatever you need with the pixels, and then call
/// Texture::LoadFromImage.
/// 
/// Since they live in the graphics card memory, the pixels of a texture
/// cannot be accessed without a slow copy first. And they cannot be
/// accessed individually. Therefore, if you need to read the texture's
/// pixels (like for pixel-perfect collisions), it is recommended to
/// store the collision information separately, for example in an array
/// of booleans.
///
/// Like sf::Image, sf::Texture can handle a unique internal
/// representation of pixels, which is RGBA 32 bits. This means
/// that a pixel must be composed of 8 bits red, green, blue and
/// alpha channels -- just like a sf::Color.
///
/// Usage example:
/// \code
/// // This example shows the most common use of sf::Texture:
/// // drawing a sprite
///
/// // Load a texture from a file
/// sf::Texture texture;
/// if (!texture.LoadFromFile("texture.png"))
///     return -1;
///
/// // Assign it to a sprite
/// sf::Sprite sprite;
/// sprite.SetTexture(texture);
///
/// // Draw the textured sprite
/// window.Draw(sprite); // window is a sf::RenderWindow
/// \endcode
///
/// \code
/// // This example shows another common use of sf::Texture:
/// // streaming real-time data, like video frames
///
/// // Create an empty texture
/// sf::Texture texture;
/// if (!texture.Create(640, 480))
///     return -1;
///
/// // Create a sprite that will display the texture
/// sf::Sprite sprite(texture);
///
/// while (...) // the main loop
/// {
///     ...
///
///     // update the texture
///     sf::Uint8* pixels = ...; // get a fresh chunk of pixels (the next frame of a movie, for example)
///     texture.Update(pixels);
///
///     // draw it
///     window.Draw(sprite);
///
///     ...
/// }
///
/// \endcode
///
/// \see sf::Sprite, sf::Image, sf::RenderTexture
///
////////////////////////////////////////////////////////////
