
////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include "wxSFMLCanvas.hpp"
#include <iostream>


////////////////////////////////////////////////////////////
/// Custom SFML canvas
////////////////////////////////////////////////////////////
class MyCanvas : public wxSFMLCanvas
{
public :

    ////////////////////////////////////////////////////////////
    /// Construct the canvas
    ///
    ////////////////////////////////////////////////////////////
    MyCanvas(wxWindow* Parent, wxWindowID Id, const wxPoint& Position, const wxSize& Size, long Style = 0) :
    wxSFMLCanvas(Parent, Id, Position, Size, Style)
    {
        // Load an image and assign it to our sprite
        myImage.LoadFromFile("datas/wxwidgets/sfml.png");
        mySprite.SetImage(myImage);
        mySprite.SetCenter(mySprite.GetSize() / 2.f);

        // Catch the mouse move event
        Connect(wxEVT_MOTION, wxMouseEventHandler(MyCanvas::OnMouseMove));
    }

private :

    ////////////////////////////////////////////////////////////
    /// /see wxSFMLCanvas::OnUpdate
    ///
    ////////////////////////////////////////////////////////////
    virtual void OnUpdate()
    {
        // Rotate the sprite
        if (GetInput().IsMouseButtonDown(sf::Mouse::Left))  mySprite.Rotate( GetFrameTime() * 50);
        if (GetInput().IsMouseButtonDown(sf::Mouse::Right)) mySprite.Rotate(-GetFrameTime() * 50);

        // Clear the view
        Clear(sf::Color(0, 128, 128));

        // Display the sprite in the view
        Draw(mySprite);
    }

    ////////////////////////////////////////////////////////////
    /// Function called when the mouse cursor moves
    ///
    ////////////////////////////////////////////////////////////
    void OnMouseMove(wxMouseEvent& Event)
    {
        // Make the sprite follow the mouse cursor
        mySprite.SetX(Event.GetX());
        mySprite.SetY(Event.GetY());
    }

    ////////////////////////////////////////////////////////////
    /// Member data
    ////////////////////////////////////////////////////////////
    sf::Image  myImage;  ///< Some image to load...
    sf::Sprite mySprite; ///< Something to draw...
};


////////////////////////////////////////////////////////////
/// Our main window
////////////////////////////////////////////////////////////
class MyFrame : public wxFrame
{
public :

    ////////////////////////////////////////////////////////////
    /// Default constructor : setup the window
    ///
    ////////////////////////////////////////////////////////////
    MyFrame() :
    wxFrame(NULL, wxID_ANY, wxT("SFML wxWidgets"), wxDefaultPosition, wxSize(440, 280))
    {
        // Let's create a SFML view
        new MyCanvas(this, wxID_ANY, wxPoint(20, 20), wxSize(400, 200));
    }
};


////////////////////////////////////////////////////////////
/// Our application class
////////////////////////////////////////////////////////////
class MyApplication : public wxApp
{
private :

    ////////////////////////////////////////////////////////////
    /// Initialize the application
    ///
    ////////////////////////////////////////////////////////////
    virtual bool OnInit()
    {
        // Create the main window
        MyFrame* MainFrame = new MyFrame;
        MainFrame->Show();

        return true;
    }
};

IMPLEMENT_APP(MyApplication);
