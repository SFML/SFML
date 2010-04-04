
////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#import <Cocoa/Cocoa.h> 
#import <SFML/Graphics.hpp> 

@interface SfmlController : NSWindowController 
{ 
	IBOutlet NSView* myView; 
	sf::RenderWindow *mySfmlWindow; 
	sf::Color myColor;
} 

- (void)display:(NSTimer*)theTimer; 
- (void)onInit; 

- (IBAction)onApply:(id)sender;

@end 
