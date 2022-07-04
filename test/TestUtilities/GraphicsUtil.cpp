#include <SFML/Graphics/BlendMode.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Transform.hpp>

#include <GraphicsUtil.hpp>
#include <ostream>

namespace sf
{
std::ostream& operator<<(std::ostream& os, const sf::BlendMode& blendMode)
{
    os << "( " << blendMode.colorSrcFactor << ", " << blendMode.colorDstFactor << ", " << blendMode.colorEquation << ", "
       << blendMode.alphaSrcFactor << ", " << blendMode.alphaDstFactor << ", " << blendMode.alphaEquation << " )";

    return os;
}

std::ostream& operator<<(std::ostream& os, const sf::Color& color)
{
    os << "0x" << std::hex << color.toInteger() << std::dec << " (r=" << static_cast<int>(color.r)
       << ", g=" << static_cast<int>(color.g) << ", b=" << static_cast<int>(color.b)
       << ", a=" << static_cast<int>(color.a) << ")";

    return os;
}

std::ostream& operator<<(std::ostream& os, const sf::Transform& transform)
{
    const auto& matrix = transform.getMatrix();
    os << matrix[0] << ", " << matrix[4] << ", " << matrix[12] << ", ";
    os << matrix[1] << ", " << matrix[5] << ", " << matrix[13] << ", ";
    os << matrix[3] << ", " << matrix[7] << ", " << matrix[15];

    return os;
}
} // namespace sf

bool operator==(const sf::Transform& lhs, const Approx<sf::Transform>& rhs)
{
    return lhs.getMatrix()[0] == Approx(rhs.value.getMatrix()[0]) &&
           lhs.getMatrix()[4] == Approx(rhs.value.getMatrix()[4]) &&
           lhs.getMatrix()[12] == Approx(rhs.value.getMatrix()[12]) &&
           lhs.getMatrix()[1] == Approx(rhs.value.getMatrix()[1]) &&
           lhs.getMatrix()[5] == Approx(rhs.value.getMatrix()[5]) &&
           lhs.getMatrix()[13] == Approx(rhs.value.getMatrix()[13]) &&
           lhs.getMatrix()[3] == Approx(rhs.value.getMatrix()[3]) &&
           lhs.getMatrix()[7] == Approx(rhs.value.getMatrix()[7]) &&
           lhs.getMatrix()[15] == Approx(rhs.value.getMatrix()[15]);
}
