
////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include "QSFMLCanvas.hpp"

// Platform-specific headers
#ifdef Q_WS_X11
    #include <Qt/qx11info_x11.h>
    #include <X11/Xlib.h>
#endif


////////////////////////////////////////////////////////////
/// Construct the QSFMLCanvas
////////////////////////////////////////////////////////////
QSFMLCanvas::QSFMLCanvas(QWidget* Parent, const QPoint& Position, const QSize& Size, unsigned int FrameTime) :
QWidget       (Parent),
myInitialized (false)
{
    // Setup some states to allow direct rendering into the widget
    setAttribute(Qt::WA_PaintOnScreen);
    setAttribute(Qt::WA_OpaquePaintEvent);
    setAttribute(Qt::WA_NoSystemBackground);

    // Set strong focus to enable keyboard events to be received
    setFocusPolicy(Qt::StrongFocus);

    // Setup the widget geometry
    move(Position);
    resize(Size);

    // Setup the timer
    myTimer.setInterval(FrameTime);
}


////////////////////////////////////////////////////////////
/// Destructor
////////////////////////////////////////////////////////////
QSFMLCanvas::~QSFMLCanvas()
{
    // Nothing to do...
}


////////////////////////////////////////////////////////////
/// Notification for the derived class that moment is good
/// for doing initializations
////////////////////////////////////////////////////////////
void QSFMLCanvas::OnInit()
{
    // Nothing to do by default...
}


////////////////////////////////////////////////////////////
/// Notification for the derived class that moment is good
/// for doing its update and drawing stuff
////////////////////////////////////////////////////////////
void QSFMLCanvas::OnUpdate()
{
    // Nothing to do by default...
}


////////////////////////////////////////////////////////////
/// Return the paint engine used by the widget to draw itself
////////////////////////////////////////////////////////////
QPaintEngine* QSFMLCanvas::paintEngine() const
{
    return 0;
}


////////////////////////////////////////////////////////////
/// Called when the widget is shown ;
/// we use it to initialize our SFML window
////////////////////////////////////////////////////////////
void QSFMLCanvas::showEvent(QShowEvent*)
{
    if (!myInitialized)
    {
        // Under X11, we need to flush the commands sent to the server to ensure that
        // SFML will get an updated view of the windows
        #ifdef Q_WS_X11
            XFlush(QX11Info::display());
        #endif

        // Create the SFML window with the widget handle
        Create(winId());

        // Let the derived class do its specific stuff
        OnInit();

        // Setup the timer to trigger a refresh at specified framerate
        connect(&myTimer, SIGNAL(timeout()), this, SLOT(repaint()));
        myTimer.start();

        myInitialized = true;
    }
}


////////////////////////////////////////////////////////////
/// Called when the widget needs to be painted ;
/// we use it to display a new frame
////////////////////////////////////////////////////////////
void QSFMLCanvas::paintEvent(QPaintEvent*)
{
    // Let the derived class do its specific stuff
    OnUpdate();

    // Display on screen
    Display();
}
