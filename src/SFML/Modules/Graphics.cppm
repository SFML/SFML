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
// Module preamble
////////////////////////////////////////////////////////////

module;

#include <SFML/Graphics.hpp>

export module sfml:Graphics;

export import :Window;

export namespace sf {
    using sf::BlendMode;
    using sf::CircleShape;
    using sf::Color;
    using sf::ConvexShape;
    using sf::CoordinateType;
    using sf::Drawable;
    using sf::Font;
    namespace Glsl {
        using sf::Glsl::Vec2;
        using sf::Glsl::Ivec2;
        using sf::Glsl::Bvec2;
        using sf::Glsl::Vec3;
        using sf::Glsl::Ivec3;
        using sf::Glsl::Bvec3;
        using sf::Glsl::Vec4;
        using sf::Glsl::Ivec4;
        using sf::Glsl::Bvec4;
        using sf::Glsl::Mat3;
        using sf::Glsl::Mat4;
    }
    using sf::Glyph;
    using sf::Image;
    using sf::PrimitiveType;
    using sf::Rect;
    using sf::IntRect;
    using sf::FloatRect;
    using sf::RectangleShape;
    using sf::RenderStates;
    using sf::RenderTarget;
    using sf::RenderTexture;
    using sf::RenderWindow;
    using sf::Shader;
    using sf::Shape;
    using sf::Sprite;
    using sf::StencilMode;
    using sf::StencilValue;
    using sf::StencilComparison;
    using sf::StencilUpdateOperation;
    using sf::Text;
    using sf::Texture;
    using sf::swap;
    using sf::Transform;
    using sf::Transformable;
    using sf::Vertex;
    using sf::VertexArray;
    using sf::VertexBuffer;
    using sf::View;
}
