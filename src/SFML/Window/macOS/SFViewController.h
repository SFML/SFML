////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007-2025 Marco Antognini (antognini.marco@gmail.com),
//                         Laurent Gomila (laurent@sfml-dev.org)
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
#import <SFML/Window/macOS/WindowImplDelegateProtocol.h>

////////////////////////////////////////////////////////////
/// Predefine some classes
////////////////////////////////////////////////////////////
@class SFOpenGLView;

////////////////////////////////////////////////////////////
/// \brief Implementation of WindowImplDelegateProtocol for view management
///
////////////////////////////////////////////////////////////
// NOLINTBEGIN(readability-identifier-naming)
@interface SFViewController : NSObject<WindowImplDelegateProtocol>
{
    NSView*                    m_view;      ///< Underlying Cocoa view
    SFOpenGLView*              m_oglView;   ///< OpenGL view
    sf::priv::WindowImplCocoa* m_requester; ///< View's requester
}
// NOLINTEND(readability-identifier-naming)

////////////////////////////////////////////////////////////
/// \brief Initialize the view controller
///
/// \param view view to be controlled
///
/// \return an initialized view controller
///
////////////////////////////////////////////////////////////
- (id)initWithView:(NSView*)view;

@end
