#ifndef SFML_X11INPUTMANAGER_HPP
#define SFML_X11INPUTMANAGER_HPP

#include <SFML/System/String.hpp>
#include <SFML/Window/Keyboard.hpp>

#include <X11/X.h> // Keycode
#include <X11/XKBlib.h>

namespace sf {
namespace priv {

class X11InputManager
{
public:
    static X11InputManager& getInstance();
private:
    X11InputManager();
public:
    void initialize(Display* display);

    sf::Keyboard::Scancode unlocalize(sf::Keyboard::Key key) const;
    sf::Keyboard::Key localize(sf::Keyboard::Scancode code) const;

    bool isKeyPressed(sf::Keyboard::Key key) const;
    bool isKeyPressed(sf::Keyboard::Scancode code) const;

    sf::Keyboard::Key getKeyFromEvent(XKeyEvent& event) const;
    sf::Keyboard::Scancode getScancodeFromEvent(XKeyEvent& event) const;

    String getDescription(Keyboard::Scancode code) const;
private:
    void buildMapping();
    KeyCode SFtoKeyCode(sf::Keyboard::Key key) const;
    KeyCode SFtoKeyCode(sf::Keyboard::Scancode code) const;
    sf::Keyboard::Scancode keyCodeToSF(KeyCode code) const;

    KeyCode m_scancodeToKeycode[sf::Keyboard::ScanCodeCount];
    sf::Keyboard::Scancode m_keycodeToScancode[256];

    Display* m_display;
};

} // end of namespace priv
} // end of namespace sf

#endif
