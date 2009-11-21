////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007 Laurent Gomila (laurent.gom@gmail.com)
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
#include <SFML/System/Unix/Platform.hpp>
#include <unistd.h>
#include <sys/time.h>


namespace sf
{
namespace priv
{
////////////////////////////////////////////////////////////
double Platform::GetSystemTime()
{
    timeval time = {0, 0};
    gettimeofday(&time, NULL);

    return time.tv_sec + time.tv_usec / 1000000.;
}


////////////////////////////////////////////////////////////
void Platform::Sleep(float time)
{
    usleep(static_cast<unsigned long>(time * 1000000));
}

} // namespace priv

} // namespace sf
