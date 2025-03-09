////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007-2025 Laurent Gomila (laurent@sfml-dev.org)
// Copyright (C) 2013 Jonathan De Wachter (dewachter.jonathan@gmail.com)
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
#include <SFML/System/Android/Activity.hpp>

#include <android/log.h>

#include <cassert>

#define LOGE(...) ((void)__android_log_print(ANDROID_LOG_INFO, "sfml-error", __VA_ARGS__))

std::streambuf::int_type LogcatStream::overflow(std::streambuf::int_type c)
{
    if (c == '\n')
    {
        m_message.push_back(static_cast<char>(c));
        LOGE("%s", m_message.c_str());
        m_message.clear();
    }

    m_message.push_back(static_cast<char>(c));

    return traits_type::not_eof(c);
}

namespace sf::priv
{

ActivityStates*& getActivityStatesPtr()
{
    static ActivityStates* states = nullptr;
    return states;
}

void resetActivity(ActivityStates* initializedStates)
{
    getActivityStatesPtr() = initializedStates;
}

ActivityStates& getActivity()
{
    ActivityStates* const states = getActivityStatesPtr();
    assert(states != nullptr &&
           "Cannot dereference null activity states pointer. Call priv::resetActivity() to initialize it.");
    return *states;
}

} // namespace sf::priv
