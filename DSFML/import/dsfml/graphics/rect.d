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

module dsfml.graphics.rect;

struct sfFloatRect
{
    float Left;
    float Top;
    float Right;
    float Bottom;
}

struct sfIntRect
{
    int Left;
    int Top;
    int Right;
    int Bottom;
}

version (Tango)
{
    import tango.core.Traits;
}
else
{
    template isIntegerType(T)
    {
        const bool isIntegerType = 
            is (T == byte) ||
            is (T == short) ||
            is (T == int) ||
            is (T == long);
    }

    template isRealType(T)
    {
        const bool isRealType =
            is (T == float) ||
            is (T == double) ||
            is (T == real);
    }
}    
/**
*   Rect is an utility class for manipulating rectangles.
*   Template parameter defines the type of coordinates (integer float, ...)
*/

class Rect (T)
{
    static if (!isIntegerType!(T) && !isRealType!(T))
    {
        static assert (0, "This type is not supported by Rectangle");
    }
    
    T min(T)(T i, T j)
    {
        return i < j ? i : j;
    }
    
    T max(T)(T i, T j)
    {
        return i > j ? i : j;
    }
    
    /**
    *   Default constructor
    */
    this()
    {
        m_Left = 0;
        m_Top = 0;
        m_Right = 0;
        m_Bottom = 0;
    }

    /**
    *   Construct the rectangle from its coordinates
    *
    *   Params:
    *       leftCoord = Left coordinate of the rectangle
    *       topCoord = Top coordinate of the rectangle
    *       rightCoord = Right coordinate of the rectangle
    *       bottomCoord = Bottom coordinate of the rectangle
    */
    this(T leftCoord, T topCoord, T rightCoord, T bottomCoord)
    {
        m_Left = leftCoord;
        m_Top = topCoord;
        m_Right = rightCoord;
        m_Bottom = bottomCoord;
    }

    /**
    *   Get the width of the rectangle
    *
    *   Returns:
    *       Width of rectangle
    */
    T getWidth() 
    {
        return m_Right - m_Left;
    }

    /**
    *   Get the height of the rectangle
    *
    *   Returns:
    *       Height of rectangle
    */
    T getHeight()
    {
        return m_Bottom - m_Top;
    }

    /**
    *   Move the whole rectangle by the given offset
    *
    *   Params:
    *       offsetX = Horizontal offset
    *       offsetY = Vertical offset
    */
    void offset(T offsetX, T offsetY)
    {
        m_Left   += offsetX;
        m_Right  += offsetX;
        m_Top    += offsetY;
        m_Bottom += offsetY;
    }

    /**
    *   Check if a point is inside the rectangle's area
    *
    *   Params:
    *       x = X coordinate of the point to test
    *       y = Y coordinate of the point to test
    *
    *   Returns:
    *       True if the point is inside
    */
    bool contains(T x, T y)
    {
        return (x >= m_Left) && (x <= m_Right) && (y >= m_Top) && (y <= m_Bottom);
    }

    /**
    *   Check intersection between two rectangles
    *
    *   Params:
    *       rectangle = Rectangle to test
    *       overlappingRect = Rectangle to be filled with overlapping rect (NULL by default)
    *
    *   Returns:
    *       True if rectangles overlap
    */
    bool intersects(Rect!(T) rectangle, out Rect!(T) overlappingRect = null)
    {
        // Compute overlapping rect
        Rect!(T) overlapping = new Rect!(T)(    
                                                max(m_Left,   rectangle.getLeft),
                                                max(m_Top,    rectangle.getTop),
                                                min(m_Right,  rectangle.getRight),
                                                min(m_Bottom, rectangle.getBottom)
                                            );
    
        // If overlapping rect is valid, then there is intersection
        if ((overlapping.getLeft() < overlapping.getRight() ) && (overlapping.getTop() < overlapping.getBottom()))
        {
            overlappingRect = overlapping;
            return true;
        }
        else
        {
            overlappingRect = new Rect!(T)();
            return false;
        }
    }
    
    /**
    *   Set left Coordinate   
    */     
    void setLeft(T left)
    {
        m_Left = left;
    }

    /**
    *   Set top Coordinate   
    */
    void setTop(T top)
    {
        m_Top = top;
    }

    /**
    *   Set right Coordinate   
    */
    void setRight(T right)
    {
        m_Right = right;
    }

    /**
    *   Set bottom Coordinate   
    */
    void setBottom(T bottom)
    {
        m_Bottom = bottom;
    }

    /**
    *   Get left Coordinate   
    */
    T getLeft()
    {
        return m_Left;
    }

    /**
    *   Get top Coordinate   
    */
    T getTop()
    {
        return m_Top;
    }

    /**
    *   Get right Coordinate   
    */
    T getRight()
    {
        return m_Right;
    }

    /**
    *   Get bottom Coordinate   
    */
    T getBottom()
    {
        return m_Bottom;
    }

package:
    sfFloatRect toCFloatRect()
    {
        return sfFloatRect(m_Left, m_Top, m_Right, m_Bottom);
    }
    
    sfIntRect toCIntRect()
    {
        return sfIntRect(cast(int)m_Left, cast(int)m_Top, cast(int)m_Right, cast(int)m_Bottom);
    }

private:
        
    T m_Left;   // Left coordinate of the rectangle
    T m_Top;    // Top coordinate of the rectangle
    T m_Right;  // Right coordinate of the rectangle
    T m_Bottom; // Bottom coordinate of the rectangle
}

///Alias
alias Rect!(int) IntRect;
///ditto
alias Rect!(float) FloatRect;



