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

#ifndef SFML_CONVERTEVENT_H
#define SFML_CONVERTEVENT_H

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Event.h>


////////////////////////////////////////////////////////////
// Define a function to convert a sf::Event ot a sfEvent
////////////////////////////////////////////////////////////
inline void ConvertEvent(const sf::Event& SFMLEvent, sfEvent* event)
{
    // Convert its type
    event->Type = static_cast<sfEventType>(SFMLEvent.Type);

    // Fill its fields
    switch (event->Type)
    {
        case sfEvtResized :
            event->Size.Width  = SFMLEvent.Size.Width;
            event->Size.Height = SFMLEvent.Size.Height;
            break;

        case sfEvtTextEntered :
            event->Text.Unicode = SFMLEvent.Text.Unicode;
            break;

        case sfEvtKeyReleased :
        case sfEvtKeyPressed :
            event->Key.Code    = static_cast<sfKeyCode>(SFMLEvent.Key.Code);
            event->Key.Alt     = SFMLEvent.Key.Alt     ? sfTrue : sfFalse;
            event->Key.Control = SFMLEvent.Key.Control ? sfTrue : sfFalse;
            event->Key.Shift   = SFMLEvent.Key.Shift   ? sfTrue : sfFalse;
            break;

        case sfEvtMouseWheelMoved :
            event->MouseWheel.Delta = SFMLEvent.MouseWheel.Delta;
            event->MouseWheel.X     = SFMLEvent.MouseWheel.X;
            event->MouseWheel.Y     = SFMLEvent.MouseWheel.Y;
            break;

        case sfEvtMouseButtonPressed :
        case sfEvtMouseButtonReleased :
            event->MouseButton.Button = static_cast<sfMouseButton>(SFMLEvent.MouseButton.Button);
            event->MouseButton.X      = SFMLEvent.MouseButton.X;
            event->MouseButton.Y      = SFMLEvent.MouseButton.Y;
            break;

        case sfEvtMouseMoved :
            event->MouseMove.X = SFMLEvent.MouseMove.X;
            event->MouseMove.Y = SFMLEvent.MouseMove.Y;
            break;

        case sfEvtJoyButtonPressed :
        case sfEvtJoyButtonReleased :
            event->JoyButton.JoystickId = SFMLEvent.JoyButton.JoystickId;
            event->JoyButton.Button     = SFMLEvent.JoyButton.Button;
            break;

        case sfEvtJoyMoved :
            event->JoyMove.JoystickId = SFMLEvent.JoyMove.JoystickId;
            event->JoyMove.Axis       = static_cast<sfJoyAxis>(SFMLEvent.JoyMove.Axis);
            event->JoyMove.Position   = SFMLEvent.JoyMove.Position;
            break;

        default :
            break;
    }
}

#endif // SFML_CONVERTEVENT_H
