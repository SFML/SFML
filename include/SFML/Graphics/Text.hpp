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

#ifndef SFML_TEXT_HPP
#define SFML_TEXT_HPP

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/System/Resource.hpp>
#include <SFML/System/String.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <string>


namespace sf
{
////////////////////////////////////////////////////////////
/// \brief Graphical text that can be drawn to a render target
///
////////////////////////////////////////////////////////////
class SFML_API Text : public Drawable
{
public :

    ////////////////////////////////////////////////////////////
    /// \brief Enumeration of the string drawing styles
    ///
    ////////////////////////////////////////////////////////////
    enum Style
    {
        Regular    = 0,      ///< Regular characters, no style
        Bold       = 1 << 0, ///< Bold characters
        Italic     = 1 << 1, ///< Italic characters
        Underlined = 1 << 2  ///< Underlined characters
    };

    ////////////////////////////////////////////////////////////
    /// \brief Default constructor
    ///
    /// Creates an empty text.
    ///
    ////////////////////////////////////////////////////////////
    Text();

    ////////////////////////////////////////////////////////////
    /// \brief Construct the string from a string, font and size
    ///
    /// \param string         Text assigned to the string
    /// \param font           Font used to draw the string
    /// \param characterSize  Base size of characters, in pixels
    ///
    ////////////////////////////////////////////////////////////
    explicit Text(const String& string, const Font& font = Font::GetDefaultFont(), unsigned int characterSize = 30);

    ////////////////////////////////////////////////////////////
    /// \brief Set the text's string
    ///
    /// The \a string argument is a sf::String, which can
    /// automatically be constructed from standard string types.
    /// So, the following calls are all valid:
    /// \code
    /// text.SetString("hello");
    /// text.SetString(L"hello");
    /// text.SetString(std::string("hello"));
    /// text.SetString(std::wstring(L"hello"));
    /// \endcode
    /// A text's string is empty by default.
    ///
    /// \param string New string
    ///
    /// \see GetString
    ///
    ////////////////////////////////////////////////////////////
    void SetString(const String& string);

    ////////////////////////////////////////////////////////////
    /// \brief Set the text's font
    ///
    /// Texts have a valid font by default, which the built-in
    /// Font::GetDefaultFont().
    ///
    /// \param font New font
    ///
    /// \see GetFont
    ///
    ////////////////////////////////////////////////////////////
    void SetFont(const Font& font);

    ////////////////////////////////////////////////////////////
    /// \brief Set the character size
    ///
    /// The default size is 30.
    ///
    /// \param size New character size, in pixels
    ///
    /// \see GetCharacterSize
    ///
    ////////////////////////////////////////////////////////////
    void SetCharacterSize(unsigned int size);

    ////////////////////////////////////////////////////////////
    /// \brief Set the text's style
    ///
    /// You can pass a combination of one or more styles, for
    /// example sf::Text::Bold | sf::Text::Italic.
    /// The default style is sf::Text::Regular.
    ///
    /// \param style New style
    ///
    /// \see GetStyle
    ///
    ////////////////////////////////////////////////////////////
    void SetStyle(unsigned long style);

    ////////////////////////////////////////////////////////////
    /// \brief Get the text's string
    ///
    /// The returned string is a sf::String, which can automatically
    /// be converted to standard string types. So, the following
    /// lines of code are all valid:
    /// \code
    /// sf::String   s1 = text.GetString();
    /// std::string  s2 = text.GetString();
    /// std::wstring s3 = text.GetString();
    /// \endcode
    ///
    /// \return Text's string
    ///
    /// \see GetString
    ///
    ////////////////////////////////////////////////////////////
    const String& GetString() const;

    ////////////////////////////////////////////////////////////
    /// \brief Get the text's font
    ///
    /// The returned reference is const, which means that you
    /// cannot modify the font when you get it from this function.
    ///
    /// \return Text's font
    ///
    /// \see SetFont
    ///
    ////////////////////////////////////////////////////////////
    const Font& GetFont() const;

    ////////////////////////////////////////////////////////////
    /// \brief Get the character size
    ///
    /// \return Size of the characters, in pixels
    ///
    /// \see SetCharacterSize
    ///
    ////////////////////////////////////////////////////////////
    unsigned int GetCharacterSize() const;

    ////////////////////////////////////////////////////////////
    /// \brief Get the text's style
    ///
    /// \return Text's style
    ///
    /// \see SetStyle
    ///
    ////////////////////////////////////////////////////////////
    unsigned long GetStyle() const;

    ////////////////////////////////////////////////////////////
    /// \brief Return the position of the \a index-th character
    ///
    /// This function computes the visual position of a character
    /// from its index in the string. The returned position is
    /// in local coordinates (translation, rotation, scale and
    /// origin are not applied). You can easily get the corresponding
    /// global position with the TransformToGlobal function.
    /// If \a index is out of range, the position of the end of
    /// the string is returned.
    ///
    /// \param index Index of the character
    ///
    /// \return Position of the character
    ///
    ////////////////////////////////////////////////////////////
    Vector2f GetCharacterPos(std::size_t index) const;

    ////////////////////////////////////////////////////////////
    /// \brief Get the bounding rectangle of the text
    ///
    /// The returned rectangle is in global coordinates.
    ///
    /// \return Bounding rectangle of the text
    ///
    ////////////////////////////////////////////////////////////
    FloatRect GetRect() const;

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
    /// \brief Recompute the bounding rectangle
    ///
    ////////////////////////////////////////////////////////////
    void UpdateRect() const;

    ////////////////////////////////////////////////////////////
    // Member data
    ////////////////////////////////////////////////////////////
    String            myString;        ///< String to display
    ResourcePtr<Font> myFont;          ///< Font used to display the string
    unsigned int      myCharacterSize; ///< Base size of characters, in pixels
    unsigned long     myStyle;         ///< Text style (see Style enum)
    mutable FloatRect myBaseRect;      ///< Bounding rectangle of the text in object coordinates
    mutable bool      myRectUpdated;   ///< Is the bounding rectangle up-to-date ?
};

} // namespace sf


#endif // SFML_TEXT_HPP


////////////////////////////////////////////////////////////
/// \class sf::Text
/// \ingroup graphics
///
/// sf::Text is a drawable class that allows to easily display
/// some text with custom style and color on a render target.
///
/// It inherits all the functions from sf::Drawable:
/// position, rotation, scale, origin, global color and blend
/// mode. It also adds text-specific properties such as the
/// font to use, the character size, the font style (bold,
/// italic, underlined), and the text to display of course.
/// It also provides convenience functions to calculate the
/// graphical size of the text, or to get the visual position
/// of a given character.
///
/// sf::Text works in combination with the sf::Font class, which
/// loads and provides the glyphs (visual characters) of a given font.
///
/// The separation of sf::Font and sf::Text allows more flexibility
/// and better performances: indeed a sf::Font is a heavy resource,
/// and any operation on it is slow (often too slow for real-time
/// applications). On the other side, a sf::Text is a lightweight
/// object which can combine the glyphs data and metrics of a sf::Font
/// to display any text on a render target.
///
/// It is important to note that the sf::Text instance doesn't
/// copy the font that it uses, it only keeps a reference to it.
/// Thus, a sf::Font must not be destructed while it is
/// used by a sf::Text (i.e. never write a function that
/// uses a local sf::Font instance for creating a text).
///
/// Usage example:
/// \code
/// // Declare and load a font
/// sf::Font font;
/// font.LoadFromFile("arial.ttf");
/// 
/// // Create a text
/// sf::Text text("hello");
/// text.SetFont(font);
/// text.SetCharacterSize(30);
/// text.SetStyle(sf::Text::Regular);
///
/// // Display it
/// window.Draw(text); // window is a sf::RenderWindow
/// \endcode
///
/// \see sf::Font
///
////////////////////////////////////////////////////////////
