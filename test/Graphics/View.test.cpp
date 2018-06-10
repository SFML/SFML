#include <SFML/Graphics/View.hpp>

#include <catch2/catch_test_macros.hpp>

#include <GraphicsUtil.hpp>
#include <type_traits>

TEST_CASE("[Graphics] sf::View")
{
    SECTION("Type traits")
    {
        STATIC_CHECK(std::is_copy_constructible_v<sf::View>);
        STATIC_CHECK(std::is_copy_assignable_v<sf::View>);
        STATIC_CHECK(std::is_nothrow_move_constructible_v<sf::View>);
        STATIC_CHECK(std::is_nothrow_move_assignable_v<sf::View>);
    }

    SECTION("Construction")
    {
        SECTION("Default constructor")
        {
            const sf::View view;
            CHECK(view.getCenter() == sf::Vector2f(500, 500));
            CHECK(view.getSize() == sf::Vector2f(1000, 1000));
            CHECK(view.getRotation() == sf::Angle::Zero);
            CHECK(view.getViewport() == sf::FloatRect({0, 0}, {1, 1}));
            CHECK(view.getScissor() == sf::FloatRect({0, 0}, {1, 1}));
            CHECK(view.getTransform() == sf::Transform(0.002f, 0, -1, 0, -0.002f, 1, 0, 0, 1));
            CHECK(view.getInverseTransform() == Approx(sf::Transform(500, 0, 500, 0, -500, 500, 0, 0, 1)));
        }

        SECTION("Rectangle constructor")
        {
            const sf::View view(sf::FloatRect({10, 20}, {400, 600}));
            CHECK(view.getCenter() == sf::Vector2f(210, 320));
            CHECK(view.getSize() == sf::Vector2f(400, 600));
            CHECK(view.getRotation() == sf::Angle::Zero);
            CHECK(view.getViewport() == sf::FloatRect({0, 0}, {1, 1}));
            CHECK(view.getScissor() == sf::FloatRect({0, 0}, {1, 1}));
            CHECK(view.getTransform() == Approx(sf::Transform(0.005f, 0, -1.05f, 0, -0.00333333f, 1.06667f, 0, 0, 1)));
            CHECK(view.getInverseTransform() == Approx(sf::Transform(200, 0, 210, 0, -300, 320, 0, 0, 1)));
        }

        SECTION("Center + size constructor")
        {
            const sf::View view({520, 960}, {1080, 1920});
            CHECK(view.getCenter() == sf::Vector2f(520, 960));
            CHECK(view.getSize() == sf::Vector2f(1080, 1920));
            CHECK(view.getRotation() == sf::Angle::Zero);
            CHECK(view.getViewport() == sf::FloatRect({0, 0}, {1, 1}));
            CHECK(view.getScissor() == sf::FloatRect({0, 0}, {1, 1}));
            CHECK(view.getTransform() == Approx(sf::Transform(0.00185185f, 0, -0.962963f, 0, -0.00104167f, 1, 0, 0, 1)));
            CHECK(view.getInverseTransform() == Approx(sf::Transform(540, 0, 520, 0, -960, 960, 0, 0, 1)));
        }
    }

    SECTION("Set/get center")
    {
        sf::View view;
        view.setCenter({3.14f, 4.2f});
        CHECK(view.getCenter() == sf::Vector2f(3.14f, 4.2f));
        CHECK(view.getTransform() == Approx(sf::Transform(0.002f, 0, -0.00628f, 0, -0.002f, 0.0084f, 0, 0, 1)));
        CHECK(view.getInverseTransform() == Approx(sf::Transform(500, 0, 3.14f, 0, -500, 4.2f, 0, 0, 1)));
    }

    SECTION("Set/get size")
    {
        sf::View view;
        view.setSize({600, 900});
        CHECK(view.getSize() == sf::Vector2f(600, 900));
        CHECK(view.getTransform() == Approx(sf::Transform(0.00333333f, 0, -1.66667f, 0, -0.00222222f, 1.11111f, 0, 0, 1)));
        CHECK(view.getInverseTransform() == Approx(sf::Transform(300, 0, 500, 0, -450, 500, 0, 0, 1)));
    }

    SECTION("Set/get rotation")
    {
        sf::View view;
        view.setRotation(sf::degrees(-345));
        CHECK(view.getRotation() == sf::degrees(15));
        CHECK(view.getTransform() ==
              Approx(sf::Transform(0.00193185f, 0.000517638f, -1.22474f, 0.000517638f, -0.00193185f, 0.707107f, 0, 0, 1)));
        CHECK(view.getInverseTransform() ==
              Approx(sf::Transform(482.963f, 129.41f, 500, 129.41f, -482.963f, 500, 0, 0, 1)));
        view.setRotation(sf::degrees(400));
        CHECK(view.getRotation() == sf::degrees(40));
        CHECK(view.getTransform() ==
              Approx(sf::Transform(0.00153209f, 0.00128558f, -1.40883f, 0.00128558f, -0.00153209f, 0.123257f, 0, 0, 1)));
        CHECK(view.getInverseTransform() ==
              Approx(sf::Transform(383.022f, 321.394f, 500, 321.394f, -383.022f, 500, 0, 0, 1)));
    }

    SECTION("Set/get viewport")
    {
        sf::View view;
        view.setViewport({{150, 250}, {500, 750}});
        CHECK(view.getViewport() == sf::FloatRect({150, 250}, {500, 750}));
        CHECK(view.getTransform() == Approx(sf::Transform(0.002f, 0, -1, 0, -0.002f, 1, 0, 0, 1)));
        CHECK(view.getInverseTransform() == Approx(sf::Transform(500, 0, 500, 0, -500, 500, 0, 0, 1)));
        CHECK(view.getScissor() == sf::FloatRect({0, 0}, {1, 1}));
    }

    SECTION("Set/get scissor")
    {
        sf::View view;
        view.setScissor({{0, 0}, {0.5f, 1}});
        CHECK(view.getScissor() == sf::FloatRect({0, 0}, {0.5, 1}));
        CHECK(view.getViewport() == sf::FloatRect({0, 0}, {1, 1}));
    }

    SECTION("reset()")
    {
        sf::View view;
        view.setCenter({3.14f, 4.2f});
        view.setSize({600, 900});
        view.setRotation(sf::degrees(15));
        view.setViewport({{150, 250}, {500, 750}});
        view.setScissor({{0.2f, 0.3f}, {0.4f, 0.5f}});
        view.reset({{1, 2}, {3, 4}});
        CHECK(view.getCenter() == sf::Vector2f(2.5f, 4));
        CHECK(view.getSize() == sf::Vector2f(3, 4));
        CHECK(view.getRotation() == sf::Angle::Zero);
        CHECK(view.getViewport() == sf::FloatRect({150, 250}, {500, 750}));
        CHECK(view.getScissor() == sf::FloatRect({0.2f, 0.3f}, {0.4f, 0.5f}));
        CHECK(view.getTransform() == Approx(sf::Transform(0.666667f, 0, -1.66667f, 0, -0.5f, 2, 0, 0, 1)));
        CHECK(view.getInverseTransform() == Approx(sf::Transform(1.5f, 0, 2.5f, 0, -2, 4, 0, 0, 1)));
    }

    SECTION("move()")
    {
        sf::View view;
        view.setCenter({25, 25});
        view.move({15, 25});
        CHECK(view.getCenter() == sf::Vector2f(40, 50));
        CHECK(view.getTransform() == Approx(sf::Transform(0.002f, 0, -0.08f, 0, -0.002f, 0.1f, 0, 0, 1)));
        CHECK(view.getInverseTransform() == Approx(sf::Transform(500, 0, 40, 0, -500, 50, 0, 0, 1)));
    }

    SECTION("rotate()")
    {
        sf::View view;
        view.setRotation(sf::degrees(45));
        view.rotate(sf::degrees(-15));
        CHECK(view.getRotation() == sf::degrees(30));
        CHECK(view.getTransform() ==
              Approx(sf::Transform(0.00173205f, 0.001f, -1.36603f, 0.001f, -0.00173205f, 0.366025f, 0, 0, 1)));
        CHECK(view.getInverseTransform() == Approx(sf::Transform(433.013f, 250, 500, 250, -433.013f, 500, 0, 0, 1)));
    }

    SECTION("zoom()")
    {
        sf::View view;
        view.setSize({25, 25});
        view.zoom(4);
        CHECK(view.getSize() == sf::Vector2f(100, 100));
        CHECK(view.getTransform() == Approx(sf::Transform(0.02f, 0, -10, 0, -0.02f, 10, 0, 0, 1)));
        CHECK(view.getInverseTransform() == Approx(sf::Transform(50, 0, 500, 0, -50, 500, 0, 0, 1)));
    }
}
