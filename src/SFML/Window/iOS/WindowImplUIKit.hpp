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
#include <SFML/Window/WindowImpl.hpp>
#include <SFML/Window/iOS/ObjCType.hpp>


SFML_DECLARE_OBJC_CLASS(UIWindow);
SFML_DECLARE_OBJC_CLASS(SFView);
SFML_DECLARE_OBJC_CLASS(SFViewController);


namespace sf::priv
{
////////////////////////////////////////////////////////////
class WindowImplUIKit : public WindowImpl
{
public:
    ////////////////////////////////////////////////////////////
    WindowImplUIKit(WindowHandle handle);

    ////////////////////////////////////////////////////////////
    WindowImplUIKit(VideoMode mode, const String& title, std::uint32_t style, State state, const ContextSettings& settings);

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
    [[nodiscard]] bool hasFocus() const override;

    ////////////////////////////////////////////////////////////
    /// \brief Notify an event
    ///
    /// \param event Event to forward
    ///
    ////////////////////////////////////////////////////////////
    void forwardEvent(const Event& event);

    ////////////////////////////////////////////////////////////
    /// \brief Get the window's view
    ///
    /// \return Pointer to the window's view
    ///
    ////////////////////////////////////////////////////////////
    [[nodiscard]] SFView* getGlView() const;

    ////////////////////////////////////////////////////////////
    void setVirtualKeyboardVisible(bool visible);

protected:
    ////////////////////////////////////////////////////////////
    void processEvents() override;

private:
    ////////////////////////////////////////////////////////////
    // Member data
    ////////////////////////////////////////////////////////////
    UIWindow*         m_window{};         ///< Pointer to the internal UIKit window
    SFView*           m_view{};           ///< OpenGL view of the window
    SFViewController* m_viewController{}; ///< Controller attached to the view
    bool              m_hasFocus{};       ///< Current focus state of the window
    float             m_backingScale{};   ///< Converts from points to pixels and vice versa
};

} // namespace sf::priv
