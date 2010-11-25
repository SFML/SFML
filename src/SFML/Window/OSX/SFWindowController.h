

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
