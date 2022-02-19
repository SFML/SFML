# define the sfml-main target
sfml_add_library(Main STATIC)

set(INCROOT ${PROJECT_SOURCE_DIR}/include/SFML/Main)
set(SRCROOT ${PROJECT_SOURCE_DIR}/src/SFML/Main)

# sources
if(SFML_OS_WINDOWS)
    target_sources(sfml-main PRIVATE ${SRCROOT}/MainWin32.cpp)
elseif(SFML_OS_IOS)
    target_sources(sfml-main PRIVATE ${SRCROOT}/MainiOS.mm)
elseif(SFML_OS_ANDROID)
    target_sources(sfml-main PRIVATE ${SRCROOT}/MainAndroid.cpp)
else()
    return()
endif()

if(SFML_OS_ANDROID)
    # glad sources
    target_include_directories(sfml-main SYSTEM PRIVATE "${PROJECT_SOURCE_DIR}/extlibs/headers/glad/include")
endif()

# overwrite sfml-main suffix for backward compatibility with FindSFML.cmake
set_target_properties(sfml-main PROPERTIES
                      DEBUG_POSTFIX -d
                      RELEASE_POSTFIX ""
                      MINSIZEREL_POSTFIX ""
                      RELWITHDEBINFO_POSTFIX "")

# because of a current limitation on Android (which prevents one library
# from depending on shared libraries), we need a boostrap activity which
# will load our shared libraries manually
if(SFML_OS_ANDROID)
    sfml_add_library(Activity)
    target_sources(sfml-activity PRIVATE ${SRCROOT}/SFMLActivity.cpp)
endif()
