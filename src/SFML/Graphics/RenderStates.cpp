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

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Graphics/RenderStates.hpp>


namespace sf
{
////////////////////////////////////////////////////////////
// We cannot use the default constructor here, because it accesses BlendAlpha, which is also global (and dynamically
// initialized). Initialization order of global objects in different translation units is not defined.
const RenderStates RenderStates::Default(BlendMode(
    BlendMode::Factor::SrcAlpha,
    BlendMode::Factor::OneMinusSrcAlpha,
    BlendMode::Equation::Add,
    BlendMode::Factor::One,
    BlendMode::Factor::OneMinusSrcAlpha,
    BlendMode::Equation::Add));


////////////////////////////////////////////////////////////
RenderStates::RenderStates(const Transform& theTransform) : transform(theTransform)
{
}


////////////////////////////////////////////////////////////
RenderStates::RenderStates(const BlendMode& theBlendMode) : blendMode(theBlendMode)
{
}


////////////////////////////////////////////////////////////
RenderStates::RenderStates(const StencilMode& theStencilMode) : stencilMode(theStencilMode)
{
}


////////////////////////////////////////////////////////////
RenderStates::RenderStates(const Texture* theTexture) : texture(theTexture)
{
}


////////////////////////////////////////////////////////////
RenderStates::RenderStates(const Shader* theShader) : shader(theShader)
{
}


////////////////////////////////////////////////////////////
RenderStates::RenderStates(const BlendMode&   theBlendMode,
                           const StencilMode& theStencilMode,
                           const Transform&   theTransform,
                           CoordinateType     theCoordinateType,
                           const Texture*     theTexture,
                           const Shader*      theShader) :
blendMode(theBlendMode),
stencilMode(theStencilMode),
transform(theTransform),
coordinateType(theCoordinateType),
texture(theTexture),
shader(theShader)
{
}

} // namespace sf
