
set(INCROOT ${PROJECT_SOURCE_DIR}/include/SFML/System)
set(SRCROOT ${PROJECT_SOURCE_DIR}/src/SFML/System)

# all source files
set(SRC
    ${SRCROOT}/Clock.cpp
    ${INCROOT}/Clock.hpp
    ${SRCROOT}/Err.cpp
    ${INCROOT}/Err.hpp
    ${INCROOT}/Export.hpp
    ${SRCROOT}/Lock.cpp
    ${INCROOT}/Lock.hpp
    ${SRCROOT}/Mutex.cpp
    ${INCROOT}/Mutex.hpp
    ${INCROOT}/NonCopyable.hpp
    ${SRCROOT}/Sleep.cpp
    ${INCROOT}/Sleep.hpp
    ${SRCROOT}/String.cpp
    ${INCROOT}/String.hpp
    ${SRCROOT}/Thread.cpp
    ${INCROOT}/Thread.hpp
    ${INCROOT}/Thread.inl
    ${SRCROOT}/ThreadLocal.cpp
    ${INCROOT}/ThreadLocal.hpp
    ${INCROOT}/ThreadLocalPtr.hpp
    ${INCROOT}/ThreadLocalPtr.inl
    ${SRCROOT}/Time.cpp
    ${INCROOT}/Time.hpp
    ${INCROOT}/Utf.hpp
    ${INCROOT}/Utf.inl
    ${INCROOT}/Vector2.hpp
    ${INCROOT}/Vector2.inl
    ${INCROOT}/Vector3.hpp
    ${INCROOT}/Vector3.inl
)

# add platform specific sources
if(WINDOWS)
    set(SRC
        ${SRC}
        ${SRCROOT}/Win32/ClockImpl.cpp
        ${SRCROOT}/Win32/ClockImpl.hpp
        ${SRCROOT}/Win32/MutexImpl.cpp
        ${SRCROOT}/Win32/MutexImpl.hpp
        ${SRCROOT}/Win32/SleepImpl.cpp
        ${SRCROOT}/Win32/SleepImpl.hpp
        ${SRCROOT}/Win32/ThreadImpl.cpp
        ${SRCROOT}/Win32/ThreadImpl.hpp
        ${SRCROOT}/Win32/ThreadLocalImpl.cpp
        ${SRCROOT}/Win32/ThreadLocalImpl.hpp
    )
else()
    set(SRC
        ${SRC}
        ${SRCROOT}/Unix/ClockImpl.cpp
        ${SRCROOT}/Unix/ClockImpl.hpp
        ${SRCROOT}/Unix/MutexImpl.cpp
        ${SRCROOT}/Unix/MutexImpl.hpp
        ${SRCROOT}/Unix/SleepImpl.cpp
        ${SRCROOT}/Unix/SleepImpl.hpp
        ${SRCROOT}/Unix/ThreadImpl.cpp
        ${SRCROOT}/Unix/ThreadImpl.hpp
        ${SRCROOT}/Unix/ThreadLocalImpl.cpp
        ${SRCROOT}/Unix/ThreadLocalImpl.hpp
    )
endif()

# build the list of external libraries to link
set(SYSTEM_EXT_LIBS)
if(UNIX)
    set(SYSTEM_EXT_LIBS ${SYSTEM_EXT_LIBS} pthread)
endif()
if(LINUX)
    set(SYSTEM_EXT_LIBS ${SYSTEM_EXT_LIBS} rt)
endif()

# define the sfml-system target
sfml_add_library(sfml-system
                 SOURCES ${SRC}
                 EXTERNAL_LIBS ${SYSTEM_EXT_LIBS})
