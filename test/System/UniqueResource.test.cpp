#include <SFML/System/UniqueResource.hpp>

#include <catch2/catch_test_macros.hpp>

#include <SystemUtil.hpp>
#include <type_traits>

namespace
{
struct Deleter
{
    static inline std::size_t count{0};

    void operator()(int)
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
            }
            CHECK(Deleter::count == deleteCount);
        }

        SECTION("Handle constructor")
        {
            const auto deleteCount = Deleter::count;
            {
                const sf::UniqueResource<int, Deleter> uniqueResource(42);
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
                CHECK(Deleter::count == deleteCount);
            }
            CHECK(Deleter::count == deleteCount + 1);
        }

        SECTION("Assignment")
        {
            const auto deleteCount = Deleter::count;
            {
                sf::UniqueResource<int, Deleter> movedUniqueResource(42);
                sf::UniqueResource<int, Deleter> uniqueResource;
                uniqueResource = std::move(movedUniqueResource);
                CHECK(Deleter::count == deleteCount);
            }
            CHECK(Deleter::count == deleteCount + 1);
        }
    }

    SECTION("release()")
    {
        const auto deleteCount = Deleter::count;
        {
            sf::UniqueResource<int, Deleter> uniqueResource(42);
            uniqueResource.release();
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
            CHECK(Deleter::count == deleteCount + 1);
        }
        CHECK(Deleter::count == deleteCount + 2);
    }
}
