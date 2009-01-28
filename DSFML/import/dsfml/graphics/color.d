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

module dsfml.graphics.color;


/**
*   Color is an utility structure for manipulating colors
*/
struct Color
{
    /**
    *   Construct the color from its 4 RGBA components
    *   
    *   Params:    
    *       r = Red component   (0 .. 255)
    *       g = Green component (0 .. 255)
    *       b = Blue component  (0 .. 255)
    *       a = Alpha component (0 .. 255) (255 by default)
    */
    static Color opCall(ubyte r, ubyte g, ubyte b, ubyte a = 255)
	{
		Color c;
        c.r = r;
        c.g = g;
        c.b = b;
        c.a = a;
        
        return c;
	}

    /**
     *  Operator == and != overload to compare two colors    
     */    
    int opEquals(Color color2)
    {
        return 
            (r == color2.r) 
            && (g == color2.g) 
            && (b == color2.b) 
            && (a == color2.a);
    }
    /**
    *   Operator + overload to add two colors
    */
    Color opAdd(Color color2)
    {
        ubyte r = this.r + color2.r > 255 ? 255 : this.r + color2.r;
        ubyte g = this.g + color2.g > 255 ? 255 : this.g + color2.g;
        ubyte b = this.b + color2.b > 255 ? 255 : this.b + color2.b;
        ubyte a = this.a + color2.a > 255 ? 255 : this.a + color2.a;
    
        return Color(r, g, b, a);
    }

    /**
    *   Operator += overload
    */        
    Color opAddAssign(Color color2)
    {
        this.r = this.r + color2.r > 255 ? 255 : this.r + color2.r;
        this.g = this.g + color2.g > 255 ? 255 : this.g + color2.g;
        this.b = this.b + color2.b > 255 ? 255 : this.b + color2.b;
        this.a = this.a + color2.a > 255 ? 255 : this.a + color2.a;
        
        return *this;
    }

    /**
    *   Operator * overload to modulate colors
    */        
    Color opMul(Color color2)
    {
        ubyte r = this.r * color2.r / 255;
        ubyte g = this.g * color2.g / 255;
        ubyte b = this.b * color2.b / 255;
        ubyte a = this.a * color2.a / 255;
        
        return Color(r, g, b, a);
    }

    /**
    *   Operator *= overload
    */        
    Color opMulAssign(Color color2)
    {
        this.r = this.r * color2.r / 255;
        this.g = this.g * color2.g / 255;
        this.b = this.b * color2.b / 255;
        this.a = this.a * color2.a / 255;
        
        return *this;
    }
     
    ubyte r;            /// Red component
    ubyte g;            /// Green component
    ubyte b;            /// Blue component
    ubyte a = 255;      /// Alpha (transparency) component
    
    static const Color BLACK = {0, 0, 0};           /// Black predefined color
    static const Color WHITE = {255, 255, 255};     /// White predefined color
    static const Color  RED = {255, 0, 0};          /// Red predefined color
    static const Color GREEN = {0, 255, 0};         /// Green predefined color
    static const Color BLUE = {0, 0, 255};          /// Blue predefined color
    static const Color YELLOW = {255, 0, 255};      /// Yellow predefined color
    static const Color MAGENTA = {255, 0, 255};     /// Magenta predefined color
    static const Color CYAN = {0, 255, 255};        /// Cyan predefined color
}
