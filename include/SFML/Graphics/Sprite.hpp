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
/// Sprite defines a sprite : texture, transformations,
/// color, and draw on screen
////////////////////////////////////////////////////////////
class SFML_API Sprite : public Drawable
{
public :

    ////////////////////////////////////////////////////////////
    /// Default constructor
    ///
    ////////////////////////////////////////////////////////////
    Sprite();

    ////////////////////////////////////////////////////////////
    /// Construct the sprite from a source image
    ///
    /// \param Img :      Image of the sprite
    /// \param Position : Position of the sprite (0, 0 by default)
    /// \param Scale :    Scale factor (1, 1 by default)
    /// \param Rotation : Orientation, in degrees (0 by default)
    /// \param Col :      Color of the sprite (white by default)
    ///
    ////////////////////////////////////////////////////////////
    Sprite(const Image& Img, const Vector2f& Position = Vector2f(0, 0), const Vector2f& Scale = Vector2f(1, 1), float Rotation = 0.f, const Color& Col = Color(255, 255, 255, 255));

    ////////////////////////////////////////////////////////////
    /// Change the image of the sprite
    ///
    /// \param Img : New image
    ///
    ////////////////////////////////////////////////////////////
    void SetImage(const Image& Img);

    ////////////////////////////////////////////////////////////
    /// Set the sub-rectangle of the sprite inside the source image.
    /// By default, the subrect covers the entire source image
    ///
    /// \param SubRect : New sub-rectangle
    ///
    ////////////////////////////////////////////////////////////
    void SetSubRect(const IntRect& SubRect);

    ////////////////////////////////////////////////////////////
    /// Resize the sprite (by changing its scale factors) (take 2 values).
    /// The default size is defined by the subrect
    ///
    /// \param Width :  New width (must be strictly positive)
    /// \param Height : New height (must be strictly positive)
    ///
    ////////////////////////////////////////////////////////////
    void Resize(float Width, float Height);

    ////////////////////////////////////////////////////////////
    /// Resize the sprite (by changing its scale factors) (take a 2D vector).
    /// The default size is defined by the subrect
    ///
    /// \param Size : New size (both coordinates must be strictly positive)
    ///
    ////////////////////////////////////////////////////////////
    void Resize(const Vector2f& Size);

    ////////////////////////////////////////////////////////////
    /// Flip the sprite horizontally
    ///
    /// \param Flipped : True to flip the sprite
    ///
    ////////////////////////////////////////////////////////////
    void FlipX(bool Flipped);

    ////////////////////////////////////////////////////////////
    /// Flip the sprite vertically
    ///
    /// \param Flipped : True to flip the sprite
    ///
    ////////////////////////////////////////////////////////////
    void FlipY(bool Flipped);

    ////////////////////////////////////////////////////////////
    /// Get the source image of the sprite
    ///
    /// \return Pointer to the image (can be NULL)
    ///
    ////////////////////////////////////////////////////////////
    const Image* GetImage() const;

    ////////////////////////////////////////////////////////////
    /// Get the sub-rectangle of the sprite inside the source image
    ///
    /// \return Sub-rectangle
    ///
    ////////////////////////////////////////////////////////////
    const IntRect& GetSubRect() const;

    ////////////////////////////////////////////////////////////
    /// Get the sprite size
    ///
    /// \return Size of the sprite
    ///
    ////////////////////////////////////////////////////////////
    Vector2f GetSize() const;

    ////////////////////////////////////////////////////////////
    /// Get the color of a given pixel in the sprite
    /// (point is in local coordinates)
    ///
    /// \param X : X coordinate of the pixel to get
    /// \param Y : Y coordinate of the pixel to get
    ///
    /// \return Color of pixel (X, Y)
    ///
    ////////////////////////////////////////////////////////////
    Color GetPixel(unsigned int X, unsigned int Y) const;

protected :

    ////////////////////////////////////////////////////////////
    /// /see Drawable::Render
    ///
    ////////////////////////////////////////////////////////////
    virtual void Render(RenderTarget& Target) const;

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
