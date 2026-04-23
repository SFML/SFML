////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007-2026 Laurent Gomila (laurent@sfml-dev.org)
//
// This software is provided 'as-is', without any express or implied warranty.
// In no event will the authors be held liable for any damages arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter this and redistribute it freely,
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
// 3. This notice may not be removed or altered from this source distribution.
//
////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Window/MonitorImpl.hpp>
#include <SFML/Window/VideoMode.hpp>

#include <SFML/System/Android/Activity.hpp>
#include <SFML/System/Err.hpp>

#include <SFML/Window/Android/JniHelper.hpp>

#include <ostream>
#include <string>


namespace sf::priv
{
namespace
{
// Fallback monitor used when the activity/JNI isn't available yet
MonitorImpl getDefaultMonitor()
{
    MonitorImpl monitor;
    monitor.primary          = true;
    monitor.name             = "Android Display";
    monitor.identifier       = "android_0";
    monitor.position         = {0, 0};
    monitor.resolution       = {1920, 1080};
    monitor.refreshRate      = 60;
    monitor.scaledResolution = {1920, 1080};
    monitor.workAreaPosition = {0, 0};
    monitor.workAreaSize     = {1920, 1080};
    return monitor;
}

// Convert a jstring to a std::string, treating it as UTF-8
std::string javaStringToStd(JNIEnv& env, jstring str)
{
    if (!str)
        return {};

    const char*       utfChars = env.GetStringUTFChars(str, nullptr);
    const std::string result(utfChars);
    env.ReleaseStringUTFChars(str, utfChars);
    return result;
}

// Query android.os.Build.VERSION.SDK_INT
int getSdkInt(JNIEnv& env)
{
    const jclass classVersion = env.FindClass("android/os/Build$VERSION");
    if (!classVersion)
        return 0;

    const jfieldID fieldSdkInt = env.GetStaticFieldID(classVersion, "SDK_INT", "I");
    if (!fieldSdkInt)
        return 0;

    return env.GetStaticIntField(classVersion, fieldSdkInt);
}

// Fill in the resolution, density and refresh rate of a MonitorImpl from a Display object
void fillDisplayMetrics(JNIEnv& env, jobject display, int sdkInt, MonitorImpl& impl)
{
    const jclass classDisplay = env.FindClass("android/view/Display");
    if (!classDisplay)
        return;

    // Query the display name
    if (const jmethodID methodGetName = env.GetMethodID(classDisplay, "getName", "()Ljava/lang/String;"))
    {
        if (const auto name = static_cast<jstring>(env.CallObjectMethod(display, methodGetName)))
        {
            const std::string nameStd = javaStringToStd(env, name);
            env.DeleteLocalRef(name);

            if (!nameStd.empty())
                impl.name = String::fromUtf8(nameStd.begin(), nameStd.end());
        }
    }

    // Query the refresh rate
    if (const jmethodID methodGetRefreshRate = env.GetMethodID(classDisplay, "getRefreshRate", "()F"))
    {
        const jfloat refreshRate = env.CallFloatMethod(display, methodGetRefreshRate);
        impl.refreshRate         = (refreshRate > 0.f) ? static_cast<unsigned int>(refreshRate + 0.5f) : 60;
    }

    // Query the resolution and density through a DisplayMetrics object:
    //
    // DisplayMetrics dm = new DisplayMetrics();
    // display.getRealMetrics(dm); // or display.getMetrics(dm) below API level 17
    const jclass classDisplayMetrics = env.FindClass("android/util/DisplayMetrics");
    if (!classDisplayMetrics)
        return;

    const jmethodID methodInitDisplayMetrics = env.GetMethodID(classDisplayMetrics, "<init>", "()V");
    if (!methodInitDisplayMetrics)
        return;

    const jobject displayMetrics = env.NewObject(classDisplayMetrics, methodInitDisplayMetrics);
    if (!displayMetrics)
        return;

    const jmethodID methodGetMetrics = (sdkInt >= 17)
                                            ? env.GetMethodID(classDisplay, "getRealMetrics", "(Landroid/util/DisplayMetrics;)V")
                                            : env.GetMethodID(classDisplay, "getMetrics", "(Landroid/util/DisplayMetrics;)V");

    if (methodGetMetrics)
    {
        env.CallVoidMethod(display, methodGetMetrics, displayMetrics);

        const jfieldID fieldWidthPixels  = env.GetFieldID(classDisplayMetrics, "widthPixels", "I");
        const jfieldID fieldHeightPixels = env.GetFieldID(classDisplayMetrics, "heightPixels", "I");
        const jfieldID fieldDensity      = env.GetFieldID(classDisplayMetrics, "density", "F");

        if (fieldWidthPixels && fieldHeightPixels)
        {
            impl.resolution.x = static_cast<unsigned int>(env.GetIntField(displayMetrics, fieldWidthPixels));
            impl.resolution.y = static_cast<unsigned int>(env.GetIntField(displayMetrics, fieldHeightPixels));
        }

        const jfloat density = fieldDensity ? env.GetFloatField(displayMetrics, fieldDensity) : 1.f;

        impl.scaledResolution = (density > 0.f)
                                    ? Vector2u(static_cast<unsigned int>(static_cast<float>(impl.resolution.x) / density),
                                               static_cast<unsigned int>(static_cast<float>(impl.resolution.y) / density))
                                    : impl.resolution;
    }

    env.DeleteLocalRef(displayMetrics);
}

// Retrieve the default (and only) display through the activity's WindowManager
jobject getDefaultDisplay(JNIEnv& env, jobject activityObject)
{
    const jclass classActivity = env.GetObjectClass(activityObject);

    const jmethodID methodGetWindowManager = env.GetMethodID(classActivity,
                                                              "getWindowManager",
                                                              "()Landroid/view/WindowManager;");
    if (!methodGetWindowManager)
        return nullptr;

    const jobject windowManager = env.CallObjectMethod(activityObject, methodGetWindowManager);
    if (!windowManager)
        return nullptr;

    const jclass classWindowManager = env.FindClass("android/view/WindowManager");
    if (!classWindowManager)
        return nullptr;

    const jmethodID methodGetDefaultDisplay = env.GetMethodID(classWindowManager,
                                                               "getDefaultDisplay",
                                                               "()Landroid/view/Display;");
    if (!methodGetDefaultDisplay)
        return nullptr;

    return env.CallObjectMethod(windowManager, methodGetDefaultDisplay);
}

// Build a MonitorImpl describing Android's single display, queried live from the OS
MonitorImpl getLiveMonitor()
{
    ActivityStates&       states = getActivity();
    const std::lock_guard lock(states.mutex);

    if (!states.activity)
        return getDefaultMonitor();

    auto jni = Jni::attachCurrentThread(*states.activity);
    if (!jni)
    {
        err() << "Failed to initialize JNI" << std::endl;
        return getDefaultMonitor();
    }

    JNIEnv& env = jni->getEnv();

    const jobject display = getDefaultDisplay(env, states.activity->clazz);
    if (!display)
        return getDefaultMonitor();

    MonitorImpl impl = getDefaultMonitor();
    fillDisplayMetrics(env, display, getSdkInt(env), impl);

    impl.workAreaPosition = impl.position;
    impl.workAreaSize     = impl.resolution;

    env.DeleteLocalRef(display);

    return impl;
}
} // namespace


////////////////////////////////////////////////////////////
std::vector<VideoMode> MonitorImpl::getAvailableVideoModes() const
{
    std::vector<VideoMode> modes;

    // Android doesn't expose a discrete list of supported resolutions for its
    // built-in display; report the current resolution as the only available mode
    if (identifier == "android_0")
        modes.emplace_back(resolution);

    return modes;
}


////////////////////////////////////////////////////////////
std::vector<MonitorImpl> MonitorImpl::getAvailableMonitors()
{
    // Android typically has a single display
    return {getLiveMonitor()};
}


////////////////////////////////////////////////////////////
MonitorImpl MonitorImpl::getPrimary()
{
    return getLiveMonitor();
}

} // namespace sf::priv
