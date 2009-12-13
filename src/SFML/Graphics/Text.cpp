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
#include <SFML/Graphics/RenderQueue.hpp>


namespace sf
{
////////////////////////////////////////////////////////////
/// Default constructor
////////////////////////////////////////////////////////////
Text::Text() :
myFont          (&Font::GetDefaultFont()),
myCharacterSize (30),
myStyle         (Regular),
myNeedRectUpdate(true)
{

}


////////////////////////////////////////////////////////////
/// Construct the string from any kind of text
////////////////////////////////////////////////////////////
Text::Text(const String& string, const Font& font, unsigned int characterSize) :
myFont          (&font),
myCharacterSize (characterSize),
myStyle         (Regular),
myNeedRectUpdate(true)
{
    SetString(string);
}


////////////////////////////////////////////////////////////
/// Set the text (from any kind of string)
////////////////////////////////////////////////////////////
void Text::SetString(const String& string)
{
    myNeedRectUpdate = true;
    myString = string;
}


////////////////////////////////////////////////////////////
/// Set the font of the string
////////////////////////////////////////////////////////////
void Text::SetFont(const Font& font)
{
    if (myFont != &font)
    {
        myNeedRectUpdate = true;
        myFont = &font;
    }
}


////////////////////////////////////////////////////////////
/// Set the base size for the characters.
////////////////////////////////////////////////////////////
void Text::SetCharacterSize(unsigned int size)
{
    if (myCharacterSize != size)
    {
        myNeedRectUpdate = true;
        myCharacterSize = size;
    }
}


////////////////////////////////////////////////////////////
/// Set the style of the text
/// The default style is Regular
////////////////////////////////////////////////////////////
void Text::SetStyle(unsigned long style)
{
    if (myStyle != style)
    {
        myNeedRectUpdate = true;
        myStyle = style;
    }
}


////////////////////////////////////////////////////////////
/// Get the text (the returned text can be converted implicitely to any kind of string)
////////////////////////////////////////////////////////////
const String& Text::GetString() const
{
    return myString;
}


////////////////////////////////////////////////////////////
/// Get the font used by the string
////////////////////////////////////////////////////////////
const Font& Text::GetFont() const
{
    return *myFont;
}


////////////////////////////////////////////////////////////
/// Get the base size of characters
////////////////////////////////////////////////////////////
unsigned int Text::GetCharacterSize() const
{
    return myCharacterSize;
}


////////////////////////////////////////////////////////////
/// Get the style of the text
////////////////////////////////////////////////////////////
unsigned long Text::GetStyle() const
{
    return myStyle;
}


////////////////////////////////////////////////////////////
/// Return the visual position of the Index-th character of the string,
/// in coordinates relative to the string
/// (note : translation, center, rotation and scale are not applied)
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
    float space = static_cast<float>(myFont->GetGlyph(L' ', myCharacterSize).Advance);

    // Compute the position
    sf::Vector2f position;
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
        position.x += static_cast<float>(myFont->GetGlyph(curChar, myCharacterSize).Advance);
    }

    return position;
}


////////////////////////////////////////////////////////////
/// Get the string rectangle on screen
////////////////////////////////////////////////////////////
FloatRect Text::GetRect() const
{
    if (myNeedRectUpdate)
        const_cast<Text*>(this)->RecomputeRect();

    FloatRect rect;
    rect.Left   = (myBaseRect.Left   - GetOrigin().x) * GetScale().x + GetPosition().x;
    rect.Top    = (myBaseRect.Top    - GetOrigin().y) * GetScale().y + GetPosition().y;
    rect.Right  = (myBaseRect.Right  - GetOrigin().x) * GetScale().x + GetPosition().x;
    rect.Bottom = (myBaseRect.Bottom - GetOrigin().y) * GetScale().y + GetPosition().y;

    return rect;
}


////////////////////////////////////////////////////////////
/// /see Drawable::Render
////////////////////////////////////////////////////////////
void Text::Render(RenderTarget&, RenderQueue& queue) const
{
    // No text or not font: nothing to render
    if (!myFont || myString.IsEmpty())
        return;

    // Bind the font texture
    queue.SetTexture(&myFont->GetImage(myCharacterSize));

    // Initialize the rendering coordinates
    float space       = static_cast<float>(myFont->GetGlyph(L' ', myCharacterSize).Advance);
    float lineSpacing = static_cast<float>(myFont->GetLineSpacing(myCharacterSize));
    float x = 0.f;
    float y = static_cast<float>(myCharacterSize);

    // Holds the lines to draw later, for underlined style
    std::vector<float> underlineCoords;
    underlineCoords.reserve(16);

    // Compute the shearing to apply if we're using the italic style
    float italicCoeff = (myStyle & Italic) ? 0.208f : 0.f; // 12 degrees

    // Draw one quad for each character
    unsigned int index = 0;
    Uint32 prevChar = 0;
    queue.BeginBatch();
    for (std::size_t i = 0; i < myString.GetSize(); ++i)
    {
        Uint32 curChar = myString[i];

        // Apply the kerning offset
        x += static_cast<float>(myFont->GetKerning(prevChar, curChar, myCharacterSize));
        prevChar = curChar;

        // If we're using the underlined style and there's a new line,
        // we keep track of the previous line to draw it later
        if ((curChar == L'\n') && (myStyle & Underlined))
        {
            underlineCoords.push_back(x);
            underlineCoords.push_back(y + 2);
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
        const Glyph&     curGlyph = myFont->GetGlyph(curChar, myCharacterSize);
        int              advance  = curGlyph.Advance;
        const IntRect&   rect     = curGlyph.Rectangle;
        const FloatRect& coord    = curGlyph.TexCoords;

        // Draw a textured quad for the current character
        queue.AddVertex(x + rect.Left  - italicCoeff * rect.Top,    y + rect.Top,    coord.Left,  coord.Top);
        queue.AddVertex(x + rect.Left  - italicCoeff * rect.Bottom, y + rect.Bottom, coord.Left,  coord.Bottom);
        queue.AddVertex(x + rect.Right - italicCoeff * rect.Bottom, y + rect.Bottom, coord.Right, coord.Bottom);
        queue.AddVertex(x + rect.Right - italicCoeff * rect.Top,    y + rect.Top,    coord.Right, coord.Top);

        queue.AddTriangle(index + 0, index + 1, index + 3);
        queue.AddTriangle(index + 3, index + 1, index + 2);
        index += 4;

        // Advance to the next character
        x += advance;
    }

    // If we're using the bold style, we must render the string 4 more times,
    // slightly offseted, to simulate a higher weight
    if (myStyle & Bold)
    {
        float offset = myCharacterSize * 0.02f;
        static const float offsetsX[] = {-offset, offset, 0.f, 0.f};
        static const float offsetsY[] = {0.f, 0.f, -offset, offset};

        for (int j = 0; j < 4; ++j)
        {
            index = 0;
            x = 0.f;
            y = static_cast<float>(myCharacterSize);

            Uint32 prevChar = 0;
            queue.BeginBatch();
            for (std::size_t i = 0; i < myString.GetSize(); ++i)
            {
                Uint32 curChar = myString[i];

                // Apply the kerning offset
                x += static_cast<float>(myFont->GetKerning(prevChar, curChar, myCharacterSize));
                prevChar = curChar;

                // Handle special characters
                switch (curChar)
                {
                    case L' ' :  x += space;              continue;
                    case L'\t' : x += space * 4;          continue;
                    case L'\n' : y += lineSpacing; x = 0; continue;
                    case L'\v' : y += lineSpacing * 4;    continue;
                }

                // Extract the current glyph's description
                const Glyph&     curGlyph = myFont->GetGlyph(curChar, myCharacterSize);
                int              advance  = curGlyph.Advance;
                const IntRect&   rect     = curGlyph.Rectangle;
                const FloatRect& coord    = curGlyph.TexCoords;

                // Draw a textured quad for the current character
                queue.AddVertex(x + offsetsX[j] + rect.Left  - italicCoeff * rect.Top,    y + offsetsY[j] + rect.Top,    coord.Left,  coord.Top);
                queue.AddVertex(x + offsetsX[j] + rect.Left  - italicCoeff * rect.Bottom, y + offsetsY[j] + rect.Bottom, coord.Left,  coord.Bottom);
                queue.AddVertex(x + offsetsX[j] + rect.Right - italicCoeff * rect.Bottom, y + offsetsY[j] + rect.Bottom, coord.Right, coord.Bottom);
                queue.AddVertex(x + offsetsX[j] + rect.Right - italicCoeff * rect.Top,    y + offsetsY[j] + rect.Top,    coord.Right, coord.Top);

                queue.AddTriangle(index + 0, index + 1, index + 3);
                queue.AddTriangle(index + 3, index + 1, index + 2);
                index += 4;

                // Advance to the next character
                x += advance;
            }
        }
    }

    // Draw the underlines if needed
    if (myStyle & Underlined)
    {
        // Compute the line thickness
        float thickness = (myStyle & Bold) ? 3.f : 2.f;

        // Add the last line (which was not finished with a \n)
        underlineCoords.push_back(x);
        underlineCoords.push_back(y + 2);

        // Draw the underlines as quads
        index = 0;
        queue.SetTexture(NULL);
        queue.BeginBatch();
        for (std::size_t i = 0; i < underlineCoords.size(); i += 2)
        {
            queue.AddVertex(0,                  underlineCoords[i + 1]);
            queue.AddVertex(0,                  underlineCoords[i + 1] + thickness);
            queue.AddVertex(underlineCoords[i], underlineCoords[i + 1] + thickness);
            queue.AddVertex(underlineCoords[i], underlineCoords[i + 1]);

            queue.AddTriangle(index + 0, index + 1, index + 3);
            queue.AddTriangle(index + 3, index + 1, index + 2);
            index += 4;
        }
    }
}


////////////////////////////////////////////////////////////
/// Recompute the bounding rectangle of the text
////////////////////////////////////////////////////////////
void Text::RecomputeRect()
{
    // Reset the previous states
    myNeedRectUpdate = false;
    myBaseRect = FloatRect(0, 0, 0, 0);

    // No text or not font: empty box
    if (!myFont || myString.IsEmpty())
        return;

    // Initial values
    float  charSize    = static_cast<float>(myCharacterSize);
    float  space       = static_cast<float>(myFont->GetGlyph(L' ', myCharacterSize).Advance);
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
        const Glyph& curGlyph = myFont->GetGlyph(curChar, myCharacterSize);

        // Advance to the next character
        curWidth += static_cast<float>(curGlyph.Advance);

        // Update the maximum height
        float charHeight = charSize + curGlyph.Rectangle.Bottom;
        if (charHeight > curHeight)
            curHeight = charHeight;
    }

    // Update the last line
    if (curWidth > width)
        width = curWidth;
    height += curHeight;

    // Add a slight width / height if we're using the bold style
    if (myStyle & Bold)
    {
        width  += 1.f;
        height += 1.f;
    }

    // Add a slight width if we're using the italic style
    if (myStyle & Italic)
    {
        width += 0.208f * charSize;
    }

    // Add a slight height if we're using the underlined style
    if (myStyle & Underlined)
    {
        if (curHeight < charSize + 4.f)
            height += 4.f;
    }

    // Finally update the rectangle
    myBaseRect.Left   = 0;
    myBaseRect.Top    = 0;
    myBaseRect.Right  = width;
    myBaseRect.Bottom = height;
}

} // namespace sf
