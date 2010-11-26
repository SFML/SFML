////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007-2010 Marco Antognini (antognini.marco@gmail.com), 
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

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Window/OSX/WindowImplCocoa.hpp>
#include <SFML/System/Err.hpp>

#import <SFML/Window/OSX/SFWindowController.h>
//#import <SFML/Window/OSX/SFViewController.h>
#warning SFViewController not yet implemented.
#import <SFML/Window/OSX/cpp_objc_conversion.h>

namespace sf
{
namespace priv
{

#pragma mark
#pragma mark WindowImplCocoa's ctor/dtor

////////////////////////////////////////////////////////////
WindowImplCocoa::WindowImplCocoa(WindowHandle handle)
{
	sf::Err() << "Not yet fully supported." << std::endl;
#warning WindowImplCocoa(WindowHandle handle) not yet fully implemented
	
	SetUpPoolAndApplication();
	
	// Treat the handle as it real type
	id nsHandle = (id)handle;
	if ([nsHandle isKindOfClass:[NSWindow class]]) {
		
		// We have a window.
		myDelegate = [[SFWindowController alloc] initWithWindow:nsHandle];
	
	} /*else if ([nsHandle isKindOfClass:[NSView class]]) {
		
		// We have a view.
		myDelegate = [[SFViewController alloc] initWithView:nsHandle];
		
	} */ else {
		
		sf::Err()
			<< "Cannot import this Window Handle because it is neither "
			<< "a <NSWindow*> nor <NSView*> object "
			<< "(or any of their subclasses). You gave a <" 
			<< [[nsHandle className] UTF8String]
			<< "> object."
			<< std::endl;
		return;			
		
	}
	 
	// NO :
	// [myDelegate setRequesterTo:this];
	// because we don't handle event.
}
	
	
////////////////////////////////////////////////////////////
WindowImplCocoa::WindowImplCocoa(VideoMode mode, 
																 const std::string& title, 
																 unsigned long style)
{
	SetUpPoolAndApplication();
	
	// Don't forget to update our parent (that is, WindowImpl) size :
	myWidth = mode.Width;
	myHeight = mode.Height;
	
	myDelegate = [[SFWindowController alloc] initWithMode:&mode andStyle:style];
	[myDelegate changeTitle:stringToNSString(title)];
	[myDelegate setRequesterTo:this];
}

	
////////////////////////////////////////////////////////////
WindowImplCocoa::~WindowImplCocoa()
{
	[myDelegate closeWindow];
	
	[myDelegate release];
	[myPool release];
}
	
	
////////////////////////////////////////////////////////////	
void WindowImplCocoa::ApplyContext(NSOpenGLContextRef context) const
{
	[myDelegate applyContext:context];
}
	
	
#pragma mark
#pragma mark WindowImplCocoa's window-event methods
	
	
////////////////////////////////////////////////////////////
void WindowImplCocoa::WindowClosed(void)
{			
	Event event;
	event.Type = Event::Closed;
	
	PushEvent(event);
}
	
	
////////////////////////////////////////////////////////////
void WindowImplCocoa::WindowResized(unsigned int width, unsigned int height)
{
	// Don't forget to update our parent (that is, WindowImpl) size :
	myWidth  = width;
	myHeight = height;
	
	Event event;
	event.Type = Event::Resized;
	event.Size.Width  = myWidth;
	event.Size.Height = myHeight;
	
	PushEvent(event);
}

	
////////////////////////////////////////////////////////////
void WindowImplCocoa::WindowLostFocus(void)
{
	Event event;
	event.Type = Event::LostFocus;
	
	PushEvent(event);
}

	
////////////////////////////////////////////////////////////
void WindowImplCocoa::WindowGainedFocus(void)
{
	Event event;
	event.Type = Event::GainedFocus;
	
	PushEvent(event);
}
	
#pragma mark
#pragma mark WindowImplCocoa's mouse-event methods
	
	
////////////////////////////////////////////////////////////
void WindowImplCocoa::MouseDownAt(Mouse::Button button, int x, int y)
{
	Event event;
	event.Type = Event::MouseButtonPressed;
	event.MouseButton.Button = button;
	event.MouseButton.X = x;
	event.MouseButton.Y = y;
	
	PushEvent(event);
}

	
////////////////////////////////////////////////////////////
void WindowImplCocoa::MouseUpAt(Mouse::Button button, int x, int y)
{
	Event event;
	event.Type = Event::MouseButtonReleased;
	event.MouseButton.Button = button;
	event.MouseButton.X = x;
	event.MouseButton.Y = y;
	
	PushEvent(event);
}


////////////////////////////////////////////////////////////
void WindowImplCocoa::MouseMovedAt(int x, int y)
{
	Event event;
	event.Type = Event::MouseMoved;
	event.MouseMove.X = x;
	event.MouseMove.Y = y;
	
	PushEvent(event);
}

////////////////////////////////////////////////////////////
void WindowImplCocoa::MouseWheelScrolledAt(float delta, int x, int y)
{
	Event event;
	event.Type = Event::MouseWheelMoved;
	event.MouseWheel.Delta = delta;
	event.MouseWheel.X = x;
	event.MouseWheel.Y = y;
	
	PushEvent(event);
}

////////////////////////////////////////////////////////////
void WindowImplCocoa::MouseMovedIn(void)
{
	Event event;
	event.Type = Event::MouseEntered;
	
	PushEvent(event);
}
	
////////////////////////////////////////////////////////////
void WindowImplCocoa::MouseMovedOut(void)
{
	Event event;
	event.Type = Event::MouseLeft;
	
	PushEvent(event);
}
	
	
#pragma mark
#pragma mark WindowImplCocoa's key-event methods
	
	
////////////////////////////////////////////////////////////
void WindowImplCocoa::KeyDown(unsigned short keycode, unsigned int modifierFlags)
{
	Event event;
	event.Type = Event::KeyPressed;
	
	event.Key.Code = NSKeyCodeToSFMLKeyCode(keycode);
	if (event.Key.Code == Key::Count) {
		sf::Err() << "Unknown key (pressed)" << std::endl;
		return; // Not a valid/supported key.
	}
	
	event.Key.Alt = modifierFlags & NSAlternateKeyMask;
	event.Key.Control = modifierFlags & NSControlKeyMask;
	event.Key.Shift = modifierFlags & NSShiftKeyMask;
	
	PushEvent(event);
}

	
////////////////////////////////////////////////////////////
void WindowImplCocoa::KeyUp(unsigned short keycode, unsigned int modifierFlags)
{
	Event event;
	event.Type = Event::KeyReleased;
	
	event.Key.Code = NSKeyCodeToSFMLKeyCode(keycode);
	if (event.Key.Code == Key::Count) {
		sf::Err() << "Unknown key (released)" << std::endl;
		return; // Not a valid/supported key.
	}
	
	event.Key.Alt = modifierFlags & NSAlternateKeyMask;
	event.Key.Control = modifierFlags & NSControlKeyMask;
	event.Key.Shift = modifierFlags & NSShiftKeyMask;
	
	PushEvent(event);
}


////////////////////////////////////////////////////////////
void WindowImplCocoa::TextEntred(Uint32 charcode)
{
	Event event;
	event.Type = Event::TextEntered;
	event.Text.Unicode = charcode;
	
	PushEvent(event);
}
	
	
////////////////////////////////////////////////////////////
Key::Code WindowImplCocoa::NSKeyCodeToSFMLKeyCode(unsigned short keycode) {
	/* Based on http://forums.macrumors.com/showthread.php?t=780577 */
	switch (keycode) {
		case 0x00: 			return Key::A;
		case 0x0b:			return Key::B;
		case 0x08:			return Key::C;
		case 0x02:			return Key::D;
		case 0x0e:			return Key::E;
		case 0x03:			return Key::F;
		case 0x05:			return Key::G;
		case 0x04:			return Key::H;
		case 0x22:			return Key::I;
		case 0x26:			return Key::J;
		case 0x28:			return Key::K;
		case 0x25:			return Key::L;
		case 0x2e:			return Key::M;
		case 0x2d:			return Key::N;
		case 0x1f:			return Key::O;
		case 0x23:			return Key::P;
		case 0x0c:			return Key::Q;
		case 0x0f:			return Key::R;
		case 0x01:			return Key::S;
		case 0x11:			return Key::T;
		case 0x20:			return Key::U;
		case 0x09:			return Key::V;
		case 0x0d:			return Key::W;
		case 0x07:			return Key::X;
		case 0x10:			return Key::Y;
		case 0x06:			return Key::Z;
		case 0x1d:			return Key::Num0;
		case 0x12:			return Key::Num1;
		case 0x13:			return Key::Num2;
		case 0x14:			return Key::Num3;
		case 0x15:			return Key::Num4;
		case 0x17:			return Key::Num5;
		case 0x16:			return Key::Num6;
		case 0x1a:			return Key::Num7;
		case 0x1c:			return Key::Num8;
		case 0x19:			return Key::Num9;
		case 0x35:			return Key::Escape;
		case 0x3b:			return Key::LControl;
		case 0x38:			return Key::LShift;
		case 0x3a:			return Key::LAlt;
		case 0x37:			return Key::LSystem;
		case 0x3e:			return Key::RControl;
		case 0x3c:			return Key::RShift;
		case 0x3d:			return Key::RAlt;
		case 0x36:			return Key::RSystem;
#warning unknown key code for Menu
			//		case 0x:			return Key::Menu;
		case 0x21:			return Key::LBracket;
		case 0x1e:			return Key::RBracket;
		case 0x29:			return Key::SemiColon;
		case 0x2b:			return Key::Comma;
		case 0x2f:			return Key::Period;
		case 0x27:			return Key::Quote;
		case 0x2c:			return Key::Slash;
		case 0x2a:			return Key::BackSlash;
#warning 0x0a is for "Non-US Backslash" (from HID Calibrator, a sample provided by Apple).
		case 0x0a:			return Key::Tilde;
		case 0x18:			return Key::Equal;
		case 0x32:			return Key::Dash;
		case 0x31:			return Key::Space;
		case 0x24:			return Key::Return;
#warning unknown key code for Back
			//		case 0x:			return Key::Back;
		case 0x30:			return Key::Tab;
		case 0x74:			return Key::PageUp;
		case 0x79:			return Key::PageDown;
		case 0x77:			return Key::End;
		case 0x73:			return Key::Home;
#warning unknown key code for Insert
			//		case 0x:			return Key::Insert;
		case 0x33:			return Key::Delete;
		case 0x45:			return Key::Add;
		case 0x4e:			return Key::Subtract;
		case 0x43:			return Key::Multiply;
		case 0x4b:			return Key::Divide;
		case 0x7b:			return Key::Left;
		case 0x7c:			return Key::Right;
		case 0x7e:			return Key::Up;
		case 0x7d:			return Key::Down;
		case 0x52:			return Key::Numpad0;
		case 0x53:			return Key::Numpad1;
		case 0x54:			return Key::Numpad2;
		case 0x55:			return Key::Numpad3;
		case 0x56:			return Key::Numpad4;
		case 0x57:			return Key::Numpad5;
		case 0x58:			return Key::Numpad6;
		case 0x59:			return Key::Numpad7;
		case 0x5b:			return Key::Numpad8;
		case 0x5c:			return Key::Numpad9;
		case 0x7a:			return Key::F1;
		case 0x78:			return Key::F2;
		case 0x63:			return Key::F3;
		case 0x76:			return Key::F4;
		case 0x60:			return Key::F5;
		case 0x61:			return Key::F6;
		case 0x62:			return Key::F7;
		case 0x64:			return Key::F8;
		case 0x65:			return Key::F9;
		case 0x6d:			return Key::F10;
		case 0x67:			return Key::F11;
		case 0x6f:			return Key::F12;
		case 0x69:			return Key::F13;
		case 0x6b:			return Key::F14;
		case 0x71:			return Key::F15;
#warning unknown key code for PAUSE
			//		case 0x:			return Key::PAUSE;
		default:			return Key::Count; // An unknown key.
	}
}
	
#pragma mark
#pragma mark WindowImplCocoa's event-related methods

////////////////////////////////////////////////////////////
void WindowImplCocoa::ProcessEvents(bool block)
{
	[myDelegate processEventWithBlockingMode:(block ? YES : NO)];
}
	
#pragma mark
#pragma mark WindowImplCocoa's private methods

////////////////////////////////////////////////////////////
WindowHandle WindowImplCocoa::GetSystemHandle() const
{
	return [myDelegate getSystemHandle];
}

	
////////////////////////////////////////////////////////////
void WindowImplCocoa::ShowMouseCursor(bool show)
{
	if (show) {
		[myDelegate showMouseCursor];
	} else {
		[myDelegate hideMouseCursor];
	}
}

	
////////////////////////////////////////////////////////////
void WindowImplCocoa::SetCursorPosition(unsigned int x, unsigned int y)
{
	[myDelegate setCursorPositionToX:x Y:y];
}

	
////////////////////////////////////////////////////////////
void WindowImplCocoa::SetPosition(int x, int y)
{
	[myDelegate setWindowPositionToX:x Y:y];
}

	
////////////////////////////////////////////////////////////
void WindowImplCocoa::SetSize(unsigned int width, unsigned int height)
{
	// Don't forget to update our parent (that is, WindowImpl) size :
	myWidth = width;
	myHeight = height;
	
	[myDelegate resizeTo:width by:height];
}

	
////////////////////////////////////////////////////////////
void WindowImplCocoa::SetTitle(const std::string& title)
{
	[myDelegate changeTitle:stringToNSString(title)];
}

	
////////////////////////////////////////////////////////////
void WindowImplCocoa::Show(bool show)
{
	if (show) {
		[myDelegate showWindow];
	} else {
		[myDelegate hideWindow];
	}
}

	
////////////////////////////////////////////////////////////
void WindowImplCocoa::EnableKeyRepeat(bool enabled)
{
	if (enabled) {
		[myDelegate enableKeyRepeat];
	} else {
		[myDelegate disableKeyRepeat];
	}
}

	
////////////////////////////////////////////////////////////
void WindowImplCocoa::SetIcon(unsigned int width, unsigned int height, 
															const Uint8* pixels)
{
	[myDelegate setIconTo:width by:height with:pixels];
}

#pragma mark
#pragma mark WindowImplCocoa's init methods

////////////////////////////////////////////////////////////
void WindowImplCocoa::SetUpPoolAndApplication(void)
{
	// Ensure NSApp exists.
	[NSApplication sharedApplication];
	
	// Create the pool.
	myPool = [[NSAutoreleasePool alloc] init];
	
	// Transform the app process.
	SetUpProcessAsApplication();
}

	
////////////////////////////////////////////////////////////
void WindowImplCocoa::SetUpProcessAsApplication(void)
{
	static bool isTheProcessSetAsApplication = false;
	
	if (!isTheProcessSetAsApplication) {
		// Do it only once !
		isTheProcessSetAsApplication = true;
		
		// Set the process as a normal application so it can get focus.
		ProcessSerialNumber psn;
		if (!GetCurrentProcess(&psn)) {
			TransformProcessType(&psn, kProcessTransformToForegroundApplication);
			SetFrontProcess(&psn);
		}
		
		// Tell the application to stop bouncing in the Dock.
		[[NSApplication sharedApplication] finishLaunching];
		// NOTE : This last call won't harm anything even if SFML window was
		// created with an external handle.
	}
}
	
} // namespace priv
	
} // namespace sf
