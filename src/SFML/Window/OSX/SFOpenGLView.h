
////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#import <AppKit/AppKit.h>

namespace sf {
	namespace priv {
		class WindowImplCocoa;
	}
}

////////////////////////////////////////////////////////////
/// \brief Spesialized NSOpenGLView
///
/// Handle event and send them back to the requester.
////////////////////////////////////////////////////////////
@interface SFOpenGLView : NSOpenGLView {
	sf::priv::WindowImplCocoa*	myRequester;
	BOOL												myUseKeyRepeat;
	NSTrackingRectTag						myTrackingTag;
	BOOL												myMouseIsIn;
}


-(id)initWithFrame:(NSRect)frameRect;
-(void)setRequesterTo:(sf::priv::WindowImplCocoa*)requester;

-(void)enableKeyRepeat;
-(void)disableKeyRepeat;

-(void)frameDidChange:(NSNotification*)notification;

-(BOOL)isMouseInside;

@end
