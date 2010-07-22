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
/// \brief Utility class for generating pseudo-random numbers
///
////////////////////////////////////////////////////////////
class SFML_API Randomizer
{
public :

    ////////////////////////////////////////////////////////////
    /// \brief Set a new seed for the generator
    ///
    /// Using a known seed allows you to reproduce the same
    /// sequence of random numbers.
    ///
    /// \param seed Number to use as the seed
    ///
    ////////////////////////////////////////////////////////////
    static void SetSeed(unsigned int seed);

    ////////////////////////////////////////////////////////////
    /// \brief Get the current seed of the generator
    ///
    /// \return Current seed
    ///
    ////////////////////////////////////////////////////////////
    static unsigned int GetSeed();

    ////////////////////////////////////////////////////////////
    /// \brief Get a random float number in a given range
    ///
    /// \param begin Beginning of the range
    /// \param end   End of the range
    ///
    /// \return Random number in [begin, end]
    ///
    ////////////////////////////////////////////////////////////
    static float Random(float begin, float end);

    ////////////////////////////////////////////////////////////
    /// \brief Get a random integer number in a given range
    ///
    /// \param begin Beginning of the range
    /// \param end   End of the range
    ///
    /// \return Random number in [begin, end]
    ///
    ////////////////////////////////////////////////////////////
    static int Random(int begin, int end);
};

} // namespace sf


#endif // SFML_RANDOMIZER_HPP


////////////////////////////////////////////////////////////
/// \class sf::Randomizer
/// \ingroup system
///
/// sf::Randomizer generates pseudo-random numbers using the
/// standard library.
///
/// Usage example:
/// \code
/// int x1 = sf::Randomizer::Random(0, 6);
/// float x2 = sf::Randomizer::Random(0.f, 1.f);
/// \endcode
///
/// Note that, unlike the standard rand() function, you don't
/// have to initialize the seed before using this class. SFML does
/// it for you, so that you will automatically get different results
/// for every execution of the program.
///
/// The SetSeed and GetSeed functions are provided mainly for being
/// able to reproduce the same set of numbers in a recorded
/// sequence. This is useful when implementing replays, for example.
///
/// The technique used by sf::Randomizer for getting random numbers
/// is not the most accurate: some numbers may have a slightly higher
/// probability than others, under certain circumstancies.
/// This doesn't matter in 99.9% of situations, but if you really
/// need a generator which is mathematically more robust
/// you should use another library for that part (see boost.random).
///
////////////////////////////////////////////////////////////
