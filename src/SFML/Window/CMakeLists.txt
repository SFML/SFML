
set(INCROOT ${PROJECT_SOURCE_DIR}/include/SFML/Window)
set(SRCROOT ${PROJECT_SOURCE_DIR}/src/SFML/Window)

# all source files
set(SRC
    ${INCROOT}/Clipboard.hpp
    ${SRCROOT}/Clipboard.cpp
    ${SRCROOT}/ClipboardImpl.hpp
    ${SRCROOT}/Context.cpp
    ${INCROOT}/Context.hpp
    ${SRCROOT}/Cursor.cpp
    ${INCROOT}/Cursor.hpp
    ${SRCROOT}/CursorImpl.hpp
    ${SRCROOT}/EGLCheck.cpp
    ${SRCROOT}/EGLCheck.hpp
    ${SRCROOT}/EglContext.cpp
    ${SRCROOT}/EglContext.hpp
    ${INCROOT}/Export.hpp
    ${SRCROOT}/GlContext.cpp
    ${SRCROOT}/GlContext.hpp
    ${SRCROOT}/GlResource.cpp
    ${INCROOT}/GlResource.hpp
    ${INCROOT}/ContextSettings.hpp
    ${INCROOT}/Event.hpp
    ${SRCROOT}/InputImpl.hpp
    ${INCROOT}/Joystick.hpp
    ${SRCROOT}/Joystick.cpp
    ${SRCROOT}/JoystickImpl.hpp
    ${SRCROOT}/JoystickManager.cpp
    ${SRCROOT}/JoystickManager.hpp
    ${INCROOT}/Keyboard.hpp
    ${SRCROOT}/Keyboard.cpp
    ${INCROOT}/Mouse.hpp
    ${SRCROOT}/Mouse.cpp
    ${INCROOT}/Touch.hpp
    ${SRCROOT}/Touch.cpp
    ${INCROOT}/Sensor.hpp
    ${SRCROOT}/Sensor.cpp
    ${SRCROOT}/SensorImpl.hpp
    ${SRCROOT}/SensorManager.cpp
    ${SRCROOT}/SensorManager.hpp
    ${SRCROOT}/VideoMode.cpp
    ${INCROOT}/VideoMode.hpp
    ${SRCROOT}/VideoModeImpl.hpp
    ${SRCROOT}/Vulkan.cpp
    ${INCROOT}/Vulkan.hpp
    ${SRCROOT}/Window.cpp
    ${INCROOT}/Window.hpp
    ${SRCROOT}/WindowBase.cpp
    ${INCROOT}/WindowBase.hpp
    ${INCROOT}/WindowHandle.hpp
    ${SRCROOT}/WindowImpl.cpp
    ${SRCROOT}/WindowImpl.hpp
    ${INCROOT}/WindowStyle.hpp
)
source_group("" FILES ${SRC})

# add platform specific sources
if(SFML_OS_WINDOWS)
    set(PLATFORM_SRC
        ${SRCROOT}/Win32/CursorImpl.hpp
        ${SRCROOT}/Win32/CursorImpl.cpp
        ${SRCROOT}/Win32/ClipboardImpl.hpp
        ${SRCROOT}/Win32/ClipboardImpl.cpp
        ${SRCROOT}/Win32/WglContext.cpp
        ${SRCROOT}/Win32/WglContext.hpp
        ${SRCROOT}/Win32/InputImpl.cpp
        ${SRCROOT}/Win32/InputImpl.hpp
        ${SRCROOT}/Win32/JoystickImpl.cpp
        ${SRCROOT}/Win32/JoystickImpl.hpp
        ${SRCROOT}/Win32/SensorImpl.hpp
        ${SRCROOT}/Win32/SensorImpl.cpp
        ${SRCROOT}/Win32/VideoModeImpl.cpp
        ${SRCROOT}/Win32/VulkanImplWin32.cpp
        ${SRCROOT}/Win32/VulkanImplWin32.hpp
        ${SRCROOT}/Win32/WindowImplWin32.cpp
        ${SRCROOT}/Win32/WindowImplWin32.hpp
    )
    source_group("windows" FILES ${PLATFORM_SRC})

    # make sure that we use the Unicode version of the Win API functions
    add_definitions(-DUNICODE -D_UNICODE)
elseif(SFML_OS_LINUX OR SFML_OS_FREEBSD OR SFML_OS_OPENBSD OR SFML_OS_NETBSD)
    if(SFML_USE_DRM)
        add_definitions(-DSFML_USE_DRM)
        set(PLATFORM_SRC
            ${PLATFORM_SRC}
            ${SRCROOT}/DRM/CursorImpl.hpp
            ${SRCROOT}/DRM/CursorImpl.cpp
            ${SRCROOT}/DRM/ClipboardImpl.hpp
            ${SRCROOT}/DRM/ClipboardImpl.cpp
            ${SRCROOT}/Unix/SensorImpl.cpp
            ${SRCROOT}/Unix/SensorImpl.hpp
            ${SRCROOT}/DRM/InputImplUDev.cpp
            ${SRCROOT}/DRM/InputImplUDev.hpp
            ${SRCROOT}/DRM/VideoModeImpl.cpp
            ${SRCROOT}/DRM/DRMContext.cpp
            ${SRCROOT}/DRM/DRMContext.hpp
            ${SRCROOT}/DRM/WindowImplDRM.cpp
            ${SRCROOT}/DRM/WindowImplDRM.hpp
        )
    else()
        set(PLATFORM_SRC
            ${PLATFORM_SRC}
            ${SRCROOT}/Unix/CursorImpl.hpp
            ${SRCROOT}/Unix/CursorImpl.cpp
            ${SRCROOT}/Unix/ClipboardImpl.hpp
            ${SRCROOT}/Unix/ClipboardImpl.cpp
            ${SRCROOT}/Unix/InputImpl.cpp
            ${SRCROOT}/Unix/InputImpl.hpp
            ${SRCROOT}/Unix/SensorImpl.cpp
            ${SRCROOT}/Unix/SensorImpl.hpp
            ${SRCROOT}/Unix/Display.cpp
            ${SRCROOT}/Unix/Display.hpp
            ${SRCROOT}/Unix/GlxContext.cpp
            ${SRCROOT}/Unix/GlxContext.hpp
            ${SRCROOT}/Unix/VideoModeImpl.cpp
            ${SRCROOT}/Unix/VulkanImplX11.cpp
            ${SRCROOT}/Unix/VulkanImplX11.hpp
            ${SRCROOT}/Unix/WindowImplX11.cpp
            ${SRCROOT}/Unix/WindowImplX11.hpp
        )
    endif()
    if(SFML_OS_LINUX)
        set(PLATFORM_SRC
            ${PLATFORM_SRC}
            ${SRCROOT}/Unix/JoystickImpl.cpp
            ${SRCROOT}/Unix/JoystickImpl.hpp
        )
    elseif(SFML_OS_FREEBSD)
        set(PLATFORM_SRC
            ${PLATFORM_SRC}
            ${SRCROOT}/FreeBSD/JoystickImpl.cpp
            ${SRCROOT}/FreeBSD/JoystickImpl.hpp
        )
    elseif(SFML_OS_OPENBSD)
        set(PLATFORM_SRC
            ${PLATFORM_SRC}
            ${SRCROOT}/OpenBSD/JoystickImpl.cpp
            ${SRCROOT}/OpenBSD/JoystickImpl.hpp
        )
    elseif(SFML_OS_NETBSD)
        set(PLATFORM_SRC
            ${PLATFORM_SRC}
            ${SRCROOT}/NetBSD/JoystickImpl.cpp
            ${SRCROOT}/NetBSD/JoystickImpl.hpp
        )

    endif()
    source_group("unix" FILES ${PLATFORM_SRC})
elseif(SFML_OS_MACOSX)
    enable_language(OBJC OBJCXX)
    set(PLATFORM_SRC
        ${SRCROOT}/OSX/cpp_objc_conversion.h
        ${SRCROOT}/OSX/cpp_objc_conversion.mm
        ${SRCROOT}/OSX/cg_sf_conversion.hpp
        ${SRCROOT}/OSX/cg_sf_conversion.mm
        ${SRCROOT}/OSX/CursorImpl.hpp
        ${SRCROOT}/OSX/CursorImpl.mm
        ${SRCROOT}/OSX/ClipboardImpl.hpp
        ${SRCROOT}/OSX/ClipboardImpl.mm
        ${SRCROOT}/OSX/InputImpl.mm
        ${SRCROOT}/OSX/InputImpl.hpp
        ${SRCROOT}/OSX/HIDInputManager.hpp
        ${SRCROOT}/OSX/HIDInputManager.mm
        ${SRCROOT}/OSX/HIDJoystickManager.hpp
        ${SRCROOT}/OSX/HIDJoystickManager.cpp
        ${SRCROOT}/OSX/JoystickImpl.cpp
        ${SRCROOT}/OSX/JoystickImpl.hpp
        ${SRCROOT}/OSX/NSImage+raw.h
        ${SRCROOT}/OSX/NSImage+raw.mm
        ${SRCROOT}/OSX/Scaling.h
        ${SRCROOT}/OSX/SensorImpl.cpp
        ${SRCROOT}/OSX/SensorImpl.hpp
        ${SRCROOT}/OSX/SFApplication.h
        ${SRCROOT}/OSX/SFApplication.m
        ${SRCROOT}/OSX/SFApplicationDelegate.h
        ${SRCROOT}/OSX/SFApplicationDelegate.m
        ${SRCROOT}/OSX/SFContext.hpp
        ${SRCROOT}/OSX/SFContext.mm
        ${SRCROOT}/OSX/SFKeyboardModifiersHelper.h
        ${SRCROOT}/OSX/SFKeyboardModifiersHelper.mm
        ${SRCROOT}/OSX/SFOpenGLView.h
        ${SRCROOT}/OSX/SFOpenGLView.mm
        ${SRCROOT}/OSX/SFOpenGLView+keyboard.mm
        ${SRCROOT}/OSX/SFOpenGLView+keyboard_priv.h
        ${SRCROOT}/OSX/SFOpenGLView+mouse.mm
        ${SRCROOT}/OSX/SFOpenGLView+mouse_priv.h
        ${SRCROOT}/OSX/SFSilentResponder.h
        ${SRCROOT}/OSX/SFSilentResponder.m
        ${SRCROOT}/OSX/SFWindow.h
        ${SRCROOT}/OSX/SFWindow.m
        ${SRCROOT}/OSX/SFWindowController.h
        ${SRCROOT}/OSX/SFWindowController.mm
        ${SRCROOT}/OSX/SFViewController.h
        ${SRCROOT}/OSX/SFViewController.mm
        ${SRCROOT}/OSX/VideoModeImpl.cpp
        ${SRCROOT}/OSX/WindowImplCocoa.hpp
        ${SRCROOT}/OSX/WindowImplCocoa.mm
        ${SRCROOT}/OSX/WindowImplDelegateProtocol.h
        ${SRCROOT}/OSX/AutoreleasePoolWrapper.hpp
        ${SRCROOT}/OSX/AutoreleasePoolWrapper.mm
    )
    source_group("mac" FILES ${PLATFORM_SRC})
elseif(SFML_OS_IOS)
    enable_language(OBJCXX)
    set(PLATFORM_SRC
        ${SRCROOT}/iOS/CursorImpl.hpp
        ${SRCROOT}/iOS/CursorImpl.cpp
        ${SRCROOT}/iOS/ClipboardImpl.mm
        ${SRCROOT}/iOS/ClipboardImpl.hpp
        ${SRCROOT}/iOS/EaglContext.mm
        ${SRCROOT}/iOS/EaglContext.hpp
        ${SRCROOT}/iOS/InputImpl.mm
        ${SRCROOT}/iOS/InputImpl.hpp
        ${SRCROOT}/iOS/JoystickImpl.mm
        ${SRCROOT}/iOS/JoystickImpl.hpp
        ${SRCROOT}/iOS/SensorImpl.mm
        ${SRCROOT}/iOS/SensorImpl.hpp
        ${SRCROOT}/iOS/VideoModeImpl.mm
        ${SRCROOT}/iOS/WindowImplUIKit.hpp
        ${SRCROOT}/iOS/WindowImplUIKit.mm
        ${SRCROOT}/iOS/ObjCType.hpp
        ${SRCROOT}/iOS/SFAppDelegate.hpp
        ${SRCROOT}/iOS/SFAppDelegate.mm
        ${SRCROOT}/iOS/SFView.hpp
        ${SRCROOT}/iOS/SFView.mm
        ${SRCROOT}/iOS/SFViewController.hpp
        ${SRCROOT}/iOS/SFViewController.mm
        ${SRCROOT}/iOS/SFMain.hpp
        ${SRCROOT}/iOS/SFMain.mm
    )
    source_group("ios" FILES ${PLATFORM_SRC})
elseif(SFML_OS_ANDROID)
    set(PLATFORM_SRC
        ${SRCROOT}/Android/CursorImpl.hpp
        ${SRCROOT}/Android/CursorImpl.cpp
        ${SRCROOT}/Android/ClipboardImpl.hpp
        ${SRCROOT}/Android/ClipboardImpl.cpp
        ${SRCROOT}/Android/WindowImplAndroid.hpp
        ${SRCROOT}/Android/WindowImplAndroid.cpp
        ${SRCROOT}/Android/VideoModeImpl.cpp
        ${SRCROOT}/Android/InputImpl.hpp
        ${SRCROOT}/Android/InputImpl.cpp
        ${SRCROOT}/Android/JoystickImpl.hpp
        ${SRCROOT}/Android/JoystickImpl.cpp
        ${SRCROOT}/Android/SensorImpl.hpp
        ${SRCROOT}/Android/SensorImpl.cpp
    )
    source_group("android" FILES ${PLATFORM_SRC})
endif()

# define the sfml-window target
sfml_add_library(Window
                 SOURCES ${SRC} ${PLATFORM_SRC})
# DRM libraries
if(SFML_OS_LINUX OR SFML_OS_FREEBSD OR SFML_OS_OPENBSD OR SFML_OS_NETBSD)
    if(SFML_USE_DRM)
        target_sources(sfml-window PRIVATE $<TARGET_OBJECTS:drm-common>)
        sfml_find_package(DRM INCLUDE "DRM_INCLUDE_DIR" LINK "DRM_LIBRARY")
        target_include_directories(sfml-window PRIVATE ${PROJECT_SOURCE_DIR}/extlibs/headers/drm ${DRM_INCLUDE_DIR}/libdrm)
        sfml_find_package(GBM INCLUDE "GBM_INCLUDE_DIR" LINK "GBM_LIBRARY")
        target_link_libraries(sfml-window PRIVATE drm gbm EGL)
        add_library(drm-common OBJECT ${PROJECT_SOURCE_DIR}/extlibs/headers/drm/drm-common.c)
        target_include_directories(drm-common PRIVATE ${PROJECT_SOURCE_DIR}/extlibs/headers/drm ${DRM_INCLUDE_DIR}/libdrm)
    else()
        sfml_find_package(X11 INCLUDE "X11_INCLUDE_DIR" LINK "X11_X11_LIB" "X11_Xrandr_LIB" "X11_Xcursor_LIB")
        target_link_libraries(sfml-window PRIVATE X11)
    endif()
endif()
target_link_libraries(sfml-window PUBLIC SFML::System)

# glad sources
target_include_directories(sfml-window SYSTEM PRIVATE "${PROJECT_SOURCE_DIR}/extlibs/headers/glad/include")

# Vulkan headers
target_include_directories(sfml-window SYSTEM PRIVATE "${PROJECT_SOURCE_DIR}/extlibs/headers/vulkan")

# CMake 3.11 and later prefer to choose GLVND, but we choose legacy OpenGL for backward compability
# (unless the OpenGL_GL_PREFERENCE was explicitly set)
# See CMP0072 for more details (cmake --help-policy CMP0072)
if (NOT OpenGL_GL_PREFERENCE)
    set(OpenGL_GL_PREFERENCE "LEGACY")
endif()

if(SFML_OS_IOS)
    sfml_add_external(GLES LINK "-framework OpenGLES")

    target_link_libraries(sfml-window PRIVATE GLES)
elseif(SFML_OS_ANDROID)
    sfml_find_package(GLES INCLUDE "GLES_INCLUDE_DIR" LINK "GLES_LIBRARY")
    sfml_find_package(EGL INCLUDE "EGL_INCLUDE_DIR" LINK "EGL_LIBRARY")
    target_link_libraries(sfml-window PRIVATE EGL)
    target_link_libraries(sfml-window PRIVATE GLES)
else()
    sfml_find_package(OpenGL INCLUDE "OPENGL_INCLUDE_DIR" LINK "OPENGL_gl_LIBRARY")
    target_link_libraries(sfml-window PRIVATE OpenGL)
endif()

if(SFML_OS_WINDOWS AND NOT SFML_COMPILER_MSVC)
    include(CheckIncludeFile)
    check_include_file(dinput.h DINPUT_H_FOUND)
    if(NOT DINPUT_H_FOUND)
        target_include_directories(sfml-window SYSTEM PRIVATE "${PROJECT_SOURCE_DIR}/extlibs/headers/mingw")
    endif()
endif()

if(SFML_OS_LINUX)
    sfml_find_package(UDev INCLUDE "UDEV_INCLUDE_DIR" LINK "UDEV_LIBRARIES")
    target_link_libraries(sfml-window PRIVATE UDev dl)
elseif(SFML_OS_WINDOWS)
    target_link_libraries(sfml-window PRIVATE winmm gdi32)
elseif(SFML_OS_FREEBSD)
    target_link_libraries(sfml-window PRIVATE usbhid)
elseif(SFML_OS_MACOSX)
    target_link_libraries(sfml-window PRIVATE "-framework Foundation" "-framework AppKit" "-framework IOKit" "-framework Carbon")
elseif(SFML_OS_IOS)
    target_link_libraries(sfml-window PRIVATE "-framework Foundation" "-framework UIKit" "-framework CoreGraphics" "-framework QuartzCore" "-framework CoreMotion")
elseif(SFML_OS_ANDROID)
    target_link_libraries(sfml-window PRIVATE android)
endif()
