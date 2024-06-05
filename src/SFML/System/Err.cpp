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

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/System/Err.hpp>

#include <iostream>

#include <cstdio>


namespace sf
{
////////////////////////////////////////////////////////////
ErrStream::Guard::Guard(std::ostream& stream, std::unique_lock<std::mutex>&& lockGuard) :
m_stream(stream),
m_lockGuard(std::move(lockGuard))
{
}


////////////////////////////////////////////////////////////
ErrStream::Guard& ErrStream::Guard::operator<<(std::ostream& (*func)(std::ostream&))
{
    return this->operator<< <decltype(func)>(func);
}


////////////////////////////////////////////////////////////
ErrStream::ErrStream(std::streambuf* sbuf) : m_stream(sbuf)
{
}


////////////////////////////////////////////////////////////
ErrStream::Guard ErrStream::operator<<(std::ostream& (*func)(std::ostream&))
{
    return this->operator<< <decltype(func)>(func);
}


////////////////////////////////////////////////////////////
std::streambuf* ErrStream::rdbuf()
{
    const std::unique_lock lockGuard(m_mutex);
    return m_stream.rdbuf();
}


////////////////////////////////////////////////////////////
void ErrStream::rdbuf(std::streambuf* sbuf)
{
    const std::unique_lock lockGuard(m_mutex);
    m_stream.rdbuf(sbuf);
}


////////////////////////////////////////////////////////////
ErrStream& err()
{
    static ErrStream stream(std::cerr.rdbuf());
    return stream;
}

} // namespace sf
