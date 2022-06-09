#include <SFML/Graphics/Shape.hpp>
#include "GraphicsUtil.hpp"

#include <doctest.h>

class TriangleShape : public sf::Shape
{
public:
    explicit TriangleShape(const sf::Vector2f& size)
        : m_size(size)
    {
        update();
    }

    std::size_t getPointCount() const override
    {
        return 3;
    }

    sf::Vector2f getPoint(std::size_t index) const override
    {
        switch (index)
        {
            default:
            case 0: return sf::Vector2f(m_size.x / 2.f, 0.f);
            case 1: return sf::Vector2f(0.f, m_size.y);
            case 2: return sf::Vector2f(m_size.x, m_size.y);
        }
    }

private:
    sf::Vector2f m_size;
};

TEST_CASE("sf::Shape class - [graphics]")
{
    SUBCASE("Default constructor")
    {
        const TriangleShape triangleShape({0.f, 0.f});
        CHECK(triangleShape.getTexture() == nullptr);
        CHECK(triangleShape.getTextureRect() == sf::IntRect());
        CHECK(triangleShape.getFillColor() == sf::Color::White);
        CHECK(triangleShape.getOutlineColor() == sf::Color::White);
        CHECK(triangleShape.getOutlineThickness() == 0.f);
        CHECK(triangleShape.getLocalBounds() == sf::FloatRect());
        CHECK(triangleShape.getGlobalBounds() == sf::FloatRect());
    }

    SUBCASE("Set/get texture rect")
    {
        TriangleShape triangleShape({});
        triangleShape.setTextureRect({{4, 5}, {6, 7}});
        CHECK(triangleShape.getTextureRect() == sf::IntRect({4, 5}, {6, 7}));
    }

    SUBCASE("Set/get fill color")
    {
        TriangleShape triangleShape({});
        triangleShape.setFillColor(sf::Color::Cyan);
        CHECK(triangleShape.getFillColor() == sf::Color::Cyan);
    }

    SUBCASE("Set/get outline color")
    {
        TriangleShape triangleShape({});
        triangleShape.setOutlineColor(sf::Color::Magenta);
        CHECK(triangleShape.getOutlineColor() == sf::Color::Magenta);
    }

    SUBCASE("Set/get outline thickness")
    {
        TriangleShape triangleShape({});
        triangleShape.setOutlineThickness(3.14f);
        CHECK(triangleShape.getOutlineThickness() == 3.14f);
    }

    SUBCASE("Virtual functions: getPoint, getPointCount")
    {
        const TriangleShape triangleShape({2.f, 2.f});
        CHECK(triangleShape.getPointCount() == 3);
        CHECK(triangleShape.getPoint(0) == sf::Vector2f(1.f, 0.f));
        CHECK(triangleShape.getPoint(1) == sf::Vector2f(0.f, 2.f));
        CHECK(triangleShape.getPoint(2) == sf::Vector2f(2.f, 2.f));
    }

    SUBCASE("Get bounds")
    {
        TriangleShape triangleShape({2, 3});
        CHECK(triangleShape.getLocalBounds() == sf::FloatRect({0.f, 0.f}, {2.f, 3.f}));
        CHECK(triangleShape.getGlobalBounds() == sf::FloatRect({0.f, 0.f}, {2.f, 3.f}));
        triangleShape.move({1.f, 1.f});
        triangleShape.rotate(sf::degrees(90));
        CHECK(triangleShape.getLocalBounds() == sf::FloatRect({0.f, 0.f}, {2.f, 3.f}));
        CHECK(triangleShape.getGlobalBounds().left == Approx(-2.f));
        CHECK(triangleShape.getGlobalBounds().top == Approx(1.f));
        CHECK(triangleShape.getGlobalBounds().width == Approx(3.f));
        CHECK(triangleShape.getGlobalBounds().height == Approx(2.f));
    }
}
