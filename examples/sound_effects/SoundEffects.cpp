////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Graphics.hpp>

#include <SFML/Audio.hpp>

#include <algorithm>
#include <limits>
#include <memory>
#include <vector>

#include <cmath>


namespace
{
constexpr auto windowWidth  = 800u;
constexpr auto windowHeight = 600u;
constexpr auto pi           = 3.14159265359f;

std::filesystem::path resourcesDir()
{
#ifdef SFML_SYSTEM_IOS
    return "";
#else
    return "resources";
#endif
}
} // namespace


////////////////////////////////////////////////////////////
// Base class for effects
////////////////////////////////////////////////////////////
class Effect : public sf::Drawable
{
public:
    static void setFont(const sf::Font& font)
    {
        s_font = &font;
    }

    const std::string& getName() const
    {
        return m_name;
    }

    void update(float time, float x, float y)
    {
        onUpdate(time, x, y);
    }

    void draw(sf::RenderTarget& target, sf::RenderStates states) const override
    {
        onDraw(target, states);
    }

    void start()
    {
        onStart();
    }

    void stop()
    {
        onStop();
    }

    void handleKey(sf::Keyboard::Key key)
    {
        onKey(key);
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
    virtual void onUpdate(float time, float x, float y)                                 = 0;
    virtual void onDraw(sf::RenderTarget& target, const sf::RenderStates& states) const = 0;
    virtual void onStart()                                                              = 0;
    virtual void onStop()                                                               = 0;

    virtual void onKey(sf::Keyboard::Key)
    {
    }

    std::string m_name;

    // NOLINTNEXTLINE(readability-identifier-naming)
    static inline const sf::Font* s_font{nullptr};
};


////////////////////////////////////////////////////////////
// Surround Sound / Positional Audio Effect / Attenuation
////////////////////////////////////////////////////////////
class Surround : public Effect
{
public:
    Surround() : Effect("Surround / Attenuation")
    {
        m_listener.setPosition({(windowWidth - 20.f) / 2.f, (windowHeight - 20.f) / 2.f});
        m_listener.setFillColor(sf::Color::Red);

        // Load the music file
        if (!m_music.openFromFile(resourcesDir() / "doodle_pop.ogg"))
            sf::err() << "Failed to load " << (resourcesDir() / "doodle_pop.ogg").string() << std::endl;

        // Set the music to loop
        m_music.setLoop(true);

        // Set attenuation to a nice value
        m_music.setAttenuation(0.04f);
    }

    void onUpdate(float /*time*/, float x, float y) override
    {
        m_position = {windowWidth * x - 10.f, windowHeight * y - 10.f};
        m_music.setPosition({m_position.x, m_position.y, 0.f});
    }

    void onDraw(sf::RenderTarget& target, const sf::RenderStates& states) const override
    {
        auto statesCopy(states);
        statesCopy.transform = sf::Transform::Identity;
        statesCopy.transform.translate(m_position);

        target.draw(m_listener, states);
        target.draw(m_soundShape, statesCopy);
    }

    void onStart() override
    {
        // Synchronize listener audio position with graphical position
        sf::Listener::setPosition({m_listener.getPosition().x, m_listener.getPosition().y, 0.f});

        m_music.play();
    }

    void onStop() override
    {
        m_music.stop();
    }

private:
    sf::CircleShape m_listener{20.f};
    sf::CircleShape m_soundShape{20.f};
    sf::Vector2f    m_position;
    sf::Music       m_music;
};


////////////////////////////////////////////////////////////
// Pitch / Volume Effect
////////////////////////////////////////////////////////////
class PitchVolume : public Effect
{
public:
    PitchVolume() :
    Effect("Pitch / Volume"),
    m_pitchText(getFont(), "Pitch: " + std::to_string(m_pitch)),
    m_volumeText(getFont(), "Volume: " + std::to_string(m_volume))
    {
        // Load the music file
        if (!m_music.openFromFile(resourcesDir() / "doodle_pop.ogg"))
            sf::err() << "Failed to load " << (resourcesDir() / "doodle_pop.ogg").string() << std::endl;

        // Set the music to loop
        m_music.setLoop(true);

        // We don't care about attenuation in this effect
        m_music.setAttenuation(0.f);

        // Set initial pitch
        m_music.setPitch(m_pitch);

        // Set initial volume
        m_music.setVolume(m_volume);

        m_pitchText.setPosition({windowWidth / 2.f - 120.f, windowHeight / 2.f - 80.f});
        m_volumeText.setPosition({windowWidth / 2.f - 120.f, windowHeight / 2.f - 30.f});
    }

    void onUpdate(float /*time*/, float x, float y) override
    {
        m_pitch  = std::clamp(2.f * x, 0.f, 2.f);
        m_volume = std::clamp(100.f * (1.f - y), 0.f, 100.f);

        m_music.setPitch(m_pitch);
        m_music.setVolume(m_volume);

        m_pitchText.setString("Pitch: " + std::to_string(m_pitch));
        m_volumeText.setString("Volume: " + std::to_string(m_volume));
    }

    void onDraw(sf::RenderTarget& target, const sf::RenderStates& states) const override
    {
        target.draw(m_pitchText, states);
        target.draw(m_volumeText, states);
    }

    void onStart() override
    {
        // We set the listener position back to the default
        // so that the music is right on top of the listener
        sf::Listener::setPosition({0.f, 0.f, 0.f});

        m_music.play();
    }

    void onStop() override
    {
        m_music.stop();
    }

private:
    float     m_pitch{1.f};
    float     m_volume{100.f};
    sf::Text  m_pitchText;
    sf::Text  m_volumeText;
    sf::Music m_music;
};


////////////////////////////////////////////////////////////
// Attenuation Effect
////////////////////////////////////////////////////////////
class Attenuation : public Effect
{
public:
    Attenuation() : Effect("Attenuation"), m_text(getFont())
    {
        m_listener.setPosition({(windowWidth - 20.f) / 2.f, (windowHeight - 20.f) / 2.f + 100.f});
        m_listener.setFillColor(sf::Color::Red);

        m_soundShape.setFillColor(sf::Color::Magenta);

        // Sound cone parameters
        static constexpr auto coneHeight     = windowHeight * 2.f;
        static constexpr auto outerConeAngle = sf::degrees(120.f);
        static constexpr auto innerConeAngle = sf::degrees(30.f);

        // Set common properties of both cones
        for (auto* cone : {&m_soundConeOuter, &m_soundConeInner})
        {
            cone->setPointCount(3);
            cone->setPoint(0, {0.f, 0.f});
            cone->setPosition({20.f, 20.f});
        }

        m_soundConeOuter.setFillColor(sf::Color::Black);
        m_soundConeInner.setFillColor(sf::Color::Cyan);

        // Make each cone based on their angle and height
        static constexpr auto makeCone = [](auto& shape, const auto& angle)
        {
            const auto theta = sf::degrees(90.f) - (angle / 2);
            const auto x     = coneHeight / std::tan(theta.asRadians());

            shape.setPoint(1, {-x, coneHeight});
            shape.setPoint(2, {x, coneHeight});
        };

        makeCone(m_soundConeOuter, outerConeAngle);
        makeCone(m_soundConeInner, innerConeAngle);

        // Load the music file
        if (!m_music.openFromFile(resourcesDir() / "doodle_pop.ogg"))
            sf::err() << "Failed to load " << (resourcesDir() / "doodle_pop.ogg").string() << std::endl;

        // Set the music to loop
        m_music.setLoop(true);

        // Set attenuation factor
        m_music.setAttenuation(m_attenuation);

        // Set direction to face "downwards"
        m_music.setDirection({0.f, 1.f, 0.f});

        // Set cone
        m_music.setCone({innerConeAngle, outerConeAngle, 0.f});

        m_text.setString(
            "Attenuation factor dampens full volume of sound while within inner cone based on distance to "
            "listener.\nCone outer gain determines "
            "volume of sound while outside outer cone.\nWhen within outer cone, volume is linearly interpolated "
            "between "
            "inner and outer volumes.");
        m_text.setCharacterSize(18);
        m_text.setPosition({20.f, 20.f});
    }

    void onUpdate(float /*time*/, float x, float y) override
    {
        m_position = {windowWidth * x - 10.f, windowHeight * y - 10.f};
        m_music.setPosition({m_position.x, m_position.y, 0.f});
    }

    void onDraw(sf::RenderTarget& target, const sf::RenderStates& states) const override
    {
        auto statesCopy(states);

        statesCopy.transform = sf::Transform::Identity;
        statesCopy.transform.translate(m_position);

        target.draw(m_soundConeOuter, statesCopy);
        target.draw(m_soundConeInner, statesCopy);
        target.draw(m_soundShape, statesCopy);
        target.draw(m_listener, states);
        target.draw(m_text, states);
    }

    void onStart() override
    {
        // Synchronize listener audio position with graphical position
        sf::Listener::setPosition({m_listener.getPosition().x, m_listener.getPosition().y, 0.f});

        m_music.play();
    }

    void onStop() override
    {
        m_music.stop();
    }

private:
    sf::CircleShape m_listener{20.f};
    sf::CircleShape m_soundShape{20.f};
    sf::ConvexShape m_soundConeOuter;
    sf::ConvexShape m_soundConeInner;
    sf::Text        m_text;
    sf::Vector2f    m_position;
    sf::Music       m_music;

    float m_attenuation{0.01f};
};


////////////////////////////////////////////////////////////
// Tone Generator
////////////////////////////////////////////////////////////
class Tone : public sf::SoundStream, public Effect
{
public:
    Tone() :
    Effect("Tone Generator"),
    m_instruction(getFont(), "Press up and down arrows to change the current wave type"),
    m_currentType(getFont(), "Wave Type: Triangle"),
    m_currentAmplitude(getFont(), "Amplitude: 0.05"),
    m_currentFrequency(getFont(), "Frequency: 200 Hz")
    {
        m_instruction.setPosition({windowWidth / 2.f - 370.f, windowHeight / 2.f - 200.f});
        m_currentType.setPosition({windowWidth / 2.f - 150.f, windowHeight / 2.f - 100.f});
        m_currentAmplitude.setPosition({windowWidth / 2.f - 150.f, windowHeight / 2.f - 50.f});
        m_currentFrequency.setPosition({windowWidth / 2.f - 150.f, windowHeight / 2.f});

        sf::SoundStream::initialize(1, sampleRate, {sf::SoundChannel::Mono});
    }

    void onUpdate(float /*time*/, float x, float y) override
    {
        m_amplitude = std::clamp(0.2f * (1.f - y), 0.f, 0.2f);
        m_frequency = std::clamp(500.f * x, 0.f, 500.f);

        m_currentAmplitude.setString("Amplitude: " + std::to_string(m_amplitude));
        m_currentFrequency.setString("Frequency: " + std::to_string(m_frequency) + " Hz");
    }

    void onDraw(sf::RenderTarget& target, const sf::RenderStates& states) const override
    {
        target.draw(m_instruction, states);
        target.draw(m_currentType, states);
        target.draw(m_currentAmplitude, states);
        target.draw(m_currentFrequency, states);
    }

    void onStart() override
    {
        // We set the listener position back to the default
        // so that the tone is right on top of the listener
        sf::Listener::setPosition({0.f, 0.f, 0.f});

        play();
    }

    void onStop() override
    {
        SoundStream::stop();
    }

    void onKey(sf::Keyboard::Key key) override
    {
        auto ticks = 0;

        if (key == sf::Keyboard::Key::Down)
            ticks = 1; // Forward
        else if (key == sf::Keyboard::Key::Up)
            ticks = 3; // Reverse

        while (ticks--)
        {
            switch (m_type)
            {
                case Type::Sine:
                    m_type = Type::Square;
                    m_currentType.setString("Wave Type: Square");
                    break;
                case Type::Square:
                    m_type = Type::Triangle;
                    m_currentType.setString("Wave Type: Triangle");
                    break;
                case Type::Triangle:
                    m_type = Type::Sawtooth;
                    m_currentType.setString("Wave Type: Sawtooth");
                    break;
                case Type::Sawtooth:
                    m_type = Type::Sine;
                    m_currentType.setString("Wave Type: Sine");
                    break;
            }
        }
    }

private:
    bool onGetData(sf::SoundStream::Chunk& chunk) override
    {
        const auto period = 1.f / m_frequency;

        for (auto i = 0u; i < chunkSize; ++i)
        {
            auto value = 0.f;

            switch (m_type)
            {
                case Type::Sine:
                {
                    value = m_amplitude * std::sin(2 * pi * m_frequency * m_time);
                    break;
                }
                case Type::Square:
                {
                    value = m_amplitude *
                            (2 * (2 * std::floor(m_frequency * m_time) - std::floor(2 * m_frequency * m_time)) + 1);
                    break;
                }
                case Type::Triangle:
                {
                    value = 4 * m_amplitude / period *
                                std::abs(std::fmod(((std::fmod((m_time - period / 4), period)) + period), period) -
                                         period / 2) -
                            m_amplitude;
                    break;
                }
                case Type::Sawtooth:
                {
                    value = m_amplitude * 2 * (m_time / period - std::floor(0.5f + m_time / period));
                    break;
                }
            }

            m_sampleBuffer[i] = static_cast<std::int16_t>(std::lround(value * std::numeric_limits<std::int16_t>::max()));
            m_time += timePerSample;
        }

        chunk.sampleCount = chunkSize;
        chunk.samples     = m_sampleBuffer.data();

        return true;
    }

    void onSeek(sf::Time) override
    {
        // It doesn't make sense to seek in a tone generator
    }

    enum class Type
    {
        Sine,
        Square,
        Triangle,
        Sawtooth
    };

    static constexpr unsigned int sampleRate{44100};
    static constexpr std::size_t  chunkSize{sampleRate / 100};
    static constexpr float        timePerSample{1.f / static_cast<float>(sampleRate)};

    std::vector<std::int16_t> m_sampleBuffer = std::vector<std::int16_t>(chunkSize, 0);
    Type                      m_type{Type::Triangle};
    float                     m_amplitude{0.05f};
    float                     m_frequency{220};
    float                     m_time{};

    sf::Text m_instruction;
    sf::Text m_currentType;
    sf::Text m_currentAmplitude;
    sf::Text m_currentFrequency;
};


////////////////////////////////////////////////////////////
// Dopper Shift Effect
////////////////////////////////////////////////////////////
class Doppler : public sf::SoundStream, public Effect
{
public:
    Doppler() :
    Effect("Doppler Shift"),
    m_currentVelocity(getFont(), "Velocity: " + std::to_string(m_velocity)),
    m_currentFactor(getFont(), "Doppler Factor: " + std::to_string(m_factor))
    {
        m_listener.setPosition({(windowWidth - 20.f) / 2.f, (windowHeight - 20.f) / 2.f});
        m_listener.setFillColor(sf::Color::Red);

        m_position.y = (windowHeight - 20.f) / 2.f - 40.f;

        m_currentVelocity.setPosition({windowWidth / 2.f - 150.f, windowHeight * 3.f / 4.f - 50.f});
        m_currentFactor.setPosition({windowWidth / 2.f - 150.f, windowHeight * 3.f / 4.f});

        // Set attenuation to a nice value
        setAttenuation(0.05f);

        sf::SoundStream::initialize(1, sampleRate, {sf::SoundChannel::Mono});
    }

    void onUpdate(float time, float x, float y) override
    {
        m_velocity = std::clamp(150.f * (1.f - y), 0.f, 150.f);
        m_factor   = std::clamp(x, 0.f, 1.f);

        m_currentVelocity.setString("Velocity: " + std::to_string(m_velocity));
        m_currentFactor.setString("Doppler Factor: " + std::to_string(m_factor));

        m_position.x = std::fmod(time, 8.f) * windowWidth / 8.f;

        setPosition({m_position.x, m_position.y, 0.f});
        setVelocity({m_velocity, 0.f, 0.f});
        setDopplerFactor(m_factor);
    }

    void onDraw(sf::RenderTarget& target, const sf::RenderStates& states) const override
    {
        auto statesCopy(states);
        statesCopy.transform = sf::Transform::Identity;
        statesCopy.transform.translate(m_position - sf::Vector2f({20.f, 0.f}));

        target.draw(m_listener, states);
        target.draw(m_soundShape, statesCopy);
        target.draw(m_currentVelocity, states);
        target.draw(m_currentFactor, states);
    }

    void onStart() override
    {
        // Synchronize listener audio position with graphical position
        sf::Listener::setPosition({m_listener.getPosition().x, m_listener.getPosition().y, 0.f});

        play();
    }

    void onStop() override
    {
        SoundStream::stop();
    }

private:
    bool onGetData(sf::SoundStream::Chunk& chunk) override
    {
        const auto period = 1.f / m_frequency;

        for (auto i = 0u; i < chunkSize; ++i)
        {
            const auto value = m_amplitude * 2 * (m_time / period - std::floor(0.5f + m_time / period));

            m_sampleBuffer[i] = static_cast<std::int16_t>(std::lround(value * std::numeric_limits<std::int16_t>::max()));
            m_time += timePerSample;
        }

        chunk.sampleCount = chunkSize;
        chunk.samples     = m_sampleBuffer.data();

        return true;
    }

    void onSeek(sf::Time) override
    {
        // It doesn't make sense to seek in a tone generator
    }

    static constexpr unsigned int sampleRate{44100};
    static constexpr std::size_t  chunkSize{sampleRate / 100};
    static constexpr float        timePerSample{1.f / static_cast<float>(sampleRate)};

    std::vector<std::int16_t> m_sampleBuffer = std::vector<std::int16_t>(chunkSize, 0);
    float                     m_amplitude{0.05f};
    float                     m_frequency{220};
    float                     m_time{};

    float           m_velocity{0.f};
    float           m_factor{1.f};
    sf::CircleShape m_listener{20.f};
    sf::CircleShape m_soundShape{20.f};
    sf::Vector2f    m_position;
    sf::Text        m_currentVelocity;
    sf::Text        m_currentFactor;
};


////////////////////////////////////////////////////////////
/// Entry point of application
///
/// \return Application exit code
///
////////////////////////////////////////////////////////////
int main()
{
    // Create the main window
    sf::RenderWindow window(sf::VideoMode({windowWidth, windowHeight}),
                            "SFML Sound Effects",
                            sf::Style::Titlebar | sf::Style::Close);
    window.setVerticalSyncEnabled(true);

    // Load the application font and pass it to the Effect class
    sf::Font font;
    if (!font.loadFromFile(resourcesDir() / "tuffy.ttf"))
        return EXIT_FAILURE;
    Effect::setFont(font);

    // Create the effects
    Surround    surroundEffect;
    PitchVolume pitchVolumeEffect;
    Attenuation attenuationEffect;
    Tone        toneEffect;
    Doppler     dopplerEffect;

    const std::array<Effect*, 5> effects{
        &surroundEffect,
        &pitchVolumeEffect,
        &attenuationEffect,
        &toneEffect,
        &dopplerEffect,
    };

    std::size_t current = 0;

    effects[current]->start();

    // Create the messages background
    sf::Texture textBackgroundTexture;
    if (!textBackgroundTexture.loadFromFile(resourcesDir() / "text-background.png"))
        return EXIT_FAILURE;
    sf::Sprite textBackground(textBackgroundTexture);
    textBackground.setPosition({0.f, 520.f});
    textBackground.setColor(sf::Color(255, 255, 255, 200));

    // Create the description text
    sf::Text description(font, "Current effect: " + effects[current]->getName(), 20);
    description.setPosition({10.f, 530.f});
    description.setFillColor(sf::Color(80, 80, 80));

    // Create the instructions text
    sf::Text instructions(font, "Press left and right arrows to change the current effect", 20);
    instructions.setPosition({280.f, 555.f});
    instructions.setFillColor(sf::Color(80, 80, 80));

    // Start the game loop
    const sf::Clock clock;
    while (window.isOpen())
    {
        // Process events
        for (sf::Event event; window.pollEvent(event);)
        {
            // Close window: exit
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::KeyPressed)
            {
                switch (event.key.code)
                {
                    // Escape key: exit
                    case sf::Keyboard::Key::Escape:
                        window.close();
                        break;

                    // Left arrow key: previous effect
                    case sf::Keyboard::Key::Left:
                        effects[current]->stop();
                        if (current == 0)
                            current = effects.size() - 1;
                        else
                            --current;
                        effects[current]->start();
                        description.setString("Current effect: " + effects[current]->getName());
                        break;

                    // Right arrow key: next effect
                    case sf::Keyboard::Key::Right:
                        effects[current]->stop();
                        if (current == effects.size() - 1)
                            current = 0;
                        else
                            ++current;
                        effects[current]->start();
                        description.setString("Current effect: " + effects[current]->getName());
                        break;

                    default:
                        effects[current]->handleKey(event.key.code);
                        break;
                }
            }
        }

        // Update the current example
        const auto [x, y] = sf::Vector2f(sf::Mouse::getPosition(window)).cwiseDiv(sf::Vector2f(window.getSize()));
        effects[current]->update(clock.getElapsedTime().asSeconds(), x, y);

        // Clear the window
        window.clear(sf::Color(50, 50, 50));

        // Draw the current example
        window.draw(*effects[current]);

        // Draw the text
        window.draw(textBackground);
        window.draw(instructions);
        window.draw(description);

        // Finally, display the rendered frame on screen
        window.display();
    }

    // Stop effect so that tone generators don't have to keep generating data while being destroyed
    effects[current]->stop();
}
