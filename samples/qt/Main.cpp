
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
    MyCanvas(QWidget* parent = NULL) :
    QSFMLCanvas(QSize(100, 100), 0, parent)
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
        sf::Event event;
        while (GetEvent(event))
        {
            // Stick the sprite to the mouse cursor
            if (event.Type == sf::Event::MouseMoved)
            {
                mySprite.SetPosition(ConvertCoords(event.MouseMove.X, event.MouseMove.Y));
            }

            // Adjust the size of the default view when the widget is resized
            if (event.Type == sf::Event::Resized)
            {
                GetDefaultView().Reset(sf::FloatRect(0, 0, event.Size.Width, event.Size.Height));
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
    QApplication application(argc, argv);

    // Create the main frame
    QFrame* mainFrame = new QFrame;
    mainFrame->setWindowTitle("Qt SFML");
    mainFrame->resize(400, 400);
    mainFrame->show();

    // Create a label for showing some text
    QLabel* label = new QLabel("This is a SFML window\nembedded into a Qt frame :", mainFrame);
    label->setFont(QFont("courier new", 14, 1, false));

    // Create a SFML view inside the main frame
    MyCanvas* SFMLView = new MyCanvas(mainFrame);

    // Create the main layout
    QVBoxLayout* layout = new QVBoxLayout;
    layout->addWidget(label, 0);
    layout->addWidget(SFMLView, 1);
    mainFrame->setLayout(layout);

    return application.exec();
}
