#include <AudioUtil.hpp>

std::string runAudioDeviceTests()
{
#ifdef SFML_RUN_AUDIO_DEVICE_TESTS
    return "";
#else
    // https://github.com/catchorg/Catch2/blob/devel/docs/test-cases-and-sections.md#special-tags
    // This tag tells Catch2 to not run a given TEST_CASE
    return "[.audio_device]";
#endif
}
