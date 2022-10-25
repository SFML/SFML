# Check executable exists
if(NOT EXISTS ${CLANG_TIDY_EXECUTABLE})
    find_program(CLANG_TIDY_EXEC_TEMP ${CLANG_TIDY_EXECUTABLE})
    if(CLANG_TIDY_EXEC_TEMP)
        set(CLANG_TIDY_EXECUTABLE ${CLANG_TIDY_EXEC_TEMP})
        unset(CLANG_TIDY_EXEC_TEMP)
    else()
        message(FATAL_ERROR "Unable to find clang-tidy executable: \"${CLANG_TIDY_EXECUTABLE}\"")
    endif()
endif()

# Check executable version
execute_process(COMMAND ${CLANG_TIDY_EXECUTABLE} --version OUTPUT_VARIABLE CLANG_TIDY_VERSION)
string(REGEX MATCH "version ([0-9]+)" CLANG_TIDY_VERSION ${CLANG_TIDY_VERSION})
unset(CLANG_TIDY_VERSION)
if(CMAKE_MATCH_1 GREATER_EQUAL 12)
    message(STATUS "Using clang-tidy version ${CMAKE_MATCH_1}")
else()
    message(FATAL_ERROR "clang-tidy version ${CMAKE_MATCH_1} is too low")
endif()

# Run
execute_process(COMMAND run-clang-tidy -clang-tidy-binary ${CLANG_TIDY_EXECUTABLE} -p ${PROJECT_BINARY_DIR})
