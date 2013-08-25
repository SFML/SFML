# As Android native applications are compiled by the NDK and so, use its build
# system (mandatory) which contains many toolchains, we must fool CMake to
# obtain the same build interface. Here, we're setting non-existing toolchains
# to satisfy CMake. Only CMAKE_SYSTEM_NAME is relevant as it will identify the
# targetted platfrom across our CMake scripts.

include(CMakeForceCompiler)

set(CMAKE_SYSTEM_NAME Android)
set(CMAKE_SYSTEM_VERSION 0)
set(CMAKE_SYSTEM_PROCESSOR unknown)

CMAKE_FORCE_C_COMPILER(/ GNU)
CMAKE_FORCE_CXX_COMPILER(/ GNU)

# We need to know where the NDK is located. We'll define some options to
# customize the build process using NDK/ndk-build: a library might be built for
# multiple architecture targetting different API level at the same time.

# See the NDK as an entire filesystem on its own where we would install headers
# and libraries in it.

# attempts to know the NDK path from various environment variable
if(NOT $ENV{NDK} STREQUAL "")
	set(ANDROID_NDK_PATH $ENV{NDK})
elseif(NOT $ENV{NDK_PATH} STREQUAL "")
	set(ANDROID_NDK_PATH $ENV{NDK_PATH})
elseif(NOT $ENV{ANDROID_NDK} STREQUAL "")
	set(ANDROID_NDK_PATH $ENV{ANDROID_NDK})
elseif(NOT $ENV{ANDROID_NDK_PATH} STREQUAL "")
	set(ANDROID_NDK_PATH $ENV{ANDROID_NDK_PATH})
else()
	set(ANDROID_NDK_PATH /path/to/the/NDK)
endif()

# add an option for choosing the NDK
sfml_set_option(ANDROID_NDK_PATH ${ANDROID_NDK_PATH} STRING "Where is the NDK located ?")

# add an option for choosing the API level (by default, it chooses the most widespread and used)
sfml_set_option(ANDROID_API_LEVEL 9 STRING "What API level should it target ?")

# add options for choosing the targetted arches
sfml_set_option(ANDROID_ABI_ARM TRUE BOOL "Build for arm architecture ?")
sfml_set_option(ANDROID_ABI_ARMv7 FALSE BOOL "Build for armv7 architecture ?")
sfml_set_option(ANDROID_ABI_MIPS FALSE BOOL "Build for mipsel architecture ?")
sfml_set_option(ANDROID_ABI_x86 FALSE BOOL "Build for x86 architecture ?")

# where SFML headers and libs need to be installed
set(CMAKE_INSTALL_PREFIX ${ANDROID_NDK_PATH}/sources/sfml)
