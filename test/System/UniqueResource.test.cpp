#include <SFML/System/UniqueResource.hpp>

#include <catch2/catch_test_macros.hpp>

#include <type_traits>

namespace
{
struct Deleter
{
    static inline auto count{0};

    void operator()(int) const
    {
        ++count;
    }
};
} // namespace

TEST_CASE("[System] sf::UniqueResource")
{
    SECTION("Type traits")
    {
        STATIC_CHECK(!std::is_copy_constructible_v<sf::UniqueResource<int, Deleter>>);
        STATIC_CHECK(!std::is_copy_assignable_v<sf::UniqueResource<int, Deleter>>);
        STATIC_CHECK(std::is_nothrow_move_constructible_v<sf::UniqueResource<int, Deleter>>);
        STATIC_CHECK(std::is_nothrow_move_assignable_v<sf::UniqueResource<int, Deleter>>);
    }

    SECTION("Construction")
    {
        SECTION("Default constructor")
        {
            const auto deleteCount = Deleter::count;
            {
                const sf::UniqueResource<int, Deleter> uniqueResource;
                CHECK(!uniqueResource);
                CHECK(uniqueResource.get() == 0);
            }
            CHECK(Deleter::count == deleteCount);
        }

        SECTION("Handle constructor")
        {
            const auto deleteCount = Deleter::count;
            {
                const sf::UniqueResource<int, Deleter> uniqueResource(42);
                CHECK(!!uniqueResource);
                CHECK(uniqueResource.get() == 42);
            }
            CHECK(Deleter::count == deleteCount + 1);
        }
    }

    SECTION("Move semantics")
    {
        SECTION("Construction")
        {
            const auto deleteCount = Deleter::count;
            {
                sf::UniqueResource<int, Deleter>       movedUniqueResource(42);
                const sf::UniqueResource<int, Deleter> uniqueResource(std::move(movedUniqueResource));
                CHECK(!!uniqueResource);
                CHECK(uniqueResource.get() == 42);
                CHECK(Deleter::count == deleteCount);
            }
            CHECK(Deleter::count == deleteCount + 1);
        }

        SECTION("Assignment")
        {
            const auto deleteCount = Deleter::count;
            {
                sf::UniqueResource<int, Deleter> uniqueResource(123);
                uniqueResource = sf::UniqueResource<int, Deleter>(42);
                CHECK(!!uniqueResource);
                CHECK(uniqueResource.get() == 42);
                CHECK(Deleter::count == deleteCount + 1);
            }
            CHECK(Deleter::count == deleteCount + 2);
        }
    }

    SECTION("release()")
    {
        const auto deleteCount = Deleter::count;
        {
            sf::UniqueResource<int, Deleter> uniqueResource(42);
            uniqueResource.release();
            CHECK(!uniqueResource);
            CHECK(uniqueResource.get() == 0);
            CHECK(Deleter::count == deleteCount);
        }
        CHECK(Deleter::count == deleteCount);
    }

    SECTION("reset()")
    {
        const auto deleteCount = Deleter::count;
        {
            sf::UniqueResource<int, Deleter> uniqueResource(42);
            uniqueResource.reset();
            CHECK(!uniqueResource);
            CHECK(uniqueResource.get() == 0);
            CHECK(Deleter::count == deleteCount + 1);
        }
        CHECK(Deleter::count == deleteCount + 1);
    }

    SECTION("reset(T)")
    {
        const auto deleteCount = Deleter::count;
        {
            sf::UniqueResource<int, Deleter> uniqueResource(42);
            uniqueResource.reset(123);
            CHECK(!!uniqueResource);
            CHECK(uniqueResource.get() == 123);
            CHECK(Deleter::count == deleteCount + 1);
        }
        CHECK(Deleter::count == deleteCount + 2);
    }
}
