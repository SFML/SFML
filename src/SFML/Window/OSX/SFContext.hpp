

#ifndef SFML_SFCONTEXT_HPP
#define SFML_SFCONTEXT_HPP

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Window/GlContext.hpp>

////////////////////////////////////////////////////////////
/// Predefine OBJC classes
////////////////////////////////////////////////////////////
#ifdef __OBJC__

@class NSOpenGLContext;
typedef NSOpenGLContext* NSOpenGLContextRef;

@class NSAutoreleasePool;
typedef NSAutoreleasePool* NSAutoreleasePoolRef;

#else // If C++

typedef void* NSOpenGLContextRef;
typedef void* NSAutoreleasePoolRef;

#endif


namespace sf
{
namespace priv
{
////////////////////////////////////////////////////////////
/// \brief OSX (Cocoa) implementation of OpenGL contexts
///
////////////////////////////////////////////////////////////
class SFContext : public GlContext
{
public:
	////////////////////////////////////////////////////////////
	/// \brief Create a new context, not associated to a window
	///
	/// \param shared Context to share the new one with (can be NULL)
	///
	////////////////////////////////////////////////////////////
	SFContext(SFContext* shared);
	
	////////////////////////////////////////////////////////////
	/// \brief Create a new context attached to a window
	///
	/// \param shared       Context to share the new one with (can be NULL)
	/// \param owner        Pointer to the owner window
	/// \param bitsPerPixel Pixel depth (in bits per pixel)
	/// \param settings     Creation parameters
	///
	////////////////////////////////////////////////////////////
	SFContext(SFContext* shared, const WindowImpl* owner,
						unsigned int bitsPerPixel, const ContextSettings& settings);
	
	////////////////////////////////////////////////////////////
	/// \brief Destructor
	///
	////////////////////////////////////////////////////////////
	~SFContext();
	
	////////////////////////////////////////////////////////////
	/// \brief Display what has been rendered to the context so far
	///
	////////////////////////////////////////////////////////////
	virtual void Display();
	
	////////////////////////////////////////////////////////////
	/// \brief Enable or disable vertical synchronization
	///
	/// Activating vertical synchronization will limit the number
	/// of frames displayed to the refresh rate of the monitor.
	/// This can avoid some visual artifacts, and limit the framerate
	/// to a good value (but not constant across different computers).
	///
	/// \param enabled : True to enable v-sync, false to deactivate
	///
	////////////////////////////////////////////////////////////
	virtual void UseVerticalSync(bool enabled);
	
protected:
	////////////////////////////////////////////////////////////
	/// \brief Activate the context as the current target
	///        for rendering
	///
	/// \return True on success, false if any error happened
	///
	////////////////////////////////////////////////////////////
	virtual bool MakeCurrent();
	
private:
	////////////////////////////////////////////////////////////
	/// \brief Create the context
	/// \note Must only be called from Ctor.
	///
	/// \param shared       Context to share the new one with (can be NULL)
	/// \param settings     Creation parameters
	/// \param bitsPerPixel bpp
	///
	////////////////////////////////////////////////////////////
	void CreateContext(SFContext* shared, 
										 const ContextSettings& settings,
										 unsigned int bitsPerPixel);
	
	////////////////////////////////////////////////////////////
	// Member data
	////////////////////////////////////////////////////////////
	NSOpenGLContextRef		myContext;	///< OpenGL context
	NSAutoreleasePoolRef  myPool;			///< Memory manager for this class.
};
	
} // namespace priv
	
} // namespace sf

#endif // SFML_AGLCONTEXT_HPP
