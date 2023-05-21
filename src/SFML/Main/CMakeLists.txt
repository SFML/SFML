
set(INCROOT ${PROJECT_SOURCE_DIR}/include/SFML/Main)
set(SRCROOT ${PROJECT_SOURCE_DIR}/src/SFML/Main)

# sources
if(SFML_OS_WINDOWS)
    set(SRC ${SRCROOT}/MainWin32.cpp)
elseif(SFML_OS_IOS)
    enable_language(OBJCXX)
    set(SRC ${SRCROOT}/MainiOS.mm)
elseif(SFML_OS_ANDROID)
    set(SRC ${SRCROOT}/MainAndroid.cpp)
else()
    return()
endif()

# define the sfml-main target
sfml_add_library(Main STATIC SOURCES ${SRC})

if(SFML_OS_ANDROID)
    # ensure that linking into shared libraries doesn't fail
    set_target_properties(sfml-main PROPERTIES POSITION_INDEPENDENT_CODE ON)

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
# from depending on shared libraries), we need a bootstrap activity which
# will load our shared libraries manually
if(SFML_OS_ANDROID)
    sfml_add_library(Activity SOURCES ${SRCROOT}/SFMLActivity.cpp)
endif()
