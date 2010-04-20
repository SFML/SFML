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
#include <SFML/Graphics/Image.hpp>
#include <SFML/Graphics/Renderer.hpp>


namespace sf
{
////////////////////////////////////////////////////////////
Text::Text() :
myFont         (&Font::GetDefaultFont()),
myCharacterSize(30),
myStyle        (Regular),
myRectUpdated  (true)
{

}


////////////////////////////////////////////////////////////
Text::Text(const String& string, const Font& font, unsigned int characterSize) :
myFont         (&font),
myCharacterSize(characterSize),
myStyle        (Regular),
myRectUpdated  (true)
{
    SetString(string);
}


////////////////////////////////////////////////////////////
void Text::SetString(const String& string)
{
    myString = string;
    myRectUpdated = false;
}


////////////////////////////////////////////////////////////
void Text::SetFont(const Font& font)
{
    if (myFont != &font)
    {
        myFont = &font;
        myRectUpdated = false;
    }
}


////////////////////////////////////////////////////////////
void Text::SetCharacterSize(unsigned int size)
{
    if (myCharacterSize != size)
    {
        myCharacterSize = size;
        myRectUpdated = false;
    }
}


////////////////////////////////////////////////////////////
void Text::SetStyle(unsigned long style)
{
    if (myStyle != style)
    {
        myStyle = style;
        myRectUpdated = false;
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
    return *myFont;
}


////////////////////////////////////////////////////////////
unsigned int Text::GetCharacterSize() const
{
    return myCharacterSize;
}


////////////////////////////////////////////////////////////
unsigned long Text::GetStyle() const
{
    return myStyle;
}


////////////////////////////////////////////////////////////
Vector2f Text::GetCharacterPos(std::size_t index) const
{
    // Make sure that we have a valid font
    if (!myFont)
        return Vector2f(0, 0);

    // Adjust the index if it's out of range
    if (index > myString.GetSize())
        index = myString.GetSize();

    // We'll need this a lot
    bool  bold  = (myStyle & Bold) != 0;
    float space = static_cast<float>(myFont->GetGlyph(L' ', myCharacterSize, bold).Advance);

    // Compute the position
    Vector2f position;
    Uint32 prevChar = 0;
    float lineSpacing = static_cast<float>(myFont->GetLineSpacing(myCharacterSize));
    for (std::size_t i = 0; i < index; ++i)
    {
        Uint32 curChar = myString[i];

        // Apply the kerning offset
        position.x += static_cast<float>(myFont->GetKerning(prevChar, curChar, myCharacterSize));
        prevChar = curChar;

        // Handle special characters
        switch (curChar)
        {
            case L' ' :  position.x += space;                       continue;
            case L'\t' : position.x += space * 4;                   continue;
            case L'\v' : position.y += lineSpacing * 4;             continue;
            case L'\n' : position.y += lineSpacing; position.x = 0; continue;
        }

        // For regular characters, add the advance offset of the glyph
        position.x += static_cast<float>(myFont->GetGlyph(curChar, myCharacterSize, bold).Advance);
    }

    return position;
}


////////////////////////////////////////////////////////////
FloatRect Text::GetRect() const
{
    UpdateRect();

    FloatRect rect;
    rect.Left   = (myBaseRect.Left   - GetOrigin().x) * GetScale().x + GetPosition().x;
    rect.Top    = (myBaseRect.Top    - GetOrigin().y) * GetScale().y + GetPosition().y;
    rect.Width  = myBaseRect.Width  * GetScale().x;
    rect.Height = myBaseRect.Height * GetScale().y;

    return rect;
}


////////////////////////////////////////////////////////////
void Text::Render(RenderTarget&, Renderer& renderer) const
{
    // No text or not font: nothing to render
    if (!myFont || myString.IsEmpty())
        return;

    // Bind the font texture
    const Image& texture = myFont->GetImage(myCharacterSize);
    renderer.SetTexture(&texture);

    // Computes values related to the text style
    bool  bold                = (myStyle & Bold) != 0;
    bool  underlined          = (myStyle & Underlined) != 0;
    float italicCoeff         = (myStyle & Italic) ? 0.208f : 0.f; // 12 degrees
    float underlineOffset     = myCharacterSize * 0.1f;
    float underlineThickness  = myCharacterSize * (bold ? 0.1f : 0.07f);
    FloatRect underlineCoords = texture.GetTexCoords(IntRect(1, 1, 1, 1));
    float underlineLeft       = underlineCoords.Left;
    float underlineTop        = underlineCoords.Top;
    float underlineRight      = underlineCoords.Left + underlineCoords.Width;
    float underlineBottom     = underlineCoords.Top + underlineCoords.Height;

    // Initialize the rendering coordinates
    float space       = static_cast<float>(myFont->GetGlyph(L' ', myCharacterSize, bold).Advance);
    float lineSpacing = static_cast<float>(myFont->GetLineSpacing(myCharacterSize));
    float x = 0.f;
    float y = static_cast<float>(myCharacterSize);

    // Note:
    // Here we use a Begin/End pair for each quad because
    // the font's texture may change in a call to GetGlyph

    // Draw one quad for each character
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

            renderer.Begin(Renderer::QuadList);
                renderer.AddVertex(0, top,    underlineLeft,  underlineTop);
                renderer.AddVertex(x, top,    underlineRight, underlineTop);
                renderer.AddVertex(x, bottom, underlineRight, underlineBottom);
                renderer.AddVertex(0, bottom, underlineLeft,  underlineBottom);
            renderer.End();
        }

        // Handle special characters
        switch (curChar)
        {
            case L' ' :  x += space;              continue;
            case L'\t' : x += space * 4;          continue;
            case L'\n' : y += lineSpacing; x = 0; continue;
            case L'\v' : y += lineSpacing * 4;    continue;
        }

        // Extract the current glyph's description
        const Glyph&     glyph   = myFont->GetGlyph(curChar, myCharacterSize, bold);
        int              advance = glyph.Advance;
        const IntRect&   bounds  = glyph.Bounds;
        const FloatRect& coords  = texture.GetTexCoords(glyph.SubRect);

        int   boundsRight  = bounds.Left + bounds.Width;
        int   boundsBottom = bounds.Top  + bounds.Height;
        float coordsRight  = coords.Left + coords.Width;
        float coordsBottom = coords.Top  + coords.Height;

        // Draw a textured quad for the current character
        renderer.Begin(Renderer::QuadList);
            renderer.AddVertex(x + bounds.Left - italicCoeff * bounds.Top,   y + bounds.Top,   coords.Left, coords.Top);
            renderer.AddVertex(x + boundsRight - italicCoeff * bounds.Top,   y + bounds.Top,   coordsRight, coords.Top);
            renderer.AddVertex(x + boundsRight - italicCoeff * boundsBottom, y + boundsBottom, coordsRight, coordsBottom);
            renderer.AddVertex(x + bounds.Left - italicCoeff * boundsBottom, y + boundsBottom, coords.Left, coordsBottom);
        renderer.End();

        // Advance to the next character
        x += advance;
    }

    // If we're using the underlined style, add the last line
    if (underlined)
    {
        float top = y + underlineOffset;
        float bottom = top + underlineThickness;

        renderer.Begin(Renderer::QuadList);
            renderer.AddVertex(0, top,    underlineLeft,  underlineTop);
            renderer.AddVertex(x, top,    underlineRight, underlineTop);
            renderer.AddVertex(x, bottom, underlineRight, underlineBottom);
            renderer.AddVertex(0, bottom, underlineLeft,  underlineBottom);
        renderer.End();
    }
}


////////////////////////////////////////////////////////////
void Text::UpdateRect() const
{
    if (myRectUpdated)
        return;

    // Reset the previous states
    myRectUpdated = true;
    myBaseRect = FloatRect(0, 0, 0, 0);

    // No text or not font: empty box
    if (!myFont || myString.IsEmpty())
        return;

    // Initial values
    bool   bold        = (myStyle & Bold) != 0;
    float  charSize    = static_cast<float>(myCharacterSize);
    float  space       = static_cast<float>(myFont->GetGlyph(L' ', myCharacterSize, bold).Advance);
    float  lineSpacing = static_cast<float>(myFont->GetLineSpacing(myCharacterSize));
    float  curWidth    = 0;
    float  curHeight   = 0;
    float  width       = 0;
    float  height      = 0;
    Uint32 prevChar    = 0;

    // Go through each character
    for (std::size_t i = 0; i < myString.GetSize(); ++i)
    {
        Uint32 curChar = myString[i];

        // Apply the kerning offset
        curWidth += static_cast<float>(myFont->GetKerning(prevChar, curChar, myCharacterSize));
        prevChar = curChar;

        // Handle special characters
        switch (curChar)
        {
            case L' ' :
                curWidth += space;
                continue;

            case L'\t' : 
                curWidth += space * 4;
                continue;

            case L'\v' :
                height += lineSpacing * 4;
                curHeight = 0;
                continue;

            case L'\n' :
                height += lineSpacing;
                curHeight = 0;
                if (curWidth > width)
                    width = curWidth;
                curWidth = 0;
                continue;
        }

        // Extract the current glyph's description
        const Glyph& curGlyph = myFont->GetGlyph(curChar, myCharacterSize, bold);

        // Advance to the next character
        curWidth += static_cast<float>(curGlyph.Advance);

        // Update the maximum height
        float charHeight = charSize + curGlyph.Bounds.Top + curGlyph.Bounds.Height;
        if (charHeight > curHeight)
            curHeight = charHeight;
    }

    // Update the last line
    if (curWidth > width)
        width = curWidth;
    height += curHeight;

    // Add a slight width if we're using the italic style
    if (myStyle & Italic)
    {
        width += 0.208f * charSize;
    }

    // Add a slight height if we're using the underlined style
    if (myStyle & Underlined)
    {
        float underlineOffset    = myCharacterSize * 0.1f;
        float underlineThickness = myCharacterSize * (bold ? 0.1f : 0.07f);

        if (curHeight < charSize + underlineOffset + underlineThickness)
            height += underlineOffset + underlineThickness;
    }

    // Finally update the rectangle
    myBaseRect.Left   = 0;
    myBaseRect.Top    = 0;
    myBaseRect.Width  = width;
    myBaseRect.Height = height;
}

} // namespace sf
