#include <SFML/Network/Socket.hpp>

#include <catch2/catch_test_macros.hpp>

#include <type_traits>

class TestSocket : public sf::Socket
{
public:
    TestSocket() : sf::Socket(sf::Socket::Type::Udp)
    {
    }

    using sf::Socket::close;
    using sf::Socket::create;
    using sf::Socket::getNativeHandle;
};

TEST_CASE("[Network] sf::Socket")
{
    SECTION("Type traits")
    {
        STATIC_CHECK(!std::is_constructible_v<sf::Socket>);
        STATIC_CHECK(!std::is_copy_constructible_v<sf::Socket>);
        STATIC_CHECK(!std::is_copy_assignable_v<sf::Socket>);
        STATIC_CHECK(std::is_nothrow_move_constructible_v<sf::Socket>);
        STATIC_CHECK(std::is_nothrow_move_assignable_v<sf::Socket>);
    }

    SECTION("Constants")
    {
        STATIC_CHECK(sf::Socket::AnyPort == 0);
    }

    const auto invalidHandle = static_cast<sf::SocketHandle>(-1);

    SECTION("Construction")
    {
        const TestSocket testSocket;
        CHECK(testSocket.isBlocking());
        CHECK(testSocket.getNativeHandle() == invalidHandle);
    }

    SECTION("Move semantics")
    {
        SECTION("Construction")
        {
            TestSocket movedTestSocket;
            movedTestSocket.setBlocking(false);
            movedTestSocket.create();
            const TestSocket testSocket(std::move(movedTestSocket));
            CHECK(!testSocket.isBlocking());
            CHECK(testSocket.getNativeHandle() != invalidHandle);
        }

        SECTION("Assignment")
        {
            TestSocket movedTestSocket;
            movedTestSocket.setBlocking(false);
            movedTestSocket.create();
            TestSocket testSocket;
            testSocket = std::move(movedTestSocket);
            CHECK(!testSocket.isBlocking());
            CHECK(testSocket.getNativeHandle() != invalidHandle);
        }
    }

    SECTION("Set/get blocking")
    {
        TestSocket testSocket;
        testSocket.setBlocking(false);
        CHECK(!testSocket.isBlocking());
    }

    SECTION("create()")
    {
        TestSocket testSocket;
        testSocket.create();
        CHECK(testSocket.isBlocking());
        CHECK(testSocket.getNativeHandle() != invalidHandle);

        // Recreate socket to ensure nothing changed
        testSocket.create();
        CHECK(testSocket.isBlocking());
        CHECK(testSocket.getNativeHandle() != invalidHandle);
    }

    SECTION("close()")
    {
        TestSocket testSocket;
        testSocket.create();
        CHECK(testSocket.isBlocking());
        CHECK(testSocket.getNativeHandle() != invalidHandle);
        testSocket.close();
        CHECK(testSocket.isBlocking());
        CHECK(testSocket.getNativeHandle() == invalidHandle);

        // Reclose socket to ensure nothing changed
        testSocket.close();
        CHECK(testSocket.isBlocking());
        CHECK(testSocket.getNativeHandle() == invalidHandle);
    }
}
