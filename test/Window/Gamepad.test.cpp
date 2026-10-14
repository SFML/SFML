#include <SFML/Window/Gamepad.hpp>

// Other 1st party headers
#include <SFML/Window/Joystick.hpp>

#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators_range.hpp>

#include <string>

namespace
{
#if defined(SFML_SYSTEM_WINDOWS)
const std::string platform      = "Windows";
const std::string otherPlatform = "Linux";
#elif defined(SFML_SYSTEM_MACOS)
const std::string platform      = "Mac OS X";
const std::string otherPlatform = "Windows";
#elif defined(SFML_SYSTEM_LINUX)
const std::string platform      = "Linux";
const std::string otherPlatform = "Windows";
#elif defined(SFML_SYSTEM_ANDROID)
const std::string platform      = "Android";
const std::string otherPlatform = "Windows";
#elif defined(SFML_SYSTEM_IOS)
const std::string platform      = "iOS";
const std::string otherPlatform = "Windows";
#elif defined(SFML_SYSTEM_FREEBSD)
const std::string platform      = "FreeBSD";
const std::string otherPlatform = "Windows";
#elif defined(SFML_SYSTEM_OPENBSD)
const std::string platform      = "OpenBSD";
const std::string otherPlatform = "Windows";
#elif defined(SFML_SYSTEM_NETBSD)
const std::string platform      = "NetBSD";
const std::string otherPlatform = "Windows";
#endif

const std::string xbox360 =
    "030000005e0400008e02000000000000,Xbox 360 Controller,a:b0,b:b1,back:b6,dpdown:h0.4,"
    "dpleft:h0.8,dpright:h0.2,dpup:h0.1,leftshoulder:b4,leftstick:b8,lefttrigger:+a2,"
    "leftx:a0,lefty:a1,rightshoulder:b5,rightstick:b9,righttrigger:-a2,rightx:a3,righty:a4,"
    "start:b7,x:b2,y:b3,";
} // namespace

TEST_CASE("[Window] sf::Gamepad")
{
    SECTION("Constants")
    {
        STATIC_CHECK(sf::Gamepad::ButtonCount == 21);
        STATIC_CHECK(sf::Gamepad::AxisCount == 6);
    }

    SECTION("addMappings()")
    {
        SECTION("Current platform")
        {
            CHECK(sf::Gamepad::addMappings(xbox360 + "platform:" + platform + ",") == 1);
        }

        SECTION("Other platform")
        {
            CHECK(sf::Gamepad::addMappings(xbox360 + "platform:" + otherPlatform + ",") == 0);
        }

        SECTION("Any platform")
        {
            CHECK(sf::Gamepad::addMappings(xbox360) == 1);
        }

        SECTION("Several lines")
        {
            const std::string mappings = "# Comment\r\n\r\n" + xbox360 + "\r\n" +
                                         "030000004c050000c405000000000000,PS4 Controller,a:b1,b:b2,x:b0,y:b3,\n" +
                                         "03000000c82d00000121000000000000,8BitDo,a:b0,b:b1,platform:" + otherPlatform;
            CHECK(sf::Gamepad::addMappings(mappings) == 2);
        }

        SECTION("Invalid mappings")
        {
            CHECK(sf::Gamepad::addMappings("") == 0);
            CHECK(sf::Gamepad::addMappings("# Comment") == 0);
            CHECK(sf::Gamepad::addMappings("xinput,XInput Controller,a:b0,b:b1,") == 0);
            CHECK(sf::Gamepad::addMappings("0300,Too Short,a:b0,b:b1,") == 0);
            CHECK(sf::Gamepad::addMappings("030000005e0400008e02000000000000") == 0);
        }

        SECTION("SDL driver")
        {
            // Mappings for SDL's own drivers (here HIDAPI) don't apply to what the operating system reports
            CHECK(sf::Gamepad::addMappings("050000004c050000cc09000000006800,PS4 Controller,a:b0,b:b1,") == 0);
        }

        SECTION("Hints")
        {
            CHECK(sf::Gamepad::addMappings(xbox360 + "hint:!SDL_GAMECONTROLLER_USE_BUTTON_LABELS:=1,") == 1);
            CHECK(sf::Gamepad::addMappings(xbox360 + "hint:SDL_GAMECONTROLLER_USE_BUTTON_LABELS:=1,") == 0);
        }
    }

    SECTION("addMappingsFromFile()")
    {
        CHECK(sf::Gamepad::addMappingsFromFile("does/not/exist.txt") == 0);
    }

    // By avoiding calling sf::Joystick::update() we can guarantee that
    // no joysticks will be detected. This is how we can ensure these
    // tests are portable and reliable.

    const auto joystick = GENERATE(range(0u, sf::Joystick::Count));

    SECTION("isConnected()")
    {
        CHECK(!sf::Gamepad::isConnected(joystick));
    }

    SECTION("hasButton()")
    {
        const auto button = static_cast<sf::Gamepad::Button>(GENERATE(range(0u, sf::Gamepad::ButtonCount)));
        CHECK(!sf::Gamepad::hasButton(joystick, button));
    }

    SECTION("hasAxis()")
    {
        const auto axis = static_cast<sf::Gamepad::Axis>(GENERATE(range(0u, sf::Gamepad::AxisCount)));
        CHECK(!sf::Gamepad::hasAxis(joystick, axis));
    }

    SECTION("isButtonPressed()")
    {
        const auto button = static_cast<sf::Gamepad::Button>(GENERATE(range(0u, sf::Gamepad::ButtonCount)));
        CHECK(!sf::Gamepad::isButtonPressed(joystick, button));
    }

    SECTION("getAxisPosition()")
    {
        const auto axis = static_cast<sf::Gamepad::Axis>(GENERATE(range(0u, sf::Gamepad::AxisCount)));
        CHECK(sf::Gamepad::getAxisPosition(joystick, axis) == 0);
    }

    SECTION("getName()")
    {
        CHECK(sf::Gamepad::getName(joystick).isEmpty());
    }

    SECTION("getMapping()")
    {
        CHECK(sf::Gamepad::getMapping(joystick).empty());
    }
}
