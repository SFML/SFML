# Get executable
if(NOT CLANG_FORMAT_EXECUTABLE)
    set(CLANG_FORMAT_EXECUTABLE clang-format)
endif()

# Check executable exists
if(NOT EXISTS ${CLANG_FORMAT_EXECUTABLE})
    find_program(CLANG_FORMAT_EXEC_TEMP ${CLANG_FORMAT_EXECUTABLE})
    if(CLANG_FORMAT_EXEC_TEMP)
        set(CLANG_FORMAT_EXECUTABLE ${CLANG_FORMAT_EXEC_TEMP})
        unset(CLANG_FORMAT_EXEC_TEMP)
    else()
        message(FATAL_ERROR "Unable to find clang-format executable: \"${CLANG_FORMAT_EXECUTABLE}\"")
    endif()
endif()

# Check executable version
execute_process(COMMAND ${CLANG_FORMAT_EXECUTABLE} --version OUTPUT_VARIABLE CLANG_FORMAT_VERSION)
string(REGEX MATCH "clang-format version ([0-9]+)" CLANG_FORMAT_VERSION ${CLANG_FORMAT_VERSION})
unset(CLANG_FORMAT_VERSION)
if(CMAKE_MATCH_1 GREATER_EQUAL 12)
    message(STATUS "Using clang-format version ${CMAKE_MATCH_1}")
else()
    message(FATAL_ERROR "clang-format version ${CMAKE_MATCH_1} is too low")
endif()

# Run
set(SOURCES "")
foreach(FOLDER IN ITEMS examples include src test tools)
    file(GLOB_RECURSE folder_files "${FOLDER}/*.hpp" "${FOLDER}/*.inl" "${FOLDER}/*.cpp")
    list(APPEND SOURCES ${folder_files})
endforeach()

execute_process(COMMAND ${CLANG_FORMAT_EXECUTABLE} -i ${SOURCES})