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

#ifndef SFML_RANDOMIZER_HPP
#define SFML_RANDOMIZER_HPP

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Config.hpp>


namespace sf
{
////////////////////////////////////////////////////////////
/// Randomizer is an utility class for generating pseudo-random
/// numbers
////////////////////////////////////////////////////////////
class SFML_API Randomizer
{
public :

    ////////////////////////////////////////////////////////////
    /// Set the seed for the generator. Using a known seed
    /// allows you to reproduce the same sequence of random number
    ///
    /// \param Seed : Number to use as the seed
    ///
    ////////////////////////////////////////////////////////////
    static void SetSeed(unsigned int Seed);

    ////////////////////////////////////////////////////////////
    /// Get the seed used to generate random numbers the generator.
    ///
    /// \return Current seed
    ///
    ////////////////////////////////////////////////////////////
    static unsigned int GetSeed();

    ////////////////////////////////////////////////////////////
    /// Get a random float number in a given range
    ///
    /// \return Start : Start of the range
    /// \return End :   End of the range
    ///
    /// \return Random number in [Begin, End]
    ///
    ////////////////////////////////////////////////////////////
    static float Random(float Begin, float End);

    ////////////////////////////////////////////////////////////
    /// Get a random integer number in a given range
    ///
    /// \return Start : Start of the range
    /// \return End :   End of the range
    ///
    /// \return Random number in [Begin, End]
    ///
    ////////////////////////////////////////////////////////////
    static int Random(int Begin, int End);

private :

    ////////////////////////////////////////////////////////////
    // Static member variables
    ////////////////////////////////////////////////////////////
    static unsigned int ourSeed;
};

} // namespace sf


#endif // SFML_RANDOMIZER_HPP
