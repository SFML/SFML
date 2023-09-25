#include <SFML/Window/Context.hpp>

// Other 1st party headers
#include <SFML/Window/ContextSettings.hpp>

#include <catch2/catch_test_macros.hpp>

#include <WindowUtil.hpp>
#include <string>
#include <type_traits>

#if defined(SFML_SYSTEM_WINDOWS)
#define GLAPI __stdcall
#else
#define GLAPI
#endif

TEST_CASE("[Window] sf::Context", runDisplayTests())
{
    SECTION("Type traits")
    {
        STATIC_CHECK(!std::is_copy_constructible_v<sf::Context>);
        STATIC_CHECK(!std::is_copy_assignable_v<sf::Context>);
        STATIC_CHECK(!std::is_nothrow_move_constructible_v<sf::Context>);
        STATIC_CHECK(!std::is_nothrow_move_assignable_v<sf::Context>);
    }

    SECTION("Construction")
    {
        const sf::Context context;
        CHECK(context.getSettings().majorVersion > 0);
        CHECK(context.getActiveContext() == &context);
        CHECK(context.getActiveContextId() != 0);
    }

    SECTION("setActive()")
    {
        sf::Context context;
        CHECK(context.setActive(false));
        CHECK(context.getActiveContext() == nullptr);
        CHECK(context.getActiveContextId() == 0);
        CHECK(context.setActive(true));
        CHECK(context.getActiveContext() == &context);
        CHECK(context.getActiveContextId() != 0);
    }

    SECTION("getActiveContext()/getActiveContextId()")
    {
        CHECK(sf::Context::getActiveContext() == nullptr);
        CHECK(sf::Context::getActiveContextId() == 0);

        {
            const sf::Context context;
            CHECK(context.getSettings().majorVersion > 0);
            CHECK(sf::Context::getActiveContext() == &context);
            CHECK(sf::Context::getActiveContextId() != 0);
        }

        CHECK(sf::Context::getActiveContext() == nullptr);
        CHECK(sf::Context::getActiveContextId() == 0);
    }

    SECTION("Version String")
    {
        sf::Context context;
        CHECK(context.setActive(true));

        using glGetStringFuncType  = const char*(GLAPI*)(unsigned int);
        const auto glGetStringFunc = reinterpret_cast<glGetStringFuncType>(sf::Context::getFunction("glGetString"));
        REQUIRE(glGetStringFunc);

        constexpr unsigned int glVendor   = 0x1F00;
        constexpr unsigned int glRenderer = 0x1F01;
        constexpr unsigned int glVersion  = 0x1F02;

        const char* vendor   = glGetStringFunc(glVendor);
        const char* renderer = glGetStringFunc(glRenderer);
        const char* version  = glGetStringFunc(glVersion);

        REQUIRE(vendor != nullptr);
        REQUIRE(renderer != nullptr);
        REQUIRE(version != nullptr);

        SUCCEED(std::string("OpenGL vendor: ") + vendor);
        SUCCEED(std::string("OpenGL renderer: ") + renderer);
        SUCCEED(std::string("OpenGL version: ") + version);
    }
}
