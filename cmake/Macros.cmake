include(CMakeParseArguments)

# include the compiler warnings helpers
include(${CMAKE_CURRENT_LIST_DIR}/CompilerWarnings.cmake)

# This little macro lets you set any Xcode specific property
macro (sfml_set_xcode_property TARGET XCODE_PROPERTY XCODE_VALUE)
    set_property (TARGET ${TARGET} PROPERTY XCODE_ATTRIBUTE_${XCODE_PROPERTY} ${XCODE_VALUE})
endmacro ()

# set the appropriate standard library on each platform for the given target
# example: sfml_set_stdlib(sfml-system)
function(sfml_set_stdlib target)
    # for gcc on Windows, apply the SFML_USE_STATIC_STD_LIBS option if it is enabled
    if(SFML_OS_WINDOWS AND SFML_COMPILER_GCC)
        if(SFML_USE_STATIC_STD_LIBS AND NOT SFML_COMPILER_GCC_TDM)
            target_link_libraries(${target} PRIVATE "-static-libgcc" "-static-libstdc++")
        elseif(NOT SFML_USE_STATIC_STD_LIBS AND SFML_COMPILER_GCC_TDM)
            target_link_libraries(${target} PRIVATE "-shared-libgcc" "-shared-libstdc++")
        endif()
    endif()

    if (SFML_OS_MACOSX)
        if (${CMAKE_GENERATOR} MATCHES "Xcode")
            sfml_set_xcode_property(${target} CLANG_CXX_LIBRARY "libc++")
        elseif(SFML_COMPILER_CLANG)
            target_compile_options(${target} PRIVATE "-stdlib=libc++")
            target_link_libraries(${target} PRIVATE "-stdlib=libc++")
        else()
            message(FATAL_ERROR "Clang is the only supported compiler on macOS")
        endif()
    endif()
endfunction()

function(sfml_set_common_ios_properties target)
    # enable automatic reference counting on iOS
    sfml_set_xcode_property(${target} CLANG_ENABLE_OBJC_ARC YES)
    sfml_set_xcode_property(${target} IPHONEOS_DEPLOYMENT_TARGET "${SFML_IOS_DEPLOYMENT_TARGET}")
    sfml_set_xcode_property(${target} CODE_SIGN_IDENTITY "${SFML_CODE_SIGN_IDENTITY}")

    get_target_property(target_type ${target} TYPE)
    if (target_type STREQUAL "EXECUTABLE")
        set_target_properties(${target} PROPERTIES
            MACOSX_BUNDLE TRUE # Bare executables are not usable on iOS, only bundle applications
            MACOSX_BUNDLE_GUI_IDENTIFIER "org.sfml-dev.${target}" # If missing, trying to launch an example in simulator will make Xcode < 9.3 crash
            MACOSX_BUNDLE_BUNDLE_NAME "${target}"
            MACOSX_BUNDLE_LONG_VERSION_STRING "${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}"
        )
    endif()
endfunction()

# add a new target which is a SFML library
# example: sfml_add_library(sfml-graphics
#                           SOURCES sprite.cpp image.cpp ...
#                           [STATIC]) # Always create a static library and ignore BUILD_SHARED_LIBS
macro(sfml_add_library target)

    # parse the arguments
    cmake_parse_arguments(THIS "STATIC" "" "SOURCES" ${ARGN})
    if (NOT "${THIS_UNPARSED_ARGUMENTS}" STREQUAL "")
        message(FATAL_ERROR "Extra unparsed arguments when calling sfml_add_library: ${THIS_UNPARSED_ARGUMENTS}")
    endif()

    # create the target
    if (THIS_STATIC)
        add_library(${target} STATIC ${THIS_SOURCES})
    else()
        add_library(${target} ${THIS_SOURCES})
    endif()

    # enable C++17 support
    target_compile_features(${target} PUBLIC cxx_std_17)

    set_file_warnings(${THIS_SOURCES})

    # define the export symbol of the module
    string(REPLACE "-" "_" NAME_UPPER "${target}")
    string(TOUPPER "${NAME_UPPER}" NAME_UPPER)
    set_target_properties(${target} PROPERTIES DEFINE_SYMBOL ${NAME_UPPER}_EXPORTS)

    # adjust the output file prefix/suffix to match our conventions
    if(BUILD_SHARED_LIBS AND NOT THIS_STATIC)
        if(SFML_OS_WINDOWS)
            # include the major version number in Windows shared library names (but not import library names)
            set_target_properties(${target} PROPERTIES DEBUG_POSTFIX -d)
            set_target_properties(${target} PROPERTIES SUFFIX "-${VERSION_MAJOR}${CMAKE_SHARED_LIBRARY_SUFFIX}")

            # fill out all variables we use to generate the .rc file
            string(TIMESTAMP RC_CURRENT_YEAR "%Y")
            string(REGEX REPLACE "sfml-([a-z])([a-z]*)" "\\1" RC_MODULE_NAME_HEAD "${target}")
            string(REGEX REPLACE "sfml-([a-z])([a-z]*)" "\\2" RC_MODULE_NAME_TAIL "${target}")
            string(TOUPPER "${RC_MODULE_NAME_HEAD}" RC_MODULE_NAME_HEAD)
            set(RC_MODULE_NAME "${RC_MODULE_NAME_HEAD}${RC_MODULE_NAME_TAIL}")
            set(RC_VERSION_SUFFIX "") # Add something like the git revision short SHA-1 in the future
            set(RC_PRERELEASE "0") # Set to 1 to mark the DLL as a pre-release DLL
            set(RC_TARGET_NAME "${target}")
            set(RC_TARGET_FILE_NAME_SUFFIX "-${VERSION_MAJOR}${CMAKE_SHARED_LIBRARY_SUFFIX}")

            # generate the .rc file
            configure_file(
                "${SFML_SOURCE_DIR}/tools/windows/resource.rc.in"
                "${CMAKE_CURRENT_BINARY_DIR}/${target}.rc"
                @ONLY
            )
            target_sources(${target} PRIVATE "${CMAKE_CURRENT_BINARY_DIR}/${target}.rc")
            source_group("" FILES "${CMAKE_CURRENT_BINARY_DIR}/${target}.rc")
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

        if(BUILD_SHARED_LIBS AND NOT THIS_STATIC)
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

    # if using gcc or clang on a non-Windows platform, we must hide public symbols by default
    # (exported ones are explicitly marked)
    if(NOT SFML_OS_WINDOWS AND (SFML_COMPILER_GCC OR SFML_COMPILER_CLANG))
        set_target_properties(${target} PROPERTIES COMPILE_FLAGS -fvisibility=hidden)
    endif()

    # build frameworks or dylibs
    if(SFML_OS_MACOSX AND BUILD_SHARED_LIBS AND NOT THIS_STATIC)
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
            set_target_properties(${target} PROPERTIES INSTALL_NAME_DIR "@rpath")
            if(NOT CMAKE_SKIP_BUILD_RPATH)
                if (CMAKE_VERSION VERSION_LESS 3.9)
                    set_target_properties(${target} PROPERTIES BUILD_WITH_INSTALL_RPATH TRUE)
                else()
                    set_target_properties(${target} PROPERTIES BUILD_WITH_INSTALL_NAME_DIR TRUE)
                endif()
            endif()
        endif()
    endif()

    if (SFML_OS_IOS)
        sfml_set_common_ios_properties(${target})
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

    # add the install rule
    install(TARGETS ${target} EXPORT SFMLConfigExport
            RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR} COMPONENT bin
            LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR} COMPONENT bin
            ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR} COMPONENT devel
            FRAMEWORK DESTINATION "." COMPONENT bin)

    # add <project>/include as public include directory
    target_include_directories(${target}
                               PUBLIC $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
                               PRIVATE ${PROJECT_SOURCE_DIR}/src)

    if (SFML_BUILD_FRAMEWORKS)
        target_include_directories(${target} INTERFACE $<INSTALL_INTERFACE:SFML.framework>)
    else()
        target_include_directories(${target} INTERFACE $<INSTALL_INTERFACE:include>)
    endif()

    # define SFML_STATIC if the build type is not set to 'shared'
    if(NOT BUILD_SHARED_LIBS)
        target_compile_definitions(${target} PUBLIC "SFML_STATIC")
    endif()

endmacro()

# add a new target which is a SFML example
# example: sfml_add_example(ftp
#                           SOURCES ftp.cpp ...
#                           BUNDLE_RESOURCES MainMenu.nib ...    # Files to be added in target but not installed next to the executable
#                           DEPENDS sfml-network
#                           RESOURCES_DIR resources)             # A directory to install next to the executable and sources
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
        target_link_libraries(${target} PRIVATE sfml-main)
    elseif(THIS_GUI_APP AND SFML_OS_IOS)

        # For iOS apps we need the launch screen storyboard,
        # and a custom info.plist to use it
        SET(LAUNCH_SCREEN "${CMAKE_SOURCE_DIR}/examples/assets/LaunchScreen.storyboard")
        SET(LOGO "${CMAKE_SOURCE_DIR}/examples/assets/logo.png")
        SET(INFO_PLIST "${CMAKE_SOURCE_DIR}/examples/assets/info.plist")
        SET(ICONS "${CMAKE_SOURCE_DIR}/examples/assets/icon.icns")
        add_executable(${target} MACOSX_BUNDLE ${target_input} ${LAUNCH_SCREEN} ${LOGO} ${ICONS})
        set(RESOURCES ${LAUNCH_SCREEN} ${LOGO} ${ICONS})
        set_target_properties(${target} PROPERTIES RESOURCE "${RESOURCES}"
                                                   MACOSX_BUNDLE_INFO_PLIST ${INFO_PLIST}
                                                   MACOSX_BUNDLE_ICON_FILE icon.icns)
        target_link_libraries(${target} PRIVATE sfml-main)
    else()
        add_executable(${target} ${target_input})
    endif()

    set_file_warnings(${target_input})

    # set the debug suffix
    set_target_properties(${target} PROPERTIES DEBUG_POSTFIX -d)

    # set the target's folder (for IDEs that support it, e.g. Visual Studio)
    set_target_properties(${target} PROPERTIES FOLDER "Examples")

    # set the target flags to use the appropriate C++ standard library
    sfml_set_stdlib(${target})

    # set the Visual Studio startup path for debugging
    set_target_properties(${target} PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

    # link the target to its SFML dependencies
    if(THIS_DEPENDS)
        target_link_libraries(${target} PRIVATE ${THIS_DEPENDS})
    endif()

    if (SFML_OS_IOS)
        sfml_set_common_ios_properties(${target})
    endif()

endmacro()

# add a new target which is a SFML test
# example: sfml_add_test(sfml-test
#                           ftp.cpp ...
#                           sfml-network)
function(sfml_add_test target SOURCES DEPENDS)

    # set a source group for the source files
    source_group("" FILES ${SOURCES})

    # create the target
    add_executable(${target} ${SOURCES})

    # set the target's folder (for IDEs that support it, e.g. Visual Studio)
    set_target_properties(${target} PROPERTIES FOLDER "Tests")

    # link the target to its SFML dependencies
    target_link_libraries(${target} PRIVATE ${DEPENDS})

    # Add the test
    add_test(${target} ${target})

    # If building shared libs on windows we must copy the dependencies into the folder
    if (WIN32 AND BUILD_SHARED_LIBS)
        foreach (DEPENDENCY ${DEPENDS})
            add_custom_command(TARGET ${target} PRE_BUILD
                                COMMAND ${CMAKE_COMMAND} -E copy
                                $<TARGET_FILE:${DEPENDENCY}>
                                $<TARGET_FILE_DIR:${target}>)
        endforeach()
    endif()
endfunction()

# Create an interface library for an external dependency. This virtual target can provide
# link specifications and include directories to be used by dependees.
# The created INTERFACE library is tagged for export to be part of the generated SFMLConfig
# Usage: sfml_add_external(target_name
#                          [INCLUDE "extlibs/include"]
#                          [LINK "extlibs/libfoo/libfoo.a"])
function(sfml_add_external)
    list(GET ARGN 0 target)
    list(REMOVE_AT ARGN 0)

    if (TARGET ${target})
        message(FATAL_ERROR "Target '${target}' is already defined")
    endif()

    cmake_parse_arguments(THIS "" "" "INCLUDE;LINK" ${ARGN})
    if (THIS_UNPARSED_ARGUMENTS)
        message(FATAL_ERROR "Unknown arguments when calling sfml_import_library: ${THIS_UNPARSED_ARGUMENTS}")
    endif()

    add_library(${target} INTERFACE)

    if (THIS_INCLUDE)
        foreach(include_dir IN LISTS THIS_INCLUDE)
            if (NOT include_dir)
                message(FATAL_ERROR "No path given for include dir ${THIS_INCLUDE}")
            endif()
            target_include_directories(${target} SYSTEM INTERFACE "$<BUILD_INTERFACE:${include_dir}>")
        endforeach()
    endif()

    if (THIS_LINK)
        foreach(link_item IN LISTS THIS_LINK)
            if (NOT link_item)
                message(FATAL_ERROR "Missing item in ${THIS_LINK}")
            endif()
            target_link_libraries(${target} INTERFACE "$<BUILD_INTERFACE:${link_item}>")
        endforeach()
    endif()

    install(TARGETS ${target} EXPORT SFMLConfigExport)
endfunction()

# Find the requested package and make an INTERFACE library from it
# The created INTERFACE library is tagged for export to be part of the generated SFMLConfig
# Usage: sfml_find_package(wanted_target_name
#                          [INCLUDE "OPENGL_INCLUDE_DIR"]
#                          [LINK "OPENGL_gl_LIBRARY"])
function(sfml_find_package)
    list(GET ARGN 0 target)
    list(REMOVE_AT ARGN 0)

    if (TARGET ${target})
        message(FATAL_ERROR "Target '${target}' is already defined")
    endif()

    cmake_parse_arguments(THIS "" "" "INCLUDE;LINK" ${ARGN})
    if (THIS_UNPARSED_ARGUMENTS)
        message(FATAL_ERROR "Unknown arguments when calling sfml_import_library: ${THIS_UNPARSED_ARGUMENTS}")
    endif()

    set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake/Modules/")
    if (SFML_OS_IOS)
        find_host_package(${target} REQUIRED)
    else()
        find_package(${target} REQUIRED)
    endif()

    # Make sure to interpret the items in INCLUDE and LINK parameters. sfml_add_external()
    # does not interpret given items in order to also accept parameters that must not be interpreted
    set(LINK_LIST "")
    if (THIS_LINK)
        foreach(link_item IN LISTS THIS_LINK)
            list(APPEND LINK_LIST "${${link_item}}")
        endforeach()
    endif()

    set(INCLUDE_LIST "")
    if (THIS_INCLUDE)
        foreach(include_dir IN LISTS THIS_INCLUDE)
            list(APPEND INCLUDE_LIST "${${include_dir}}")
        endforeach()
    endif()

    sfml_add_external(${target} INCLUDE ${INCLUDE_LIST} LINK ${LINK_LIST})
endfunction()

# Generate a SFMLConfig.cmake file (and associated files) from the targets registered against
# the EXPORT name "SFMLConfigExport" (EXPORT parameter of install(TARGETS))
function(sfml_export_targets)
    # CMAKE_CURRENT_LIST_DIR or CMAKE_CURRENT_SOURCE_DIR not usable for files that are to be included like this one
    set(CURRENT_DIR "${PROJECT_SOURCE_DIR}/cmake")

    include(CMakePackageConfigHelpers)
    write_basic_package_version_file("${CMAKE_CURRENT_BINARY_DIR}/SFMLConfigVersion.cmake"
                                     VERSION ${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}
                                     COMPATIBILITY SameMajorVersion)

    if (BUILD_SHARED_LIBS)
        set(config_name "Shared")
    else()
        set(config_name "Static")
    endif()
    set(targets_config_filename "SFML${config_name}Targets.cmake")

    export(EXPORT SFMLConfigExport
           FILE "${CMAKE_CURRENT_BINARY_DIR}/${targets_config_filename}")

    if (SFML_BUILD_FRAMEWORKS)
        set(config_package_location "SFML.framework/Resources/CMake")
    else()
        set(config_package_location ${CMAKE_INSTALL_LIBDIR}/cmake/SFML)
    endif()
    configure_package_config_file("${CURRENT_DIR}/SFMLConfig.cmake.in" "${CMAKE_CURRENT_BINARY_DIR}/SFMLConfig.cmake"
        INSTALL_DESTINATION "${config_package_location}")
    configure_package_config_file("${CURRENT_DIR}/SFMLConfigDependencies.cmake.in" "${CMAKE_CURRENT_BINARY_DIR}/SFMLConfigDependencies.cmake"
        INSTALL_DESTINATION "${config_package_location}")


    install(EXPORT SFMLConfigExport
            FILE ${targets_config_filename}
            DESTINATION ${config_package_location})

    install(FILES "${CMAKE_CURRENT_BINARY_DIR}/SFMLConfig.cmake"
                  "${CMAKE_CURRENT_BINARY_DIR}/SFMLConfigDependencies.cmake"
                  "${CMAKE_CURRENT_BINARY_DIR}/SFMLConfigVersion.cmake"
            DESTINATION ${config_package_location}
            COMPONENT devel)
endfunction()
