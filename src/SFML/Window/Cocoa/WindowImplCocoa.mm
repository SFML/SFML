////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007-2009 Lucas Soltic (ceylow@gmail.com) and Laurent Gomila (laurent.gom@gmail.com)
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
#import <SFML/Window/Cocoa/WindowImplCocoa.hpp>
#import <SFML/Window/Cocoa/AppController.h>
#import <SFML/Window/Cocoa/GLKit.h>
#import <SFML/Window/WindowStyle.hpp>
#import <SFML/System.hpp>
#import <iostream>


namespace sf
{
namespace priv
{

// Do something only once (useful in loops)
#define ONCE(make) \
{ static int __done = 0;\
if (!__done) {\
make;\
__done = 1;\
} }


////////////////////////////////////////////////////////////
/// Private function declarations
////////////////////////////////////////////////////////////
namespace {
	Key::Code		KeyForVirtualCode(unsigned short vCode);
	Key::Code		KeyForUnicode(unsigned short uniCode);
} // anonymous namespace


////////////////////////////////////////////////////////////
/// Default constructor
/// (creates a dummy window to provide a valid OpenGL context)
////////////////////////////////////////////////////////////
WindowImplCocoa::WindowImplCocoa() :
myWrapper(nil),
myUseKeyRepeat(false),
myMouseIn(false),
myWheelStatus(0.0f)
{
	[sfPrivAppController sharedController];
	
    // Create the shared OpenGL context
	if ([sfPrivGLContext sharedContext]) {
		// Then we make it the current active OpenGL context
		SetActive();
	} else {
		std::cerr << "Unable to make the main shared OpenGL context" << std::endl;
	}
}


////////////////////////////////////////////////////////////
/// Create the window implementation from an existing control
////////////////////////////////////////////////////////////
WindowImplCocoa::WindowImplCocoa(WindowHandle Handle, WindowSettings& params) :
myWrapper(NULL),
myUseKeyRepeat(false),
myMouseIn(false),
myWheelStatus(0.0f)
{
	if (Handle) {
		// Classical window import
		if ([(id)Handle isKindOfClass:[NSWindow class]]) {
			myWrapper = [[sfPrivImportedWindow alloc]
						 initWithWindow:(NSWindow *)Handle
						 settings:params];
 		}
		// Qt "window" import
		else if ([(id)Handle isKindOfClass:[NSView class]]) {
			myWrapper = [[sfPrivImportedView alloc]
						 initWithView:(NSView *)Handle
						 settings:params];
		} else {
			std::cerr 
			<< "Cannot import this Window Handle because it is neither"
			<< "a <NSWindow *> nor <NSView *> object"
			<< "(or any of its subclasses). You gave a <" 
			<< [[(id)Handle className] UTF8String]
			<< "> object."
			<< std::endl;
			
		}

		if (myWrapper) {
			[myWrapper setDelegate:this];
			
			// initial mouse state
			myMouseIn = [myWrapper mouseInside];
			
			// We set the myWidth and myHeight members to the correct values
			myWidth = (int) [[myWrapper view] frame].size.width;
			myHeight = (int) [[myWrapper view] frame].size.height;
		} else {
			std::cerr << "Failed to make the public window" << std::endl;
		}
	} else {
		std::cerr
		<< "Invalid null handle given to "
		<< "Window::Window(WindowHandle Handle, const WindowSettings& Params)"
		<< std::endl;
	}
}


////////////////////////////////////////////////////////////
/// Create the window implementation
////////////////////////////////////////////////////////////
WindowImplCocoa::WindowImplCocoa(VideoMode Mode, const std::string& Title, unsigned long WindowStyle, WindowSettings& params) :
myWrapper(NULL),
myUseKeyRepeat(false),
myMouseIn(false),
myWheelStatus(0.0f)
{
    // Create a new window with given size, title and style
	// First we define some objects used for our window
	NSString *title = [NSString stringWithCString:(Title.c_str()) ? (Title.c_str()) : ""
										 encoding:NSASCIIStringEncoding];
	
	// We create the window
	myWrapper = [[sfPrivOwnedWindow alloc]
				 initWithVideoMode:Mode
				 settings:params
				 style:WindowStyle
				 title:title];
	
	
	
	if (myWrapper)
	{
		[myWrapper setDelegate:this];
		
		// initial mouse state
		myMouseIn = [myWrapper mouseInside];
		
		// We set the myWidth and myHeight members to the correct values
		myWidth = Mode.Width;
		myHeight = Mode.Height;
	} else {
		std::cerr << "Failed to make the public window" << std::endl;
	}
}


////////////////////////////////////////////////////////////
/// Destructor
////////////////////////////////////////////////////////////
WindowImplCocoa::~WindowImplCocoa()
{
	// Release the window wrapper
	[myWrapper release];
}


////////////////////////////////////////////////////////////
/// Check if there's an active context on the current thread
////////////////////////////////////////////////////////////
bool WindowImplCocoa::IsContextActive()
{
	return ([NSOpenGLContext currentContext] != NULL);
}


////////////////////////////////////////////////////////////
/// Handle event sent by the default NSNotificationCenter
////////////////////////////////////////////////////////////
void WindowImplCocoa::HandleNotifiedEvent(Event& event)
{
	// Set myWidth and myHeight to correct value if
	// window size changed
	switch (event.Type) {
		case Event::Resized:
			myWidth = event.Size.Width;
			myHeight = event.Size.Height;
			break;
			
		default:
			break;
	}
	
	// And send the event
	SendEvent(event);
}


////////////////////////////////////////////////////////////
/// Handle a key down event (NSEvent)
////////////////////////////////////////////////////////////
void WindowImplCocoa::HandleKeyDown(void *eventRef)
{
	NSEvent *event = reinterpret_cast <NSEvent *> (eventRef);
	
	Event sfEvent;
	unichar chr = 0, rawchr = 0;
	unsigned long length = [[event characters] length];
	unsigned mods = [event modifierFlags];
	
	if (length) {
		chr = [[event characters] characterAtIndex:0];
		
		// Note : I got a crash (out of bounds exception) while typing so now I test...
		if ([[event charactersIgnoringModifiers] length])
			rawchr = [[event charactersIgnoringModifiers] characterAtIndex:0];
		
		
		// Don't handle repeated events if we chose not to send them
		if (!myUseKeyRepeat && [event isARepeat])
			return;
		
		// Is it also a text event ?
		if (IsTextEvent(event)) {
			// buffer for the UTF-32 characters
			Uint32 utf32Characters[2] = {0};
			
			// convert the characters
			// note: using CFString in order to keep compatibility with Mac OS X 10.4
			// (as NSUTF32StringEncoding is only being defined in Mac OS X 10.5 and later)
			if (!CFStringGetCString ((CFStringRef)[event characters],
									 (char *)utf32Characters,
									 sizeof(utf32Characters),
									 kCFStringEncodingUTF32))
			{
				char asciiChar[3] = {0};
				if ([[event characters] lengthOfBytesUsingEncoding:NSASCIIStringEncoding])
					[[event characters] getCString:asciiChar
										 maxLength:3
										  encoding:NSASCIIStringEncoding];
				
				std::cerr << "Error while converting character to UTF32 : \""
				<< asciiChar << "\"" << std::endl;
			}
			else
			{
				sfEvent.Type = Event::TextEntered;
				sfEvent.Text.Unicode = utf32Characters[0];
				
				SendEvent(sfEvent);
			}
		}
		
		// Anyway it's also a KeyPressed event
		sfEvent.Type = Event::KeyPressed;
		
		// Get the keys
		if (Key::Code(0) == (sfEvent.Key.Code = KeyForUnicode(rawchr))) {
			sfEvent.Key.Code = KeyForVirtualCode([event keyCode]);
		}
		
		// Get the modifiers
		sfEvent.Key.Alt = mods & NSAlternateKeyMask;
		sfEvent.Key.Control = mods & NSControlKeyMask;
		sfEvent.Key.Shift = mods & NSShiftKeyMask;
		
		// Send the event
		SendEvent(sfEvent);
	}
}


////////////////////////////////////////////////////////////
/// Handle a key up event (NSEvent)
////////////////////////////////////////////////////////////
void WindowImplCocoa::HandleKeyUp(void *eventRef)
{
	NSEvent *event = reinterpret_cast <NSEvent *> (eventRef);
	
	Event sfEvent;
	unsigned mods = [event modifierFlags];
	unichar chr = 0, rawchr = 0;
	
	if ([[event characters] length]) {
		chr = [[event characters] characterAtIndex:0];
		
		if ([[event charactersIgnoringModifiers] length])
			rawchr = [[event charactersIgnoringModifiers] characterAtIndex:0];
		
		sfEvent.Type = Event::KeyReleased;
		
		// Get the code
		if (Key::Code(0) == (sfEvent.Key.Code = KeyForUnicode(rawchr))) {
			sfEvent.Key.Code = KeyForVirtualCode([event keyCode]);
		}
		
		// Get the modifiers
		sfEvent.Key.Alt = mods & NSAlternateKeyMask;
		sfEvent.Key.Control = mods & NSControlKeyMask;
		sfEvent.Key.Shift = mods & NSShiftKeyMask;
		
		// Send the event
		SendEvent(sfEvent);
	}
}


////////////////////////////////////////////////////////////
/// Handle a key modifier event [Command, Option, Control, Shift]
////////////////////////////////////////////////////////////
void WindowImplCocoa::HandleModifierKey(void *eventRef)
{
	NSEvent *event = reinterpret_cast <NSEvent *> (eventRef);
	Event sfEvent;
	unsigned mods = [event modifierFlags];
	
	sfEvent.Type = Event::KeyPressed;
	
	// Get the code
	sfEvent.Key.Code = KeyForVirtualCode([event keyCode]);
	
	// Get the modifiers
	sfEvent.Key.Alt = mods & NSAlternateKeyMask;
	sfEvent.Key.Control = mods & NSControlKeyMask;
	sfEvent.Key.Shift = mods & NSShiftKeyMask;
	
	// Guess whether it's a pressed or released event
	// Note: this does not work fine is both left and right modifiers are pressed
	// I did not find any way to fix this.
	// TODO: fix handling of modifier flags for use of left and right key at the same time
	if (!(mods & NSAlternateKeyMask) &&
		(sfEvent.Key.Code == Key::LAlt || sfEvent.Key.Code == Key::RAlt)) {
		sfEvent.Type = Event::KeyReleased;
	}
	
	if (!(mods & NSControlKeyMask) &&
		(sfEvent.Key.Code == Key::LControl || sfEvent.Key.Code == Key::RControl)) {
		sfEvent.Type = Event::KeyReleased;
	}
	
	if (!(mods & NSShiftKeyMask) &&
		(sfEvent.Key.Code == Key::LShift || sfEvent.Key.Code == Key::RShift)) {
		sfEvent.Type = Event::KeyReleased;
	}
	
	if (!(mods & NSCommandKeyMask) &&
		(sfEvent.Key.Code == Key::LSystem || sfEvent.Key.Code == Key::RSystem)) {
		sfEvent.Type = Event::KeyReleased;
	}
	
	// Send the event
	SendEvent(sfEvent);
}


////////////////////////////////////////////////////////////
/// Handle a mouse down event (NSEvent)
////////////////////////////////////////////////////////////
void WindowImplCocoa::HandleMouseDown(void *eventRef)
{
	NSEvent *event = reinterpret_cast <NSEvent *> (eventRef);
	Event sfEvent;
	
	// Get mouse position relative to the window
	NSPoint loc = [myWrapper mouseLocation];
	unsigned mods = [event modifierFlags];
	
	switch ([event type]) {
		case NSLeftMouseDown:
			sfEvent.Type = Event::MouseButtonPressed;
			
			// Guess whether it's a mouse left or mouse right event
			if (mods & NSControlKeyMask) {
				sfEvent.MouseButton.Button = Mouse::Right;
			} else {
				sfEvent.MouseButton.Button = Mouse::Left;
			}
			
			sfEvent.MouseButton.X = (int) loc.x;
			sfEvent.MouseButton.Y = (int) loc.y;
			
			// Send the event
			SendEvent(sfEvent);
			break;
			
		case NSRightMouseDown:
			sfEvent.Type = Event::MouseButtonPressed;
			sfEvent.MouseButton.Button = Mouse::Right;
			
			sfEvent.MouseButton.X = (int) loc.x;
			sfEvent.MouseButton.Y = (int) loc.y;
			
			// Send the event
			SendEvent(sfEvent);
			break;
			
		default:
			break;
	}
}


////////////////////////////////////////////////////////////
/// Handle a mouse up event (NSEvent)
////////////////////////////////////////////////////////////
void WindowImplCocoa::HandleMouseUp(void *eventRef)
{
	NSEvent *event = reinterpret_cast <NSEvent *> (eventRef);
	Event sfEvent;
	
	// Get mouse position relative to the window
	NSPoint loc = [myWrapper mouseLocation];
	unsigned mods = [event modifierFlags];
	
	switch ([event type]) {
		case NSLeftMouseUp:
			sfEvent.Type = Event::MouseButtonReleased;
			
			// Guess whether it's a mouse left or mouse right event
			if (mods & NSControlKeyMask) {
				sfEvent.MouseButton.Button = Mouse::Right;
			} else {
				sfEvent.MouseButton.Button = Mouse::Left;
			}
			
			sfEvent.MouseButton.X = (int) loc.x;
			sfEvent.MouseButton.Y = (int) loc.y;
			
			// Send the event
			SendEvent(sfEvent);
			break;
			
		case NSRightMouseUp:
			sfEvent.Type = Event::MouseButtonReleased;
			sfEvent.MouseButton.Button = Mouse::Right;
			
			sfEvent.MouseButton.X = (int) loc.x;
			sfEvent.MouseButton.Y = (int) loc.y;
			
			// Send the event
			SendEvent(sfEvent);
			break;
			
		default:
			break;
	}
}


////////////////////////////////////////////////////////////
/// Handle a mouse move event (NSEvent)
////////////////////////////////////////////////////////////
void WindowImplCocoa::HandleMouseMove(void *eventRef)
{
	Event sfEvent;
	NSPoint loc = [myWrapper mouseLocation];
	sfEvent.Type = Event::MouseMoved;
	
	sfEvent.MouseMove.X = (int) loc.x;
	sfEvent.MouseMove.Y = (int) loc.y;
	
	SendEvent(sfEvent);
	
	if ([myWrapper mouseInside] && !myMouseIn) {
		// If mouse IS inside but WAS not inside last time
		sfEvent.Type = Event::MouseEntered;
		myMouseIn = true;
		
		SendEvent(sfEvent);
	} else if (![myWrapper mouseInside] && myMouseIn) {
		// Is mouse WAS not inside but IS now inside
		sfEvent.Type = Event::MouseLeft;
		myMouseIn	= false;
		
		SendEvent(sfEvent);
	}
}


////////////////////////////////////////////////////////////
/// Handle a mouse wheel event (NSEvent)
////////////////////////////////////////////////////////////
void WindowImplCocoa::HandleMouseWheel(void *eventRef)
{
	NSEvent *event = reinterpret_cast <NSEvent *> (eventRef);
	
	// SFML uses integer values for delta but Cocoa uses float and it is mostly fewer than 1.0
	// Therefore I chose to add the float value to a 'wheel status' and
	// send a sf event only when it's greater than 1.0
	myWheelStatus += [event deltaY];
	
	if (fabs(myWheelStatus) > 1.0f) {
		// Make the event and send it
		Event sfEvent;
		sfEvent.Type = Event::MouseWheelMoved;
		sfEvent.MouseWheel.Delta = (int) myWheelStatus;
		SendEvent(sfEvent);
		
		// Remove as much integer units as the one that have been put in the event
		// (was a mistake to set this to 0)
		myWheelStatus -= (int) myWheelStatus;
	}
}
	
////////////////////////////////////////////////////////////
/// Return whether 'ev' must be considered as a TextEntered event
////////////////////////////////////////////////////////////
bool WindowImplCocoa::IsTextEvent(void *eventRef)
{
	NSEvent *event = (NSEvent *)eventRef;
	bool res = false;
	
	if (event && [event type] == NSKeyDown && [[event characters] length]) {
		unichar code = [[event characters] characterAtIndex:0];
		
		// Codes from 0xF700 to 0xF8FF are non text keys (see NSEvent.h)
		// 0x35 is the Escape key
		if ([event keyCode] != 0x35 && (code < 0xF700 || code > 0xF8FF))
			res = true;
	}
	
	return res;
}
	
////////////////////////////////////////////////////////////
/// /see sfWindowImpl::Display
////////////////////////////////////////////////////////////
void WindowImplCocoa::Display()
{
	// Forward flush call to the window
	[myWrapper flushBuffer];
}


////////////////////////////////////////////////////////////
/// /see sfWindowImpl::ProcessEvents
////////////////////////////////////////////////////////////
void WindowImplCocoa::ProcessEvents()
{
	// Forward event handling call to the application controller
	[[sfPrivAppController sharedController] processEvents];
}


////////////////////////////////////////////////////////////
/// /see sfWindowImpl::MakeActive
////////////////////////////////////////////////////////////
void WindowImplCocoa::SetActive(bool Active) const
{
	// Forward the call to the window
	if (myWrapper)
		[myWrapper setActive:Active];
	else {
		// Or directly activate the shared OpenGL context if we're not using a window
		if (Active) {
			if ([NSOpenGLContext currentContext] != [sfPrivGLContext sharedContext])
				[[sfPrivGLContext sharedContext] makeCurrentContext];
		} else {
			if ([NSOpenGLContext currentContext] == [sfPrivGLContext sharedContext])
				[NSOpenGLContext clearCurrentContext];
		}
	}
}


////////////////////////////////////////////////////////////
/// /see sfWindowImpl::UseVerticalSync
////////////////////////////////////////////////////////////
void WindowImplCocoa::UseVerticalSync(bool Enabled)
{
	// Forward the call to the window
	[myWrapper enableVerticalSync:Enabled];
}


////////////////////////////////////////////////////////////
/// /see sfWindowImpl::ShowMouseCursor
////////////////////////////////////////////////////////////
void WindowImplCocoa::ShowMouseCursor(bool flag)
{
	if (flag) {
		[NSCursor unhide];
	} else {
		[NSCursor hide];
	}
}


////////////////////////////////////////////////////////////
/// /see sfWindowImpl::SetCursorPosition
////////////////////////////////////////////////////////////
void WindowImplCocoa::SetCursorPosition(unsigned int Left, unsigned int Top)
{
	NSPoint pos = NSMakePoint ((float) Left, (float) Top);
	
	if (myWrapper) {
		// Flip for SFML window coordinate system
		pos.y = [[myWrapper window] frame].size.height - pos.y;
		
		// Adjust for view reference instead of window
		pos.y -= [[myWrapper window] frame].size.height - [[myWrapper view] frame].size.height;
		
		// Convert to screen coordinates
		NSPoint absolute = [[myWrapper window] convertBaseToScreen:pos];
		
		// Flip screen coodinates
		absolute.y = [[NSScreen mainScreen] frame].size.height - absolute.y;
		
		// Move cursor
		CGDisplayMoveCursorToPoint([sfPrivAppController primaryScreen],
								   CGPointMake(absolute.x, absolute.y));
	}
}


////////////////////////////////////////////////////////////
/// /see sfWindowImpl::SetPosition
////////////////////////////////////////////////////////////
void WindowImplCocoa::SetPosition(int Left, int Top)
{
	[myWrapper setPosition:NSMakePoint(Left, Top)];
}


////////////////////////////////////////////////////////////
/// /see WindowImpl::SetSize
///
////////////////////////////////////////////////////////////
void WindowImplCocoa::SetSize(unsigned int Width, unsigned int Height)
{
	[myWrapper setSize:NSMakeSize(Width, Height)];
}


////////////////////////////////////////////////////////////
/// /see sfWindowImpl::Show
////////////////////////////////////////////////////////////
void WindowImplCocoa::Show(bool State)
{
	[myWrapper show:State];
}


////////////////////////////////////////////////////////////
/// /see sfWindowImpl::EnableKeyRepeat
////////////////////////////////////////////////////////////
void WindowImplCocoa::EnableKeyRepeat(bool Enabled)
{
	myUseKeyRepeat = Enabled;
}


////////////////////////////////////////////////////////////
/// see WindowImpl::SetIcon
////////////////////////////////////////////////////////////
void WindowImplCocoa::SetIcon(unsigned int Width, unsigned int Height, const Uint8* Pixels)
{
	// Nothing to do
}


namespace {
	////////////////////////////////////////////////////////////
	/// Return the SFML key corresponding to a key code
	////////////////////////////////////////////////////////////
	Key::Code KeyForVirtualCode(unsigned short vCode)
	{
		static struct {
			unsigned short code;
			Key::Code sfKey;
		} virtualTable[] =
		{
			{0x35, Key::Escape},
			{0x31, Key::Space},
			{0x24, Key::Return},	// main Return key
			{0x4C, Key::Return},	// pav Return key
			{0x33, Key::Back},
			{0x30, Key::Tab},
			{0x74, Key::PageUp},
			{0x79, Key::PageDown},
			{0x77, Key::End},
			{0x73, Key::Home},
			{0x72, Key::Insert},
			{0x75, Key::Delete},
			{0x45, Key::Add},
			{0x4E, Key::Subtract},
			{0x43, Key::Multiply},
			{0x4B, Key::Divide},
			
			{0x7A, Key::F1}, {0x78, Key::F2}, {0x63, Key::F3},
			{0x76, Key::F4}, {0x60, Key::F5}, {0x61, Key::F6},
			{0x62, Key::F7}, {0x64, Key::F8}, {0x65, Key::F9},
			{0x6D, Key::F10}, {0x67, Key::F11}, {0x6F, Key::F12},
			{0x69, Key::F13}, {0x6B, Key::F14}, {0x71, Key::F15},
			
			{0x7B, Key::Left},
			{0x7C, Key::Right},
			{0x7E, Key::Up},
			{0x7D, Key::Down},
			
			{0x52, Key::Numpad0}, {0x53, Key::Numpad1}, {0x54, Key::Numpad2},
			{0x55, Key::Numpad3}, {0x56, Key::Numpad4}, {0x57, Key::Numpad5},
			{0x58, Key::Numpad6}, {0x59, Key::Numpad7}, {0x5B, Key::Numpad8},
			{0x5C, Key::Numpad9},
			
			{0x1D, Key::Num0}, {0x12, Key::Num1}, {0x13, Key::Num2},
			{0x14, Key::Num3}, {0x15, Key::Num4}, {0x17, Key::Num5},
			{0x16, Key::Num6}, {0x1A, Key::Num7}, {0x1C, Key::Num8},
			{0x19, Key::Num9},
			
			{0x3B, Key::LControl},	//< Left Ctrl
			{0x3A, Key::LAlt},		//< Left Option/Alt
			{0x37, Key::LSystem},	//< Left Command
			{0x38, Key::LShift},	//< Left Shift
			{0x3E, Key::RControl},	//< Right Ctrl
			{0x3D, Key::RAlt},		//< Right Option/Alt
			{0x36, Key::RSystem},	//< Right Command
			{0x3C, Key::RShift},	//< Right Shift
			
			{0x39, Key::Code(0)}	//< Caps Lock (not handled by SFML for now)
		};
		
		Key::Code result = Key::Code(0);
		
		for (unsigned i = 0;virtualTable[i].code;i++) {
			if (virtualTable[i].code == vCode) {
				result = virtualTable[i].sfKey;
				break;
			}
		}
		
		return result;
	}
	
	
	////////////////////////////////////////////////////////////
	/// Return the SFML key corresponding to a unicode code
	////////////////////////////////////////////////////////////
	Key::Code KeyForUnicode(unsigned short uniCode)
	{
		// TODO: find a better way to get the language independant key
		static struct {
			unsigned short character;
			Key::Code sfKey;
		} unicodeTable[] =
		{
			{'!', Key::Code(0)}, //< No Key for this code
			{'"', Key::Code(0)}, //< No Key for this code
			{'#', Key::Code(0)}, //< No Key for this code
			{'$', Key::Code(0)}, //< No Key for this code
			{'%', Key::Code(0)}, //< No Key for this code
			{'&', Key::Code(0)}, //< No Key for this code
			{'\'', Key::Quote},
			{'(', Key::Code(0)}, //< No Key for this code
			{')', Key::Code(0)}, //< No Key for this code
			{'*', Key::Multiply},
			{'+', Key::Add},
			{',', Key::Comma},
			{'-', Key::Code(0)}, //< Handled by KeyForVirtualCode()
			{'.', Key::Period},
			{'/', Key::Code(0)}, //< Handled by KeyForVirtualCode()
			{'0', Key::Code(0)}, //< Handled by KeyForVirtualCode()
			{'1', Key::Code(0)}, //< Handled by KeyForVirtualCode()
			{'2', Key::Code(0)}, //< Handled by KeyForVirtualCode()
			{'3', Key::Code(0)}, //< Handled by KeyForVirtualCode()
			{'4', Key::Code(0)}, //< Handled by KeyForVirtualCode()
			{'5', Key::Code(0)}, //< Handled by KeyForVirtualCode()
			{'6', Key::Code(0)}, //< Handled by KeyForVirtualCode()
			{'7', Key::Code(0)}, //< Handled by KeyForVirtualCode()
			{'8', Key::Code(0)}, //< Handled by KeyForVirtualCode()
			{'9', Key::Code(0)}, //< Handled by KeyForVirtualCode()
			{':', Key::Code(0)}, //< No Key for this code
			{';', Key::SemiColon},
			{'<', Key::Code(0)}, //< No Key for this code
			{'=', Key::Equal},
			{'>', Key::Code(0)}, //< No Key for this code
			{'?', Key::Code(0)}, //< No Key for this code
			{'@', Key::Code(0)}, //< No Key for this code
			{'A', Key::A}, {'B', Key::B}, {'C', Key::C},
			{'D', Key::D}, {'E', Key::E}, {'F', Key::F},
			{'G', Key::G}, {'H', Key::H}, {'I', Key::I},
			{'J', Key::J}, {'K', Key::K}, {'L', Key::L},
			{'M', Key::M}, {'N', Key::N}, {'O', Key::O},
			{'P', Key::P}, {'Q', Key::Q}, {'R', Key::R},
			{'S', Key::S}, {'T', Key::T}, {'U', Key::U},
			{'V', Key::V}, {'W', Key::W}, {'X', Key::X},
			{'Y', Key::Y}, {'Z', Key::Z},
			{'[', Key::LBracket},
			{'\\', Key::BackSlash},
			{']', Key::RBracket},
			{'^', Key::Code(0)}, //< No Key for this code
			{'_', Key::Code(0)}, //< No Key for this code
			{'`', Key::Code(0)}, //< No Key for this code
			{'a', Key::A}, {'b', Key::B}, {'c', Key::C},
			{'d', Key::D}, {'e', Key::E}, {'f', Key::F},
			{'g', Key::G}, {'h', Key::H}, {'i', Key::I},
			{'j', Key::J}, {'k', Key::K}, {'l', Key::L},
			{'m', Key::M}, {'n', Key::N}, {'o', Key::O},
			{'p', Key::P}, {'q', Key::Q}, {'r', Key::R},
			{'s', Key::S}, {'t', Key::T}, {'u', Key::U},
			{'v', Key::V}, {'w', Key::W}, {'x', Key::X},
			{'y', Key::Y}, {'z', Key::Z},
			{'{', Key::Code(0)}, //< No Key for this code
			{'|', Key::Code(0)}, //< No Key for this code
			{'}', Key::Code(0)}, //< No Key for this code
			{'~', Key::Tilde},
			{0, Key::Code(0)}
		};
		
		Key::Code result = Key::Code(0);
		
		for (unsigned i = 0;unicodeTable[i].character;i++) {
			if (unicodeTable[i].character == uniCode) {
				result = unicodeTable[i].sfKey;
				break;
			}
		}
		
		return result;
	}
	
} // anonymous namespace


} // namespace priv

} // namespace sf

