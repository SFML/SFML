////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007-2024 Laurent Gomila (laurent@sfml-dev.org)
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

#include <SFML/Config.hpp>

#include <cstdint>

namespace sf
{
////////////////////////////////////////////////////////////
/// \brief Structure defining the settings of the OpenGL
///        context attached to a window
///
////////////////////////////////////////////////////////////
struct ContextSettings
{
    ////////////////////////////////////////////////////////////
    /// \brief Enumeration of the context attribute flags
    ///
    ////////////////////////////////////////////////////////////
    enum Attribute
    {
        Default = 0,      //!< Non-debug, compatibility context (this and the core attribute are mutually exclusive)
        Core    = 1 << 0, //!< Core attribute
        Debug   = 1 << 2  //!< Debug attribute
    };


    ////////////////////////////////////////////////////////////
    // Member data
    ////////////////////////////////////////////////////////////
    unsigned int  depthBits{};                        //!< Bits of the depth buffer
    unsigned int  stencilBits{};                      //!< Bits of the stencil buffer
    unsigned int  antiAliasingLevel{};                //!< Level of anti-aliasing
    unsigned int  majorVersion{1};                    //!< Major number of the context version to create
    unsigned int  minorVersion{1};                    //!< Minor number of the context version to create
    std::uint32_t attributeFlags{Attribute::Default}; //!< The attribute flags to create the context with
    bool          sRgbCapable{};                      //!< Whether the context framebuffer is sRGB capable
};

} // namespace sf


////////////////////////////////////////////////////////////
/// \class sf::ContextSettings
/// \ingroup window
///
/// ContextSettings allows to define several advanced settings
/// of the OpenGL context attached to a window. All these
/// settings with the exception of the compatibility flag
/// and anti-aliasing level have no impact on the regular
/// SFML rendering (graphics module), so you may need to use
/// this structure only if you're using SFML as a windowing
/// system for custom OpenGL rendering.
///
/// The depthBits and stencilBits members define the number
/// of bits per pixel requested for the (respectively) depth
/// and stencil buffers.
///
/// antiAliasingLevel represents the requested number of
/// multisampling levels for anti-aliasing.
///
/// majorVersion and minorVersion define the version of the
/// OpenGL context that you want. Only versions greater or
/// equal to 3.0 are relevant; versions lesser than 3.0 are
/// all handled the same way (i.e. you can use any version
/// < 3.0 if you don't want an OpenGL 3 context).
///
/// When requesting a context with a version greater or equal
/// to 3.2, you have the option of specifying whether the
/// context should follow the core or compatibility profile
/// of all newer (>= 3.2) OpenGL specifications. For versions
/// 3.0 and 3.1 there is only the core profile. By default
/// a compatibility context is created. You only need to specify
/// the core flag if you want a core profile context to use with
/// your own OpenGL rendering.
/// <b>Warning: The graphics module will not function if you
/// request a core profile context. Make sure the attributes are
/// set to Default if you want to use the graphics module.</b>
///
/// Setting the debug attribute flag will request a context with
/// additional debugging features enabled. Depending on the
/// system, this might be required for advanced OpenGL debugging.
/// OpenGL debugging is disabled by default.
///
/// <b>Special Note for macOS:</b>
/// Apple only supports choosing between either a legacy context
/// (OpenGL 2.1) or a core context (OpenGL version depends on the
/// operating system version but is at least 3.2). Compatibility
/// contexts are not supported. Further information is available on the
/// <a href="https://developer.apple.com/opengl/capabilities/index.html">
/// OpenGL Capabilities Tables</a> page. macOS also currently does
/// not support debug contexts.
///
/// Please note that these values are only a hint.
/// No failure will be reported if one or more of these values
/// are not supported by the system; instead, SFML will try to
/// find the closest valid match. You can then retrieve the
/// settings that the window actually used to create its context,
/// with `Window::getSettings()`.
///
////////////////////////////////////////////////////////////
