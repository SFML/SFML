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
#include <SFML/Window/InputImpl.hpp>

#include <SFML/System/Android/Activity.hpp>
#include <SFML/System/Err.hpp>

#include <jni.h>

#include <mutex>
#include <ostream>


namespace sf::priv::InputImpl
{
////////////////////////////////////////////////////////////
bool isKeyPressed(Keyboard::Key /* key */)
{
    // Not applicable
    return false;
}


////////////////////////////////////////////////////////////
bool isKeyPressed(Keyboard::Scancode /* codes */)
{
    // Not applicable
    return false;
}


////////////////////////////////////////////////////////////
Keyboard::Key localize(Keyboard::Scancode /* code */)
{
    // Not applicable
    return Keyboard::Key::Unknown;
}


////////////////////////////////////////////////////////////
Keyboard::Scancode delocalize(Keyboard::Key /* key */)
{
    // Not applicable
    return Keyboard::Scan::Unknown;
}


////////////////////////////////////////////////////////////
String getDescription(Keyboard::Scancode code)
{
    switch (code)
    {
            // clang-format off
        case sf::Keyboard::Scan::A:                 return "A";
        case sf::Keyboard::Scan::B:                 return "B";
        case sf::Keyboard::Scan::C:                 return "C";
        case sf::Keyboard::Scan::D:                 return "D";
        case sf::Keyboard::Scan::E:                 return "E";
        case sf::Keyboard::Scan::F:                 return "F";
        case sf::Keyboard::Scan::G:                 return "G";
        case sf::Keyboard::Scan::H:                 return "H";
        case sf::Keyboard::Scan::I:                 return "I";
        case sf::Keyboard::Scan::J:                 return "J";
        case sf::Keyboard::Scan::K:                 return "K";
        case sf::Keyboard::Scan::L:                 return "L";
        case sf::Keyboard::Scan::M:                 return "M";
        case sf::Keyboard::Scan::N:                 return "N";
        case sf::Keyboard::Scan::O:                 return "O";
        case sf::Keyboard::Scan::P:                 return "P";
        case sf::Keyboard::Scan::Q:                 return "Q";
        case sf::Keyboard::Scan::R:                 return "R";
        case sf::Keyboard::Scan::S:                 return "S";
        case sf::Keyboard::Scan::T:                 return "T";
        case sf::Keyboard::Scan::U:                 return "U";
        case sf::Keyboard::Scan::V:                 return "V";
        case sf::Keyboard::Scan::W:                 return "W";
        case sf::Keyboard::Scan::X:                 return "X";
        case sf::Keyboard::Scan::Y:                 return "Y";
        case sf::Keyboard::Scan::Z:                 return "Z";
        case sf::Keyboard::Scan::Num1:              return "1";
        case sf::Keyboard::Scan::Num2:              return "2";
        case sf::Keyboard::Scan::Num3:              return "3";
        case sf::Keyboard::Scan::Num4:              return "4";
        case sf::Keyboard::Scan::Num5:              return "5";
        case sf::Keyboard::Scan::Num6:              return "6";
        case sf::Keyboard::Scan::Num7:              return "7";
        case sf::Keyboard::Scan::Num8:              return "8";
        case sf::Keyboard::Scan::Num9:              return "9";
        case sf::Keyboard::Scan::Num0:              return "0";
        case sf::Keyboard::Scan::Enter:             return "Enter";
        case sf::Keyboard::Scan::Escape:            return "Escape";
        case sf::Keyboard::Scan::Backspace:         return "Backspace";
        case sf::Keyboard::Scan::Tab:               return "Tab";
        case sf::Keyboard::Scan::Space:             return "Space";
        case sf::Keyboard::Scan::Hyphen:            return "-";
        case sf::Keyboard::Scan::Equal:             return "=";
        case sf::Keyboard::Scan::LBracket:          return "[";
        case sf::Keyboard::Scan::RBracket:          return "]";
        case sf::Keyboard::Scan::Backslash:         return "\\";
        case sf::Keyboard::Scan::Semicolon:         return ";";
        case sf::Keyboard::Scan::Apostrophe:        return "'";
        case sf::Keyboard::Scan::Grave:             return "`";
        case sf::Keyboard::Scan::Comma:             return ",";
        case sf::Keyboard::Scan::Period:            return ".";
        case sf::Keyboard::Scan::Slash:             return "/";
        case sf::Keyboard::Scan::F1:                return "F1";
        case sf::Keyboard::Scan::F2:                return "F2";
        case sf::Keyboard::Scan::F3:                return "F3";
        case sf::Keyboard::Scan::F4:                return "F4";
        case sf::Keyboard::Scan::F5:                return "F5";
        case sf::Keyboard::Scan::F6:                return "F6";
        case sf::Keyboard::Scan::F7:                return "F7";
        case sf::Keyboard::Scan::F8:                return "F8";
        case sf::Keyboard::Scan::F9:                return "F9";
        case sf::Keyboard::Scan::F10:               return "F10";
        case sf::Keyboard::Scan::F11:               return "F11";
        case sf::Keyboard::Scan::F12:               return "F12";
        case sf::Keyboard::Scan::F13:               return "F13";
        case sf::Keyboard::Scan::F14:               return "F14";
        case sf::Keyboard::Scan::F15:               return "F15";
        case sf::Keyboard::Scan::F16:               return "F16";
        case sf::Keyboard::Scan::F17:               return "F17";
        case sf::Keyboard::Scan::F18:               return "F18";
        case sf::Keyboard::Scan::F19:               return "F19";
        case sf::Keyboard::Scan::F20:               return "F20";
        case sf::Keyboard::Scan::F21:               return "F21";
        case sf::Keyboard::Scan::F22:               return "F22";
        case sf::Keyboard::Scan::F23:               return "F23";
        case sf::Keyboard::Scan::F24:               return "F24";
        case sf::Keyboard::Scan::CapsLock:          return "Caps Lock";
        case sf::Keyboard::Scan::PrintScreen:       return "Print Screen";
        case sf::Keyboard::Scan::ScrollLock:        return "Scroll Lock";
        case sf::Keyboard::Scan::Pause:             return "Pause";
        case sf::Keyboard::Scan::Insert:            return "Insert";
        case sf::Keyboard::Scan::Home:              return "Home";
        case sf::Keyboard::Scan::PageUp:            return "Page Up";
        case sf::Keyboard::Scan::Delete:            return "Delete";
        case sf::Keyboard::Scan::End:               return "End";
        case sf::Keyboard::Scan::PageDown:          return "Page Down";
        case sf::Keyboard::Scan::Right:             return "Right Arrow";
        case sf::Keyboard::Scan::Left:              return "Left Arrow";
        case sf::Keyboard::Scan::Down:              return "Down Arrow";
        case sf::Keyboard::Scan::Up:                return "Up Arrow";
        case sf::Keyboard::Scan::NumLock:           return "Num Lock";
        case sf::Keyboard::Scan::NumpadDivide:      return "Numpad /";
        case sf::Keyboard::Scan::NumpadMultiply:    return "Numpad *";
        case sf::Keyboard::Scan::NumpadMinus:       return "Numpad -";
        case sf::Keyboard::Scan::NumpadPlus:        return "Numpad +";
        case sf::Keyboard::Scan::NumpadEqual:       return "Numpad =";
        case sf::Keyboard::Scan::NumpadEnter:       return "Numpad Enter";
        case sf::Keyboard::Scan::NumpadDecimal:     return "Numpad .";
        case sf::Keyboard::Scan::Numpad1:           return "Numpad 1";
        case sf::Keyboard::Scan::Numpad2:           return "Numpad 2";
        case sf::Keyboard::Scan::Numpad3:           return "Numpad 3";
        case sf::Keyboard::Scan::Numpad4:           return "Numpad 4";
        case sf::Keyboard::Scan::Numpad5:           return "Numpad 5";
        case sf::Keyboard::Scan::Numpad6:           return "Numpad 6";
        case sf::Keyboard::Scan::Numpad7:           return "Numpad 7";
        case sf::Keyboard::Scan::Numpad8:           return "Numpad 8";
        case sf::Keyboard::Scan::Numpad9:           return "Numpad 9";
        case sf::Keyboard::Scan::Numpad0:           return "Numpad 0";
        case sf::Keyboard::Scan::NonUsBackslash:    return "Non-US Backslash";
        case sf::Keyboard::Scan::Application:       return "Application";
        case sf::Keyboard::Scan::Execute:           return "Execute";
        case sf::Keyboard::Scan::ModeChange:        return "Mode Change";
        case sf::Keyboard::Scan::Help:              return "Help";
        case sf::Keyboard::Scan::Menu:              return "Menu";
        case sf::Keyboard::Scan::Select:            return "Select";
        case sf::Keyboard::Scan::Redo:              return "Redo";
        case sf::Keyboard::Scan::Undo:              return "Undo";
        case sf::Keyboard::Scan::Cut:               return "Cut";
        case sf::Keyboard::Scan::Copy:              return "Copy";
        case sf::Keyboard::Scan::Paste:             return "Paste";
        case sf::Keyboard::Scan::VolumeMute:        return "Volume Mute";
        case sf::Keyboard::Scan::VolumeUp:          return "Volume Up";
        case sf::Keyboard::Scan::VolumeDown:        return "Volume Down";
        case sf::Keyboard::Scan::MediaPlayPause:    return "Media Play/Pause";
        case sf::Keyboard::Scan::MediaStop:         return "Media Stop";
        case sf::Keyboard::Scan::MediaNextTrack:    return "Media Next Track";
        case sf::Keyboard::Scan::MediaPreviousTrack:return "Media Previous Track";
        case sf::Keyboard::Scan::LControl:          return "Left Control";
        case sf::Keyboard::Scan::LShift:            return "Left Shift";
        case sf::Keyboard::Scan::LAlt:              return "Left Alt";
        case sf::Keyboard::Scan::LSystem:           return "Left System";
        case sf::Keyboard::Scan::RControl:          return "Right Control";
        case sf::Keyboard::Scan::RShift:            return "Right Shift";
        case sf::Keyboard::Scan::RAlt:              return "Right Alt";
        case sf::Keyboard::Scan::RSystem:           return "Right System";
        case sf::Keyboard::Scan::Back:              return "Back";
        case sf::Keyboard::Scan::Forward:           return "Forward";
        case sf::Keyboard::Scan::Refresh:           return "Refresh";
        case sf::Keyboard::Scan::Stop:              return "Stop";
        case sf::Keyboard::Scan::Search:            return "Search";
        case sf::Keyboard::Scan::Favorites:         return "Favorites";
        case sf::Keyboard::Scan::HomePage:          return "Home Page";
        case sf::Keyboard::Scan::LaunchApplication1:return "Launch Application 1";
        case sf::Keyboard::Scan::LaunchApplication2:return "Launch Application 2";
        case sf::Keyboard::Scan::LaunchMail:        return "Launch Mail";
        case sf::Keyboard::Scan::LaunchMediaSelect: return "Launch Media Select";
        default: return "Unknown key";
            // clang-format on
    }
}


////////////////////////////////////////////////////////////
void setVirtualKeyboardVisible(bool visible)
{
    // TODO: Check if the window is active

    ActivityStates&       states = getActivity();
    const std::lock_guard lock(states.mutex);

    // Initializes JNI
    const jint lFlags = 0;

    JavaVM* lJavaVM = states.activity->vm;
    JNIEnv* lJNIEnv = states.activity->env;

    JavaVMAttachArgs lJavaVMAttachArgs;
    lJavaVMAttachArgs.version = JNI_VERSION_1_6;
    lJavaVMAttachArgs.name    = "NativeThread";
    lJavaVMAttachArgs.group   = nullptr;

    const jint lResult = lJavaVM->AttachCurrentThread(&lJNIEnv, &lJavaVMAttachArgs);

    if (lResult == JNI_ERR)
        err() << "Failed to initialize JNI, couldn't switch the keyboard visibility" << std::endl;

    // Retrieves NativeActivity
    jobject lNativeActivity     = states.activity->clazz;
    jclass  classNativeActivity = lJNIEnv->GetObjectClass(lNativeActivity);

    // Retrieves Context.INPUT_METHOD_SERVICE
    jclass   classContext            = lJNIEnv->FindClass("android/content/Context");
    jfieldID fieldInputMethodService = lJNIEnv->GetStaticFieldID(classContext,
                                                                 "INPUT_METHOD_SERVICE",
                                                                 "Ljava/lang/String;");
    jobject  inputMethodService      = lJNIEnv->GetStaticObjectField(classContext, fieldInputMethodService);
    lJNIEnv->DeleteLocalRef(classContext);

    // Runs getSystemService(Context.INPUT_METHOD_SERVICE)
    jclass    classInputMethodManager = lJNIEnv->FindClass("android/view/inputmethod/InputMethodManager");
    jmethodID methodGetSystemService  = lJNIEnv->GetMethodID(classNativeActivity,
                                                            "getSystemService",
                                                            "(Ljava/lang/String;)Ljava/lang/Object;");
    jobject lInputMethodManager = lJNIEnv->CallObjectMethod(lNativeActivity, methodGetSystemService, inputMethodService);
    lJNIEnv->DeleteLocalRef(inputMethodService);

    // Runs getWindow().getDecorView()
    jmethodID methodGetWindow    = lJNIEnv->GetMethodID(classNativeActivity, "getWindow", "()Landroid/view/Window;");
    jobject   lWindow            = lJNIEnv->CallObjectMethod(lNativeActivity, methodGetWindow);
    jclass    classWindow        = lJNIEnv->FindClass("android/view/Window");
    jmethodID methodGetDecorView = lJNIEnv->GetMethodID(classWindow, "getDecorView", "()Landroid/view/View;");
    jobject   lDecorView         = lJNIEnv->CallObjectMethod(lWindow, methodGetDecorView);
    lJNIEnv->DeleteLocalRef(lWindow);
    lJNIEnv->DeleteLocalRef(classWindow);

    if (visible)
    {
        // Runs lInputMethodManager.showSoftInput(...)
        jmethodID methodShowSoftInput = lJNIEnv->GetMethodID(classInputMethodManager,
                                                             "showSoftInput",
                                                             "(Landroid/view/View;I)Z");
        lJNIEnv->CallBooleanMethod(lInputMethodManager, methodShowSoftInput, lDecorView, lFlags);
    }
    else
    {
        // Runs lWindow.getViewToken()
        jclass    classView            = lJNIEnv->FindClass("android/view/View");
        jmethodID methodGetWindowToken = lJNIEnv->GetMethodID(classView, "getWindowToken", "()Landroid/os/IBinder;");
        jobject   lBinder              = lJNIEnv->CallObjectMethod(lDecorView, methodGetWindowToken);
        lJNIEnv->DeleteLocalRef(classView);

        // lInputMethodManager.hideSoftInput(...)
        jmethodID methodHideSoftInput = lJNIEnv->GetMethodID(classInputMethodManager,
                                                             "hideSoftInputFromWindow",
                                                             "(Landroid/os/IBinder;I)Z");
        lJNIEnv->CallBooleanMethod(lInputMethodManager, methodHideSoftInput, lBinder, lFlags);
        lJNIEnv->DeleteLocalRef(lBinder);
    }
    lJNIEnv->DeleteLocalRef(classNativeActivity);
    lJNIEnv->DeleteLocalRef(classInputMethodManager);
    lJNIEnv->DeleteLocalRef(lDecorView);

    // Finished with the JVM
    lJavaVM->DetachCurrentThread();
}


////////////////////////////////////////////////////////////
bool isMouseButtonPressed(Mouse::Button button)
{
    while (ALooper_pollOnce(0, nullptr, nullptr, nullptr) >= 0)
        ;

    ActivityStates&       states = getActivity();
    const std::lock_guard lock(states.mutex);

    return states.isButtonPressed[button];
}


////////////////////////////////////////////////////////////
Vector2i getMousePosition()
{
    while (ALooper_pollOnce(0, nullptr, nullptr, nullptr) >= 0)
        ;

    ActivityStates&       states = getActivity();
    const std::lock_guard lock(states.mutex);

    return states.mousePosition;
}


////////////////////////////////////////////////////////////
Vector2i getMousePosition(const WindowBase& /* relativeTo */)
{
    return getMousePosition();
}


////////////////////////////////////////////////////////////
void setMousePosition(Vector2i /* position */)
{
    // Injecting events is impossible on Android
}


////////////////////////////////////////////////////////////
void setMousePosition(Vector2i position, const WindowBase& /* relativeTo */)
{
    setMousePosition(position);
}


////////////////////////////////////////////////////////////
bool isTouchDown(unsigned int finger)
{
    while (ALooper_pollOnce(0, nullptr, nullptr, nullptr) >= 0)
        ;

    ActivityStates&       states = getActivity();
    const std::lock_guard lock(states.mutex);

    return states.touchEvents.find(static_cast<int>(finger)) != states.touchEvents.end();
}


////////////////////////////////////////////////////////////
Vector2i getTouchPosition(unsigned int finger)
{
    while (ALooper_pollOnce(0, nullptr, nullptr, nullptr) >= 0)
        ;

    ActivityStates&       states = getActivity();
    const std::lock_guard lock(states.mutex);

    return states.touchEvents.find(static_cast<int>(finger))->second;
}


////////////////////////////////////////////////////////////
Vector2i getTouchPosition(unsigned int finger, const WindowBase& /* relativeTo */)
{
    return getTouchPosition(finger);
}

} // namespace sf::priv::InputImpl
