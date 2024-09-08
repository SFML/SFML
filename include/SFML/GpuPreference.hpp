////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007-2024 Laurent Gomila (laurent@sfml-dev.org)
//
// This software is provided 'as-is', without any express or implied warranty.
// In no event will the authors be held liable for any damages arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it freely,
// subject to the following restrictions:
//
// 1. The origin of this software must not be misrepresented;
//    you must not claim that you wrote the original software.
//    If you use this software in a product, an acknowledgment
//    in the product documentation would be appreciated but is not required.
//
// 2. Altered source versions must be plainly marked as such,
//    and must not be misrepresented as being the original software.
//
// 3. This notice may not be removed or altered from any source distribution.
//
////////////////////////////////////////////////////////////

#pragma once


////////////////////////////////////////////////////////////
/// Headers
////////////////////////////////////////////////////////////
#include <SFML/Config.hpp>


////////////////////////////////////////////////////////////
/// \file
///
/// \brief File containing SFML_DEFINE_DISCRETE_GPU_PREFERENCE
///
////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////
/// \def SFML_DEFINE_DISCRETE_GPU_PREFERENCE
///
/// \brief A macro to encourage usage of the discrete GPU
///
/// In order to inform the Nvidia/AMD driver that an SFML
/// application could benefit from using the more powerful
/// discrete GPU, special symbols have to be publicly
/// exported from the final executable.
///
/// SFML defines a helper macro to easily do this.
///
/// Place `SFML_DEFINE_DISCRETE_GPU_PREFERENCE` in the
/// global scope of a source file that will be linked into
/// the final executable. Typically it is best to place it
/// where the main function is also defined.
///
////////////////////////////////////////////////////////////
#if defined(SFML_SYSTEM_WINDOWS)

#define SFML_DEFINE_DISCRETE_GPU_PREFERENCE                                                  \
    extern "C" __declspec(dllexport) unsigned long NvOptimusEnablement                  = 1; \
    extern "C" __declspec(dllexport) unsigned long AmdPowerXpressRequestHighPerformance = 1;

#else

#define SFML_DEFINE_DISCRETE_GPU_PREFERENCE

#endif
