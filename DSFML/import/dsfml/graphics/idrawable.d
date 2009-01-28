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

module dsfml.graphics.idrawable;

import dsfml.system.vector2;

import dsfml.graphics.color;
import dsfml.graphics.blendmode;
import dsfml.graphics.renderwindow;


/**
*   Interface for drawable object
*   
*   Shape, String and Sprite implement IDrawable
*/
interface IDrawable
{
    /**
    *   Set the left position of the object
    *   
    *   Params:
    *       x = New left coordinate            
    */
    void setX(float x);

    /**
    *   Set the top position of the object
    *   
    *   Params:
    *       y = New top coordinate            
    */
    void setY(float y);

  
    /**
    *   Set the position of the object
    *   
    *   Params: 
    *       x = New left coordinate
    *       y = New top coordinate            
    */    
    void setPosition(float x, float y);
    
    /**
    *   Set the position of the object
    *   
    *   Params:
    *       vec = new position             
    */        
    void setPosition(Vector2f vec);
    
    /**
    *   Set the horizontal scale of the object
    *   
    *   Params:
    *       scale = New horizontal scale (Strictly positive)
    */        
    void setScaleX(float scale);
        
    /**
    *   Set the vertical scale of the object
    *   
    *   Params:
    *       scale = New vertical scale (Strictly positive)            
    */
    void setScaleY(float scale);

    /**
    *   Set the scale of the object
    *   
    *   Params:
    *       scaleX = New horizontal scale
    *       scaleY = New vertical scale
    */
    void setScale(float scaleX, float scaleY);
           
    /**
    *   Set the scale of the object
    *   
    *   Params:
    *       scale = new scale            
    */
    void setScale(Vector2f scale);
    
    /**
    *   Set the center of the object, in coordinates relative to the
    *   top-left of the object (take 2 values).
    *   The default center is (0, 0)
    *
    *   Params:
    *       centerX : X coordinate of the center
    *       centerY : Y coordinate of the center
    */
    void setCenter(float centerX, float centerY);

    /**
    *   Set the center of the object, in coordinates relative to the
    *   top-left of the object (take a 2D vector).
    *   The default center is (0, 0)
    *
    *   Params:
    *      center : New center
    */
    void setCenter(Vector2f center);

             
    /**
    *   Set the rotation of the object
    *   
    *   Params: 
    *       angle = Angle of rotation, in degree            
    */        
    void setRotation(float angle);
        
    /**
    *   Set the color
    *   
    *   Params:
    *       c = New color            
    */           
    void setColor(Color c);
            
    /**
    *   Set the blending mode for the object.
    *   The default blend mode is Blend.Alpha
    *
    *   Params: 
    *       mode = New blending mode
    */
    void setBlendMode(BlendMode mode);

    /**
    *   Get the position of the object
    *
    *   Returns:
    *       Current position
    *
    */
    Vector2f getPosition();

    /**
    *   Get the current scale of the object
    *   
    *   Returns:
    *       Current scale            
    */       
    Vector2f getScale();
    
    /**
    *   Get the center of the object
    *
    *   Returns: 
    *       Current position of the center
    *
    */
    Vector2f getCenter();
    
    /**
    *   Get the rotation angle of the object
    *   
    *   Returns: 
    *       Angle of rotation, in degree            
    */        
    float getRotation();
    
    /**
    *   Get the color of the string
    *   
    *   Returns:
    *       Current color                
    */        
    Color getColor();

    /**
    *   Get the current blending mode
    *
    *   Returns: 
    *       Current blending mode
    */
    BlendMode getBlendMode();

    /**
    *   Rotate the object
    *   Angle is added to the current orientation of the objet    
    *   
    *   Params:
    *       angle = Angle of rotation in degree            
    */        
    void rotate(float angle);

    /**
    *   Move the object
    *   New offset is added to object current position   
    *    
    *   Params:
    *       offsetX = Offset on the X axis
    *       offsetY = Offset on the Y axis                
    */        
    void move(float offsetX, float offsetY);
    
    /**
    *   Move the object
    *   New offset is added to object current position   
    *    
    *   Params:
    *       offset = Amount of units to move the object of             
    */        
    void move(Vector2f offset);

    /**
    *   Set the scale of the object
    *   
    *   Params:
    *       scaleX = New horizontal scale (Strictly positive)
    *       scaleY = New vertical scale (Strictly positive)                
    */        
    void scale(float scaleX, float scaleY);
    
    /**
    *   Scale the object (take a 2D vector)
    *
    *   Params:
    *      factor = Scaling factors (both values must be strictly positive)
    */
    void scale(Vector2f factor);
    
    /**
    *   Transform a point from global coordinates into local coordinates
    *   (ie it applies the inverse of object's center, translation, rotation and scale to the point)
    *
    *   Params: 
    *       point = Point to transform
    *
    *   Returns: 
    *       Transformed point
    */        
    Vector2f tranformToLocal(Vector2f point);

    /**
    *   Transform a point from local coordinates into global coordinates
    *   (ie it applies the inverse of object's center, translation, rotation and scale to the point)
    *
    *   Params: 
    *       point = Point to transform
    *
    *   Returns: 
    *       Transformed point
    */        
    Vector2f tranformToGlobal(Vector2f point);
    
    /**
    *   Render the specific geometry of the object
    *   
    *   Params:
    *       window = Target into which render the object        
    */        
    void render(RenderWindow window);
}


