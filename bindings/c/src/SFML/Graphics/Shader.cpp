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

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Graphics/Shader.h>
#include <SFML/Graphics/ShaderStruct.h>
#include <SFML/Graphics/ImageStruct.h>
#include <SFML/Internal.h>


////////////////////////////////////////////////////////////
/// Create a new shader from a file
////////////////////////////////////////////////////////////
sfShader* sfShader_CreateFromFile(const char* filename)
{
    sfShader* shader = new sfShader;

    if (!shader->This.LoadFromFile(filename))
    {
        delete shader;
        shader = NULL;
    }

    return shader;
}


////////////////////////////////////////////////////////////
/// Create a new shader from an effect source code
////////////////////////////////////////////////////////////
sfShader* sfShader_CreateFromMemory(const char* effect)
{
    sfShader* shader = new sfShader;

    if (!shader->This.LoadFromMemory(effect))
    {
        delete shader;
        shader = NULL;
    }

    return shader;
}


////////////////////////////////////////////////////////////
/// Copy an existing clock
////////////////////////////////////////////////////////////
sfShader* sfShader_Copy(sfShader* shader)
{
    CSFML_CHECK_RETURN(shader, NULL);

    return new sfShader(*shader);
}


////////////////////////////////////////////////////////////
/// Destroy an existing shader
////////////////////////////////////////////////////////////
void sfShader_Destroy(sfShader* shader)
{
    delete shader;
}


////////////////////////////////////////////////////////////
/// Change a parameter of a shader (1 float)
////////////////////////////////////////////////////////////
void sfShader_SetParameter1(sfShader* shader, const char* name, float x)
{
    CSFML_CALL(shader, SetParameter(name, x))
}


////////////////////////////////////////////////////////////
/// Change a parameter of a shader (2 floats)
////////////////////////////////////////////////////////////
void sfShader_SetParameter2(sfShader* shader, const char* name, float x, float y)
{
    CSFML_CALL(shader, SetParameter(name, x, y))
}


////////////////////////////////////////////////////////////
/// Change a parameter of a shader (3 floats)
////////////////////////////////////////////////////////////
void sfShader_SetParameter3(sfShader* shader, const char* name, float x, float y, float z)
{
    CSFML_CALL(shader, SetParameter(name, x, y, z))
}


////////////////////////////////////////////////////////////
/// Change a parameter of a shader (4 floats)
////////////////////////////////////////////////////////////
void sfShader_SetParameter4(sfShader* shader, const char* name, float x, float y, float z, float w)
{
    CSFML_CALL(shader, SetParameter(name, x, y, z, w))
}


////////////////////////////////////////////////////////////
/// Set a texture parameter in a shader
////////////////////////////////////////////////////////////
void sfShader_SetTexture(sfShader* shader, const char* name, const sfImage* texture)
{
    CSFML_CHECK(texture);
    CSFML_CALL(shader, SetTexture(name, *texture->This))
}


////////////////////////////////////////////////////////////
/// Set the current texture parameter in a shader
////////////////////////////////////////////////////////////
void sfShader_SetCurrentTexture(sfShader* shader, const char* name)
{
    CSFML_CALL(shader, SetCurrentTexture(name))
}


////////////////////////////////////////////////////////////
/// Bind a shader for rendering
////////////////////////////////////////////////////////////
void sfShader_Bind(const sfShader* shader)
{
    CSFML_CALL(shader, Bind())
}


////////////////////////////////////////////////////////////
/// Unbind a shader
////////////////////////////////////////////////////////////
void sfShader_Unbind(const sfShader* shader)
{
    CSFML_CALL(shader, Unbind())
}


////////////////////////////////////////////////////////////
/// Tell whether or not the system supports shaders
////////////////////////////////////////////////////////////
sfBool sfShader_IsAvailable(void)
{
    return sf::Shader::IsAvailable() ? sfTrue : sfFalse;
}
