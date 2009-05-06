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
#include <SFML/Graphics/FontLoader.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Image.hpp>
#include <SFML/Graphics/GraphicsContext.hpp>
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
        bool operator ()(FT_BitmapGlyph Glyph1, FT_BitmapGlyph Glyph2) const
        {
            return Glyph2->bitmap.rows < Glyph1->bitmap.rows;
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
    static FontLoader Instance;

    return Instance;
}


////////////////////////////////////////////////////////////
/// Default constructor
////////////////////////////////////////////////////////////
FontLoader::FontLoader()
{
    // Initialize FreeType library
    FT_Error Error = FT_Init_FreeType(&myLibrary);
    if (Error)
    {
        std::cerr << "Failed to initialize FreeType library (error code : " << Error << ")" << std::endl;
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
bool FontLoader::LoadFontFromFile(const std::string& Filename, unsigned int CharSize, const Unicode::UTF32String& Charset, Font& LoadedFont)
{
    // Check if Freetype is correctly initialized
    if (!myLibrary)
    {
        std::cerr << "Failed to load font \"" << Filename << "\", FreeType has not been initialized" << std::endl;
        return false;
    }

    // Create a new font face from the specified file
    FT_Face FontFace;
    FT_Error Error = FT_New_Face(myLibrary, Filename.c_str(), 0, &FontFace);
    if (Error)
    {
        std::cerr << "Failed to load font \"" << Filename << "\" (" << GetErrorDesc(Error) << ")" << std::endl;
        return false;
    }

    // Create the bitmap font
    Error = CreateBitmapFont(FontFace, CharSize, Charset, LoadedFont);
    if (Error)
        std::cerr << "Failed to load font \"" << Filename << "\" (" << GetErrorDesc(Error) << ")" << std::endl;

    // Delete the font
    FT_Done_Face(FontFace);

    return Error == 0;
}


////////////////////////////////////////////////////////////
/// Load the font from a file in memory
////////////////////////////////////////////////////////////
bool FontLoader::LoadFontFromMemory(const char* Data, std::size_t SizeInBytes, unsigned int CharSize, const Unicode::UTF32String& Charset, Font& LoadedFont)
{
    // Check if Freetype is correctly initialized
    if (!myLibrary)
    {
        std::cerr << "Failed to load font from memory, FreeType has not been initialized" << std::endl;
        return false;
    }

    // Create a new font face from the specified memory data
    FT_Face FontFace;
    FT_Error Error = FT_New_Memory_Face(myLibrary, reinterpret_cast<const FT_Byte*>(Data), static_cast<FT_Long>(SizeInBytes), 0, &FontFace);
    if (Error)
    {
        std::cerr << "Failed to load font from memory (" << GetErrorDesc(Error) << ")" << std::endl;
        return false;
    }

    // Create the bitmap font
    Error = CreateBitmapFont(FontFace, CharSize, Charset, LoadedFont);
    if (Error)
        std::cerr << "Failed to load font from memory (" << GetErrorDesc(Error) << ")" << std::endl;

    // Delete the font
    FT_Done_Face(FontFace);

    return Error == 0;
}


////////////////////////////////////////////////////////////
/// Create a bitmap font from a font face and a characters set
////////////////////////////////////////////////////////////
FT_Error FontLoader::CreateBitmapFont(FT_Face FontFace, unsigned int CharSize, const Unicode::UTF32String& Charset, Font& LoadedFont)
{
    // Make sure we have a valid context
    priv::GraphicsContext Ctx;

    // Let's find how many characters to put in each row to make them fit into a squared texture
    GLint MaxSize;
    GLCheck(glGetIntegerv(GL_MAX_TEXTURE_SIZE, &MaxSize));
    int NbChars = static_cast<int>(sqrt(static_cast<double>(Charset.length())) * 0.75);

    // Clamp the character size to make sure we won't create a texture too big
    if (NbChars * CharSize >= static_cast<unsigned int>(MaxSize))
        CharSize = MaxSize / NbChars;

    // Initialize the dimensions
    unsigned int Left      = 0;
    unsigned int Top       = 0;
    unsigned int TexWidth  = Image::GetValidTextureSize(CharSize * NbChars);
    unsigned int TexHeight = CharSize * NbChars;
    std::vector<unsigned int> Tops(TexWidth, 0);

    // Create a pixel buffer for rendering every glyph
    std::vector<Uint8> GlyphsBuffer(TexWidth * TexHeight * 4);

    // Setup the font size
    FT_Error Error = FT_Set_Pixel_Sizes(FontFace, CharSize, CharSize);
    if (Error)
        return Error;

    // Select the unicode character map
    Error = FT_Select_Charmap(FontFace, FT_ENCODING_UNICODE);
    if (Error)
        return Error;

    // Render all glyphs and sort them by size to optimize texture space
    typedef std::multimap<FT_BitmapGlyph, Uint32, SizeCompare> GlyphTable;
    GlyphTable Glyphs;
    for (std::size_t i = 0; i < Charset.length(); ++i)
    {
        // Load the glyph corresponding to the current character
        Error = FT_Load_Char(FontFace, Charset[i], FT_LOAD_TARGET_NORMAL);
        if (Error)
            return Error;

        // Convert the glyph to a bitmap (ie. rasterize it)
        FT_Glyph Glyph;
        Error = FT_Get_Glyph(FontFace->glyph, &Glyph);
        if (Error)
            return Error;
        FT_Glyph_To_Bitmap(&Glyph, FT_RENDER_MODE_NORMAL, 0, 1);
        FT_BitmapGlyph BitmapGlyph = (FT_BitmapGlyph)Glyph;

        // Should we handle other pixel modes ?
        if (BitmapGlyph->bitmap.pixel_mode != FT_PIXEL_MODE_GRAY)
            return FT_Err_Cannot_Render_Glyph;

        // Add it to the sorted table of glyphs
        Glyphs.insert(std::make_pair(BitmapGlyph, Charset[i]));
    }

    // Copy the rendered glyphs into the texture
    unsigned int MaxHeight = 0;
    std::map<Uint32, IntRect> Coords;
    for (GlyphTable::const_iterator i = Glyphs.begin(); i != Glyphs.end(); ++i)
    {
        // Get the bitmap of the current glyph
        Glyph&         CurGlyph    = LoadedFont.myGlyphs[i->second];
        FT_BitmapGlyph BitmapGlyph = i->first;
        FT_Bitmap&     Bitmap      = BitmapGlyph->bitmap;

        // Make sure we don't go over the texture width
        if (Left + Bitmap.width + 1 >= TexWidth)
            Left = 0;

        // Compute the top coordinate
        Top = Tops[Left];
        for (int x = 0; x < Bitmap.width + 1; ++x)
            Top = std::max(Top, Tops[Left + x]);
        Top++;

        // Make sure we don't go over the texture height -- resize it if we need more space
        if (Top + Bitmap.rows + 1 >= TexHeight)
        {
            TexHeight *= 2;
            GlyphsBuffer.resize(TexWidth * TexHeight * 4);
        }

        // Store the character's position and size
        CurGlyph.Rectangle.Left   = BitmapGlyph->left;
        CurGlyph.Rectangle.Top    = -BitmapGlyph->top;
        CurGlyph.Rectangle.Right  = CurGlyph.Rectangle.Left + Bitmap.width;
        CurGlyph.Rectangle.Bottom = Bitmap.rows - BitmapGlyph->top;
        CurGlyph.Advance          = BitmapGlyph->root.advance.x >> 16;

        // Texture size may change, so let the texture coordinates be calculated later
        Coords[i->second] = IntRect(Left + 1, Top + 1, Left + Bitmap.width + 1, Top + Bitmap.rows + 1);

        // Draw the glyph into our bitmap font
        const Uint8* Pixels = Bitmap.buffer;
        for (int y = 0; y < Bitmap.rows; ++y)
        {
            for (int x = 0; x < Bitmap.width; ++x)
            {
                std::size_t Index = x + Left + 1 + (y + Top + 1) * TexWidth;
                GlyphsBuffer[Index * 4 + 0] = 255;
                GlyphsBuffer[Index * 4 + 1] = 255;
                GlyphsBuffer[Index * 4 + 2] = 255;
                GlyphsBuffer[Index * 4 + 3] = Pixels[x];
            }
            Pixels += Bitmap.pitch;
        }

        // Update the rendering coordinates
        for (int x = 0; x < Bitmap.width + 1; ++x)
            Tops[Left + x] = Top + Bitmap.rows;
        Left += Bitmap.width + 1;
        if (Top + Bitmap.rows > MaxHeight)
            MaxHeight = Top + Bitmap.rows;

        // Delete the glyph
        FT_Done_Glyph((FT_Glyph)BitmapGlyph);
    }

    // Create the font's texture
    TexHeight = MaxHeight + 1;
    GlyphsBuffer.resize(TexWidth * TexHeight * 4);
    LoadedFont.myTexture.LoadFromPixels(TexWidth, TexHeight, &GlyphsBuffer[0]);

    // Now that the texture is created, we can precompute texture coordinates
    for (std::size_t i = 0; i < Charset.size(); ++i)
    {
        Uint32 CurChar = Charset[i];
        LoadedFont.myGlyphs[CurChar].TexCoords = LoadedFont.myTexture.GetTexCoords(Coords[CurChar], false);
    }

    // Update the character size (it may have been changed by the function)
    LoadedFont.myCharSize = CharSize;

    return 0;
}


////////////////////////////////////////////////////////////
/// Get a description from a FT error code
////////////////////////////////////////////////////////////
std::string FontLoader::GetErrorDesc(FT_Error Error)
{
    switch (Error)
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

