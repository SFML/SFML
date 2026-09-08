////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007-2026 Laurent Gomila (laurent@sfml-dev.org)
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
#include <SFML/Window/Joystick.hpp>

#import <GameController/GameController.h>
#include <array>
#include <mutex>

#include <cstdint>


namespace sf::priv
{
////////////////////////////////////////////////////////////
class JoystickRegistryImpl
{
public:
    ////////////////////////////////////////////////////////////
    void initialize();

    ////////////////////////////////////////////////////////////
    void cleanup();

    ////////////////////////////////////////////////////////////
    void refresh();

    ////////////////////////////////////////////////////////////
    [[nodiscard]] bool isConnected(unsigned int index);

    ////////////////////////////////////////////////////////////
    [[nodiscard]] bool open(unsigned int index, std::uint64_t& generation);

    ////////////////////////////////////////////////////////////
    [[nodiscard]] GCController* getController(unsigned int index, std::uint64_t generation);

private:
    ////////////////////////////////////////////////////////////
    struct Slot
    {
        GCController* controller{};
        std::uint64_t generation{};
    };

    ////////////////////////////////////////////////////////////
    void connect(GCController* controller);

    ////////////////////////////////////////////////////////////
    void disconnect(GCController* controller);

    ////////////////////////////////////////////////////////////
    [[nodiscard]] bool assignController(GCController* controller);

    ////////////////////////////////////////////////////////////
    // Member data
    ////////////////////////////////////////////////////////////
    std::mutex                        m_mutex;
    std::array<Slot, Joystick::Count> m_slots{};
    id                                m_connectObserver{};
    id                                m_disconnectObserver{};
    std::uint64_t                     m_revision{};
    bool                              m_initialized{};
};

} // namespace sf::priv
