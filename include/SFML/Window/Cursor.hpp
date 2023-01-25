////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007-2023 Laurent Gomila (laurent@sfml-dev.org)
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

#pragma once

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Window/Export.hpp>

#include <SFML/System/Vector2.hpp>

#include <cstdint>
#include <memory>

namespace sf
{
namespace priv
{
class CursorImpl;
}

////////////////////////////////////////////////////////////
/// \brief Cursor defines the appearance of a system cursor
///
////////////////////////////////////////////////////////////
class SFML_WINDOW_API Cursor
{
public:
    ////////////////////////////////////////////////////////////
    /// \brief Enumeration of the native system cursor types
    ///
    /// Refer to the following table to determine which cursor
    /// is available on which platform.
    ///
    ///  Type                               | Linux | Mac OS X | Windows  |
    /// ------------------------------------|:-----:|:--------:|:--------:|
    ///  sf::Cursor::Arrow                  |  yes  |    yes   |   yes    |
    ///  sf::Cursor::ArrowWait              |  no   |    no    |   yes    |
    ///  sf::Cursor::Wait                   |  yes  |    no    |   yes    |
    ///  sf::Cursor::Text                   |  yes  |    yes   |   yes    |
    ///  sf::Cursor::Hand                   |  yes  |    yes   |   yes    |
    ///  sf::Cursor::SizeHorizontal         |  yes  |    yes   |   yes    |
    ///  sf::Cursor::SizeVertical           |  yes  |    yes   |   yes    |
    ///  sf::Cursor::SizeTopLeftBottomRight |  no   |    yes*  |   yes    |
    ///  sf::Cursor::SizeBottomLeftTopRight |  no   |    yes*  |   yes    |
    ///  sf::Cursor::SizeLeft               |  yes  |    yes** |   yes**  |
    ///  sf::Cursor::SizeRight              |  yes  |    yes** |   yes**  |
    ///  sf::Cursor::SizeTop                |  yes  |    yes** |   yes**  |
    ///  sf::Cursor::SizeBottom             |  yes  |    yes** |   yes**  |
    ///  sf::Cursor::SizeTopLeft            |  yes  |    yes** |   yes**  |
    ///  sf::Cursor::SizeTopRight           |  yes  |    yes** |   yes**  |
    ///  sf::Cursor::SizeBottomLeft         |  yes  |    yes** |   yes**  |
    ///  sf::Cursor::SizeBottomRight        |  yes  |    yes** |   yes**  |
    ///  sf::Cursor::SizeAll                |  yes  |    no    |   yes    |
    ///  sf::Cursor::Cross                  |  yes  |    yes   |   yes    |
    ///  sf::Cursor::Help                   |  yes  |    yes*  |   yes    |
    ///  sf::Cursor::NotAllowed             |  yes  |    yes   |   yes    |
    ///
    ///  * These cursor types are undocumented so may not
    ///    be available on all versions, but have been tested on 10.13
    ///
    ///  ** On Windows and macOS, double-headed arrows are used
    ///
    ////////////////////////////////////////////////////////////
    enum Type
    {
        Arrow,                  //!< Arrow cursor (default)
        ArrowWait,              //!< Busy arrow cursor
        Wait,                   //!< Busy cursor
        Text,                   //!< I-beam, cursor when hovering over a field allowing text entry
        Hand,                   //!< Pointing hand cursor
        SizeHorizontal,         //!< Horizontal double arrow cursor
        SizeVertical,           //!< Vertical double arrow cursor
        SizeTopLeftBottomRight, //!< Double arrow cursor going from top-left to bottom-right
        SizeBottomLeftTopRight, //!< Double arrow cursor going from bottom-left to top-right
        SizeLeft,               //!< Left arrow cursor on Linux, same as SizeHorizontal on other platforms
        SizeRight,              //!< Right arrow cursor on Linux, same as SizeHorizontal on other platforms
        SizeTop,                //!< Up arrow cursor on Linux, same as SizeVertical on other platforms
        SizeBottom,             //!< Down arrow cursor on Linux, same as SizeVertical on other platforms
        SizeTopLeft,            //!< Top-left arrow cursor on Linux, same as SizeTopLeftBottomRight on other platforms
        SizeBottomRight, //!< Bottom-right arrow cursor on Linux, same as SizeTopLeftBottomRight on other platforms
        SizeBottomLeft,  //!< Bottom-left arrow cursor on Linux, same as SizeBottomLeftTopRight on other platforms
        SizeTopRight,    //!< Top-right arrow cursor on Linux, same as SizeBottomLeftTopRight on other platforms
        SizeAll,         //!< Combination of SizeHorizontal and SizeVertical
        Cross,           //!< Crosshair cursor
        Help,            //!< Help cursor
        NotAllowed       //!< Action not allowed cursor
    };

    ////////////////////////////////////////////////////////////
    /// \brief Default constructor
    ///
    /// This constructor doesn't actually create the cursor;
    /// initially the new instance is invalid and must not be
    /// used until either loadFromPixels() or loadFromSystem()
    /// is called and successfully created a cursor.
    ///
    ////////////////////////////////////////////////////////////
    Cursor();

    ////////////////////////////////////////////////////////////
    /// \brief Destructor
    ///
    /// This destructor releases the system resources
    /// associated with this cursor, if any.
    ///
    ////////////////////////////////////////////////////////////
    ~Cursor();

    ////////////////////////////////////////////////////////////
    /// \brief Deleted copy constructor
    ///
    ////////////////////////////////////////////////////////////
    Cursor(const Cursor&) = delete;

    ////////////////////////////////////////////////////////////
    /// \brief Deleted copy assignment
    ///
    ////////////////////////////////////////////////////////////
    Cursor& operator=(const Cursor&) = delete;

    ////////////////////////////////////////////////////////////
    /// \brief Create a cursor with the provided image
    ///
    /// \a pixels must be an array of \a width by \a height pixels
    /// in 32-bit RGBA format. If not, this will cause undefined behavior.
    ///
    /// If \a pixels is null or either \a width or \a height are 0,
    /// the current cursor is left unchanged and the function will
    /// return false.
    ///
    /// In addition to specifying the pixel data, you can also
    /// specify the location of the hotspot of the cursor. The
    /// hotspot is the pixel coordinate within the cursor image
    /// which will be located exactly where the mouse pointer
    /// position is. Any mouse actions that are performed will
    /// return the window/screen location of the hotspot.
    ///
    /// \warning On Unix platforms which do not support colored
    ///          cursors, the pixels are mapped into a monochrome
    ///          bitmap: pixels with an alpha channel to 0 are
    ///          transparent, black if the RGB channel are close
    ///          to zero, and white otherwise.
    ///
    /// \param pixels   Array of pixels of the image
    /// \param size     Width and height of the image
    /// \param hotspot  (x,y) location of the hotspot
    /// \return true if the cursor was successfully loaded;
    ///         false otherwise
    ///
    ////////////////////////////////////////////////////////////
    [[nodiscard]] bool loadFromPixels(const std::uint8_t* pixels, Vector2u size, Vector2u hotspot);

    ////////////////////////////////////////////////////////////
    /// \brief Create a native system cursor
    ///
    /// Refer to the list of cursor available on each system
    /// (see sf::Cursor::Type) to know whether a given cursor is
    /// expected to load successfully or is not supported by
    /// the operating system.
    ///
    /// \param type Native system cursor type
    /// \return true if and only if the corresponding cursor is
    ///         natively supported by the operating system;
    ///         false otherwise
    ///
    ////////////////////////////////////////////////////////////
    [[nodiscard]] bool loadFromSystem(Type type);

private:
    friend class WindowBase;

    ////////////////////////////////////////////////////////////
    /// \brief Get access to the underlying implementation
    ///
    /// This is primarily designed for sf::Window::setMouseCursor,
    /// hence the friendship.
    ///
    /// \return a reference to the OS-specific implementation
    ///
    ////////////////////////////////////////////////////////////
    const priv::CursorImpl& getImpl() const;

    ////////////////////////////////////////////////////////////
    // Member data
    ////////////////////////////////////////////////////////////
    std::unique_ptr<priv::CursorImpl> m_impl; //!< Platform-specific implementation of the cursor
};

} // namespace sf


////////////////////////////////////////////////////////////
/// \class sf::Cursor
/// \ingroup window
///
/// \warning Features related to Cursor are not supported on
///          iOS and Android.
///
/// This class abstracts the operating system resources
/// associated with either a native system cursor or a custom
/// cursor.
///
/// After loading the cursor the graphical appearance
/// with either loadFromPixels() or loadFromSystem(), the
/// cursor can be changed with sf::Window::setMouseCursor().
///
/// The behaviour is undefined if the cursor is destroyed while
/// in use by the window.
///
/// Usage example:
/// \code
/// sf::Window window;
///
/// // ... create window as usual ...
///
/// sf::Cursor cursor;
/// if (cursor.loadFromSystem(sf::Cursor::Hand))
///     window.setMouseCursor(cursor);
/// \endcode
///
/// \see sf::Window::setMouseCursor
///
////////////////////////////////////////////////////////////
