include(CMakeParseArguments)

# set the appropriate standard library on each platform for the given target
# ex: sfml_set_stdlib(sfml-system)
function(sfml_set_stdlib target)
    # for gcc >= 4.0 on Windows, apply the SFML_USE_STATIC_STD_LIBS option if it is enabled
    if(SFML_OS_WINDOWS AND SFML_COMPILER_GCC AND NOT SFML_GCC_VERSION VERSION_LESS "4")
        if(SFML_USE_STATIC_STD_LIBS AND NOT SFML_COMPILER_GCC_TDM)
            target_link_libraries(${target} "-static-libgcc" "-static-libstdc++")
        elseif(NOT SFML_USE_STATIC_STD_LIBS AND SFML_COMPILER_GCC_TDM)
            target_link_libraries(${target} "-shared-libgcc" "-shared-libstdc++")
        endif()
    endif()

    if (SFML_OS_MACOSX)
        if (${CMAKE_GENERATOR} MATCHES "Xcode")
            set_property(TARGET ${target} PROPERTY XCODE_ATTRIBUTE_CLANG_CXX_LIBRARY "libc++")
        else()
            target_compile_options(${target} PRIVATE "-stdlib=libc++")
            target_link_libraries(${target} "-stdlib=libc++")
        endif()
    endif()
endfunction()

# add a new target which is a SFML library
# ex: sfml_add_library(sfml-graphics
#                      SOURCES sprite.cpp image.cpp ...
#                      DEPENDS sfml-window sfml-system
#                      EXTERNAL_LIBS opengl freetype ...)
macro(sfml_add_library target)

    # parse the arguments
    cmake_parse_arguments(THIS "" "" "SOURCES;DEPENDS;EXTERNAL_LIBS" ${ARGN})

    # create the target
    add_library(${target} ${THIS_SOURCES})

    # define the export symbol of the module
    string(REPLACE "-" "_" NAME_UPPER "${target}")
    string(TOUPPER "${NAME_UPPER}" NAME_UPPER)
    set_target_properties(${target} PROPERTIES DEFINE_SYMBOL ${NAME_UPPER}_EXPORTS)

    # adjust the output file prefix/suffix to match our conventions
    if(BUILD_SHARED_LIBS)
        if(SFML_OS_WINDOWS)
            # include the major version number in Windows shared library names (but not import library names)
            set_target_properties(${target} PROPERTIES DEBUG_POSTFIX -d)
            set_target_properties(${target} PROPERTIES SUFFIX "-${VERSION_MAJOR}${CMAKE_SHARED_LIBRARY_SUFFIX}")
        else()
            set_target_properties(${target} PROPERTIES DEBUG_POSTFIX -d)
        endif()
        if (SFML_OS_WINDOWS AND SFML_COMPILER_GCC)
            # on Windows/gcc get rid of "lib" prefix for shared libraries,
            # and transform the ".dll.a" suffix into ".a" for import libraries
            set_target_properties(${target} PROPERTIES PREFIX "")
            set_target_properties(${target} PROPERTIES IMPORT_SUFFIX ".a")
        endif()
    else()
        set_target_properties(${target} PROPERTIES DEBUG_POSTFIX -s-d)
        set_target_properties(${target} PROPERTIES RELEASE_POSTFIX -s)
        set_target_properties(${target} PROPERTIES MINSIZEREL_POSTFIX -s)
        set_target_properties(${target} PROPERTIES RELWITHDEBINFO_POSTFIX -s)
    endif()

    # set the version and soversion of the target (for compatible systems -- mostly Linuxes)
    # except for Android which strips soversion suffixes
    if(NOT SFML_OS_ANDROID)
        set_target_properties(${target} PROPERTIES SOVERSION ${VERSION_MAJOR}.${VERSION_MINOR})
        set_target_properties(${target} PROPERTIES VERSION ${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH})
    endif()

    # set the target's folder (for IDEs that support it, e.g. Visual Studio)
    set_target_properties(${target} PROPERTIES FOLDER "SFML")

    # set the target flags to use the appropriate C++ standard library
    sfml_set_stdlib(${target})

    # For Visual Studio on Windows, export debug symbols (PDB files) to lib directory
    if(SFML_GENERATE_PDB)
        # PDB files are only generated in Debug and RelWithDebInfo configurations, find out which one
        if(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
            set(SFML_PDB_POSTFIX "-d")
        else()
            set(SFML_PDB_POSTFIX "")
        endif()

        if(BUILD_SHARED_LIBS)
            # DLLs export debug symbols in the linker PDB (the compiler PDB is an intermediate file)
            set_target_properties(${target} PROPERTIES
                                  PDB_NAME "${target}${SFML_PDB_POSTFIX}"
                                  PDB_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/lib")
        else()
            # Static libraries have no linker PDBs, thus the compiler PDBs are relevant
            set_target_properties(${target} PROPERTIES
                                  COMPILE_PDB_NAME "${target}-s${SFML_PDB_POSTFIX}"
                                  COMPILE_PDB_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/lib")
        endif()
    endif()

    # if using gcc >= 4.0 or clang >= 3.0 on a non-Windows platform, we must hide public symbols by default
    # (exported ones are explicitly marked)
    if(NOT SFML_OS_WINDOWS AND ((SFML_COMPILER_GCC AND NOT SFML_GCC_VERSION VERSION_LESS "4") OR (SFML_COMPILER_CLANG AND NOT SFML_CLANG_VERSION VERSION_LESS "3")))
        set_target_properties(${target} PROPERTIES COMPILE_FLAGS -fvisibility=hidden)
    endif()

    # link the target to its SFML dependencies
    if(THIS_DEPENDS)
        target_link_libraries(${target} ${THIS_DEPENDS})
    endif()

    # build frameworks or dylibs
    if(SFML_OS_MACOSX AND BUILD_SHARED_LIBS)
        if(SFML_BUILD_FRAMEWORKS)
            # adapt target to build frameworks instead of dylibs
            set_target_properties(${target} PROPERTIES
                                  FRAMEWORK TRUE
                                  FRAMEWORK_VERSION ${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}
                                  MACOSX_FRAMEWORK_IDENTIFIER org.sfml-dev.${target}
                                  MACOSX_FRAMEWORK_SHORT_VERSION_STRING ${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}
                                  MACOSX_FRAMEWORK_BUNDLE_VERSION ${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH})
        endif()

        # adapt install directory to allow distributing dylibs/frameworks in user's frameworks/application bundle
        # but only if cmake rpath options aren't set
        if(NOT CMAKE_SKIP_RPATH AND NOT CMAKE_SKIP_INSTALL_RPATH AND NOT CMAKE_INSTALL_RPATH AND NOT CMAKE_INSTALL_RPATH_USE_LINK_PATH AND NOT CMAKE_INSTALL_NAME_DIR)
            if(CMAKE_SKIP_BUILD_RPATH)
                set_target_properties(${target} PROPERTIES
                                      INSTALL_NAME_DIR "@rpath")
            else()
                set_target_properties(${target} PROPERTIES
                                      BUILD_WITH_INSTALL_RPATH 1
                                      INSTALL_NAME_DIR "@rpath")
            endif()
        endif()
    endif()

    # enable automatic reference counting on iOS
    if (SFML_OS_IOS)
        set_target_properties(${target} PROPERTIES XCODE_ATTRIBUTE_CLANG_ENABLE_OBJC_ARC YES)
    endif()

    # sfml-activity library is our bootstrap activity and must not depend on stlport_shared
    # (otherwise Android will fail to load it)
    if (SFML_OS_ANDROID)
        if (${target} MATCHES "sfml-activity")
            set_target_properties(${target} PROPERTIES COMPILE_FLAGS -fpermissive)
            set_target_properties(${target} PROPERTIES LINK_FLAGS "-landroid -llog")
            set(CMAKE_CXX_CREATE_SHARED_LIBRARY ${CMAKE_CXX_CREATE_SHARED_LIBRARY_WITHOUT_STL})
        else()
            set(CMAKE_CXX_CREATE_SHARED_LIBRARY ${CMAKE_CXX_CREATE_SHARED_LIBRARY_WITH_STL})
        endif()
    endif()

    # link the target to its external dependencies
    if(THIS_EXTERNAL_LIBS)
        target_link_libraries(${target} ${THIS_EXTERNAL_LIBS})
    endif()

    # add the install rule
    install(TARGETS ${target}
            RUNTIME DESTINATION bin COMPONENT bin
            LIBRARY DESTINATION lib${LIB_SUFFIX} COMPONENT bin
            ARCHIVE DESTINATION lib${LIB_SUFFIX} COMPONENT devel
            FRAMEWORK DESTINATION ${CMAKE_INSTALL_FRAMEWORK_PREFIX} COMPONENT bin)
endmacro()

# add a new target which is a SFML example
# ex: sfml_add_example(ftp
#                      SOURCES ftp.cpp ...
#                      BUNDLE_RESOURCES MainMenu.nib ...    # Files to be added in target but not installed next to the executable
#                      DEPENDS sfml-network sfml-system
#                      RESOURCES_DIR resources)             # A directory to install next to the executable and sources
macro(sfml_add_example target)

    # parse the arguments
    cmake_parse_arguments(THIS "GUI_APP" "RESOURCES_DIR" "SOURCES;BUNDLE_RESOURCES;DEPENDS" ${ARGN})

    # set a source group for the source files
    source_group("" FILES ${THIS_SOURCES})

    # check whether resources must be added in target
    set(target_input ${THIS_SOURCES})
    if(THIS_BUNDLE_RESOURCES)
        set(target_input ${target_input} ${THIS_BUNDLE_RESOURCES})
    endif()

    # create the target
    if(THIS_GUI_APP AND SFML_OS_WINDOWS AND NOT DEFINED CMAKE_CONFIGURATION_TYPES AND ${CMAKE_BUILD_TYPE} STREQUAL "Release")
        add_executable(${target} WIN32 ${target_input})
        target_link_libraries(${target} sfml-main)
    else()
        add_executable(${target} ${target_input})
    endif()

    # set the debug suffix
    set_target_properties(${target} PROPERTIES DEBUG_POSTFIX -d)

    # set the target's folder (for IDEs that support it, e.g. Visual Studio)
    set_target_properties(${target} PROPERTIES FOLDER "Examples")

    # set the target flags to use the appropriate C++ standard library
    sfml_set_stdlib(${target})

    # link the target to its SFML dependencies
    if(THIS_DEPENDS)
        target_link_libraries(${target} ${THIS_DEPENDS})
    endif()

    # add the install rule
    install(TARGETS ${target}
            RUNTIME DESTINATION ${INSTALL_MISC_DIR}/examples/${target} COMPONENT examples
            BUNDLE DESTINATION ${INSTALL_MISC_DIR}/examples/${target} COMPONENT examples)

    # install the example's source code
    install(FILES ${THIS_SOURCES}
            DESTINATION ${INSTALL_MISC_DIR}/examples/${target}
            COMPONENT examples)

    if (THIS_RESOURCES_DIR)
        # install the example's resources as well
        get_filename_component(THIS_RESOURCES_DIR "${THIS_RESOURCES_DIR}" ABSOLUTE)

        if(NOT EXISTS "${THIS_RESOURCES_DIR}")
            message(FATAL_ERROR "Given resources directory to install does not exist: ${THIS_RESOURCES_DIR}")
        endif()
        install(DIRECTORY ${THIS_RESOURCES_DIR}
                DESTINATION ${INSTALL_MISC_DIR}/examples/${target}
                COMPONENT examples)
    endif()

endmacro()
