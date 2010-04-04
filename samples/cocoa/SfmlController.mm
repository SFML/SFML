
////////////////////////////////////////////////////////////
// Note :
// - we use .mm extension (Objective-C++) instead of .m (Objective-C) to allow
// the use of SFML objects in a Cocoa application
////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#import "SfmlController.h"

#define FPS 60.0f // framerate limit

@implementation SfmlController 

// Called when all the outlets are valid
- (void)awakeFromNib 
{ 
	// Bind the SFML window to the 'myView' NSView object
	mySfmlWindow = new sf::RenderWindow(myView); 
	
	// Create the SFML loop for handling events and drawing
	[NSTimer scheduledTimerWithTimeInterval:(1.f/FPS)
									 target:self
								   selector:@selector(display:)
								   userInfo:nil
									repeats:YES];
	
	[self onInit];
} 

- (void)onInit 
{
	// Do your SFML initialization stuff here
	myColor = sf::Color::Blue;
}

// Called when the Apply button is clicked
- (IBAction)onApply:(id)sender
{
	// Define your 'Apply' action here
	if (myColor == sf::Color::Blue) {
		myColor = sf::Color::Red;
	} else {
		myColor = sf::Color::Blue;
	}

}

- (void)display:(NSTimer *)theTimer 
{ 
	if(!mySfmlWindow->IsOpened()) {
		// Stop the SFML loop as soon as the window is closed
		// (thus the user can still use the app)
		[theTimer invalidate];
	} else {
		// Do the general SFML event handling
		sf::Event Event; 
		while (mySfmlWindow->GetEvent(Event)) { 
			if (Event.Type == sf::Event::Closed) {
				mySfmlWindow->Close(); 
			}
			
			if ((Event.Type == sf::Event::KeyPressed) &&
				(Event.Key.Code == sf::Key::Escape)) {
				mySfmlWindow->Close(); 
			}
		} 
		
		// Do the general SFML display process
		mySfmlWindow->SetActive(); 
		mySfmlWindow->Clear(); 
		mySfmlWindow->Draw(sf::Shape::Circle(50.f, 50.f, 20.f, myColor)); 
		mySfmlWindow->Display(); 
	}
} 

@end 

