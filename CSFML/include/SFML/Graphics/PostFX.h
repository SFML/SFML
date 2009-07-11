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

#ifndef SFML_POSTFX_H
#define SFML_POSTFX_H

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Config.h>
#include <SFML/Graphics/Types.h>


////////////////////////////////////////////////////////////
/// Create a new post-fx from a file
///
/// \param filename : File to load
///
/// \return A new sfPostFX object, or NULL if it failed
///
////////////////////////////////////////////////////////////
CSFML_API sfPostFX* sfPostFX_CreateFromFile(const char* Filename);

////////////////////////////////////////////////////////////
/// Create a new post-fx from an effect source code
///
/// \param effect : Source code of the effect
///
/// \return A new sfPostFX object, or NULL if it failed
///
////////////////////////////////////////////////////////////
CSFML_API sfPostFX* sfPostFX_CreateFromMemory(const char* effect);

////////////////////////////////////////////////////////////
/// Destroy an existing post-fx
///
/// \param postFX : PostFX to delete
///
////////////////////////////////////////////////////////////
CSFML_API void sfPostFX_Destroy(sfPostFX* postFX);

////////////////////////////////////////////////////////////
/// Change a parameter of a post-fx (1 float)
///
/// \param postFX : Post-effect to modify
/// \param name :   Parameter name in the effect
/// \param x :      Value to assign
///
////////////////////////////////////////////////////////////
CSFML_API void sfPostFX_SetParameter1(sfPostFX* postFX, const char* name, float x);

////////////////////////////////////////////////////////////
/// Change a parameter of a post-fx (2 floats)
///
/// \param postFX : Post-effect to modify
/// \param name :   Parameter name in the effect
/// \param x, y :   Values to assign
///
////////////////////////////////////////////////////////////
CSFML_API void sfPostFX_SetParameter2(sfPostFX* postFX, const char* name, float x, float y);

////////////////////////////////////////////////////////////
/// Change a parameter of a post-fx (3 floats)
///
/// \param postFX :  Post-effect to modify
/// \param name :    Parameter name in the effect
/// \param x, y, z : Values to assign
///
////////////////////////////////////////////////////////////
CSFML_API void sfPostFX_SetParameter3(sfPostFX* postFX, const char* name, float x, float y, float z);

////////////////////////////////////////////////////////////
/// Change a parameter of a post-fx (4 floats)
///
/// \param postFX :     Post-effect to modify
/// \param name :       Parameter name in the effect
/// \param x, y, z, w : Values to assign
///
////////////////////////////////////////////////////////////
CSFML_API void sfPostFX_SetParameter4(sfPostFX* postFX, const char* name, float x, float y, float z, float w);

////////////////////////////////////////////////////////////
/// Set a texture parameter in a post-fx
///
/// \param postFX :  Post-effect to modify
/// \param name :    Texture name in the effect
/// \param texture : Image to set (pass NULL to use content of current framebuffer)
///
////////////////////////////////////////////////////////////
CSFML_API void sfPostFX_SetTexture(sfPostFX* postFX, const char* name, sfImage* texture);

////////////////////////////////////////////////////////////
/// Tell whether or not the system supports post-effects
///
/// \return sfTrue if the system can use post-effects
///
////////////////////////////////////////////////////////////
CSFML_API sfBool sfPostFX_CanUsePostFX();


#endif // SFML_POSTFX_H
