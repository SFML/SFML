////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007-2023 Laurent Gomila (laurent@sfml-dev.org)
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
template <typename Handle, typename Deleter>
UniqueResource<Handle, Deleter>::UniqueResource() = default;


////////////////////////////////////////////////////////////
template <typename Handle, typename Deleter>
UniqueResource<Handle, Deleter>::UniqueResource(Handle handle) : m_handle(handle)
{
}


////////////////////////////////////////////////////////////
template <typename Handle, typename Deleter>
UniqueResource<Handle, Deleter>::~UniqueResource()
{
    reset();
}


////////////////////////////////////////////////////////////
template <typename Handle, typename Deleter>
UniqueResource<Handle, Deleter>::UniqueResource(UniqueResource&& other) noexcept :
m_handle(std::exchange(other.m_handle, {})),
m_deleter(std::exchange(other.m_deleter, {}))
{
}


////////////////////////////////////////////////////////////
template <typename Handle, typename Deleter>
UniqueResource<Handle, Deleter>& UniqueResource<Handle, Deleter>::operator=(UniqueResource&& other) noexcept
{
    if (this != &other)
    {
        reset();
        m_handle  = std::exchange(other.m_handle, {});
        m_deleter = std::exchange(other.m_deleter, {});
    }
    return *this;
}


////////////////////////////////////////////////////////////
template <typename Handle, typename Deleter>
void UniqueResource<Handle, Deleter>::release()
{
    m_handle.reset();
}


////////////////////////////////////////////////////////////
template <typename Handle, typename Deleter>
void UniqueResource<Handle, Deleter>::reset()
{
    if (m_handle.has_value())
        m_deleter(*m_handle);
    m_handle.reset();
}


////////////////////////////////////////////////////////////
template <typename Handle, typename Deleter>
void UniqueResource<Handle, Deleter>::reset(Handle handle)
{
    reset();
    m_handle = handle;
}


////////////////////////////////////////////////////////////
template <typename Handle, typename Deleter>
Handle UniqueResource<Handle, Deleter>::get() const
{
    assert(m_handle);
    return *m_handle;
}
