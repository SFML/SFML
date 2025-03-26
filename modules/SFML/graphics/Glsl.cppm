/**
 * @file Glsl.cppm
 * @brief Module file for the Glsl namespace of the Graphics module
 */

module;

#include <SFML/Graphics/Glsl.hpp>

export module sfml.graphics.Glsl;

/**
 * @namespace sf
 * @brief The SFML sf:: namespace 
 */
export namespace sf {
    /**
     * @namespace Glsl
     * @brief The SFML sf::Glsl:: namespace 
     */
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
}