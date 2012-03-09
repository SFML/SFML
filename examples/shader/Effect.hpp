#ifndef EFFECT_HPP
#define EFFECT_HPP

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Graphics.hpp>
#include <string>


////////////////////////////////////////////////////////////
// Base class for effects
////////////////////////////////////////////////////////////
class Effect : public sf::Drawable
{
public :

    virtual ~Effect()
    {
    }

    const std::string& GetName() const
    {
        return m_name;
    }

    void Load()
    {
        m_isLoaded = sf::Shader::IsAvailable() && OnLoad();
    }

    void Update(float time, float x, float y)
    {
        if (m_isLoaded)
            OnUpdate(time, x, y);
    }

    void Draw(sf::RenderTarget& target, sf::RenderStates states) const
    {
        if (m_isLoaded)
        {
            OnDraw(target, states);
        }
        else
        {
            sf::Text error("Shader not\nsupported");
            error.SetPosition(320.f, 200.f);
            error.SetCharacterSize(36);
            target.Draw(error, states);
        }
    }

protected :

    Effect(const std::string& name) :
    m_name(name),
    m_isLoaded(false)
    {
    }

private :

    // Virtual functions to be implemented in derived effects
    virtual bool OnLoad() = 0;
    virtual void OnUpdate(float time, float x, float y) = 0;
    virtual void OnDraw(sf::RenderTarget& target, sf::RenderStates states) const = 0;

private :

    std::string m_name;
    bool m_isLoaded;
};

#endif // EFFECT_HPP
