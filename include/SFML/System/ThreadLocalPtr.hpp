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

#ifndef SFML_THREADLOCALPTR_HPP
#define SFML_THREADLOCALPTR_HPP

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/System/ThreadLocal.hpp>


namespace sf
{
////////////////////////////////////////////////////////////
/// Type-safe wrapper for thread local pointer variables
////////////////////////////////////////////////////////////
template <typename T>
class ThreadLocalPtr : private ThreadLocal
{
public :

    ////////////////////////////////////////////////////////////
    /// Default constructor
    ///
    /// \param value : Optional value to initalize the variable (NULL by default)
    ///
    ////////////////////////////////////////////////////////////
    ThreadLocalPtr(T* value = NULL);

    ////////////////////////////////////////////////////////////
    /// Operator * overload to return a reference to the variable
    ///
    /// \return Reference to the thread-local value of the variable
    ///
    ////////////////////////////////////////////////////////////
    T& operator *() const;

    ////////////////////////////////////////////////////////////
    /// Operator -> overload to return a pointer to the variable
    ///
    /// \return Pointer to the thread-local value of the variable
    ///
    ////////////////////////////////////////////////////////////
    T* operator ->() const;

    ////////////////////////////////////////////////////////////
    /// Implicit cast operator to T*
    ///
    /// \return Value of the pointer for this thread
    ///
    ////////////////////////////////////////////////////////////
    operator T*() const;

    ////////////////////////////////////////////////////////////
    /// Assignment operator
    ///
    /// \param value : New pointer value to assign for this thread
    ///
    /// \return Reference to this
    ///
    ////////////////////////////////////////////////////////////
    ThreadLocalPtr<T>& operator =(T* value);

    ////////////////////////////////////////////////////////////
    /// Assignment operator
    ///
    /// \param other : Other thread-local pointer value to assign
    ///
    /// \return Reference to this
    ///
    ////////////////////////////////////////////////////////////
    ThreadLocalPtr<T>& operator =(const ThreadLocalPtr<T>& other);
};

} // namespace sf

#include <SFML/System/ThreadLocalPtr.inl>


#endif // SFML_THREADLOCALPTR_HPP
