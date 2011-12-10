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
        return myName;
    }

    void Load()
    {
        myIsLoaded = sf::Shader::IsAvailable() && OnLoad();
    }

    void Update(float time, float x, float y)
    {
        if (myIsLoaded)
            OnUpdate(time, x, y);
    }

    void Draw(sf::RenderTarget& target, sf::RenderStates states) const
    {
        if (myIsLoaded)
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
    myName(name),
    myIsLoaded(false)
    {
    }

private :

    // Virtual functions to be implemented in derived effects
    virtual bool OnLoad() = 0;
    virtual void OnUpdate(float time, float x, float y) = 0;
    virtual void OnDraw(sf::RenderTarget& target, sf::RenderStates states) const = 0;

private :

    std::string myName;
    bool myIsLoaded;
};

#endif // EFFECT_HPP
