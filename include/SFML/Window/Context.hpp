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

#ifndef SFML_CONTEXT_HPP
#define SFML_CONTEXT_HPP

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
}

////////////////////////////////////////////////////////////
/// Abstract class representing an OpenGL context
////////////////////////////////////////////////////////////
class SFML_API Context : NonCopyable
{
public :

    ////////////////////////////////////////////////////////////
    /// Create a new context, not associated to a window
    ///
    /// \return Pointer to the created context
    ///
    ////////////////////////////////////////////////////////////
    static Context* New();

    ////////////////////////////////////////////////////////////
    /// Create a new context attached to a window
    ///
    /// \param Owner :        Pointer to the owner window
    /// \param BitsPerPixel : Pixel depth (in bits per pixel)
    /// \param Settings :     Creation parameters
    ///
    /// \return Pointer to the created context
    ///
    ////////////////////////////////////////////////////////////
    static Context* New(const priv::WindowImpl* Owner, unsigned int BitsPerPixel, const ContextSettings& Settings);

    ////////////////////////////////////////////////////////////
    /// Check if a context is active on the current thread
    ///
    /// \return True if there's an active context, false otherwise
    ///
    ////////////////////////////////////////////////////////////
    static bool IsContextActive();

    ////////////////////////////////////////////////////////////
    /// Return the default context
    ///
    /// \return Reference to the default context
    ///
    ////////////////////////////////////////////////////////////
    static Context& GetDefault();

public :

    ////////////////////////////////////////////////////////////
    /// Virtual destructor
    ///
    ////////////////////////////////////////////////////////////
    virtual ~Context();

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
    /// \param Active : True to activate, false to deactivate
    ///
    /// \return True if operation was successful, false otherwise
    ///
    ////////////////////////////////////////////////////////////
    bool SetActive(bool Active);

    ////////////////////////////////////////////////////////////
    /// Display the contents of the context
    ///
    ////////////////////////////////////////////////////////////
    virtual void Display() = 0;

    ////////////////////////////////////////////////////////////
    /// Enable / disable vertical synchronization
    ///
    /// \param Enabled : True to enable v-sync, false to deactivate
    ///
    ////////////////////////////////////////////////////////////
    virtual void UseVerticalSync(bool Enabled) = 0;

protected :

    ////////////////////////////////////////////////////////////
    /// Default constructor
    ///
    ////////////////////////////////////////////////////////////
    Context();

    ////////////////////////////////////////////////////////////
    /// Make this context the current one
    ///
    /// \param Active : True to activate, false to deactivate
    ///
    /// \return True on success, false if any error happened
    ///
    ////////////////////////////////////////////////////////////
    virtual bool MakeCurrent(bool Active) = 0;

    ////////////////////////////////////////////////////////////
    /// Evaluate a pixel format configuration.
    /// This functions can be used by implementations that have
    /// several valid formats and want to get the best one
    ///
    /// \param BitsPerPixel : Requested pixel depth (bits per pixel)
    /// \param Settings :     Requested additionnal settings
    /// \param ColorBits :    Color bits of the configuration to evaluate
    /// \param DepthBits :    Depth bits of the configuration to evaluate
    /// \param StencilBits :  Stencil bits of the configuration to evaluate
    /// \param Antialiasing : Antialiasing level of the configuration to evaluate
    ///
    /// \return Score of the configuration : the lower the better
    ///
    ////////////////////////////////////////////////////////////
    static int EvaluateFormat(unsigned int BitsPerPixel, const ContextSettings& Settings, int ColorBits, int DepthBits, int StencilBits, int Antialiasing);

    ////////////////////////////////////////////////////////////
    // Member data
    ////////////////////////////////////////////////////////////
    ContextSettings mySettings; ///< Creation settings of the context
};

} // namespace sf


#endif // SFML_CONTEXT_HPP
