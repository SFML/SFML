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
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <cassert>


namespace sf
{
////////////////////////////////////////////////////////////
Text::Text() :
myString       (),
myFont         (&Font::GetDefaultFont()),
myCharacterSize(30),
myStyle        (Regular),
myVertices     (Quads),
myBounds       ()
{

}


////////////////////////////////////////////////////////////
Text::Text(const String& string, const Font& font, unsigned int characterSize) :
myString       (string),
myFont         (&font),
myCharacterSize(characterSize),
myStyle        (Regular),
myVertices     (Quads),
myBounds       ()
{
    UpdateGeometry();
}


////////////////////////////////////////////////////////////
void Text::SetString(const String& string)
{
    myString = string;
    UpdateGeometry();
}


////////////////////////////////////////////////////////////
void Text::SetFont(const Font& font)
{
    if (myFont != &font)
    {
        myFont = &font;
        UpdateGeometry();
    }
}


////////////////////////////////////////////////////////////
void Text::SetCharacterSize(unsigned int size)
{
    if (myCharacterSize != size)
    {
        myCharacterSize = size;
        UpdateGeometry();
    }
}


////////////////////////////////////////////////////////////
void Text::SetStyle(Uint32 style)
{
    if (myStyle != style)
    {
        myStyle = style;
        UpdateGeometry();
    }
}


////////////////////////////////////////////////////////////
void Text::SetColor(const Color& color)
{
    if (color != myColor)
    {
        myColor = color;
        for (unsigned int i = 0; i < myVertices.GetVertexCount(); ++i)
            myVertices[i].Color = myColor;
    }
}


////////////////////////////////////////////////////////////
const String& Text::GetString() const
{
    return myString;
}


////////////////////////////////////////////////////////////
const Font& Text::GetFont() const
{
    assert(myFont != NULL); // can never be NULL, always &Font::GetDefaultFont() by default
    return *myFont;
}


////////////////////////////////////////////////////////////
unsigned int Text::GetCharacterSize() const
{
    return myCharacterSize;
}


////////////////////////////////////////////////////////////
Uint32 Text::GetStyle() const
{
    return myStyle;
}


////////////////////////////////////////////////////////////
const Color& Text::GetColor() const
{
    return myColor;
}


////////////////////////////////////////////////////////////
Vector2f Text::FindCharacterPos(std::size_t index) const
{
    assert(myFont != NULL);

    // Adjust the index if it's out of range
    if (index > myString.GetSize())
        index = myString.GetSize();

    // Precompute the variables needed by the algorithm
    bool  bold   = (myStyle & Bold) != 0;
    float hspace = static_cast<float>(myFont->GetGlyph(L' ', myCharacterSize, bold).Advance);
    float vspace = static_cast<float>(myFont->GetLineSpacing(myCharacterSize));

    // Compute the position
    Vector2f position;
    Uint32 prevChar = 0;
    for (std::size_t i = 0; i < index; ++i)
    {
        Uint32 curChar = myString[i];

        // Apply the kerning offset
        position.x += static_cast<float>(myFont->GetKerning(prevChar, curChar, myCharacterSize));
        prevChar = curChar;

        // Handle special characters
        switch (curChar)
        {
            case L' ' :  position.x += hspace;                 continue;
            case L'\t' : position.x += hspace * 4;             continue;
            case L'\n' : position.y += vspace; position.x = 0; continue;
            case L'\v' : position.y += vspace * 4;             continue;
        }

        // For regular characters, add the advance offset of the glyph
        position.x += static_cast<float>(myFont->GetGlyph(curChar, myCharacterSize, bold).Advance);
    }

    // Transform the position to global coordinates
    position = GetTransform().TransformPoint(position);

    return position;
}


////////////////////////////////////////////////////////////
FloatRect Text::GetLocalBounds() const
{
    return myBounds;
}


////////////////////////////////////////////////////////////
FloatRect Text::GetGlobalBounds() const
{
    return GetTransform().TransformRect(GetLocalBounds());
}


////////////////////////////////////////////////////////////
void Text::Draw(RenderTarget& target, RenderStates states) const
{
    assert(myFont != NULL);

    states.Transform *= GetTransform();
    states.BlendMode = BlendAlpha; // alpha blending is mandatory for proper text rendering
    states.Texture = &myFont->GetTexture(myCharacterSize);
    target.Draw(myVertices, states);
}


////////////////////////////////////////////////////////////
void Text::UpdateGeometry()
{
    assert(myFont != NULL);

    // Clear the previous geometry
    myVertices.Clear();

    // No text: nothing to draw
    if (myString.IsEmpty())
        return;

    // Compute values related to the text style
    bool  bold               = (myStyle & Bold) != 0;
    bool  underlined         = (myStyle & Underlined) != 0;
    float italic             = (myStyle & Italic) ? 0.208f : 0.f; // 12 degrees
    float underlineOffset    = myCharacterSize * 0.1f;
    float underlineThickness = myCharacterSize * (bold ? 0.1f : 0.07f);

    // Precompute the variables needed by the algorithm
    float hspace = static_cast<float>(myFont->GetGlyph(L' ', myCharacterSize, bold).Advance);
    float vspace = static_cast<float>(myFont->GetLineSpacing(myCharacterSize));
    float x      = 0.f;
    float y      = static_cast<float>(myCharacterSize);

    // Create one quad for each character
    Uint32 prevChar = 0;
    for (std::size_t i = 0; i < myString.GetSize(); ++i)
    {
        Uint32 curChar = myString[i];

        // Apply the kerning offset
        x += static_cast<float>(myFont->GetKerning(prevChar, curChar, myCharacterSize));
        prevChar = curChar;

        // If we're using the underlined style and there's a new line, draw a line
        if (underlined && (curChar == L'\n'))
        {
            float top = y + underlineOffset;
            float bottom = top + underlineThickness;

            myVertices.Append(Vertex(Vector2f(0, top),    myColor, Vector2f(1, 1)));
            myVertices.Append(Vertex(Vector2f(x, top),    myColor, Vector2f(2, 1)));
            myVertices.Append(Vertex(Vector2f(x, bottom), myColor, Vector2f(2, 2)));
            myVertices.Append(Vertex(Vector2f(0, bottom), myColor, Vector2f(1, 2)));
        }

        // Handle special characters
        switch (curChar)
        {
            case L' ' :  x += hspace;        continue;
            case L'\t' : x += hspace * 4;    continue;
            case L'\n' : y += vspace; x = 0; continue;
            case L'\v' : y += vspace * 4;    continue;
        }

        // Extract the current glyph's description
        const Glyph& glyph = myFont->GetGlyph(curChar, myCharacterSize, bold);

        int left   = glyph.Bounds.Left;
        int top    = glyph.Bounds.Top;
        int right  = glyph.Bounds.Left + glyph.Bounds.Width;
        int bottom = glyph.Bounds.Top  + glyph.Bounds.Height;

        float u1 = static_cast<float>(glyph.TextureRect.Left);
        float v1 = static_cast<float>(glyph.TextureRect.Top);
        float u2 = static_cast<float>(glyph.TextureRect.Left + glyph.TextureRect.Width);
        float v2 = static_cast<float>(glyph.TextureRect.Top  + glyph.TextureRect.Height);

        // Add a quad for the current character
        myVertices.Append(Vertex(Vector2f(x + left  - italic * top,    y + top),    myColor, Vector2f(u1, v1)));
        myVertices.Append(Vertex(Vector2f(x + right - italic * top,    y + top),    myColor, Vector2f(u2, v1)));
        myVertices.Append(Vertex(Vector2f(x + right - italic * bottom, y + bottom), myColor, Vector2f(u2, v2)));
        myVertices.Append(Vertex(Vector2f(x + left  - italic * bottom, y + bottom), myColor, Vector2f(u1, v2)));

        // Advance to the next character
        x += glyph.Advance;
    }

    // If we're using the underlined style, add the last line
    if (underlined)
    {
        float top = y + underlineOffset;
        float bottom = top + underlineThickness;

        myVertices.Append(Vertex(Vector2f(0, top),    myColor, Vector2f(1, 1)));
        myVertices.Append(Vertex(Vector2f(x, top),    myColor, Vector2f(2, 1)));
        myVertices.Append(Vertex(Vector2f(x, bottom), myColor, Vector2f(2, 2)));
        myVertices.Append(Vertex(Vector2f(0, bottom), myColor, Vector2f(1, 2)));
    }

    // Recompute the bounding rectangle
    myBounds = myVertices.GetBounds();
}

} // namespace sf
