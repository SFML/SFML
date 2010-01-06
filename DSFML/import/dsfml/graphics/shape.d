/*
*   DSFML - SFML Library wrapper for the D programming language.
*   Copyright (C) 2008 Julien Dagorn (sirjulio13@gmail.com)
*   Copyright (C) 2010 Andreas Hollandt
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

module dsfml.graphics.shape;

import dsfml.system.vector2;

import dsfml.graphics.blendmode;
import dsfml.graphics.color;
import dsfml.graphics.drawableimpl;

/**
*   Shape defines a drawable convex shape ; it also defines
*   helper functions to draw simple shapes like
*   lines, rectangles, circles, etc.
*/
class Shape : Drawableimpl!(sfShape)
{
    this()
    {
        super();
    }

    /**
    *   Add a point to the shape
    *
    *   Params:
    *       x = X position of the point
    *       y = Y position of the point
    *       col = Color of the point (white by default)
    *       outlineCol = Outline color of the point (black by default)
    */        
    void addPoint(float x, float y, Color col = Color.WHITE, Color outlineCol = Color.BLACK)
    {
        sfShape_AddPoint(m_ptr, x, y, col, outlineCol);
    }
 
    /**
    *   Add a point to the shape
    *
    *   Params:
    *       position = position of the point
    *       col = Color of the point (white by default)
    *       outlineCol = Outline color of the point (black by default)
    */        
    void addPoint(Vector2f position, Color col = Color.WHITE, Color outlineCol = Color.BLACK)
    {
        sfShape_AddPoint(m_ptr, position.x, position.x, col, outlineCol);
    }
 
    /**
    *   Enable or disable filling the shape.
    *   Fill is enabled by default.
    *   
    *   Params:
    *       enable = True to enable, false to disable                
    */        
    void enableFill(bool enable)
    {
        sfShape_EnableFill(m_ptr, enable);
    }

    /**
    *   Enable or disable drawing a shape outline.
    *   Outline is enabled by default
    *
    *   Params:
    *       enable = True to enable, false to disable
    */
    void enableOutline(bool enable)
    {
        sfShape_EnableOutline(m_ptr, enable);
    }

    /**
    *   Change the width of a shape outline
    *
    *   Params: 
    *       width = New width
    */
    void setOutlineWidth(float width)
    {
        sfShape_SetOutlineWidth(m_ptr, width);
    }

    /**
    *   Get the width of the shape outline
    *
    *   Returns:
    *       Current outline width
    *
    */
    float getOutlineWidth() 
    {
        return sfShape_GetOutlineWidth(m_ptr);
    }

    /**
    *   Get the number of points composing a shape
    *
    *   Returns:
    *       Total number of points
    */
    uint getNbPoints()
    {
        return sfShape_GetNbPoints(m_ptr);
    }

    /**
    *   Get a point of the shape
    *
    *   Params:    
    *       index = Index of the point
    *       
    *   Returns:
    *       position of the point            
    */
    Vector2f getPointPosition(uint index)
    {
        float x, y;
        sfShape_GetPointPosition(m_ptr, index, &x, &y);
        return Vector2f(x, y);
    }

    /**
    *   Set the position of a shape point
    *   
    *   Params:
    *       index = Index of the point
    *       position = New position of the point                
    */        
    void setPointPosition(uint index, Vector2f position)
    {
        sfShape_SetPointPosition(m_ptr, index, position.x, position.y);
    }

    /**
    *   Get the color of a shape's point
    *   
    *   Params:    
    *       index = Index of the point
    *       
    *   Returns:
    *       Color of the point             
    */
    Color getPointColor(uint index)
    {
        return sfShape_GetPointColor(m_ptr, index);
    }

    /**
    *   Set the color of a shape's point
    *   
    *   Params:    
    *       index = Index of the point
    *       color = new color of the point           
    */        
    void setPointColor(uint index, Color color)
    {
        sfShape_SetPointColor(m_ptr, index, color);
    }

    /**
    *   Get the outline color of a shape's point
    *   
    *   Params:
    *       index = Index of the point
    *       
    *   Returns:
    *       Color of the outline                      
    */        
    Color getPointOutlineColor(uint index)
    {
        return sfShape_GetPointOutlineColor(m_ptr, index);
    }

    /**
    *   Set the outline color of a shape's point
    *   
    *   Params:
    *       index = Index of the point
    *       color = new color of the point                
    */        
    void setPointOutlineColor(uint index, Color color)
    {
        sfShape_SetPointOutlineColor(m_ptr, index, color);
    }


 
    /**
    *   Create a shape made of a single line
    *
    *   Params:
    *       p1X, p1Y = Position of the first point
    *       p2X, p2Y = Position second point
    *       thickness = Line thickness
    *       col = Color used to draw the line
    *       outline = Outline width (0 by default)
    *       outlineCol = Color used to draw the outline (black by default)
    *   
    *   Returns:
    *       New line shape        
    */
    static Shape line(float p1X, float p1Y, float p2X, float p2Y, float thickness, Color col, float outline = 0.f, Color outlineCol = Color.BLACK)
    {
        
        return new Shape(sfShape_CreateLine(p1X, p1Y, p2X, p2Y, thickness, col, outline, outlineCol));
    }

    /**
    *   Create a shape made of a single rectangle
    *
    *   Params:    
    *       p1X = X position of the first point
    *       p1Y = Y position of the first point    
    *       p2X = X position second point 
    *       p2Y = Y position second point    
    *       col = Color used to fill the rectangle
    *       outline = Outline width (0 by default)
    *       outlineCol = Color used to draw the outline (black by default)
    *       
    *   Returns:
    *       new rectangle shape            
    */
    static Shape rectangle(float p1X, float p1Y, float p2X, float p2Y, Color col, float outline = 0.f, Color outlineCol = Color.BLACK)
    {
        return new Shape(sfShape_CreateRectangle(p1X, p1Y, p2X, p2Y, col, outline, outlineCol));
    }

    /**
    *   Create a shape made of a single circle
    *
    *   Params:    
    *       x = X position of the center
    *       y = Y position of the center    
    *       radius = Radius
    *       col = Color used to fill the circle
    *       outline = Outline width (0 by default)
    *       outlineCol = Color used to draw the outline (black by default)
    *
    *   Returns:
    *       new circle shape        
    */
    static Shape circle(float x, float y, float radius, Color col, float outline = 0.f, Color outlineCol = Color.BLACK)
    {
        return new Shape(sfShape_CreateCircle(x, y, radius, col, outline, outlineCol));
    }

private:
    this (void* ptr)
    {
        super(ptr);
    }
    
    extern (C)
    {
        typedef void* function(float, float, float, float, float, Color, float, Color) pf_sfShape_CreateLine;
        typedef void* function(float, float, float, float, Color, float, Color) pf_sfShape_CreateRectangle;
        typedef void* function(float, float, float, Color, float, Color) pf_sfShape_CreateCircle;
        typedef void function(void* Shape, float, float, Color, Color) pf_sfShape_AddPoint;
        typedef void function(void* Shape, int) pf_sfShape_EnableFill;
        typedef void function(void* Shape, int) pf_sfShape_EnableOutline;
        typedef void function (void* Shape, float Width) pf_sfShape_SetOutlineWidth;
        typedef float function (void* Shape) pf_sfShape_GetOutlineWidth;
        typedef uint function (void* Shape) pf_sfShape_GetNbPoints;
        typedef void function (void* Shape, uint Index, float* X, float* Y) pf_sfShape_GetPointPosition;
        typedef void function (void* Shape, uint Index, float X, float Y) pf_sfShape_SetPointPosition;
        typedef Color function (void* Shape, uint index) pf_sfShape_GetPointColor;
        typedef void function (void* Shape, uint index, Color color) pf_sfShape_SetPointColor;
        typedef Color function (void* Shape, uint index) pf_sfShape_GetPointOutlineColor;
        typedef void function (void* Shape, uint index, Color color) pf_sfShape_SetPointOutlineColor;
            
        static pf_sfShape_CreateLine sfShape_CreateLine;
        static pf_sfShape_CreateRectangle sfShape_CreateRectangle;
        static pf_sfShape_CreateCircle sfShape_CreateCircle;
        static pf_sfShape_AddPoint sfShape_AddPoint;
        static pf_sfShape_EnableFill sfShape_EnableFill;
        static pf_sfShape_EnableOutline sfShape_EnableOutline;
        static pf_sfShape_SetOutlineWidth sfShape_SetOutlineWidth;
        static pf_sfShape_GetOutlineWidth sfShape_GetOutlineWidth;
        static pf_sfShape_GetNbPoints sfShape_GetNbPoints;
        static pf_sfShape_GetPointPosition sfShape_GetPointPosition;
        static pf_sfShape_SetPointPosition sfShape_SetPointPosition;
        static pf_sfShape_GetPointColor sfShape_GetPointColor;
        static pf_sfShape_SetPointColor sfShape_SetPointColor;
        static pf_sfShape_GetPointOutlineColor sfShape_GetPointOutlineColor;
        static pf_sfShape_SetPointOutlineColor sfShape_SetPointOutlineColor;
    }

    static this()
    {
	debug
		DllLoader dll = DllLoader.load("csfml-graphics-d");
	else
		DllLoader dll = DllLoader.load("csfml-graphics");
        
        sfShape_CreateLine = cast(pf_sfShape_CreateLine)dll.getSymbol("sfShape_CreateLine");
        sfShape_CreateRectangle = cast(pf_sfShape_CreateRectangle)dll.getSymbol("sfShape_CreateRectangle");
        sfShape_CreateCircle = cast(pf_sfShape_CreateCircle)dll.getSymbol("sfShape_CreateCircle");
        sfShape_AddPoint = cast(pf_sfShape_AddPoint)dll.getSymbol("sfShape_AddPoint");
        sfShape_EnableFill = cast(pf_sfShape_EnableFill)dll.getSymbol("sfShape_EnableFill");
        sfShape_EnableOutline = cast(pf_sfShape_EnableOutline)dll.getSymbol("sfShape_EnableOutline");
        sfShape_SetOutlineWidth = cast(pf_sfShape_SetOutlineWidth)dll.getSymbol("sfShape_SetOutlineWidth");
        sfShape_GetOutlineWidth = cast(pf_sfShape_GetOutlineWidth)dll.getSymbol("sfShape_GetOutlineWidth");
        sfShape_GetNbPoints = cast(pf_sfShape_GetNbPoints)dll.getSymbol("sfShape_GetNbPoints");
        sfShape_GetPointPosition = cast(pf_sfShape_GetPointPosition)dll.getSymbol("sfShape_GetPointPosition");
        sfShape_SetPointPosition = cast(pf_sfShape_SetPointPosition)dll.getSymbol("sfShape_SetPointPosition");
        sfShape_GetPointColor = cast (pf_sfShape_GetPointColor)dll.getSymbol("sfShape_GetPointColor");
        sfShape_SetPointColor = cast (pf_sfShape_SetPointColor)dll.getSymbol("sfShape_SetPointColor");
        sfShape_GetPointOutlineColor = cast(pf_sfShape_GetPointOutlineColor)dll.getSymbol("sfShape_GetPointOutlineColor");
        sfShape_SetPointOutlineColor = cast(pf_sfShape_SetPointOutlineColor)dll.getSymbol("sfShape_SetPointOutlineColor");
    }
}
