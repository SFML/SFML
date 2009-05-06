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

#ifndef SFML_STRING_HPP
#define SFML_STRING_HPP

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/System/Resource.hpp>
#include <SFML/System/Unicode.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <string>


namespace sf
{
////////////////////////////////////////////////////////////
/// String defines a graphical 2D text, that can be drawn on screen
////////////////////////////////////////////////////////////
class SFML_API String : public Drawable
{
public :

    ////////////////////////////////////////////////////////////
    /// Enumerate the string drawing styles
    ////////////////////////////////////////////////////////////
    enum Style
    {
        Regular    = 0,      ///< Regular characters, no style
        Bold       = 1 << 0, ///< Characters are bold
        Italic     = 1 << 1, ///< Characters are in italic
        Underlined = 1 << 2  ///< Characters are underlined
    };

    ////////////////////////////////////////////////////////////
    /// Default constructor
    ///
    ////////////////////////////////////////////////////////////
    String();

    ////////////////////////////////////////////////////////////
    /// Construct the string from any kind of text
    ///
    /// \param Text : Text assigned to the string
    /// \param Font : Font used to draw the string (SFML built-in font by default)
    /// \param Size : Characters size (30 by default)
    ///
    ////////////////////////////////////////////////////////////
    explicit String(const Unicode::Text& Text, const Font& CharFont = Font::GetDefaultFont(), float Size = 30.f);

    ////////////////////////////////////////////////////////////
    /// Set the text (from any kind of string)
    ///
    /// \param Text : New text
    ///
    ////////////////////////////////////////////////////////////
    void SetText(const Unicode::Text& Text);

    ////////////////////////////////////////////////////////////
    /// Set the font of the string
    ///
    /// \param Font : Font to use
    ///
    ////////////////////////////////////////////////////////////
    void SetFont(const Font& CharFont);

    ////////////////////////////////////////////////////////////
    /// Set the size of the string
    /// The default size is 30
    ///
    /// \param Size : New size, in pixels
    ///
    ////////////////////////////////////////////////////////////
    void SetSize(float Size);

    ////////////////////////////////////////////////////////////
    /// Set the style of the text
    /// The default style is Regular
    ///
    /// \param TextStyle : New text style, (combination of Style enum values)
    ///
    ////////////////////////////////////////////////////////////
    void SetStyle(unsigned long TextStyle);

    ////////////////////////////////////////////////////////////
    /// Get the text (the returned text can be converted implicitely to any kind of string)
    ///
    /// \return String's text
    ///
    ////////////////////////////////////////////////////////////
    const Unicode::Text& GetText() const;

    ////////////////////////////////////////////////////////////
    /// Get the font used by the string
    ///
    /// \return Font used
    ///
    ////////////////////////////////////////////////////////////
    const Font& GetFont() const;

    ////////////////////////////////////////////////////////////
    /// Get the size of the characters
    ///
    /// \return Size of the characters
    ///
    ////////////////////////////////////////////////////////////
    float GetSize() const;

    ////////////////////////////////////////////////////////////
    /// Get the style of the text
    ///
    /// \return Current string style (combination of Style enum values)
    ///
    ////////////////////////////////////////////////////////////
    unsigned long GetStyle() const;

    ////////////////////////////////////////////////////////////
    /// Return the visual position of the Index-th character of the string,
    /// in coordinates relative to the string
    /// (note : translation, center, rotation and scale are not applied)
    ///
    /// \param Index : Index of the character
    ///
    /// \return Position of the Index-th character (end of string if Index is out of range)
    ///
    ////////////////////////////////////////////////////////////
    sf::Vector2f GetCharacterPos(std::size_t Index) const;

    ////////////////////////////////////////////////////////////
    /// Get the string rectangle on screen
    ///
    /// \return Rectangle contaning the string in screen coordinates
    ///
    ////////////////////////////////////////////////////////////
    FloatRect GetRect() const;

protected :

    ////////////////////////////////////////////////////////////
    /// /see Drawable::Render
    ///
    ////////////////////////////////////////////////////////////
    virtual void Render(RenderTarget& Target) const;

private :

    ////////////////////////////////////////////////////////////
    /// Recompute the bounding rectangle of the text
    ///
    ////////////////////////////////////////////////////////////
    void RecomputeRect();

    ////////////////////////////////////////////////////////////
    // Member data
    ////////////////////////////////////////////////////////////
    Unicode::Text     myText;           ///< Text to display
    ResourcePtr<Font> myFont;           ///< Font used to display the string
    float             mySize;           ///< Size of the characters
    unsigned long     myStyle;          ///< Text style (see Style enum)
    FloatRect         myBaseRect;       ///< Bounding rectangle of the text in object coordinates
    bool              myNeedRectUpdate; ///< Does the bounding rect need an update ?
};

} // namespace sf


#endif // SFML_STRING_HPP
