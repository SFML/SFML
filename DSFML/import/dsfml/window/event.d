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

module dsfml.window.event;

/**
*   Definition of key codes for keyboard events
*   
*   $(B Possibles values:)$(BR)
*       Except letters and numbers, you can use :$(BR)
*           * LCONTROL, LSHIFT, LALT, LSYSTEM, RCONTROL, RSHIFT, RALT, RSYSTEM.$(BR)
*           * LBRACKET, RBRACKET, SEMICOLON, COMMA, PERIOD, QUOTE, SLASH, BACKSLASH, TILDE, EQUAL, DASH.$(BR)
*           * SPACE, RETURN, BACK, TAB, PAGEUP, PAGEDOWN, END, HOME, INSERT, DELETE.$(BR)
*           * ADD, SUBTRACT, MULTIPLY, DIVIDE, LEFT, RIGHT, UP, DOWN.$(BR)
*           * NUMPAD0, NUMPAD1, NUMPAD2, NUMPAD3, NUMPAD4, NUMPAD5, NUMPAD6, NUMPAD7, NUMPAD8, NUMPAD9.$(BR)
*           * F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12, F13, F14, F15.$(BR)
*/
enum KeyCode
{
        A = 'a',
        B = 'b',
        C = 'c',
        D = 'd',
        E = 'e',
        F = 'f',
        G = 'g',
        H = 'h',
        I = 'i',
        J = 'j',
        K = 'k',
        L = 'l',
        M = 'm',
        N = 'n',
        O = 'o',
        P = 'p',
        Q = 'q',
        R = 'r',
        S = 's',
        T = 't',
        U = 'u',
        V = 'v',
        W = 'w',
        X = 'x',
        Y = 'y',
        Z = 'z',
        NUM0 = '0',
        NUM1 = '1',
        NUM2 = '2',
        NUM3 = '3',
        NUM4 = '4',
        NUM5 = '5',
        NUM6 = '6',
        NUM7 = '7',
        NUM8 = '8',
        NUM9 = '9', 
        ESCAPE = 256,
        LCONTROL,         
        LSHIFT,         
        LALT,           
        LSYSTEM,        
        RCONTROL,       
        RSHIFT,         
        RALT,           
        RSYSTEM,        
        MENU,           
        LBRACKET,       
        RBRACKET,       
        SEMICOLON,      
        COMMA,          
        PERIOD,         
        QUOTE,          
        SLASH,          
        BACKSLASH,
        TILDE,          
        EQUAL,          
        DASH,           
        SPACE,
        RETURN,
        BACK,
        TAB,
        PAGEUP,
        PAGEDOWN,
        END,
        HOME,
        INSERT,
        DELETE,
        ADD,            
        SUBTRACT,       
        MULTIPLY,      
        DIVIDE,         
        LEFT,           
        RIGHT,          
        UP,             
        DOWN,          
        NUMPAD0,        
        NUMPAD1,        
        NUMPAD2,       
        NUMPAD3,       
        NUMPAD4,        
        NUMPAD5,        
        NUMPAD6,        
        NUMPAD7,        
        NUMPAD8,       
        NUMPAD9,        
        F1,
        F2,
        F3,
        F4,
        F5,
        F6,
        F7,
        F8,
        F9,
        F10,
        F11,
        F12,
        F13,
        F14,
        F15,
        PAUSE,
}


/**
*   Definition of button codes for mouse events 
*/
enum MouseButtons
{
    LEFT, ///
    RIGHT, ///
    MIDDLE, /// 
    XBUTTON1, ///
    XBUTTON2 ///
}



/**
*   Definition of joystick axis for joystick events
*/
enum JoyAxis
{
    AXISX, ///
    AXISY, ///
    AXISZ, ///
    AXISR, ///
    AXISU, ///
    AXISV, ///
    AXISPOV ///
}



/**
*   Event defines a system event and its parameters
*/
align(1) struct Event
{
    enum EventType
    {
        CLOSED,                 
        RESIZED,                
        LOSTFOCUS,              
        GAINEDFOCUS,            
        TEXTENTERED,            
        KEYPRESSED,             
        KEYRELEASED,            
        MOUSEWHEELMOVED,        
        MOUSEBUTTONPRESSED,     
        MOUSEBUTTONRELEASED,    
        MOUSEMOVED,
        MOUSEENTERED,
        MOUSELEFT,            
        JOYBUTTONPRESSED,       
        JOYBUTTONRELEASED,      
        JOYMOVED                
    }

    /**
    *   Enumeration of the different types of events. Accessing a value of another event that the one received (e.g. Event.Size.Width when receiving an KEYPRESSED event) will result in undefined behavior.
    *       $(UL
    *           $(LI CLOSED)  
    *           $(LI LOSTFOCUS)              
    *           $(LI GAINEDFOCUS)
    *           $(LI RESIZED
    *               $(UL
    *                   $(LI    Event.Size.Width : new Width, in pixels.)
    *                   $(LI    Event.Size.Height : new height, in pixels.)    
    *               )                    
    *           )          
    *           $(LI TEXTENTERED
    *               $(UL
    *                   $(LI    Event.Text.Unicode : dchar entered.)
    *               )    
    *           )            
    *           $(LI KEYPRESSED, KEYRELEASED
    *               $(UL
    *                   $(LI    Event.Key.Code      : Key code of the key.)
    *                   $(LI    Event.Key.Alt       : Alt pressed ?)
    *                   $(LI    Event.Key.Control   : Control pressed ?)
    *                   $(LI    Event.Key.Shift     : Shift pressed ?)
    *               )    
    *           )              
    *           $(LI MOUSEWHEELMOVED
    *               $(UL
    *                   $(LI    Event.MouseWheel.Delta : Wheel move (positive if forward, negative else.) ) 
    *               )    
    *           )           
    *           $(LI MOUSEBUTTONPRESSED, MOUSEBUTTONRELEASED
    *               $(UL
    *                   $(LI    Event.MouseButton.Button : Mouse button pressed.)
    *                   $(LI    Event.MouseButton.X : Cursor X position.)  
    *                   $(LI    Event.MouseButton.Y : Cursor X position.)              
    *               )    
    *           )              
    *           $(LI MOUSEMOVED
    *               $(UL
    *                   $(LI    Event.MouseMove.X   : Cursor X position. Local coordinates.)
    *                   $(LI    Event.MouseMove.Y   : Cursor Y position. Local coordinates.)        
    *               )    
    *           )
    *           $(LI MOUSEENTERED)
    *           $(LI MOUSELEFT)                   
    *           $(LI JOYBUTTONPRESSED, JOYBUTTONRELEASED
    *               $(UL
    *                   $(LI    Event.JoyButton.JoystickId  : Id of the joystick.)    
    *                   $(LI    Event.JoyButton.Button      : Joystick button pressed.)    
    *               )
    *           )    
    *           $(LI JOYMOVED
    *               $(UL
    *                   $(LI    Event.JoyMove.JoystickId    : Id of the joystick.)
    *                   $(LI    Event.JoyMove.Axis          : Moved axis.)
    *                   $(LI    Event.JoyMove.Position      : Actual position of the axis [-100, 100], except for POV [0, 360].)            
    *               )
    *           )                                 
    *       )              
    */    
    EventType Type;
    
    union
    {
        struct SText
        {
            dchar Unicode;
        }       
        SText Text;
        
        struct SKey
        {
            KeyCode Code;
            bool    Alt;
            byte[3] Filler1;            
            bool    Control;
            byte[3] Filler2;
            bool    Shift;
            byte[3] Filler3;
        }
        SKey Key;
    
        struct SMouseMove
        {
            int X;
            int Y;
        }
        SMouseMove MouseMove;
        
        struct SMouseButton
        {
            MouseButtons Button;
            int X;
            int Y;
        }
        SMouseButton MouseButton;
        
        struct SMouseWheel
        {
            int Delta;
        }
        SMouseWheel MouseWheel;
        
        struct SJoyMove
        {
            uint    JoystickId;
            JoyAxis Axis;
            float   Position;
        }
        SJoyMove JoyMove;
        
        struct SJoyButton
        {
            uint JoystickId;
            uint Button;
        }
        SJoyButton JoyButton;
        
        struct SSize
        {
            uint Width;
            uint Height;
        }
        SSize Size;
    }
} 


 
