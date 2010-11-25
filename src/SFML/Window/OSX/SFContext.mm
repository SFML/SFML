

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Window/OSX/SFContext.hpp>
#include <SFML/Window/OSX/WindowImplCocoa.hpp>
#include <SFML/System/Err.hpp>


/*
 * DISCUSSION :
 * ============
 * 
 * [1] (2010_07)	
 *					should AA-related NSOpenGLPixelFormatAttribute not be in the array 
 *					if AA is not enable (settings.AntialiasingLevel == 0) ?
 *					=> will not be present in attributs array if 0.
 *
 * [2] (2010_07)	
 *					how many buffer should be used for AA ?
 *					=> «1» was choosen.
 */

namespace sf
{
namespace priv
{

////////////////////////////////////////////////////////////
SFContext::SFContext(SFContext* shared)
{
	myPool = [[NSAutoreleasePool alloc] init];
	
	// Create the context
	CreateContext(shared, ContextSettings(0, 0, 0), 0);
}

	
////////////////////////////////////////////////////////////
SFContext::SFContext(SFContext* shared, const WindowImpl* owner, 
										 unsigned int bitsPerPixel, const ContextSettings& settings)
{
	myPool = [[NSAutoreleasePool alloc] init];
	
	// Create the context.
	CreateContext(shared, settings, bitsPerPixel);
	
	// Apply context.
	WindowImplCocoa const * ownerCocoa = static_cast<WindowImplCocoa const *>(owner);
	ownerCocoa->ApplyContext(myContext);
}

	
////////////////////////////////////////////////////////////
SFContext::~SFContext()
{
	[myContext release];
	[myPool release];
}

	
////////////////////////////////////////////////////////////
bool SFContext::MakeCurrent()
{
	[myContext makeCurrentContext];
}

	
////////////////////////////////////////////////////////////
void SFContext::Display()
{
	[myContext flushBuffer];
}

	
////////////////////////////////////////////////////////////
void SFContext::UseVerticalSync(bool enabled)
{
	// Make compiler happy
#ifdef USE_OS_X_VERSION_10_4
	long int swapInterval = enabled ? 1 : 0;
#else /* USE_OS_X_VERSION_10_6 */
	GLint swapInterval = enabled ? 1 : 0;
#endif
	
	[myContext setValues:&swapInterval forParameter:NSOpenGLCPSwapInterval];
}

	
////////////////////////////////////////////////////////////	
void SFContext::CreateContext(SFContext* shared, 
															const ContextSettings& settings,
															unsigned int bitsPerPixel)
{
	// Choose the attributs of OGL context.
	std::vector<NSOpenGLPixelFormatAttribute> attrs;
	attrs.reserve(20); // max attributs (estimation).
	
	// These casts are safe. C++ is much more stric than Obj-C.
	
	attrs.push_back(NSOpenGLPFAClosestPolicy);
	
	attrs.push_back(NSOpenGLPFADoubleBuffer);
	
	if (bitsPerPixel > 24) {
		attrs.push_back(NSOpenGLPFAAlphaSize);
		attrs.push_back((NSOpenGLPixelFormatAttribute)8);
	}
	
	attrs.push_back(NSOpenGLPFADepthSize);
	attrs.push_back((NSOpenGLPixelFormatAttribute)settings.DepthBits);
	
	attrs.push_back(NSOpenGLPFAStencilSize);
	attrs.push_back((NSOpenGLPixelFormatAttribute)settings.StencilBits);
	
	if (settings.AntialiasingLevel > 0) { // [1]
		attrs.push_back(NSOpenGLPFAMultisample);
		
		attrs.push_back(NSOpenGLPFASampleBuffers);
		attrs.push_back((NSOpenGLPixelFormatAttribute)1); // [2]
		
		attrs.push_back(NSOpenGLPFASamples);
		attrs.push_back((NSOpenGLPixelFormatAttribute)settings.AntialiasingLevel);
	}
	
	attrs.push_back((NSOpenGLPixelFormatAttribute)0); // end of array
	
	// Create the pixel pormat.
	NSOpenGLPixelFormat* pixFmt = [[NSOpenGLPixelFormat alloc] initWithAttributes:&attrs[0]];
	
	if(pixFmt == nil) {
		sf::Err() << "Error. Unable to find a suitable pixel format." << std::endl;
		return;
	}
	
	// Use the shared context if one is given.
	NSOpenGLContext* sharedContext = shared != NULL ? shared->myContext : nil;
	
	// Create the context.
	myContext = [[NSOpenGLContext alloc] initWithFormat:pixFmt
																				 shareContext:sharedContext];
	
	// Free up.
	[pixFmt release];
	
#warning update settings with ogl version not yet implemented
	
	// Save the creation settings
	mySettings = settings;
}
	
} // namespace priv
	
} // namespace sf

