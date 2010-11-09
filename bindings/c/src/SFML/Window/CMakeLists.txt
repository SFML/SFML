
set(INCROOT ${CMAKE_SOURCE_DIR}/include/SFML/Window)
set(SRCROOT ${CMAKE_SOURCE_DIR}/src/SFML/Window)

# all source files
set(SRC
    ${SRCROOT}/Context.cpp
    ${SRCROOT}/ContextStruct.h
    ${INCROOT}/Context.h
    ${INCROOT}/Event.h
    ${SRCROOT}/Input.cpp
    ${SRCROOT}/InputStruct.h
    ${INCROOT}/Input.h
    ${INCROOT}/Types.h
    ${SRCROOT}/VideoMode.cpp
    ${INCROOT}/VideoMode.h
    ${SRCROOT}/Window.cpp
    ${SRCROOT}/WindowStruct.h
    ${INCROOT}/Window.h
    ${INCROOT}/WindowHandle.h
)

# define the csfml-window target
csfml_add_library(csfml-window
                  SOURCES ${SRC}
                  DEPENDS optimized ${SFML_WINDOW_LIBRARY} debug ${SFML_WINDOW_LIBRARY_DEBUG}
                          optimized ${SFML_SYSTEM_LIBRARY} debug ${SFML_SYSTEM_LIBRARY_DEBUG})
