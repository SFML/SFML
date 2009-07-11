////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007-2009 Laurent Gomila (laurent.gom@gmail.com)
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

#ifndef SFML_CONTEXTGL_HPP
#define SFML_CONTEXTGL_HPP

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Config.hpp>
#include <SFML/Window/ContextSettings.hpp>
#include <SFML/System/NonCopyable.hpp>


namespace sf
{
namespace priv
{
class WindowImpl;

////////////////////////////////////////////////////////////
/// Abstract class representing an OpenGL context
////////////////////////////////////////////////////////////
class ContextGL : NonCopyable
{
public :

    ////////////////////////////////////////////////////////////
    /// Create a new context, not associated to a window
    ///
    /// \return Pointer to the created context
    ///
    ////////////////////////////////////////////////////////////
    static ContextGL* New();

    ////////////////////////////////////////////////////////////
    /// Create a new context attached to a window
    ///
    /// \param owner :        Pointer to the owner window
    /// \param bitsPerPixel : Pixel depth (in bits per pixel)
    /// \param settings :     Creation parameters
    ///
    /// \return Pointer to the created context
    ///
    ////////////////////////////////////////////////////////////
    static ContextGL* New(const WindowImpl* owner, unsigned int bitsPerPixel, const ContextSettings& settings);

public :

    ////////////////////////////////////////////////////////////
    /// Virtual destructor
    ///
    ////////////////////////////////////////////////////////////
    virtual ~ContextGL();

    ////////////////////////////////////////////////////////////
    /// Get the settings of the context
    ///
    /// \return Structure containing the settings
    ///
    ////////////////////////////////////////////////////////////
    const ContextSettings& GetSettings() const;

    ////////////////////////////////////////////////////////////
    /// Activate or deactivate the context as the current target
    /// for rendering
    ///
    /// \param active : True to activate, false to deactivate
    ///
    /// \return True if operation was successful, false otherwise
    ///
    ////////////////////////////////////////////////////////////
    bool SetActive(bool active);

    ////////////////////////////////////////////////////////////
    /// Display the contents of the context
    ///
    ////////////////////////////////////////////////////////////
    virtual void Display() = 0;

    ////////////////////////////////////////////////////////////
    /// Enable / disable vertical synchronization
    ///
    /// \param enabled : True to enable v-sync, false to deactivate
    ///
    ////////////////////////////////////////////////////////////
    virtual void UseVerticalSync(bool enabled) = 0;

protected :

    ////////////////////////////////////////////////////////////
    /// Default constructor
    ///
    ////////////////////////////////////////////////////////////
    ContextGL();

    ////////////////////////////////////////////////////////////
    /// Make this context the current one
    ///
    /// \param active : True to activate, false to deactivate
    ///
    /// \return True on success, false if any error happened
    ///
    ////////////////////////////////////////////////////////////
    virtual bool MakeCurrent(bool active) = 0;

    ////////////////////////////////////////////////////////////
    /// Evaluate a pixel format configuration.
    /// This functions can be used by implementations that have
    /// several valid formats and want to get the best one
    ///
    /// \param bitsPerPixel : Requested pixel depth (bits per pixel)
    /// \param settings :     Requested additionnal settings
    /// \param colorBits :    Color bits of the configuration to evaluate
    /// \param depthBits :    Depth bits of the configuration to evaluate
    /// \param stencilBits :  Stencil bits of the configuration to evaluate
    /// \param antialiasing : Antialiasing level of the configuration to evaluate
    ///
    /// \return Score of the configuration : the lower the better
    ///
    ////////////////////////////////////////////////////////////
    static int EvaluateFormat(unsigned int bitsPerPixel, const ContextSettings& settings, int colorBits, int depthBits, int stencilBits, int antialiasing);

    ////////////////////////////////////////////////////////////
    // Member data
    ////////////////////////////////////////////////////////////
    ContextSettings mySettings; ///< Creation settings of the context
};

} // namespace priv

} // namespace sf


#endif // SFML_CONTEXTGL_HPP
