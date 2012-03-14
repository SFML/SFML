////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007-2012 Marco Antognini (antognini.marco@gmail.com), 
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
#include <SFML/System/ThreadLocalPtr.hpp>
#include <SFML/System/NonCopyable.hpp>
#include <SFML/System/Err.hpp>

#import <Foundation/Foundation.h>
#import <SFML/Window/OSX/AutoreleasePoolWrapper.h>


////////////////////////////////////////////////////////////
/// Here we manage one and only one pool by thread. This prevents draining one 
/// pool and making other pools invalid which can lead to a crash on 10.5 and an
/// annoying message on 10.6 (*** attempt to pop an unknown autorelease pool).
///
/// Because NSAutoreleasePool cannot be retain we have to do it ourself.
/// We use an sf::ThreadLocalPtr to have one PoolWrapper in each thread.
///
/// SPECIAL CONSIDERATION :
/// =======================
/// This implies that if retainPool is called X times in a thread Y then
/// releasePool must be called X times too in the same thread Y.
///
////////////////////////////////////////////////////////////

namespace sf
{
namespace priv
{
////////////////////////////////////////////////////////////
/// \brief C++ Wrapper of Obj-C Autorelease Pool.
///
////////////////////////////////////////////////////////////
class PoolWrapper : NonCopyable {
public :
    
    ////////////////////////////////////////////////////////////
    /// \brief Default constructor
    ///
    ////////////////////////////////////////////////////////////
    PoolWrapper();
    
    ////////////////////////////////////////////////////////////
    /// \brief Default destructor
    ///
    ////////////////////////////////////////////////////////////
    ~PoolWrapper();
    
    ////////////////////////////////////////////////////////////
    /// \brief Increment retain count and allocate memory if needed
    ///
    ////////////////////////////////////////////////////////////
    void retain();
    
    ////////////////////////////////////////////////////////////
    /// \brief Decrement retain count and releasing memory if needed
    ///
    ////////////////////////////////////////////////////////////
    void release();
    
    ////////////////////////////////////////////////////////////
    /// \brief Drain the pool
    ///
    ////////////////////////////////////////////////////////////
    void Drain();
    
private:
    
    ////////////////////////////////////////////////////////////
    // Member data
    ////////////////////////////////////////////////////////////
    int m_count;                  ///< How many times was the pool retained ?
    NSAutoreleasePool* m_pool;    ///< Our dedicated pool
};


////////////////////////////////////////////////////////////
PoolWrapper::PoolWrapper()
: m_count(0)
, m_pool(0) 
{
    /* Nothing else */
}


////////////////////////////////////////////////////////////
PoolWrapper::~PoolWrapper()
{
#ifdef SFML_DEBUG
    if (m_count < 0) {
        sf::err() << "~PoolWrapper : m_count is less than zero! "
                     "You called releasePool from a thread too many times."
                  << std::endl;
    } else if (m_count > 0) {
        sf::err() << "~PoolWrapper : m_count is greater than zero! "
                     "You called releasePool from a thread to few times."
                  << std::endl;
    } else { // m_count == 0
        sf::err() << "~PoolWrapper is HAPPY!" << std::endl;
    }
#endif
}


////////////////////////////////////////////////////////////
void PoolWrapper::retain()
{
    // Increase counter.
    ++m_count;
    
    // Allocate pool if required.
    if (m_pool == 0) {
        m_pool = [[NSAutoreleasePool alloc] init];
    }
    
#ifdef SFML_DEBUG
    if (m_count <= 0) {
        sf::err() << "PoolWrapper::retain : m_count <= 0! " << std::endl;
    }
#endif
}


////////////////////////////////////////////////////////////
void PoolWrapper::release()
{
    // Decrease counter.
    --m_count;
    
    // Drain pool if required.
    if (m_count == 0) {
        Drain();
    }
    
#ifdef SFML_DEBUG
    if (m_count < 0) {
        sf::err() << "PoolWrapper::release : m_count < 0! " << std::endl;
    }
#endif
}

void PoolWrapper::Drain()
{
    [m_pool drain];
    m_pool = 0;
    
    if (m_count != 0) {
        m_pool = [[NSAutoreleasePool alloc] init];
    }
}

    
} // namespace priv
    
} // namespace sf

////////////////////////////////////////////////////////////
// Private data
////////////////////////////////////////////////////////////
namespace
{
    // This per-thread variable holds the current autorelease pool for each thread
    sf::ThreadLocalPtr<sf::priv::PoolWrapper> localPool;
}


////////////////////////////////////////////////////////////
void retainPool(void)
{
    // First, Check that we have a valid PoolWrapper object in our local pool.
    if (localPool == NULL) {
        localPool = new sf::priv::PoolWrapper();
    }
    
    // Then retains!
    localPool->retain();
}


////////////////////////////////////////////////////////////
void releasePool(void)
{
#ifdef SFML_DEBUG
    if (localPool == NULL) {
        sf::err() << "releasePool : You must call retainPool at least once "
                     "in this thread before calling releasePool."
                  << std::endl;
    } else {
#endif
    
    // Releases, that's all.
    localPool->release();
    
#ifdef SFML_DEBUG
    }
#endif
}


////////////////////////////////////////////////////////////
void drainPool()
{
    if (localPool != NULL) {
        localPool->Drain();
    }
#ifdef SFML_DEBUG
    else {
        sf::err() << "releasePool must be called at least one before drainPool"
                  << std::endl;
    }
#endif
}

