#include <SFML/Graphics/BlendMode.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/Transform.hpp>

#include <GraphicsUtil.hpp>
#include <SystemUtil.hpp>
#include <ostream>

namespace sf
{
std::ostream& operator<<(std::ostream& os, const BlendMode& blendMode)
{
    return os << "( " << blendMode.colorSrcFactor << ", " << blendMode.colorDstFactor << ", " << blendMode.colorEquation
              << ", " << blendMode.alphaSrcFactor << ", " << blendMode.alphaDstFactor << ", " << blendMode.alphaEquation
              << " )";
}

std::ostream& operator<<(std::ostream& os, const Color& color)
{
    return os << "0x" << std::hex << color.toInteger() << std::dec << " (r=" << int{color.r} << ", g=" << int{color.g}
              << ", b=" << int{color.b} << ", a=" << int{color.a} << ")";
}

std::ostream& operator<<(std::ostream& os, const Transform& transform)
{
    const auto& matrix = transform.getMatrix();
    os << matrix[0] << ", " << matrix[4] << ", " << matrix[12] << ", ";
    os << matrix[1] << ", " << matrix[5] << ", " << matrix[13] << ", ";
    os << matrix[3] << ", " << matrix[7] << ", " << matrix[15];
    return os;
}

template <typename T>
std::ostream& operator<<(std::ostream& os, const Rect<T>& rect)
{
    const auto flags = os.flags();
    setStreamPrecision(os, std::numeric_limits<T>::max_digits10);
    os << "(left=" << rect.left << ", top=" << rect.top << ", width=" << rect.width << ", height=" << rect.height << ")";
    os.flags(flags);
    return os;
}

template std::ostream& operator<<(std::ostream&, const Rect<int>&);
template std::ostream& operator<<(std::ostream&, const Rect<float>&);
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

template <>
std::ostream& operator<<(std::ostream& os, const Approx<sf::Transform>& approx)
{
    return os << approx.value;
}
