set(SRCROOT "${PROJECT_SOURCE_DIR}/test/src")

SET(SRC
    "${SRCROOT}/Main.cpp"
    "${SRCROOT}/UnitTests.cpp"
    "${SRCROOT}/Vector2.cpp"
    "${SRCROOT}/Rect.cpp"
)

include_directories("${PROJECT_SOURCE_DIR}/include")
include_directories("${PROJECT_SOURCE_DIR}/extlibs/headers")

# Using sfmltest instead of test because the latter is reserved.
add_executable(sfmltest ${SRC})

target_link_libraries(sfmltest sfml-graphics sfml-window sfml-audio sfml-network sfml-system)

add_custom_target(runtests ALL
    DEPENDS sfmltest
    WORKING_DIRECTORY "${PROJECT_BINARY_DIR}/test"
    COMMAND ${CMAKE_COMMAND} -E echo "Running test suite..."
    COMMAND sfmltest
)
