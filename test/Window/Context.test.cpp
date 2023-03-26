#include <SFML/Window/Context.hpp>

// Other 1st party headers
#include <SFML/Window/ContextSettings.hpp>

#include <doctest/doctest.h>

#include <WindowUtil.hpp>
#include <string>
#include <type_traits>

#if defined(SFML_SYSTEM_WINDOWS)
#define GLAPI __stdcall
#else
#define GLAPI
#endif

static_assert(!std::is_copy_constructible_v<sf::Context>);
static_assert(!std::is_copy_assignable_v<sf::Context>);
static_assert(!std::is_nothrow_move_constructible_v<sf::Context>);
static_assert(!std::is_nothrow_move_assignable_v<sf::Context>);

TEST_CASE("[Window] sf::Context" * doctest::skip(skipDisplayTests))
{
    SUBCASE("Construction")
    {
        const sf::Context context;

        CHECK(context.getSettings().majorVersion > 0);
    }

    SUBCASE("Version String")
    {
        sf::Context context;

        CHECK(context.setActive(true));

        using glGetStringFuncType = const char*(GLAPI*)(unsigned int);

        auto glGetStringFunc = reinterpret_cast<glGetStringFuncType>(sf::Context::getFunction("glGetString"));

        REQUIRE_UNARY(!!glGetStringFunc);

        constexpr unsigned int glVendor   = 0x1F00;
        constexpr unsigned int glRenderer = 0x1F01;
        constexpr unsigned int glVersion  = 0x1F02;

        const char* vendor   = glGetStringFunc(glVendor);
        const char* renderer = glGetStringFunc(glRenderer);
        const char* version  = glGetStringFunc(glVersion);

        REQUIRE(vendor != nullptr);
        REQUIRE(renderer != nullptr);
        REQUIRE(version != nullptr);

        MESSAGE("\nOpenGL vendor: ",
                std::string(vendor),
                "\nOpenGL renderer: ",
                std::string(renderer),
                "\nOpenGL version: ",
                std::string(version));
    }
}
