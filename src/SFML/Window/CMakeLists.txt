# define the sfml-window target
sfml_add_library(Window)

# all source files
target_sources(sfml-window PRIVATE
    Clipboard.cpp
    Context.cpp
    Cursor.cpp
    EGLCheck.cpp
    EglContext.cpp
    GlContext.cpp
    GlResource.cpp
    Joystick.cpp
    JoystickManager.cpp
    Keyboard.cpp
    Mouse.cpp
    Touch.cpp
    Sensor.cpp
    SensorManager.cpp
    VideoMode.cpp
    Vulkan.cpp
    Window.cpp
    WindowBase.cpp
    WindowImpl.cpp
)

# add platform specific sources
if(SFML_OS_WINDOWS)
    target_sources(sfml-window PRIVATE
        Win32/CursorImpl.cpp
        Win32/ClipboardImpl.cpp
        Win32/WglContext.cpp
        Win32/InputImpl.cpp
        Win32/JoystickImpl.cpp
        Win32/SensorImpl.cpp
        Win32/VideoModeImpl.cpp
        Win32/VulkanImplWin32.cpp
        Win32/WindowImplWin32.cpp
    )

    # make sure that we use the Unicode version of the Win API functions
    add_definitions(-DUNICODE -D_UNICODE)
elseif(SFML_OS_LINUX OR SFML_OS_FREEBSD OR SFML_OS_OPENBSD OR SFML_OS_NETBSD)
    target_sources(sfml-window PRIVATE
        Unix/CursorImpl.cpp
        Unix/ClipboardImpl.cpp
        Unix/Display.cpp
        Unix/InputImpl.cpp
        Unix/SensorImpl.cpp
        Unix/VideoModeImpl.cpp
        Unix/VulkanImplX11.cpp
        Unix/WindowImplX11.cpp
    )
    if(NOT SFML_OS_ANDROID)
        target_sources(sfml-window PRIVATE Unix/GlxContext.cpp)
    endif()
    if(SFML_OS_LINUX)
        target_sources(sfml-window PRIVATE Unix/JoystickImpl.cpp)
    elseif(SFML_OS_FREEBSD)
        target_sources(sfml-window PRIVATE FreeBSD/JoystickImpl.cpp)
    elseif(SFML_OS_OPENBSD)
        target_sources(sfml-window PRIVATE OpenBSD/JoystickImpl.cpp)
    elseif(SFML_OS_NETBSD)
        target_sources(sfml-window PRIVATE NetBSD/JoystickImpl.cpp)
    endif()
elseif(SFML_OS_MACOSX)
    target_sources(sfml-window PRIVATE
        OSX/cpp_objc_conversion.mm
        OSX/cg_sf_conversion.mm
        OSX/CursorImpl.mm
        OSX/ClipboardImpl.mm
        OSX/InputImpl.mm
        OSX/HIDInputManager.mm
        OSX/HIDJoystickManager.cpp
        OSX/JoystickImpl.cpp
        OSX/NSImage+raw.mm
        OSX/SensorImpl.cpp
        OSX/SFApplication.m
        OSX/SFApplicationDelegate.m
        OSX/SFContext.mm
        OSX/SFKeyboardModifiersHelper.mm
        OSX/SFOpenGLView.mm
        OSX/SFOpenGLView+keyboard.mm
        OSX/SFOpenGLView+mouse.mm
        OSX/SFSilentResponder.m
        OSX/SFWindow.m
        OSX/SFWindowController.mm
        OSX/SFViewController.mm
        OSX/VideoModeImpl.cpp
        OSX/WindowImplCocoa.mm
        OSX/AutoreleasePoolWrapper.mm
    )
elseif(SFML_OS_IOS)
    target_sources(sfml-window PRIVATE
        iOS/CursorImpl.cpp
        iOS/ClipboardImpl.mm
        iOS/EaglContext.mm
        iOS/InputImpl.mm
        iOS/JoystickImpl.mm
        iOS/SensorImpl.mm
        iOS/VideoModeImpl.mm
        iOS/WindowImplUIKit.mm
        iOS/SFAppDelegate.mm
        iOS/SFView.mm
        iOS/SFViewController.mm
        iOS/SFMain.mm
    )
elseif(SFML_OS_ANDROID)
    target_sources(sfml-window PRIVATE
        Android/CursorImpl.cpp
        Android/ClipboardImpl.cpp
        Android/WindowImplAndroid.cpp
        Android/VideoModeImpl.cpp
        Android/InputImpl.cpp
        Android/JoystickImpl.cpp
        Android/SensorImpl.cpp
    )
endif()

target_link_libraries(sfml-window PUBLIC SFML::System)

# glad sources
target_include_directories(sfml-window SYSTEM PRIVATE "${PROJECT_SOURCE_DIR}/extlibs/headers/glad/include")

# When static linking on macOS, we need to add this flag for objective C to work
if ((NOT BUILD_SHARED_LIBS) AND SFML_OS_MACOSX)
    target_link_libraries(sfml-window PRIVATE -ObjC)
endif()

# Vulkan headers
target_include_directories(sfml-window SYSTEM PRIVATE "${PROJECT_SOURCE_DIR}/extlibs/headers/vulkan")

# find and setup usage for external libraries
if(SFML_OS_LINUX OR SFML_OS_FREEBSD OR SFML_OS_OPENBSD OR SFML_OS_NETBSD)
    sfml_find_package(X11 INCLUDE "X11_INCLUDE_DIR" LINK "X11_X11_LIB" "X11_Xrandr_LIB" "X11_Xcursor_LIB")
    target_link_libraries(sfml-window PRIVATE X11)
endif()

# CMake 3.11 and later prefer to choose GLVND, but we choose legacy OpenGL for backward compability
# (unless the OpenGL_GL_PREFERENCE was explicitly set)
# See CMP0072 for more details (cmake --help-policy CMP0072)
if ((NOT ${CMAKE_VERSION} VERSION_LESS 3.11) AND (NOT OpenGL_GL_PREFERENCE))
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
