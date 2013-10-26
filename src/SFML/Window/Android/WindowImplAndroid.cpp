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


////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Window/WindowStyle.hpp> // important to be included first (conflict with None)
#include <SFML/Window/Android/WindowImplAndroid.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/System/Android/Activity.hpp>
#include <SFML/System/Lock.hpp>
#include <android/looper.h>


////////////////////////////////////////////////////////////
// Private data
////////////////////////////////////////////////////////////
namespace sf
{
namespace priv
{
////////////////////////////////////////////////////////////
WindowImplAndroid::WindowImplAndroid(WindowHandle handle)
{
}


////////////////////////////////////////////////////////////
WindowImplAndroid::WindowImplAndroid(VideoMode mode, const std::string& title, unsigned long style, const ContextSettings& settings)
{
    ActivityStates* states = getActivity(NULL);
    sf::Lock lock(states->mutex);

    // Replace our dummy process event function with the actual one
    AInputQueue_detachLooper(states->inputQueue);
    AInputQueue_attachLooper(states->inputQueue, states->looper, 1, processEvent, NULL);

    // Register the actual process event function so it can be set automatically later by the OS
    states->processEvent = processEvent;
}


////////////////////////////////////////////////////////////
WindowImplAndroid::~WindowImplAndroid()
{
}


////////////////////////////////////////////////////////////
WindowHandle WindowImplAndroid::getSystemHandle() const
{
    ActivityStates* states = getActivity(NULL);
    sf::Lock lock(states->mutex);

    return states->window;
}


////////////////////////////////////////////////////////////
void WindowImplAndroid::processEvents()
{
    ActivityStates* states = getActivity(NULL);
    sf::Lock lock(states->mutex);

    // Process incoming OS events
    ALooper_pollAll(0, NULL, NULL, NULL);

    while (!states->pendingEvents.empty())
    {
        Event tempEvent = states->pendingEvents.back();
        states->pendingEvents.pop_back();

        if (tempEvent.type == sf::Event::Resized)
        {
            m_width = tempEvent.size.width;
            m_height = tempEvent.size.height;
        }
        else if (tempEvent.type == Event::GainedFocus)
        {
            states->context->createSurface(states->window);
            states->updated = true;
        }
        else if (tempEvent.type == Event::LostFocus)
        {
            states->context->destroySurface();
            states->updated = true;
        }

        pushEvent(tempEvent);
    }
}


////////////////////////////////////////////////////////////
Vector2i WindowImplAndroid::getPosition() const
{
    // Not applicable
    return Vector2i(0, 0);
}


////////////////////////////////////////////////////////////
void WindowImplAndroid::setPosition(const Vector2i& position)
{
    // Not applicable
}


////////////////////////////////////////////////////////////
Vector2u WindowImplAndroid::getSize() const
{
    return Vector2u(static_cast<unsigned int>(m_width), static_cast<unsigned int>(m_height));
}


////////////////////////////////////////////////////////////
void WindowImplAndroid::setSize(const Vector2u& size)
{
}


////////////////////////////////////////////////////////////
void WindowImplAndroid::setTitle(const String& title)
{
    // Not applicable
}


////////////////////////////////////////////////////////////
void WindowImplAndroid::setIcon(unsigned int width, unsigned int height, const Uint8* pixels)
{
    // Not applicable
}


////////////////////////////////////////////////////////////
void WindowImplAndroid::setVisible(bool visible)
{
}


////////////////////////////////////////////////////////////
void WindowImplAndroid::setMouseCursorVisible(bool visible)
{
}


////////////////////////////////////////////////////////////
void WindowImplAndroid::setKeyRepeatEnabled(bool enabled)
{
}

Keyboard::Key WindowImplAndroid::androidKeyToSF(int32_t key)
{
    switch (key)
    {
        case AKEYCODE_UNKNOWN            :
        case AKEYCODE_SOFT_LEFT          :
        case AKEYCODE_SOFT_RIGHT         :
        case AKEYCODE_HOME               :
        case AKEYCODE_BACK               :
        case AKEYCODE_CALL               :
        case AKEYCODE_ENDCALL            : return Keyboard::Unknown;
        case AKEYCODE_0                  : return Keyboard::Num0;
        case AKEYCODE_1                  : return Keyboard::Num1;
        case AKEYCODE_2                  : return Keyboard::Num2;
        case AKEYCODE_3                  : return Keyboard::Num3;
        case AKEYCODE_4                  : return Keyboard::Num4;
        case AKEYCODE_5                  : return Keyboard::Num5;
        case AKEYCODE_6                  : return Keyboard::Num6;
        case AKEYCODE_7                  : return Keyboard::Num7;
        case AKEYCODE_8                  : return Keyboard::Num8;
        case AKEYCODE_9                  : return Keyboard::Num9;
        case AKEYCODE_STAR               :
        case AKEYCODE_POUND              :
        case AKEYCODE_DPAD_UP            :
        case AKEYCODE_DPAD_DOWN          :
        case AKEYCODE_DPAD_LEFT          :
        case AKEYCODE_DPAD_RIGHT         :
        case AKEYCODE_DPAD_CENTER        :
        case AKEYCODE_VOLUME_UP          :
        case AKEYCODE_VOLUME_DOWN        :
        case AKEYCODE_POWER              :
        case AKEYCODE_CAMERA             :
        case AKEYCODE_CLEAR              : return Keyboard::Unknown;
        case AKEYCODE_A                  : return Keyboard::A;
        case AKEYCODE_B                  : return Keyboard::B;
        case AKEYCODE_C                  : return Keyboard::C;
        case AKEYCODE_D                  : return Keyboard::D;
        case AKEYCODE_E                  : return Keyboard::E;
        case AKEYCODE_F                  : return Keyboard::F;
        case AKEYCODE_G                  : return Keyboard::G;
        case AKEYCODE_H                  : return Keyboard::H;
        case AKEYCODE_I                  : return Keyboard::I;
        case AKEYCODE_J                  : return Keyboard::J;
        case AKEYCODE_K                  : return Keyboard::K;
        case AKEYCODE_L                  : return Keyboard::L;
        case AKEYCODE_M                  : return Keyboard::M;
        case AKEYCODE_N                  : return Keyboard::N;
        case AKEYCODE_O                  : return Keyboard::O;
        case AKEYCODE_P                  : return Keyboard::P;
        case AKEYCODE_Q                  : return Keyboard::Q;
        case AKEYCODE_R                  : return Keyboard::R;
        case AKEYCODE_S                  : return Keyboard::S;
        case AKEYCODE_T                  : return Keyboard::T;
        case AKEYCODE_U                  : return Keyboard::U;
        case AKEYCODE_V                  : return Keyboard::V;
        case AKEYCODE_W                  : return Keyboard::W;
        case AKEYCODE_X                  : return Keyboard::X;
        case AKEYCODE_Y                  : return Keyboard::Y;
        case AKEYCODE_Z                  : return Keyboard::Z;
        case AKEYCODE_COMMA              : return Keyboard::Comma;
        case AKEYCODE_PERIOD             : return Keyboard::Period;
        case AKEYCODE_ALT_LEFT           : return Keyboard::LAlt;
        case AKEYCODE_ALT_RIGHT          : return Keyboard::RAlt;
        case AKEYCODE_SHIFT_LEFT         : return Keyboard::LShift;
        case AKEYCODE_SHIFT_RIGHT        : return Keyboard::RShift;
        case AKEYCODE_TAB                : return Keyboard::Tab;
        case AKEYCODE_SPACE              : return Keyboard::Space;
        case AKEYCODE_SYM                :
        case AKEYCODE_EXPLORER           :
        case AKEYCODE_ENVELOPE           : return Keyboard::Unknown;
        case AKEYCODE_ENTER              : return Keyboard::Return;
        case AKEYCODE_DEL                : return Keyboard::Delete;
        case AKEYCODE_GRAVE              : return Keyboard::Tilde;
        case AKEYCODE_MINUS              : return Keyboard::Subtract;
        case AKEYCODE_EQUALS             : return Keyboard::Equal;
        case AKEYCODE_LEFT_BRACKET       : return Keyboard::LBracket;
        case AKEYCODE_RIGHT_BRACKET      : return Keyboard::RBracket;
        case AKEYCODE_BACKSLASH          : return Keyboard::BackSlash;
        case AKEYCODE_SEMICOLON          : return Keyboard::SemiColon;
        case AKEYCODE_APOSTROPHE         : return Keyboard::Quote;
        case AKEYCODE_SLASH              : return Keyboard::Slash;
        case AKEYCODE_AT                 :
        case AKEYCODE_NUM                :
        case AKEYCODE_HEADSETHOOK        :
        case AKEYCODE_FOCUS              : // *Camera* focus
        case AKEYCODE_PLUS               :
        case AKEYCODE_MENU               :
        case AKEYCODE_NOTIFICATION       :
        case AKEYCODE_SEARCH             :
        case AKEYCODE_MEDIA_PLAY_PAUSE   :
        case AKEYCODE_MEDIA_STOP         :
        case AKEYCODE_MEDIA_NEXT         :
        case AKEYCODE_MEDIA_PREVIOUS     :
        case AKEYCODE_MEDIA_REWIND       :
        case AKEYCODE_MEDIA_FAST_FORWARD :
        case AKEYCODE_MUTE               : return Keyboard::Unknown;
        case AKEYCODE_PAGE_UP            : return Keyboard::PageUp;
        case AKEYCODE_PAGE_DOWN          : return Keyboard::PageDown;
        case AKEYCODE_PICTSYMBOLS        :
        case AKEYCODE_SWITCH_CHARSET     :
        case AKEYCODE_BUTTON_A           :
        case AKEYCODE_BUTTON_B           :
        case AKEYCODE_BUTTON_C           :
        case AKEYCODE_BUTTON_X           :
        case AKEYCODE_BUTTON_Y           :
        case AKEYCODE_BUTTON_Z           :
        case AKEYCODE_BUTTON_L1          :
        case AKEYCODE_BUTTON_R1          :
        case AKEYCODE_BUTTON_L2          :
        case AKEYCODE_BUTTON_R2          :
        case AKEYCODE_BUTTON_THUMBL      :
        case AKEYCODE_BUTTON_THUMBR      :
        case AKEYCODE_BUTTON_START       :
        case AKEYCODE_BUTTON_SELECT      :
        case AKEYCODE_BUTTON_MODE        : return Keyboard::Unknown;
    }
}

int WindowImplAndroid::processEvent(int fd, int events, void* data)
{
    ActivityStates* states = getActivity(NULL);
    Lock lock(states->mutex);

    AInputEvent* _event = NULL;

    if (AInputQueue_getEvent(states->inputQueue, &_event) >= 0)
    {
        if (AInputQueue_preDispatchEvent(states->inputQueue, _event))
            return 1;

        int32_t handled = 0;

        int32_t type = AInputEvent_getType(_event);

        if (type == AINPUT_EVENT_TYPE_KEY)
        {
            int32_t action = AKeyEvent_getAction(_event);

            if (action == AKEY_EVENT_ACTION_DOWN || action == AKEY_EVENT_ACTION_UP)
            {
                int32_t key = AKeyEvent_getKeyCode(_event);
                int32_t metakey = AKeyEvent_getMetaState(_event);

                sf::Event event;
                event.type = (action == AKEY_EVENT_ACTION_DOWN) ? Event::KeyPressed : Event::KeyReleased;
                event.key.code    = androidKeyToSF(key);
                event.key.alt     = metakey & AMETA_ALT_ON;
                event.key.control = false;
                event.key.shift   = metakey & AMETA_SHIFT_ON;

                states->pendingEvents.push_back(event);
            }
        }
        else if (type == AINPUT_EVENT_TYPE_MOTION)
        {
            int32_t action = AMotionEvent_getAction(_event);

            switch (action & AMOTION_EVENT_ACTION_MASK)
            {
                case AMOTION_EVENT_ACTION_MOVE:
                {
                    int pointerCount = AMotionEvent_getPointerCount(_event);

                    Event event;
                    event.type = Event::TouchMoved;

                    for (int p = 0; p < pointerCount; p++)
                    {
                        int id = AMotionEvent_getPointerId(_event, p);
                        event.touch.finger = id;

                        float x = AMotionEvent_getX(_event, p);
                        float y = AMotionEvent_getY(_event, p);

                        event.touch.x = x;
                        event.touch.y = y;
                        states->pendingEvents.push_back(event);

                        states->touchEvents[id] = Vector2i(event.touch.x, event.touch.y);
                     }

                    break;
                }

                case AMOTION_EVENT_ACTION_POINTER_DOWN:
                case AMOTION_EVENT_ACTION_DOWN:
                {
                    int index = (action & AMOTION_EVENT_ACTION_POINTER_INDEX_MASK) >> AMOTION_EVENT_ACTION_POINTER_INDEX_SHIFT;
                    int id = AMotionEvent_getPointerId(_event, index);

                    float x = AMotionEvent_getX(_event, 0);
                    float y = AMotionEvent_getY(_event, 0);

                    Event event;
                    event.type = Event::TouchBegan;
                    event.touch.finger = id;
                    event.touch.x = x;
                    event.touch.y = y;
                    states->pendingEvents.push_back(event);


                    states->touchEvents[id] = Vector2i(x, y);

                    break;
                }

                case AMOTION_EVENT_ACTION_POINTER_UP:
                case AMOTION_EVENT_ACTION_UP:
                case AMOTION_EVENT_ACTION_CANCEL:
                {
                    int index = (action & AMOTION_EVENT_ACTION_POINTER_INDEX_MASK) >> AMOTION_EVENT_ACTION_POINTER_INDEX_SHIFT;
                    int id = AMotionEvent_getPointerId(_event, index);

                    float x = AMotionEvent_getX(_event, 0);
                    float y = AMotionEvent_getY(_event, 0);

                    Event event;
                    event.type = Event::TouchEnded;
                    event.touch.finger = id;
                    event.touch.x = x;
                    event.touch.y = y;
                    states->pendingEvents.push_back(event);


                    states->touchEvents.erase(id);

                    break;
                }
            }

        }

        handled = 1;
        AInputQueue_finishEvent(states->inputQueue, _event, handled);
    }

    return 1;
}
} // namespace priv
} // namespace sf
