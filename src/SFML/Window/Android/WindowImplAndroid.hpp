////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2013 Jonathan De Wachter (dewachter.jonathan@gmail.com)
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
#include <SFML/Window/EglContext.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/WindowImpl.hpp>

#include <SFML/System/Android/Activity.hpp>

#include <android/input.h>


namespace sf::priv
{
////////////////////////////////////////////////////////////
class WindowImplAndroid : public WindowImpl
{
public:
    ////////////////////////////////////////////////////////////
    WindowImplAndroid(WindowHandle handle);

    ////////////////////////////////////////////////////////////
    WindowImplAndroid(VideoMode mode, const String& title, std::uint32_t style, State state, const ContextSettings& settings);

    ////////////////////////////////////////////////////////////
    ~WindowImplAndroid() override;

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

    static void               forwardEvent(const Event& event);
    static WindowImplAndroid* singleInstance;

protected:
    ////////////////////////////////////////////////////////////
    void processEvents() override;

private:
    ////////////////////////////////////////////////////////////
    /// \brief Process messages from the looper associated with the main thread
    ///
    /// \param fd     File descriptor
    /// \param events Bitmask of the poll events that were triggered
    /// \param data   Data pointer supplied
    ///
    /// \return Whether it should continue (1) or unregister the callback (0)
    ///
    ////////////////////////////////////////////////////////////
    static int processEvent(int fd, int events, void* data);

    static int processScrollEvent(AInputEvent* inputEvent, ActivityStates& states);
    static int processKeyEvent(AInputEvent* inputEvent, ActivityStates& states);
    static int processMotionEvent(AInputEvent* inputEvent, ActivityStates& states);
    static int processPointerEvent(bool isDown, AInputEvent* event, ActivityStates& states);

    ////////////////////////////////////////////////////////////
    /// \brief Convert a Android key to SFML key code
    ///
    /// \param symbol Android key to convert
    ///
    /// \return Corresponding SFML key code
    ///
    ////////////////////////////////////////////////////////////
    static Keyboard::Key androidKeyToSF(std::int32_t key);

    ////////////////////////////////////////////////////////////
    /// \brief Convert a Android scan code to SFML key code
    ///
    /// Scan code mappings on android are not reliable, and can
    /// also be customised by users in their own projects. For
    /// this method we match the provided generic layout
    ///
    /// \see https://source.android.com/docs/core/interaction/input/key-layout-files
    ///
    /// \param symbol Android scan code to convert
    ///
    /// \return Corresponding SFML scan code
    ///
    ////////////////////////////////////////////////////////////
    static Keyboard::Scancode androidScanToSF(std::int32_t key);

    ////////////////////////////////////////////////////////////
    /// \brief Get Unicode decoded from the input event
    ///
    /// \param Event Input event
    ///
    /// \return The Unicode value
    ///
    ////////////////////////////////////////////////////////////
    static char32_t getUnicode(AInputEvent* event);

    Vector2u m_size;
    bool     m_windowBeingCreated{};
    bool     m_windowBeingDestroyed{};
    bool     m_hasFocus{true};
};

} // namespace sf::priv
