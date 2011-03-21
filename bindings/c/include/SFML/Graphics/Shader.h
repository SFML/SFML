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

#ifndef SFML_SHADER_H
#define SFML_SHADER_H

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Config.h>
#include <SFML/Graphics/Types.h>


////////////////////////////////////////////////////////////
/// Create a new shader from a file
///
/// \param filename : File to load
///
/// \return A new sfShader object, or NULL if it failed
///
////////////////////////////////////////////////////////////
CSFML_API sfShader* sfShader_CreateFromFile(const char* filename);

////////////////////////////////////////////////////////////
/// Create a new shader from an effect source code
///
/// \param effect : Source code of the effect
///
/// \return A new sfShader object, or NULL if it failed
///
////////////////////////////////////////////////////////////
CSFML_API sfShader* sfShader_CreateFromMemory(const char* effect);

////////////////////////////////////////////////////////////
/// Copy an existing shader
///
/// \param shader : Shader to copy
///
/// \return Copied object
///
////////////////////////////////////////////////////////////
CSFML_API sfShader* sfShader_Copy(sfShader* shader);

////////////////////////////////////////////////////////////
/// Destroy an existing shader
///
/// \param shader : Shader to delete
///
////////////////////////////////////////////////////////////
CSFML_API void sfShader_Destroy(sfShader* shader);

////////////////////////////////////////////////////////////
/// Change a parameter of a shader (1 float)
///
/// \param shader : Shader to modify
/// \param name :   Parameter name in the effect
/// \param x :      Value to assign
///
////////////////////////////////////////////////////////////
CSFML_API void sfShader_SetParameter1(sfShader* shader, const char* name, float x);

////////////////////////////////////////////////////////////
/// Change a parameter of a shader (2 floats)
///
/// \param shader : Shader to modify
/// \param name :   Parameter name in the effect
/// \param x, y :   Values to assign
///
////////////////////////////////////////////////////////////
CSFML_API void sfShader_SetParameter2(sfShader* shader, const char* name, float x, float y);

////////////////////////////////////////////////////////////
/// Change a parameter of a shader (3 floats)
///
/// \param shader :  Shader to modify
/// \param name :    Parameter name in the effect
/// \param x, y, z : Values to assign
///
////////////////////////////////////////////////////////////
CSFML_API void sfShader_SetParameter3(sfShader* shader, const char* name, float x, float y, float z);

////////////////////////////////////////////////////////////
/// Change a parameter of a shader (4 floats)
///
/// \param shader :     Shader to modify
/// \param name :       Parameter name in the effect
/// \param x, y, z, w : Values to assign
///
////////////////////////////////////////////////////////////
CSFML_API void sfShader_SetParameter4(sfShader* shader, const char* name, float x, float y, float z, float w);

////////////////////////////////////////////////////////////
/// Set a texture parameter in a shader
///
/// \param shader :  Shader to modify
/// \param name :    Texture name in the effect
/// \param texture : Image to set
///
////////////////////////////////////////////////////////////
CSFML_API void sfShader_SetTexture(sfShader* shader, const char* name, const sfImage* texture);

////////////////////////////////////////////////////////////
/// Set the current texture parameter in a shader
///
/// \param shader : Shader to modify
/// \param name :   Texture name in the effect
///
////////////////////////////////////////////////////////////
CSFML_API void sfShader_SetCurrentTexture(sfShader* shader, const char* name);

////////////////////////////////////////////////////////////
/// Bind a shader for rendering
///
/// \param shader : Shader to bind
///
////////////////////////////////////////////////////////////
CSFML_API void sfShader_Bind(const sfShader* shader);

////////////////////////////////////////////////////////////
/// Unbind a shader
///
/// \param shader : Shader to unbind
///
////////////////////////////////////////////////////////////
CSFML_API void sfShader_Unbind(const sfShader* shader);

////////////////////////////////////////////////////////////
/// Tell whether or not the system supports shaders
///
/// \return sfTrue if the system can use shaders
///
////////////////////////////////////////////////////////////
CSFML_API sfBool sfShader_IsAvailable(void);


#endif // SFML_SHADER_H
