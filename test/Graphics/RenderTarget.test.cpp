#include <SFML/Graphics/RenderTarget.hpp>

#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>

#include <SystemUtil.hpp>
#include <type_traits>

class RenderTarget : public sf::RenderTarget
{
public:
    RenderTarget() = default;

private:
    sf::Vector2u getSize() const override
    {
        return {640, 480};
    }
};

TEST_CASE("[Graphics] sf::RenderTarget")
{
    SECTION("Type traits")
    {
        STATIC_CHECK(!std::is_constructible_v<sf::RenderTarget>);
        STATIC_CHECK(!std::is_copy_constructible_v<sf::RenderTarget>);
        STATIC_CHECK(!std::is_copy_assignable_v<sf::RenderTarget>);
        STATIC_CHECK(!std::is_nothrow_move_constructible_v<sf::RenderTarget>);
        STATIC_CHECK(std::is_nothrow_move_assignable_v<sf::RenderTarget>);
    }

    SECTION("Construction")
    {
        const RenderTarget renderTarget;
        CHECK(renderTarget.getView().getCenter() == sf::Vector2f(500, 500));
        CHECK(renderTarget.getView().getSize() == sf::Vector2f(1000, 1000));
        CHECK(renderTarget.getView().getRotation() == sf::Angle::Zero);
        CHECK(renderTarget.getView().getViewport() == sf::FloatRect({0, 0}, {1, 1}));
        CHECK(renderTarget.getView().getTransform() == sf::Transform(.002f, 0, -1, 0, -.002f, 1, 0, 0, 1));
        CHECK(renderTarget.getDefaultView().getCenter() == sf::Vector2f(500, 500));
        CHECK(renderTarget.getDefaultView().getSize() == sf::Vector2f(1000, 1000));
        CHECK(renderTarget.getDefaultView().getRotation() == sf::Angle::Zero);
        CHECK(renderTarget.getDefaultView().getViewport() == sf::FloatRect({0, 0}, {1, 1}));
        CHECK(renderTarget.getDefaultView().getTransform() == sf::Transform(.002f, 0, -1, 0, -.002f, 1, 0, 0, 1));
        CHECK(!renderTarget.isSrgb());
    }

    SECTION("Set/get view")
    {
        RenderTarget renderTarget;
        renderTarget.setView({{1, 2}, {3, 4}});
        CHECK(renderTarget.getView().getCenter() == sf::Vector2f(1, 2));
        CHECK(renderTarget.getView().getSize() == sf::Vector2f(3, 4));
    }

    SECTION("setActive()")
    {
        RenderTarget renderTarget;
        CHECK(renderTarget.setActive());
        CHECK(renderTarget.setActive(false));
        CHECK(renderTarget.setActive(true));
    }

    SECTION("getViewport(const View&)")
    {
        const auto makeView = [](const auto& viewport)
        {
            sf::View view;
            view.setViewport(viewport);
            return view;
        };

        const RenderTarget renderTarget;
        CHECK(renderTarget.getViewport(makeView(sf::FloatRect({0, 0}, {1, 1}))) == sf::IntRect({0, 0}, {640, 480}));
        CHECK(renderTarget.getViewport(makeView(sf::FloatRect({1, 1}, {.5f, .25f}))) ==
              sf::IntRect({640, 480}, {320, 120}));
        CHECK(renderTarget.getViewport(makeView(sf::FloatRect({.5f, .5f}, {.25f, .75f}))) ==
              sf::IntRect({320, 240}, {160, 360}));
    }

    SECTION("mapPixelToCoords(const Vector2i&)")
    {
        sf::View view;
        view.move({5, 5});
        view.setViewport(sf::FloatRect({0, 0}, {.5f, 1}));
        RenderTarget renderTarget;
        renderTarget.setView(view);
        const auto [x1, y1] = renderTarget.mapPixelToCoords({0, 0});
        CHECK_THAT(x1, Catch::Matchers::WithinRel(5, 1e-4));
        CHECK_THAT(y1, Catch::Matchers::WithinRel(5, 1e-4));
        const auto [x2, y2] = renderTarget.mapPixelToCoords({1, 1});
        CHECK_THAT(x2, Catch::Matchers::WithinRel(8.125, 1e-4));
        CHECK_THAT(y2, Catch::Matchers::WithinRel(7.0833, 1e-4));
        const auto [x3, y3] = renderTarget.mapPixelToCoords({320, 240});
        CHECK_THAT(x3, Catch::Matchers::WithinRel(1005, 1e-5));
        CHECK_THAT(y3, Catch::Matchers::WithinRel(505, 1e-5));
    }

    SECTION("mapPixelToCoords(const Vector2i&, const View&)")
    {
        sf::View view;
        view.move({5, 5});
        view.setViewport(sf::FloatRect({.5f, .5f}, {.5f, 1}));
        const RenderTarget renderTarget;
        const auto [x1, y1] = renderTarget.mapPixelToCoords({0, 0}, view);
        CHECK_THAT(x1, Catch::Matchers::WithinRel(-995, 1e-5));
        CHECK_THAT(y1, Catch::Matchers::WithinRel(-495, 1e-5));
        const auto [x2, y2] = renderTarget.mapPixelToCoords({320, 480}, view);
        CHECK_THAT(x2, Catch::Matchers::WithinAbs(5, 1e-4));
        CHECK_THAT(y2, Catch::Matchers::WithinRel(505, 1e-5));
    }

    SECTION("mapCoordsToPixel(const Vector2f&)")
    {
        sf::View view;
        view.move({5, 5});
        view.setViewport(sf::FloatRect({.25f, 0}, {1, 1}));
        RenderTarget renderTarget;
        renderTarget.setView(view);
        CHECK(renderTarget.mapCoordsToPixel({0, 0}) == sf::Vector2i(156, -2));
        CHECK(renderTarget.mapCoordsToPixel({-500, 0}) == sf::Vector2i(-163, -2));
        CHECK(renderTarget.mapCoordsToPixel({0, -250}) == sf::Vector2i(156, -122));
    }

    SECTION("mapCoordsToPixel(const Vector2f&, const View&)")
    {
        sf::View view;
        view.move({5, 5});
        view.setViewport(sf::FloatRect({0, 0}, {.5, .25f}));
        RenderTarget renderTarget;
        renderTarget.setView(view);
        CHECK(renderTarget.mapCoordsToPixel({0, 0}) == sf::Vector2i(-1, 0));
        CHECK(renderTarget.mapCoordsToPixel({320, 0}) == sf::Vector2i(100, 0));
        CHECK(renderTarget.mapCoordsToPixel({0, 480}) == sf::Vector2i(-1, 57));
        CHECK(renderTarget.mapCoordsToPixel({640, 480}) == sf::Vector2i(203, 57));
    }
}
