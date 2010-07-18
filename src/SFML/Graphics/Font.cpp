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
#include <SFML/Graphics/Font.hpp>
#include <SFML/System/Err.hpp>
#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_GLYPH_H
#include FT_OUTLINE_H
#include FT_BITMAP_H


namespace sf
{
////////////////////////////////////////////////////////////
Font::Font() :
myLibrary (NULL),
myFace    (NULL),
myRefCount(NULL)
{

}


////////////////////////////////////////////////////////////
Font::Font(const Font& copy) :
Resource<Font>(),
myLibrary    (copy.myLibrary),
myFace       (copy.myFace),
myRefCount   (copy.myRefCount),
myPages      (copy.myPages),
myPixelBuffer(copy.myPixelBuffer)
{
    // Note: as FreeType doesn't provide functions for copying/cloning,
    // we must share all the FreeType pointers

    if (myRefCount)
        (*myRefCount)++;
}


////////////////////////////////////////////////////////////
Font::~Font()
{
    Cleanup();
}


////////////////////////////////////////////////////////////
bool Font::LoadFromFile(const std::string& filename)
{
    // Cleanup the previous resources
    Cleanup();
    myRefCount = new int(1);

    // Initialize FreeType
    // Note: we initialize FreeType for every font instance in order to avoid having a single
    // global manager that would create a lot of issues regarding creation and destruction order.
    FT_Library library;
    if (FT_Init_FreeType(&library) != 0)
    {
        Err() << "Failed to load font \"" << filename << "\" (failed to initialize FreeType)" << std::endl;
        return false;
    }
    myLibrary = library;

    // Load the new font face from the specified file
    FT_Face face;
    if (FT_New_Face(static_cast<FT_Library>(myLibrary), filename.c_str(), 0, &face) != 0)
    {
        Err() << "Failed to load font \"" << filename << "\" (failed to create the font face)" << std::endl;
        return false;
    }

    // Select the unicode character map
    if (FT_Select_Charmap(face, FT_ENCODING_UNICODE) != 0)
    {
        Err() << "Failed to load font \"" << filename << "\" (failed to set the Unicode character set)" << std::endl;
        return false;
    }

    // Store the loaded font in our ugly void* :)
    myFace = face;

    return true;
}


////////////////////////////////////////////////////////////
bool Font::LoadFromMemory(const void* data, std::size_t sizeInBytes)
{
    // Cleanup the previous resources
    Cleanup();
    myRefCount = new int(1);

    // Initialize FreeType
    // Note: we initialize FreeType for every font instance in order to avoid having a single
    // global manager that would create a lot of issues regarding creation and destruction order.
    FT_Library library;
    if (FT_Init_FreeType(&library) != 0)
    {
        Err() << "Failed to load font from memory (failed to initialize FreeType)" << std::endl;
        return false;
    }
    myLibrary = library;

    // Load the new font face from the specified file
    FT_Face face;
    if (FT_New_Memory_Face(static_cast<FT_Library>(myLibrary), reinterpret_cast<const FT_Byte*>(data), static_cast<FT_Long>(sizeInBytes), 0, &face) != 0)
    {
        Err() << "Failed to load font from memory (failed to create the font face)" << std::endl;
        return false;
    }

    // Select the unicode character map
    if (FT_Select_Charmap(face, FT_ENCODING_UNICODE) != 0)
    {
        Err() << "Failed to load font from memory (failed to set the Unicode character set)" << std::endl;
        return false;
    }

    // Store the loaded font in our ugly void* :)
    myFace = face;

    return true;
}


////////////////////////////////////////////////////////////
const Glyph& Font::GetGlyph(Uint32 codePoint, unsigned int characterSize, bool bold) const
{
    // Get the page corresponding to the character size
    GlyphTable& glyphs = myPages[characterSize].Glyphs;

    // Build the key by combining the code point and the bold flag
    Uint32 key = ((bold ? 1 : 0) << 31) | codePoint;

    // Search the glyph into the cache
    GlyphTable::const_iterator it = glyphs.find(key);
    if (it != glyphs.end())
    {
        // Found: just return it
        return it->second;
    }
    else
    {
        // Not found: we have to load it
        Glyph glyph = LoadGlyph(codePoint, characterSize, bold);
        return glyphs.insert(std::make_pair(key, glyph)).first->second;
    }
}


////////////////////////////////////////////////////////////
int Font::GetKerning(Uint32 first, Uint32 second, unsigned int characterSize) const
{
    // Special case where first or second is 0 (null character)
    if (first == 0 || second == 0)
        return 0;

    FT_Face face = static_cast<FT_Face>(myFace);

    if (face && FT_HAS_KERNING(face) && SetCurrentSize(characterSize))
    {
        // Convert the characters to indices
        FT_UInt index1 = FT_Get_Char_Index(face, first);
        FT_UInt index2 = FT_Get_Char_Index(face, second);

        // Get the kerning vector
        FT_Vector kerning;
        FT_Get_Kerning(face, index1, index2, FT_KERNING_DEFAULT, &kerning);

        // Return the X advance
        return kerning.x >> 6;
    }
    else
    {
        // Invalid font, or no kerning
        return 0;
    }
}


////////////////////////////////////////////////////////////
int Font::GetLineSpacing(unsigned int characterSize) const
{
    FT_Face face = static_cast<FT_Face>(myFace);

    if (face && SetCurrentSize(characterSize))
    {
        return (face->size->metrics.height >> 6);
    }
    else
    {
        return 0;
    }
}


////////////////////////////////////////////////////////////
const Image& Font::GetImage(unsigned int characterSize) const
{
    return myPages[characterSize].Texture;
}


////////////////////////////////////////////////////////////
Font& Font::operator =(const Font& right)
{
    Font temp(right);

    std::swap(myLibrary,     temp.myLibrary);
    std::swap(myFace,        temp.myFace);
    std::swap(myPages,       temp.myPages);
    std::swap(myPixelBuffer, temp.myPixelBuffer);
    std::swap(myRefCount,    temp.myRefCount);

    return *this;
}


////////////////////////////////////////////////////////////
const Font& Font::GetDefaultFont()
{
    static Font font;
    static bool loaded = false;

    // Load the default font on first call
    if (!loaded)
    {
        static const char data[] =
        {
            #include <SFML/Graphics/Arial.hpp>
        };

        font.LoadFromMemory(data, sizeof(data));
        loaded = true;
    }

    return font;
}


////////////////////////////////////////////////////////////
void Font::Cleanup()
{
    // Check if we must destroy the FreeType pointers
    if (myRefCount)
    {
        // Decrease the reference counter
        (*myRefCount)--;

        // Free the resources only if we are the last owner
        if (*myRefCount == 0)
        {
            // Delete the reference counter
            delete myRefCount;

            // Destroy the font face
            if (myFace)
                FT_Done_Face(static_cast<FT_Face>(myFace));

            // Close the library
            if (myLibrary)
                FT_Done_FreeType(static_cast<FT_Library>(myLibrary));
        }
    }

    // Reset members
    myLibrary  = NULL;
    myFace     = NULL;
    myRefCount = NULL;
    myPages.clear();
    myPixelBuffer.clear();
}


////////////////////////////////////////////////////////////
Glyph Font::LoadGlyph(Uint32 codePoint, unsigned int characterSize, bool bold) const
{
    // The glyph to return
    Glyph glyph;

    // First, transform our ugly void* to a FT_Face
    FT_Face face = static_cast<FT_Face>(myFace);
    if (!face)
        return glyph;

    // Set the character size
    if (!SetCurrentSize(characterSize))
        return glyph;

    // Load the glyph corresponding to the code point
    if (FT_Load_Char(face, codePoint, FT_LOAD_TARGET_NORMAL) != 0)
        return glyph;

    // Retrieve the glyph
    FT_Glyph glyphDesc;
    if (FT_Get_Glyph(face->glyph, &glyphDesc) != 0)
        return glyph;

    // Apply bold if necessary -- first technique using outline (highest quality)
    FT_Pos weight = 1 << 6;
    bool outline = (glyphDesc->format == FT_GLYPH_FORMAT_OUTLINE);
    if (bold && outline)
    {
        FT_OutlineGlyph outlineGlyph = (FT_OutlineGlyph)glyphDesc;
        FT_Outline_Embolden(&outlineGlyph->outline, weight);
    }

    // Convert the glyph to a bitmap (i.e. rasterize it)
    FT_Glyph_To_Bitmap(&glyphDesc, FT_RENDER_MODE_NORMAL, 0, 1);
    FT_BitmapGlyph bitmapGlyph = (FT_BitmapGlyph)glyphDesc;
    FT_Bitmap& bitmap = bitmapGlyph->bitmap;

    // Apply bold if necessary -- fallback technique using bitmap (lower quality)
    if (bold && !outline)
    {
        FT_Bitmap_Embolden(static_cast<FT_Library>(myLibrary), &bitmap, weight, weight);
    }

    // Compute the glyph's advance offset
    glyph.Advance = glyphDesc->advance.x >> 16;
    if (bold)
        glyph.Advance += weight >> 6;

    int width  = bitmap.width;
    int height = bitmap.rows;
    if ((width > 0) && (height > 0))
    {
        // Leave a small padding around characters, so that filtering doesn't
        // pollute them with pixels from neighbours
        const unsigned int padding = 1;

        // Get the glyphs page corresponding to the character size
        Page& page = myPages[characterSize];

        // Find a good position for the new glyph into the texture
        glyph.SubRect = FindGlyphRect(page, width + 2 * padding, height + 2 * padding);

        // Compute the glyph's bounding box
        glyph.Bounds.Left   = bitmapGlyph->left - padding;
        glyph.Bounds.Top    = -bitmapGlyph->top - padding;
        glyph.Bounds.Width  = width + 2 * padding;
        glyph.Bounds.Height = height + 2 * padding;

        // Extract the glyph's pixels from the bitmap
        myPixelBuffer.resize(width * height * 4, 255);
        const Uint8* pixels = bitmap.buffer;
        if (bitmap.pixel_mode == FT_PIXEL_MODE_MONO)
        {
            // Pixels are 1 bit monochrome values
            for (int y = 0; y < height; ++y)
            {
                for (int x = 0; x < width; ++x)
                {
                    // The color channels remain white, just fill the alpha channel
                    std::size_t index = (x + y * width) * 4 + 3;
                    myPixelBuffer[index] = ((pixels[x / 8]) & (1 << (7 - (x % 8)))) ? 255 : 0;
                }
                pixels += bitmap.pitch;
            }
        }
        else
        {
            // Pixels are 8 bits gray levels
            for (int y = 0; y < height; ++y)
            {
                for (int x = 0; x < width; ++x)
                {
                    // The color channels remain white, just fill the alpha channel
                    std::size_t index = (x + y * width) * 4 + 3;
                    myPixelBuffer[index] = pixels[x];
                }
                pixels += bitmap.pitch;
            }
        }

        // Write the pixels to the texture
        IntRect subrect = glyph.SubRect;
        subrect.Left   += padding;
        subrect.Top    += padding;
        subrect.Width  -= 2 * padding;
        subrect.Height -= 2 * padding;
        page.Texture.UpdatePixels(&myPixelBuffer[0], subrect);
    }

    // Delete the FT glyph
    FT_Done_Glyph(glyphDesc);

    // Done :)
    return glyph;
}


////////////////////////////////////////////////////////////
IntRect Font::FindGlyphRect(Page& page, unsigned int width, unsigned int height) const
{
    // Find the line that fits well the glyph
    Row* row = NULL;
    float bestRatio = 0;
    for (std::vector<Row>::iterator it = page.Rows.begin(); it != page.Rows.end() && !row; ++it)
    {
        float ratio = static_cast<float>(height) / it->Height;

        // Ignore rows that are either too small or too high
        if ((ratio < 0.7f) || (ratio > 1.f))
            continue;

        // Check if there's enough horizontal space left in the row
        if (width > page.Texture.GetWidth() - it->Width)
            continue;

        // Make sure that this new row is the best found so far
        if (ratio < bestRatio)
            continue;

        // The current row passed all the tests: we can select it
        row = &*it;
        bestRatio = ratio;
    }

    // If we didn't find a matching row, create a new one (10% taller than the glyph)
    if (!row)
    {
        int rowHeight = height + height / 10;
        if (page.NextRow + rowHeight >= page.Texture.GetHeight())
        {
            // Not enough space: resize the texture if possible
            unsigned int textureWidth  = page.Texture.GetWidth();
            unsigned int textureHeight = page.Texture.GetHeight();
            if ((textureWidth * 2 <= Image::GetMaximumSize()) && (textureHeight * 2 <= Image::GetMaximumSize()))
            {
                // Make the texture 2 times bigger
                std::size_t size = textureWidth * textureHeight * 4;
                std::vector<Uint8> pixels(size);
                memcpy(&pixels[0], page.Texture.GetPixelsPtr(), size);
                page.Texture.Create(textureWidth * 2, textureHeight * 2, Color(255, 255, 255, 0));
                page.Texture.UpdatePixels(&pixels[0], IntRect(0, 0, textureWidth, textureHeight));
            }
            else
            {
                // Oops, we've reached the maximum texture size...
                Err() << "Failed to add a new character to the font: the maximum image size has been reached" << std::endl;
                return IntRect(0, 0, 2, 2);
            }
        }

        // We can now create the new row
        page.Rows.push_back(Row(page.NextRow, rowHeight));
        page.NextRow += rowHeight;
        row = &page.Rows.back();
    }

    // Find the glyph's rectangle on the selected row
    IntRect rect(row->Width, row->Top, width, height);

    // Update the row informations
    row->Width += width;

    return rect;
}


////////////////////////////////////////////////////////////
bool Font::SetCurrentSize(unsigned int characterSize) const
{
    // FT_Set_Pixel_Sizes is an expensive function, so we must call it
    // only when necessary to avoid killing performances

    FT_Face face = static_cast<FT_Face>(myFace);
    FT_UShort currentSize = face->size->metrics.x_ppem;

    if (currentSize != characterSize)
    {
        return FT_Set_Pixel_Sizes(face, 0, characterSize) == 0;
    }
    else
    {
        return true;
    }
}


////////////////////////////////////////////////////////////
Font::Page::Page() :
NextRow(2)
{
    // Make sure that the texture is initialized by default
    Texture.Create(128, 128, Color(255, 255, 255, 0));

    // Reserve a 2x2 white square for texturing underlines
    for (int x = 0; x < 2; ++x)
        for (int y = 0; y < 2; ++y)
            Texture.SetPixel(x, y, Color(255, 255, 255, 255));
}

} // namespace sf
