# define the sfml-main target
sfml_add_library(Main STATIC)

# sources
if(SFML_OS_WINDOWS)
    target_sources(sfml-main PRIVATE MainWin32.cpp)
elseif(SFML_OS_IOS)
    target_sources(sfml-main PRIVATE MainiOS.mm)
elseif(SFML_OS_ANDROID)
    target_sources(sfml-main PRIVATE MainAndroid.cpp)
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
    target_sources(sfml-activity PRIVATE SFMLActivity.cpp)
endif()
