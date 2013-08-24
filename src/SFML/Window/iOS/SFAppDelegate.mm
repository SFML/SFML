////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007-2013 Laurent Gomila (laurent.gom@gmail.com)
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
#include <SFML/Window/iOS/SFAppDelegate.hpp>
#include <SFML/Window/iOS/SFMain.hpp>


namespace
{
    // Save the main's arguments, to pass them back to the user's main
    int mainArgc;
    char** mainArgv;

    // Save the global instance of the delegate
    SFAppDelegate* delegateInstance = NULL;
}


@interface SFAppDelegate()

@property (nonatomic) CMMotionManager* motionManager;
@property (nonatomic) sf::Vector2i touchPosition;

@end


@implementation SFAppDelegate

@synthesize sfWindow;


////////////////////////////////////////////////////////////
+(int)main:(int)argc argv:(char**)argv
{
    mainArgc = argc;
    mainArgv = argv;
    return UIApplicationMain(argc, argv, nil, NSStringFromClass([SFAppDelegate class]));
}


////////////////////////////////////////////////////////////
+(SFAppDelegate*)getInstance
{
    return delegateInstance;
}


////////////////////////////////////////////////////////////
-(void)runUserMain
{
    sfmlMain(mainArgc, mainArgv);
}


////////////////////////////////////////////////////////////
- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions
{
    // Save the delegate instance
    delegateInstance = self;

    // Instanciate the motion manager
    self.motionManager = [[CMMotionManager alloc] init];

    // Initialize the touch position
    self.touchPosition = sf::Vector2i(-1, -1);

    // Register orientation changes notifications
    [[UIDevice currentDevice] beginGeneratingDeviceOrientationNotifications];
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(deviceOrientationDidChange:) name:UIDeviceOrientationDidChangeNotification object: nil];
    
    // Schedule an indirect call to the user main, so that this call (and the whole
    // init sequence) can end, and the default splashscreen can be destroyed
    [self performSelector:@selector(runUserMain) withObject:nil afterDelay:0.0];

    return true;
}


////////////////////////////////////////////////////////////
- (void)applicationWillResignActive:(UIApplication *)application
{
    // Called when:
    // - the application is sent to background
    // - the application is interrupted by a call or message

    // Generate a LostFocus event
    if (self.sfWindow)
    {
        sf::Event event;
        event.type = sf::Event::LostFocus;
        sfWindow->pushEvent(event);
    }
}


////////////////////////////////////////////////////////////
- (void)applicationDidEnterBackground:(UIApplication *)application
{
    // Called when the application is sent to background (home button pressed)
}


////////////////////////////////////////////////////////////
- (void)applicationDidBecomeActive:(UIApplication *)application
{
    // Called when:
    // - the application is sent to foreground
    // - the application was interrupted by a call or message

    // Generate a GainedFocus event
    if (self.sfWindow)
    {
        sf::Event event;
        event.type = sf::Event::GainedFocus;
        sfWindow->pushEvent(event);
    }
}


////////////////////////////////////////////////////////////
- (void)applicationWillEnterForeground:(UIApplication *)application
{
    // Called when the application is sent to foreground (app icon pressed)
}


////////////////////////////////////////////////////////////
- (void)applicationWillTerminate:(UIApplication *)application
{
    // Generate a Closed event
    if (self.sfWindow)
    {
        sf::Event event;
        event.type = sf::Event::Closed;
        sfWindow->pushEvent(event);
    }
}


////////////////////////////////////////////////////////////
- (void)deviceOrientationDidChange:(NSNotification *)notification
{
    if (self.sfWindow)
    {
        // Get the new orientation
        UIDeviceOrientation orientation = [[UIDevice currentDevice] orientation];

        // Filter interesting orientations
        if ((orientation == UIDeviceOrientationLandscapeLeft) ||
            (orientation == UIDeviceOrientationLandscapeRight) ||
            (orientation == UIDeviceOrientationPortrait) ||
            (orientation == UIDeviceOrientationPortraitUpsideDown))
        {
            // Get the new size
            CGSize size = [UIScreen mainScreen].bounds.size;
            if (UIDeviceOrientationIsLandscape(orientation))
                std::swap(size.width, size.height);

            // Send a Resized event to the current window
            sf::Event event;
            event.type = sf::Event::Resized;
            event.size.width = size.width;
            event.size.height = size.height;
            sfWindow->pushEvent(event);
        }
    }
}

////////////////////////////////////////////////////////////
- (void)setVirtualKeyboardVisible:(bool)visible
{
    if (self.sfWindow)
        self.sfWindow->setVirtualKeyboardVisible(visible);
}


////////////////////////////////////////////////////////////
- (sf::Vector2i)getTouchPosition
{
    return self.touchPosition;
}


////////////////////////////////////////////////////////////
- (void)notifyTouchBeginAt:(CGPoint)position
{
    self.touchPosition = sf::Vector2i(static_cast<int>(position.x), static_cast<int>(position.y));

    if (self.sfWindow)
    {
        sf::Event event;
        event.type = sf::Event::MouseButtonPressed;
        event.mouseButton.x = position.x;
        event.mouseButton.y = position.y;
        event.mouseButton.button = sf::Mouse::Left;
        sfWindow->pushEvent(event);
    }
}


////////////////////////////////////////////////////////////
- (void)notifyTouchMoveAt:(CGPoint)position
{
    self.touchPosition = sf::Vector2i(static_cast<int>(position.x), static_cast<int>(position.y));

    if (self.sfWindow)
    {
        sf::Event event;
        event.type = sf::Event::MouseMoved;
        event.mouseMove.x = position.x;
        event.mouseMove.y = position.y;
        sfWindow->pushEvent(event);
    }
}


////////////////////////////////////////////////////////////
- (void)notifyTouchEndAt:(CGPoint)position
{
    self.touchPosition = sf::Vector2i(-1, -1);

    if (self.sfWindow)
    {
        sf::Event event;
        event.type = sf::Event::MouseButtonReleased;
        event.mouseButton.x = position.x;
        event.mouseButton.y = position.y;
        event.mouseButton.button = sf::Mouse::Left;
        sfWindow->pushEvent(event);
    }
}


////////////////////////////////////////////////////////////
- (void)notifyCharacter:(sf::Uint32)character
{
    if (self.sfWindow)
    {
        sf::Event event;
        event.type = sf::Event::TextEntered;
        event.text.unicode = character;
        sfWindow->pushEvent(event);
    }
}


@end
