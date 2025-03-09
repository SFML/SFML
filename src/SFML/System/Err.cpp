////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007-2025 Laurent Gomila (laurent@sfml-dev.org)
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
#include <SFML/System/Err.hpp>

#include <iostream>
#include <streambuf>

#include <cstdio>


namespace
{
// This class will be used as the default streambuf of sf::Err,
// it outputs to stderr by default (to keep the default behavior)
class DefaultErrStreamBuf : public std::streambuf
{
public:
    DefaultErrStreamBuf()
    {
        // Allocate the write buffer
        constexpr int size   = 64;
        char*         buffer = new char[size];
        setp(buffer, buffer + size);
    }

    ~DefaultErrStreamBuf() override
    {
        // Synchronize
        sync();

        // Delete the write buffer
        delete[] pbase();
    }

private:
    int overflow(int character) override
    {
        if ((character != EOF) && (pptr() != epptr()))
        {
            // Valid character
            return sputc(static_cast<char>(character));
        }
        if (character != EOF)
        {
            // Not enough space in the buffer: synchronize output and try again
            sync();
            return overflow(character);
        }

        // Invalid character: synchronize output
        return sync();
    }

    int sync() override
    {
        // Check if there is something into the write buffer
        if (pbase() != pptr())
        {
            // Print the contents of the write buffer into the standard error output
            const auto size = static_cast<std::size_t>(pptr() - pbase());
            std::fwrite(pbase(), 1, size, stderr);

            // Reset the pointer position to the beginning of the write buffer
            setp(pbase(), epptr());
        }

        return 0;
    }
};
} // namespace

namespace sf
{
////////////////////////////////////////////////////////////
std::ostream& err()
{
    static DefaultErrStreamBuf buffer;
    static std::ostream        stream(&buffer);

    return stream;
}


} // namespace sf
