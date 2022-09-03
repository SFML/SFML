#include <SFML/Network/Packet.hpp>

#include <doctest/doctest.h>

#include <limits>

#define CHECK_PACKET_STREAM_OPERATORS(expected) \
    do                                          \
    {                                           \
        sf::Packet packet;                      \
        packet << expected;                     \
        decltype(expected) received;            \
        packet >> received;                     \
        CHECK(expected == received);            \
    } while (false)

TEST_CASE("sf::Packet class - [network]")
{
    SUBCASE("Stream operators")
    {
        SUBCASE("std::int8_t")
        {
            CHECK_PACKET_STREAM_OPERATORS(std::int8_t(0));
            CHECK_PACKET_STREAM_OPERATORS(std::int8_t(1));
            CHECK_PACKET_STREAM_OPERATORS(std::numeric_limits<std::int8_t>::min());
            CHECK_PACKET_STREAM_OPERATORS(std::numeric_limits<std::int8_t>::max());
        }

        SUBCASE("std::int16_t")
        {
            CHECK_PACKET_STREAM_OPERATORS(std::int16_t(0));
            CHECK_PACKET_STREAM_OPERATORS(std::int16_t(1));
            CHECK_PACKET_STREAM_OPERATORS(std::numeric_limits<std::int16_t>::min());
            CHECK_PACKET_STREAM_OPERATORS(std::numeric_limits<std::int16_t>::max());
        }

        SUBCASE("std::int32_t")
        {
            CHECK_PACKET_STREAM_OPERATORS(std::int32_t(0));
            CHECK_PACKET_STREAM_OPERATORS(std::int32_t(1));
            CHECK_PACKET_STREAM_OPERATORS(std::numeric_limits<std::int32_t>::min());
            CHECK_PACKET_STREAM_OPERATORS(std::numeric_limits<std::int32_t>::max());
        }

        SUBCASE("std::int64_t")
        {
            CHECK_PACKET_STREAM_OPERATORS(std::int64_t(0));
            CHECK_PACKET_STREAM_OPERATORS(std::int64_t(1));
            CHECK_PACKET_STREAM_OPERATORS(std::numeric_limits<std::int64_t>::min());
            CHECK_PACKET_STREAM_OPERATORS(std::numeric_limits<std::int64_t>::max());
        }
    }
}
