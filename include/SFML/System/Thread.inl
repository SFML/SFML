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

namespace priv
{
// Base class for abstract thread functions
struct ThreadFunc
{
    virtual ~ThreadFunc() {}
    virtual void Run() = 0;
};

// Specialization using a functor (including free functions) with no argument
template <typename T>
struct ThreadFunctor : ThreadFunc
{
    ThreadFunctor(T functor) : myFunctor(functor) {}
    virtual void Run() {myFunctor();}
    T myFunctor;
};

// Specialization using a functor (including free functions) with one argument
template <typename F, typename A>
struct ThreadFunctorWithArg : ThreadFunc
{
    ThreadFunctorWithArg(F function, A arg) : myFunction(function), myArg(arg) {}
    virtual void Run() {myFunction(myArg);}
    F myFunction;
    A myArg;
};

// Specialization using a member function
template <typename C>
struct ThreadMemberFunc : ThreadFunc
{
    ThreadMemberFunc(void(C::*function)(), C* object) : myFunction(function), myObject(object) {}
    virtual void Run() {(myObject->*myFunction)();}
    void(C::*myFunction)();
    C* myObject;
};

} // namespace priv


////////////////////////////////////////////////////////////
template <typename F>
Thread::Thread(F functor) :
myImpl    (NULL),
myFunction(new priv::ThreadFunctor<F>(functor))
{
}


////////////////////////////////////////////////////////////
template <typename F, typename A>
Thread::Thread(F function, A argument) :
myImpl    (NULL),
myFunction(new priv::ThreadFunctorWithArg<F, A>(function, argument))
{
}


////////////////////////////////////////////////////////////
template <typename C>
Thread::Thread(void(C::*function)(), C* object) :
myImpl    (NULL),
myFunction(new priv::ThreadMemberFunc<C>(function, object))
{
}
