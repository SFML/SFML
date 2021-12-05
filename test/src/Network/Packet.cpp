#include <SFML/Network.hpp>

#include <catch.hpp>
#include <limits>

template <typename IntegerType>
static void testPacketStreamOperators(IntegerType expected)
{
    sf::Packet packet;
    packet << expected;
    IntegerType received;
    packet >> received;
    CHECK(expected == received);
}

TEST_CASE("sf::Packet class", "[network]")
{
    SECTION("Stream operators")
    {
        SECTION("Int8")
        {
            testPacketStreamOperators(sf::Int8(0));
            testPacketStreamOperators(sf::Int8(1));
            testPacketStreamOperators(std::numeric_limits<sf::Int8>::min());
            testPacketStreamOperators(std::numeric_limits<sf::Int8>::max());
        }

        SECTION("Int16")
        {
            testPacketStreamOperators(sf::Int16(0));
            testPacketStreamOperators(sf::Int16(1));
            testPacketStreamOperators(std::numeric_limits<sf::Int16>::min());
            testPacketStreamOperators(std::numeric_limits<sf::Int16>::max());
        }

        SECTION("Int32")
        {
            testPacketStreamOperators(sf::Int32(0));
            testPacketStreamOperators(sf::Int32(1));
            testPacketStreamOperators(std::numeric_limits<sf::Int32>::min());
            testPacketStreamOperators(std::numeric_limits<sf::Int32>::max());
        }

        SECTION("Int64")
        {
            testPacketStreamOperators(sf::Int64(0));
            testPacketStreamOperators(sf::Int64(1));
            testPacketStreamOperators(std::numeric_limits<sf::Int64>::min());
            testPacketStreamOperators(std::numeric_limits<sf::Int64>::max());
        }
    }
}
