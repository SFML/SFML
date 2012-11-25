////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007-2012 Laurent Gomila (laurent.gom@gmail.com)
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

#ifndef AUTOPOINTER_HPP
#define AUTOPOINTER_HPP

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <X11/Xlib-xcb.h>

namespace sf
{
namespace priv
{
////////////////////////////////////////////////////////////
/// \brief Pointer Wrapper. Memory is automatically free'd
/// on deletion of the object.
///
////////////////////////////////////////////////////////////
template<typename T>
class AutoPointer
{
public:
    ////////////////////////////////////////////////////////////
    /// \brief Constructor
    ///
    ////////////////////////////////////////////////////////////
    AutoPointer(T* ptr)
    {
        pointer = ptr;
    }

    ////////////////////////////////////////////////////////////
    /// \brief Constructor
    ///
    ////////////////////////////////////////////////////////////
    AutoPointer()
        : pointer(NULL)
    {
    }

    ////////////////////////////////////////////////////////////
    /// \brief Destructor, frees the error pointer
    ///
    ////////////////////////////////////////////////////////////
    ~AutoPointer()
    {
        free(pointer);
    }

    ////////////////////////////////////////////////////////////
    /// \brief Operator for error member access
    ///
    ////////////////////////////////////////////////////////////
    T* operator->() const
    {
        return pointer;
    }

    ////////////////////////////////////////////////////////////
    /// \brief Assignment operator. Frees the previous pointer
    ///
    ////////////////////////////////////////////////////////////
    T* operator=(T*& new_pointer) const
    {
        free(pointer);
        pointer = new_pointer;
    }

    T** operator&()
    {
        return &pointer;
    }

    ////////////////////////////////////////////////////////////
    /// \brief Check if reqeust succeeded
    ///
    /// \return true if reqeust succeeded
    ///
    ////////////////////////////////////////////////////////////
    bool isNull() const
    {
        return pointer == NULL;
    }

private:
    T* pointer;
};


////////////////////////////////////////////////////////////
/// \brief Wrapper for automatically creating and freeing
/// an error struct out of a cookie
///
////////////////////////////////////////////////////////////
class ErrorPointer : public AutoPointer<xcb_generic_error_t>
{
public:
    ////////////////////////////////////////////////////////////
    /// \brief Construct the error pointer from a cookie
    ///
    ////////////////////////////////////////////////////////////
    ErrorPointer(xcb_connection_t*& connection, xcb_void_cookie_t& cookie);
};

} // namespace priv

} // namespace sf

#endif // AUTOPOINTER_HPP
