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
#include <SFML/Window/iOS/SFView.hpp>
#include <SFML/Window/iOS/SFAppDelegate.hpp>
#include <SFML/System/Utf.hpp>
#include <QuartzCore/CAEAGLLayer.h>
#include <cstring>


@implementation SFView


////////////////////////////////////////////////////////////
-(BOOL)canBecomeFirstResponder
{
    return true;
}


////////////////////////////////////////////////////////////
- (BOOL)hasText
{
    return true;
}


////////////////////////////////////////////////////////////
- (void)deleteBackward
{
    [[SFAppDelegate getInstance] notifyCharacter:'\b'];
}


////////////////////////////////////////////////////////////
- (void)insertText:(NSString*)text
{
    // Convert the NSString to UTF-8
    const char* utf8 = [text UTF8String];

    // Then convert to UTF-32 and notify the application delegate of each new character
    const char* end = utf8 + std::strlen(utf8);
    while (utf8 < end)
    {
        sf::Uint32 character;
        utf8 = sf::Utf8::decode(utf8, end, character);
        [[SFAppDelegate getInstance] notifyCharacter:character];
    }
}


////////////////////////////////////////////////////////////
- (void)touchesBegan:(NSSet*)touches withEvent:(UIEvent*)event
{
    UITouch* touch = [touches anyObject];
    CGPoint position = [touch locationInView:self];

    [[SFAppDelegate getInstance] notifyTouchBeginAt:position];
}


////////////////////////////////////////////////////////////
- (void)touchesMoved:(NSSet*)touches withEvent:(UIEvent*)event
{
    UITouch* touch = [touches anyObject];
    CGPoint position = [touch locationInView:self];

    [[SFAppDelegate getInstance] notifyTouchMoveAt:position];
}


////////////////////////////////////////////////////////////
- (void)touchesEnded:(NSSet*)touches withEvent:(UIEvent*)event
{
    UITouch* touch = [touches anyObject];
    CGPoint position = [touch locationInView:self];

    [[SFAppDelegate getInstance] notifyTouchEndAt:position];
}


////////////////////////////////////////////////////////////
- (void)touchesCancelled:(NSSet*)touches withEvent:(UIEvent*)event
{
    // Treat touch cancel events the same way as touch end
    [self touchesEnded:touches withEvent:event];
}


////////////////////////////////////////////////////////////
+(Class)layerClass
{
	return [CAEAGLLayer class];
}


////////////////////////////////////////////////////////////
-(id)initWithFrame:(CGRect)frame
{
	self = [super initWithFrame:frame];
	if (self)
    {
		if (![self initialize])
        {
            [self release];
            self = nil;
        }
    }

	return self;
}


////////////////////////////////////////////////////////////
-(bool)initialize
{
    // Configure the EAGL layer
	CAEAGLLayer* eaglLayer = (CAEAGLLayer*)self.layer;
	eaglLayer.opaque = YES;
	eaglLayer.drawableProperties = [NSDictionary dictionaryWithObjectsAndKeys:
                                    [NSNumber numberWithBool:FALSE], kEAGLDrawablePropertyRetainedBacking,
                                    kEAGLColorFormatRGBA8, kEAGLDrawablePropertyColorFormat,
                                    nil];

    // Enable user interactions on the view (touch events)
    self.userInteractionEnabled = true;

	return true;
}


@end
