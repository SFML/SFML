////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007 Laurent Gomila (laurent.gom@gmail.com)
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

#ifndef SFML_RESOURCE_HPP
#define SFML_RESOURCE_HPP

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <set>


namespace sf
{
////////////////////////////////////////////////////////////
// These two classes are defined in the same header because
// they depend on each other. And as they're template classes,
// they must be entirely defined in header files, which
// prevents from proper separate compiling
////////////////////////////////////////////////////////////

template <typename> class ResourcePtr;

////////////////////////////////////////////////////////////
/// Base class for every resource that needs to notify
/// dependent classes about its destruction
////////////////////////////////////////////////////////////
template <typename T>
class Resource
{
protected :

    ////////////////////////////////////////////////////////////
    /// Default constructor
    ///
    ////////////////////////////////////////////////////////////
    Resource();

    ////////////////////////////////////////////////////////////
    /// Copy constructor
    ///
    /// \param Copy : Resource to copy
    ///
    ////////////////////////////////////////////////////////////
    Resource(const Resource<T>& Copy);

    ////////////////////////////////////////////////////////////
    /// Destructor
    ///
    ////////////////////////////////////////////////////////////
    ~Resource();

    ////////////////////////////////////////////////////////////
    /// Assignment operator
    ///
    /// \param Other : Resource to copy
    ///
    /// \return Reference to this
    ///
    ////////////////////////////////////////////////////////////
    Resource<T>& operator =(const Resource<T>& Other);

private :

    friend class ResourcePtr<T>;

    ////////////////////////////////////////////////////////////
    /// Connect a ResourcePtr to this resource
    ///
    /// \param Observer : Observer to add
    ///
    ////////////////////////////////////////////////////////////
    void Connect(ResourcePtr<T>& Observer) const;

    ////////////////////////////////////////////////////////////
    /// Disconnect a ResourcePtr from this resource
    ///
    /// \param Observer : Observer to remove
    ///
    ////////////////////////////////////////////////////////////
    void Disconnect(ResourcePtr<T>& Observer) const;

    ////////////////////////////////////////////////////////////
    // Member data
    ////////////////////////////////////////////////////////////
    mutable std::set<ResourcePtr<T>*> myObservers;
};


////////////////////////////////////////////////////////////
/// Safe pointer to a T resource (inheriting from sf::Resource<T>),
/// its pointer is automatically reseted when the resource is destroyed
////////////////////////////////////////////////////////////
template <typename T>
class ResourcePtr
{
public :

    ////////////////////////////////////////////////////////////
    /// Default constructor
    ///
    ////////////////////////////////////////////////////////////
    ResourcePtr();

    ////////////////////////////////////////////////////////////
    /// Construct from a raw resource
    ///
    /// \param Resource : Internal resource
    ///
    ////////////////////////////////////////////////////////////
    ResourcePtr(const T* Resource);

    ////////////////////////////////////////////////////////////
    /// Copy constructor
    ///
    /// \param Copy : Instance to copy
    ///
    ////////////////////////////////////////////////////////////
    ResourcePtr(const ResourcePtr<T>& Copy);

    ////////////////////////////////////////////////////////////
    /// Destructor
    ///
    ////////////////////////////////////////////////////////////
    ~ResourcePtr();

    ////////////////////////////////////////////////////////////
    /// Assignment operator from another ResourcePtr
    ///
    /// \param Other : Resource pointer to assign
    ///
    /// \return Reference to this
    ///
    ////////////////////////////////////////////////////////////
    ResourcePtr<T>& operator =(const ResourcePtr<T>& Other);

    ////////////////////////////////////////////////////////////
    /// Assignment operator from a raw resource
    ///
    /// \param Resource : Resource to assign
    ///
    /// \return Reference to this
    ///
    ////////////////////////////////////////////////////////////
    ResourcePtr<T>& operator =(const T* Resource);

    ////////////////////////////////////////////////////////////
    /// Cast operator to implicitely convert the resource pointer to
    /// its raw pointer type.
    /// This might be dangerous in the general case, but in this context
    /// it is safe enough to define this operator
    ///
    /// \return Pointer to the actual resource
    ///
    ////////////////////////////////////////////////////////////
    operator const T*() const;

    ////////////////////////////////////////////////////////////
    /// Operator * overload to return a reference to the actual resource
    ///
    /// \return Reference to the internal resource
    ///
    ////////////////////////////////////////////////////////////
    const T& operator *() const;

    ////////////////////////////////////////////////////////////
    /// Operator -> overload to return a pointer to the actual resource
    ///
    /// \return Pointer to the internal resource
    ///
    ////////////////////////////////////////////////////////////
    const T* operator ->() const;

    ////////////////////////////////////////////////////////////
    /// Function called when the observed resource is about to be
    /// destroyed
    ///
    ////////////////////////////////////////////////////////////
    void OnResourceDestroyed();

private :

    ////////////////////////////////////////////////////////////
    // Member data
    ////////////////////////////////////////////////////////////
    const T* myResource; /// Pointer to the actual resource
};

#include <SFML/System/Resource.inl>
#include <SFML/System/ResourcePtr.inl>

} // namespace sf


#endif // SFML_RESOURCE_HPP
