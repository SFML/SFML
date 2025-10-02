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

#pragma once

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////

#include <android/native_activity.h>

#include <algorithm>
#include <optional>
#include <string>

#include <cassert>

////////////////////////////////////////////////////////////
/// \brief C++ wrapper over Java arrays
///
////////////////////////////////////////////////////////////
template<class T>
class JniArray {
public:
    JniArray(JNIEnv* env, jintArray array) :
    m_env(env),
    m_array(array),
    m_length(env->GetArrayLength(array)),
    m_data(env->GetIntArrayElements(array, nullptr))
    {
    }

    JniArray(JniArray&& other) noexcept
    {
        std::swap(m_env, other.m_env);
        std::swap(m_array, other.m_array);
        std::swap(m_length, other.m_length);
        std::swap(m_data, other.m_data);
    }

    ~JniArray()
    {
        if (m_data)
            m_env->ReleaseIntArrayElements(m_array, m_data, 0);
    }

    T operator[](ssize_t idx)
    {
        assert(0 <= idx && idx <= m_length);
        return m_data[idx];
    }

    T operator[](ssize_t idx) const
    {
        assert(0 <= idx && idx <= m_length);
        return m_data[idx];
    }

    [[nodiscard]] ssize_t getLength() const noexcept
    {
        return m_length;
    }

private:
    JNIEnv*   m_env = nullptr;
    jintArray m_array = nullptr;
    ssize_t   m_length = 0;
    T*        m_data = nullptr;
};

class JniInputDeviceClass;

////////////////////////////////////////////////////////////
/// \brief C++ wrapper over JNI InputDevice instances
///
/// This class abstracts access to native Java object
////////////////////////////////////////////////////////////
class JniInputDevice
{
private:
    JniInputDevice(JNIEnv* env,
                   jobject inputDevice,
                   jmethodID getNameMethod,
                   jmethodID getVendorIdMethod,
                   jmethodID getProductIdMethod,
                   jmethodID supportsSourceMethod) :
    m_env(env),
    m_inputDevice(inputDevice),
    m_getNameMethod(getNameMethod),
    m_getVendorIdMethod(getVendorIdMethod),
    m_getProductIdMethod(getProductIdMethod),
    m_supportsSourceMethod(supportsSourceMethod)
    {
    }

    friend class JniInputDeviceClass;

public:
    [[nodiscard]] unsigned getVendorId() const
    {
        return static_cast<unsigned>(m_env->CallIntMethod(m_inputDevice, m_getVendorIdMethod));
    }

    [[nodiscard]] unsigned getProductId() const
    {
        return static_cast<unsigned>(m_env->CallIntMethod(m_inputDevice, m_getProductIdMethod));
    }

    [[nodiscard]] std::string getName() const
    {
        return javaStringToStd(static_cast<jstring>(m_env->CallObjectMethod(m_inputDevice, m_getNameMethod)));
    }

    [[nodiscard]] bool supportsSource(size_t sourceFlags) const
    {
        return m_env->CallBooleanMethod(m_inputDevice, m_supportsSourceMethod, jint(sourceFlags));
    }

private:
    std::string javaStringToStd(jstring str) const;

    JNIEnv*   m_env;
    jobject   m_inputDevice;
    jmethodID m_getNameMethod;
    jmethodID m_getVendorIdMethod;
    jmethodID m_getProductIdMethod;
    jmethodID m_supportsSourceMethod;
};

////////////////////////////////////////////////////////////
/// \brief C++ wrapper over JNI InputDevice class
///
/// This class abstracts access to native Java class
////////////////////////////////////////////////////////////
class JniInputDeviceClass
{
private:
    JniInputDeviceClass(JNIEnv* env,
                        jclass inputDeviceClass,
                        jmethodID getDeviceIdsMethod,
                        jmethodID getDeviceMethod) :
    m_env(env),
    m_inputDeviceClass(inputDeviceClass),
    m_getDeviceIdsMethod(getDeviceIdsMethod),
    m_getDeviceMethod(getDeviceMethod)
    {
    }

public:
    static std::optional<JniInputDeviceClass> findClass(JNIEnv* env);

    std::optional<JniArray<jint>> getDeviceIds();

    std::optional<JniInputDevice> getDevice(jint idx);

private:
    JNIEnv*   m_env;
    jclass    m_inputDeviceClass;
    jmethodID m_getDeviceIdsMethod;
    jmethodID m_getDeviceMethod;
};

////////////////////////////////////////////////////////////
/// \brief RAII wrapper for attaching JavaVM thread
///
/// Thread is detached automatically when this class is destroyed
////////////////////////////////////////////////////////////
struct Jni
{
private:
    explicit Jni(JavaVM* vm) :
    m_vm(vm)
    {
    }

public:
    Jni(Jni&& other) noexcept { std::swap(m_vm, other.m_vm); };
    Jni(const Jni&&) = delete;
    ~Jni() { if (m_vm) m_vm->DetachCurrentThread(); }

    ////////////////////////////////////////////////////////////
    /// \brief Binds \p env to thread called NativeThread
    ///
    /// The thread will be detached once the instance of the optional
    /// is detached.
    ///
    /// \param vm Pointer to JavaVM instance (from ActivityStates)
    /// \param env Pointer to pointer to JNIEnv that will be used to
    /// invoke JNI.
    ///
    /// \return Returns instance of this object on success or std::nullopt on fail
    ///
    ////////////////////////////////////////////////////////////
    static std::optional<Jni> attachCurrentThread(JavaVM* vm, JNIEnv** env);

private:
    JavaVM* m_vm = nullptr;
};
