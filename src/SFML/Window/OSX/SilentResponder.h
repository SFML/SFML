
#import <AppKit/AppKit.h>

@interface SilentResponder : NSResponder {
}
- (void) doCommandBySelector:(SEL) sel;
@end

