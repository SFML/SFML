#include <SFML/Audio/AlResource.hpp>
#include <utility>

#include <doctest.h>

namespace {

struct TestAlResource : sf::AlResource { };

}

TEST_CASE("sf::AlResource class - [audio]")
{
    SUBCASE("Never used")
    {
        CHECK(sf::priv::getOpenAlResourceCount() == 0u);
    }

    SUBCASE("Basic ctor/dtor")
    {
        CHECK(sf::priv::getOpenAlResourceCount() == 0u);

        {
            TestAlResource r0;
            CHECK(sf::priv::getOpenAlResourceCount() == 1u);
        }

        CHECK(sf::priv::getOpenAlResourceCount() == 0u);
    }

    SUBCASE("Nested ctor/dtor")
    {
        CHECK(sf::priv::getOpenAlResourceCount() == 0u);

        {
            TestAlResource r0;
            CHECK(sf::priv::getOpenAlResourceCount() == 1u);

            {
                TestAlResource r1;
                CHECK(sf::priv::getOpenAlResourceCount() == 2u);
            }

            CHECK(sf::priv::getOpenAlResourceCount() == 1u);
        }

        CHECK(sf::priv::getOpenAlResourceCount() == 0u);
    }

    SUBCASE("Copy ctor")
    {
        CHECK(sf::priv::getOpenAlResourceCount() == 0u);

        {
            TestAlResource r0;
            CHECK(sf::priv::getOpenAlResourceCount() == 1u);

            {
                TestAlResource r1 = r0;
                CHECK(sf::priv::getOpenAlResourceCount() == 2u);
            }

            CHECK(sf::priv::getOpenAlResourceCount() == 1u);
        }

        CHECK(sf::priv::getOpenAlResourceCount() == 0u);
    }

    SUBCASE("Move ctor")
    {
        CHECK(sf::priv::getOpenAlResourceCount() == 0u);

        {
            TestAlResource r0;
            CHECK(sf::priv::getOpenAlResourceCount() == 1u);

            {
                TestAlResource r1 = std::move(r0);
                CHECK(sf::priv::getOpenAlResourceCount() == 1u);
            }

            CHECK(sf::priv::getOpenAlResourceCount() == 0u);
        }

        CHECK(sf::priv::getOpenAlResourceCount() == 0u);
    }

    SUBCASE("Copy assignment from active to active")
    {
        CHECK(sf::priv::getOpenAlResourceCount() == 0u);

        {
            TestAlResource r0;
            CHECK(sf::priv::getOpenAlResourceCount() == 1u);

            TestAlResource r1;
            CHECK(sf::priv::getOpenAlResourceCount() == 2u);

            r1 = r0;
            CHECK(sf::priv::getOpenAlResourceCount() == 2u);
        }

        CHECK(sf::priv::getOpenAlResourceCount() == 0u);
    }

    SUBCASE("Copy assignment from inactive to active")
    {
        CHECK(sf::priv::getOpenAlResourceCount() == 0u);

        {
            TestAlResource r0;
            CHECK(sf::priv::getOpenAlResourceCount() == 1u);

            TestAlResource r1 = std::move(r0);
            CHECK(sf::priv::getOpenAlResourceCount() == 1u);

            r1 = r0;
            CHECK(sf::priv::getOpenAlResourceCount() == 0u);
        }

        CHECK(sf::priv::getOpenAlResourceCount() == 0u);
    }

    SUBCASE("Move assignment from active to active")
    {
        CHECK(sf::priv::getOpenAlResourceCount() == 0u);

        {
            TestAlResource r0;
            CHECK(sf::priv::getOpenAlResourceCount() == 1u);

            TestAlResource r1;
            CHECK(sf::priv::getOpenAlResourceCount() == 2u);

            r1 = std::move(r0);
            CHECK(sf::priv::getOpenAlResourceCount() == 1u);
        }

        CHECK(sf::priv::getOpenAlResourceCount() == 0u);
    }

    SUBCASE("Move assignment from inactive to active")
    {
        CHECK(sf::priv::getOpenAlResourceCount() == 0u);

        {
            TestAlResource r0;
            CHECK(sf::priv::getOpenAlResourceCount() == 1u);

            TestAlResource r1 = std::move(r0);
            CHECK(sf::priv::getOpenAlResourceCount() == 1u);

            r1 = std::move(r0);
            CHECK(sf::priv::getOpenAlResourceCount() == 0u);
        }

        CHECK(sf::priv::getOpenAlResourceCount() == 0u);
    }

    SUBCASE("Copy assignment from active to inactive")
    {
        CHECK(sf::priv::getOpenAlResourceCount() == 0u);

        {
            TestAlResource r0;
            CHECK(sf::priv::getOpenAlResourceCount() == 1u);

            TestAlResource r1 = std::move(r0);
            CHECK(sf::priv::getOpenAlResourceCount() == 1u);

            r1 = r0;
            CHECK(sf::priv::getOpenAlResourceCount() == 0u);
        }

        CHECK(sf::priv::getOpenAlResourceCount() == 0u);
    }

    SUBCASE("Move assignment from active to inactive")
    {
        CHECK(sf::priv::getOpenAlResourceCount() == 0u);

        {
            TestAlResource r0;
            CHECK(sf::priv::getOpenAlResourceCount() == 1u);

            TestAlResource r1 = std::move(r0);
            CHECK(sf::priv::getOpenAlResourceCount() == 1u);

            r1 = std::move(r0);
            CHECK(sf::priv::getOpenAlResourceCount() == 0u);
        }

        CHECK(sf::priv::getOpenAlResourceCount() == 0u);
    }

    SUBCASE("Copy assignment from inactive to inactive")
    {
        CHECK(sf::priv::getOpenAlResourceCount() == 0u);

        {
            TestAlResource r0;
            TestAlResource r1;
            CHECK(sf::priv::getOpenAlResourceCount() == 2u);

            TestAlResource r2 = std::move(r0);
            CHECK(sf::priv::getOpenAlResourceCount() == 2u);

            TestAlResource r3 = std::move(r1);
            CHECK(sf::priv::getOpenAlResourceCount() == 2u);

            r0 = r1;
            CHECK(sf::priv::getOpenAlResourceCount() == 2u);
        }

        CHECK(sf::priv::getOpenAlResourceCount() == 0u);
    }
}
