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
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Image.hpp>
#include <SFML/Graphics/GraphicsContext.hpp>


namespace sf
{
////////////////////////////////////////////////////////////
/// Default constructor
////////////////////////////////////////////////////////////
Sprite::Sprite() :
mySubRect   (0, 0, 1, 1),
myIsFlippedX(false),
myIsFlippedY(false)
{

}


////////////////////////////////////////////////////////////
/// Construct the sprite from a source image
////////////////////////////////////////////////////////////
Sprite::Sprite(const Image& Img, const Vector2f& Position, const Vector2f& Scale, float Rotation, const Color& Col) :
Drawable    (Position, Scale, Rotation, Col),
mySubRect   (0, 0, 1, 1),
myIsFlippedX(false),
myIsFlippedY(false)
{
    SetImage(Img);
}


////////////////////////////////////////////////////////////
/// Set the image of the sprite
////////////////////////////////////////////////////////////
void Sprite::SetImage(const Image& Img)
{
    // If there was no source image before and the new image is valid, adjust the source rectangle
    if (!myImage && (Img.GetWidth() > 0) && (Img.GetHeight() > 0))
    {
        SetSubRect(IntRect(0, 0, Img.GetWidth(), Img.GetHeight()));
    }

    // Assign the new image
    myImage = &Img;
}


////////////////////////////////////////////////////////////
/// Set the sub-rectangle of the sprite inside the source image
////////////////////////////////////////////////////////////
void Sprite::SetSubRect(const IntRect& SubRect)
{
    mySubRect = SubRect;
}


////////////////////////////////////////////////////////////
/// Resize the sprite (by changing its scale factors) (take 2 values).
/// The default size is defined by the subrect
////////////////////////////////////////////////////////////
void Sprite::Resize(float Width, float Height)
{
    int LocalWidth  = mySubRect.GetWidth();
    int LocalHeight = mySubRect.GetHeight();

    if ((LocalWidth > 0) && (LocalHeight > 0))
        SetScale(Width / LocalWidth, Height / LocalHeight);
}


////////////////////////////////////////////////////////////
/// Resize the object (by changing its scale factors) (take a 2D vector)
/// The default size is defined by the subrect
////////////////////////////////////////////////////////////
void Sprite::Resize(const Vector2f& Size)
{
    Resize(Size.x, Size.y);
}


////////////////////////////////////////////////////////////
/// Flip the sprite horizontally
////////////////////////////////////////////////////////////
void Sprite::FlipX(bool Flipped)
{
    myIsFlippedX = Flipped;
}


////////////////////////////////////////////////////////////
/// Flip the sprite vertically
////////////////////////////////////////////////////////////
void Sprite::FlipY(bool Flipped)
{
    myIsFlippedY = Flipped;
}


////////////////////////////////////////////////////////////
/// Get the source image of the sprite
////////////////////////////////////////////////////////////
const Image* Sprite::GetImage() const
{
    return myImage;
}


////////////////////////////////////////////////////////////
/// Get the sub-rectangle of the sprite inside the source image
////////////////////////////////////////////////////////////
const IntRect& Sprite::GetSubRect() const
{
    return mySubRect;
}


////////////////////////////////////////////////////////////
/// Get the sprite size
////////////////////////////////////////////////////////////
Vector2f Sprite::GetSize() const
{
    return Vector2f(mySubRect.GetWidth() * GetScale().x, mySubRect.GetHeight() * GetScale().y);
}


////////////////////////////////////////////////////////////
/// Get the color of a given pixel in the sprite
/// (point is in local coordinates)
////////////////////////////////////////////////////////////
Color Sprite::GetPixel(unsigned int X, unsigned int Y) const
{
    if (myImage)
    {
        unsigned int ImageX = mySubRect.Left + X;
        unsigned int ImageY = mySubRect.Top  + Y;

        if (myIsFlippedX) ImageX = mySubRect.GetWidth()  - ImageX - 1;
        if (myIsFlippedY) ImageY = mySubRect.GetHeight() - ImageY - 1;

        return myImage->GetPixel(ImageX, ImageY) * GetColor();
    }
    else
    {
        return GetColor();
    }
}


////////////////////////////////////////////////////////////
/// /see sfDrawable::Render
////////////////////////////////////////////////////////////
void Sprite::Render(RenderTarget&) const
{
    // Get the sprite size
    float Width  = static_cast<float>(mySubRect.GetWidth());
    float Height = static_cast<float>(mySubRect.GetHeight());

    // Check if the image is valid
    if (myImage && (myImage->GetWidth() > 0) && (myImage->GetHeight() > 0))
    {
        // Bind the texture
        myImage->Bind();

        // Calculate the texture coordinates
        FloatRect TexCoords = myImage->GetTexCoords(mySubRect);
        FloatRect Rect(myIsFlippedX ? TexCoords.Right  : TexCoords.Left,
                       myIsFlippedY ? TexCoords.Bottom : TexCoords.Top,
                       myIsFlippedX ? TexCoords.Left   : TexCoords.Right,
                       myIsFlippedY ? TexCoords.Top    : TexCoords.Bottom);

        // Draw the sprite's triangles
        glBegin(GL_QUADS);
            glTexCoord2f(Rect.Left,  Rect.Top);    glVertex2f(0,     0);
            glTexCoord2f(Rect.Left,  Rect.Bottom); glVertex2f(0,     Height);
            glTexCoord2f(Rect.Right, Rect.Bottom); glVertex2f(Width, Height);
            glTexCoord2f(Rect.Right, Rect.Top);    glVertex2f(Width, 0) ;
        glEnd();
    }
    else
    {
        // Disable texturing
        GLCheck(glDisable(GL_TEXTURE_2D));

        // Draw the sprite's triangles
        glBegin(GL_QUADS);
            glVertex2f(0,     0);
            glVertex2f(0,     Height);
            glVertex2f(Width, Height);
            glVertex2f(Width, 0);
        glEnd();
    }
}

} // namespace sf
