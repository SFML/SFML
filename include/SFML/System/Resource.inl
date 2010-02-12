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
Resource<T>::Resource()
{
    // Nothing to do
}


////////////////////////////////////////////////////////////
template <typename T>
Resource<T>::Resource(const Resource<T>&)
{
    // Nothing to do, we don't want to copy observers
}


////////////////////////////////////////////////////////////
template <typename T>
Resource<T>::~Resource()
{
    // Notify all observers
    for (typename std::set<ResourcePtr<T>*>::iterator i = myObservers.begin(); i != myObservers.end(); ++i)
    {
        (*i)->OnResourceDestroyed();
    }
}


////////////////////////////////////////////////////////////
template <typename T>
Resource<T>& Resource<T>::operator =(const Resource<T>&)
{
    // Nothing to do, we don't want to copy observers
    return *this;
}


////////////////////////////////////////////////////////////
template <typename T>
void Resource<T>::Connect(ResourcePtr<T>& observer) const
{
    sf::Lock lock(myMutex);
    myObservers.insert(&observer);
}


////////////////////////////////////////////////////////////
template <typename T>
void Resource<T>::Disconnect(ResourcePtr<T>& observer) const
{
    sf::Lock lock(myMutex);
    myObservers.erase(&observer);
}
