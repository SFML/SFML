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
blendMode(BlendAlpha),
transform(),
texture  (NULL),
shader   (NULL)
{
}


////////////////////////////////////////////////////////////
RenderStates::RenderStates(const Transform& transform) :
blendMode(BlendAlpha),
transform(transform),
texture  (NULL),
shader   (NULL)
{
}


////////////////////////////////////////////////////////////
RenderStates::RenderStates(BlendMode blendMode) :
blendMode(blendMode),
transform(),
texture  (NULL),
shader   (NULL)
{
}


////////////////////////////////////////////////////////////
RenderStates::RenderStates(const Texture* texture) :
blendMode(BlendAlpha),
transform(),
texture  (texture),
shader   (NULL)
{
}


////////////////////////////////////////////////////////////
RenderStates::RenderStates(const Shader* shader) :
blendMode(BlendAlpha),
transform(),
texture  (NULL),
shader   (shader)
{
}


////////////////////////////////////////////////////////////
RenderStates::RenderStates(BlendMode blendMode, const Transform& transform,
                           const Texture* texture, const Shader* shader) :
blendMode(blendMode),
transform(transform),
texture  (texture),
shader   (shader)
{
}

} // namespace sf
