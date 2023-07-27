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

#include <SFML/Config.hpp>

#include <android/log.h>
#include <android/native_activity.h>
#include <jni.h>

#include <dlfcn.h>
#include <string>

#include <cerrno>
#include <cstdlib>
#include <cstring>

#define LOGE(...) ((void)__android_log_print(ANDROID_LOG_INFO, "sfml-activity", __VA_ARGS__))

namespace
{
using activityOnCreatePointer = void (*)(ANativeActivity*, void*, std::size_t);
}

const char* getLibraryName(JNIEnv* lJNIEnv, jobject& objectActivityInfo)
{
    // This function reads the value of meta-data "sfml.app.lib_name"
    // found in the Android Manifest file and returns it. It performs the
    // following Java code using the JNI interface:
    //
    // ai.metaData.getString("sfml.app.lib_name");
    static char name[256];

    // Get metaData instance from the ActivityInfo object
    jclass   classActivityInfo = lJNIEnv->FindClass("android/content/pm/ActivityInfo");
    jfieldID fieldMetaData     = lJNIEnv->GetFieldID(classActivityInfo, "metaData", "Landroid/os/Bundle;");
    jobject  objectMetaData    = lJNIEnv->GetObjectField(objectActivityInfo, fieldMetaData);

    // Create a java string object containing "sfml.app.lib_name"
    jobject objectName = lJNIEnv->NewStringUTF("sfml.app.lib_name");

    // Get the value of meta-data named "sfml.app.lib_name"
    jclass    classBundle     = lJNIEnv->FindClass("android/os/Bundle");
    jmethodID methodGetString = lJNIEnv->GetMethodID(classBundle, "getString", "(Ljava/lang/String;)Ljava/lang/String;");
    auto* valueString = static_cast<jstring>(lJNIEnv->CallObjectMethod(objectMetaData, methodGetString, objectName));

    // No meta-data "sfml.app.lib_name" was found so we abort and inform the user
    if (valueString == nullptr)
    {
        LOGE("No meta-data 'sfml.app.lib_name' found in AndroidManifest.xml file");
        std::exit(1);
    }

    // Convert the application name to a C++ string and return it
    const auto  applicationNameLength = static_cast<std::size_t>(lJNIEnv->GetStringUTFLength(valueString));
    const char* applicationName       = lJNIEnv->GetStringUTFChars(valueString, nullptr);

    if (applicationNameLength >= 256)
    {
        LOGE("The value of 'sfml.app.lib_name' must not be longer than 255 characters.");
        std::exit(1);
    }

    strncpy(name, applicationName, static_cast<std::size_t>(applicationNameLength));
    name[applicationNameLength] = '\0';
    lJNIEnv->ReleaseStringUTFChars(valueString, applicationName);

    return name;
}

void* loadLibrary(const char* libraryName, JNIEnv* lJNIEnv, jobject& objectActivityInfo)
{
    // Find out the absolute path of the library
    jclass   classActivityInfo     = lJNIEnv->FindClass("android/content/pm/ActivityInfo");
    jfieldID fieldApplicationInfo  = lJNIEnv->GetFieldID(classActivityInfo,
                                                        "applicationInfo",
                                                        "Landroid/content/pm/ApplicationInfo;");
    jobject  objectApplicationInfo = lJNIEnv->GetObjectField(objectActivityInfo, fieldApplicationInfo);

    jclass   classApplicationInfo  = lJNIEnv->FindClass("android/content/pm/ApplicationInfo");
    jfieldID fieldNativeLibraryDir = lJNIEnv->GetFieldID(classApplicationInfo, "nativeLibraryDir", "Ljava/lang/String;");

    jobject objectDirPath = lJNIEnv->GetObjectField(objectApplicationInfo, fieldNativeLibraryDir);

    jclass    classSystem                = lJNIEnv->FindClass("java/lang/System");
    jmethodID staticMethodMapLibraryName = lJNIEnv->GetStaticMethodID(classSystem,
                                                                      "mapLibraryName",
                                                                      "(Ljava/lang/String;)Ljava/lang/String;");

    jstring libNameObject = lJNIEnv->NewStringUTF(libraryName);
    jobject objectName    = lJNIEnv->CallStaticObjectMethod(classSystem, staticMethodMapLibraryName, libNameObject);

    jclass    classFile       = lJNIEnv->FindClass("java/io/File");
    jmethodID fileConstructor = lJNIEnv->GetMethodID(classFile, "<init>", "(Ljava/lang/String;Ljava/lang/String;)V");
    jobject   objectFile      = lJNIEnv->NewObject(classFile, fileConstructor, objectDirPath, objectName);

    // Get the library absolute path and convert it
    jmethodID   methodGetPath   = lJNIEnv->GetMethodID(classFile, "getPath", "()Ljava/lang/String;");
    auto*       javaLibraryPath = static_cast<jstring>(lJNIEnv->CallObjectMethod(objectFile, methodGetPath));
    const char* libraryPath     = lJNIEnv->GetStringUTFChars(javaLibraryPath, nullptr);

    // Manually load the library
    void* handle = dlopen(libraryPath, RTLD_NOW | RTLD_GLOBAL);
    if (!handle)
    {
        LOGE("dlopen(\"%s\"): %s", libraryPath, dlerror());
        std::exit(1);
    }

    // Release the Java string
    lJNIEnv->ReleaseStringUTFChars(javaLibraryPath, libraryPath);

    return handle;
}

JNIEXPORT void ANativeActivity_onCreate(ANativeActivity* activity, void* savedState, std::size_t savedStateSize)
{
    // Before we can load a library, we need to find out its location. As
    // we're powerless here in C/C++, we need the JNI interface to communicate
    // with the attached Java virtual machine and perform some Java calls in
    // order to retrieve the absolute path of our libraries.
    //
    // Here's the snippet of Java code it performs:
    // --------------------------------------------
    // ai = getPackageManager().getActivityInfo(getIntent().getComponent(), PackageManager.GET_META_DATA);
    // File libraryFile = new File(ai.applicationInfo.nativeLibraryDir, System.mapLibraryName(libname));
    // String path = libraryFile.getPath();
    //
    // With libname being the library name such as "jpeg".

    // Retrieve JNI environment and JVM instance
    JNIEnv* lJNIEnv = activity->env;

    // Retrieve the NativeActivity
    jobject objectNativeActivity = activity->clazz;
    jclass  classNativeActivity  = lJNIEnv->GetObjectClass(objectNativeActivity);

    // Retrieve the ActivityInfo
    jmethodID methodGetPackageManager = lJNIEnv->GetMethodID(classNativeActivity,
                                                             "getPackageManager",
                                                             "()Landroid/content/pm/PackageManager;");
    jobject   objectPackageManager    = lJNIEnv->CallObjectMethod(objectNativeActivity, methodGetPackageManager);

    jmethodID methodGetIndent = lJNIEnv->GetMethodID(classNativeActivity, "getIntent", "()Landroid/content/Intent;");
    jobject   objectIntent    = lJNIEnv->CallObjectMethod(objectNativeActivity, methodGetIndent);

    jclass    classIntent        = lJNIEnv->FindClass("android/content/Intent");
    jmethodID methodGetComponent = lJNIEnv->GetMethodID(classIntent,
                                                        "getComponent",
                                                        "()Landroid/content/ComponentName;");

    jobject objectComponentName = lJNIEnv->CallObjectMethod(objectIntent, methodGetComponent);

    jclass classPackageManager = lJNIEnv->FindClass("android/content/pm/PackageManager");

    jfieldID fieldGetMetaData = lJNIEnv->GetStaticFieldID(classPackageManager, "GET_META_DATA", "I");
    jint     getMetaData      = lJNIEnv->GetStaticIntField(classPackageManager, fieldGetMetaData);

    jmethodID methodGetActivityInfo = lJNIEnv->GetMethodID(classPackageManager,
                                                           "getActivityInfo",
                                                           "(Landroid/content/ComponentName;I)Landroid/content/pm/"
                                                           "ActivityInfo;");
    jobject   objectActivityInfo = lJNIEnv->CallObjectMethod(objectPackageManager, methodGetActivityInfo, objectComponentName, getMetaData);

    // Load our libraries in reverse order
#if defined(STL_LIBRARY)
#define SFML_QS(s) SFML_S(s)
#define SFML_S(s)  #s
    loadLibrary(SFML_QS(STL_LIBRARY), lJNIEnv, objectActivityInfo);
#undef SFML_S
#undef SFML_QS
#endif
    loadLibrary("openal", lJNIEnv, objectActivityInfo);

#if !defined(SFML_DEBUG)
    loadLibrary("sfml-system", lJNIEnv, objectActivityInfo);
    loadLibrary("sfml-window", lJNIEnv, objectActivityInfo);
    loadLibrary("sfml-graphics", lJNIEnv, objectActivityInfo);
    loadLibrary("sfml-audio", lJNIEnv, objectActivityInfo);
    loadLibrary("sfml-network", lJNIEnv, objectActivityInfo);
#else
    loadLibrary("sfml-system-d", lJNIEnv, objectActivityInfo);
    loadLibrary("sfml-window-d", lJNIEnv, objectActivityInfo);
    loadLibrary("sfml-graphics-d", lJNIEnv, objectActivityInfo);
    loadLibrary("sfml-audio-d", lJNIEnv, objectActivityInfo);
    loadLibrary("sfml-network-d", lJNIEnv, objectActivityInfo);
#endif

    void* handle = loadLibrary(getLibraryName(lJNIEnv, objectActivityInfo), lJNIEnv, objectActivityInfo);

    // Call the original ANativeActivity_onCreate function
    auto nativeActivityOnCreate = reinterpret_cast<activityOnCreatePointer>(dlsym(handle, "ANativeActivity_onCreate"));

    if (!nativeActivityOnCreate)
    {
        LOGE("sfml-activity: Undefined symbol ANativeActivity_onCreate");
        std::exit(1);
    }

    nativeActivityOnCreate(activity, savedState, savedStateSize);
}
