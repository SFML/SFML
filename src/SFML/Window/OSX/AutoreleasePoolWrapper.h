////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007-2012 Marco Antognini (antognini.marco@gmail.com), 
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
/// \brief Ensure at least one autorelease pool is available on this thread.
///
/// Increment a retain count.
/// See SPECIAL CONSIDERATION in implementation file.
///
//////////////////////////////////////////////////////////// 
void retainPool(void);

////////////////////////////////////////////////////////////
/// \brief Release the pool.
///
/// Drain the pool if it is no more needed (retain count is zero).
/// See SPECIAL CONSIDERATION in implementation file.
///
//////////////////////////////////////////////////////////// 
void releasePool(void);

////////////////////////////////////////////////////////////
/// \brief Drain the pool.
///
/// releasePool must be called at least once before drainPool.
///
//////////////////////////////////////////////////////////// 
void drainPool();

