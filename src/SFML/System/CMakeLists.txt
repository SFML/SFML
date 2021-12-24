
set(INCROOT ${PROJECT_SOURCE_DIR}/include/SFML/System)
set(SRCROOT ${PROJECT_SOURCE_DIR}/src/SFML/System)

# all source files
set(SRC
    ${SRCROOT}/Clock.cpp
    ${INCROOT}/Clock.hpp
    ${SRCROOT}/Err.cpp
    ${INCROOT}/Err.hpp
    ${INCROOT}/Export.hpp
    ${INCROOT}/InputStream.hpp
    ${INCROOT}/NativeActivity.hpp
    ${SRCROOT}/Sleep.cpp
    ${INCROOT}/Sleep.hpp
    ${SRCROOT}/String.cpp
    ${INCROOT}/String.hpp
    ${INCROOT}/String.inl
    ${SRCROOT}/Time.cpp
    ${INCROOT}/Time.hpp
    ${INCROOT}/Time.inl
    ${INCROOT}/Utf.hpp
    ${INCROOT}/Utf.inl
    ${INCROOT}/Vector2.hpp
    ${INCROOT}/Vector2.inl
    ${INCROOT}/Vector3.hpp
    ${INCROOT}/Vector3.inl
    ${SRCROOT}/FileInputStream.cpp
    ${INCROOT}/FileInputStream.hpp
    ${SRCROOT}/MemoryInputStream.cpp
    ${INCROOT}/MemoryInputStream.hpp
    ${INCROOT}/SuspendAwareClock.hpp
)
source_group("" FILES ${SRC})

# add platform specific sources
if(SFML_OS_ANDROID)
    set(PLATFORM_SRC ${PLATFORM_SRC}
        ${SRCROOT}/Android/Activity.hpp
        ${SRCROOT}/Android/Activity.cpp
        ${SRCROOT}/Android/NativeActivity.cpp
        ${SRCROOT}/Android/ResourceStream.cpp
        ${SRCROOT}/Android/ResourceStream.cpp
        ${SRCROOT}/Android/SuspendAwareClock.cpp
    )
endif()
source_group("unix" FILES ${PLATFORM_SRC})

# define the sfml-system target
sfml_add_library(sfml-system
                 SOURCES ${SRC} ${PLATFORM_SRC})

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
