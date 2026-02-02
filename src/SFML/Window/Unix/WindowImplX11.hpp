////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007-2025 Laurent Gomila (laurent@sfml-dev.org)
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
#include <SFML/Window/Event.hpp>
#include <SFML/Window/WindowEnums.hpp> // Prevent conflict with macro None from Xlib
#include <SFML/Window/WindowImpl.hpp>

#include <X11/Xlib.h>
#include <X11/extensions/Xrandr.h>

#include <memory>


namespace sf::priv
{
////////////////////////////////////////////////////////////
class WindowImplX11 : public WindowImpl
{
public:
    ////////////////////////////////////////////////////////////
    explicit WindowImplX11(WindowHandle handle);

    ////////////////////////////////////////////////////////////
    WindowImplX11(VideoMode mode, const String& title, std::uint32_t style, State state, const ContextSettings& settings);

    ////////////////////////////////////////////////////////////
    ~WindowImplX11() override;

    ////////////////////////////////////////////////////////////
    [[nodiscard]] WindowHandle getNativeHandle() const override;

    ////////////////////////////////////////////////////////////
    [[nodiscard]] Vector2i getPosition() const override;

    ////////////////////////////////////////////////////////////
    void setPosition(Vector2i position) override;

    ////////////////////////////////////////////////////////////
    [[nodiscard]] Vector2u getSize() const override;

    ////////////////////////////////////////////////////////////
    void setSize(Vector2u size) override;

    ////////////////////////////////////////////////////////////
    void setMinimumSize(const std::optional<Vector2u>& minimumSize) override;

    ////////////////////////////////////////////////////////////
    void setMaximumSize(const std::optional<Vector2u>& maximumSize) override;

    ////////////////////////////////////////////////////////////
    void setTitle(const String& title) override;

    ////////////////////////////////////////////////////////////
    void setIcon(Vector2u size, const std::uint8_t* pixels) override;

    ////////////////////////////////////////////////////////////
    void setVisible(bool visible) override;

    ////////////////////////////////////////////////////////////
    void setMouseCursorVisible(bool visible) override;

    ////////////////////////////////////////////////////////////
    void setMouseCursorGrabbed(bool grabbed) override;

    ////////////////////////////////////////////////////////////
    void setMouseCursor(const CursorImpl& cursor) override;

    ////////////////////////////////////////////////////////////
    void setKeyRepeatEnabled(bool enabled) override;

    ////////////////////////////////////////////////////////////
    void requestFocus() override;

    ////////////////////////////////////////////////////////////
    /// \brief Enable or disable file dropping.
    ///
    /// \param enabled True to enable, false to disable
    ///
    ////////////////////////////////////////////////////////////
    void setFileDroppingEnabled(bool enabled = true) override;

    ////////////////////////////////////////////////////////////
    /// \brief Check whether the window has the input focus
    ///
    /// \return `true` if window has focus, `false` otherwise
    ///
    ////////////////////////////////////////////////////////////
    [[nodiscard]] bool hasFocus() const override;

protected:
    ////////////////////////////////////////////////////////////
    void processEvents() override;

    ////////////////////////////////////////////////////////////
    /// \brief Check if the given file type can be accepted
    ///
    /// \param fileType The file type to check
    ///
    /// \return If the file type is acceptable
    ///
    ////////////////////////////////////////////////////////////
    bool canAcceptFileType(const Atom& fileType);

private:
    ////////////////////////////////////////////////////////////
    /// \brief Request the WM to make the current window active
    ///
    ////////////////////////////////////////////////////////////
    void grabFocus();

    ////////////////////////////////////////////////////////////
    void setVideoMode(const VideoMode& mode);

    ////////////////////////////////////////////////////////////
    /// \brief Reset to desktop video mode
    ///
    ////////////////////////////////////////////////////////////
    void resetVideoMode();

    ////////////////////////////////////////////////////////////
    void switchToFullscreen();

    ////////////////////////////////////////////////////////////
    /// \brief Set the WM protocols we support
    ///
    ////////////////////////////////////////////////////////////
    void setProtocols();

    ////////////////////////////////////////////////////////////
    /// \brief Update the last time we received user input
    ///
    /// \param time Last time we received user input
    ///
    ////////////////////////////////////////////////////////////
    void updateLastInputTime(::Time time);

    ////////////////////////////////////////////////////////////
    void initialize();

    ////////////////////////////////////////////////////////////
    void createHiddenCursor();

    ////////////////////////////////////////////////////////////
    void cleanup();

    ////////////////////////////////////////////////////////////
    bool processEvent(XEvent& windowEvent);

    ////////////////////////////////////////////////////////////
    /// \brief Check if a valid version of XRandR extension is present
    ///
    /// \return `true` if a valid XRandR version found, `false` otherwise
    ///
    ////////////////////////////////////////////////////////////
    bool checkXRandR();

    ////////////////////////////////////////////////////////////
    /// \brief Get the RROutput of the primary monitor
    ///
    /// \param rootWindow the root window
    /// \param res screen resources
    ///
    /// \return RROutput of the primary monitor
    ///
    ////////////////////////////////////////////////////////////
    RROutput getOutputPrimary(::Window& rootWindow, XRRScreenResources* res);

    ////////////////////////////////////////////////////////////
    /// \brief Get coordinates of the primary monitor
    ///
    /// \return Position of the primary monitor
    ///
    ////////////////////////////////////////////////////////////
    Vector2i getPrimaryMonitorPosition();

    ////////////////////////////////////////////////////////////
    /// \brief Set min/max window size
    ///
    ////////////////////////////////////////////////////////////
    void setWindowSizeConstraints() const;

    ////////////////////////////////////////////////////////////
    // Member data
    ////////////////////////////////////////////////////////////
    ::Window                 m_window{};       ///< X identifier defining our window
    std::shared_ptr<Display> m_display;        ///< Pointer to the display
    int                      m_screen;         ///< Screen identifier
    std::shared_ptr<_XIM>    m_inputMethod;    ///< Input method linked to the X display
    XIC                      m_inputContext{}; ///< Input context used to get unicode input in our window
    bool                     m_isExternal{};   ///< Tell whether the window has been created externally or by SFML
    RRMode                   m_oldVideoMode{}; ///< Video mode in use before we switch to fullscreen
    RRCrtc                   m_oldRRCrtc{};    ///< RRCrtc in use before we switch to fullscreen
    ::Cursor m_hiddenCursor{}; ///< As X11 doesn't provide cursor hiding, we must create a transparent one
    ::Cursor m_lastCursor{None}; ///< Last cursor used -- this data is not owned by the window and is required to be always valid
    bool m_keyRepeat{true}; ///< Is the KeyRepeat feature enabled?
    Vector2i m_previousSize{-1, -1}; ///< Previous size of the window, to find if a ConfigureNotify event is a resize event (could be a move event only)
    bool     m_useSizeHints{};   ///< Is the size of the window fixed with size hints?
    bool     m_fullscreen{};     ///< Is the window in fullscreen?
    bool     m_cursorGrabbed{};  ///< Is the mouse cursor trapped?
    bool     m_windowMapped{};   ///< Has the window been mapped by the window manager?
    Pixmap   m_iconPixmap{};     ///< The current icon pixmap if in use
    Pixmap   m_iconMaskPixmap{}; ///< The current icon mask pixmap if in use
    ::Time   m_lastInputTime{};  ///< Last time we received user input
    ::Window m_dropSource{};     ///< The window which is giving the dropped item
    Atom m_acceptedFileType{}; ///< The MIME type that the other window supports that we also support for file dropping
};

} // namespace sf::priv
