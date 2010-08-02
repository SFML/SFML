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
#include <SFML/System/Randomizer.hpp>
#include <cstdlib>
#include <ctime>


////////////////////////////////////////////////////////////
// Private data
////////////////////////////////////////////////////////////
namespace
{
    // Initialize the generator's seed with the current system time
    // in milliseconds, so that it is always different
    unsigned int InitializeSeed()
    {
        unsigned int seed = static_cast<unsigned int>(std::time(NULL));
        std::srand(seed);
        return seed;
    }

    // Global variable storing the current seed
    unsigned int globalSeed = InitializeSeed();
}


namespace sf
{
////////////////////////////////////////////////////////////
void Randomizer::SetSeed(unsigned int seed)
{
    std::srand(seed);
    globalSeed = seed;
}


////////////////////////////////////////////////////////////
unsigned int Randomizer::GetSeed()
{
    return globalSeed;
}


////////////////////////////////////////////////////////////
float Randomizer::Random(float begin, float end)
{
    // This is not the best algorithm, but it is fast and will be enough in most cases

    return static_cast<float>(std::rand()) / RAND_MAX * (end - begin) + begin;
}


////////////////////////////////////////////////////////////
/// Get a random integer number in a given range
////////////////////////////////////////////////////////////
int Randomizer::Random(int begin, int end)
{
    // This is not the best algorithm, but it is fast and will be enough in most cases

    return std::rand() % (end - begin + 1) + begin;
}

} // namespace sf
