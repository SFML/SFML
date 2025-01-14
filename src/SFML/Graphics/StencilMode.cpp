////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007-2025 Laurent Gomila (laurent@sfml-dev.org)
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
#include <SFML/Graphics/StencilMode.hpp>


namespace sf
{
////////////////////////////////////////////////////////////
StencilValue::StencilValue(int theValue) : value(static_cast<unsigned int>(theValue))
{
}


////////////////////////////////////////////////////////////
StencilValue::StencilValue(unsigned int theValue) : value(theValue)
{
}


////////////////////////////////////////////////////////////
bool operator==(const StencilMode& left, const StencilMode& right)
{
    return left.stencilUpdateOperation == right.stencilUpdateOperation &&
           left.stencilComparison == right.stencilComparison &&
           left.stencilReference.value == right.stencilReference.value &&
           left.stencilMask.value == right.stencilMask.value && left.stencilOnly == right.stencilOnly;
}


////////////////////////////////////////////////////////////
bool operator!=(const StencilMode& left, const StencilMode& right)
{
    return !(left == right);
}

} // namespace sf
