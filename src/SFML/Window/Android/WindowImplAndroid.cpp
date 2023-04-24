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
#include <SFML/Window/Android/WindowImplAndroid.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/WindowStyle.hpp>

#include <SFML/System/Err.hpp>

#include <android/looper.h>

#include <mutex>
#include <ostream>

// Define missing constants for older API levels
#if __ANDROID_API__ < 13
#define AMOTION_EVENT_ACTION_HOVER_MOVE 0x00000007
#define AMOTION_EVENT_ACTION_SCROLL     0x00000008
#endif

////////////////////////////////////////////////////////////
// Private data
////////////////////////////////////////////////////////////
namespace sf::priv
{
WindowImplAndroid* WindowImplAndroid::singleInstance = nullptr;

////////////////////////////////////////////////////////////
WindowImplAndroid::WindowImplAndroid(WindowHandle /* handle */)
{
}


////////////////////////////////////////////////////////////
WindowImplAndroid::WindowImplAndroid(VideoMode mode,
                                     const String& /* title */,
                                     unsigned long style,
                                     const ContextSettings& /* settings */) :
m_size(mode.size)
{
    ActivityStates& states = getActivity();
    std::lock_guard lock(states.mutex);

    if (style & Style::Fullscreen)
        states.fullscreen = true;

    WindowImplAndroid::singleInstance = this;
    states.forwardEvent               = forwardEvent;

    // Register process event callback
    states.processEvent = processEvent;

    states.initialized = true;
}


////////////////////////////////////////////////////////////
WindowImplAndroid::~WindowImplAndroid()
{
    WindowImplAndroid::singleInstance = nullptr;
}


////////////////////////////////////////////////////////////
WindowHandle WindowImplAndroid::getSystemHandle() const
{
    ActivityStates& states = getActivity();
    std::lock_guard lock(states.mutex);

    return states.window;
}


////////////////////////////////////////////////////////////
void WindowImplAndroid::processEvents()
{
    // Process incoming OS events
    ALooper_pollAll(0, nullptr, nullptr, nullptr);

    ActivityStates& states = getActivity();
    std::lock_guard lock(states.mutex);

    if (m_windowBeingCreated)
    {
        states.context->createSurface(states.window);
        m_windowBeingCreated = false;
    }

    if (m_windowBeingDestroyed)
    {
        states.context->destroySurface();
        m_windowBeingDestroyed = false;
    }

    states.updated = true;
}


////////////////////////////////////////////////////////////
Vector2i WindowImplAndroid::getPosition() const
{
    // Not applicable
    return Vector2i(0, 0);
}


////////////////////////////////////////////////////////////
void WindowImplAndroid::setPosition(const Vector2i& /* position */)
{
    // Not applicable
}


////////////////////////////////////////////////////////////
Vector2u WindowImplAndroid::getSize() const
{
    return m_size;
}


////////////////////////////////////////////////////////////
void WindowImplAndroid::setSize(const Vector2u& /* size */)
{
}


////////////////////////////////////////////////////////////
void WindowImplAndroid::setTitle(const String& /* title */)
{
    // Not applicable
}


////////////////////////////////////////////////////////////
void WindowImplAndroid::setIcon(const Vector2u& /* size */, const std::uint8_t* /* pixels */)
{
    // Not applicable
}


////////////////////////////////////////////////////////////
void WindowImplAndroid::setVisible(bool /* visible */)
{
    // Not applicable
}


////////////////////////////////////////////////////////////
void WindowImplAndroid::setMouseCursorVisible(bool /* visible */)
{
    // Not applicable
}


////////////////////////////////////////////////////////////
void WindowImplAndroid::setMouseCursorGrabbed(bool /* grabbed */)
{
    // Not applicable
}


////////////////////////////////////////////////////////////
void WindowImplAndroid::setMouseCursor(const CursorImpl& /* cursor */)
{
    // Not applicable
}


////////////////////////////////////////////////////////////
void WindowImplAndroid::setKeyRepeatEnabled(bool /* enabled */)
{
    // Not applicable
}


////////////////////////////////////////////////////////////
void WindowImplAndroid::requestFocus()
{
    // Not applicable
}


////////////////////////////////////////////////////////////
bool WindowImplAndroid::hasFocus() const
{
    return m_hasFocus;
}


////////////////////////////////////////////////////////////
void WindowImplAndroid::forwardEvent(const Event& event)
{
    if (WindowImplAndroid::singleInstance != nullptr)
    {
        ActivityStates& states = getActivity();

        if (event.type == Event::GainedFocus)
        {
            WindowImplAndroid::singleInstance->m_size.x = static_cast<unsigned int>(ANativeWindow_getWidth(states.window));
            WindowImplAndroid::singleInstance->m_size.y = static_cast<unsigned int>(ANativeWindow_getHeight(states.window));
            WindowImplAndroid::singleInstance->m_windowBeingCreated = true;
            WindowImplAndroid::singleInstance->m_hasFocus           = true;
        }
        else if (event.type == Event::LostFocus)
        {
            WindowImplAndroid::singleInstance->m_windowBeingDestroyed = true;
            WindowImplAndroid::singleInstance->m_hasFocus             = false;
        }

        WindowImplAndroid::singleInstance->pushEvent(event);
    }
}


////////////////////////////////////////////////////////////
int WindowImplAndroid::processEvent(int /* fd */, int /* events */, void* /* data */)
{
    ActivityStates& states = getActivity();
    std::lock_guard lock(states.mutex);

    AInputEvent* _event = nullptr;

    if (AInputQueue_getEvent(states.inputQueue, &_event) >= 0)
    {
        if (AInputQueue_preDispatchEvent(states.inputQueue, _event))
            return 1;

        int handled = 0;

        std::int32_t type = AInputEvent_getType(_event);

        if (type == AINPUT_EVENT_TYPE_KEY)
        {
            std::int32_t action = AKeyEvent_getAction(_event);
            std::int32_t key    = AKeyEvent_getKeyCode(_event);

            if ((action == AKEY_EVENT_ACTION_DOWN || action == AKEY_EVENT_ACTION_UP || action == AKEY_EVENT_ACTION_MULTIPLE) &&
                key != AKEYCODE_VOLUME_UP && key != AKEYCODE_VOLUME_DOWN)
            {
                handled = processKeyEvent(_event, states);
            }
        }
        else if (type == AINPUT_EVENT_TYPE_MOTION)
        {
            std::int32_t action = AMotionEvent_getAction(_event);

            switch (action & AMOTION_EVENT_ACTION_MASK)
            {
                case AMOTION_EVENT_ACTION_SCROLL:
                {
                    handled = processScrollEvent(_event, states);
                    break;
                }

                // todo: should hover_move indeed trigger the event?
                // case AMOTION_EVENT_ACTION_HOVER_MOVE:
                case AMOTION_EVENT_ACTION_MOVE:
                {
                    handled = processMotionEvent(_event, states);
                    break;
                }

                // todo: investigate AMOTION_EVENT_OUTSIDE
                case AMOTION_EVENT_ACTION_POINTER_DOWN:
                case AMOTION_EVENT_ACTION_DOWN:
                {
                    handled = processPointerEvent(true, _event, states);
                    break;
                }

                case AMOTION_EVENT_ACTION_POINTER_UP:
                case AMOTION_EVENT_ACTION_UP:
                case AMOTION_EVENT_ACTION_CANCEL:
                {
                    handled = processPointerEvent(false, _event, states);
                    break;
                }
            }
        }

        AInputQueue_finishEvent(states.inputQueue, _event, handled);
    }

    return 1;
}


////////////////////////////////////////////////////////////
int WindowImplAndroid::processScrollEvent(AInputEvent* _event, ActivityStates& states)
{
    // Prepare the Java virtual machine
    jint lResult;

    JavaVM* lJavaVM = states.activity->vm;
    JNIEnv* lJNIEnv = states.activity->env;

    JavaVMAttachArgs lJavaVMAttachArgs;
    lJavaVMAttachArgs.version = JNI_VERSION_1_6;
    lJavaVMAttachArgs.name    = "NativeThread";
    lJavaVMAttachArgs.group   = nullptr;

    lResult = lJavaVM->AttachCurrentThread(&lJNIEnv, &lJavaVMAttachArgs);

    if (lResult == JNI_ERR)
    {
        err() << "Failed to initialize JNI, couldn't get the Unicode value" << std::endl;
        return 0;
    }

    // Retrieve everything we need to create this MotionEvent in Java
    std::int64_t downTime   = AMotionEvent_getDownTime(_event);
    std::int64_t eventTime  = AMotionEvent_getEventTime(_event);
    std::int32_t action     = AMotionEvent_getAction(_event);
    float        x          = AMotionEvent_getX(_event, 0);
    float        y          = AMotionEvent_getY(_event, 0);
    float        pressure   = AMotionEvent_getPressure(_event, 0);
    float        size       = AMotionEvent_getSize(_event, 0);
    std::int32_t metaState  = AMotionEvent_getMetaState(_event);
    float        xPrecision = AMotionEvent_getXPrecision(_event);
    float        yPrecision = AMotionEvent_getYPrecision(_event);
    std::int32_t deviceId   = AInputEvent_getDeviceId(_event);
    std::int32_t edgeFlags  = AMotionEvent_getEdgeFlags(_event);

    // Create the MotionEvent object in Java trough its static constructor obtain()
    jclass    ClassMotionEvent   = lJNIEnv->FindClass("android/view/MotionEvent");
    jmethodID StaticMethodObtain = lJNIEnv->GetStaticMethodID(ClassMotionEvent,
                                                              "obtain",
                                                              "(JJIFFFFIFFII)Landroid/view/MotionEvent;");
    // Note: C standard compatibility, varargs
    //       automatically promote floats to doubles
    //       even though the function signature declares float
    jobject ObjectMotionEvent = lJNIEnv->CallStaticObjectMethod(ClassMotionEvent,
                                                                StaticMethodObtain,
                                                                downTime,
                                                                eventTime,
                                                                action,
                                                                static_cast<double>(x),
                                                                static_cast<double>(y),
                                                                static_cast<double>(pressure),
                                                                static_cast<double>(size),
                                                                metaState,
                                                                static_cast<double>(xPrecision),
                                                                static_cast<double>(yPrecision),
                                                                deviceId,
                                                                edgeFlags);

    // Call its getAxisValue() method to get the delta value of our wheel move event
    jmethodID MethodGetAxisValue = lJNIEnv->GetMethodID(ClassMotionEvent, "getAxisValue", "(I)F");
    jfloat    delta              = lJNIEnv->CallFloatMethod(ObjectMotionEvent, MethodGetAxisValue, 0x00000001);

    lJNIEnv->DeleteLocalRef(ClassMotionEvent);
    lJNIEnv->DeleteLocalRef(ObjectMotionEvent);

    // Create and send our mouse wheel event
    Event event;
    event.type                   = Event::MouseWheelScrolled;
    event.mouseWheelScroll.wheel = Mouse::VerticalWheel;
    event.mouseWheelScroll.delta = static_cast<float>(delta);
    event.mouseWheelScroll.x     = static_cast<int>(AMotionEvent_getX(_event, 0));
    event.mouseWheelScroll.y     = static_cast<int>(AMotionEvent_getY(_event, 0));

    forwardEvent(event);

    // Detach this thread from the JVM
    lJavaVM->DetachCurrentThread();

    return 1;
}


////////////////////////////////////////////////////////////
int WindowImplAndroid::processKeyEvent(AInputEvent* _event, ActivityStates& /* states */)
{
    std::int32_t action = AKeyEvent_getAction(_event);

    std::int32_t key     = AKeyEvent_getKeyCode(_event);
    std::int32_t metakey = AKeyEvent_getMetaState(_event);

    Event event;
    event.key.code    = androidKeyToSF(key);
    event.key.alt     = metakey & AMETA_ALT_ON;
    event.key.control = false;
    event.key.shift   = metakey & AMETA_SHIFT_ON;

    switch (action)
    {
        case AKEY_EVENT_ACTION_DOWN:
            event.type = Event::KeyPressed;
            forwardEvent(event);
            return 1;
        case AKEY_EVENT_ACTION_UP:
            event.type = Event::KeyReleased;
            forwardEvent(event);

            if (std::uint32_t unicode = static_cast<std::uint32_t>(getUnicode(_event)))
            {
                event.type         = Event::TextEntered;
                event.text.unicode = static_cast<std::uint32_t>(unicode);
                forwardEvent(event);
            }
            return 1;
        case AKEY_EVENT_ACTION_MULTIPLE:
            // Since complex inputs don't get separate key down/up events
            // both have to be faked at once
            event.type = Event::KeyPressed;
            forwardEvent(event);
            event.type = Event::KeyReleased;
            forwardEvent(event);

            // This requires some special treatment, since this might represent
            // a repetition of key presses or a complete sequence
            if (key == AKEYCODE_UNKNOWN)
            {
                // This is a unique sequence, which is not yet exposed in the NDK
                // https://code.google.com/p/android/issues/detail?id=33998
                return 0;
            }
            else if (std::uint32_t unicode = static_cast<std::uint32_t>(getUnicode(_event))) // This is a repeated sequence
            {
                event.type         = Event::TextEntered;
                event.text.unicode = static_cast<std::uint32_t>(unicode);

                std::int32_t repeats = AKeyEvent_getRepeatCount(_event);
                for (std::int32_t i = 0; i < repeats; ++i)
                    forwardEvent(event);
                return 1;
            }
            break;
    }
    return 0;
}


////////////////////////////////////////////////////////////
int WindowImplAndroid::processMotionEvent(AInputEvent* _event, ActivityStates& states)
{
    std::int32_t device = AInputEvent_getSource(_event);

    Event event;

    if (device == AINPUT_SOURCE_MOUSE)
        event.type = Event::MouseMoved;
    else if (static_cast<std::uint32_t>(device) & AINPUT_SOURCE_TOUCHSCREEN)
        event.type = Event::TouchMoved;

    std::size_t pointerCount = AMotionEvent_getPointerCount(_event);

    for (std::size_t p = 0; p < pointerCount; ++p)
    {
        std::int32_t id = AMotionEvent_getPointerId(_event, p);

        int x = static_cast<int>(AMotionEvent_getX(_event, p));
        int y = static_cast<int>(AMotionEvent_getY(_event, p));

        if (device == AINPUT_SOURCE_MOUSE)
        {
            event.mouseMove.x = x;
            event.mouseMove.y = y;

            states.mousePosition = Vector2i(event.mouseMove.x, event.mouseMove.y);
        }
        else if (static_cast<std::uint32_t>(device) & AINPUT_SOURCE_TOUCHSCREEN)
        {
            if (states.touchEvents[id].x == x && states.touchEvents[id].y == y)
                continue;

            event.touch.finger = static_cast<unsigned int>(id);
            event.touch.x      = x;
            event.touch.y      = y;

            states.touchEvents[id] = Vector2i(event.touch.x, event.touch.y);
        }

        forwardEvent(event);
    }
    return 1;
}


////////////////////////////////////////////////////////////
int WindowImplAndroid::processPointerEvent(bool isDown, AInputEvent* _event, ActivityStates& states)
{
    std::int32_t device = AInputEvent_getSource(_event);
    std::int32_t action = AMotionEvent_getAction(_event);

    std::size_t  index = (action & AMOTION_EVENT_ACTION_POINTER_INDEX_MASK) >> AMOTION_EVENT_ACTION_POINTER_INDEX_SHIFT;
    std::int32_t id    = AMotionEvent_getPointerId(_event, index);

    int x = static_cast<int>(AMotionEvent_getX(_event, index));
    int y = static_cast<int>(AMotionEvent_getY(_event, index));

    Event event;

    if (isDown)
    {
        if (device == AINPUT_SOURCE_MOUSE)
        {
            event.type               = Event::MouseButtonPressed;
            event.mouseButton.button = static_cast<Mouse::Button>(id);
            event.mouseButton.x      = x;
            event.mouseButton.y      = y;

            if (id >= 0 && id < Mouse::ButtonCount)
                states.isButtonPressed[id] = true;
        }
        else if (static_cast<unsigned int>(device) & AINPUT_SOURCE_TOUCHSCREEN)
        {
            event.type         = Event::TouchBegan;
            event.touch.finger = static_cast<unsigned int>(id);
            event.touch.x      = x;
            event.touch.y      = y;

            states.touchEvents[id] = Vector2i(event.touch.x, event.touch.y);
        }
    }
    else
    {
        if (device == AINPUT_SOURCE_MOUSE)
        {
            event.type               = Event::MouseButtonReleased;
            event.mouseButton.button = static_cast<Mouse::Button>(id);
            event.mouseButton.x      = x;
            event.mouseButton.y      = y;

            if (id >= 0 && id < Mouse::ButtonCount)
                states.isButtonPressed[id] = false;
        }
        else if (static_cast<std::uint32_t>(device) & AINPUT_SOURCE_TOUCHSCREEN)
        {
            event.type         = Event::TouchEnded;
            event.touch.finger = static_cast<unsigned int>(id);
            event.touch.x      = x;
            event.touch.y      = y;

            states.touchEvents.erase(id);
        }
    }

    forwardEvent(event);
    return 1;
}


////////////////////////////////////////////////////////////
Keyboard::Key WindowImplAndroid::androidKeyToSF(std::int32_t key)
{
    // clang-format off
    switch (key)
    {
        case AKEYCODE_UNKNOWN:
        case AKEYCODE_SOFT_LEFT:
        case AKEYCODE_SOFT_RIGHT:
        case AKEYCODE_HOME:               return Keyboard::Unknown;
        case AKEYCODE_BACK:               return Keyboard::Escape;
        case AKEYCODE_CALL:
        case AKEYCODE_ENDCALL:            return Keyboard::Unknown;
        case AKEYCODE_0:                  return Keyboard::Num0;
        case AKEYCODE_1:                  return Keyboard::Num1;
        case AKEYCODE_2:                  return Keyboard::Num2;
        case AKEYCODE_3:                  return Keyboard::Num3;
        case AKEYCODE_4:                  return Keyboard::Num4;
        case AKEYCODE_5:                  return Keyboard::Num5;
        case AKEYCODE_6:                  return Keyboard::Num6;
        case AKEYCODE_7:                  return Keyboard::Num7;
        case AKEYCODE_8:                  return Keyboard::Num8;
        case AKEYCODE_9:                  return Keyboard::Num9;
        case AKEYCODE_STAR:
        case AKEYCODE_POUND:
        case AKEYCODE_DPAD_UP:
        case AKEYCODE_DPAD_DOWN:
        case AKEYCODE_DPAD_LEFT:
        case AKEYCODE_DPAD_RIGHT:
        case AKEYCODE_DPAD_CENTER:
        case AKEYCODE_VOLUME_UP:
        case AKEYCODE_VOLUME_DOWN:
        case AKEYCODE_POWER:
        case AKEYCODE_CAMERA:
        case AKEYCODE_CLEAR:              return Keyboard::Unknown;
        case AKEYCODE_A:                  return Keyboard::A;
        case AKEYCODE_B:                  return Keyboard::B;
        case AKEYCODE_C:                  return Keyboard::C;
        case AKEYCODE_D:                  return Keyboard::D;
        case AKEYCODE_E:                  return Keyboard::E;
        case AKEYCODE_F:                  return Keyboard::F;
        case AKEYCODE_G:                  return Keyboard::G;
        case AKEYCODE_H:                  return Keyboard::H;
        case AKEYCODE_I:                  return Keyboard::I;
        case AKEYCODE_J:                  return Keyboard::J;
        case AKEYCODE_K:                  return Keyboard::K;
        case AKEYCODE_L:                  return Keyboard::L;
        case AKEYCODE_M:                  return Keyboard::M;
        case AKEYCODE_N:                  return Keyboard::N;
        case AKEYCODE_O:                  return Keyboard::O;
        case AKEYCODE_P:                  return Keyboard::P;
        case AKEYCODE_Q:                  return Keyboard::Q;
        case AKEYCODE_R:                  return Keyboard::R;
        case AKEYCODE_S:                  return Keyboard::S;
        case AKEYCODE_T:                  return Keyboard::T;
        case AKEYCODE_U:                  return Keyboard::U;
        case AKEYCODE_V:                  return Keyboard::V;
        case AKEYCODE_W:                  return Keyboard::W;
        case AKEYCODE_X:                  return Keyboard::X;
        case AKEYCODE_Y:                  return Keyboard::Y;
        case AKEYCODE_Z:                  return Keyboard::Z;
        case AKEYCODE_COMMA:              return Keyboard::Comma;
        case AKEYCODE_PERIOD:             return Keyboard::Period;
        case AKEYCODE_ALT_LEFT:           return Keyboard::LAlt;
        case AKEYCODE_ALT_RIGHT:          return Keyboard::RAlt;
        case AKEYCODE_SHIFT_LEFT:         return Keyboard::LShift;
        case AKEYCODE_SHIFT_RIGHT:        return Keyboard::RShift;
        case AKEYCODE_TAB:                return Keyboard::Tab;
        case AKEYCODE_SPACE:              return Keyboard::Space;
        case AKEYCODE_SYM:
        case AKEYCODE_EXPLORER:
        case AKEYCODE_ENVELOPE:           return Keyboard::Unknown;
        case AKEYCODE_ENTER:              return Keyboard::Enter;
        case AKEYCODE_DEL:                return Keyboard::Backspace;
        case AKEYCODE_FORWARD_DEL:        return Keyboard::Delete;
        case AKEYCODE_GRAVE:              return Keyboard::Grave;
        case AKEYCODE_MINUS:              return Keyboard::Subtract;
        case AKEYCODE_EQUALS:             return Keyboard::Equal;
        case AKEYCODE_LEFT_BRACKET:       return Keyboard::LBracket;
        case AKEYCODE_RIGHT_BRACKET:      return Keyboard::RBracket;
        case AKEYCODE_BACKSLASH:          return Keyboard::Backslash;
        case AKEYCODE_SEMICOLON:          return Keyboard::Semicolon;
        case AKEYCODE_APOSTROPHE:         return Keyboard::Apostrophe;
        case AKEYCODE_SLASH:              return Keyboard::Slash;
        case AKEYCODE_AT:
        case AKEYCODE_NUM:
        case AKEYCODE_HEADSETHOOK:
        case AKEYCODE_FOCUS: // *Camera* focus
        case AKEYCODE_PLUS:
        case AKEYCODE_MENU:
        case AKEYCODE_NOTIFICATION:
        case AKEYCODE_SEARCH:
        case AKEYCODE_MEDIA_PLAY_PAUSE:
        case AKEYCODE_MEDIA_STOP:
        case AKEYCODE_MEDIA_NEXT:
        case AKEYCODE_MEDIA_PREVIOUS:
        case AKEYCODE_MEDIA_REWIND:
        case AKEYCODE_MEDIA_FAST_FORWARD:
        case AKEYCODE_MUTE:               return Keyboard::Unknown;
        case AKEYCODE_PAGE_UP:            return Keyboard::PageUp;
        case AKEYCODE_PAGE_DOWN:          return Keyboard::PageDown;
        case AKEYCODE_PICTSYMBOLS:
        case AKEYCODE_SWITCH_CHARSET:
        case AKEYCODE_BUTTON_A:
        case AKEYCODE_BUTTON_B:
        case AKEYCODE_BUTTON_C:
        case AKEYCODE_BUTTON_X:
        case AKEYCODE_BUTTON_Y:
        case AKEYCODE_BUTTON_Z:
        case AKEYCODE_BUTTON_L1:
        case AKEYCODE_BUTTON_R1:
        case AKEYCODE_BUTTON_L2:
        case AKEYCODE_BUTTON_R2:
        case AKEYCODE_BUTTON_THUMBL:
        case AKEYCODE_BUTTON_THUMBR:
        case AKEYCODE_BUTTON_START:
        case AKEYCODE_BUTTON_SELECT:
        case AKEYCODE_BUTTON_MODE:
        default:                          return Keyboard::Unknown;
    }
    // clang-format on
}


////////////////////////////////////////////////////////////
int WindowImplAndroid::getUnicode(AInputEvent* event)
{
    // Retrieve activity states
    ActivityStates& states = getActivity();
    std::lock_guard lock(states.mutex);

    // Initializes JNI
    jint lResult;

    JavaVM* lJavaVM = states.activity->vm;
    JNIEnv* lJNIEnv = states.activity->env;

    JavaVMAttachArgs lJavaVMAttachArgs;
    lJavaVMAttachArgs.version = JNI_VERSION_1_6;
    lJavaVMAttachArgs.name    = "NativeThread";
    lJavaVMAttachArgs.group   = nullptr;

    lResult = lJavaVM->AttachCurrentThread(&lJNIEnv, &lJavaVMAttachArgs);

    if (lResult == JNI_ERR)
        err() << "Failed to initialize JNI, couldn't get the Unicode value" << std::endl;

    // Retrieve key data from the input event
    jlong downTime  = AKeyEvent_getDownTime(event);
    jlong eventTime = AKeyEvent_getEventTime(event);
    jint  action    = AKeyEvent_getAction(event);
    jint  code      = AKeyEvent_getKeyCode(event);
    jint  repeat    = AKeyEvent_getRepeatCount(event); // not sure!
    jint  metaState = AKeyEvent_getMetaState(event);
    jint  deviceId  = AInputEvent_getDeviceId(event);
    jint  scancode  = AKeyEvent_getScanCode(event);
    jint  flags     = AKeyEvent_getFlags(event);
    jint  source    = AInputEvent_getSource(event);

    // Construct a KeyEvent object from the event data
    jclass    ClassKeyEvent       = lJNIEnv->FindClass("android/view/KeyEvent");
    jmethodID KeyEventConstructor = lJNIEnv->GetMethodID(ClassKeyEvent, "<init>", "(JJIIIIIIII)V");
    jobject   ObjectKeyEvent      = lJNIEnv->NewObject(ClassKeyEvent,
                                                KeyEventConstructor,
                                                downTime,
                                                eventTime,
                                                action,
                                                code,
                                                repeat,
                                                metaState,
                                                deviceId,
                                                scancode,
                                                flags,
                                                source);

    // Call its getUnicodeChar() method to get the Unicode value
    jmethodID MethodGetUnicode = lJNIEnv->GetMethodID(ClassKeyEvent, "getUnicodeChar", "(I)I");
    int       unicode          = lJNIEnv->CallIntMethod(ObjectKeyEvent, MethodGetUnicode, metaState);

    lJNIEnv->DeleteLocalRef(ClassKeyEvent);
    lJNIEnv->DeleteLocalRef(ObjectKeyEvent);

    // Detach this thread from the JVM
    lJavaVM->DetachCurrentThread();

    return unicode;
}

} // namespace sf::priv
