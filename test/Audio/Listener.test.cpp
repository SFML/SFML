#include <SFML/Audio/Listener.hpp>

#include <catch2/catch_test_macros.hpp>

#include <SystemUtil.hpp>

TEST_CASE("[Audio] sf::Listener")
{
    SECTION("Cone")
    {
        constexpr sf::Listener::Cone cone;
        STATIC_CHECK(cone.innerAngle == sf::radians(0));
        STATIC_CHECK(cone.outerAngle == sf::radians(0));
        STATIC_CHECK(cone.outerGain == 0);
    }

    SECTION("Set/get global volume")
    {
        CHECK(sf::Listener::getGlobalVolume() == 100);
        sf::Listener::setGlobalVolume(50);
        CHECK(sf::Listener::getGlobalVolume() == 50);
    }

    SECTION("Set/get position")
    {
        CHECK(sf::Listener::getPosition() == sf::Vector3f());
        sf::Listener::setPosition({1, 2, 3});
        CHECK(sf::Listener::getPosition() == sf::Vector3f(1, 2, 3));
    }

    SECTION("Set/get direction")
    {
        CHECK(sf::Listener::getDirection() == sf::Vector3f(0, 0, -1));
        sf::Listener::setDirection({4, 5, 6});
        CHECK(sf::Listener::getDirection() == sf::Vector3f(4, 5, 6));
    }

    SECTION("Set/get velocity")
    {
        CHECK(sf::Listener::getVelocity() == sf::Vector3f());
        sf::Listener::setVelocity({7, 8, 9});
        CHECK(sf::Listener::getVelocity() == sf::Vector3f(7, 8, 9));
    }

    SECTION("Set/get cone")
    {
        auto cone = sf::Listener::getCone();
        CHECK(cone.innerAngle == sf::degrees(360));
        CHECK(cone.outerAngle == sf::degrees(360));
        CHECK(cone.outerGain == 1);
        sf::Listener::setCone({sf::radians(1), sf::radians(2), 3});
        cone = sf::Listener::getCone();
        CHECK(cone.innerAngle == sf::radians(1));
        CHECK(cone.outerAngle == sf::radians(2));
        CHECK(cone.outerGain == 3);
    }

    SECTION("Set/get up vector")
    {
        CHECK(sf::Listener::getUpVector() == sf::Vector3f(0, 1, 0));
        sf::Listener::setUpVector({1, 0, 1});
        CHECK(sf::Listener::getUpVector() == sf::Vector3f(1, 0, 1));
    }
}
