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


////////////////////////////////////////////////////////////
/// Default constructor
////////////////////////////////////////////////////////////
template <typename T>
ResourcePtr<T>::ResourcePtr() :
myResource(NULL)
{

}


////////////////////////////////////////////////////////////
/// Construct from a raw resource
////////////////////////////////////////////////////////////
template <typename T>
ResourcePtr<T>::ResourcePtr(const T* Resource) :
myResource(Resource)
{
    if (myResource)
        myResource->Connect(*this);
}


////////////////////////////////////////////////////////////
/// Copy constructor
////////////////////////////////////////////////////////////
template <typename T>
ResourcePtr<T>::ResourcePtr(const ResourcePtr<T>& Copy) :
myResource(Copy.myResource)
{
    if (myResource)
        myResource->Connect(*this);
}


////////////////////////////////////////////////////////////
/// Destructor
////////////////////////////////////////////////////////////
template <typename T>
ResourcePtr<T>::~ResourcePtr()
{
    if (myResource)
        myResource->Disconnect(*this);
}


////////////////////////////////////////////////////////////
/// Assignment operator from another ResourcePtr
////////////////////////////////////////////////////////////
template <typename T>
ResourcePtr<T>& ResourcePtr<T>::operator =(const ResourcePtr<T>& Other)
{
    if (myResource)
        myResource->Disconnect(*this);

    myResource = Other.myResource;

    if (myResource)
        myResource->Connect(*this);

    return *this;
}


////////////////////////////////////////////////////////////
/// Assignment operator from a raw resource
////////////////////////////////////////////////////////////
template <typename T>
ResourcePtr<T>& ResourcePtr<T>::operator =(const T* Resource)
{
    if (myResource)
        myResource->Disconnect(*this);

    myResource = Resource;

    if (myResource)
        myResource->Connect(*this);

    return *this;
}


////////////////////////////////////////////////////////////
/// Cast operator to implicitely convert the resource pointer to
/// its raw pointer type.
/// This might be dangerous in the general case, but in this context
/// it is safe enough to define this operator
////////////////////////////////////////////////////////////
template <typename T>
ResourcePtr<T>::operator const T*() const
{
    return myResource;
}


////////////////////////////////////////////////////////////
/// Operator * overload to return a reference to the actual resource
////////////////////////////////////////////////////////////
template <typename T>
const T& ResourcePtr<T>::operator *() const
{
    return *myResource;
}


////////////////////////////////////////////////////////////
/// Operator -> overload to return a pointer to the actual resource
////////////////////////////////////////////////////////////
template <typename T>
const T* ResourcePtr<T>::operator ->() const
{
    return myResource;
}


////////////////////////////////////////////////////////////
/// Function called when the observed resource is about to be
/// destroyed
////////////////////////////////////////////////////////////
template <typename T>
void ResourcePtr<T>::OnResourceDestroyed()
{
    myResource = NULL;
}
