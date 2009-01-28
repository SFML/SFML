module sound3d;

import dsfml.system.all;
import dsfml.window.all;
import dsfml.graphics.all;
import dsfml.audio.all;

void main()
{
    //We create our window with a limit of 100 and a white backcolor.
    RenderWindow app = new RenderWindow (VideoMode(800, 600, 32), "Sound Spatialization Sample");
    app.useVerticalSync(true);
    
    Font f = new Font("Data/cheeseburger.ttf", 34);
    
    //Some instructions
    String s = new String("Click anywhere on screen to change listener position.\nPress + or - to modify the speed of the car."c, f);
    s.setPosition(20, 30);
    s.setColor(Color.BLACK);
    
    //We prepare our images and the sound
    char[][2] images = ["Data/bluerallyecarleft.bmp", "Data/bluerallyecarright.bmp"];
    Car c = new Car(images, "Data/car_idle.wav");
    
    int carSpeed = 100;
    
    //Set default position for the car and the listener
    c.setPosition(Vector2f(0, 300));
    SoundListener.setPosition(Vector2f(400, 300));
    
    c.startPlaying();
    
    //Start the main loop
    while (app.isOpened())
    {
        app.clear(Color.WHITE);
        Event evt;
        //The event loop
        while (app.getEvent(evt))
        {
            // if the window is closed, we can leave the game loop
            if (evt.Type == Event.EventType.CLOSED) 
                app.close();
            // we handle the click event to change listener position
            else if (evt.Type == Event.EventType.MOUSEBUTTONPRESSED && evt.MouseButton.Button == MouseButtons.LEFT)
            {
                Input i = app.getInput();
                SoundListener.setPosition(Vector2f(i.getMouseX(), i.getMouseY()));
            }
            // and eventual keys press
            else if (evt.Type == Event.EventType.KEYPRESSED)
            {
                //Change the car speed
                if (evt.Key.Code == KeyCode.ADD)
                {
                    carSpeed += 25;
                }
                else if (evt.Key.Code == KeyCode.SUBTRACT)
                {
                    carSpeed -= 25;
                }
            }
        }
        
        //We move constantly our car.
        c.move(Vector2f(app.getFrameTime() * carSpeed, 0));
    
        //Draw all the sprite and string on render window
        app.draw(s);
        app.draw(c.getSprite());
        app.draw(SoundListener.getSprite());
        
        //And finally display the window
        app.display();
    } 
    
    
}

// Encapsulate the listener position and the visor sprite.
// There is only one listener so all the methods are statics.
class SoundListener
{
    static Sprite s_crosshair;
    static Vector2f s_p;
    
    static this()
    {
        Image crosshairImg = new Image("Data/crosshair.tga");                
        crosshairImg.createMaskFromColor(Color.WHITE);
        
        s_crosshair = new Sprite(crosshairImg);
        s_crosshair.setCenter(Vector2f(s_crosshair.getSize().x / 2, s_crosshair.getSize().y / 2));
        
        //Listener.setTarget(1.f, 0.f, 0.f);
    }
    
    // Adjust position of the listener
    static void setPosition(Vector2f p)
    {
        Listener.setPosition(p.x, p.y, 5.f);
        s_crosshair.setPosition(p);
    }
    
    static Sprite getSprite()
    {
        return s_crosshair;
    }  
}


// Class encapsulating all data for our car
class Car
{
    Vector2f m_actual;
    Sprite m_sprite;
    Sound m_sound;
    bool reverse;
    Image[2] imgs;
    
    //Constructor with with a fixed size string array of image path, and a string for the sound path 
    this (char[][2] images, char[] soundFilename)
    {
        //load images and create filter
        imgs[0] = new Image(images[0]); imgs[1] = new Image(images[1]);
            
        foreach(img; imgs)
            img.createMaskFromColor(Color(97, 68, 43));
        
        m_sprite = new Sprite(imgs[0]);
        m_sprite.setCenter(Vector2f(m_sprite.getSize().x / 2, m_sprite.getSize().y / 2));
        
        SoundBuffer buff = new SoundBuffer(soundFilename);
        
        //load our sound with loop enabled
        m_sound = new Sound(buff, true);
        m_sound.setAttenuation(.05f);
    }
    
    // Begin the sound play
    void startPlaying()
    {
        m_sound.play();   
    }
    
    // Set the position of the car on the window
    // Used to setup the begin car window and sound location 
    void setPosition(Vector2f p)
    {
        m_sprite.setPosition(p);
        m_sound.setPosition(p.x, 0, p.y);
    }
    
    //Move the car (visual and sound position)
    //If the car leave the screen, we change the sprite image and reverse moving
    void move(Vector2f vec)
    {
        // if the car is beyond the right screen limit
        if (!reverse && m_sprite.getPosition().x > 850)
        {
            m_sprite.setImage(imgs[1]);
            reverse = true;
        }
        // same as above but for left limit
        else if (reverse && vec.x + m_sprite.getPosition().x < -50)
        {
            m_sprite.setImage(imgs[0]);
            reverse = false;
        }
        
        if (reverse)
            vec = -vec;
            
        m_sprite.move(vec);
        Vector2f pos = m_sprite.getPosition();
        m_sound.setPosition(pos.x , pos.y, 0);
    }

    Sprite getSprite()
    {
        return m_sprite;
    }
}







