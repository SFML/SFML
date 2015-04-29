#include <SFML/Graphics/StencilMode.hpp>

#include <catch2/catch_test_macros.hpp>

#include <type_traits>

TEST_CASE("[Graphics] sf::StencilMode")
{
    SECTION("Type traits")
    {
        STATIC_CHECK(std::is_copy_constructible_v<sf::StencilMode>);
        STATIC_CHECK(std::is_copy_assignable_v<sf::StencilMode>);
        STATIC_CHECK(std::is_nothrow_move_constructible_v<sf::StencilMode>);
        STATIC_CHECK(std::is_nothrow_move_assignable_v<sf::StencilMode>);
    }

    SECTION("Construction")
    {
        const sf::StencilMode stencilMode;
        CHECK(stencilMode.stencilComparison == sf::StencilComparison::Always);
        CHECK(stencilMode.stencilUpdateOperation == sf::StencilUpdateOperation::Keep);
        CHECK(stencilMode.stencilReference.value == 0u);
        CHECK(stencilMode.stencilMask.value == ~0u);
        CHECK(stencilMode.stencilOnly == false);
    }

    SECTION("Stencil value type traits")
    {
        STATIC_CHECK(!std::is_default_constructible_v<sf::StencilValue>);
        STATIC_CHECK(!std::is_convertible_v<bool, sf::StencilValue>);
        STATIC_CHECK(!std::is_convertible_v<char, sf::StencilValue>);
        STATIC_CHECK(!std::is_convertible_v<unsigned char, sf::StencilValue>);
        STATIC_CHECK(!std::is_convertible_v<short, sf::StencilValue>);
        STATIC_CHECK(!std::is_convertible_v<unsigned short, sf::StencilValue>);
        STATIC_CHECK(std::is_convertible_v<int, sf::StencilValue>);
        STATIC_CHECK(std::is_convertible_v<unsigned int, sf::StencilValue>);
    }

    SECTION("Stencil value construction")
    {
        const sf::StencilValue stencilValue{0};
        CHECK(stencilValue.value == 0u);
    }

    SECTION("Operators")
    {
        SECTION("operator==")
        {
            CHECK(sf::StencilMode{} == sf::StencilMode{});
            CHECK(sf::StencilMode{sf::StencilComparison::Equal, sf::StencilUpdateOperation::Replace, 1, 0u, true} ==
                  sf::StencilMode{sf::StencilComparison::Equal, sf::StencilUpdateOperation::Replace, 1, 0u, true});

            CHECK_FALSE(sf::StencilMode{} ==
                        sf::StencilMode{sf::StencilComparison::Equal, sf::StencilUpdateOperation::Replace, 1, 0u, true});
            CHECK_FALSE(sf::StencilMode{sf::StencilComparison::Greater, sf::StencilUpdateOperation::Invert, 0, ~0u, false} ==
                        sf::StencilMode{sf::StencilComparison::Equal, sf::StencilUpdateOperation::Replace, 1, 0u, true});
        }

        SECTION("operator!=")
        {
            CHECK_FALSE(sf::StencilMode{} != sf::StencilMode{});
            CHECK_FALSE(sf::StencilMode{sf::StencilComparison::Equal, sf::StencilUpdateOperation::Replace, 1, 0u, true} !=
                        sf::StencilMode{sf::StencilComparison::Equal, sf::StencilUpdateOperation::Replace, 1, 0u, true});

            CHECK(sf::StencilMode{} !=
                  sf::StencilMode{sf::StencilComparison::Equal, sf::StencilUpdateOperation::Replace, 1, 0u, true});
            CHECK(sf::StencilMode{sf::StencilComparison::Greater, sf::StencilUpdateOperation::Invert, 0, ~0u, false} !=
                  sf::StencilMode{sf::StencilComparison::Equal, sf::StencilUpdateOperation::Replace, 1, 0u, true});
        }
    }
}
