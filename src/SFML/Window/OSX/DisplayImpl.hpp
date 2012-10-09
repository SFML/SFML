////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007-2012 Laurent Gomila (laurent.gom@gmail.com)
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

#ifndef SFML_DISPLAYIMPL_HPP
#define SFML_DISPLAYIMPL_HPP

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <QuartzCore/QuartzCore.h>
#include <SFML/Window/VideoMode.hpp>

namespace sf
{
namespace priv
{
////////////////////////////////////////////////////////////
/// \brief OS-specific implementation of display management
///
////////////////////////////////////////////////////////////
class DisplayImpl
{
public :
    
    ////////////////////////////////////////////////////////////
    /// \brief Default constructor
    ///
    /// \param display Id of display to manage
    ///
    ////////////////////////////////////////////////////////////
    DisplayImpl(CGDirectDisplayID display);
    
    ////////////////////////////////////////////////////////////
    /// \brief Destructor
    ///
    ////////////////////////////////////////////////////////////
    ~DisplayImpl();

    ////////////////////////////////////////////////////////////
    /// \brief Get the modes supported by the display
    ///
    ////////////////////////////////////////////////////////////
    std::vector<VideoMode> getModes();

    ////////////////////////////////////////////////////////////
    /// \brief Set the video mode for the display
    ///
    /// \param mode Video mode to use
    ///
    ////////////////////////////////////////////////////////////
    bool setMode(VideoMode const& mode);

public :

    ////////////////////////////////////////////////////////////
    /// \brief Wrapper linking video modes to native modes
    ///
    ////////////////////////////////////////////////////////////
    
    typedef struct {
        VideoMode           mode;   ///< Video mode
        CGDisplayModeRef    ref;    ///< Native mode reference
    } NativeDisplayMode;
    
private :
    
    ////////////////////////////////////////////////////////////
    // Member data
    ////////////////////////////////////////////////////////////
    std::vector<NativeDisplayMode>  m_modes;    ///< Supported modes
    CGDirectDisplayID               m_display;  ///< Id of the display
};

} // namespace priv
    
} // namespace sf


#endif // SFML_DISPLAYIMPL_HPP
