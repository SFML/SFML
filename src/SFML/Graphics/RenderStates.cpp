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

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Graphics/RenderStates.hpp>
#include <cstddef>


namespace sf
{
////////////////////////////////////////////////////////////
const RenderStates RenderStates::Default;


////////////////////////////////////////////////////////////
RenderStates::RenderStates() :
BlendMode(BlendAlpha),
Transform(),
Texture  (NULL),
Shader   (NULL)
{
}


////////////////////////////////////////////////////////////
RenderStates::RenderStates(const sf::Transform& transform) :
BlendMode(BlendAlpha),
Transform(transform),
Texture  (NULL),
Shader   (NULL)
{
}


////////////////////////////////////////////////////////////
RenderStates::RenderStates(sf::BlendMode blendMode) :
BlendMode(blendMode),
Transform(),
Texture  (NULL),
Shader   (NULL)
{
}


////////////////////////////////////////////////////////////
RenderStates::RenderStates(const sf::Texture* texture) :
BlendMode(BlendAlpha),
Transform(),
Texture  (texture),
Shader   (NULL)
{
}


////////////////////////////////////////////////////////////
RenderStates::RenderStates(const sf::Shader* shader) :
BlendMode(BlendAlpha),
Transform(),
Texture  (NULL),
Shader   (shader)
{
}


////////////////////////////////////////////////////////////
RenderStates::RenderStates(sf::BlendMode blendMode, const sf::Transform& transform,
                           const sf::Texture* texture, const sf::Shader* shader) :
BlendMode(blendMode),
Transform(transform),
Texture  (texture),
Shader   (shader)
{
}

} // namespace sf
