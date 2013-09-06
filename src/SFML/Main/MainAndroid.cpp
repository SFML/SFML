////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007-2014 Laurent Gomila (laurent.gom@gmail.com)
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


#include <SFML/Config.hpp>

#ifdef SFML_SYSTEM_ANDROID

#include <SFML/Window/Keyboard.hpp>
#include <SFML/System/Sleep.hpp>
#include <SFML/System/Thread.hpp>
#include <SFML/System/Lock.hpp>
#include <SFML/Main/activity.hpp>

extern int main(int argc, char *argv[]);
JavaVM* javaVM;


namespace sf
{
namespace priv
{

Keyboard::Key mapAndroidKeyToSFMLKey(int32_t key)
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


void processEvent(ActivityStates* states)
{
    // The caller must ensure states can be safely accessed!

    AInputEvent* _event = NULL;

    if (AInputQueue_getEvent(states->inputQueue, &_event) >= 0)
    {
        if (AInputQueue_preDispatchEvent(states->inputQueue, _event))
            return;

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
                if (action == AKEY_EVENT_ACTION_DOWN)
                    event.type = Event::KeyPressed;
                else
                    event.type = Event::KeyReleased;

                event.key.code    = mapAndroidKeyToSFMLKey(key);
                event.key.alt     = metakey & AMETA_ALT_ON;
                event.key.control = metakey & AMETA_SHIFT_ON;
                event.key.shift   = false;

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
                     int historySize = AMotionEvent_getHistorySize(_event);
                     int pointerCount = AMotionEvent_getPointerCount(_event);
                     for (int h = 0; h < historySize; h++)
                     {
                        for (int p = 0; p < pointerCount; p++)
                        {
                            int id = AMotionEvent_getPointerId(_event, p);
                            float x = AMotionEvent_getHistoricalX(_event, p, h);
                            float y = AMotionEvent_getHistoricalY(_event, p, h);

                            sf::Event event;
                            event.type = Event::MouseMoved;
                            event.mouseButton.button = static_cast<Mouse::Button>(id);
                            event.mouseButton.x = x;
                            event.mouseButton.y = y;
                            states->pendingEvents.push_back(event);
                        }
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

                    sf::Event event;
                    event.type = Event::MouseButtonPressed;
                    event.mouseButton.button = static_cast<Mouse::Button>(id);
                    event.mouseButton.x = x;
                    event.mouseButton.y = y;
                    states->pendingEvents.push_back(event);

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

                    sf::Event event;
                    event.type = Event::MouseButtonReleased;
                    event.mouseButton.button = static_cast<Mouse::Button>(id);
                    event.mouseButton.x = x;
                    event.mouseButton.y = y;
                    states->pendingEvents.push_back(event);

                    break;
                }
            }

        }

        handled = 1;
        AInputQueue_finishEvent(states->inputQueue, _event, handled);
    }
}

void processSensorEvents(ActivityStates* states)
{
    // The caller must ensure states can be safely accessed!

    ASensorEvent _event;
    while (ASensorEventQueue_getEvents(states->sensorEventQueue, &_event, 1) > 0)
    {
        sf::Event event;
        event.type       = sf::Event::MouseWheelMoved;
        event.mouseWheel.delta = static_cast<int>(_event.acceleration.x*1000);
        event.mouseWheel.x     = static_cast<int>(_event.acceleration.y*1000);
        event.mouseWheel.y     = static_cast<int>(_event.acceleration.z*1000);
        states->pendingEvents.push_back(event);
    }
}

ActivityStates* getActivityStates(ActivityStates* initializedStates)
{
    static ActivityStates* states = NULL;

    if (!states)
        states = initializedStates;

    return states;
}

ActivityStates* retrieveStates(ANativeActivity* activity)
{
    // Hide the ugly cast we find in each callback function
    return (sf::priv::ActivityStates*)activity->instance;
}

static void initializeMain(ActivityStates* states)
{
    // Protect from concurent access
    sf::Lock lock(states->mutex);

    // Prepare and share the looper to be read later
    ALooper* looper = ALooper_prepare(ALOOPER_PREPARE_ALLOW_NON_CALLBACKS);
    states->looper = looper;

    // Prepare the sensor event queue
    states->sensorEventQueue = ASensorManager_createEventQueue(states->sensorManager, states->looper, 2, NULL, (void*)&sf::priv::processSensorEvents);

    // Get the default configuration
    states->config = AConfiguration_new();
    AConfiguration_fromAssetManager(states->config, states->activity->assetManager);

}

static void terminateMain(ActivityStates* states)
{
    // Protect from concurent access
    sf::Lock lock(states->mutex);

    // The main thread is over, we must explicitly ask the activity to finish
    states->mainOver = true;
    ANativeActivity_finish(states->activity);
}

void* main(ActivityStates* states)
{
    // Initialize the thread before giving the hand
    initializeMain(states);

    {
        sf::Lock lock(states->mutex);

        states->initialized = true;
    }

    sf::sleep(sf::seconds(0.5));
    ::main(0, NULL);

    // Terminate properly the thread and Wait until it's done
    terminateMain(states);

    {
        sf::Lock lock(states->mutex);

        states->terminated = true;
    }

    return NULL;
}

} // namespace priv
} // namespace sf

static void onStart(ANativeActivity* activity)
{
}

static void onResume(ANativeActivity* activity)
{
    // Retrieve our activity states from the activity instance
    sf::priv::ActivityStates* states = sf::priv::retrieveStates(activity);
    sf::Lock lock(states->mutex);

    // Send an event to warn people the activity has been resumed
    sf::Event event;
    event.type = sf::Event::MouseEntered;

    states->pendingEvents.push_back(event);
}

static void onPause(ANativeActivity* activity)
{
    // Retrieve our activity states from the activity instance
    sf::priv::ActivityStates* states = sf::priv::retrieveStates(activity);
    sf::Lock lock(states->mutex);

    // Send an event to warn people the activity has been paused
    sf::Event event;
    event.type = sf::Event::MouseLeft;

    states->pendingEvents.push_back(event);
}

static void onStop(ANativeActivity* activity)
{
}

static void onDestroy(ANativeActivity* activity)
{
    // Retrieve our activity states from the activity instance
    sf::priv::ActivityStates* states = sf::priv::retrieveStates(activity);

    // Send an event to warn people the activity is being destroyed
    {
        sf::Lock lock(states->mutex);

        // If the main thread hasn't yet finished, send the event and wait for
        // it to finish.
        if (!states->mainOver)
        {
            sf::Event event;
            event.type = sf::Event::Closed;

            states->pendingEvents.push_back(event);
        }
    }

    // Wait for the main thread to be terminated
    states->mutex.lock();

    while (!states->terminated)
    {
        states->mutex.unlock();
        sf::sleep(sf::milliseconds(20));
        states->mutex.lock();
    }

    states->mutex.unlock();

    // Terminate EGL display
    eglCheck(eglTerminate(states->display));

    // Delete our allocated states
    delete states;

    // The application should now terminate
}

static void onNativeWindowCreated(ANativeActivity* activity, ANativeWindow* window)
{
    sf::priv::ActivityStates* states = sf::priv::retrieveStates(activity);
    sf::Lock lock(states->mutex);

    // Update the activity states
    states->window = window;

    // Notify SFML mechanism
    sf::Event event;
    event.type = sf::Event::GainedFocus;
    states->pendingEvents.push_back(event);

    // Wait for the event to be taken into account by SFML
    states->updated = false;
    while(!states->updated)
    {
        states->mutex.unlock();
        sf::sleep(sf::milliseconds(10));
        states->mutex.lock();
    }
}

static void onNativeWindowDestroyed(ANativeActivity* activity, ANativeWindow* window)
{
    sf::priv::ActivityStates* states = sf::priv::retrieveStates(activity);
    sf::Lock lock(states->mutex);

    // Update the activity states
    states->window = NULL;

    // Notify SFML mechanism
    sf::Event event;
    event.type = sf::Event::LostFocus;
    states->pendingEvents.push_back(event);

    // Wait for the event to be taken into account by SFML
    states->updated = false;
    while(!states->updated)
    {
        states->mutex.unlock();
        sf::sleep(sf::milliseconds(10));
        states->mutex.lock();
    }
}

static void onNativeWindowRedrawNeeded(ANativeActivity* activity, ANativeWindow* window)
{
}

static void onNativeWindowResized(ANativeActivity* activity, ANativeWindow* window)
{
}

static void onInputQueueCreated(ANativeActivity* activity, AInputQueue* queue)
{
    // Retrieve our activity states from the activity instance
    sf::priv::ActivityStates* states = sf::priv::retrieveStates(activity);

    // Attach the input queue
    {
        sf::Lock lock(states->mutex);

        AInputQueue_attachLooper(queue, states->looper, 1, NULL, (void*)&sf::priv::processEvent);
        states->inputQueue = queue;
    }
}

static void onInputQueueDestroyed(ANativeActivity* activity, AInputQueue* queue)
{
    // Retrieve our activity states from the activity instance
    sf::priv::ActivityStates* states = sf::priv::retrieveStates(activity);

    // Detach the input queue
    {
        sf::Lock lock(states->mutex);

        states->inputQueue = NULL;
        AInputQueue_detachLooper(queue);
    }
}

static void onWindowFocusChanged(ANativeActivity* activity, int focused)
{
    // Retrieve our activity states from the activity instance
    sf::priv::ActivityStates* states = sf::priv::retrieveStates(activity);

    if (focused)
    {
        // We start monitoring the accelerometer with 60 events per second
        if (states->accelerometerSensor != NULL)
        {
            ASensorEventQueue_enableSensor(states->sensorEventQueue, states->accelerometerSensor);
            ASensorEventQueue_setEventRate(states->sensorEventQueue, states->accelerometerSensor, (1000L/60)*1000);
        }
    }
    else
    {
        // We stop monitoring the accelerometer (it avoids consuming battery)
        if (states->accelerometerSensor != NULL)
        {
            ASensorEventQueue_disableSensor(states->sensorEventQueue, states->accelerometerSensor);
        }
    }
}

static void onContentRectChanged(ANativeActivity* activity, const ARect* rect)
{
    // Retrieve our activity states from the activity instance
    sf::priv::ActivityStates* states = sf::priv::retrieveStates(activity);
    sf::Lock lock(states->mutex);

    // Send an event to warn people about the window move/resize
    sf::Event event;
    event.type = sf::Event::Resized;
    event.size.width = ANativeWindow_getWidth(states->window);
    event.size.height = ANativeWindow_getHeight(states->window);

    states->pendingEvents.push_back(event);
}

static void onConfigurationChanged(ANativeActivity* activity)
{
}

static void* onSaveInstanceState(ANativeActivity* activity, size_t* outLen)
{
    *outLen = 0;

    return NULL;
}

static void onLowMemory(ANativeActivity* activity)
{
}

void ANativeActivity_onCreate(ANativeActivity* activity, void* savedState, size_t savedStateSize)
{
    // Feed OpenAL-Soft's javaVM to make so he can function
    javaVM = activity->vm;

    // Create an activity states (will keep us in the know, about events we care)
    sf::priv::ActivityStates* states = NULL;
    states = new sf::priv::ActivityStates;

    // Initialize the states value
    states->activity   = NULL;
    states->window     = NULL;
    states->looper     = NULL;
    states->inputQueue = NULL;
    states->config     = NULL;

    states->sensorManager = ASensorManager_getInstance();
    states->accelerometerSensor = ASensorManager_getDefaultSensor(states->sensorManager, ASENSOR_TYPE_ACCELEROMETER);
    states->sensorEventQueue = NULL;

    states->display = eglCheck(eglGetDisplay(EGL_DEFAULT_DISPLAY));

    if (savedState != NULL) {
        states->savedState = malloc(savedStateSize);
        states->savedStateSize = savedStateSize;
        memcpy(states->savedState, savedState, savedStateSize);
    }

    states->mainOver = false;

    states->initialized = false;
    states->terminated = false;

    // Share it across the SFML modules
    sf::priv::getActivityStates(states);

    // These functions will update the activity states and therefore, will allow
    // SFML to be kept in the know
    activity->callbacks->onStart   = onStart;
    activity->callbacks->onResume  = onResume;
    activity->callbacks->onPause   = onPause;
    activity->callbacks->onStop    = onStop;
    activity->callbacks->onDestroy = onDestroy;

    activity->callbacks->onNativeWindowCreated = onNativeWindowCreated;
    activity->callbacks->onNativeWindowDestroyed = onNativeWindowDestroyed;
    activity->callbacks->onNativeWindowRedrawNeeded = onNativeWindowRedrawNeeded;
    activity->callbacks->onNativeWindowResized = onNativeWindowResized;

    activity->callbacks->onInputQueueCreated = onInputQueueCreated;
    activity->callbacks->onInputQueueDestroyed = onInputQueueDestroyed;

    activity->callbacks->onWindowFocusChanged = onWindowFocusChanged;
    activity->callbacks->onContentRectChanged = onContentRectChanged;
    activity->callbacks->onConfigurationChanged = onConfigurationChanged;

    activity->callbacks->onSaveInstanceState = onSaveInstanceState;
    activity->callbacks->onLowMemory = onLowMemory;

    // Share this activity with the callback functions
    states->activity = activity;

    // Initialize the display
    eglCheck(eglInitialize(states->display, NULL, NULL));

    // Launch the main thread
    sf::Thread* thread = new sf::Thread(sf::priv::main, states);
    thread->launch();

    // Wait for the main thread to be initialized
    states->mutex.lock();

    while (!states->initialized)
    {
        states->mutex.unlock();
        sf::sleep(sf::milliseconds(20));
        states->mutex.lock();
    }

    states->mutex.unlock();

    // Share this state with the callback functions
    activity->instance = states;
}

#endif // SFML_SYSTEM_ANDROID
