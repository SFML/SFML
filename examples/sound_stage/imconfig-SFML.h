// Add this to your imconfig.h

#include <SFML/Graphics/Color.hpp>
#include <SFML/System/Vector2.hpp>

#include <cstdint>

#include "imgui-SFML_export.h"

#define IM_VEC2_CLASS_EXTRA           \
    template<typename T>              \
    ImVec2(const sf::Vector2<T>& v) { \
        x = static_cast<float>(v.x);  \
        y = static_cast<float>(v.y);  \
    }                                 \
                                      \
    template<typename T>              \
    operator sf::Vector2<T>() const { \
        return sf::Vector2<T>(x, y);  \
    }

#define IM_VEC4_CLASS_EXTRA                                                                        \
    ImVec4(const sf::Color& c) : x(c.r / 255.f), y(c.g / 255.f), z(c.b / 255.f), w(c.a / 255.f) {} \
    operator sf::Color() const {                                                                   \
        return sf::Color(static_cast<std::uint8_t>(x * 255.f),                                     \
                         static_cast<std::uint8_t>(y * 255.f),                                     \
                         static_cast<std::uint8_t>(z * 255.f),                                     \
                         static_cast<std::uint8_t>(w * 255.f));                                    \
    }
