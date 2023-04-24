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
#include <SFML/Window/WindowStyle.hpp> // Prevent conflict with macro None from Xlib

#include <SFML/System/String.hpp>

#include <X11/Xlib.h>

#include <deque>


namespace sf::priv
{

////////////////////////////////////////////////////////////
/// \brief Give access to the system clipboard
///
////////////////////////////////////////////////////////////
class ClipboardImpl
{
public:
    ////////////////////////////////////////////////////////////
    /// \brief Get the content of the clipboard as string data
    ///
    /// This function returns the content of the clipboard
    /// as a string. If the clipboard does not contain string
    /// it returns an empty sf::String object.
    ///
    /// \return Current content of the clipboard
    ///
    ////////////////////////////////////////////////////////////
    static String getString();

    ////////////////////////////////////////////////////////////
    /// \brief Set the content of the clipboard as string data
    ///
    /// This function sets the content of the clipboard as a
    /// string.
    ///
    /// \param text sf::String object containing the data to be sent
    /// to the clipboard
    ///
    ////////////////////////////////////////////////////////////
    static void setString(const String& text);

    ////////////////////////////////////////////////////////////
    /// \brief Process pending events for the hidden clipboard window
    ///
    /// This function has to be called as part of normal window
    /// event processing in order for our application to respond
    /// to selection requests from other applications.
    ///
    ////////////////////////////////////////////////////////////
    static void processEvents();

private:
    ////////////////////////////////////////////////////////////
    /// \brief Constructor
    ///
    ////////////////////////////////////////////////////////////
    ClipboardImpl();

    ////////////////////////////////////////////////////////////
    /// \brief Destructor
    ///
    ////////////////////////////////////////////////////////////
    ~ClipboardImpl();

    ////////////////////////////////////////////////////////////
    /// \brief Get singleton instance
    ///
    /// \return Singleton instance
    ///
    ////////////////////////////////////////////////////////////
    static ClipboardImpl& getInstance();

    ////////////////////////////////////////////////////////////
    /// \brief getString implementation
    ///
    /// \return Current content of the clipboard
    ///
    ////////////////////////////////////////////////////////////
    String getStringImpl();

    ////////////////////////////////////////////////////////////
    /// \brief setString implementation
    ///
    /// \param text sf::String object containing the data to be sent to the clipboard
    ///
    ////////////////////////////////////////////////////////////
    void setStringImpl(const String& text);

    ////////////////////////////////////////////////////////////
    /// \brief processEvents implementation
    ///
    ////////////////////////////////////////////////////////////
    void processEventsImpl();

    ////////////////////////////////////////////////////////////
    /// \brief Process an incoming event from the window
    ///
    /// \param windowEvent Event which has been received
    ///
    ////////////////////////////////////////////////////////////
    void processEvent(XEvent& windowEvent);

    ////////////////////////////////////////////////////////////
    // Member data
    ////////////////////////////////////////////////////////////
    ::Window           m_window{};           ///< X identifier defining our window
    ::Display*         m_display;            ///< Pointer to the display
    Atom               m_clipboard;          ///< X Atom identifying the CLIPBOARD selection
    Atom               m_targets;            ///< X Atom identifying TARGETS
    Atom               m_text;               ///< X Atom identifying TEXT
    Atom               m_utf8String;         ///< X Atom identifying UTF8_STRING
    Atom               m_targetProperty;     ///< X Atom identifying our destination window property
    String             m_clipboardContents;  ///< Our clipboard contents
    std::deque<XEvent> m_events;             ///< Queue we use to store pending events for this window
    bool               m_requestResponded{}; ///< Holds whether our selection request has been responded to or not
};

} // namespace sf::priv
