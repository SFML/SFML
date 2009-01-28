/*
*   DSFML - SFML Library binding in D language.
*   Copyright (C) 2008 Julien Dagorn (sirjulio13@gmail.com)
*
*   This software is provided 'as-is', without any express or
*   implied warranty. In no event will the authors be held
*   liable for any damages arising from the use of this software.
*
*   Permission is granted to anyone to use this software for any purpose,
*   including commercial applications, and to alter it and redistribute
*   it freely, subject to the following restrictions:
*
*   1.  The origin of this software must not be misrepresented;
*       you must not claim that you wrote the original software.
*       If you use this software in a product, an acknowledgment
*       in the product documentation would be appreciated but
*       is not required.
*
*   2.  Altered source versions must be plainly marked as such,
*       and must not be misrepresented as being the original software.
*
*   3.  This notice may not be removed or altered from any
*       source distribution.
*/

module dsfml.window.windowsettings;

/**
* Structure defining the creation settings of windows
*/
struct WindowSettings
{
    ///
    static WindowSettings opCall(uint depth = 24, uint stencil = 8, uint antialiasing = 0)
    {
        WindowSettings ret;
        
        ret.DepthBits = depth;
        ret.StencilBits = stencil;
        ret.AntialiasingLevel = antialiasing;
        
        return ret;
    }

    uint DepthBits;         /// Bits of the depth buffer
    uint StencilBits;       /// Bits of the stencil buffer
    uint AntialiasingLevel; /// Level of antialiasing
}
