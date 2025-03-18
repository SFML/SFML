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
// Android specific: we define the ANativeActivity_onCreate
// entry point, handling all the native activity stuff, then
// we call the user defined (and portable) main function in
// an external thread so developers can keep a portable code
////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Config.hpp>

#include <SFML/System/Android/Activity.hpp>
#include <SFML/System/Err.hpp>
#include <SFML/System/Sleep.hpp>
#include <SFML/System/Time.hpp>

#include <android/native_activity.h>
#include <android/window.h>

#include <mutex>
#include <thread>

#include <cassert>
#include <cstring>

#define SF_GLAD_EGL_IMPLEMENTATION
#include <glad/egl.h>


extern int main(int argc, char* argv[]);

namespace
{
////////////////////////////////////////////////////////////
void initializeMain(sf::priv::ActivityStates& states)
{
    // Protect from concurrent access
    const std::lock_guard lock(states.mutex);

    // Prepare and share the looper to be read later
    ALooper* looper = ALooper_prepare(ALOOPER_PREPARE_ALLOW_NON_CALLBACKS);
    states.looper   = looper;

    /**
     * Acquire increments a reference counter on the looper. This keeps android
     * from collecting it before the activity thread has a chance to detach its
     * input queue.
     */
    ALooper_acquire(states.looper);
    ALooper_wake(states.looper);

    // Get the default configuration
    states.config = AConfiguration_new();
    AConfiguration_fromAssetManager(states.config, states.activity->assetManager);
}

////////////////////////////////////////////////////////////
void terminateMain(sf::priv::ActivityStates& states)
{
    // Protect from concurrent access
    const std::lock_guard lock(states.mutex);

    // The main thread has finished, we must explicitly ask the activity to finish
    states.mainOver = true;
    ANativeActivity_finish(states.activity);
}

////////////////////////////////////////////////////////////
void onStart(ANativeActivity* /* activity */)
{
}

////////////////////////////////////////////////////////////
int getAndroidApiLevel(ANativeActivity& activity)
{
    JNIEnv* lJNIEnv = activity.env;

    jclass versionClass = lJNIEnv->FindClass("android/os/Build$VERSION");
    if (versionClass == nullptr)
        return 0;

    jfieldID sdkIntFieldID = lJNIEnv->GetStaticFieldID(versionClass, "SDK_INT", "I");
    if (sdkIntFieldID == nullptr)
        return 0;

    jint sdkInt = 0;
    sdkInt      = lJNIEnv->GetStaticIntField(versionClass, sdkIntFieldID);

    return sdkInt;
}

////////////////////////////////////////////////////////////
sf::priv::ActivityStates& retrieveStates(ANativeActivity& activity)
{
    // Hide the ugly cast we find in each callback function
    return *static_cast<sf::priv::ActivityStates*>(activity.instance);
}

////////////////////////////////////////////////////////////
void goToFullscreenMode(ANativeActivity& activity)
{
    // Get the current Android API level.
    const int apiLevel = getAndroidApiLevel(activity);

    // Hide the status bar
    ANativeActivity_setWindowFlags(&activity, AWINDOW_FLAG_FULLSCREEN, AWINDOW_FLAG_FULLSCREEN);

    // Hide the navigation bar
    JNIEnv* lJNIEnv = activity.env;

    jobject objectActivity = activity.clazz;
    jclass  classActivity  = lJNIEnv->GetObjectClass(objectActivity);

    jmethodID methodGetWindow = lJNIEnv->GetMethodID(classActivity, "getWindow", "()Landroid/view/Window;");
    jobject   objectWindow    = lJNIEnv->CallObjectMethod(objectActivity, methodGetWindow);

    jclass    classWindow        = lJNIEnv->FindClass("android/view/Window");
    jmethodID methodGetDecorView = lJNIEnv->GetMethodID(classWindow, "getDecorView", "()Landroid/view/View;");
    jobject   objectDecorView    = lJNIEnv->CallObjectMethod(objectWindow, methodGetDecorView);

    jclass classView = lJNIEnv->FindClass("android/view/View");

    // Default flags
    jint flags = 0;

    // API Level 14
    if (apiLevel >= 14)
    {
        jfieldID   fieldSystemUiFlagLowProfile = lJNIEnv->GetStaticFieldID(classView,
                                                                         "SYSTEM_UI_FLAG_HIDE_NAVIGATION",
                                                                         "I");
        const jint systemUiFlagLowProfile      = lJNIEnv->GetStaticIntField(classView, fieldSystemUiFlagLowProfile);
        flags |= systemUiFlagLowProfile;
    }

    // API Level 16
    if (apiLevel >= 16)
    {
        jfieldID   fieldSystemUiFlagFullscreen = lJNIEnv->GetStaticFieldID(classView, "SYSTEM_UI_FLAG_FULLSCREEN", "I");
        const jint systemUiFlagFullscreen      = lJNIEnv->GetStaticIntField(classView, fieldSystemUiFlagFullscreen);
        flags |= systemUiFlagFullscreen;
    }

    // API Level 19
    if (apiLevel >= 19)
    {
        jfieldID fieldSystemUiFlagImmersiveSticky = lJNIEnv->GetStaticFieldID(classView,
                                                                              "SYSTEM_UI_FLAG_IMMERSIVE_STICKY",
                                                                              "I");
        const jint systemUiFlagImmersiveSticky = lJNIEnv->GetStaticIntField(classView, fieldSystemUiFlagImmersiveSticky);
        flags |= systemUiFlagImmersiveSticky;
    }

    jmethodID methodsetSystemUiVisibility = lJNIEnv->GetMethodID(classView, "setSystemUiVisibility", "(I)V");
    lJNIEnv->CallVoidMethod(objectDecorView, methodsetSystemUiVisibility, flags);
}

////////////////////////////////////////////////////////////
void getScreenSizeInPixels(ANativeActivity& activity, int& width, int& height)
{
    // Perform the following Java code:
    //
    // DisplayMetrics dm = new DisplayMetrics();
    // getWindowManager().getDefaultDisplay().getMetrics(dm);

    JNIEnv* lJNIEnv = activity.env;

    jobject objectActivity = activity.clazz;
    jclass  classActivity  = lJNIEnv->GetObjectClass(objectActivity);

    jclass    classDisplayMetrics  = lJNIEnv->FindClass("android/util/DisplayMetrics");
    jmethodID initDisplayMetrics   = lJNIEnv->GetMethodID(classDisplayMetrics, "<init>", "()V");
    jobject   objectDisplayMetrics = lJNIEnv->NewObject(classDisplayMetrics, initDisplayMetrics);

    jmethodID methodGetWindowManager = lJNIEnv->GetMethodID(classActivity,
                                                            "getWindowManager",
                                                            "()Landroid/view/WindowManager;");
    jobject   objectWindowManager    = lJNIEnv->CallObjectMethod(objectActivity, methodGetWindowManager);

    jclass    classWindowManager      = lJNIEnv->FindClass("android/view/WindowManager");
    jmethodID methodGetDefaultDisplay = lJNIEnv->GetMethodID(classWindowManager,
                                                             "getDefaultDisplay",
                                                             "()Landroid/view/Display;");
    jobject   objectDisplay           = lJNIEnv->CallObjectMethod(objectWindowManager, methodGetDefaultDisplay);

    jclass    classDisplay     = lJNIEnv->FindClass("android/view/Display");
    jmethodID methodGetMetrics = lJNIEnv->GetMethodID(classDisplay, "getMetrics", "(Landroid/util/DisplayMetrics;)V");
    lJNIEnv->CallVoidMethod(objectDisplay, methodGetMetrics, objectDisplayMetrics);

    jfieldID fieldWidthPixels  = lJNIEnv->GetFieldID(classDisplayMetrics, "widthPixels", "I");
    jfieldID fieldHeightPixels = lJNIEnv->GetFieldID(classDisplayMetrics, "heightPixels", "I");

    width  = lJNIEnv->GetIntField(objectDisplayMetrics, fieldWidthPixels);
    height = lJNIEnv->GetIntField(objectDisplayMetrics, fieldHeightPixels);
}


////////////////////////////////////////////////////////////
void getFullScreenSizeInPixels(ANativeActivity& activity, int& width, int& height)
{
    // Perform the following Java code:
    //
    // DisplayMetrics dm = new DisplayMetrics();
    // getWindowManager().getDefaultDisplay().getRealMetrics(dm);

    JNIEnv* lJNIEnv = activity.env;

    jobject objectActivity = activity.clazz;
    jclass  classActivity  = lJNIEnv->GetObjectClass(objectActivity);

    jclass    classDisplayMetrics  = lJNIEnv->FindClass("android/util/DisplayMetrics");
    jmethodID initDisplayMetrics   = lJNIEnv->GetMethodID(classDisplayMetrics, "<init>", "()V");
    jobject   objectDisplayMetrics = lJNIEnv->NewObject(classDisplayMetrics, initDisplayMetrics);

    jmethodID methodGetWindowManager = lJNIEnv->GetMethodID(classActivity,
                                                            "getWindowManager",
                                                            "()Landroid/view/WindowManager;");
    jobject   objectWindowManager    = lJNIEnv->CallObjectMethod(objectActivity, methodGetWindowManager);

    jclass    classWindowManager      = lJNIEnv->FindClass("android/view/WindowManager");
    jmethodID methodGetDefaultDisplay = lJNIEnv->GetMethodID(classWindowManager,
                                                             "getDefaultDisplay",
                                                             "()Landroid/view/Display;");
    jobject   objectDisplay           = lJNIEnv->CallObjectMethod(objectWindowManager, methodGetDefaultDisplay);

    jclass    classDisplay     = lJNIEnv->FindClass("android/view/Display");
    jmethodID methodGetMetrics = nullptr;

    // getRealMetrics is only supported on API level 17 and above, if we are below that, we will fall back to getMetrics
    if (getAndroidApiLevel(activity) >= 17)
        methodGetMetrics = lJNIEnv->GetMethodID(classDisplay, "getRealMetrics", "(Landroid/util/DisplayMetrics;)V");
    else
        methodGetMetrics = lJNIEnv->GetMethodID(classDisplay, "getMetrics", "(Landroid/util/DisplayMetrics;)V");
    lJNIEnv->CallVoidMethod(objectDisplay, methodGetMetrics, objectDisplayMetrics);

    jfieldID fieldWidthPixels  = lJNIEnv->GetFieldID(classDisplayMetrics, "widthPixels", "I");
    jfieldID fieldHeightPixels = lJNIEnv->GetFieldID(classDisplayMetrics, "heightPixels", "I");

    width  = lJNIEnv->GetIntField(objectDisplayMetrics, fieldWidthPixels);
    height = lJNIEnv->GetIntField(objectDisplayMetrics, fieldHeightPixels);
}


////////////////////////////////////////////////////////////
void onResume(ANativeActivity* activity)
{
    // Retrieve our activity states from the activity instance
    sf::priv::ActivityStates& states = retrieveStates(*activity);
    const std::lock_guard     lock(states.mutex);

    if (states.fullscreen)
        goToFullscreenMode(*activity);

    // Send an event to warn people the activity has been resumed
    states.forwardEvent(sf::Event::MouseEntered{});
}

////////////////////////////////////////////////////////////
void onPause(ANativeActivity* activity)
{
    // Retrieve our activity states from the activity instance
    sf::priv::ActivityStates& states = retrieveStates(*activity);
    const std::lock_guard     lock(states.mutex);

    // Send an event to warn people the activity has been paused
    states.forwardEvent(sf::Event::MouseLeft{});
}

////////////////////////////////////////////////////////////
void onStop(ANativeActivity* /* activity */)
{
}

////////////////////////////////////////////////////////////
void onDestroy(ANativeActivity* activity)
{
    // Retrieve our activity states from the activity instance
    sf::priv::ActivityStates& states = retrieveStates(*activity);

    // Send an event to warn people the activity is being destroyed
    {
        const std::lock_guard lock(states.mutex);

        // If the main thread hasn't yet finished, send the event and wait for
        // it to finish.
        if (!states.mainOver)
            states.forwardEvent(sf::Event::Closed{});
    }

    // Wait for the main thread to be terminated
    states.mutex.lock();

    while (!states.terminated)
    {
        states.mutex.unlock();
        sf::sleep(sf::milliseconds(20));
        states.mutex.lock();
    }

    states.mutex.unlock();

    // Terminate EGL display
    eglTerminate(states.display);

    // Delete our allocated states
    delete &states;

    // Reset the activity pointer for all modules
    sf::priv::resetActivity(nullptr);

    // The application should now terminate
}

////////////////////////////////////////////////////////////
void onNativeWindowCreated(ANativeActivity* activity, ANativeWindow* window)
{
    sf::priv::ActivityStates& states = retrieveStates(*activity);
    const std::lock_guard     lock(states.mutex);

    // Update the activity states
    states.window = window;

    // Notify SFML mechanism
    states.forwardEvent(sf::Event::FocusGained{});

    // Wait for the event to be taken into account by SFML
    states.updated = false;
    while (!(states.updated | states.terminated))
    {
        states.mutex.unlock();
        sf::sleep(sf::milliseconds(10));
        states.mutex.lock();
    }
}

////////////////////////////////////////////////////////////
void onNativeWindowDestroyed(ANativeActivity* activity, ANativeWindow* /* window */)
{
    sf::priv::ActivityStates& states = retrieveStates(*activity);
    const std::lock_guard     lock(states.mutex);

    // Update the activity states
    states.window = nullptr;

    // Notify SFML mechanism
    states.forwardEvent(sf::Event::FocusLost{});

    // Wait for the event to be taken into account by SFML
    states.updated = false;
    while (!(states.updated | states.terminated))
    {
        states.mutex.unlock();
        sf::sleep(sf::milliseconds(10));
        states.mutex.lock();
    }
}

////////////////////////////////////////////////////////////
void onNativeWindowRedrawNeeded(ANativeActivity* /* activity */, ANativeWindow* /* window */)
{
}

////////////////////////////////////////////////////////////
void onNativeWindowResized(ANativeActivity* /* activity */, ANativeWindow* /* window */)
{
}

////////////////////////////////////////////////////////////
void onInputQueueCreated(ANativeActivity* activity, AInputQueue* queue)
{
    // Retrieve our activity states from the activity instance
    sf::priv::ActivityStates& states = retrieveStates(*activity);

    // Attach the input queue
    {
        const std::lock_guard lock(states.mutex);

        AInputQueue_attachLooper(queue, states.looper, 1, states.processEvent, nullptr);
        states.inputQueue = queue;
    }
}

////////////////////////////////////////////////////////////
void onInputQueueDestroyed(ANativeActivity* activity, AInputQueue* queue)
{
    // Retrieve our activity states from the activity instance
    sf::priv::ActivityStates& states = retrieveStates(*activity);

    // Detach the input queue
    {
        const std::lock_guard lock(states.mutex);

        AInputQueue_detachLooper(queue);
        states.inputQueue = nullptr;

        ALooper_release(states.looper);
    }
}

////////////////////////////////////////////////////////////
void onWindowFocusChanged(ANativeActivity* /* activity */, int /* focused */)
{
}

////////////////////////////////////////////////////////////
void onContentRectChanged(ANativeActivity* activity, const ARect* /* rect */)
{
    // Retrieve our activity states from the activity instance
    sf::priv::ActivityStates& states = retrieveStates(*activity);
    const std::lock_guard     lock(states.mutex);

    // Make sure the window still exists before we access the dimensions on it
    if (states.window != nullptr)
    {
        // Send an event to warn people about the window move/resize
        const sf::Event::Resized event{
            sf::Vector2u(sf::Vector2(ANativeWindow_getWidth(states.window), ANativeWindow_getHeight(states.window)))};
        states.forwardEvent(event);
    }
}

////////////////////////////////////////////////////////////
void onConfigurationChanged(ANativeActivity* /* activity */)
{
}

////////////////////////////////////////////////////////////
void* onSaveInstanceState(ANativeActivity* /* activity */, std::size_t* outLen)
{
    *outLen = 0;
    return nullptr;
}

////////////////////////////////////////////////////////////
void onLowMemory(ANativeActivity* /* activity */)
{
}
} // namespace


namespace sf::priv
{
////////////////////////////////////////////////////////////
void* main(ActivityStates* states)
{
    // Initialize the thread before giving the hand
    initializeMain(*states);

    sleep(seconds(0.5));
    ::main(0, nullptr);

    // Terminate properly the main thread and wait until it's done
    terminateMain(*states);

    {
        const std::lock_guard lock(states->mutex);
        states->terminated = true;
    }

    return nullptr;
}
} // namespace sf::priv


////////////////////////////////////////////////////////////
JNIEXPORT void ANativeActivity_onCreate(ANativeActivity* activity, void* savedState, std::size_t savedStateSize)
{
    // Create an activity states (will keep us in the know, about events we care)
    auto* states = new sf::priv::ActivityStates();

    for (auto& isButtonPressed : states->isButtonPressed)
        isButtonPressed = false;

    gladLoaderLoadEGL(EGL_DEFAULT_DISPLAY);
    states->display = eglGetDisplay(EGL_DEFAULT_DISPLAY);

    if (savedState != nullptr)
    {
        const auto* begin = static_cast<const std::byte*>(savedState);
        const auto* end   = begin + savedStateSize;
        states->savedState.assign(begin, end);
    }

    states->mainOver = false;

    states->initialized = false;
    states->terminated  = false;

    // Share it across the SFML modules
    sf::priv::resetActivity(states);

    // These functions will update the activity states and therefore, will allow
    // SFML to be kept in the know
    activity->callbacks->onStart   = onStart;
    activity->callbacks->onResume  = onResume;
    activity->callbacks->onPause   = onPause;
    activity->callbacks->onStop    = onStop;
    activity->callbacks->onDestroy = onDestroy;

    activity->callbacks->onNativeWindowCreated      = onNativeWindowCreated;
    activity->callbacks->onNativeWindowDestroyed    = onNativeWindowDestroyed;
    activity->callbacks->onNativeWindowRedrawNeeded = onNativeWindowRedrawNeeded;
    activity->callbacks->onNativeWindowResized      = onNativeWindowResized;

    activity->callbacks->onInputQueueCreated   = onInputQueueCreated;
    activity->callbacks->onInputQueueDestroyed = onInputQueueDestroyed;

    activity->callbacks->onWindowFocusChanged   = onWindowFocusChanged;
    activity->callbacks->onContentRectChanged   = onContentRectChanged;
    activity->callbacks->onConfigurationChanged = onConfigurationChanged;

    activity->callbacks->onSaveInstanceState = onSaveInstanceState;
    activity->callbacks->onLowMemory         = onLowMemory;

    // Share this activity with the callback functions
    states->activity = activity;

    // Keep the screen turned on and bright
    ANativeActivity_setWindowFlags(activity, AWINDOW_FLAG_KEEP_SCREEN_ON, AWINDOW_FLAG_KEEP_SCREEN_ON);

    // Initialize the display
    eglInitialize(states->display, nullptr, nullptr);

    getScreenSizeInPixels(*activity, states->screenSize.x, states->screenSize.y);
    getFullScreenSizeInPixels(*activity, states->fullScreenSize.x, states->fullScreenSize.y);

    // Redirect error messages to logcat
    sf::err().rdbuf(&states->logcat);

    // Launch the main thread
    std::thread(sf::priv::main, states).detach();

    // Wait for the main thread to be initialized
    states->mutex.lock();

    while (!(states->initialized | states->terminated))
    {
        states->mutex.unlock();
        sf::sleep(sf::milliseconds(20));
        states->mutex.lock();
    }

    states->mutex.unlock();

    // Share this state with the callback functions
    activity->instance = states;
}
