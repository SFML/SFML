#include <SFML/System/Err.hpp>

#include <catch2/catch_test_macros.hpp>

#include <sstream>

TEST_CASE("[System] sf::err")
{
    SECTION("Overflow default buffer")
    {
        // No assertion macros in this section since nothing about this can be directly observed.
        // Intention is to ensure DefaultErrStreamBuf::overflow gets called.
        sf::err() << "SFML is a simple, fast, cross-platform and object-oriented multimedia API."
                     "It provides access to windowing, graphics, audio and network."
                     "It is written in C++, and has bindings for various languages such as C, .Net, Ruby, Python.";
    }

    SECTION("Redirect buffer to observe contents")
    {
        sf::err() << "We'll never be able to observe this" << std::endl; // Ensure buffer is flushed
        auto* const defaultStreamBuffer = sf::err().rdbuf();
        CHECK(defaultStreamBuffer != nullptr);

        const std::stringstream stream;
        sf::err().rdbuf(stream.rdbuf());
        sf::err() << "Something went wrong!\n";
        CHECK(stream.str() == "Something went wrong!\n");

        sf::err().rdbuf(nullptr);
        sf::err() << "Sent to the abyss";
        CHECK(stream.str() == "Something went wrong!\n");

        sf::err().rdbuf(stream.rdbuf());
        sf::err() << "Back to the stringstream :)\n";
        CHECK(stream.str() == "Something went wrong!\nBack to the stringstream :)\n");

        // Restore sf::err to default stream defaultStreamBuffer
        sf::err().rdbuf(defaultStreamBuffer);
        CHECK(sf::err().rdbuf() == defaultStreamBuffer);
    }
}
