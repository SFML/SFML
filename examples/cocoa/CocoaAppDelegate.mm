////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007-2011 Marco Antognini (antognini.marco@gmail.com), 
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

#import "CocoaAppDelegate.h"
#import "NSString+stdstring.h"

// These define are used for converting the color of the NSPopUpButton
#define BLUE    @"Blue"
#define GREEN   @"Green"
#define RED     @"Red"

// Our PIMPL
struct SFMLmainWindow
{
    SFMLmainWindow(sf::WindowHandle win)
    : renderWindow(win)
    , background(sf::Color::Blue)
    {
        std::string resPath = [[[NSBundle mainBundle] resourcePath] tostdstring];
        if (!logo.LoadFromFile(resPath + "/logo.png")) {
            NSLog(@"Couldn't load the logo image");
        }
        
        logo.SetSmooth(true);
        
        sprite.SetTexture(logo, true);
        sf::FloatRect rect = sprite.GetLocalBounds();
        sf::Vector2f size(rect.Width, rect.Height);
        sprite.SetOrigin(size / 2.f);
        sprite.Scale(0.3, 0.3);
        
        unsigned int ww = renderWindow.GetWidth();
        unsigned int wh = renderWindow.GetHeight();
        sprite.SetPosition(sf::Vector2f(ww, wh) / 2.f);
        
        text.SetColor(sf::Color::White);
    }
    
    sf::RenderWindow    renderWindow;
    sf::Text            text;
    sf::Texture         logo;
    sf::Sprite          sprite;
    sf::Color           background;
};

// Private stuff
@interface CocoaAppDelegate ()

@property (assign) SFMLmainWindow   *mainWindow;
@property (retain) NSTimer          *renderTimer;
@property (assign) BOOL              visible;

@property (assign) BOOL              initialized;

-(void)renderMainWindow:(NSTimer *)aTimer;

@end


// Finally, the implementation
@implementation CocoaAppDelegate

@synthesize window          = _window;
@synthesize sfmlView        = _sfmlView;
@synthesize textField       = _textField;

@synthesize mainWindow      = _mainWindow;
@synthesize renderTimer     = _renderTimer;
@synthesize visible         = _visible;

@synthesize initialized     = _initialized;

- (id)init {
    self = [super init];
    if (self) {
        self.initialized = NO;
    }
    return self;
}

-(void)applicationDidFinishLaunching:(NSNotification *)aNotification
{
    if (!self.initialized)
    {
        // Init the 1st SFML render area.
        self.mainWindow = new SFMLmainWindow(self.sfmlView);
        self.mainWindow->text.SetString([self.textField.stringValue tostdwstring]);
        self.visible = YES;
        
        // Launch the timer to periodically display our stuff into the Cocoa view.
        self.renderTimer = [NSTimer timerWithTimeInterval:1.f/60.f
                                                   target:self
                                                 selector:@selector(renderMainWindow:)
                                                 userInfo:nil 
                                                  repeats:YES];
        [[NSRunLoop mainRunLoop] addTimer:self.renderTimer
                                  forMode:NSDefaultRunLoopMode];
        [[NSRunLoop mainRunLoop] addTimer:self.renderTimer
                                  forMode:NSEventTrackingRunLoopMode];
        /*
         * This is really some ugly code but in order to have the timer fired
         * periodically we need to add it to the two above runloop mode.
         *
         * The default mode allows timer firing while the user doesn't do anything
         * while the second mode allows timer firing while he is using a slider
         * or a menu.
         */
        
        self.initialized = YES;
    }
}

-(void)dealloc
{
    [self.renderTimer invalidate];
    self.mainWindow->renderWindow.Close();
    
    self.window             = nil;
    self.sfmlView           = nil;
    self.textField          = nil;
    
    delete self.mainWindow;
    self.mainWindow         = 0;
    self.renderTimer        = nil;
    
    [super dealloc];
}

-(void)renderMainWindow:(NSTimer *)aTimer
{
    // Scaling
    /* /!\ we do this at 60fps so choose low scaling factor! /!\ */
    if (sf::Keyboard::IsKeyPressed(sf::Keyboard::Up))
    {
        self.mainWindow->sprite.Scale(1.01, 1.01);
    }
    if (sf::Keyboard::IsKeyPressed(sf::Keyboard::Down))
    {
        self.mainWindow->sprite.Scale(0.99, 0.99);
    }
    
    // Clear the window, display some stuff and display it into our view.
    
    self.mainWindow->renderWindow.Clear(self.mainWindow->background);
    
    if (self.visible)
    {
        self.mainWindow->renderWindow.Draw(self.mainWindow->sprite);
    }
    
    self.mainWindow->renderWindow.Draw(self.mainWindow->text);
    
    self.mainWindow->renderWindow.Display();
}

-(IBAction)colorChanged:(NSPopUpButton *)sender
{
    if (self.initialized)
    {
        // Convert title to color
        NSString *color = [[sender selectedItem] title];
        if ([color isEqualToString:BLUE])
        {
            self.mainWindow->background = sf::Color::Blue;
        } 
        else if ([color isEqualToString:GREEN])
        {
            self.mainWindow->background = sf::Color::Green;
        } 
        else
        {
            self.mainWindow->background = sf::Color::Red;
        }
    }
}

-(IBAction)rotationChanged:(NSSlider *)sender
{
    if (self.initialized)
    {
        float angle = [sender floatValue];
        self.mainWindow->sprite.SetRotation(angle);
    }
}

-(IBAction)visibleChanged:(NSButton *)sender
{
    if (self.initialized)
        self.visible = [sender state] == NSOnState;
}

-(IBAction)textChanged:(NSTextField *)sender
{
    if (self.initialized)
        self.mainWindow->text.SetString([[sender stringValue] tostdwstring]);
}

- (IBAction)updateText:(NSButton *)sender
{
    // Simply simulate textChanged :
    [self textChanged:self.textField];
}

@end

@implementation SilentWindow

-(void)keyDown:(NSEvent *)theEvent
{
    // Do nothing except preventing this alert.
}

@end
