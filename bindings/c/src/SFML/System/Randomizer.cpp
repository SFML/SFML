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

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/System/Randomizer.h>
#include <SFML/System/Randomizer.hpp>
#include <SFML/Internal.h>


////////////////////////////////////////////////////////////
/// Set the seed for the random numbers generator. Using a known seed
/// allows you to reproduce the same sequence of random numbers
////////////////////////////////////////////////////////////
void sfRandom_SetSeed(unsigned int seed)
{
    sf::Randomizer::SetSeed(seed);
}


////////////////////////////////////////////////////////////
/// Get the seed used to generate random numbers the generator
////////////////////////////////////////////////////////////
unsigned int sfRandom_GetSeed(void)
{
    return sf::Randomizer::GetSeed();
}


////////////////////////////////////////////////////////////
/// Get a random float number in a given range
////////////////////////////////////////////////////////////
float sfRandom_Float(float begin, float end)
{
    return sf::Randomizer::Random(begin, end);
}


////////////////////////////////////////////////////////////
/// Get a random integer number in a given range
////////////////////////////////////////////////////////////
int sfRandom_Int(int begin, int end)
{
    return sf::Randomizer::Random(begin, end);
}
