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
/// Text defines a graphical 2D text, that can be drawn on screen
////////////////////////////////////////////////////////////
class SFML_API Text : public Drawable
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
    Text();

    ////////////////////////////////////////////////////////////
    /// Construct the string from any kind of text
    ///
    /// \param string :        Text assigned to the string
    /// \param font :          Font used to draw the string
    /// \param characterSize : Base size of characters, in pixels
    ///
    ////////////////////////////////////////////////////////////
    explicit Text(const String& string, const Font& font = Font::GetDefaultFont(), unsigned int characterSize = 30);

    ////////////////////////////////////////////////////////////
    /// Set the text (from any kind of string)
    ///
    /// \param string : New text
    ///
    ////////////////////////////////////////////////////////////
    void SetString(const String& string);

    ////////////////////////////////////////////////////////////
    /// Set the font of the string
    ///
    /// \param font : Font to use
    ///
    ////////////////////////////////////////////////////////////
    void SetFont(const Font& font);

    ////////////////////////////////////////////////////////////
    /// Set the base size for the characters.
    /// The default size is 30
    ///
    /// \param size : New size, in pixels
    ///
    ////////////////////////////////////////////////////////////
    void SetCharacterSize(unsigned int size);

    ////////////////////////////////////////////////////////////
    /// Set the style of the text
    /// The default style is Regular
    ///
    /// \param style : New text style (combination of Style enum values)
    ///
    ////////////////////////////////////////////////////////////
    void SetStyle(unsigned long style);

    ////////////////////////////////////////////////////////////
    /// Get the text (the returned text can be converted implicitely to any kind of string)
    ///
    /// \return String's text
    ///
    ////////////////////////////////////////////////////////////
    const String& GetString() const;

    ////////////////////////////////////////////////////////////
    /// Get the font used by the string
    ///
    /// \return Font used
    ///
    ////////////////////////////////////////////////////////////
    const Font& GetFont() const;

    ////////////////////////////////////////////////////////////
    /// Get the base size of characters
    ///
    /// \return Size of the characters, in pixels
    ///
    ////////////////////////////////////////////////////////////
    unsigned int GetCharacterSize() const;

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
    /// \param index : Index of the character
    ///
    /// \return Position of the index-th character (end of string if Index is out of range)
    ///
    ////////////////////////////////////////////////////////////
    Vector2f GetCharacterPos(std::size_t index) const;

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
    virtual void Render(RenderTarget& target, RenderQueue& queue) const;

private :

    ////////////////////////////////////////////////////////////
    /// Recompute the bounding rectangle of the text
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
