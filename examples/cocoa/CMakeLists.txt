
set(SRCROOT ${PROJECT_SOURCE_DIR}/examples/cocoa)

# Usage: compile_xib(INPUT path/to/file.xib OUTPUT path/to/file.nib)
function(compile_xib)
    cmake_parse_arguments(THIS "" "INPUT;OUTPUT" "" ${ARGN})
    if (NOT THIS_INPUT)
        message(FATAL_ERROR "Missing required argument INPUT in call to compile_xib()")
    endif()

    if (NOT THIS_OUTPUT)
        message(FATAL_ERROR "Missing required argument OUTPUT in call to compile_xib()")
    endif()

    if (NOT DEFINED IBTOOL)
        find_program(IBTOOL ibtool HINTS "/usr/bin" "${OSX_DEVELOPER_ROOT}/usr/bin")
    endif()
    if(NOT IBTOOL)
        message(FATAL_ERROR "ibtool is required to compile .xib files but wasn't found.")
    endif()

    # Default args taken from Xcode 9 when it generates a nib from a xib
    set(DEFAULT_ARGS --errors --warnings --notices --module cocoa --auto-activate-custom-fonts --target-device mac --output-format human-readable-text)

    add_custom_command(OUTPUT "${THIS_OUTPUT}"
        COMMAND "${IBTOOL}" ${DEFAULT_ARGS} "${THIS_INPUT}" --compile "${THIS_OUTPUT}"
        DEPENDS "${THIS_INPUT}"
        COMMENT "Generating ${THIS_OUTPUT}"
        VERBATIM)
endfunction()

# all source files
set(SRC ${SRCROOT}/CocoaAppDelegate.h
        ${SRCROOT}/CocoaAppDelegate.mm
        ${SRCROOT}/NSString+stdstring.h
        ${SRCROOT}/NSString+stdstring.mm
        ${SRCROOT}/main.m)

compile_xib(INPUT "${SRCROOT}/MainMenu.xib" OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/MainMenu.nib")

# all resource files
set(RESOURCES ${SRCROOT}/resources/icon.icns
              ${SRCROOT}/resources/tuffy.ttf
              ${SRCROOT}/resources/logo.png
              ${SRCROOT}/resources/blue.png
              ${SRCROOT}/resources/green.png
              ${SRCROOT}/resources/red.png
              ${SRCROOT}/resources/Credits.rtf
              ${CMAKE_CURRENT_BINARY_DIR}/MainMenu.nib)
set_source_files_properties(${RESOURCES} PROPERTIES
                            MACOSX_PACKAGE_LOCATION Resources)

# define the cocoa target and customize it
sfml_add_example(cocoa
                 SOURCES ${SRC}
                 BUNDLE_RESOURCES ${RESOURCES}
                 DEPENDS SFML::System SFML::Window SFML::Graphics)
set_target_properties(cocoa PROPERTIES
                      MACOSX_BUNDLE TRUE
                      MACOSX_BUNDLE_INFO_PLIST ${SRCROOT}/resources/Cocoa-Info.plist)
target_link_libraries(cocoa PRIVATE "-framework Cocoa" "-framework Foundation" SFML::Graphics)
