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

#ifndef SFML_VIEWSTRUCT_H
#define SFML_VIEWSTRUCT_H

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Graphics/View.hpp>


////////////////////////////////////////////////////////////
// Internal structure of sfMusic
////////////////////////////////////////////////////////////
struct sfView
{
    sfView() :
        This(new sf::View),
        OwnInstance(true)
    {
    }

    sfView(sfFloatRect Rect) :
        This(new sf::View(sf::FloatRect(Rect.Left, Rect.Top, Rect.Right, Rect.Bottom))),
        OwnInstance(true)
    {
    }

    sfView(sf::View* View) :
        This(View),
        OwnInstance(false)
    {
    }

    sfView(const sfView& View) :
        This(View.This ? new sf::View(*View.This) : NULL),
        OwnInstance(true)
    {
    }

    ~sfView()
    {
        if (OwnInstance)
            delete This;
    }

    sf::View* This; // pointer needed for RenderWindow
    bool OwnInstance;
};


#endif // SFML_VIEWSTRUCT_H
