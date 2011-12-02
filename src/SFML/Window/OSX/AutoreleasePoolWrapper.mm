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
/// This implies that if RetainPool is called X times in a thread Y then
/// ReleasePool must be called X times too in the same thread Y.
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
    void Retain();
    
    ////////////////////////////////////////////////////////////
    /// \brief Decrement retain count and releasing memory if needed
    ///
    ////////////////////////////////////////////////////////////
    void Release();
    
    ////////////////////////////////////////////////////////////
    /// \brief Drain the pool
    ///
    ////////////////////////////////////////////////////////////
    void Drain();
    
private:
    
    ////////////////////////////////////////////////////////////
    // Member data
    ////////////////////////////////////////////////////////////
    int count;                  ///< How many times was the pool retained ?
    NSAutoreleasePool* pool;    ///< Our dedicated pool
};


////////////////////////////////////////////////////////////
PoolWrapper::PoolWrapper()
: count(0)
, pool(0) 
{
    /* Nothing else */
}


////////////////////////////////////////////////////////////
PoolWrapper::~PoolWrapper()
{
#ifdef SFML_DEBUG
    if (count < 0) {
        sf::Err() << "~PoolWrapper : count is less than zero! "
                     "You called ReleasePool from a thread too many times."
                  << std::endl;
    } else if (count > 0) {
        sf::Err() << "~PoolWrapper : count is greater than zero! "
                     "You called ReleasePool from a thread to few times."
                  << std::endl;
    } else { // count == 0
        sf::Err() << "~PoolWrapper is HAPPY!" << std::endl;
    }
#endif
}


////////////////////////////////////////////////////////////
void PoolWrapper::Retain()
{
    // Increase counter.
    ++count;
    
    // Allocate pool if required.
    if (pool == 0) {
        pool = [[NSAutoreleasePool alloc] init];
    }
    
#ifdef SFML_DEBUG
    if (count <= 0) {
        sf::Err() << "PoolWrapper::Retain : count <= 0! " << std::endl;
    }
#endif
}


////////////////////////////////////////////////////////////
void PoolWrapper::Release()
{
    // Decrease counter.
    --count;
    
    // Drain pool if required.
    if (count == 0) {
        Drain();
    }
    
#ifdef SFML_DEBUG
    if (count < 0) {
        sf::Err() << "PoolWrapper::Release : count < 0! " << std::endl;
    }
#endif
}

void PoolWrapper::Drain()
{
    [pool drain];
    pool = 0;
    
    if (count != 0) {
        pool = [[NSAutoreleasePool alloc] init];
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
void RetainPool(void)
{
    // First, Check that we have a valid PoolWrapper object in our local pool.
    if (localPool == NULL) {
        localPool = new sf::priv::PoolWrapper();
    }
    
    // Then retains!
    localPool->Retain();
}


////////////////////////////////////////////////////////////
void ReleasePool(void)
{
#ifdef SFML_DEBUG
    if (localPool == NULL) {
        sf::Err() << "ReleasePool : You must call RetainPool at least once "
                     "in this thread before calling ReleasePool."
                  << std::endl;
    } else {
#endif
    
    // Releases, that's all.
    localPool->Release();
    
#ifdef SFML_DEBUG
    }
#endif
}


////////////////////////////////////////////////////////////
void DrainPool()
{
    if (localPool != NULL) {
        localPool->Drain();
    }
#ifdef SFML_DEBUG
    else {
        sf::Err() << "ReleasePool must be called at least one before DrainPool"
                  << std::endl;
    }
#endif
}

