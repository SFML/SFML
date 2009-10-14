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

//#define SFML_USE_STBTT
#ifndef SFML_USE_STBTT

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Graphics/FontLoader.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Image.hpp>
#include <SFML/Graphics/GLCheck.hpp>
#include FT_GLYPH_H
#include <iostream>
#include <map>
#include <vector>
#include <math.h>


namespace
{
    ////////////////////////////////////////////////////////////
    // Functor to sort glyphs by size
    ////////////////////////////////////////////////////////////
    struct SizeCompare
    {
        bool operator ()(FT_BitmapGlyph left, FT_BitmapGlyph right) const
        {
            return left->bitmap.rows < right->bitmap.rows;
        }
    };
}

namespace sf
{
namespace priv
{
////////////////////////////////////////////////////////////
/// Get the unique instance of the class
////////////////////////////////////////////////////////////
FontLoader& FontLoader::GetInstance()
{
    static FontLoader instance;

    return instance;
}


////////////////////////////////////////////////////////////
/// Default constructor
////////////////////////////////////////////////////////////
FontLoader::FontLoader()
{
    // Initialize FreeType library
    FT_Error error = FT_Init_FreeType(&myLibrary);
    if (error)
    {
        std::cerr << "Failed to initialize FreeType library (error code : " << error << ")" << std::endl;
        return;
    }
}


////////////////////////////////////////////////////////////
/// Destructor
////////////////////////////////////////////////////////////
FontLoader::~FontLoader()
{
    // Shutdown FreeType library
    if (myLibrary)
        FT_Done_FreeType(myLibrary);
}


////////////////////////////////////////////////////////////
/// Load a font from a file
////////////////////////////////////////////////////////////
bool FontLoader::LoadFontFromFile(const std::string& filename, unsigned int charSize, const Unicode::UTF32String& charset, Font& font)
{
    // Check if Freetype is correctly initialized
    if (!myLibrary)
    {
        std::cerr << "Failed to load font \"" << filename << "\", FreeType has not been initialized" << std::endl;
        return false;
    }

    // Create a new font face from the specified file
    FT_Face face;
    FT_Error error = FT_New_Face(myLibrary, filename.c_str(), 0, &face);
    if (error)
    {
        std::cerr << "Failed to load font \"" << filename << "\" (" << GetErrorDesc(error) << ")" << std::endl;
        return false;
    }

    // Create the bitmap font
    error = CreateBitmapFont(face, charSize, charset, font);
    if (error)
        std::cerr << "Failed to load font \"" << filename << "\" (" << GetErrorDesc(error) << ")" << std::endl;

    // Delete the font
    FT_Done_Face(face);

    return error == 0;
}


////////////////////////////////////////////////////////////
/// Load the font from a file in memory
////////////////////////////////////////////////////////////
bool FontLoader::LoadFontFromMemory(const char* data, std::size_t sizeInBytes, unsigned int charSize, const Unicode::UTF32String& charset, Font& font)
{
    // Check if Freetype is correctly initialized
    if (!myLibrary)
    {
        std::cerr << "Failed to load font from memory, FreeType has not been initialized" << std::endl;
        return false;
    }

    // Create a new font face from the specified memory data
    FT_Face face;
    FT_Error error = FT_New_Memory_Face(myLibrary, reinterpret_cast<const FT_Byte*>(data), static_cast<FT_Long>(sizeInBytes), 0, &face);
    if (error)
    {
        std::cerr << "Failed to load font from memory (" << GetErrorDesc(error) << ")" << std::endl;
        return false;
    }

    // Create the bitmap font
    error = CreateBitmapFont(face, charSize, charset, font);
    if (error)
        std::cerr << "Failed to load font from memory (" << GetErrorDesc(error) << ")" << std::endl;

    // Delete the font
    FT_Done_Face(face);

    return error == 0;
}


////////////////////////////////////////////////////////////
/// Create a bitmap font from a font face and a characters set
////////////////////////////////////////////////////////////
FT_Error FontLoader::CreateBitmapFont(FT_Face face, unsigned int charSize, const Unicode::UTF32String& charset, Font& font)
{
    // Let's find how many characters to put in each row to make them fit into a squared texture
    GLint maxSize;
    GLCheck(glGetIntegerv(GL_MAX_TEXTURE_SIZE, &maxSize));
    int nbChars = static_cast<int>(sqrt(static_cast<double>(charset.length())) * 0.75);

    // Clamp the character size to make sure we won't create a texture too big
    if (nbChars * charSize >= static_cast<unsigned int>(maxSize))
        charSize = maxSize / nbChars;

    // Initialize the dimensions
    unsigned int left      = 0;
    unsigned int top       = 0;
    unsigned int texWidth  = Image::GetValidTextureSize(charSize * nbChars);
    unsigned int texHeight = charSize * nbChars;
    std::vector<unsigned int> tops(texWidth, 0);

    // Create a pixel buffer for rendering every glyph
    std::vector<Uint8> glyphsBuffer(texWidth * texHeight * 4);

    // Setup the font size
    FT_Error error = FT_Set_Pixel_Sizes(face, charSize, charSize);
    if (error)
        return error;

    // Select the unicode character map
    error = FT_Select_Charmap(face, FT_ENCODING_UNICODE);
    if (error)
        return error;

    // Render all glyphs and sort them by size to optimize texture space
    typedef std::multimap<FT_BitmapGlyph, Uint32, SizeCompare> GlyphTable;
    GlyphTable glyphs;
    for (std::size_t i = 0; i < charset.length(); ++i)
    {
        // Load the glyph corresponding to the current character
        error = FT_Load_Char(face, charset[i], FT_LOAD_TARGET_NORMAL);
        if (error)
            return error;

        // Convert the glyph to a bitmap (ie. rasterize it)
        FT_Glyph glyph;
        error = FT_Get_Glyph(face->glyph, &glyph);
        if (error)
            return error;
        FT_Glyph_To_Bitmap(&glyph, FT_RENDER_MODE_NORMAL, 0, 1);
        FT_BitmapGlyph bitmapGlyph = (FT_BitmapGlyph)glyph;

        // Add it to the sorted table of glyphs
        glyphs.insert(std::make_pair(bitmapGlyph, charset[i]));
    }

    // Leave a small margin around characters, so that filtering doesn't
    // pollute them with pixels from neighbours
    unsigned int padding = 1;
    unsigned int margin = 1;

    // Copy the rendered glyphs into the texture
    unsigned int maxHeight = 0;
    std::map<Uint32, IntRect> coords;
    for (GlyphTable::const_iterator i = glyphs.begin(); i != glyphs.end(); ++i)
    {
        // Get the bitmap of the current glyph
        Glyph&         curGlyph    = font.myGlyphs[i->second];
        FT_BitmapGlyph bitmapGlyph = i->first;
        FT_Bitmap&     bitmap      = bitmapGlyph->bitmap;

        // Make sure we don't go over the texture width
        if (left + bitmap.width + 2 * padding + margin >= texWidth)
            left = 0;

        // Compute the top coordinate
        top = tops[left];
        for (unsigned int x = 0; x < bitmap.width + 2 * padding + margin; ++x)
            top = std::max(top, tops[left + x]);

        // Make sure we don't go over the texture height -- resize it if we need more space
        if (top + bitmap.rows + 2 * padding + margin >= texHeight)
        {
            texHeight *= 2;
            glyphsBuffer.resize(texWidth * texHeight * 4);
        }

        // Store the character's position and size
        curGlyph.Rectangle.Left   = bitmapGlyph->left - padding;
        curGlyph.Rectangle.Top    = -bitmapGlyph->top - padding;
        curGlyph.Rectangle.Right  = bitmapGlyph->left + bitmap.width + padding;
        curGlyph.Rectangle.Bottom = -bitmapGlyph->top + bitmap.rows + padding;
        curGlyph.Advance          = bitmapGlyph->root.advance.x >> 16;

        // Texture size may change, so let the texture coordinates be calculated later
        coords[i->second] = IntRect(left, top, left + bitmap.width + 2 * padding, top + bitmap.rows + 2 * padding);

        // Draw the glyph into our bitmap font
        const Uint8* pixels = bitmap.buffer;
        for (int y = 0; y < bitmap.rows; ++y)
        {
            for (int x = 0; x < bitmap.width; ++x)
            {
                std::size_t index = x + left + padding + (y + top + padding) * texWidth;
                glyphsBuffer[index * 4 + 0] = 255;
                glyphsBuffer[index * 4 + 1] = 255;
                glyphsBuffer[index * 4 + 2] = 255;
                glyphsBuffer[index * 4 + 3] = pixels[x];

                // Formula for FT_RENDER_MODE_MONO
                // glyphsBuffer[index * 4 + 3] = ((pixels[x / 8]) & (1 << (7 - (x % 8)))) ? 255 : 0;
            }
            pixels += bitmap.pitch;
        }

        // Update the rendering coordinates
        for (unsigned int x = 0; x < bitmap.width + 2 * padding + margin; ++x)
            tops[left + x] = top + bitmap.rows + 2 * padding + margin;
        left += bitmap.width + 2 * padding + margin;
        if (top + bitmap.rows + 2 * padding > maxHeight)
            maxHeight = top + bitmap.rows + 2 * padding;

        // Delete the glyph
        FT_Done_Glyph((FT_Glyph)bitmapGlyph);
    }

    // Create the font's texture
    texHeight = maxHeight;
    glyphsBuffer.resize(texWidth * texHeight * 4);
    font.myTexture.LoadFromPixels(texWidth, texHeight, &glyphsBuffer[0]);

    // Now that the texture is created, we can precompute texture coordinates
    for (std::size_t i = 0; i < charset.size(); ++i)
    {
        Uint32 curChar = charset[i];
        font.myGlyphs[curChar].TexCoords = font.myTexture.GetTexCoords(coords[curChar]);
    }

    // Update the character size (it may have been changed by the function)
    font.myCharSize = charSize;

    return 0;
}


////////////////////////////////////////////////////////////
/// Get a description from a FT error code
////////////////////////////////////////////////////////////
std::string FontLoader::GetErrorDesc(FT_Error error)
{
    switch (error)
    {
        // Generic errors
        case FT_Err_Cannot_Open_Resource :      return "cannot open resource";
        case FT_Err_Unknown_File_Format :       return "unknown file format";
        case FT_Err_Invalid_File_Format :       return "broken file";
        case FT_Err_Invalid_Version :           return "invalid FreeType version";
        case FT_Err_Lower_Module_Version :      return "module version is too low";
        case FT_Err_Invalid_Argument :          return "invalid argument";
        case FT_Err_Unimplemented_Feature :     return "unimplemented feature";
        case FT_Err_Invalid_Table :             return "broken table";
        case FT_Err_Invalid_Offset :            return "broken offset within table";

        // Glyph / character errors
        case FT_Err_Invalid_Glyph_Index :       return "invalid glyph index";
        case FT_Err_Invalid_Character_Code :    return "invalid character code";
        case FT_Err_Invalid_Glyph_Format :      return "unsupported glyph image format";
        case FT_Err_Cannot_Render_Glyph :       return "cannot render this glyph format";
        case FT_Err_Invalid_Outline :           return "invalid outline";
        case FT_Err_Invalid_Composite :         return "invalid composite glyph";
        case FT_Err_Too_Many_Hints :            return "too many hints";
        case FT_Err_Invalid_Pixel_Size :        return "invalid pixel size";

        // Handle errors
        case FT_Err_Invalid_Handle :            return "invalid object handle";
        case FT_Err_Invalid_Library_Handle :    return "invalid library handle";
        case FT_Err_Invalid_Driver_Handle :     return "invalid module handle";
        case FT_Err_Invalid_Face_Handle :       return "invalid face handle";
        case FT_Err_Invalid_Size_Handle :       return "invalid size handle";
        case FT_Err_Invalid_Slot_Handle :       return "invalid glyph slot handle";
        case FT_Err_Invalid_CharMap_Handle :    return "invalid charmap handle";
        case FT_Err_Invalid_Cache_Handle :      return "invalid cache manager handle";
        case FT_Err_Invalid_Stream_Handle :     return "invalid stream handle";

        // Driver errors
        case FT_Err_Too_Many_Drivers :          return "too many modules";
        case FT_Err_Too_Many_Extensions :       return "too many extensions";

        // Memory errors
        case FT_Err_Out_Of_Memory :             return "out of memory";
        case FT_Err_Unlisted_Object :           return "unlisted object";

        // Stream errors
        case FT_Err_Cannot_Open_Stream :        return "cannot open stream";
        case FT_Err_Invalid_Stream_Seek :       return "invalid stream seek";
        case FT_Err_Invalid_Stream_Skip :       return "invalid stream skip";
        case FT_Err_Invalid_Stream_Read :       return "invalid stream read";
        case FT_Err_Invalid_Stream_Operation :  return "invalid stream operation";
        case FT_Err_Invalid_Frame_Operation :   return "invalid frame operation";
        case FT_Err_Nested_Frame_Access :       return "nested frame access";
        case FT_Err_Invalid_Frame_Read :        return "invalid frame read";

        // Raster errors
        case FT_Err_Raster_Uninitialized :      return "raster uninitialized";
        case FT_Err_Raster_Corrupted :          return "raster corrupted";
        case FT_Err_Raster_Overflow :           return "raster overflow";
        case FT_Err_Raster_Negative_Height :    return "negative height while rastering";

        // Cache errors
        case FT_Err_Too_Many_Caches :           return "too many registered caches";

        // TrueType and SFNT errors
        case FT_Err_Invalid_Opcode :            return "invalid opcode";
        case FT_Err_Too_Few_Arguments :         return "too few arguments";
        case FT_Err_Stack_Overflow :            return "stack overflow";
        case FT_Err_Code_Overflow :             return "code overflow";
        case FT_Err_Bad_Argument :              return "bad argument";
        case FT_Err_Divide_By_Zero :            return "division by zero";
        case FT_Err_Invalid_Reference :         return "invalid reference";
        case FT_Err_Debug_OpCode :              return "found debug opcode";
        case FT_Err_ENDF_In_Exec_Stream :       return "found ENDF opcode in execution stream";
        case FT_Err_Nested_DEFS :               return "nested DEFS";
        case FT_Err_Invalid_CodeRange :         return "invalid code range";
        case FT_Err_Execution_Too_Long :        return "execution context too long";
        case FT_Err_Too_Many_Function_Defs :    return "too many function definitions";
        case FT_Err_Too_Many_Instruction_Defs : return "too many instruction definitions";
        case FT_Err_Table_Missing :             return "SFNT font table missing";
        case FT_Err_Horiz_Header_Missing :      return "horizontal header (hhea) table missing";
        case FT_Err_Locations_Missing :         return "locations (loca) table missing";
        case FT_Err_Name_Table_Missing :        return "name table missing";
        case FT_Err_CMap_Table_Missing :        return "character map (cmap) table missing";
        case FT_Err_Hmtx_Table_Missing :        return "horizontal metrics (hmtx) table missing";
        case FT_Err_Post_Table_Missing :        return "PostScript (post) table missing";
        case FT_Err_Invalid_Horiz_Metrics :     return "invalid horizontal metrics";
        case FT_Err_Invalid_CharMap_Format :    return "invalid character map (cmap) format";
        case FT_Err_Invalid_PPem :              return "invalid ppem value";
        case FT_Err_Invalid_Vert_Metrics :      return "invalid vertical metrics";
        case FT_Err_Could_Not_Find_Context :    return "could not find context";
        case FT_Err_Invalid_Post_Table_Format : return "invalid PostScript (post) table format";
        case FT_Err_Invalid_Post_Table :        return "invalid PostScript (post) table";

        // CCF, CID and Type 1 errors
        case FT_Err_Syntax_Error :              return "opcode syntax error";
        case FT_Err_Stack_Underflow :           return "argument stack underflow";
        case FT_Err_Ignore :                    return "ignore";

        // BDF errors
        case FT_Err_Missing_Startfont_Field :   return "`STARTFONT' field missing";
        case FT_Err_Missing_Font_Field :        return "`FONT' field missing";
        case FT_Err_Missing_Size_Field :        return "`SIZE' field missing";
        case FT_Err_Missing_Chars_Field :       return "`CHARS' field missing";
        case FT_Err_Missing_Startchar_Field :   return "`STARTCHAR' field missing";
        case FT_Err_Missing_Encoding_Field :    return "`ENCODING' field missing";
        case FT_Err_Missing_Bbx_Field :         return "`BBX' field missing";
    }

    return "unknown error";
}

} // namespace priv

} // namespace sf

#else

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Graphics/FontLoader.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Image.hpp>
#include <SFML/Graphics/GLCheck.hpp>
#define STB_TRUETYPE_IMPLEMENTATION
#include <SFML/Graphics/stb_truetype/stb_truetype.h>
#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include <math.h>


namespace
{
    ////////////////////////////////////////////////////////////
    // Functor to sort glyphs by size
    ////////////////////////////////////////////////////////////
    struct SizeCompare
    {
        bool operator ()(const sf::Glyph& left, const sf::Glyph& right) const
        {
            return left.Rectangle.GetSize().y < right.Rectangle.GetSize().y;
        }
    };
}

namespace sf
{
namespace priv
{
////////////////////////////////////////////////////////////
/// Get the unique instance of the class
////////////////////////////////////////////////////////////
FontLoader& FontLoader::GetInstance()
{
    static FontLoader instance;

    return instance;
}


////////////////////////////////////////////////////////////
/// Default constructor
////////////////////////////////////////////////////////////
FontLoader::FontLoader()
{
}


////////////////////////////////////////////////////////////
/// Destructor
////////////////////////////////////////////////////////////
FontLoader::~FontLoader()
{
}


////////////////////////////////////////////////////////////
/// Load a font from a file
////////////////////////////////////////////////////////////
bool FontLoader::LoadFontFromFile(const std::string& filename, unsigned int charSize, const Unicode::UTF32String& charset, Font& font)
{
    // Get the contents of the font file
    std::ifstream file(filename.c_str(), std::ios_base::binary);
    if (!file)
        return false;
    file.seekg(0, std::ios::end);
    std::size_t length = file.tellg();
    file.seekg(0, std::ios::beg);
    std::vector<char> data(length);
    file.read(&data[0], static_cast<std::streamsize>(length));

    // Load from memory
    return LoadFontFromMemory(&data[0], data.size(), charSize, charset, font);
}


////////////////////////////////////////////////////////////
/// Load the font from a file in memory
////////////////////////////////////////////////////////////
bool FontLoader::LoadFontFromMemory(const char* data, std::size_t sizeInBytes, unsigned int charSize, const Unicode::UTF32String& charset, Font& font)
{
    // Let's find how many characters to put in each row to make them fit into a squared texture
    GLint maxSize;
    GLCheck(glGetIntegerv(GL_MAX_TEXTURE_SIZE, &maxSize));
    int nbChars = static_cast<int>(sqrt(static_cast<double>(charset.length())) * 0.75);

    // Clamp the character size to make sure we won't create a texture too big
    if (nbChars * charSize >= static_cast<unsigned int>(maxSize))
        charSize = maxSize / nbChars;

    // Initialize the dimensions
    unsigned int left      = 0;
    unsigned int top       = 0;
    unsigned int texWidth  = Image::GetValidTextureSize(charSize * nbChars);
    unsigned int texHeight = charSize * nbChars;
    std::vector<unsigned int> tops(texWidth, 0);

    // Create a pixel buffer for rendering every glyph
    std::vector<Uint8> glyphsBuffer(texWidth * texHeight * 4);

    // Load the font
    stbtt_fontinfo info;
    int success = stbtt_InitFont(&info, reinterpret_cast<const unsigned char*>(data), 0);
    if (!success)
        return false;

    // Compute the global scale to apply to match the character size
    float scale = stbtt_ScaleForPixelHeight(&info, static_cast<float>(charSize));

    // Render all glyphs and sort them by size to optimize texture space
    typedef std::multimap<Glyph, Uint32, SizeCompare> GlyphTable;
    GlyphTable glyphs;
    for (std::size_t i = 0; i < charset.length(); ++i)
    {
        // Load the glyph corresponding to the current character
        int index = stbtt_FindGlyphIndex(&info, static_cast<int>(charset[i]));

        // Extract the glyph parameters (bounding box, horizontal advance)
        Glyph glyph;
        stbtt_GetGlyphHMetrics(&info, index, &glyph.Advance, NULL);
        stbtt_GetGlyphBitmapBox(&info, index, scale, scale, &glyph.Rectangle.Left, &glyph.Rectangle.Top, &glyph.Rectangle.Right, &glyph.Rectangle.Bottom);

        // Apply the global scale to the horizontal advance
        glyph.Advance = static_cast<int>(glyph.Advance * scale);

        // Add it to the sorted table of glyphs
        glyphs.insert(std::make_pair(glyph, charset[i]));
    }

    // Leave a small margin around characters, so that filtering doesn't
    // pollute them with pixels from neighbours
    unsigned int padding = 1;
    unsigned int margin = 1;

    // Copy the rendered glyphs into the texture
    unsigned int maxHeight = 0;
    std::map<Uint32, IntRect> coords;
    for (GlyphTable::const_iterator i = glyphs.begin(); i != glyphs.end(); ++i)
    {
        // Get the bitmap of the current glyph
        Glyph&          curGlyph    = font.myGlyphs[i->second];
        const Glyph&    bitmapGlyph = i->first;
        const Vector2i& glyphSize   = bitmapGlyph.Rectangle.GetSize();

        // Make sure we don't go over the texture width
        if (left + glyphSize.x + 2 * padding + margin >= texWidth)
            left = 0;

        // Compute the top coordinate
        top = tops[left];
        for (unsigned int x = 0; x < glyphSize.x + 2 * padding + margin; ++x)
            top = std::max(top, tops[left + x]);

        // Make sure we don't go over the texture height -- resize it if we need more space
        if (top + glyphSize.x + 2 * padding + margin >= texHeight)
        {
            texHeight *= 2;
            glyphsBuffer.resize(texWidth * texHeight * 4);
        }

        // Store the character's position and size
        curGlyph.Rectangle.Left   = bitmapGlyph.Rectangle.Left - padding;
        curGlyph.Rectangle.Top    = bitmapGlyph.Rectangle.Top - padding;
        curGlyph.Rectangle.Right  = bitmapGlyph.Rectangle.Right + padding;
        curGlyph.Rectangle.Bottom = bitmapGlyph.Rectangle.Bottom + padding;
        curGlyph.Advance          = bitmapGlyph.Advance;

        // Texture size may change, so let the texture coordinates be calculated later
        coords[i->second] = IntRect(left, top, left + glyphSize.x + 2 * padding, top + glyphSize.y + 2 * padding);

        // Draw the glyph into our bitmap font
        int width, height;
        unsigned char* bitmap = stbtt_GetCodepointBitmap(&info, scale, scale, i->second, &width, &height, NULL, NULL);
        unsigned char* pixels = bitmap;
        for (int y = 0; y < height; ++y)
        {
            for (int x = 0; x < width; ++x)
            {
                std::size_t index = x + left + padding + (y + top + padding) * texWidth;
                glyphsBuffer[index * 4 + 0] = 255;
                glyphsBuffer[index * 4 + 1] = 255;
                glyphsBuffer[index * 4 + 2] = 255;
                glyphsBuffer[index * 4 + 3] = pixels[x];
            }
            pixels += width;
        }

        // Update the rendering coordinates
        for (unsigned int x = 0; x < width + 2 * padding + margin; ++x)
            tops[left + x] = top + height + 2 * padding + margin;
        left += width + 2 * padding + margin;
        if (top + height + 2 * padding > maxHeight)
            maxHeight = top + height + 2 * padding;

        // Delete the bitmap
        stbtt_FreeBitmap(bitmap, NULL);
    }

    // Create the font's texture
    texHeight = maxHeight;
    glyphsBuffer.resize(texWidth * texHeight * 4);
    font.myTexture.LoadFromPixels(texWidth, texHeight, &glyphsBuffer[0]);

    // Now that the texture is created, we can precompute texture coordinates
    for (std::size_t i = 0; i < charset.size(); ++i)
    {
        Uint32 curChar = charset[i];
        font.myGlyphs[curChar].TexCoords = font.myTexture.GetTexCoords(coords[curChar]);
    }

    // Update the character size (it may have been changed by the function)
    font.myCharSize = charSize;

    return 0;
}


////////////////////////////////////////////////////////////
/// Create a bitmap font from a font face and a characters set
////////////////////////////////////////////////////////////
FT_Error FontLoader::CreateBitmapFont(FT_Face face, unsigned int charSize, const Unicode::UTF32String& charset, Font& font)
{
    return 0;
}


////////////////////////////////////////////////////////////
/// Get a description from a FT error code
////////////////////////////////////////////////////////////
std::string FontLoader::GetErrorDesc(FT_Error error)
{
    return "";
}

} // namespace priv

} // namespace sf

#endif
