////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Graphics.hpp>

#include <SFML/Audio.hpp>

#ifdef SFML_SYSTEM_IOS
#include <SFML/Main.hpp>
#endif

#include <sstream>

#include <cassert>
#include <cmath>


namespace
{
std::filesystem::path resourcesDir()
{
#ifdef SFML_SYSTEM_IOS
    return "";
#else
    return "resources";
#endif
}

// Get the C++ enumerator name of the given `sf::Keyboard::Key` value including `Key::` prefix
std::string keyIdentifier(sf::Keyboard::Key code)
{
    switch (code)
    {
#define CASE(code)                \
    case sf::Keyboard::Key::code: \
        return "Key::" #code
        CASE(Unknown);
        CASE(A);
        CASE(B);
        CASE(C);
        CASE(D);
        CASE(E);
        CASE(F);
        CASE(G);
        CASE(H);
        CASE(I);
        CASE(J);
        CASE(K);
        CASE(L);
        CASE(M);
        CASE(N);
        CASE(O);
        CASE(P);
        CASE(Q);
        CASE(R);
        CASE(S);
        CASE(T);
        CASE(U);
        CASE(V);
        CASE(W);
        CASE(X);
        CASE(Y);
        CASE(Z);
        CASE(Num0);
        CASE(Num1);
        CASE(Num2);
        CASE(Num3);
        CASE(Num4);
        CASE(Num5);
        CASE(Num6);
        CASE(Num7);
        CASE(Num8);
        CASE(Num9);
        CASE(Escape);
        CASE(LControl);
        CASE(LShift);
        CASE(LAlt);
        CASE(LSystem);
        CASE(RControl);
        CASE(RShift);
        CASE(RAlt);
        CASE(RSystem);
        CASE(Menu);
        CASE(LBracket);
        CASE(RBracket);
        CASE(Semicolon);
        CASE(Comma);
        CASE(Period);
        CASE(Apostrophe);
        CASE(Slash);
        CASE(Backslash);
        CASE(Grave);
        CASE(Equal);
        CASE(Hyphen);
        CASE(Space);
        CASE(Enter);
        CASE(Backspace);
        CASE(Tab);
        CASE(PageUp);
        CASE(PageDown);
        CASE(End);
        CASE(Home);
        CASE(Insert);
        CASE(Delete);
        CASE(Add);
        CASE(Subtract);
        CASE(Multiply);
        CASE(Divide);
        CASE(Left);
        CASE(Right);
        CASE(Up);
        CASE(Down);
        CASE(Numpad0);
        CASE(Numpad1);
        CASE(Numpad2);
        CASE(Numpad3);
        CASE(Numpad4);
        CASE(Numpad5);
        CASE(Numpad6);
        CASE(Numpad7);
        CASE(Numpad8);
        CASE(Numpad9);
        CASE(F1);
        CASE(F2);
        CASE(F3);
        CASE(F4);
        CASE(F5);
        CASE(F6);
        CASE(F7);
        CASE(F8);
        CASE(F9);
        CASE(F10);
        CASE(F11);
        CASE(F12);
        CASE(F13);
        CASE(F14);
        CASE(F15);
        CASE(Pause);
#undef CASE
    }

    throw std::runtime_error("invalid keyboard code");
}

// Get the C++ enumerator name of the given `sf::Keyboard::Scancode` value including `Scan::` prefix
std::string scancodeIdentifier(sf::Keyboard::Scancode scancode)
{
    switch (scancode)
    {
#define CASE(scancode)                 \
    case sf::Keyboard::Scan::scancode: \
        return "Scan::" #scancode
        CASE(Unknown);
        CASE(A);
        CASE(B);
        CASE(C);
        CASE(D);
        CASE(E);
        CASE(F);
        CASE(G);
        CASE(H);
        CASE(I);
        CASE(J);
        CASE(K);
        CASE(L);
        CASE(M);
        CASE(N);
        CASE(O);
        CASE(P);
        CASE(Q);
        CASE(R);
        CASE(S);
        CASE(T);
        CASE(U);
        CASE(V);
        CASE(W);
        CASE(X);
        CASE(Y);
        CASE(Z);
        CASE(Num1);
        CASE(Num2);
        CASE(Num3);
        CASE(Num4);
        CASE(Num5);
        CASE(Num6);
        CASE(Num7);
        CASE(Num8);
        CASE(Num9);
        CASE(Num0);
        CASE(Enter);
        CASE(Escape);
        CASE(Backspace);
        CASE(Tab);
        CASE(Space);
        CASE(Hyphen);
        CASE(Equal);
        CASE(LBracket);
        CASE(RBracket);
        CASE(Backslash);
        CASE(Semicolon);
        CASE(Apostrophe);
        CASE(Grave);
        CASE(Comma);
        CASE(Period);
        CASE(Slash);
        CASE(F1);
        CASE(F2);
        CASE(F3);
        CASE(F4);
        CASE(F5);
        CASE(F6);
        CASE(F7);
        CASE(F8);
        CASE(F9);
        CASE(F10);
        CASE(F11);
        CASE(F12);
        CASE(F13);
        CASE(F14);
        CASE(F15);
        CASE(F16);
        CASE(F17);
        CASE(F18);
        CASE(F19);
        CASE(F20);
        CASE(F21);
        CASE(F22);
        CASE(F23);
        CASE(F24);
        CASE(CapsLock);
        CASE(PrintScreen);
        CASE(ScrollLock);
        CASE(Pause);
        CASE(Insert);
        CASE(Home);
        CASE(PageUp);
        CASE(Delete);
        CASE(End);
        CASE(PageDown);
        CASE(Right);
        CASE(Left);
        CASE(Down);
        CASE(Up);
        CASE(NumLock);
        CASE(NumpadDivide);
        CASE(NumpadMultiply);
        CASE(NumpadMinus);
        CASE(NumpadPlus);
        CASE(NumpadEqual);
        CASE(NumpadEnter);
        CASE(NumpadDecimal);
        CASE(Numpad1);
        CASE(Numpad2);
        CASE(Numpad3);
        CASE(Numpad4);
        CASE(Numpad5);
        CASE(Numpad6);
        CASE(Numpad7);
        CASE(Numpad8);
        CASE(Numpad9);
        CASE(Numpad0);
        CASE(NonUsBackslash);
        CASE(Application);
        CASE(Execute);
        CASE(ModeChange);
        CASE(Help);
        CASE(Menu);
        CASE(Select);
        CASE(Redo);
        CASE(Undo);
        CASE(Cut);
        CASE(Copy);
        CASE(Paste);
        CASE(VolumeMute);
        CASE(VolumeUp);
        CASE(VolumeDown);
        CASE(MediaPlayPause);
        CASE(MediaStop);
        CASE(MediaNextTrack);
        CASE(MediaPreviousTrack);
        CASE(LControl);
        CASE(LShift);
        CASE(LAlt);
        CASE(LSystem);
        CASE(RControl);
        CASE(RShift);
        CASE(RAlt);
        CASE(RSystem);
        CASE(Back);
        CASE(Forward);
        CASE(Refresh);
        CASE(Stop);
        CASE(Search);
        CASE(Favorites);
        CASE(HomePage);
        CASE(LaunchApplication1);
        CASE(LaunchApplication2);
        CASE(LaunchMail);
        CASE(LaunchMediaSelect);
#undef CASE
    }

    throw std::runtime_error("invalid keyboard scancode");
}

////////////////////////////////////////////////////////////
// Entity showing keyboard events and real-time state on a keyboard
////////////////////////////////////////////////////////////
class KeyboardView : public sf::Drawable, public sf::Transformable
{
public:
    explicit KeyboardView(const sf::Font& font) : m_labels(sf::Keyboard::ScancodeCount, sf::Text(font, "", 14))
    {
        // Check all the scancodes are in the matrix exactly once
        {
            std::unordered_set<sf::Keyboard::Scancode> scancodesInMatrix;
            for (const auto& [cells, marginBottom] : m_matrix)
            {
                for (const auto& [scancode, size, marginRight] : cells)
                {
                    assert(scancodesInMatrix.count(scancode) == 0);
                    scancodesInMatrix.insert(scancode);
                }
            }
            assert(scancodesInMatrix.size() == sf::Keyboard::ScancodeCount);
        }

        // Initialize keys color and label
        forEachKey(
            [this](sf::Keyboard::Scancode scancode, const sf::FloatRect& rect)
            {
                const auto scancodeIndex = static_cast<std::size_t>(scancode);

                for (std::size_t vertexIndex = 0u; vertexIndex < 6u; ++vertexIndex)
                    m_triangles[6u * scancodeIndex + vertexIndex]
                        .color = sf::Keyboard::delocalize(sf::Keyboard::localize(scancode)) != scancode
                                     ? sf::Color::Red
                                     : sf::Color::White;

                sf::Text& label = m_labels[scancodeIndex];
                label.setString(sf::Keyboard::getDescription(scancode));
                label.setPosition(rect.position + rect.size / 2.f);

                if (rect.size.x < label.getLocalBounds().size.x + padding * 2.f + 2.f)
                {
                    sf::String string = label.getString();
                    string.replace(" ", "\n");
                    label.setString(string);
                }
                while (rect.size.x < label.getLocalBounds().size.x + padding * 2.f + 2.f)
                    label.setCharacterSize(label.getCharacterSize() - 2);

                const sf::FloatRect bounds = label.getLocalBounds();
                label.setOrigin({std::round(bounds.position.x + bounds.size.x / 2.f),
                                 std::round(static_cast<float>(label.getCharacterSize()) / 2.f)});
            });
    }

    void handle(const sf::Event& event)
    {
        // React to keyboard events by starting an animation
        if (const auto* keyPressed = event.getIf<sf::Event::KeyPressed>())
        {
            if (keyPressed->scancode != sf::Keyboard::Scan::Unknown)
                m_moveFactors[static_cast<std::size_t>(keyPressed->scancode)] = 1.f;
        }
        else if (const auto* keyReleased = event.getIf<sf::Event::KeyReleased>())
        {
            if (keyReleased->scancode != sf::Keyboard::Scan::Unknown)
                m_moveFactors[static_cast<std::size_t>(keyReleased->scancode)] = -1.f;
        }
    }

    void update(sf::Time frameTime)
    {
        // Animate m_moveFactors values linearly towards zero
        static constexpr sf::Time transitionDuration = sf::milliseconds(200);
        for (float& factor : m_moveFactors)
        {
            const float absoluteChange = std::min(std::abs(factor), frameTime / transitionDuration);
            factor += factor > 0.f ? -absoluteChange : absoluteChange;
        }

        // Update vertices positions from m_moveFactors and opacity from real-time keyboard state
        forEachKey(
            [this](sf::Keyboard::Scancode scancode, const sf::FloatRect& rect)
            {
                const auto scancodeIndex = static_cast<std::size_t>(scancode);

                static constexpr std::array square = {
                    sf::Vector2f(0.f, 0.f),
                    sf::Vector2f(1.f, 0.f),
                    sf::Vector2f(1.f, 1.f),
                    sf::Vector2f(0.f, 1.f),
                };
                static constexpr std::array cornerIndexes = {0u, 1u, 3u, 3u, 1u, 2u};

                const float        moveFactor = m_moveFactors[scancodeIndex];
                const sf::Vector2f move(0.f, 2.f * moveFactor * (1.f - std::abs(moveFactor)) * padding);

                const bool pressed = sf::Keyboard::isKeyPressed(scancode);

                for (std::size_t vertexIndex = 0u; vertexIndex < 6u; ++vertexIndex)
                {
                    sf::Vertex&                   vertex = m_triangles[6u * scancodeIndex + vertexIndex];
                    const sf::Vector2f&           corner = square[cornerIndexes[vertexIndex]];
                    static constexpr sf::Vector2f pad(padding, padding);
                    vertex.position = rect.position + pad + (rect.size - 2.f * pad).componentWiseMul(corner) + move;
                    vertex.color.a  = pressed ? 96 : 48;
                }
                m_labels[scancodeIndex].setPosition(rect.position + rect.size / 2.f + move);
            });
    }

private:
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override
    {
        states.transform *= getTransform();
        target.draw(m_triangles, states);
        for (const sf::Text& label : m_labels)
            target.draw(label, states);
    }

    // Template to iterate on scancodes and the corresponding computed rectangle in local coordinates
    template <typename F>
    void forEachKey(F function) const
    {
        sf::Vector2f position;
        for (const auto& [cells, marginBottom] : m_matrix)
        {
            for (const auto& [scancode, size, marginRight] : cells)
            {
                function(scancode, sf::FloatRect(position, size));
                position.x += size.x + marginRight;
            }
            position.x = 0.f;
            position.y += keySize + marginBottom;
        }
    }

    static constexpr float keySize = 54.f;
    static constexpr float padding = 4.f;

    struct Cell
    {
        Cell(sf::Keyboard::Scancode theScancode, sf::Vector2f sizeRatio = {1.f, 1.f}, float marginRightRatio = 0.f) :
        scancode(theScancode),
        size(sizeRatio * keySize),
        marginRight(marginRightRatio * keySize)
        {
        }

        Cell(sf::Keyboard::Scancode theScancode, float marginRightRatio) :
        Cell(theScancode, {1.f, 1.f}, marginRightRatio)
        {
        }

        sf::Keyboard::Scancode scancode;
        sf::Vector2f           size;
        float                  marginRight;
    };

    struct Row
    {
        Row(std::vector<Cell> theCells, float marginBottomRatio = 0.f) :
        cells(std::move(theCells)),
        marginBottom(marginBottomRatio * keySize)
        {
        }

        std::vector<Cell> cells;
        float             marginBottom;
    };

    const std::array<Row, 9> m_matrix{{
        {{{sf::Keyboard::Scan::Escape, 1},
          {sf::Keyboard::Scan::F1},
          {sf::Keyboard::Scan::F2},
          {sf::Keyboard::Scan::F3},
          {sf::Keyboard::Scan::F4, 0.5},
          {sf::Keyboard::Scan::F5},
          {sf::Keyboard::Scan::F6},
          {sf::Keyboard::Scan::F7},
          {sf::Keyboard::Scan::F8, 0.5},
          {sf::Keyboard::Scan::F9},
          {sf::Keyboard::Scan::F10},
          {sf::Keyboard::Scan::F11},
          {sf::Keyboard::Scan::F12, 0.5},
          {sf::Keyboard::Scan::PrintScreen},
          {sf::Keyboard::Scan::ScrollLock},
          {sf::Keyboard::Scan::Pause}},
         0.5},
        {{{sf::Keyboard::Scan::Grave}, //
          {sf::Keyboard::Scan::Num1},
          {sf::Keyboard::Scan::Num2},
          {sf::Keyboard::Scan::Num3},
          {sf::Keyboard::Scan::Num4},
          {sf::Keyboard::Scan::Num5},
          {sf::Keyboard::Scan::Num6},
          {sf::Keyboard::Scan::Num7},
          {sf::Keyboard::Scan::Num8},
          {sf::Keyboard::Scan::Num9},
          {sf::Keyboard::Scan::Num0},
          {sf::Keyboard::Scan::Hyphen},
          {sf::Keyboard::Scan::Equal},
          {sf::Keyboard::Scan::Backspace, {2, 1}, 0.5},
          {sf::Keyboard::Scan::Insert},
          {sf::Keyboard::Scan::Home},
          {sf::Keyboard::Scan::PageUp, 0.5},
          {sf::Keyboard::Scan::NumLock},
          {sf::Keyboard::Scan::NumpadDivide},
          {sf::Keyboard::Scan::NumpadMultiply},
          {sf::Keyboard::Scan::NumpadMinus}}},
        {{{sf::Keyboard::Scan::Tab, {1.5, 1}},
          {sf::Keyboard::Scan::Q},
          {sf::Keyboard::Scan::W},
          {sf::Keyboard::Scan::E},
          {sf::Keyboard::Scan::R},
          {sf::Keyboard::Scan::T},
          {sf::Keyboard::Scan::Y},
          {sf::Keyboard::Scan::U},
          {sf::Keyboard::Scan::I},
          {sf::Keyboard::Scan::O},
          {sf::Keyboard::Scan::P},
          {sf::Keyboard::Scan::LBracket},
          {sf::Keyboard::Scan::RBracket},
          {sf::Keyboard::Scan::Backslash, {1.5, 1}, 0.5},
          {sf::Keyboard::Scan::Delete},
          {sf::Keyboard::Scan::End},
          {sf::Keyboard::Scan::PageDown, 0.5},
          {sf::Keyboard::Scan::Numpad7},
          {sf::Keyboard::Scan::Numpad8},
          {sf::Keyboard::Scan::Numpad9},
          {sf::Keyboard::Scan::NumpadPlus}}},
        {{{sf::Keyboard::Scan::CapsLock, {1.75, 1}},
          {sf::Keyboard::Scan::A},
          {sf::Keyboard::Scan::S},
          {sf::Keyboard::Scan::D},
          {sf::Keyboard::Scan::F},
          {sf::Keyboard::Scan::G},
          {sf::Keyboard::Scan::H},
          {sf::Keyboard::Scan::J},
          {sf::Keyboard::Scan::K},
          {sf::Keyboard::Scan::L},
          {sf::Keyboard::Scan::Semicolon},
          {sf::Keyboard::Scan::Apostrophe},
          {sf::Keyboard::Scan::Enter, {2.25, 1}, 4},
          {sf::Keyboard::Scan::Numpad4},
          {sf::Keyboard::Scan::Numpad5},
          {sf::Keyboard::Scan::Numpad6},
          {sf::Keyboard::Scan::NumpadEqual}}},
        {{{sf::Keyboard::Scan::LShift, {1.25, 1}},
          {sf::Keyboard::Scan::NonUsBackslash},
          {sf::Keyboard::Scan::Z},
          {sf::Keyboard::Scan::X},
          {sf::Keyboard::Scan::C},
          {sf::Keyboard::Scan::V},
          {sf::Keyboard::Scan::B},
          {sf::Keyboard::Scan::N},
          {sf::Keyboard::Scan::M},
          {sf::Keyboard::Scan::Comma},
          {sf::Keyboard::Scan::Period},
          {sf::Keyboard::Scan::Slash},
          {sf::Keyboard::Scan::RShift, {2.75, 1}, 1.5},
          {sf::Keyboard::Scan::Up, 1.5},
          {sf::Keyboard::Scan::Numpad1},
          {sf::Keyboard::Scan::Numpad2},
          {sf::Keyboard::Scan::Numpad3},
          {sf::Keyboard::Scan::NumpadEnter, {1, 2}}}},
        {{{sf::Keyboard::Scan::LControl, {1.5, 1}},
          {sf::Keyboard::Scan::LSystem, {1.25, 1}},
          {sf::Keyboard::Scan::LAlt, {1.5, 1}},
          {sf::Keyboard::Scan::Space, {5.75, 1}},
          {sf::Keyboard::Scan::RAlt, {1.25, 1}},
          {sf::Keyboard::Scan::RSystem, {1.25, 1}},
          {sf::Keyboard::Scan::Menu, {1.25, 1}},
          {sf::Keyboard::Scan::RControl, {1.25, 1}, 0.5},
          {sf::Keyboard::Scan::Left},
          {sf::Keyboard::Scan::Down},
          {sf::Keyboard::Scan::Right, 0.5},
          {sf::Keyboard::Scan::Numpad0, {2, 1}},
          {sf::Keyboard::Scan::NumpadDecimal}},
         0.5},
        {{{sf::Keyboard::Scan::F13},
          {sf::Keyboard::Scan::F14},
          {sf::Keyboard::Scan::F15},
          {sf::Keyboard::Scan::F16},
          {sf::Keyboard::Scan::F17},
          {sf::Keyboard::Scan::F18},
          {sf::Keyboard::Scan::F19},
          {sf::Keyboard::Scan::F20},
          {sf::Keyboard::Scan::F21},
          {sf::Keyboard::Scan::F22},
          {sf::Keyboard::Scan::F23},
          {sf::Keyboard::Scan::F24}}},
        {{{sf::Keyboard::Scan::Application},
          {sf::Keyboard::Scan::Execute},
          {sf::Keyboard::Scan::ModeChange},
          {sf::Keyboard::Scan::Help},
          {sf::Keyboard::Scan::Select},
          {sf::Keyboard::Scan::Redo},
          {sf::Keyboard::Scan::Undo},
          {sf::Keyboard::Scan::Cut},
          {sf::Keyboard::Scan::Copy},
          {sf::Keyboard::Scan::Paste},
          {sf::Keyboard::Scan::VolumeMute},
          {sf::Keyboard::Scan::VolumeUp},
          {sf::Keyboard::Scan::VolumeDown},
          {sf::Keyboard::Scan::MediaPlayPause},
          {sf::Keyboard::Scan::MediaStop},
          {sf::Keyboard::Scan::MediaNextTrack},
          {sf::Keyboard::Scan::MediaPreviousTrack}}},
        {{{sf::Keyboard::Scan::Back},
          {sf::Keyboard::Scan::Forward},
          {sf::Keyboard::Scan::Refresh},
          {sf::Keyboard::Scan::Stop},
          {sf::Keyboard::Scan::Search},
          {sf::Keyboard::Scan::Favorites},
          {sf::Keyboard::Scan::HomePage},
          {sf::Keyboard::Scan::LaunchApplication1},
          {sf::Keyboard::Scan::LaunchApplication2},
          {sf::Keyboard::Scan::LaunchMail},
          {sf::Keyboard::Scan::LaunchMediaSelect}}},
    }};

    sf::VertexArray       m_triangles{sf::PrimitiveType::Triangles, sf::Keyboard::ScancodeCount * 6};
    std::vector<sf::Text> m_labels;
    std::array<float, sf::Keyboard::ScancodeCount> m_moveFactors{};
};

////////////////////////////////////////////////////////////
// Text with fading opacity outline
////////////////////////////////////////////////////////////
class ShinyText : public sf::Text
{
public:
    using sf::Text::Text;

    // Start the outline animation
    void shine(sf::Color color = sf::Color::Yellow)
    {
        setOutlineColor(color);
        m_remaining = duration;
    }

    // Fade out ouline
    void update(sf::Time frameTime)
    {
        const float ratio = m_remaining / duration;
        const float alpha = std::max(0.f, ratio * (2.f - ratio)) * 0.5f;

        sf::Color color = getOutlineColor();
        color.a         = static_cast<std::uint8_t>(255 * alpha);
        setOutlineColor(color);

        if (m_remaining > sf::Time::Zero)
            m_remaining -= frameTime;
    }

private:
    static constexpr sf::Time duration = sf::milliseconds(150);
    sf::Time                  m_remaining;
};

////////////////////////////////////////////////////////////
// Utilities to create text objets
////////////////////////////////////////////////////////////

constexpr unsigned int textSize = 18u;
constexpr unsigned int space    = 2u;
constexpr unsigned int lineSize = textSize + space;

float getSpacingFactor(const sf::Font& font)
{
    return static_cast<float>(lineSize) / font.getLineSpacing(textSize);
}

ShinyText makeShinyText(const sf::Font& font, const sf::String& string, sf::Vector2f position)
{
    ShinyText text(font, string, textSize);
    text.setLineSpacing(getSpacingFactor(font));
    text.setOutlineThickness(2.f);
    text.setPosition(position);

    return text;
}

sf::Text makeText(const sf::Font& font, const sf::String& string, sf::Vector2f position)
{
    sf::Text text(font, string, textSize);
    text.setLineSpacing(getSpacingFactor(font));
    text.setPosition(position);

    return text;
}

////////////////////////////////////////////////////////////
// Utilities to describe keyboard events
////////////////////////////////////////////////////////////

template <typename KeyEventType>
bool somethingIsOdd(const KeyEventType& keyEvent)
{
    return keyEvent.code == sf::Keyboard::Key::Unknown || keyEvent.scancode == sf::Keyboard::Scan::Unknown ||
           sf::Keyboard::getDescription(keyEvent.scancode) == "" ||
           sf::Keyboard::localize(keyEvent.scancode) != keyEvent.code ||
           sf::Keyboard::delocalize(keyEvent.code) != keyEvent.scancode;
}

// Append information to string about a keyboard event
template <typename KeyEventType>
sf::String keyEventDescription(sf::String text, const KeyEventType& keyEvent)
{
    text += "\n\n";
    text += keyIdentifier(keyEvent.code);
    text += "\n";
    text += scancodeIdentifier(keyEvent.scancode);
    if (somethingIsOdd(keyEvent))
    {
        text += "\nLocalized:\t";
        text += keyIdentifier(sf::Keyboard::localize(keyEvent.scancode));
        text += "\nDelocalized:\t";
        text += scancodeIdentifier(sf::Keyboard::delocalize(keyEvent.code));
    }

    return text;
}

// Make a string describing a text event
sf::String textEventDescription(const sf::Event::TextEntered& textEntered)
{
    sf::String text = "Text Entered\n\n";
    text += textEntered.unicode;
    text += "\nU+";

    std::ostringstream oss;
    oss << std::hex << std::setw(4) << std::setfill('0') << std::uint32_t{textEntered.unicode};
    text += oss.str();

    return text;
}

} // namespace


////////////////////////////////////////////////////////////
/// Entry point of application
///
/// \return Application exit code
///
////////////////////////////////////////////////////////////
int main()
{
    // Create the main window
    sf::RenderWindow window(sf::VideoMode({1280, 720}), "Keyboard", sf::Style::Titlebar | sf::Style::Close);
    window.setFramerateLimit(25);

    // Load sound buffers
    const sf::SoundBuffer errorSoundBuffer(resourcesDir() / "error_005.ogg");
    const sf::SoundBuffer pressedSoundBuffer(resourcesDir() / "mouseclick1.ogg");
    const sf::SoundBuffer releasedSoundBuffer(resourcesDir() / "mouserelease1.ogg");

    // Create sound objects to play them upon keyboard events
    sf::Sound errorSound(errorSoundBuffer);
    sf::Sound pressedSound(pressedSoundBuffer);
    sf::Sound releasedSound(releasedSoundBuffer);

    // Open the font used for all texts
    const sf::Font font(resourcesDir() / "Tuffy.ttf");

    // Create object to display all scancodes descriptions, related events and real-time state
    KeyboardView keyboardView(font);
    keyboardView.setPosition({16, 16});

    // Create text to display information about keyboard events and key codes real-time state
    ShinyText keyPressedText(makeShinyText(font, "Key Pressed", {16, 575}));
    ShinyText keyReleasedText(makeShinyText(font, "Key Released", {300, 575}));
    ShinyText textEnteredText(makeShinyText(font, "Text Entered", {600, 575}));
    sf::Text  keyPressedCheckText(makeText(font, "", {900, 575}));

    sf::Clock clock;
    while (window.isOpen())
    {
        // Handle events
        while (const std::optional event = window.pollEvent())
        {
            // Window closed: exit
            if (event->is<sf::Event::Closed>())
            {
                window.close();
                break;
            }

            // Window size changed: adjust view appropriately
            if (const auto* resized = event->getIf<sf::Event::Resized>())
                window.setView(sf::View(sf::FloatRect({}, sf::Vector2f(resized->size))));

            // Key events: update text and play sound
            if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>())
            {
                keyPressedText.setString(keyEventDescription("Key Pressed", *keyPressed));
                if (somethingIsOdd(*keyPressed))
                {
                    keyPressedText.shine(sf::Color::Red);
                    errorSound.play();
                }
                else
                {
                    keyPressedText.shine(sf::Color::Green);
                    pressedSound.play();
                }
            }
            if (const auto* keyReleased = event->getIf<sf::Event::KeyReleased>())
            {
                keyReleasedText.setString(keyEventDescription("Key Released", *keyReleased));
                if (somethingIsOdd(*keyReleased))
                {
                    keyReleasedText.shine(sf::Color::Red);
                    errorSound.play();
                }
                else
                {
                    keyReleasedText.shine(sf::Color::Green);
                    releasedSound.play();
                }
            }
            if (const auto* textEntered = event->getIf<sf::Event::TextEntered>())
            {
                textEnteredText.setString(textEventDescription(*textEntered));
                textEnteredText.shine();
            }

            // Let the KeyboardView process the event
            keyboardView.handle(*event);
        }

        // Update animations and displayed keyboard real-time state
        const sf::Time frameTime = clock.restart();
        keyboardView.update(frameTime);
        keyPressedText.update(frameTime);
        keyReleasedText.update(frameTime);
        textEnteredText.update(frameTime);
        {
            sf::String text = "isKeyPressed(sf::Keyboard::Key)\n\n";
            for (std::size_t keyIndex = 0u; keyIndex < sf::Keyboard::KeyCount; ++keyIndex)
            {
                const auto key = static_cast<sf::Keyboard::Key>(keyIndex);
                if (sf::Keyboard::isKeyPressed(key))
                    text += keyIdentifier(key) + "\n";
            }
            keyPressedCheckText.setString(text);
        }

        // Render frame
        window.clear();
        window.draw(keyboardView);
        window.draw(keyPressedText);
        window.draw(keyReleasedText);
        window.draw(textEnteredText);
        window.draw(keyPressedCheckText);
        window.display();
    }

    return EXIT_SUCCESS;
}
