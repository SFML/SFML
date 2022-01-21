// Note: No need to increase compile time by including TestUtilities/GraphicsUtil.hpp
#include <SFML/Graphics/BlendMode.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Transform.hpp>

#include <doctest.h> // doctest::Approx

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

    namespace Testing
    {
        bool almostEqual(const sf::Transform& lhs, const sf::Transform& rhs)
        {
            const auto& a = lhs.getMatrix();
            const auto& b = rhs.getMatrix();

            for (std::size_t i : { 0, 1, 3, 4, 5, 7, 12, 13, 15 })
                if (a[i] != doctest::Approx(b[i])) // default epsilon
                    return false;

            return true;
        }
    }
}
