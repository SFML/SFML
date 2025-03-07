list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR})

include(CMakeFindDependencyMacro)

# start with an empty list
set(FIND_SFML_DEPENDENCIES_NOTFOUND)

find_dependency(Threads)

if(FIND_SFML_DEPENDENCIES_NOTFOUND)
    set(FIND_SFML_ERROR "SFML found but some of its dependencies are missing (${FIND_SFML_DEPENDENCIES_NOTFOUND})")
    set(SFML_FOUND OFF)
endif()
