include(CMakeParseArguments)

# add a new target which is a SFML library
# ex: sfml_add_library(sfml-graphics
#                      SOURCES sprite.cpp image.cpp ...
#                      DEPENDS sfml-window sfml-system
#                      EXTERNAL_LIBS opengl freetype ...)
macro(sfml_add_library target)

    # parse the arguments
    cmake_parse_arguments(THIS "" "" "SOURCES;DEPENDS;EXTERNAL_LIBS" ${ARGN})

if(NOT ANDROID)
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
    endif()

    # set the version and soversion of the target (for compatible systems -- mostly Linuxes)
    set_target_properties(${target} PROPERTIES SOVERSION ${VERSION_MAJOR})
    set_target_properties(${target} PROPERTIES VERSION ${VERSION_MAJOR}.${VERSION_MINOR})

    # set the target's folder (for IDEs that support it, e.g. Visual Studio)
    set_target_properties(${target} PROPERTIES FOLDER "SFML")

    # for gcc >= 4.0 on Windows, apply the SFML_USE_STATIC_STD_LIBS option if it is enabled
    if(SFML_OS_WINDOWS AND SFML_COMPILER_GCC AND NOT SFML_GCC_VERSION VERSION_LESS "4")
        if(SFML_USE_STATIC_STD_LIBS AND NOT SFML_COMPILER_GCC_TDM)
            set_target_properties(${target} PROPERTIES LINK_FLAGS "-static-libgcc -static-libstdc++")
        elseif(NOT SFML_USE_STATIC_STD_LIBS AND SFML_COMPILER_GCC_TDM)
            set_target_properties(${target} PROPERTIES LINK_FLAGS "-shared-libgcc -shared-libstdc++")
        endif()
    endif()

    # if using gcc >= 4.0 or clang >= 3.0 on a non-Windows platform, we must hide public symbols by default
    # (exported ones are explicitely marked)
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

        # adapt install directory to allow distributing dylibs/frameworks in user’s frameworks/application bundle
        set_target_properties(${target} PROPERTIES
                              BUILD_WITH_INSTALL_RPATH 1
                              INSTALL_NAME_DIR "@executable_path/../Frameworks")
    endif()

    # enable automatic reference counting on iOS
    if (IOS)
        set_target_properties(${target} PROPERTIES XCODE_ATTRIBUTE_CLANG_ENABLE_OBJC_ARC YES)
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
else()
    # create an empty target added to the default build
    add_custom_target(${target} ALL)

    # get the module name (extract 'network' from 'sfml-network' for example)
    string(SUBSTRING ${target} 5 -1 MODULE_NAME)

    # make the first letter a capital letter ('network' -> 'Network')s
    string(SUBSTRING ${MODULE_NAME} 0 1 FIRST_LETTER)
    string(TOUPPER ${FIRST_LETTER} FIRST_LETTER)
    string(REGEX REPLACE "^.(.*)" "${FIRST_LETTER}\\1" PROTO_NAME_CAP "${MODULE_NAME}")
    set(MODULE_NAME ${PROTO_NAME_CAP})

    # create an empty Application.mk ready to be filled
    set(APPLICATION_MK ${CMAKE_BINARY_DIR}/src/SFML/${MODULE_NAME}/Application.mk)
    file(WRITE ${APPLICATION_MK})

    # define several path for later use
    set(INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include)
    set(SRC_DIR ${PROJECT_SOURCE_DIR}/src)
    set(EXTLIBS_DIR ${PROJECT_SOURCE_DIR}/extlibs/android/extlibs)

	# add support for Windows under Cygwin (add  C:/cygwin64 to path)
	execute_process(COMMAND uname -o OUTPUT_VARIABLE UNAME_RESULT)
	if (UNAME_RESULT MATCHES Cygwin)
		execute_process(COMMAND cygpath --mixed / OUTPUT_VARIABLE CYGPATH)
		string(STRIP ${CYGPATH} CYGPATH)

		set(INCLUDE_DIR ${CYGPATH}${PROJECT_SOURCE_DIR}/include)
		set(SRC_DIR ${CYGPATH}${PROJECT_SOURCE_DIR}/src)
		set(EXTLIBS_DIR ${CYGPATH}${PROJECT_SOURCE_DIR}/extlibs/android/extlibs)
	endif()

    # write the Application.mk
    file(APPEND ${APPLICATION_MK} "NDK_APP_OUT := ${CMAKE_BINARY_DIR}/src/SFML/${MODULE_NAME}/obj\n")
    file(APPEND ${APPLICATION_MK} "APP_PLATFORM := android-${ANDROID_API_LEVEL}\n")
    file(APPEND ${APPLICATION_MK} "APP_STL := stlport_static\n")
    file(APPEND ${APPLICATION_MK} "APP_BUILD_SCRIPT := Android.mk\n")
    file(APPEND ${APPLICATION_MK} "APP_MODULES := ${target}\n")
    file(APPEND ${APPLICATION_MK} "APP_CPPFLAGS += -I${INCLUDE_DIR}\n")
    file(APPEND ${APPLICATION_MK} "APP_CPPFLAGS += -I${SRC_DIR}\n")
    file(APPEND ${APPLICATION_MK} "APP_CPPFLAGS += -I${EXTLIBS_DIR}/include\n")
    file(APPEND ${APPLICATION_MK} "APP_CPPFLAGS += -DSFML_OPENGL_ES\n")

    if (ANDROID_ABI_ARM)
        file(APPEND ${APPLICATION_MK} "APP_ABI += armeabi\n")
    endif()

    if (ANDROID_ABI_ARMv7)
        file(APPEND ${APPLICATION_MK} "APP_ABI += armeabi-v7a\n")
    endif()

    if (ANDROID_ABI_MIPS)
        file(APPEND ${APPLICATION_MK} "APP_ABI += mips\n")
    endif()

    if (ANDROID_ABI_x86)
        file(APPEND ${APPLICATION_MK} "APP_ABI += x86\n")
    endif()

    # define the target build rules
    set(DIR ${PROJECT_SOURCE_DIR}/src/SFML/${MODULE_NAME})
    set(NDK_MODULE_PATH ${PROJECT_SOURCE_DIR}/extlibs/android)
    set(NDK_PROJECT_PATH ${PROJECT_SOURCE_DIR}/src/SFML/${MODULE_NAME})
    set(NDK_BUILD ${ANDROID_NDK_PATH}/ndk-build)
    set(NDK_APPLICATION_MK NDK_APPLICATION_MK=${APPLICATION_MK})

    # create the environment for ndk-build
    set(NDK_ENVIRONMENT NDK_MODULE_PATH=${NDK_MODULE_PATH} NDK_PROJECT_PATH=${NDK_PROJECT_PATH})

    # invoke the ndk-build scripts from the ndk with the right parameters
    add_custom_command(TARGET ${target} POST_BUILD COMMAND ${NDK_BUILD} -C ${DIR} ${NDK_APPLICATION_MK} ${NDK_ENVIRONMENT})

    # define the target install rules
    if (ANDROID_ABI_ARM)
        install(DIRECTORY ${CMAKE_BINARY_DIR}/src/SFML/${MODULE_NAME}/obj/local/armeabi DESTINATION ${ANDROID_NDK_PATH}/sources/sfml/lib/ PATTERN "objs" EXCLUDE)
    endif()
    if (ANDROID_ABI_ARMv7)
        install(DIRECTORY ${CMAKE_BINARY_DIR}/src/SFML/${MODULE_NAME}/obj/local/armeabi-v7a DESTINATION ${ANDROID_NDK_PATH}/sources/sfml/lib PATTERN "objs" EXCLUDE)
    endif()
    if (ANDROID_ABI_MIPS)
        install(DIRECTORY ${CMAKE_BINARY_DIR}/src/SFML/${MODULE_NAME}/obj/local/mips DESTINATION ${ANDROID_NDK_PATH}/sources/sfml/lib PATTERN "objs" EXCLUDE)
    endif()
    if (ANDROID_ABI_x86)
        install(DIRECTORY ${CMAKE_BINARY_DIR}/src/SFML/${MODULE_NAME}/obj/local/x86 DESTINATION ${ANDROID_NDK_PATH}/sources/sfml/lib PATTERN "objs" EXCLUDE)
    endif()

endif()
endmacro()

# add a new target which is a SFML example
# ex: sfml_add_example(ftp
#                      SOURCES ftp.cpp ...
#                      DEPENDS sfml-network sfml-system)
macro(sfml_add_example target)

    # parse the arguments
    sfml_parse_arguments(THIS "SOURCES;DEPENDS" "GUI_APP" ${ARGN})

    # set a source group for the source files
    source_group("" FILES ${THIS_SOURCES})

    # create the target
    if(THIS_GUI_APP AND WINDOWS)
        add_executable(${target} WIN32 ${THIS_SOURCES})
        target_link_libraries(${target} sfml-main)
    else()
        add_executable(${target} ${THIS_SOURCES})
    endif()

    # set the debug suffix
    set_target_properties(${target} PROPERTIES DEBUG_POSTFIX -d)

    # set the target's folder (for IDEs that support it, e.g. Visual Studio)
    set_target_properties(${target} PROPERTIES FOLDER "Examples")

    # for gcc >= 4.0 on Windows, apply the SFML_USE_STATIC_STD_LIBS option if it is enabled
    if(WINDOWS AND COMPILER_GCC AND SFML_USE_STATIC_STD_LIBS)
        if(NOT GCC_VERSION VERSION_LESS "4")
            set_target_properties(${target} PROPERTIES LINK_FLAGS "-static-libgcc -static-libstdc++")
        endif()
    endif()

    # link the target to its SFML dependencies
    if(THIS_DEPENDS)
        target_link_libraries(${target} ${THIS_DEPENDS})
    endif()

    # add the install rule
    install(TARGETS ${target}
            RUNTIME DESTINATION ${INSTALL_MISC_DIR}/examples/${target} COMPONENT examples)

    # install the example's source code
    install(FILES ${THIS_SOURCES}
            DESTINATION ${INSTALL_MISC_DIR}/examples/${target}
            COMPONENT examples)

    # install the example's resources as well
    set(EXAMPLE_RESOURCES "${CMAKE_SOURCE_DIR}/examples/${target}/resources")
    if(EXISTS ${EXAMPLE_RESOURCES})
        install(DIRECTORY ${EXAMPLE_RESOURCES}
                DESTINATION ${INSTALL_MISC_DIR}/examples/${target}
                COMPONENT examples)
    endif()
endmacro()

# add a new target which is a SFML example
# ex: sfml_add_example(ftp
#                      SOURCES ftp.cpp ...
#                      DEPENDS sfml-network sfml-system)
macro(sfml_add_example target)

    # parse the arguments
    cmake_parse_arguments(THIS "GUI_APP" "" "SOURCES;DEPENDS" ${ARGN})

    # set a source group for the source files
    source_group("" FILES ${THIS_SOURCES})

    # create the target
    if(THIS_GUI_APP AND SFML_OS_WINDOWS)
        add_executable(${target} WIN32 ${THIS_SOURCES})
        target_link_libraries(${target} sfml-main)
    elseif(IOS)
        add_executable(${target} MACOSX_BUNDLE ${THIS_SOURCES})
    else()
        add_executable(${target} ${THIS_SOURCES})
    endif()

    # set the debug suffix
    set_target_properties(${target} PROPERTIES DEBUG_POSTFIX -d)

    # set the target's folder (for IDEs that support it, e.g. Visual Studio)
    set_target_properties(${target} PROPERTIES FOLDER "Examples")

    # for gcc >= 4.0 on Windows, apply the SFML_USE_STATIC_STD_LIBS option if it is enabled
    if(SFML_OS_WINDOWS AND SFML_COMPILER_GCC AND NOT SFML_GCC_VERSION VERSION_LESS "4")
        if(SFML_USE_STATIC_STD_LIBS AND NOT SFML_COMPILER_GCC_TDM)
            set_target_properties(${target} PROPERTIES LINK_FLAGS "-static-libgcc -static-libstdc++")
        elseif(NOT SFML_USE_STATIC_STD_LIBS AND SFML_COMPILER_GCC_TDM)
            set_target_properties(${target} PROPERTIES LINK_FLAGS "-shared-libgcc -shared-libstdc++")
        endif()
    endif()

    # link the target to its SFML dependencies
    if(THIS_DEPENDS)
        target_link_libraries(${target} ${THIS_DEPENDS})
    endif()

    # enable automatic reference counting on iOS
    if (IOS)
        set_target_properties(${target} PROPERTIES XCODE_ATTRIBUTE_CLANG_ENABLE_OBJC_ARC YES)
    endif()

    # set mandatory bundle settings on iOS
    if(IOS)
        set_target_properties(${target} PROPERTIES
            MACOSX_BUNDLE_GUI_IDENTIFIER "com.sfml.${target}"
            MACOSX_BUNDLE_BUNDLE_NAME "${target}"
            MACOSX_BUNDLE_BUNDLE_EXECUTABLE_NAME "${target}"
        )
    endif()

    # add the install rule
    install(TARGETS ${target}
            RUNTIME DESTINATION ${INSTALL_MISC_DIR}/examples/${target} COMPONENT examples
            BUNDLE DESTINATION ${INSTALL_MISC_DIR}/examples/${target} COMPONENT examples)

    # install the example's source code
    install(FILES ${THIS_SOURCES}
            DESTINATION ${INSTALL_MISC_DIR}/examples/${target}
            COMPONENT examples)

    # install the example's resources as well
    set(EXAMPLE_RESOURCES "${CMAKE_SOURCE_DIR}/examples/${target}/resources")
    if(EXISTS ${EXAMPLE_RESOURCES})
        install(DIRECTORY ${EXAMPLE_RESOURCES}
                DESTINATION ${INSTALL_MISC_DIR}/examples/${target}
                COMPONENT examples)
    endif()

endmacro()
