////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007-2022 Laurent Gomila (laurent@sfml-dev.org)
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
#include <SFML/Graphics/Export.hpp>

namespace sf
{
class SpriteBatch;

////////////////////////////////////////////////////////////
/// \brief Abstract base class for objects that can be batched
///        to a sf::SpriteBatch
///
////////////////////////////////////////////////////////////
class SFML_GRAPHICS_API Batchable
{
public:
    ////////////////////////////////////////////////////////////
    /// \brief Virtual destructor
    ///
    ////////////////////////////////////////////////////////////
    virtual ~Batchable() = default;

    ////////////////////////////////////////////////////////////
    /// \brief Batch the object using the given SpriteBatch
    ///
    /// This is a pure virtual function that has to be implemented
    /// by the derived class to define how the drawable should be
    /// batched.
    ///
    /// \param spriteBatch The SpriteBatch to use
    /// \param depth       The depth at which to render the object
    ///
    ////////////////////////////////////////////////////////////
    virtual void batch(SpriteBatch& spriteBatch, float depth = 0.f) const = 0;
};

} // namespace sf


////////////////////////////////////////////////////////////
/// \class sf::Batchable
/// \ingroup graphics
///
/// sf::Batchable is an interface for drawables that can be
/// batched using an sf::SpriteBatch. To implement the interface,
/// you need to define a batch method that makes use of
/// sf::SpriteBatch::batch(), and supplies it the vertices, texture,
/// transform and other data for drawing.
///
////////////////////////////////////////////////////////////
