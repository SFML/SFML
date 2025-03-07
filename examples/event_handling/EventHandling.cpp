////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Graphics.hpp>

#include <vector>


namespace
{
std::string vec2ToString(const sf::Vector2i vec2)
{
    return '(' + std::to_string(vec2.x) + ", " + std::to_string(vec2.y) + ')';
}
} // namespace


////////////////////////////////////////////////////////////
/// \brief Application class
///
////////////////////////////////////////////////////////////
class Application
{
public:
    ////////////////////////////////////////////////////////////
    Application()
    {
        m_window.setVerticalSyncEnabled(true);
        m_logText.setFillColor(sf::Color::White);
        m_handlerText.setFillColor(sf::Color::White);
        m_handlerText.setStyle(sf::Text::Bold);
        m_handlerText.setPosition({380.f, 260.f});
        m_instructions.setFillColor(sf::Color::White);
        m_instructions.setStyle(sf::Text::Bold);
        m_instructions.setPosition({380.f, 310.f});
    }

    // The visitor we pass to event->visit in the "Visitor" handler
    // Make sure all defined operator()s return the same type.
    // The operator()s can also have void return type if there is nothing to return.
    struct Visitor
    {
        explicit Visitor(Application& app) : application(app)
        {
        }

        std::optional<std::string> operator()(const sf::Event::Closed&)
        {
            application.m_window.close();
            return std::nullopt;
        }

        std::optional<std::string> operator()(const sf::Event::KeyPressed& keyPress)
        {
            // When the enter key is pressed, switch to the next handler type
            if (keyPress.code == sf::Keyboard::Key::Enter)
            {
                application.m_handlerType = HandlerType::Overload;
                application.m_handlerText.setString("Current Handler: Overload");
            }

            return "Key Pressed: " + sf::Keyboard::getDescription(keyPress.scancode);
        }

        std::optional<std::string> operator()(const sf::Event::MouseMoved& mouseMoved)
        {
            return "Mouse Moved: " + vec2ToString(mouseMoved.position);
        }

        std::optional<std::string> operator()(const sf::Event::MouseButtonPressed&)
        {
            return "Mouse Pressed";
        }

        std::optional<std::string> operator()(const sf::Event::TouchBegan& touchBegan)
        {
            return "Touch Began: " + vec2ToString(touchBegan.position);
        }

        // When defining a visitor, make sure all event types can be handled by it.
        // If you don't intend on exhaustively specifying an operator() for each
        // event type, you can provide a templated operator() that will be selected
        // by overload resolution when no other event type matches.
        template <typename T>
        std::optional<std::string> operator()(const T&)
        {
            // All unhandled events will end up here
            // application.m_log.emplace_back("Other Event");
            return std::nullopt;
        }

        Application& application;
    };

    ////////////////////////////////////////////////////////////
    void run()
    {
        // This example is for demonstration purposes only
        // All the following forms of event handling have equivalent behavior
        // In your own code you should decide which form of event handling
        // suits your needs best and use a single form of event handling
        while (m_window.isOpen())
        {
            if (m_handlerType == HandlerType::Classic)
            {
                // The "classical form" of event handling
                // Poll/Wait for events in a loop and handle them
                // individually based on their concrete type
                while (const std::optional event = m_window.pollEvent())
                {
                    if (event->is<sf::Event::Closed>())
                    {
                        m_window.close();
                        break;
                    }

                    if (const auto* keyPress = event->getIf<sf::Event::KeyPressed>())
                    {
                        m_log.emplace_back("Key Pressed: " + sf::Keyboard::getDescription(keyPress->scancode));

                        // When the enter key is pressed, switch to the next handler type
                        if (keyPress->code == sf::Keyboard::Key::Enter)
                        {
                            m_handlerType = HandlerType::Visitor;
                            m_handlerText.setString("Current Handler: Visitor");
                        }
                    }
                    else if (const auto* mouseMoved = event->getIf<sf::Event::MouseMoved>())
                    {
                        m_log.emplace_back("Mouse Moved: " + vec2ToString(mouseMoved->position));
                    }
                    else if (event->is<sf::Event::MouseButtonPressed>())
                    {
                        m_log.emplace_back("Mouse Pressed");
                    }
                    else if (const auto* touchBegan = event->getIf<sf::Event::TouchBegan>())
                    {
                        m_log.emplace_back("Touch Began: " + vec2ToString(touchBegan->position));
                    }
                    else
                    {
                        // All unhandled events will end up here
                        // m_log.emplace_back("Other Event");
                    }
                }
            }
            else if (m_handlerType == HandlerType::Visitor)
            {
                // Event Visitor
                // A visitor able to visit all event types is passed to the event
                // The visitor's defined operator()s can also return values
                while (const std::optional event = m_window.pollEvent())
                {
                    if (std::optional logMessage = event->visit(Visitor(*this)))
                        m_log.emplace_back(std::move(*logMessage));
                }
            }
            else if (m_handlerType == HandlerType::Overload)
            {
                // Overloaded visitation
                // A callable taking a concrete event type is provided per event type you want to handle
                m_window.handleEvents([&](const sf::Event::Closed&) { m_window.close(); },
                                      [&](const sf::Event::KeyPressed& keyPress)
                                      {
                                          m_log.emplace_back(
                                              "Key Pressed: " + sf::Keyboard::getDescription(keyPress.scancode));

                                          // When the enter key is pressed, switch to the next handler type
                                          if (keyPress.code == sf::Keyboard::Key::Enter)
                                          {
                                              m_handlerType = HandlerType::Generic;
                                              m_handlerText.setString("Current Handler: Generic");
                                          }
                                      },
                                      [&](const sf::Event::MouseMoved& mouseMoved)
                                      { m_log.emplace_back("Mouse Moved: " + vec2ToString(mouseMoved.position)); },
                                      [&](const sf::Event::MouseButtonPressed&) { m_log.emplace_back("Mouse Pressed"); },
                                      [&](const sf::Event::TouchBegan& touchBegan)
                                      { m_log.emplace_back("Touch Began: " + vec2ToString(touchBegan.position)); });

                // To handle unhandled events, just add the following lambda to the set of handlers
                // [&](const auto&) { m_log.emplace_back("Other Event"); }
            }
            else if (m_handlerType == HandlerType::Generic)
            {
                // Generic visitation
                // A generic callable is provided that can differentiate by deduced event type
                m_window.handleEvents(
                    [&](auto&& event)
                    {
                        // Remove reference and cv-qualifiers
                        using T = std::decay_t<decltype(event)>;

                        if constexpr (std::is_same_v<T, sf::Event::Closed>)
                        {
                            m_window.close();
                        }
                        else if constexpr (std::is_same_v<T, sf::Event::KeyPressed>)
                        {
                            m_log.emplace_back("Key Pressed: " + sf::Keyboard::getDescription(event.scancode));

                            // When the enter key is pressed, switch to the next handler type
                            if (event.code == sf::Keyboard::Key::Enter)
                            {
                                m_handlerType = HandlerType::Forward;
                                m_handlerText.setString("Current Handler: Forward");
                            }
                        }
                        else if constexpr (std::is_same_v<T, sf::Event::MouseMoved>)
                        {
                            m_log.emplace_back("Mouse Moved: " + vec2ToString(event.position));
                        }
                        else if constexpr (std::is_same_v<T, sf::Event::MouseButtonPressed>)
                        {
                            m_log.emplace_back("Mouse Pressed");
                        }
                        else if constexpr (std::is_same_v<T, sf::Event::TouchBegan>)
                        {
                            m_log.emplace_back("Touch Began: " + vec2ToString(event.position));
                        }
                        else
                        {
                            // All unhandled events will end up here
                            // m_log.emplace_back("Other Event");
                        }
                    });
            }
            else if (m_handlerType == HandlerType::Forward)
            {
                // Forward to other callable
                // In this case we forward it to our handle member functions
                // we defined for the concrete event types we want to handle
                // When choosing this method a default "catch-all" handler must
                // be available for unhandled events to be forwarded to
                // If you don't want to provide an empty "catch-all" handler
                // you will have to make sure (e.g. via if constexpr) that this
                // lambda doesn't attempt to call a member function that doesn't exist
                m_window.handleEvents([this](const auto& event) { handle(event); });
            }

            // Limit the log to 24 entries
            if (m_log.size() > 24u)
                m_log.erase(m_log.begin(), m_log.begin() + static_cast<int>(m_log.size() - 24u));

            // Draw the contents of the log to the window
            m_window.clear();

            for (std::size_t i = 0; i < m_log.size(); ++i)
            {
                m_logText.setPosition({50.f, static_cast<float>(i * 20) + 50.f});
                m_logText.setString(m_log[i]);
                m_window.draw(m_logText);
            }

            m_window.draw(m_handlerText);
            m_window.draw(m_instructions);
            m_window.display();
        }
    }

    // The following handle methods are called by the forwarding event handler implementation
    // A handle method is defined per event type you want to handle
    // To handle any other events that are left, the templated handle method will be called
    // Overload resolution will prefer the handle methods that fit the event type better
    // before falling back to the templated method
    void handle(const sf::Event::Closed&)
    {
        m_window.close();
    }

    void handle(const sf::Event::KeyPressed& keyPress)
    {
        m_log.emplace_back("Key Pressed: " + sf::Keyboard::getDescription(keyPress.scancode));

        // When the enter key is pressed, switch to the next handler type
        if (keyPress.code == sf::Keyboard::Key::Enter)
        {
            m_handlerType = HandlerType::Classic;
            m_handlerText.setString("Current Handler: Classic");
        }
    }

    void handle(const sf::Event::MouseMoved& mouseMoved)
    {
        m_log.emplace_back("Mouse Moved: " + vec2ToString(mouseMoved.position));
    }

    void handle(const sf::Event::MouseButtonPressed&)
    {
        m_log.emplace_back("Mouse Pressed");
    }

    void handle(const sf::Event::TouchBegan& touchBegan)
    {
        m_log.emplace_back("Touch Began: " + vec2ToString(touchBegan.position));
    }

    template <typename T>
    void handle(const T&)
    {
        // All unhandled events will end up here
        // m_log.emplace_back("Other Event");
    }

private:
    enum class HandlerType
    {
        Classic,
        Visitor,
        Overload,
        Generic,
        Forward
    };

    ////////////////////////////////////////////////////////////
    // Member data
    ////////////////////////////////////////////////////////////
    sf::RenderWindow m_window{sf::VideoMode({800u, 600u}), "SFML Event Handling", sf::Style::Titlebar | sf::Style::Close};
    const sf::Font           m_font{"resources/tuffy.ttf"};
    sf::Text                 m_logText{m_font, "", 20};
    sf::Text                 m_handlerText{m_font, "Current Handler: Classic", 24};
    sf::Text                 m_instructions{m_font, "Press Enter to change handler type", 24};
    std::vector<std::string> m_log;
    HandlerType              m_handlerType{HandlerType::Classic};
};


////////////////////////////////////////////////////////////
/// Entry point of application
///
/// \return Application exit code
///
////////////////////////////////////////////////////////////
int main()
{
    Application application;
    application.run();
}
