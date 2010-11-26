////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007-2010 Marco Antognini (antognini.marco@gmail.com), 
//                         Laurent Gomila (laurent.gom@gmail.com), 
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
#import <SFML/Window/OSX/WindowImplDelegateProtocol.h>

////////////////////////////////////////////////////////////
/// Predefine some classes
////////////////////////////////////////////////////////////
namespace sf {
	namespace priv {
		class WindowImplCocoa;
	}
	class VideoMode;
}

@class SFOpenGLView;

////////////////////////////////////////////////////////////
/// Implementation of WindowImplDelegateProtocol for window managment.
/// 
/// Key and mouse events are delegated to its view.
/// Window events are managed by this class.
///
/// Used when SFML handle everything and when a NSWindow* is given
/// as handle to WindowImpl.
////////////////////////////////////////////////////////////

#ifdef USE_OS_X_VERSION_10_4
@interface SFWindowController : NSResponder <WindowImplDelegateProtocol> {
#else /* USE_OS_X_VERSION_10_6 */
@interface SFWindowController : NSResponder <WindowImplDelegateProtocol, NSWindowDelegate> {
#endif
	NSWindow*										myWindow;
	SFOpenGLView*								myOGLView;
	sf::priv::WindowImplCocoa*	myRequester;
}

-(id)initWithWindow:(NSWindow*)window;
-(id)initWithMode:(sf::VideoMode const*)mode andStyle:(unsigned long)style;
-(void)dealloc;

-(float)titlebarHeight;

@end
