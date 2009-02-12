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
/// \param Filename : File to load
///
/// \return A new sfPostFX object, or NULL if it failed
///
////////////////////////////////////////////////////////////
CSFML_API sfPostFX* sfPostFX_CreateFromFile(const char* Filename);

////////////////////////////////////////////////////////////
/// Create a new post-fx from an effect source code
///
/// \param Effect : Source code of the effect
///
/// \return A new sfPostFX object, or NULL if it failed
///
////////////////////////////////////////////////////////////
CSFML_API sfPostFX* sfPostFX_CreateFromMemory(const char* Effect);

////////////////////////////////////////////////////////////
/// Destroy an existing post-fx
///
/// \param PostFX : PostFX to delete
///
////////////////////////////////////////////////////////////
CSFML_API void sfPostFX_Destroy(sfPostFX* PostFX);

////////////////////////////////////////////////////////////
/// Change a parameter of a post-fx (1 float)
///
/// \param PostFX : Post-effect to modify
/// \param Name :   Parameter name in the effect
/// \param X :      Value to assign
///
////////////////////////////////////////////////////////////
CSFML_API void sfPostFX_SetParameter1(sfPostFX* PostFX, const char* Name, float X);

////////////////////////////////////////////////////////////
/// Change a parameter of a post-fx (2 floats)
///
/// \param PostFX : Post-effect to modify
/// \param Name :   Parameter name in the effect
/// \param X, Y :   Values to assign
///
////////////////////////////////////////////////////////////
CSFML_API void sfPostFX_SetParameter2(sfPostFX* PostFX, const char* Name, float X, float Y);

////////////////////////////////////////////////////////////
/// Change a parameter of a post-fx (3 floats)
///
/// \param PostFX :  Post-effect to modify
/// \param Name :    Parameter name in the effect
/// \param X, Y, Z : Values to assign
///
////////////////////////////////////////////////////////////
CSFML_API void sfPostFX_SetParameter3(sfPostFX* PostFX, const char* Name, float X, float Y, float Z);

////////////////////////////////////////////////////////////
/// Change a parameter of a post-fx (4 floats)
///
/// \param PostFX :     Post-effect to modify
/// \param Name :       Parameter name in the effect
/// \param X, Y, Z, W : Values to assign
///
////////////////////////////////////////////////////////////
CSFML_API void sfPostFX_SetParameter4(sfPostFX* PostFX, const char* Name, float X, float Y, float Z, float W);

////////////////////////////////////////////////////////////
/// Set a texture parameter in a post-fx
///
/// \param PostFX :  Post-effect to modify
/// \param Name :    Texture name in the effect
/// \param Texture : Image to set (pass NULL to use content of current framebuffer)
///
////////////////////////////////////////////////////////////
CSFML_API void sfPostFX_SetTexture(sfPostFX* PostFX, const char* Name, sfImage* Texture);

////////////////////////////////////////////////////////////
/// Tell whether or not the system supports post-effects
///
/// \return sfTrue if the system can use post-effects
///
////////////////////////////////////////////////////////////
CSFML_API sfBool sfPostFX_CanUsePostFX();


#endif // SFML_POSTFX_H
