////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007-2019 Laurent Gomila (laurent@sfml-dev.org)
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

#ifndef SFML_RENDERER_HPP
#define SFML_RENDERER_HPP

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Graphics/Export.hpp>


namespace sf
{
namespace Renderer
{
    ////////////////////////////////////////////////////////////
    /// \ingroup graphics
    /// \brief Enumeration of the renderer types
    ///
    ////////////////////////////////////////////////////////////
    enum
    {
        Legacy  = 0,      ///< Let SFML choose the renderer for best compatibility
        OpenGL1 = 1 << 0, ///< OpenGL 1.x renderer

        Default = Legacy ///< Default renderer
    };
}

////////////////////////////////////////////////////////////
/// \ingroup graphics
/// \brief Get the available renderers
///
/// SFML determines at runtime the renderers that are supported
/// on the target system. These are returned by this function.
/// When calling setRenderers(), only renderers returned by this
/// function are allowed to be specified.
///
/// \return Renderers supported on the current system
///
/// \see setRenderers, getRenderer
///
////////////////////////////////////////////////////////////
Uint32 SFML_GRAPHICS_API getAvailableRenderers();

////////////////////////////////////////////////////////////
/// \ingroup graphics
/// \brief Set the renderers SFML is allowed to pick from
///
/// Before anything graphics related is performed, it is possible
/// to specify via this function the renderers that SFML is allowed
/// to choose from to perform any future rendering operations.
///
/// The renderers available on the target system can be retrieved
/// via getAvailableRenderers().
///
/// Once SFML has chosen a renderer, it is final until the next time
/// the application is run. As such, this function must be called as
/// soon as possible if the user decides they want influence the
/// selection process in any way.
///
/// If the applicable renderers are not explicitly set by calling this
/// function, SFML will automatically choose from everything that is
/// supported on the target system.
///
/// \param renderers Bit-wise OR of renderers SFML is allowed to pick from
///
/// \see getAvailableRenderers, getRenderer
///
////////////////////////////////////////////////////////////
void SFML_GRAPHICS_API setRenderers(Uint32 renderers);

////////////////////////////////////////////////////////////
/// \ingroup graphics
/// \brief Get the renderer SFML has selected to use
///
/// Calling this function will force SFML to finalize its decision
/// on which renderer it wants to make use of if it has not already
/// done so. The selected renderer is then returned by this function.
///
/// After this function has been called either from user code or
/// internally within SFML itself, calling setRenderers() will no
/// longer have any effect until the next time the application is run.
///
/// \return The renderer SFML has selected to use
///
/// \see setRenderers
///
////////////////////////////////////////////////////////////
Uint32 SFML_GRAPHICS_API getRenderer();

} // namespace sf


#endif // SFML_RENDERER_HPP
