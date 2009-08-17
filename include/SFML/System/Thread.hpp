////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007-2009 Laurent Gomila (laurent.gom@gmail.com)
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

#ifndef SFML_THREAD_HPP
#define SFML_THREAD_HPP

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Config.hpp>
#include <SFML/System/NonCopyable.hpp>
#include <cstdlib>


namespace sf
{
namespace priv
{
    class ThreadImpl;
}

////////////////////////////////////////////////////////////
/// \brief Utility class to manipulate threads
////////////////////////////////////////////////////////////
class SFML_API Thread : NonCopyable
{
public :

    typedef void (*FuncType)(void*);

    ////////////////////////////////////////////////////////////
    /// \brief Construct the thread from a function pointer
    ///
    /// \param function Entry point of the thread
    /// \param userData Data to pass to the thread function (NULL by default)
    ///
    ////////////////////////////////////////////////////////////
    Thread(FuncType function, void* userData = NULL);

    ////////////////////////////////////////////////////////////
    /// \brief Virtual destructor
    ///
    /// This destructor calls Wait(), so that the thread cannot
    /// survive when its sf::Thread instance is destroyed.
    ///
    ////////////////////////////////////////////////////////////
    virtual ~Thread();

    ////////////////////////////////////////////////////////////
    /// \brief Run the thread
    ///
    ////////////////////////////////////////////////////////////
    void Launch();

    ////////////////////////////////////////////////////////////
    /// \brief Wait until the thread finishes
    ///
    /// This function will block the execution until the
    /// thread's function ends.
    ///
    ////////////////////////////////////////////////////////////
    void Wait();

    ////////////////////////////////////////////////////////////
    /// \brief Terminate the thread
    ///
    /// This function immediately stops the thread, without waiting
    /// for its function to finish.
    /// Terminating a thread with this function is not safe,
    /// and can lead to local variables not being destroyed
    /// on some operating systems. You should rather try to make
    /// the thread function terminate by itself.
    ///
    ////////////////////////////////////////////////////////////
    void Terminate();

protected :

    ////////////////////////////////////////////////////////////
    /// \brief Default constructor
    ///
    /// This constructor is only accessible from derived classes
    ///
    ////////////////////////////////////////////////////////////
    Thread();

private :

    friend class priv::ThreadImpl;

    ////////////////////////////////////////////////////////////
    /// \brief Function called as the entry point of the thread
    ///
    /// You must override this function when inheriting from
    /// sf::Thread.
    ///
    ////////////////////////////////////////////////////////////
    virtual void Run();

    ////////////////////////////////////////////////////////////
    // Member data
    ////////////////////////////////////////////////////////////
    priv::ThreadImpl* myThreadImpl; ///< OS-specific implementation of the thread
    FuncType          myFunction;   ///< Function to call as the thread entry point
    void*             myUserData;   ///< Data to pass to the thread function
};

} // namespace sf


#endif // SFML_THREAD_HPP


////////////////////////////////////////////////////////////
/// \class sf::Thread
///
/// Threads provide a way to run multiple parts of the code
/// in parallel. When you launch a new thread, the execution
/// is split and both the new thread and the caller run
/// in parallel.
///
/// There are two ways to use sf::Thread. The first (and
/// preferred) way of using it is to created derived classes.
/// When inheriting from sf::Thread, the virtual function
/// Run() has to be overriden, and defines the entry point
/// of the thread. The thread automatically stops when
/// this function ends.
///
/// Usage example:
/// \code
/// class MyThread : public sf::Thread
/// {
/// private :
/// 
///     virtual void Run()
///     {
///         // beginning of the thread
///         ...
///         // end of the thread
///     }
/// };
///
/// MyThread mythread;
/// mythread.Launch(); // start the thread (internally calls thread.Run())
///
/// // from now on, both mythread and the main thread run in parallel
/// \endcode
///
/// The second way of using sf::Thread uses a non-member function
/// as the entry point and thus doesn't involve creating
/// a new class. However, this method is only provided for
/// compatibility with C-style code or for fast prototyping;
/// you are strongly encouraged to use the first method.
///
/// Usage example:
/// \code
/// void ThreadFunc(void* data)
/// {
///     // beginning of the thread
///     ...
///     // end of the thread
/// }
///
/// sf::Thread mythread(&ThreadFunc, NULL);
/// mythread.Launch(); // start the thread (internally calls ThreadFunc(NULL))
///
/// // from now on, both mythread and the main thread run in parallel
/// \endcode
///
/// Creating parallel threads of execution can be dangerous:
/// all threads inside the same process share the same memory space,
/// which means that you may end up accessing the same variable
/// from multiple threads at the same time. To prevent this
/// kind of situations, you can use mutexes (see sf::Mutex).
///
////////////////////////////////////////////////////////////
