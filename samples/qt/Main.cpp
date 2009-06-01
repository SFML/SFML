
////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include "QSFMLCanvas.hpp"
#include <Qt/qapplication.h>
#include <Qt/qframe.h>
#include <Qt/qlabel.h>
#include <Qt/qevent.h>


////////////////////////////////////////////////////////////
/// Custom SFML canvas
////////////////////////////////////////////////////////////
class MyCanvas : public QSFMLCanvas
{
public :

    ////////////////////////////////////////////////////////////
    /// Construct the canvas
    ///
    ////////////////////////////////////////////////////////////
    MyCanvas(QWidget* Parent, const QPoint& Position, const QSize& Size) :
    QSFMLCanvas(Parent, Position, Size)
    {

    }

private :

    ////////////////////////////////////////////////////////////
    /// /see QSFMLCanvas::OnInit
    ///
    ////////////////////////////////////////////////////////////
    virtual void OnInit()
    {
        // Load the image
        myImage.LoadFromFile("datas/qt/sfml.png");

        // Setup the sprite
        mySprite.SetImage(myImage);
        mySprite.SetOrigin(mySprite.GetSize() / 2.f);
    }

    ////////////////////////////////////////////////////////////
    /// /see QSFMLCanvas::OnUpdate
    ///
    ////////////////////////////////////////////////////////////
    virtual void OnUpdate()
    {
        sf::Event Event;
        while (GetEvent(Event))
        {
            // Stick the sprite to the mouse cursor
            if (Event.Type == sf::Event::MouseMoved)
            {
                mySprite.SetX(Event.MouseMove.X);
                mySprite.SetY(Event.MouseMove.Y);
            }
        }

        // Rotate the sprite
        mySprite.Rotate(GetFrameTime() * 100.f);

        // Clear the view
        Clear(sf::Color(0, 128, 0));

        // Draw it
        Draw(mySprite);
    }

    ////////////////////////////////////////////////////////////
    /// Member data
    ////////////////////////////////////////////////////////////
    sf::Image  myImage;  ///< Some image to show
    sf::Sprite mySprite; ///< A sprite to display the image
};


////////////////////////////////////////////////////////////
/// Entry point of application
///
/// \return Application exit code
///
////////////////////////////////////////////////////////////
int main(int argc, char **argv)
{
    QApplication App(argc, argv);

    // Create the main frame
    QFrame* MainFrame = new QFrame;
    MainFrame->setWindowTitle("Qt SFML");
    MainFrame->resize(400, 400);
    MainFrame->show();

    // Create a label for showing some text
    QLabel* Label = new QLabel("This is a SFML window\nembedded into a Qt frame :", MainFrame);
    Label->move(20, 10);
    Label->setFont(QFont("courier new", 14, 1, false));
    Label->show();

    // Create a SFML view inside the main frame
    MyCanvas* SFMLView = new MyCanvas(MainFrame, QPoint(20, 60), QSize(360, 320));
    SFMLView->show();
        
    return App.exec();
}
