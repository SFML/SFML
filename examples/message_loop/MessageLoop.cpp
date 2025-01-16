////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Graphics.hpp>

#include <vector>


////////////////////////////////////////////////////////////
/// Entry point of application
///
/// \return Application exit code
///
////////////////////////////////////////////////////////////
int main()
{
    auto window = sf::RenderWindow(sf::VideoMode({800u, 600u}), "SFML Event Loop");
    const sf::Font m_font{"resources/tuffy.ttf"};
    sf::Text       m_text(m_font);
    int            m_frame = 0;
    window.setFramerateLimit(60);

    window.eventLoop(
        [&](const sf::Event& evt)
        {
            if (evt.is<sf::Event::Idle>())
            {
                window.clear();
                m_frame += 1;
                m_text.setString("Frame: " + std::to_string(m_frame));
                window.draw(m_text);
                window.display();
            }
            else if (evt.is<sf::Event::Closed>())
            {
                window.close();
            }
        });
}
