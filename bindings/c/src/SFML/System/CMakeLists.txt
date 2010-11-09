
set(INCROOT ${CMAKE_SOURCE_DIR}/include/SFML/System)
set(SRCROOT ${CMAKE_SOURCE_DIR}/src/SFML/System)

# all source files
set(SRC
    ${SRCROOT}/Clock.cpp
    ${SRCROOT}/ClockStruct.h
    ${INCROOT}/Clock.h
    ${SRCROOT}/Mutex.cpp
    ${SRCROOT}/MutexStruct.h
    ${INCROOT}/Mutex.h
    ${SRCROOT}/Randomizer.cpp
    ${INCROOT}/Randomizer.h
    ${SRCROOT}/Sleep.cpp
    ${INCROOT}/Sleep.h
    ${SRCROOT}/Thread.cpp
    ${SRCROOT}/ThreadStruct.h
    ${INCROOT}/Thread.h
    ${INCROOT}/Types.h
)

# define the csfml-system target
csfml_add_library(csfml-system
                  SOURCES ${SRC}
                  DEPENDS optimized ${SFML_SYSTEM_LIBRARY} debug ${SFML_SYSTEM_LIBRARY_DEBUG})
