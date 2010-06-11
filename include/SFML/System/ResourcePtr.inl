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
template <typename T>
ResourcePtr<T>::ResourcePtr() :
myResource(NULL)
{

}


////////////////////////////////////////////////////////////
template <typename T>
ResourcePtr<T>::ResourcePtr(const T* resource) :
myResource(resource)
{
    if (myResource)
        myResource->Connect(*this);
}


////////////////////////////////////////////////////////////
template <typename T>
ResourcePtr<T>::ResourcePtr(const ResourcePtr<T>& copy) :
myResource(copy.myResource)
{
    if (myResource)
        myResource->Connect(*this);
}


////////////////////////////////////////////////////////////
template <typename T>
ResourcePtr<T>::~ResourcePtr()
{
    if (myResource)
        myResource->Disconnect(*this);
}


////////////////////////////////////////////////////////////
template <typename T>
ResourcePtr<T>& ResourcePtr<T>::operator =(const ResourcePtr<T>& right)
{
    if (myResource)
        myResource->Disconnect(*this);

    myResource = right.myResource;

    if (myResource)
        myResource->Connect(*this);

    return *this;
}


////////////////////////////////////////////////////////////
template <typename T>
ResourcePtr<T>& ResourcePtr<T>::operator =(const T* resource)
{
    if (myResource)
        myResource->Disconnect(*this);

    myResource = resource;

    if (myResource)
        myResource->Connect(*this);

    return *this;
}


////////////////////////////////////////////////////////////
template <typename T>
ResourcePtr<T>::operator const T*() const
{
    return myResource;
}


////////////////////////////////////////////////////////////
template <typename T>
const T& ResourcePtr<T>::operator *() const
{
    return *myResource;
}


////////////////////////////////////////////////////////////
template <typename T>
const T* ResourcePtr<T>::operator ->() const
{
    return myResource;
}


////////////////////////////////////////////////////////////
template <typename T>
void ResourcePtr<T>::OnResourceDestroyed()
{
    myResource = NULL;
}
