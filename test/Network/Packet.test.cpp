#include <SFML/Network/Packet.hpp>

#include <doctest/doctest.h>

#include <array>
#include <limits>
#include <type_traits>

static_assert(std::is_copy_constructible_v<sf::Packet>);
static_assert(std::is_copy_assignable_v<sf::Packet>);
static_assert(std::is_nothrow_move_constructible_v<sf::Packet>);
static_assert(std::is_nothrow_move_assignable_v<sf::Packet>);

#define CHECK_PACKET_STREAM_OPERATORS(expected)              \
    do                                                       \
    {                                                        \
        sf::Packet packet;                                   \
        packet << (expected);                                \
        CHECK(packet.getReadPosition() == 0);                \
        CHECK(packet.getData() != nullptr);                  \
        CHECK(packet.getDataSize() == sizeof(expected));     \
        CHECK(!packet.endOfPacket());                        \
        CHECK(static_cast<bool>(packet));                    \
                                                             \
        decltype(expected) received;                         \
        packet >> received;                                  \
        CHECK(packet.getReadPosition() == sizeof(expected)); \
        CHECK(packet.getData() != nullptr);                  \
        CHECK(packet.getDataSize() == sizeof(expected));     \
        CHECK(packet.endOfPacket());                         \
        CHECK(static_cast<bool>(packet));                    \
        CHECK((expected) == received);                       \
    } while (false)

TEST_CASE("[Network] sf::Packet")
{
    SUBCASE("Default constructor")
    {
        const sf::Packet packet;
        CHECK(packet.getReadPosition() == 0);
        CHECK(packet.getData() == nullptr);
        CHECK(packet.getDataSize() == 0);
        CHECK(packet.endOfPacket());
        CHECK(static_cast<bool>(packet));
    }

    SUBCASE("Append and clear")
    {
        constexpr std::array data = {1, 2, 3, 4, 5, 6};

        sf::Packet packet;
        packet.append(data.data(), data.size());
        CHECK(packet.getReadPosition() == 0);
        CHECK(packet.getData() != nullptr);
        CHECK(packet.getDataSize() == data.size());
        CHECK(!packet.endOfPacket());
        CHECK(static_cast<bool>(packet));

        packet.clear();
        CHECK(packet.getReadPosition() == 0);
        CHECK(packet.getData() == nullptr);
        CHECK(packet.getDataSize() == 0);
        CHECK(packet.endOfPacket());
        CHECK(static_cast<bool>(packet));
    }

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
