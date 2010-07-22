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

#ifndef SFML_SPRITE_HPP
#define SFML_SPRITE_HPP

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/System/Resource.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Rect.hpp>


namespace sf
{
class Image;

////////////////////////////////////////////////////////////
/// \brief Drawable representation of an image, with its
///        own transformations, color, blend mode, etc.
///
////////////////////////////////////////////////////////////
class SFML_API Sprite : public Drawable
{
public :

    ////////////////////////////////////////////////////////////
    /// \brief Default constructor
    ///
    /// Creates an empty sprite with no source image.
    ///
    ////////////////////////////////////////////////////////////
    Sprite();

    ////////////////////////////////////////////////////////////
    /// \brief Construct the sprite from a source image
    ///
    /// \param image    Source image, that the sprite will display
    /// \param position Position of the sprite in the scene
    /// \param scale    Scale factor of the sprite
    /// \param rotation Rotation angle, in degrees
    /// \param color    Global color of the sprite
    ///
    /// \see SetImage
    ///
    ////////////////////////////////////////////////////////////
    explicit Sprite(const Image& image, const Vector2f& position = Vector2f(0, 0), const Vector2f& scale = Vector2f(1, 1), float rotation = 0.f, const Color& color = Color(255, 255, 255, 255));

    ////////////////////////////////////////////////////////////
    /// \brief Change the source image of the sprite
    ///
    /// The \a image argument refers to an image that must
    /// exist as long as the sprite uses it. Indeed, the sprite
    /// doesn't store its own copy of the image, but rather keeps
    /// a pointer to the one that you passed to this function.
    /// If the source image is destroyed and the sprite tries to
    /// use it, it may appear as a white rectangle.
    /// If \a adjustToNewSize is true, the SubRect property of
    /// the sprite is adjusted to the size of the new image. If
    /// it is false, the SubRect is unchanged.
    ///
    /// \param image           New image
    /// \param adjustToNewSize Should the sub-rect be adjusted to the size of the new image?
    ///
    /// \see GetImage, SetSubRect
    ///
    ////////////////////////////////////////////////////////////
    void SetImage(const Image& image, bool adjustToNewSize = false);

    ////////////////////////////////////////////////////////////
    /// \brief Set the part of the image that the sprite will display
    ///
    /// The sub-rectangle is useful when you don't want to display
    /// the whole image, but rather a part of it.
    /// By default, the sub-rectangle covers the entire image.
    ///
    /// \param rectangle Rectangle defining the region of the image to display
    ///
    /// \see GetSubRect, SetImage
    ///
    ////////////////////////////////////////////////////////////
    void SetSubRect(const IntRect& rectangle);

    ////////////////////////////////////////////////////////////
    /// \brief Change the size of the sprite
    ///
    /// This function is just a shortcut that calls SetScale
    /// with the proper values, calculated from the size of
    /// the current subrect.
    /// If \a width or \a height is not strictly positive,
    /// this functions does nothing.
    ///
    /// \param width  New width of the sprite
    /// \param height New height of the sprite
    ///
    /// \see GetSize
    ///
    ////////////////////////////////////////////////////////////
    void Resize(float width, float height);

    ////////////////////////////////////////////////////////////
    /// \brief Change the size of the sprite
    ///
    /// This function is just a shortcut that calls SetScale
    /// with the proper values, calculated from the size of
    /// the current subrect.
    /// If \a size.x or \a size.y is not strictly positive,
    /// this functions does nothing.
    ///
    /// \param size New size of the sprite
    ///
    /// \see GetSize
    ///
    ////////////////////////////////////////////////////////////
    void Resize(const Vector2f& size);

    ////////////////////////////////////////////////////////////
    /// \brief Flip the sprite horizontally
    ///
    /// \param flipped True to flip the sprite
    ///
    /// \see FlipY
    ///
    ////////////////////////////////////////////////////////////
    void FlipX(bool flipped);

    ////////////////////////////////////////////////////////////
    /// \brief Flip the sprite vertically
    ///
    /// \param flipped True to flip the sprite
    ///
    /// \see FlipX
    ///
    ////////////////////////////////////////////////////////////
    void FlipY(bool flipped);

    ////////////////////////////////////////////////////////////
    /// \brief Get the source image of the sprite
    ///
    /// If the sprite has no source image, or if the image
    /// doesn't exist anymore, a NULL pointer is returned.
    /// The returned pointer is const, which means that you can't
    /// modify the image when you retrieve it with this function.
    ///
    /// \return Pointer to the sprite's image
    ///
    /// \see SetImage
    ///
    ////////////////////////////////////////////////////////////
    const Image* GetImage() const;

    ////////////////////////////////////////////////////////////
    /// \brief Get the region of the image displayed by the sprite
    ///
    /// \return Rectangle defining the region of the image
    ///
    /// \see SetSubRect
    ///
    ////////////////////////////////////////////////////////////
    const IntRect& GetSubRect() const;

    ////////////////////////////////////////////////////////////
    /// \brief Get the global size of the sprite
    ///
    /// This function is a shortcut that multiplies the
    /// size of the subrect by the scale factors.
    ///
    /// \return Size of the sprite
    ///
    /// \see Resize
    ///
    ////////////////////////////////////////////////////////////
    Vector2f GetSize() const;

    ////////////////////////////////////////////////////////////
    /// \brief Get the color of a given pixel in the sprite
    ///
    /// This function returns the source image pixel, multiplied
    /// by the global color of the sprite.
    /// The input point must be in local coordinates. If you have
    /// a global point, you can use the TransformToLocal function
    /// to make it local.
    /// This function doesn't perform any check, you must ensure that
    /// the \a x and \a y coordinates are not out of bounds.
    ///
    /// \param x X coordinate of the pixel to get
    /// \param y Y coordinate of the pixel to get
    ///
    /// \return Color of the pixel
    ///
    ////////////////////////////////////////////////////////////
    Color GetPixel(unsigned int x, unsigned int y) const;

protected :

    ////////////////////////////////////////////////////////////
    /// \brief Draw the object to a render target
    ///
    /// \param target   Render target
    /// \param renderer Renderer providing low-level rendering commands
    ///
    ////////////////////////////////////////////////////////////
    virtual void Render(RenderTarget& target, Renderer& renderer) const;

private :

    ////////////////////////////////////////////////////////////
    // Member data
    ////////////////////////////////////////////////////////////
    ResourcePtr<Image> myImage;      ///< Image used to draw the sprite
    IntRect            mySubRect;    ///< Sub-rectangle of source image to assign to the sprite
    bool               myIsFlippedX; ///< Is the sprite flipped on the X axis ?
    bool               myIsFlippedY; ///< Is the sprite flipped on the Y axis ?
};

} // namespace sf


#endif // SFML_SPRITE_HPP


////////////////////////////////////////////////////////////
/// \class sf::Sprite
/// \ingroup graphics
///
/// sf::Sprite is a drawable class that allows to easily display
/// an image (or a part of it) on a render target.
///
/// It inherits all the functions from sf::Drawable:
/// position, rotation, scale, origin, global color and blend
/// mode. It also adds sprite-specific properties such as the
/// image to use, the part of it to display, and some convenience
/// functions to flip or resize the sprite.
///
/// sf::Sprite works in combination with the sf::Image class, which
/// loads and provides the pixel data of a given image.
///
/// The separation of sf::Sprite and sf::Image allows more flexibility
/// and better performances: indeed a sf::Image is a heavy resource,
/// and any operation on it is slow (often too slow for real-time
/// applications). On the other side, a sf::Sprite is a lightweight
/// object which can use the pixel data of a sf::Image and draw
/// it with its own transformation / color / blending attributes.
///
/// It is important to note that the sf::Sprite instance doesn't
/// copy the image that it uses, it only keeps a reference to it.
/// Thus, a sf::Image must not be destructed while it is
/// used by a sf::Sprite (i.e. never write a function that
/// uses a local sf::Image instance for creating a sprite).
///
/// Usage example:
/// \code
/// // Declare and load an image
/// sf::Image image;
/// image.LoadFromFile("image.png");
/// 
/// // Create a sprite
/// sf::Sprite sprite;
/// sprite.SetImage(image);
/// sprite.SetSubRect(sf::IntRect(10, 10, 50, 30));
/// sprite.Resize(100, 60);
///
/// // Display it
/// window.Draw(sprite); // window is a sf::RenderWindow
/// \endcode
///
/// \see sf::Image
///
////////////////////////////////////////////////////////////
