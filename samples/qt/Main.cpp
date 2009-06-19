
////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include "QSFMLCanvas.hpp"
#include <QApplication>
#include <QVBoxLayout>
#include <QFrame>
#include <QLabel>


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
    MyCanvas(QWidget* Parent = NULL) :
    QSFMLCanvas(QSize(100, 100), 0, Parent)
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
        mySprite.SetCenter(mySprite.GetSize() / 2.f);
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
                mySprite.SetPosition(ConvertCoords(Event.MouseMove.X, Event.MouseMove.Y));
            }

            // Adjust the size of the default view when the widget is resized
            if (Event.Type == sf::Event::Resized)
            {
                GetDefaultView().SetFromRect(sf::FloatRect(0, 0, Event.Size.Width, Event.Size.Height));
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
    Label->setFont(QFont("courier new", 14, 1, false));

    // Create a SFML view inside the main frame
    MyCanvas* SFMLView = new MyCanvas(MainFrame);

    // Create the main layout
    QVBoxLayout* Layout = new QVBoxLayout;
    Layout->addWidget(Label, 0);
    Layout->addWidget(SFMLView, 1);
    MainFrame->setLayout(Layout);

    return App.exec();
}
