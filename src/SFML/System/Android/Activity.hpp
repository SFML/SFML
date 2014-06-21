////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
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

#ifndef SFML_ACTIVITY_HPP
#define SFML_ACTIVITY_HPP

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Window/Event.hpp>
#include <SFML/Window/EglContext.hpp>
#include <SFML/System/Mutex.hpp>
#include <android/native_activity.h>
#include <android/configuration.h>
#include <EGL/egl.h>
#include <vector>
#include <map>


namespace sf
{
namespace priv
{
struct ActivityStates
{
    ANativeActivity* activity;
    ANativeWindow* window;

    ALooper*        looper;
    AInputQueue*    inputQueue;
    AConfiguration* config;

    EGLDisplay display;
    EglContext* context;

    void* savedState;
    size_t savedStateSize;

    Mutex mutex;

    std::vector<Event> pendingEvents;
    int (*processEvent)(int fd, int events, void* data);

    std::map<int, Vector2i> touchEvents;
    Vector2i mousePosition;
    bool isButtonPressed[Mouse::ButtonCount];

    bool mainOver;

    bool initialized;
    bool terminated;

    bool updated;
};

SFML_SYSTEM_API ActivityStates* getActivity(ActivityStates* initializedStates=NULL, bool reset=false);

} // namespace priv
} // namespace sf


#endif // SFML_ACTIVITY_HPP
