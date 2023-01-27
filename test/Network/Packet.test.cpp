#include <SFML/Network/Packet.hpp>

#include <catch2/catch_test_macros.hpp>

#include <array>
#include <limits>
#include <type_traits>
#include <vector>

#include <cstddef>

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

    SECTION("Network ordering")
    {
        sf::Packet packet;

        SECTION("16 bit int")
        {
            packet << std::uint16_t{12'345};
            const auto*       dataPtr = static_cast<const std::byte*>(packet.getData());
            const std::vector bytes(dataPtr, dataPtr + packet.getDataSize());
            const std::vector expectedBytes{std::byte{0x30}, std::byte{0x39}};
            CHECK(bytes == expectedBytes);
        }

        SECTION("32 bit int")
        {
            packet << std::uint32_t{1'234'567'890};
            const auto*       dataPtr = static_cast<const std::byte*>(packet.getData());
            const std::vector bytes(dataPtr, dataPtr + packet.getDataSize());
            const std::vector expectedBytes{std::byte{0x49}, std::byte{0x96}, std::byte{0x02}, std::byte{0xD2}};
            CHECK(bytes == expectedBytes);
        }

        SECTION("float")
        {
            packet << 123.456f;
            const auto*       dataPtr = static_cast<const std::byte*>(packet.getData());
            const std::vector bytes(dataPtr, dataPtr + packet.getDataSize());
            const std::vector expectedBytes{std::byte{0x79}, std::byte{0xe9}, std::byte{0xf6}, std::byte{0x42}};
            CHECK(bytes == expectedBytes);
        }

        SECTION("double")
        {
            packet << 789.123;
            const auto*       dataPtr = static_cast<const std::byte*>(packet.getData());
            const std::vector bytes(dataPtr, dataPtr + packet.getDataSize());
            const std::vector expectedBytes{std::byte{0x44},
                                            std::byte{0x8b},
                                            std::byte{0x6c},
                                            std::byte{0xe7},
                                            std::byte{0xfb},
                                            std::byte{0xa8},
                                            std::byte{0x88},
                                            std::byte{0x40}};
            CHECK(bytes == expectedBytes);
        }
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
