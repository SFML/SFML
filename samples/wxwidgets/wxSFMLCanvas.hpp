
#ifndef WXSFMLCANVAS_HPP
#define WXSFMLCANVAS_HPP

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Graphics.hpp>
#include <wx/wx.h>


////////////////////////////////////////////////////////////
/// wxSFMLCanvas allows to run SFML in a wxWidgets control
////////////////////////////////////////////////////////////
class wxSFMLCanvas : public wxControl, public sf::RenderWindow
{
public :

    ////////////////////////////////////////////////////////////
    /// Construct the wxSFMLCanvas
    ///
    /// \param parent :   Parent of the control (NULL by default)
    /// \param id :       Identifier of the control (-1 by default)
    /// \param position : Position of the control (wxDefaultPosition by default)
    /// \param size :     Size of the control (wxDefaultSize by default)
    /// \param style :    Style of the control (0 by default)
    ///
    ////////////////////////////////////////////////////////////
    wxSFMLCanvas(wxWindow* parent = NULL, wxWindowID id = -1, const wxPoint& position = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = 0);

    ////////////////////////////////////////////////////////////
    /// Destructor
    ///
    ////////////////////////////////////////////////////////////
    virtual ~wxSFMLCanvas();

private :

    DECLARE_EVENT_TABLE()

    ////////////////////////////////////////////////////////////
    /// Notification for the derived class that moment is good
    /// for doing its update and drawing stuff
    ///
    ////////////////////////////////////////////////////////////
    virtual void OnUpdate();

    ////////////////////////////////////////////////////////////
    /// Called when the window is idle - we can refresh our
    /// SFML window
    ///
    ////////////////////////////////////////////////////////////
    void OnIdle(wxIdleEvent&);

    ////////////////////////////////////////////////////////////
    /// Called when the window is repainted - we can display our
    /// SFML window
    ///
    ////////////////////////////////////////////////////////////
    void OnPaint(wxPaintEvent&);

    ////////////////////////////////////////////////////////////
    /// Called when the window needs to draw its background
    ///
    ////////////////////////////////////////////////////////////
    void OnEraseBackground(wxEraseEvent&);
};


#endif // WXSFMLCANVAS_HPP
