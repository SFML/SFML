
////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Audio/Listener.hpp>
#include <SFML/Audio/Music.hpp>
#include <SFML/Audio/Sound.hpp>
#include <SFML/Audio/SoundBuffer.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/VideoMode.hpp>

#include "imgui-SFML.h"
#include "imgui.h"
#include "imgui_stdlib.h"

#include <cmath>
#include <limits>
#include <memory>
#include <random>
#include <vector>

namespace
{
constexpr auto windowWidth  = 1600u;
constexpr auto windowHeight = 900u;
constexpr auto markerRadius = 40.f;
constexpr auto pi           = 3.14159265359;

std::filesystem::path resourcesDir()
{
#ifdef SFML_SYSTEM_IOS
    return "";
#else
    return "resources";
#endif
}

void errorMessage(const std::string& message)
{
    if (ImGui::BeginPopupModal("Error"))
    {
        ImGui::Text(message.c_str());
        ImGui::EndPopup();
    }
}

struct Marker
{
    Marker(const sf::SoundSource* theSoundSource,
           const sf::Vector2f*    theListenerPosition,
           const sf::Font&        font,
           const sf::Color&       color,
           const sf::String&      text) :
    soundSource(theSoundSource),
    listenerPosition(theListenerPosition),
    coneCenter({120.f, 1.f}),
    coneInner({100.f, 1.f}),
    coneOuter({100.f, 1.f}),
    marker(markerRadius),
    label(font, text, static_cast<unsigned int>(markerRadius))
    {
        coneCenter.setFillColor(sf::Color::Magenta);
        coneInner.setFillColor(sf::Color(255, 0, 127));
        coneOuter.setFillColor(sf::Color::Red);
        marker.setFillColor(color);
        label.setFillColor(sf::Color::Black);
        label.setStyle(sf::Text::Bold);
    }

    void draw(sf::RenderTarget& target)
    {
        auto listenerOffset = (listenerPosition && soundSource && soundSource->isRelativeToListener())
                                  ? *listenerPosition
                                  : sf::Vector2f();
        auto labelBounds    = label.getLocalBounds().getSize();
        labelBounds.x *= 0.5f;
        labelBounds.y *= 0.75f;

        target.draw(coneCenter);

        sf::Angle innerAngle;
        sf::Angle outerAngle;

        // Check if we are a SoundSource or the Listener
        if (soundSource)
        {
            innerAngle = soundSource->getCone().innerAngle;
            outerAngle = soundSource->getCone().outerAngle;
        }
        else
        {
            innerAngle = sf::Listener::getCone().innerAngle;
            outerAngle = sf::Listener::getCone().outerAngle;
        }

        if (innerAngle != sf::degrees(360))
        {
            coneInner.setRotation(coneCenter.getRotation() - innerAngle / 2);
            target.draw(coneInner);
            coneInner.setRotation(coneCenter.getRotation() + innerAngle / 2);
            target.draw(coneInner);
        }

        if (outerAngle != sf::degrees(360))
        {
            coneOuter.setRotation(coneCenter.getRotation() - outerAngle / 2);
            target.draw(coneOuter);
            coneOuter.setRotation(coneCenter.getRotation() + outerAngle / 2);
            target.draw(coneOuter);
        }

        target.draw(marker, sf::Transform().translate(-sf::Vector2f(markerRadius, markerRadius) + listenerOffset));
        target.draw(label, sf::Transform().translate(-labelBounds + listenerOffset));
    }

    const sf::Vector2f& getPosition() const
    {
        return marker.getPosition();
    }

    void setPosition(const sf::Vector2f& position)
    {
        coneCenter.setPosition(position);
        coneInner.setPosition(position);
        coneOuter.setPosition(position);
        marker.setPosition(position);
        label.setPosition(position);
    }

    sf::Angle getRotation() const
    {
        return coneCenter.getRotation();
    }

    void setRotation(const sf::Angle& angle)
    {
        coneCenter.setRotation(angle);
    }

    const sf::SoundSource* soundSource;
    const sf::Vector2f*    listenerPosition;
    sf::RectangleShape     coneCenter;
    sf::RectangleShape     coneInner;
    sf::RectangleShape     coneOuter;
    sf::CircleShape        marker;
    sf::Text               label;
};

struct Object
{
    Object(const sf::Font&     font,
           sf::SoundSource&    theSoundSource,
           const sf::Vector2f& listenerPosition,
           const sf::Color&    markerColor,
           const std::string&  markerPrefix) :
    index(
        []()
        {
            static std::size_t nextIndex{};
            return nextIndex++;
        }()),
    soundSource(theSoundSource),
    marker(&soundSource, &listenerPosition, font, markerColor, markerPrefix + std::to_string(index))
    {
        position = []()
        {
            static std::random_device                    rd;
            static std::mt19937                          rng(rd());
            static std::uniform_real_distribution<float> xDistribution(-100.f, 100.f);
            static std::uniform_real_distribution<float> yDistribution(-100.f, 100.f);
            return sf::Vector2f(xDistribution(rng), yDistribution(rng));
        }();
        rotation = sf::degrees(270);
        marker.setPosition(position);
        marker.setRotation(rotation);
    }

    virtual ~Object() = default;

    virtual void draw(sf::RenderTarget& target) = 0;

    void drawMarker(sf::RenderTarget& target)
    {
        marker.draw(target);
    }

    void drawPositionRotationControls()
    {
        ImGui::DragFloat2("Position", &position.x);

        if (auto radians = rotation.asRadians();
            ImGui::DragFloat("Rotation", &radians, 0.01f, static_cast<float>(-2 * pi), static_cast<float>(2 * pi)))
            rotation = sf::radians(radians);
    }

    void drawConeControls()
    {
        auto cone = soundSource.getCone();

        if (auto innerAngle = cone.innerAngle.asRadians();
            ImGui::DragFloat("Cone Inner", &innerAngle, 0.01f, 0.f, static_cast<float>(2 * pi)))
            cone.innerAngle = std::clamp(sf::radians(innerAngle), sf::degrees(0), cone.outerAngle);

        if (auto outerAngle = cone.outerAngle.asRadians();
            ImGui::DragFloat("Cone Outer", &outerAngle, 0.01f, 0.f, static_cast<float>(2 * pi)))
            cone.outerAngle = std::clamp(sf::radians(outerAngle), cone.innerAngle, sf::degrees(360));

        if (auto outerGain = cone.outerGain; ImGui::DragFloat("Outer Gain", &outerGain, 0.001f, 0.f, 1.f))
            cone.outerGain = outerGain;

        soundSource.setCone(cone);
    }

    void drawSoundSourceControls()
    {
        if (auto relative = soundSource.isRelativeToListener(); ImGui::Checkbox("Relative to Listener", &relative))
            soundSource.setRelativeToListener(relative);

        if (auto pitch = soundSource.getPitch(); ImGui::DragFloat("Pitch", &pitch, 0.01f, 0.f, 10.f))
            soundSource.setPitch(pitch);

        if (auto volume = soundSource.getVolume(); ImGui::DragFloat("Volume", &volume, 1.f, 0.f, 100.f))
            soundSource.setVolume(volume);

        if (auto attenuation = soundSource.getAttenuation();
            ImGui::DragFloat("Attenuation", &attenuation, 0.01f, 0.f, 10.f))
            soundSource.setAttenuation(attenuation);

        if (auto minDistance = soundSource.getMinDistance(); ImGui::DragFloat("Min. Distance", &minDistance))
            soundSource.setMinDistance(minDistance);
    }

    void drawPlayControls()
    {
        if (ImGui::Button("Play"))
            soundSource.play();
        ImGui::SameLine();
        if (ImGui::Button("Pause"))
            soundSource.pause();
        ImGui::SameLine();
        if (ImGui::Button("Stop"))
            soundSource.stop();
    }

    void update(sf::Time /*tick*/)
    {
        marker.setPosition(position);
        marker.setRotation(rotation);
        soundSource.setPosition({position.x, 0, position.y});
        soundSource.setDirection({std::cos(rotation.asRadians()), 0, std::sin(rotation.asRadians())});
    }

    std::size_t      index;
    sf::SoundSource& soundSource;
    Marker           marker;
    sf::Vector2f     position;
    sf::Angle        rotation;
    sf::Vector2f     amplitude;
    sf::Vector2f     frequency;
    float            time = 0.f;
};

struct Sound : Object
{
    Sound(const sf::Font& font, const sf::Vector2f& listenerPosition) :
    Object(font, sound, listenerPosition, sf::Color::Yellow, "S"),
    sound(buffer)
    {
        sound.setAttenuation(0.01f);
    }

    void draw(sf::RenderTarget& target) override
    {
        if (sound.getBuffer() && sound.getBuffer()->getDuration() != sf::Time::Zero)
            drawMarker(target);

        ImGui::SetNextWindowSize({0.f, 0.f});
        ImGui::Begin(("Sound " + std::to_string(index)).c_str());

        ImGui::InputText("File Path", &path);
        ImGui::SameLine();
        if (ImGui::Button("Load"))
        {
            sound.stop();

            if (!buffer.loadFromFile(path))
                errorMessage("Failed to load " + path);
        }

        if (sound.getBuffer() && sound.getBuffer()->getDuration() != sf::Time::Zero)
        {
            auto duration = sound.getBuffer() ? sound.getBuffer()->getDuration().asSeconds() : 0.f;
            if (auto offset = sound.getPlayingOffset().asSeconds();
                ImGui::SliderFloat("Playing Offset", &offset, 0.f, duration))
                sound.setPlayingOffset(sf::seconds(offset));

            drawPositionRotationControls();
            drawConeControls();
            drawSoundSourceControls();

            if (auto loop = sound.getLoop(); ImGui::Checkbox("Loop", &loop))
                sound.setLoop(loop);

            drawPlayControls();
        }

        ImGui::End();
    }

    std::string     path = (resourcesDir() / "ding.flac").string();
    sf::SoundBuffer buffer;
    sf::Sound       sound;
};


struct Music : Object
{
    Music(const sf::Font& font, const sf::Vector2f& listenerPosition) :
    Object(font, music, listenerPosition, sf::Color::Cyan, "M")
    {
        music.setAttenuation(0.01f);
    }

    void draw(sf::RenderTarget& target) override
    {
        if (music.getDuration() != sf::Time::Zero)
            drawMarker(target);

        ImGui::SetNextWindowSize({0.f, 0.f});
        ImGui::Begin(("Music " + std::to_string(index)).c_str());

        ImGui::InputText("File Path", &path);
        ImGui::SameLine();
        if (ImGui::Button("Load"))
        {
            music.stop();

            if (!music.openFromFile(path))
                errorMessage("Failed to load " + path);
        }

        if (music.getDuration() != sf::Time::Zero)
        {
            if (auto offset = music.getPlayingOffset().asSeconds();
                ImGui::SliderFloat("Playing Offset", &offset, 0.f, music.getDuration().asSeconds()))
                music.setPlayingOffset(sf::seconds(offset));

            drawPositionRotationControls();
            drawConeControls();
            drawSoundSourceControls();

            if (auto loop = music.getLoop(); ImGui::Checkbox("Loop", &loop))
                music.setLoop(loop);

            drawPlayControls();
        }

        ImGui::End();
    }

    std::string path  = (resourcesDir() / "doodle_pop.ogg").string();
    sf::Music   music;
};


struct Tone : sf::SoundStream, Object
{
    Tone(const sf::Font& font, const sf::Vector2f& listenerPosition) :
    Object(font, *this, listenerPosition, sf::Color::Green, "T"),
    sampleBuffer(chunkSize, 0)
    {
        initialize(1, sampleRate, {sf::SoundChannel::Mono});

        setAttenuation(0.01f);
    }

    void draw(sf::RenderTarget& target) override
    {
        drawMarker(target);

        ImGui::SetNextWindowSize({0.f, 0.f});
        ImGui::Begin(("Tone " + std::to_string(index)).c_str());

        if (ImGui::RadioButton("Sine", type == Type::Sine))
            type = Type::Sine;
        ImGui::SameLine();
        if (ImGui::RadioButton("Square", type == Type::Square))
            type = Type::Square;
        ImGui::SameLine();
        if (ImGui::RadioButton("Triangle", type == Type::Triangle))
            type = Type::Triangle;
        ImGui::SameLine();
        if (ImGui::RadioButton("Sawtooth", type == Type::Sawtooth))
            type = Type::Sawtooth;

        ImGui::DragFloat("Amplitude", &amplitude, 0.01f, 0.f, 1.f);
        ImGui::DragFloat("Frequency", &frequency, 1.f, 0.f, 1000.f);

        ImGui::PlotLines("Wave",
                         [](void* data, int sampleIndex)
                         {
                             const auto& samples = *static_cast<std::vector<std::int16_t>*>(data);
                             return static_cast<float>(samples[sampleIndex]);
                         },
                         &sampleBuffer,
                         static_cast<int>(sampleBuffer.size()),
                         0,
                         nullptr,
                         std::numeric_limits<std::int16_t>::min() * amplitude,
                         std::numeric_limits<std::int16_t>::max() * amplitude,
                         {0.f, 100.f});

        drawPositionRotationControls();
        drawConeControls();
        drawSoundSourceControls();
        drawPlayControls();

        ImGui::End();
    }

    bool onGetData(sf::SoundStream::Chunk& chunk) override
    {
        const auto            period = 1.0 / frequency;

        for (auto i = 0u; i < chunkSize; ++i)
        {
            auto value = 0.0;

            switch (type)
            {
                case Type::Sine:
                {
                    value = amplitude * std::sin(2 * pi * frequency * time);
                    break;
                }
                case Type::Square:
                {
                    value = amplitude *
                            (2 * (2 * std::floor(frequency * time) - std::floor(2 * frequency * time)) + 1);
                    break;
                }
                case Type::Triangle:
                {
                    value = 4 * amplitude / period *
                                std::abs(std::fmod(((std::fmod((time - period / 4), period)) + period), period) -
                                         period / 2) -
                            amplitude;
                    break;
                }
                case Type::Sawtooth:
                {
                    value = amplitude * 2 * (time / period - std::floor(0.5 + time / period));
                    break;
                }
            }

            sampleBuffer[i]  = static_cast<std::int16_t>(std::lround(value * std::numeric_limits<std::int16_t>::max()));
            time += timePerSample;
        }

        chunk.sampleCount = chunkSize;
        chunk.samples     = sampleBuffer.data();

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

    static constexpr unsigned int sampleRate = 44100;
    static constexpr std::size_t  chunkSize  = sampleRate / 100;
    std::vector<std::int16_t>     sampleBuffer;
    Type                          type          = Type::Triangle;
    float                         amplitude     = 0.05f;
    float                         frequency     = 220.f;
    double                        time          = 0.f;
    double                        timePerSample = 1.f / static_cast<double>(sampleRate);
};
}


////////////////////////////////////////////////////////////
/// Entry point of application
///
/// \return Application exit code
///
////////////////////////////////////////////////////////////
int main()
{
    sf::RenderWindow window(sf::VideoMode({windowWidth, windowHeight}), "SFML Sound Stage");
    window.setFramerateLimit(60);
    window.setView(sf::View(sf::Vector2f(0, 0), static_cast<sf::Vector2f>(window.getSize())));

    ImGui::SFML::Init(window);

    sf::Font font;
    if (!font.loadFromFile(resourcesDir() / "tuffy.ttf"))
        return EXIT_FAILURE;

    sf::Vector2f listenerPosition;

    Marker marker(nullptr, nullptr, font, sf::Color::White, "L");
    marker.setRotation(sf::degrees(270));

    std::vector<std::unique_ptr<Object>> objects;

    sf::Clock deltaClock;
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            ImGui::SFML::ProcessEvent(window, event);

            switch (event.type)
            {
                case sf::Event::Resized:
                    window.setView(sf::View(sf::Vector2f(0, 0), static_cast<sf::Vector2f>(window.getSize())));
                    break;
                case sf::Event::Closed:
                    window.close();
                    break;
                default:
                    break;
            }
        }

        const auto tick = deltaClock.restart();

        for (auto& object : objects)
            object->update(tick);

        ImGui::SFML::Update(window, tick);

        ImGui::SetNextWindowSize({0.f, 0.f});
        ImGui::Begin("Control");

        if (ImGui::DragFloat2("Listener Position", &listenerPosition.x))
        {
            marker.setPosition(listenerPosition);
            sf::Listener::setPosition({listenerPosition.x, 0, listenerPosition.y});
        }

        if (auto rotation = marker.getRotation().asRadians();
            ImGui::DragFloat("Listener Rotation", &rotation, 0.01f, static_cast<float>(-2 * pi), static_cast<float>(2 * pi)))
        {
            marker.setRotation(sf::radians(rotation));
            sf::Listener::setDirection({std::cos(rotation), 0, std::sin(rotation)});
        }

        auto cone = sf::Listener::getCone();

        if (auto innerAngle = cone.innerAngle.asRadians();
            ImGui::DragFloat("Listener Cone Inner", &innerAngle, 0.01f, 0.f, static_cast<float>(2 * pi)))
            cone.innerAngle = std::clamp(sf::radians(innerAngle), sf::degrees(0), cone.outerAngle);

        if (auto outerAngle = cone.outerAngle.asRadians();
            ImGui::DragFloat("Listener Cone Outer", &outerAngle, 0.01f, 0.f, static_cast<float>(2 * pi)))
            cone.outerAngle = std::clamp(sf::radians(outerAngle), cone.innerAngle, sf::degrees(360));

        if (auto outerGain = cone.outerGain; ImGui::DragFloat("Outer Gain", &outerGain, 0.001f, 0.f, 1.f))
            cone.outerGain = outerGain;

        sf::Listener::setCone(cone);

        if (ImGui::Button("Add Sound"))
            objects.emplace_back(std::make_unique<Sound>(font, listenerPosition));
        ImGui::SameLine();
        if (ImGui::Button("Add Music"))
            objects.emplace_back(std::make_unique<Music>(font, listenerPosition));
        ImGui::SameLine();
        if (ImGui::Button("Add Tone"))
            objects.emplace_back(std::make_unique<Tone>(font, listenerPosition));

        ImGui::End();

        window.clear();

        for (auto& object : objects)
            object->draw(window);

        marker.draw(window);

        ImGui::SFML::Render(window);

        window.display();
    }

    ImGui::SFML::Shutdown();

    return 0;
}
