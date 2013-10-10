////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007-2013 Laurent Gomila (laurent.gom@gmail.com)
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

#ifndef SFML_COMPLEX_TEXT_HPP
#define SFML_COMPLEX_TEXT_HPP

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Graphics/Text.hpp>
#include <harfbuzz/hb.h>
#include <harfbuzz/hb-ft.h>

namespace sf
{
    
class SFML_GRAPHICS_API ComplexText : public Text {
public:
    
    ////////////////////////////////////////////////////////////
    /// \brief Default constructor
    ///
    /// Creates an empty text.
    ///
    ////////////////////////////////////////////////////////////
    ComplexText();

    ////////////////////////////////////////////////////////////
    /// \brief Set the direction of the language to be rendered
    ///
    /// Latin languages render left to right (LTR) languages
    /// such as Arabic and Hebrew render right to left (LTR)
    /// and languages such as chinese render top to bottom (TTB)
    ///
    /// If not set will try to guess from the text.
    ///
    ////////////////////////////////////////////////////////////
    void SetDirection(hb_direction_t dir);
    
    ////////////////////////////////////////////////////////////
    /// \brief Set the script that will be used to shape the 
    /// text
    ///
    /// Some complex languages like Arabic and Hindi are written
    /// using complex scripts in which a letter's form (glyph) 
    /// depends on the letters surrounding it.
    ///
    /// If not set will try to guess from the text.
    ///
    ////////////////////////////////////////////////////////////
    void SetScript(hb_script_t script);

protected :

    ////////////////////////////////////////////////////////////
    /// \brief Update the text's geometry
    ///
    ////////////////////////////////////////////////////////////
    virtual void updateGeometry();

    ////////////////////////////////////////////////////////////
    // Member data
    ////////////////////////////////////////////////////////////
    hb_direction_t m_dir;
    hb_script_t    m_script;

};

} // namespace sf


#endif // SFML_COMPLEX_TEXT_HPP


////////////////////////////////////////////////////////////
/// \class sf::ComplexText
/// \ingroup graphics
///
/// This class aims to be identical to sf::Text in behavior
/// with the added ability to render complex scripts like
/// Arabic, Hebrew and Devanagari.
///
/// If your usage does not require rendering such scripts
/// you should use the sf::Text class as it will perform better.
///
/// Usage example:
/// \code
/// // Declare and load a font
/// sf::Font font;
/// font.loadFromFile("arial.ttf");
/// 
/// // Create a text
/// sf::Text text("كيف حالك؟", font);
/// text.setCharacterSize(30);
/// text.setStyle(sf::Text::Bold);
/// text.setColor(sf::Color::Red);
///
/// // Draw it
/// window.draw(text);
/// \endcode
///
/// \see sf::Font, sf::Transformable
///
////////////////////////////////////////////////////////////
