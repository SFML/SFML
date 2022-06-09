#include <SFML/Graphics/View.hpp>
#include "GraphicsUtil.hpp"

#include <doctest.h>

TEST_CASE("sf::View class - [graphics]")
{
    SUBCASE("Construction")
    {
        SUBCASE("Default constructor")
        {
            const sf::View view;
            CHECK(view.getCenter() == sf::Vector2f(500.f, 500.f));
            CHECK(view.getSize() == sf::Vector2f(1000.f, 1000.f));
            CHECK(view.getRotation() == sf::Angle::Zero);
            CHECK(view.getViewport() == sf::FloatRect({0.f, 0.f}, {1.f, 1.f}));
            CHECK(view.getTransform() == sf::Transform(0.002f, 0.f, -1.f, 0.f, -0.002f, 1.f, 0.f, 0.f, 1.f));
            CHECK(view.getInverseTransform() == Approx(sf::Transform(500.f, 0.f, 500.f, 0.f, -500.f, 500.f, 0.f, 0.f, 1.f)));
        }

        SUBCASE("Rectangle constructor")
        {
            const sf::View view(sf::FloatRect({10.f, 20.f}, {400.f, 600.f}));
            CHECK(view.getCenter() == sf::Vector2f(210.f, 320.f));
            CHECK(view.getSize() == sf::Vector2f(400.f, 600.f));
            CHECK(view.getRotation() == sf::Angle::Zero);
            CHECK(view.getViewport() == sf::FloatRect({0.f, 0.f}, {1.f, 1.f}));
            CHECK(view.getTransform() == Approx(sf::Transform(0.005f, 0.f, -1.05f, 0.f, -0.00333333f, 1.06667f, 0.f, 0.f, 1.f)));
            CHECK(view.getInverseTransform() == Approx(sf::Transform(200.f, 0.f, 210.f, 0.f, -300.f, 320.f, 0.f, 0.f, 1.f)));
        }

        SUBCASE("Center + size constructor")
        {
            const sf::View view({520.f, 960.f}, {1080.f, 1920.f});
            CHECK(view.getCenter() == sf::Vector2f(520.f, 960.f));
            CHECK(view.getSize() == sf::Vector2f(1080.f, 1920.f));
            CHECK(view.getRotation() == sf::Angle::Zero);
            CHECK(view.getViewport() == sf::FloatRect({0.f, 0.f}, {1.f, 1.f}));
            CHECK(view.getTransform() == Approx(sf::Transform(0.00185185f, 0.f, -0.962963f, 0.f, -0.00104167f, 1.f, 0.f, 0.f, 1.f)));
            CHECK(view.getInverseTransform() == Approx(sf::Transform(540.f, 0.f, 520.f, 0.f, -960.f, 960.f, 0.f, 0.f, 1.f)));
        }
    }

    SUBCASE("Set/get center")
    {
        sf::View view;
        view.setCenter({3.14f, 4.2f});
        CHECK(view.getCenter() == sf::Vector2f(3.14f, 4.2f));
        CHECK(view.getTransform() == Approx(sf::Transform(0.002f, 0.f, -0.00628f, 0.f, -0.002f, 0.0084f, 0.f, 0.f, 1.f)));
        CHECK(view.getInverseTransform() == Approx(sf::Transform(500, 0.f, 3.14f, 0.f, -500, 4.2f, 0.f, 0.f, 1.f)));
    }

    SUBCASE("Set/get size")
    {
        sf::View view;
        view.setSize({600.f, 900.f});
        CHECK(view.getSize() == sf::Vector2f(600.f, 900.f));
        CHECK(view.getTransform() == Approx(sf::Transform(0.00333333f, 0.f, -1.66667f, 0.f, -0.00222222f, 1.11111f, 0.f, 0.f, 1.f)));
        CHECK(view.getInverseTransform() == Approx(sf::Transform(300.f, 0.f, 500.f, 0.f, -450.f, 500.f, 0.f, 0.f, 1.f)));
    }

    SUBCASE("Set/get rotation")
    {
        sf::View view;
        view.setRotation(sf::degrees(-345));
        CHECK(view.getRotation() == sf::degrees(15));
        CHECK(view.getTransform() == Approx(sf::Transform(0.00193185f, 0.000517638f, -1.22474f, 0.000517638f, -0.00193185f, 0.707107f, 0.f, 0.f, 1.f)));
        CHECK(view.getInverseTransform() == Approx(sf::Transform(482.963f, 129.41f, 500.f, 129.41f, -482.963f, 500.f, 0.f, 0.f, 1.f)));
        view.setRotation(sf::degrees(400));
        CHECK(view.getRotation() == sf::degrees(40));
        CHECK(view.getTransform() == Approx(sf::Transform(0.00153209f, 0.00128558f, -1.40883f, 0.00128558f, -0.00153209f, 0.123257f, 0.f, 0.f, 1.f)));
        CHECK(view.getInverseTransform() == Approx(sf::Transform(383.022f, 321.394f, 500.f, 321.394f, -383.022f, 500.f, 0.f, 0.f, 1.f)));
    }

    SUBCASE("Set/get viewport")
    {
        sf::View view;
        view.setViewport({{150.f, 250.f}, {500.f, 750.f}});
        CHECK(view.getViewport() == sf::FloatRect({150.f, 250.f}, {500.f, 750.f}));
        CHECK(view.getTransform() == Approx(sf::Transform(0.002f, 0.f, -1.f, 0.f, -0.002f, 1.f, 0.f, 0.f, 1.f)));
        CHECK(view.getInverseTransform() == Approx(sf::Transform(500.f, 0.f, 500.f, 0.f, -500.f, 500.f, 0.f, 0.f, 1.f)));
    }

    SUBCASE("reset()")
    {
        sf::View view;
        view.setCenter({3.14f, 4.2f});
        view.setSize({600.f, 900.f});
        view.setRotation(sf::degrees(15));
        view.setViewport({{150.f, 250.f}, {500.f, 750.f}});
        view.reset({{1.f, 2.f}, {3.f, 4.f}});
        CHECK(view.getCenter() == sf::Vector2f(2.5f, 4.f));
        CHECK(view.getSize() == sf::Vector2f(3.f, 4.f));
        CHECK(view.getRotation() == sf::Angle::Zero);
        CHECK(view.getViewport() == sf::FloatRect({150.f, 250.f}, {500.f, 750.f}));
        CHECK(view.getTransform() == Approx(sf::Transform(0.666667f, 0.f, -1.66667f, 0.f, -0.5f, 2.f, 0.f, 0.f, 1.f)));
        CHECK(view.getInverseTransform() == Approx(sf::Transform(1.5f, 0.f, 2.5f, 0.f, -2.f, 4.f, 0.f, 0.f, 1.f)));
    }

    SUBCASE("move()")
    {
        sf::View view;
        view.setCenter({25.f, 25.f});
        view.move({15.f, 25.f});
        CHECK(view.getCenter() == sf::Vector2f(40.f, 50.f));
        CHECK(view.getTransform() == Approx(sf::Transform(0.002f, 0.f, -0.08f, 0.f, -0.002f, 0.1f, 0.f, 0.f, 1.f)));
        CHECK(view.getInverseTransform() == Approx(sf::Transform(500.f, 0.f, 40.f, 0.f, -500.f, 50.f, 0.f, 0.f, 1.f)));
    }

    SUBCASE("rotate()")
    {
        sf::View view;
        view.setRotation(sf::degrees(45));
        view.rotate(sf::degrees(-15));
        CHECK(view.getRotation() == sf::degrees(30));
        CHECK(view.getTransform() == Approx(sf::Transform(0.00173205f, 0.001f, -1.36603f, 0.001f, -0.00173205f, 0.366025f, 0.f, 0.f, 1.f)));
        CHECK(view.getInverseTransform() == Approx(sf::Transform(433.013f, 250.f, 500.f, 250.f, -433.013f, 500.f, 0.f, 0.f, 1.f)));
    }

    SUBCASE("zoom()")
    {
        sf::View view;
        view.setSize({25.f, 25.f});
        view.zoom(4.f);
        CHECK(view.getSize() == sf::Vector2f(100.f, 100.f));
        CHECK(view.getTransform() == Approx(sf::Transform(0.02f, 0.f, -10.f, 0.f, -0.02f, 10.f, 0.f, 0.f, 1.f)));
        CHECK(view.getInverseTransform() == Approx(sf::Transform(50.f, 0.f, 500.f, 0.f, -50.f, 500.f, 0.f, 0.f, 1.f)));
    }
}
