
////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#import <SFML/Window/OSX/SFApplication.h>
#import <AppKit/AppKit.h>


////////////////////////////////////////////////////////////
@implementation SFApplication


////////////////////////////////////////////////////////////
+(void)processEventWithBlockingMode:(BOOL)block
{
	[NSApplication sharedApplication]; // Make sure NSApp exists
	NSEvent* event = nil;
	
	if (block) { // At least one event is read.
		event = [NSApp nextEventMatchingMask:NSAnyEventMask 
															 untilDate:[NSDate distantFuture]
																	inMode:NSDefaultRunLoopMode 
																 dequeue:YES]; // Remove the event from the dequeue
		[NSApp sendEvent:event];
	}
	
	// If there are some other event read them.
	while (event = [NSApp nextEventMatchingMask:NSAnyEventMask
																		untilDate:[NSDate distantPast]
																			 inMode:NSDefaultRunLoopMode
																			dequeue:YES]) // Remove the event from the dequeue
	{
		[NSApp sendEvent:event];
	}
}


@end


