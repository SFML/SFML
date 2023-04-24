#include <SFML/Graphics/Shape.hpp>

#include <catch2/catch_test_macros.hpp>

#include <GraphicsUtil.hpp>
#include <type_traits>

class TriangleShape : public sf::Shape
{
public:
    explicit TriangleShape(const sf::Vector2f& size) : m_size(size)
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
            case 0:
                return sf::Vector2f(m_size.x / 2, 0);
            case 1:
                return sf::Vector2f(0, m_size.y);
            case 2:
                return sf::Vector2f(m_size.x, m_size.y);
        }
    }

private:
    sf::Vector2f m_size;
};

TEST_CASE("[Graphics] sf::Shape")
{
    SECTION("Type traits")
    {
        STATIC_CHECK(!std::is_constructible_v<sf::Shape>);
        STATIC_CHECK(!std::is_copy_constructible_v<sf::Shape>);
        STATIC_CHECK(std::is_copy_assignable_v<sf::Shape>);
        STATIC_CHECK(!std::is_move_constructible_v<sf::Shape>);
        STATIC_CHECK(std::is_nothrow_move_assignable_v<sf::Shape>);
        STATIC_CHECK(std::has_virtual_destructor_v<sf::Shape>);
    }

    SECTION("Default constructor")
    {
        const TriangleShape triangleShape({0, 0});
        CHECK(triangleShape.getTexture() == nullptr);
        CHECK(triangleShape.getTextureRect() == sf::IntRect());
        CHECK(triangleShape.getFillColor() == sf::Color::White);
        CHECK(triangleShape.getOutlineColor() == sf::Color::White);
        CHECK(triangleShape.getOutlineThickness() == 0.0f);
        CHECK(triangleShape.getLocalBounds() == sf::FloatRect());
        CHECK(triangleShape.getGlobalBounds() == sf::FloatRect());
    }

    SECTION("Set/get texture rect")
    {
        TriangleShape triangleShape({});
        triangleShape.setTextureRect({{4, 5}, {6, 7}});
        CHECK(triangleShape.getTextureRect() == sf::IntRect({4, 5}, {6, 7}));
    }

    SECTION("Set/get fill color")
    {
        TriangleShape triangleShape({});
        triangleShape.setFillColor(sf::Color::Cyan);
        CHECK(triangleShape.getFillColor() == sf::Color::Cyan);
    }

    SECTION("Set/get outline color")
    {
        TriangleShape triangleShape({});
        triangleShape.setOutlineColor(sf::Color::Magenta);
        CHECK(triangleShape.getOutlineColor() == sf::Color::Magenta);
    }

    SECTION("Set/get outline thickness")
    {
        TriangleShape triangleShape({});
        triangleShape.setOutlineThickness(3.14f);
        CHECK(triangleShape.getOutlineThickness() == 3.14f);
    }

    SECTION("Virtual functions: getPoint, getPointCount, getGeometricCenter")
    {
        const TriangleShape triangleShape({2, 2});
        CHECK(triangleShape.getPointCount() == 3);
        CHECK(triangleShape.getPoint(0) == sf::Vector2f(1, 0));
        CHECK(triangleShape.getPoint(1) == sf::Vector2f(0, 2));
        CHECK(triangleShape.getPoint(2) == sf::Vector2f(2, 2));
        CHECK(triangleShape.getGeometricCenter() == sf::Vector2f(1.f, 4.f / 3.f));
    }

    SECTION("Get bounds")
    {
        TriangleShape triangleShape({2, 3});
        CHECK(triangleShape.getLocalBounds() == sf::FloatRect({0, 0}, {2, 3}));
        CHECK(triangleShape.getGlobalBounds() == sf::FloatRect({0, 0}, {2, 3}));
        triangleShape.move({1, 1});
        triangleShape.rotate(sf::degrees(90));
        CHECK(triangleShape.getLocalBounds() == sf::FloatRect({0, 0}, {2, 3}));
        CHECK(triangleShape.getGlobalBounds().left == Approx(-2.f));
        CHECK(triangleShape.getGlobalBounds().top == Approx(1.f));
        CHECK(triangleShape.getGlobalBounds().width == Approx(3.f));
        CHECK(triangleShape.getGlobalBounds().height == Approx(2.f));
    }
}
