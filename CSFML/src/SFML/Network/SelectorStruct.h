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

#ifndef SFML_SELECTORSTRUCT_H
#define SFML_SELECTORSTRUCT_H

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Network/Selector.hpp>
#include <SFML/Network/SocketTCPStruct.h>
#include <SFML/Network/SocketUDPStruct.h>
#include <map>


////////////////////////////////////////////////////////////
// Internal structure of SelectorTCP
////////////////////////////////////////////////////////////
struct sfSelectorTCP
{
    sf::SelectorTCP                       This;
    std::map<sf::SocketTCP, sfSocketTCP*> Sockets;
};


////////////////////////////////////////////////////////////
// Internal structure of sfSelectorUDP
////////////////////////////////////////////////////////////
struct sfSelectorUDP
{
    sf::SelectorUDP                       This;
    std::map<sf::SocketUDP, sfSocketUDP*> Sockets;
};


#endif // SFML_SELECTORSTRUCT_H
