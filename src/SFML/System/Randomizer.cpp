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
#include <SFML/System/Platform.hpp>
#include <cstdlib>


namespace
{
    // Set the random numbers sequence seed with the current system time, so that it is always different
    unsigned int SetRandomSeed()
    {
        unsigned int Seed = static_cast<unsigned int>(sf::priv::Platform::GetSystemTime() * 1000);
        srand(Seed);
        return Seed;
    }
}


namespace sf
{
////////////////////////////////////////////////////////////
// Static member variables
////////////////////////////////////////////////////////////
unsigned int Randomizer::ourSeed = SetRandomSeed();


////////////////////////////////////////////////////////////
/// Set the seed for the generator. Using a known seed
/// allows you to reproduce the same sequence of random number
////////////////////////////////////////////////////////////
void Randomizer::SetSeed(unsigned int Seed)
{
    srand(Seed);
    ourSeed = Seed;
}


////////////////////////////////////////////////////////////
/// Get the seed used to generate random numbers the generator.
////////////////////////////////////////////////////////////
unsigned int Randomizer::GetSeed()
{
    return ourSeed;
}


////////////////////////////////////////////////////////////
/// Get a random float number in a given range
////////////////////////////////////////////////////////////
float Randomizer::Random(float Begin, float End)
{
    // This is not the best algorithm, but it is fast and will be enough in most cases
    // (see Google for best approaches)

    return static_cast<float>(rand()) / RAND_MAX * (End - Begin) + Begin;
}


////////////////////////////////////////////////////////////
/// Get a random integer number in a given range
////////////////////////////////////////////////////////////
int Randomizer::Random(int Begin, int End)
{
    // This is not the best algorithm, but it is fast and will be enough in most cases
    // (see Google for best approaches)

    return rand() % (End - Begin + 1) + Begin;
}

} // namespace sf
