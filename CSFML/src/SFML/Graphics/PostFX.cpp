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
#include <SFML/Graphics/PostFX.h>
#include <SFML/Graphics/PostFXStruct.h>
#include <SFML/Graphics/ImageStruct.h>
#include <SFML/Internal.h>


////////////////////////////////////////////////////////////
/// Create a new post-fx from a file
////////////////////////////////////////////////////////////
sfPostFX* sfPostFX_CreateFromFile(const char* filename)
{
    sfPostFX* postFX = new sfPostFX;

    if (!postFX->This.LoadFromFile(filename))
    {
        delete postFX;
        postFX = NULL;
    }

    return postFX;
}


////////////////////////////////////////////////////////////
/// Create a new post-fx from an effect source code
////////////////////////////////////////////////////////////
sfPostFX* sfPostFX_CreateFromMemory(const char* effect)
{
    sfPostFX* postFX = new sfPostFX;

    if (!postFX->This.LoadFromMemory(effect))
    {
        delete postFX;
        postFX = NULL;
    }

    return postFX;
}


////////////////////////////////////////////////////////////
/// Destroy an existing post-fx
////////////////////////////////////////////////////////////
void sfPostFX_Destroy(sfPostFX* postFX)
{
    delete postFX;
}


////////////////////////////////////////////////////////////
/// Change a parameter of a post-fx (1 float)
////////////////////////////////////////////////////////////
void sfPostFX_SetParameter1(sfPostFX* postFX, const char* name, float x)
{
    CSFML_CALL(postFX, SetParameter(name, x))
}


////////////////////////////////////////////////////////////
/// Change a parameter of a post-fx (2 floats)
////////////////////////////////////////////////////////////
void sfPostFX_SetParameter2(sfPostFX* postFX, const char* name, float x, float y)
{
    CSFML_CALL(postFX, SetParameter(name, x, y))
}


////////////////////////////////////////////////////////////
/// Change a parameter of a post-fx (3 floats)
////////////////////////////////////////////////////////////
void sfPostFX_SetParameter3(sfPostFX* postFX, const char* name, float x, float y, float z)
{
    CSFML_CALL(postFX, SetParameter(name, x, y, z))
}


////////////////////////////////////////////////////////////
/// Change a parameter of a post-fx (4 floats)
////////////////////////////////////////////////////////////
void sfPostFX_SetParameter4(sfPostFX* postFX, const char* name, float x, float y, float z, float w)
{
    CSFML_CALL(postFX, SetParameter(name, x, y, z, w))
}


////////////////////////////////////////////////////////////
/// Set a texture parameter in a post-fx
////////////////////////////////////////////////////////////
void sfPostFX_SetTexture(sfPostFX* postFX, const char* name, sfImage* texture)
{
    CSFML_CALL(postFX, SetTexture(name, texture ? texture->This : NULL))
}


////////////////////////////////////////////////////////////
/// Tell whether or not the system supports post-effects
////////////////////////////////////////////////////////////
sfBool sfPostFX_CanUsePostFX()
{
    return sf::PostFX::CanUsePostFX() ? sfTrue : sfFalse;
}
