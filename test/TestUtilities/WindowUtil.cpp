// Note: No need to increase compile time by including TestUtilities/Window.hpp
#include <SFML/Window/VideoMode.hpp>

#include <ostream>

std::string runDisplayTests()
{
#ifdef SFML_RUN_DISPLAY_TESTS
    return "";
#else
    // https://github.com/catchorg/Catch2/blob/devel/docs/test-cases-and-sections.md#special-tags
    // This tag tells Catch2 to not run a given TEST_CASE
    return "[.display]";
#endif
}

namespace sf
{
std::ostream& operator<<(std::ostream& os, const VideoMode& videoMode)
{
    return os << videoMode.size.x << "x" << videoMode.size.y << "x" << videoMode.bitsPerPixel;
}
} // namespace sf
