////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007-2026 Laurent Gomila (laurent@sfml-dev.org)
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
#include <SFML/Window/Android/ClipboardImpl.hpp>

#include <SFML/System/Android/Activity.hpp>
#include <SFML/System/Err.hpp>
#include <SFML/System/String.hpp>

#include <jni.h>

#include <mutex>
#include <ostream>


namespace sf::priv
{
////////////////////////////////////////////////////////////
String ClipboardImpl::getString()
{
    err() << "Clipboard API not implemented for Android.\n";
    return {};
}


////////////////////////////////////////////////////////////
void ClipboardImpl::setString(const String& text)
{
    ActivityStates&       states = getActivity();
    const std::lock_guard lock(states.mutex);

    JavaVM* javaVM = states.activity->vm;
    JNIEnv* env    = states.activity->env;

    JavaVMAttachArgs attachArgs;
    attachArgs.version = JNI_VERSION_1_6;
    attachArgs.name    = "NativeThread";
    attachArgs.group   = nullptr;

    const jint envStatus = javaVM->GetEnv(reinterpret_cast<void**>(&env), JNI_VERSION_1_6);
    if (envStatus == JNI_EVERSION)
    {
        err() << "Failed to access JNI, couldn't set the clipboard contents" << std::endl;
        return;
    }

    const bool detach = (envStatus == JNI_EDETACHED);
    if (detach && javaVM->AttachCurrentThread(&env, &attachArgs) == JNI_ERR)
    {
        err() << "Failed to initialize JNI, couldn't set the clipboard contents" << std::endl;
        return;
    }

    const auto utf16 = text.toUtf16();
    jstring    label = env->NewStringUTF("SFML Clipboard");
    jstring    data  = env->NewString(reinterpret_cast<const jchar*>(utf16.data()), static_cast<jsize>(utf16.size()));

    jclass    contextClass           = env->FindClass("android/content/Context");
    jfieldID  clipboardServiceField  = env->GetStaticFieldID(contextClass, "CLIPBOARD_SERVICE", "Ljava/lang/String;");
    jobject   clipboardService       = env->GetStaticObjectField(contextClass, clipboardServiceField);
    jobject   nativeActivity         = states.activity->clazz;
    jclass    nativeActivityClass    = env->GetObjectClass(nativeActivity);
    jmethodID getSystemServiceMethod = env->GetMethodID(nativeActivityClass,
                                                        "getSystemService",
                                                        "(Ljava/lang/String;)Ljava/lang/Object;");
    jobject   clipboardManager       = env->CallObjectMethod(nativeActivity, getSystemServiceMethod, clipboardService);

    jclass    clipDataClass         = env->FindClass("android/content/ClipData");
    jmethodID newPlainTextMethod    = env->GetStaticMethodID(clipDataClass,
                                                          "newPlainText",
                                                          "(Ljava/lang/CharSequence;Ljava/lang/CharSequence;)Landroid/"
                                                             "content/ClipData;");
    jobject   clipData              = env->CallStaticObjectMethod(clipDataClass, newPlainTextMethod, label, data);
    jclass    clipboardManagerClass = env->FindClass("android/content/ClipboardManager");
    jmethodID setPrimaryClipMethod  = env->GetMethodID(clipboardManagerClass,
                                                      "setPrimaryClip",
                                                      "(Landroid/content/ClipData;)V");
    env->CallVoidMethod(clipboardManager, setPrimaryClipMethod, clipData);

    env->DeleteLocalRef(clipboardManagerClass);
    env->DeleteLocalRef(clipData);
    env->DeleteLocalRef(clipDataClass);
    env->DeleteLocalRef(clipboardManager);
    env->DeleteLocalRef(nativeActivityClass);
    env->DeleteLocalRef(clipboardService);
    env->DeleteLocalRef(contextClass);
    env->DeleteLocalRef(data);
    env->DeleteLocalRef(label);

    if (detach)
        javaVM->DetachCurrentThread();
}

} // namespace sf::priv
