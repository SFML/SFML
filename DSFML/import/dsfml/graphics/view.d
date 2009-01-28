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

module dsfml.graphics.view;

import dsfml.graphics.rect;

import dsfml.system.common;
import dsfml.system.vector2;

/**
*   This class defines a view (position, size and zoom) ;
*   you can consider it as a camera
*/
class View : DSFMLObject
{
    /**
    *   Constructor
    *   
    *   Default view (1000 x 1000)        
    */
    this()
    {
        super(sfView_Create());
    }
     
   /**
   *   Constructor
   *   
   *    Params:
   *        center = center of the view
   *        halfsize = Half-size of the view (from center to corner)            
   */              
    this(Vector2f center, Vector2f halfsize)
    {
        super(sfView_CreateFromRect(sfFloatRect(center.x - halfsize.x, center.y - halfsize.y, center.x + halfsize.x, center.y + halfsize.y) ));
    }
    
    /**
    *   Constructor
    *   
    *   Params:
    *       rect = Rectangle defining the position and size of the view                 
    */        
    this(FloatRect rect)
    {
        super(sfView_CreateFromRect(rect.toCFloatRect()));
    }

    override void dispose()
    {
        sfView_Destroy(m_ptr);
    }
    
    /**
    *   Change the center of the view
    *   
    *   Params:
    *       x = X coordinates of the new center
    *       y = Y coordinates of the new center                     
    */        
    void setCenter(float x, float y)
    {
        sfView_SetCenter(m_ptr, x, y);
        m_isModified = true;
    }

    /**
    *   Change the center of the view
    *   
    *   Params:
    *       center = New center                     
    */        
    void setCenter(Vector2f center)
    {
        sfView_SetCenter(m_ptr, center.x, center.y);
        m_isModified = true;
    }

    /**
    *   Change the half-size of the view (take 2 values)
    *
    *   Params:
    *       halfWidth = New half-width
    *       halfHeight = New half-height
    */
    void setHalfSize(float halfWidth, float HalfHeight)
    {
        sfView_SetHalfSize(m_ptr, halfWidth, HalfHeight);
        m_isModified = true;
    }

    /**
    *   Change the half-size of the view (take 2 values)
    *
    *   Params:
    *       helfSize = New halfsize
    */
    void setHalfSize(Vector2f halfSize)
    {
        sfView_SetHalfSize(m_ptr, halfSize.x, halfSize.y);
        m_isModified = true;
    }

    /**
    * Rebuild the view from a rectangle
    *
    * Params:
    *     viewRect : Rectangle defining the position and size of the view
    */
    void setFromRect(FloatRect viewRect)
    {
        sfView_SetFromRect(m_ptr, viewRect.toCFloatRect());
        m_rect = viewRect;
    }

    /**
    *   Get the center of the view
    *
    *   Returns: 
    *       Center of the view
    */
    Vector2f GetCenter()
    {
        return Vector2f(sfView_GetCenterX(m_ptr), sfView_GetCenterY(m_ptr));
    }

    /**
    *   Get the halfsize of the view
    *
    *   Returns: 
    *       Halfsize of the view
    */
    Vector2f GetHalfSize()
    {
        return Vector2f(sfView_GetHalfSizeX(m_ptr), sfView_GetHalfSizeY(m_ptr));
    }

    /**
    *   Get the bounding retangle of the view
    */              
    FloatRect getRect()
    {
        if (m_isModified)
        {
            m_isModified = false;
            sfFloatRect cRect = sfView_GetRect(m_ptr);
            m_rect = new FloatRect(cRect.Left, cRect.Top, cRect.Right, cRect.Bottom);
        }
        return m_rect;
    }
    
    /**
    *   Move the view 
    *
    *   Params:
    *       offsetX = Offset to move the view, on X axis
    *       offsetY = Offset to move the view, on Y axis
    */
    void move(float offsetX, float offsetY)
    {
        sfView_Move(m_ptr, offsetX, offsetY);
        m_isModified = true;
    }

    /**
    *   Move the view 
    *
    *   Params:
    *       offset = offsetto move the view
    */
    void move(Vector2f offset)
    {
        sfView_Move(m_ptr, offset.x, offset.y);
        m_isModified = true;
    }
    
    /**
    *   Resize the view rectangle to simulate a zoom / unzoom effect
    *
    *   Params:
    *       factor = Zoom factor to apply, relative to the current zoom
    */        
    void zoom(float factor)
    {
        sfView_Zoom(m_ptr, factor);
        m_isModified = true;
    }

package:

    this(void* ptr, bool preventDelete)
    {
        super(ptr, preventDelete);
    } 

private:
    FloatRect m_rect;
    bool m_isModified = true;
    
    extern (C)
    {
        typedef void* function() pf_sfView_Create;
        typedef void* function(sfFloatRect) pf_sfView_CreateFromRect;
        typedef void function(void*) pf_sfView_Destroy;
        typedef void function(void*, float, float) pf_sfView_SetCenter;
        typedef void function(void*, float, float) pf_sfView_SetHalfSize;
        typedef void function(void*, sfFloatRect ViewRect) pf_sfView_SetFromRect;
        typedef float function(void*) pf_sfView_GetCenterX;
        typedef float function(void*) pf_sfView_GetCenterY;
        typedef float function(void*) pf_sfView_GetHalfSizeX;
        typedef float function(void*) pf_sfView_GetHalfSizeY;
        typedef sfFloatRect function(void*) pf_sfView_GetRect;
        typedef void function(void*, float, float) pf_sfView_Move;
        typedef void function(void*, float) pf_sfView_Zoom;

        static pf_sfView_Create sfView_Create;
        static pf_sfView_CreateFromRect sfView_CreateFromRect;
        static pf_sfView_Destroy sfView_Destroy;
        static pf_sfView_SetCenter sfView_SetCenter;
        static pf_sfView_SetHalfSize sfView_SetHalfSize;
        static pf_sfView_SetFromRect sfView_SetFromRect;
        static pf_sfView_GetCenterX sfView_GetCenterX;
        static pf_sfView_GetCenterY sfView_GetCenterY;
        static pf_sfView_GetHalfSizeX sfView_GetHalfSizeX;
        static pf_sfView_GetHalfSizeY sfView_GetHalfSizeY;
        static pf_sfView_GetRect sfView_GetRect;
        static pf_sfView_Move sfView_Move;
        static pf_sfView_Zoom sfView_Zoom;
    }

    static this()
    {
        DllLoader dll = DllLoader.load("csfml-graphics");
        
        sfView_Create = cast(pf_sfView_Create) dll.getSymbol("sfView_Create");
        sfView_CreateFromRect = cast(pf_sfView_CreateFromRect) dll.getSymbol("sfView_CreateFromRect");
        sfView_Destroy = cast(pf_sfView_Destroy) dll.getSymbol("sfView_Destroy");
        sfView_SetCenter = cast(pf_sfView_SetCenter) dll.getSymbol("sfView_SetCenter");
        sfView_SetHalfSize = cast(pf_sfView_SetHalfSize) dll.getSymbol("sfView_SetHalfSize");
        sfView_SetFromRect = cast(pf_sfView_SetFromRect) dll.getSymbol("sfView_SetFromRect");
        sfView_GetCenterX = cast(pf_sfView_GetCenterX) dll.getSymbol("sfView_GetCenterX");
        sfView_GetCenterY = cast(pf_sfView_GetCenterY) dll.getSymbol("sfView_GetCenterY");
        sfView_GetHalfSizeX = cast(pf_sfView_GetHalfSizeX) dll.getSymbol("sfView_GetHalfSizeX");
        sfView_GetHalfSizeY = cast(pf_sfView_GetHalfSizeY) dll.getSymbol("sfView_GetHalfSizeY");
        sfView_GetRect = cast(pf_sfView_GetRect) dll.getSymbol("sfView_GetRect");
        sfView_Move = cast(pf_sfView_Move) dll.getSymbol("sfView_Move");
        sfView_Zoom = cast(pf_sfView_Zoom) dll.getSymbol("sfView_Zoom");
    }
}


