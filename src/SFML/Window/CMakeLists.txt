
set(INCROOT ${PROJECT_SOURCE_DIR}/include/SFML/Window)
set(SRCROOT ${PROJECT_SOURCE_DIR}/src/SFML/Window)

# all source files
set(SRC
    ${SRCROOT}/Context.cpp
    ${INCROOT}/Context.hpp
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
    ${SRCROOT}/VideoMode.cpp
    ${INCROOT}/VideoMode.hpp
    ${SRCROOT}/VideoModeImpl.hpp
    ${SRCROOT}/Window.cpp
    ${INCROOT}/Window.hpp
    ${INCROOT}/WindowHandle.hpp
    ${SRCROOT}/WindowImpl.cpp
    ${SRCROOT}/WindowImpl.hpp
    ${INCROOT}/WindowStyle.hpp
)
source_group("" FILES ${SRC})

# add platform specific sources
if(WINDOWS)
    set(PLATFORM_SRC
        ${SRCROOT}/Win32/WglContext.cpp
        ${SRCROOT}/Win32/WglContext.hpp
        ${SRCROOT}/Win32/InputImpl.cpp
        ${SRCROOT}/Win32/InputImpl.hpp
        ${SRCROOT}/Win32/JoystickImpl.cpp
        ${SRCROOT}/Win32/JoystickImpl.hpp
        ${SRCROOT}/Win32/VideoModeImpl.cpp
        ${SRCROOT}/Win32/WindowImplWin32.cpp
        ${SRCROOT}/Win32/WindowImplWin32.hpp
    )
    source_group("windows" FILES ${PLATFORM_SRC})
elseif(LINUX)
    set(PLATFORM_SRC
        ${SRCROOT}/Linux/Display.cpp
        ${SRCROOT}/Linux/Display.hpp
        ${SRCROOT}/Linux/GlxContext.cpp
        ${SRCROOT}/Linux/GlxContext.hpp
        ${SRCROOT}/Linux/InputImpl.cpp
        ${SRCROOT}/Linux/InputImpl.hpp
        ${SRCROOT}/Linux/JoystickImpl.cpp
        ${SRCROOT}/Linux/JoystickImpl.hpp
        ${SRCROOT}/Linux/VideoModeImpl.cpp
        ${SRCROOT}/Linux/WindowImplX11.cpp
        ${SRCROOT}/Linux/WindowImplX11.hpp
    )
    source_group("linux" FILES ${PLATFORM_SRC})
else() # MACOSX
    set(PLATFORM_SRC
        ${SRCROOT}/OSX/cpp_objc_conversion.h
        ${SRCROOT}/OSX/cpp_objc_conversion.mm
        ${SRCROOT}/OSX/cg_sf_conversion.hpp
        ${SRCROOT}/OSX/cg_sf_conversion.cpp
        ${SRCROOT}/OSX/InputImpl.mm
        ${SRCROOT}/OSX/InputImpl.hpp
        ${SRCROOT}/OSX/HIDInputManager.hpp
        ${SRCROOT}/OSX/HIDInputManager.mm
        ${SRCROOT}/OSX/HIDJoystickManager.hpp
        ${SRCROOT}/OSX/HIDJoystickManager.cpp
        ${SRCROOT}/OSX/JoystickImpl.cpp
        ${SRCROOT}/OSX/JoystickImpl.hpp
        ${SRCROOT}/OSX/SFApplication.h
        ${SRCROOT}/OSX/SFApplication.m
        ${SRCROOT}/OSX/SFContext.hpp
        ${SRCROOT}/OSX/SFContext.mm
        ${SRCROOT}/OSX/SFKeyboardModifiersHelper.h
        ${SRCROOT}/OSX/SFKeyboardModifiersHelper.mm
        ${SRCROOT}/OSX/SFOpenGLView.h
        ${SRCROOT}/OSX/SFOpenGLView.mm
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
        ${SRCROOT}/OSX/AutoreleasePoolWrapper.h
        ${SRCROOT}/OSX/AutoreleasePoolWrapper.mm
    )
    source_group("mac" FILES ${PLATFORM_SRC})
endif()

# find external libraries
find_package(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIR})
if(LINUX)
    find_package(X11 REQUIRED)
    if(NOT X11_Xrandr_FOUND)
        message(FATAL_ERROR "Xrandr library not found")
    endif()
    include_directories(${X11_INCLUDE_DIR})
endif()

# build the list of external libraries to link
set(WINDOW_EXT_LIBS ${OPENGL_gl_LIBRARY})
if(WINDOWS)
    set(WINDOW_EXT_LIBS ${WINDOW_EXT_LIBS} winmm gdi32)
elseif(LINUX)
    set(WINDOW_EXT_LIBS ${WINDOW_EXT_LIBS} ${X11_X11_LIB} ${X11_Xrandr_LIB})
elseif(MACOSX)
    set(WINDOW_EXT_LIBS ${WINDOW_EXT_LIBS} "-framework Foundation -framework AppKit -framework IOKit -framework Carbon")
endif()

# define the sfml-window target
sfml_add_library(sfml-window
                 SOURCES ${SRC} ${PLATFORM_SRC}
                 DEPENDS sfml-system
                 EXTERNAL_LIBS ${WINDOW_EXT_LIBS})
