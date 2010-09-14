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

#ifndef SFML_FONT_HPP
#define SFML_FONT_HPP

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/System/Resource.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/System/String.hpp>
#include <SFML/Graphics/Glyph.hpp>
#include <SFML/Graphics/Image.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <map>
#include <string>
#include <vector>


namespace sf
{
////////////////////////////////////////////////////////////
/// \brief Class for loading and manipulating character fonts
///
////////////////////////////////////////////////////////////
class SFML_API Font : public Resource<Font>
{
public :

    ////////////////////////////////////////////////////////////
    /// \brief Default constructor
    ///
    /// This constructor defines an empty font
    ///
    ////////////////////////////////////////////////////////////
    Font();

    ////////////////////////////////////////////////////////////
    /// \brief Copy constructor
    ///
    /// \param copy Instance to copy
    ///
    ////////////////////////////////////////////////////////////
    Font(const Font& copy);

    ////////////////////////////////////////////////////////////
    /// \brief Destructor
    ///
    /// Cleans up all the internal resources used by the font
    ///
    ////////////////////////////////////////////////////////////
    ~Font();

    ////////////////////////////////////////////////////////////
    /// \brief Load the font from a file
    ///
    /// The supported font formats are: TrueType, Type 1, CFF,
    /// OpenType, SFNT, X11 PCF, Windows FNT, BDF, PFR and Type 42.
    /// Note that this function know nothing about the standard
    /// fonts installed on the user's system, thus you can't
    /// load them directly.
    ///
    /// \param filename Path of the font file to load
    ///
    /// \return True if loading succeeded, false if it failed
    ///
    /// \see LoadFromMemory
    ///
    ////////////////////////////////////////////////////////////
    bool LoadFromFile(const std::string& filename);

    ////////////////////////////////////////////////////////////
    /// \brief Load the font from a file in memory
    ///
    /// The supported font formats are: TrueType, Type 1, CFF,
    /// OpenType, SFNT, X11 PCF, Windows FNT, BDF, PFR and Type 42.
    /// Note that this function know nothing about the standard
    /// fonts installed on the user's system, thus you can't
    /// load them directly.
    /// Warning: SFML cannot preload all the font data in this
    /// function, so the buffer pointed by \a data has to remain
    /// valid as long as the font is used.
    ///
    /// \param data        Pointer to the file data in memory
    /// \param sizeInBytes Size of the data to load, in bytes
    ///
    /// \return True if loading succeeded, false if it failed
    ///
    /// \see LoadFromFile
    ///
    ////////////////////////////////////////////////////////////
    bool LoadFromMemory(const void* data, std::size_t sizeInBytes);

    ////////////////////////////////////////////////////////////
    /// \brief Retrieve a glyph of the font
    ///
    /// \param codePoint     Unicode code point of the character to get
    /// \param characterSize Reference character size
    /// \param bold          Retrieve the bold version or the regular one?
    ///
    /// \return The glyph corresponding to \a codePoint and \a characterSize
    ///
    ////////////////////////////////////////////////////////////
    const Glyph& GetGlyph(Uint32 codePoint, unsigned int characterSize, bool bold) const;

    ////////////////////////////////////////////////////////////
    /// \brief Get the kerning offset of two glyphs
    ///
    /// The kerning is an extra offset (negative) to apply between two
    /// glyphs when rendering them, to make the pair look more "natural".
    /// For example, the pair "AV" have a special kerning to make them
    /// closer than other characters. Most of the glyphs pairs have a
    /// kerning offset of zero, though.
    ///
    /// \param first         Unicode code point of the first character
    /// \param second        Unicode code point of the second character
    /// \param characterSize Reference character size
    ///
    /// \return Kerning value for \a first and \a second, in pixels
    ///
    ////////////////////////////////////////////////////////////
    int GetKerning(Uint32 first, Uint32 second, unsigned int characterSize) const;

    ////////////////////////////////////////////////////////////
    /// \brief Get the line spacing
    ///
    /// Line spacing is the vertical offset to apply between two
    /// consecutive lines of text.
    ///
    /// \param characterSize Reference character size
    ///
    /// \return Line spacing, in pixels
    ///
    ////////////////////////////////////////////////////////////
    int GetLineSpacing(unsigned int characterSize) const;

    ////////////////////////////////////////////////////////////
    /// \brief Retrieve the image containing the loaded glyphs of a certain size
    ///
    /// The contents of the returned image changes as more glyphs
    /// are requested, thus it is not very relevant. It is mainly
    /// used internally by sf::Text.
    ///
    /// \param characterSize Reference character size
    ///
    /// \return Image containing the glyphs of the requested size
    ///
    ////////////////////////////////////////////////////////////
    const Image& GetImage(unsigned int characterSize) const;

    ////////////////////////////////////////////////////////////
    /// \brief Overload of assignment operator
    ///
    /// \param right Instance to assign
    ///
    /// \return Reference to self
    ///
    ////////////////////////////////////////////////////////////
    Font& operator =(const Font& right);

    ////////////////////////////////////////////////////////////
    /// \brief Return the default built-in font
    ///
    /// This font is provided for convenience, it is used by
    /// sf::Text instances by default. It is provided so that
    /// users don't have to provide and load a font file in order
    /// to display text on screen.
    /// The font used is Arial.
    ///
    /// \return Reference to the built-in default font
    ///
    ////////////////////////////////////////////////////////////
    static const Font& GetDefaultFont();

private :

    ////////////////////////////////////////////////////////////
    /// \brief Structure defining a row of glyphs
    ///
    ////////////////////////////////////////////////////////////
    struct Row
    {
        Row(unsigned int top, unsigned int height) : Width(0), Top(top), Height(height) {}

        unsigned int Width;  ///< Current width of the row
        unsigned int Top;    ///< Y position of the row into the texture
        unsigned int Height; ///< Height of the row
    };

    ////////////////////////////////////////////////////////////
    // Types
    ////////////////////////////////////////////////////////////
    typedef std::map<Uint32, Glyph> GlyphTable; ///< Table mapping a codepoint to its glyph

    ////////////////////////////////////////////////////////////
    /// \brief Structure defining a page if glyphs
    ///
    ////////////////////////////////////////////////////////////
    struct Page
    {
        Page();

        GlyphTable       Glyphs;  ///< Table mapping code points to their corresponding glyph
        Image            Texture; ///< Image containing the pixels of the glyphs
        unsigned int     NextRow; ///< Y position of the next new row in the image
        std::vector<Row> Rows;    ///< List containing the position of all the existing rows
    };

    ////////////////////////////////////////////////////////////
    /// \brief Free all the internal resources
    ///
    ////////////////////////////////////////////////////////////
    void Cleanup();

    ////////////////////////////////////////////////////////////
    /// \brief Load a new glyph and store it in the cache
    ///
    /// \param codePoint     Unicode code point of the character to load
    /// \param characterSize Reference character size
    /// \param bold          Retrieve the bold version or the regular one?
    ///
    /// \return The glyph corresponding to \a codePoint and \a characterSize
    ///
    ////////////////////////////////////////////////////////////
    Glyph LoadGlyph(Uint32 codePoint, unsigned int characterSize, bool bold) const;

    ////////////////////////////////////////////////////////////
    /// \brief Find a suitable rectangle within the texture for a glyph
    ///
    /// \param page   Page of glyphs to search in
    /// \param width  Width of the rectangle
    /// \param height Height of the rectangle
    ///
    /// \return Found rectangle within the texture
    ///
    ////////////////////////////////////////////////////////////
    IntRect FindGlyphRect(Page& page, unsigned int width, unsigned int height) const;

    ////////////////////////////////////////////////////////////
    /// \brief Make sure that the given size is the current one
    ///
    /// \param characterSize Reference character size
    ///
    /// \return True on success, false if any error happened
    ///
    ////////////////////////////////////////////////////////////
    bool SetCurrentSize(unsigned int characterSize) const;

    ////////////////////////////////////////////////////////////
    // Types
    ////////////////////////////////////////////////////////////
    typedef std::map<unsigned int, Page> PageTable; ///< Table mapping a character size to its page (image)

    ////////////////////////////////////////////////////////////
    // Member data
    ////////////////////////////////////////////////////////////
    void*                      myLibrary;     ///< Pointer to the internal library interface (it is typeless to avoid exposing implementation details)
    void*                      myFace;        ///< Pointer to the internal font face (it is typeless to avoid exposing implementation details)
    int*                       myRefCount;    ///< Reference counter used by implicit sharing
    mutable PageTable          myPages;       ///< Table containing the glyphs pages by character size
    mutable std::vector<Uint8> myPixelBuffer; ///< Pixel buffer holding a glyph's pixels before being written to the texture
};

} // namespace sf


#endif // SFML_FONT_HPP


////////////////////////////////////////////////////////////
/// \class sf::Font
/// \ingroup graphics
///
/// Fonts can be loaded from a file or from memory, from
/// the most common types of fonts. See the LoadFromFile
/// function for the complete list of supported formats.
///
/// Once it is loaded, a sf::Font instance provides three
/// types of informations about the font:
/// \li Global metrics, such as the line spacing
/// \li Per-glyph metrics, such as bounding box or kerning
/// \li Pixel representation of glyphs
///
/// Fonts alone are not very useful: they hold the font data
/// but cannot make anything useful of it. To do so you need to
/// use the sf::Text class, which is able to properly output text
/// with several options such as character size, style, color,
/// position, rotation, etc.
/// This separation allows more flexibility and better performances:
/// indeed a sf::Font is a heavy resource, and any operation on it
/// is slow (often too slow for real-time applications). On the other
/// side, a sf::Text is a lightweight object which can combine the
/// glyphs data and metrics of a sf::Font to display any text on a
/// render target.
/// Note that it is also possible to bind several sf::Text instances
/// to the same sf::Font.
///
/// It is important to note that the sf::Text instance doesn't
/// copy the font that it uses, it only keeps a reference to it.
/// Thus, a sf::Font must not be destructed while it is
/// used by a sf::Text (i.e. never write a function that
/// uses a local sf::Font instance for creating a text).
///
/// Usage example:
/// \code
/// // Declare a new font
/// sf::Font font;
/// 
/// // Load it from a file
/// if (!font.LoadFromFile("arial.ttf"))
/// {
///     // error...
/// }
/// 
/// // Create a text which uses our font
/// sf::Text text1;
/// text1.SetFont(font);
/// text1.SetCharacterSize(30);
/// text1.SetStyle(sf::Text::Regular);
/// 
/// // Create another text using the same font, but with different parameters
/// sf::Text text2;
/// text2.SetFont(font);
/// text2.SetCharacterSize(50);
/// text1.SetStyle(sf::Text::Italic);
/// \endcode
///
/// Apart from loading font files, and passing them to instances
/// of sf::Text, you should normally not have to deal directly
/// with this class. However, it may be useful to access the
/// font metrics or rasterized glyphs for advanced usage.
///
/// \see sf::Text
///
////////////////////////////////////////////////////////////
