#include <SFML/Network/Packet.hpp>

#include <catch2/catch_test_macros.hpp>

#include <array>
#include <limits>
#include <type_traits>

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
    SECTION("Type traits")
    {
        STATIC_CHECK(std::is_copy_constructible_v<sf::Packet>);
        STATIC_CHECK(std::is_copy_assignable_v<sf::Packet>);
        STATIC_CHECK(std::is_nothrow_move_constructible_v<sf::Packet>);
        STATIC_CHECK(std::is_nothrow_move_assignable_v<sf::Packet>);
    }

    SECTION("Default constructor")
    {
        const sf::Packet packet;
        CHECK(packet.getReadPosition() == 0);
        CHECK(packet.getData() == nullptr);
        CHECK(packet.getDataSize() == 0);
        CHECK(packet.endOfPacket());
        CHECK(static_cast<bool>(packet));
    }

    SECTION("Append and clear")
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

    SECTION("Stream operators")
    {
        SECTION("std::int8_t")
        {
            CHECK_PACKET_STREAM_OPERATORS(std::int8_t(0));
            CHECK_PACKET_STREAM_OPERATORS(std::int8_t(1));
            CHECK_PACKET_STREAM_OPERATORS(std::numeric_limits<std::int8_t>::min());
            CHECK_PACKET_STREAM_OPERATORS(std::numeric_limits<std::int8_t>::max());
        }

        SECTION("std::int16_t")
        {
            CHECK_PACKET_STREAM_OPERATORS(std::int16_t(0));
            CHECK_PACKET_STREAM_OPERATORS(std::int16_t(1));
            CHECK_PACKET_STREAM_OPERATORS(std::numeric_limits<std::int16_t>::min());
            CHECK_PACKET_STREAM_OPERATORS(std::numeric_limits<std::int16_t>::max());
        }

        SECTION("std::int32_t")
        {
            CHECK_PACKET_STREAM_OPERATORS(std::int32_t(0));
            CHECK_PACKET_STREAM_OPERATORS(std::int32_t(1));
            CHECK_PACKET_STREAM_OPERATORS(std::numeric_limits<std::int32_t>::min());
            CHECK_PACKET_STREAM_OPERATORS(std::numeric_limits<std::int32_t>::max());
        }

        SECTION("std::int64_t")
        {
            CHECK_PACKET_STREAM_OPERATORS(std::int64_t(0));
            CHECK_PACKET_STREAM_OPERATORS(std::int64_t(1));
            CHECK_PACKET_STREAM_OPERATORS(std::numeric_limits<std::int64_t>::min());
            CHECK_PACKET_STREAM_OPERATORS(std::numeric_limits<std::int64_t>::max());
        }
    }
}
