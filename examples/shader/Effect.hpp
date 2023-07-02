#pragma once

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Graphics.hpp>

#include <string>
#include <utility>

#include <cassert>


////////////////////////////////////////////////////////////
// Base class for effects
////////////////////////////////////////////////////////////
class Effect : public sf::Drawable
{
public:
    ~Effect() override = default;

    static void setFont(const sf::Font& font)
    {
        s_font = &font;
    }

    const std::string& getName() const
    {
        return m_name;
    }

    void load()
    {
        m_isLoaded = sf::Shader::isAvailable() && onLoad();
    }

    void update(float time, float x, float y)
    {
        if (m_isLoaded)
            onUpdate(time, x, y);
    }

    void draw(sf::RenderTarget& target, const sf::RenderStates& states) const override
    {
        if (m_isLoaded)
        {
            onDraw(target, states);
        }
        else
        {
            sf::Text error(getFont(), "Shader not\nsupported");
            error.setPosition({320.f, 200.f});
            error.setCharacterSize(36);
            target.draw(error, states);
        }
    }

protected:
    Effect(std::string name) : m_name(std::move(name))
    {
    }

    static const sf::Font& getFont()
    {
        assert(s_font != nullptr && "Cannot get font until setFont() is called");
        return *s_font;
    }

private:
    // Virtual functions to be implemented in derived effects
    virtual bool onLoad()                                                               = 0;
    virtual void onUpdate(float time, float x, float y)                                 = 0;
    virtual void onDraw(sf::RenderTarget& target, const sf::RenderStates& states) const = 0;

    std::string m_name;
    bool        m_isLoaded{};

    // NOLINTNEXTLINE(readability-identifier-naming)
    static const sf::Font* s_font;
};
