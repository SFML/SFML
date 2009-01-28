////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007-2008 Laurent Gomila (laurent.gom@gmail.com)
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
#include <SFML/Graphics/PostFX.hpp>
#include <SFML/Internal.h>


// WARNING : this structure must always be the SAME as in Graphics/Image.h
struct sfImage
{
    sf::Image This;
};
struct sfPostFX
{
    sf::PostFX This;
};


////////////////////////////////////////////////////////////
/// Create a new post-fx from a file
////////////////////////////////////////////////////////////
sfPostFX* sfPostFX_CreateFromFile(const char* Filename)
{
    sfPostFX* PostFX = new sfPostFX;

    if (!PostFX->This.LoadFromFile(Filename))
    {
        delete PostFX;
        PostFX = NULL;
    }

    return PostFX;
}


////////////////////////////////////////////////////////////
/// Create a new post-fx from an effect source code
////////////////////////////////////////////////////////////
sfPostFX* sfPostFX_CreateFromMemory(const char* Effect)
{
    sfPostFX* PostFX = new sfPostFX;

    if (!PostFX->This.LoadFromMemory(Effect))
    {
        delete PostFX;
        PostFX = NULL;
    }

    return PostFX;
}


////////////////////////////////////////////////////////////
/// Destroy an existing post-fx
////////////////////////////////////////////////////////////
void sfPostFX_Destroy(sfPostFX* PostFX)
{
    delete PostFX;
}


////////////////////////////////////////////////////////////
/// Change a parameter of a post-fx (1 float)
////////////////////////////////////////////////////////////
void sfPostFX_SetParameter1(sfPostFX* PostFX, const char* Name, float X)
{
    CSFML_CALL(PostFX, SetParameter(Name, X))
}


////////////////////////////////////////////////////////////
/// Change a parameter of a post-fx (2 floats)
////////////////////////////////////////////////////////////
void sfPostFX_SetParameter2(sfPostFX* PostFX, const char* Name, float X, float Y)
{
    CSFML_CALL(PostFX, SetParameter(Name, X, Y))
}


////////////////////////////////////////////////////////////
/// Change a parameter of a post-fx (3 floats)
////////////////////////////////////////////////////////////
void sfPostFX_SetParameter3(sfPostFX* PostFX, const char* Name, float X, float Y, float Z)
{
    CSFML_CALL(PostFX, SetParameter(Name, X, Y, Z))
}


////////////////////////////////////////////////////////////
/// Change a parameter of a post-fx (4 floats)
////////////////////////////////////////////////////////////
void sfPostFX_SetParameter4(sfPostFX* PostFX, const char* Name, float X, float Y, float Z, float W)
{
    CSFML_CALL(PostFX, SetParameter(Name, X, Y, Z, W))
}


////////////////////////////////////////////////////////////
/// Set a texture parameter in a post-fx
////////////////////////////////////////////////////////////
void sfPostFX_SetTexture(sfPostFX* PostFX, const char* Name, sfImage* Texture)
{
    CSFML_CALL(PostFX, SetTexture(Name, Texture ? &Texture->This : NULL))
}


////////////////////////////////////////////////////////////
/// Tell whether or not the system supports post-effects
////////////////////////////////////////////////////////////
sfBool sfPostFX_CanUsePostFX()
{
    return sf::PostFX::CanUsePostFX() ? sfTrue : sfFalse;
}
