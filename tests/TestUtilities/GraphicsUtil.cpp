// Note: No need to increase compile time by including TestUtilities/Graphics.hpp
#include <SFML/Graphics/BlendMode.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Transform.hpp>

#include <ostream>

namespace sf
{
    std::ostream& operator <<(std::ostream& os, const sf::BlendMode& blendMode)
    {
        os << "( " << blendMode.colorSrcFactor << ", " << blendMode.colorDstFactor << ", "
           << blendMode.colorEquation << ", " << blendMode.alphaSrcFactor << ", "
           << blendMode.alphaDstFactor << ", " << blendMode.alphaEquation << " )";

        return os;
    }

    std::ostream& operator <<(std::ostream& os, const sf::Color& color)
    {
        os << "0x" << std::hex << color.toInteger() << std::dec
           << " (r=" << static_cast<int>(color.r)
           << ", g=" << static_cast<int>(color.g)
           << ", b=" << static_cast<int>(color.b)
           << ", a=" << static_cast<int>(color.a) << ")";

        return os;
    }

    std::ostream& operator <<(std::ostream& os, const sf::Transform& transform)
    {
        const auto& matrix = transform.getMatrix();
        os << matrix[0] << ", " << matrix[4] << ", " << matrix[12] << ", ";
        os << matrix[1] << ", " << matrix[5] << ", " << matrix[13] << ", ";
        os << matrix[3] << ", " << matrix[7] << ", " << matrix[15];

        return os;
    }
}
