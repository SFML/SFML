# define the sfml-system target
sfml_add_library(System)

# all source files
target_sources(sfml-system PRIVATE
    Clock.cpp
    Err.cpp
    Sleep.cpp
    String.cpp
    FileInputStream.cpp
    MemoryInputStream.cpp
)

# add platform specific sources
if(SFML_OS_WINDOWS)
    target_sources(sfml-system PRIVATE Win32/SleepImpl.cpp)
else()
    target_sources(sfml-system PRIVATE Unix/SleepImpl.cpp)

    if(SFML_OS_ANDROID)
        target_sources(sfml-system PRIVATE
            Android/Activity.cpp
            Android/NativeActivity.cpp
            Android/ResourceStream.cpp
            Android/ResourceStream.cpp
            Android/SuspendAwareClock.cpp
        )
    endif()
endif()

if(SFML_OS_ANDROID)
    # glad sources
    target_include_directories(sfml-system PRIVATE "${PROJECT_SOURCE_DIR}/extlibs/headers/glad/include")
endif()

# setup dependencies
if(SFML_OS_LINUX OR SFML_OS_FREEBSD OR SFML_OS_MACOSX)
    target_link_libraries(sfml-system PRIVATE pthread)
endif()

if(SFML_OS_LINUX)
    target_link_libraries(sfml-system PRIVATE rt)
elseif(SFML_OS_WINDOWS)
    target_link_libraries(sfml-system PRIVATE winmm)
elseif(SFML_OS_ANDROID)
    target_link_libraries(sfml-system PRIVATE android log)
endif()
