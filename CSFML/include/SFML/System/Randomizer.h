////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007-2009 Laurent Gomila (laurent.gom@gmail.com)
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

#ifndef SFML_RANDOMIZER_H
#define SFML_RANDOMIZER_H

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Config.h>


////////////////////////////////////////////////////////////
/// Set the seed for the random numbers generator. Using a known seed
/// allows you to reproduce the same sequence of random numbers
///
/// \param Seed : Number to use as the seed
///
////////////////////////////////////////////////////////////
CSFML_API void sfRandom_SetSeed(unsigned int Seed);

////////////////////////////////////////////////////////////
/// Get the seed used to generate random numbers the generator
///
/// \return Current seed
///
////////////////////////////////////////////////////////////
CSFML_API unsigned int sfRandom_GetSeed();

////////////////////////////////////////////////////////////
/// Get a random float number in a given range
///
/// \return Start : Start of the range
/// \return End :   End of the range
///
/// \return Random number in [Begin, End]
///
////////////////////////////////////////////////////////////
CSFML_API float sfRandom_Float(float Begin, float End);

////////////////////////////////////////////////////////////
/// Get a random integer number in a given range
///
/// \return Start : Start of the range
/// \return End :   End of the range
///
/// \return Random number in [Begin, End]
///
////////////////////////////////////////////////////////////
CSFML_API int sfRandom_Int(int Begin, int End);


#endif // SFML_RANDOMIZER_H
