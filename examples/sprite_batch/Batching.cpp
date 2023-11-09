
////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

#include <array>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <map>
#include <numeric>
#include <random>
#include <vector>

#ifdef SFML_SYSTEM_IOS
#include <SFML/Main.hpp>
#endif

std::filesystem::path resourcesDir()
{
#ifdef SFML_SYSTEM_IOS
    return "";
#else
    return "resources";
#endif
}

enum class BatchDrawables
{
    Sprite,
    Text,
    Shape,
    ShapeNoTexture
};

std::string getRandomString(std::mt19937& mt)
{
    static const std::array<std::string, 7> texts =
        {"",
         "Hello World!",
         "London is the capital \nof Great Britain.",
         "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Morbi ut augue in dui \n"
         "posuere sodales nec a orci. Phasellus sit amet tellus lobortis, scelerisque \n"
         "dui dictum, posuere risus. Nam pellentesque gravida elit, id convallis tortor \n"
         "aliquet sed. Duis vitae cursus risus, fermentum condimentum ex. Proin mattis \n"
         "orci a malesuada commodo. Nunc egestas erat non volutpat elementum. Vivamus \n"
         "pulvinar tortor eleifend libero convallis, at volutpat turpis lobortis. \n",
         "'Tis but a flesh wound.",
         "SFML Test Demo Thingy Stuff (Epic)",
         "?!?!?!111(one)"};

    return texts[std::uniform_int_distribution<std::size_t>(0, texts.size() - 1)(mt)];
}

std::string getDisplayString(sf::SpriteBatch::BatchMode batchMode)
{
    static const std::map<sf::SpriteBatch::BatchMode, std::string> modeDisplay =
        {{sf::SpriteBatch::BatchMode::Deferred, "Deferred"},
         {sf::SpriteBatch::BatchMode::TextureSort, "TextureSort"},
         {sf::SpriteBatch::BatchMode::DepthSort, "DepthSort"}};

    if (modeDisplay.find(batchMode) != modeDisplay.end())
        return modeDisplay.at(batchMode);

    return "Unknown";
}

std::string getDisplayString(BatchDrawables drawableMode)
{
    static const std::map<BatchDrawables, std::string> modeDisplay =
        {{BatchDrawables::Sprite, "Sprite"},
         {BatchDrawables::Text, "Text"},
         {BatchDrawables::Shape, "Shape"},
         {BatchDrawables::ShapeNoTexture, "Shape (No Texture)"}};

    if (modeDisplay.find(drawableMode) != modeDisplay.end())
        return modeDisplay.at(drawableMode);

    return "Unknown";
}

////////////////////////////////////////////////////////////
/// Entry point of application
///
/// \return Application exit code
///
////////////////////////////////////////////////////////////
int main()
{
    std::random_device rd;
    std::mt19937       mt(rd());

    // Define some constants used within the demo
    const int         windowWidth         = 1600;
    const int         windowHeight        = 900;
    const int         sameTextureChance   = 85;
    const std::size_t drawableCountChange = 100;
    const std::size_t maxTimesSaved       = 60;
    const std::size_t drawablesPerLayer   = 250;

    // Create the window of the application
    sf::VideoMode    videMode({windowWidth, windowHeight}, 32);
    sf::RenderWindow window(videMode, "SFML SpriteBatch", sf::Style::Titlebar | sf::Style::Close);
    window.setVerticalSyncEnabled(true);

    // Load texture assets
    std::vector<sf::Texture> textures;
    sf::Texture              tempTexture;

    int currentTex = 0;
    while (tempTexture.loadFromFile(resourcesDir() / ("Tex" + std::to_string(currentTex) + ".png")))
    {
        textures.push_back(std::move(tempTexture));
        tempTexture = sf::Texture();
        ++currentTex;
    }

    // Load font for debug text
    sf::Font font;
    if (!font.loadFromFile(resourcesDir() / "tuffy.ttf"))
        return EXIT_FAILURE;

    // Different types of drawables
    // For simplicity, we keep N objects of each type
    std::vector<sf::Sprite>      sprites;
    std::vector<sf::CircleShape> shapes;
    std::vector<sf::CircleShape> shapesNoTexture;
    std::vector<sf::Text>        texts;

    // Text used for debug info
    sf::Text debugInfoText("", font);
    debugInfoText.setOutlineColor(sf::Color::Blue);
    debugInfoText.setOutlineThickness(4);
    debugInfoText.setFillColor(sf::Color::White);

    // Define some state used in the demo
    std::size_t                drawableCount = 100;
    sf::SpriteBatch            batch;
    sf::SpriteBatch::BatchMode batchMode      = sf::SpriteBatch::BatchMode::Deferred;
    bool                       batchEnabled   = false;
    bool                       useStaticBatch = false;
    std::vector<sf::Time>      timeHistory;
    sf::Time                   timeReference;
    std::size_t                framesTillRefUpdate = 0;
    sf::Clock                  clock;
    BatchDrawables             benchmarkedDrawable = BatchDrawables::Sprite;

    while (window.isOpen())
    {
        // Handle events
        for (sf::Event event; window.pollEvent(event);)
        {
            // Window closed or escape key pressed: exit
            if (event.type == sf::Event::Closed ||
                (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape))
            {
                window.close();
                break;
            }

            // W key pressed: increase drawable count
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Up)
            {
                drawableCount += drawableCountChange;
            }

            // S key pressed: decrease drawable count
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Down)
            {
                drawableCount -= std::min(drawableCount, drawableCountChange);
            }

            // R key pressed: cycle through the drawables to render
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::D)
            {
                switch (benchmarkedDrawable)
                {
                    case BatchDrawables::Sprite:
                        benchmarkedDrawable = BatchDrawables::Text;
                        break;

                    case BatchDrawables::Text:
                        benchmarkedDrawable = BatchDrawables::Shape;
                        break;

                    case BatchDrawables::Shape:
                        benchmarkedDrawable = BatchDrawables::ShapeNoTexture;
                        break;

                    case BatchDrawables::ShapeNoTexture:
                        benchmarkedDrawable = BatchDrawables::Sprite;
                        break;
                }
            }

            // F or G key pressed: cycle through batch modes
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::F)
            {
                switch (batchMode)
                {
                    case sf::SpriteBatch::BatchMode::Deferred:
                        batchMode = sf::SpriteBatch::BatchMode::TextureSort;
                        break;

                    case sf::SpriteBatch::BatchMode::TextureSort:
                        batchMode = sf::SpriteBatch::BatchMode::DepthSort;
                        break;

                    case sf::SpriteBatch::BatchMode::DepthSort:
                        batchMode = sf::SpriteBatch::BatchMode::Deferred;
                        break;
                }
            }

            // G key pressed: toggle batching
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::G)
            {
                batchEnabled = !batchEnabled;
            }

            // H key pressed: toggle static batch
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::H)
            {
                useStaticBatch = !useStaticBatch;
            }

            // Window size changed, adjust view appropriately
            if (event.type == sf::Event::Resized)
            {
                sf::View view;
                view.setSize({windowWidth, windowHeight});
                view.setCenter({windowWidth / 2.f, windowHeight / 2.f});
                window.setView(view);
            }
        }

        // Clear the window
        window.clear(sf::Color(50, 50, 50));

        // Delete excess drawables
        while (drawableCount < sprites.size())
        {
            sprites.pop_back();
            texts.pop_back();
            shapes.pop_back();
            shapesNoTexture.pop_back();
        }

        // Add new drawables
        while (drawableCount > sprites.size())
        {
            float x       = std::uniform_real_distribution<float>(0, windowWidth)(mt);
            float y       = std::uniform_real_distribution<float>(0, windowHeight)(mt);
            float degrees = std::uniform_real_distribution<float>(0, 360)(mt);

            const sf::Texture* texture = &textures[std::uniform_int_distribution<std::size_t>(0, textures.size() - 1)(mt)];

            // X% chance to use the previous texture instead
            // The ordered batcher performs badly when textures change extremely often.
            if (!sprites.empty() && std::uniform_int_distribution<int>(0, 99)(mt) < sameTextureChance)
                texture = (sprites.end() - 1)->getTexture();

            auto& sprite = sprites.emplace_back();

            sprite.setPosition(sf::Vector2f(x, y));
            sprite.setRotation(sf::degrees(degrees));
            sprite.setTexture(*texture);

            auto& shape = shapes.emplace_back(60.0f, 30);

            shape.setPosition(sf::Vector2f(x, y));
            shape.setRotation(sf::degrees(degrees));
            shape.setTexture(texture);
            shape.setOutlineColor(sf::Color::Blue);
            shape.setOutlineThickness(2);

            auto& shapeNoTexture = shapesNoTexture.emplace_back(60.0f, 30);

            shapeNoTexture.setPosition(sf::Vector2f(x, y));
            shapeNoTexture.setRotation(sf::degrees(degrees));
            shapeNoTexture.setOutlineColor(sf::Color::Blue);
            shapeNoTexture.setOutlineThickness(2);

            auto& text = texts.emplace_back(getRandomString(mt), font, 60);

            text.setPosition(sf::Vector2f(x, y));
            text.setRotation(sf::degrees(degrees));
            text.setFillColor(sf::Color::Black);
            text.setOutlineColor(sf::Color::White);
            text.setOutlineThickness(8);
            text.setStyle(sf::Text::Bold);
        }

        // Update drawables
        for (std::size_t i = 0; i < sprites.size(); i++)
        {
            const sf::Angle amount = sf::degrees(2);

            sprites[i].setRotation(sprites[i].getRotation() + amount);
            texts[i].setRotation(texts[i].getRotation() + amount / 8);
            shapes[i].setRotation(shapes[i].getRotation() + amount);
            shapesNoTexture[i].setRotation(shapesNoTexture[i].getRotation() + amount);
        }

        // Batch drawables
        clock.restart();

        float       depth = 1.f;
        std::size_t count = 0;

        if (batchEnabled && !useStaticBatch)
        {
            batch.clear();
            batch.setBatchMode(batchMode);

            for (std::size_t i = 0; i < sprites.size(); i++)
            {
                const sf::Batchable* batchable = nullptr;

                switch (benchmarkedDrawable)
                {
                    case BatchDrawables::Sprite:
                        batchable = &sprites[i];
                        break;

                    case BatchDrawables::Shape:
                        batchable = &shapes[i];
                        break;

                    case BatchDrawables::ShapeNoTexture:
                        batchable = &shapesNoTexture[i];
                        break;

                    case BatchDrawables::Text:
                        batchable = &texts[i];
                        break;
                }

                batch.batch(*batchable, depth);

                count++;

                // Move to another layer for DepthSort case
                // In a
                if (count > drawablesPerLayer)
                {
                    count -= drawablesPerLayer;
                    depth -= 0.01f;
                }
            }
        }

        if (!batchEnabled)
        {
            for (std::size_t i = 0; i < sprites.size(); i++)
            {
                const sf::Drawable* drawable = nullptr;

                switch (benchmarkedDrawable)
                {
                    case BatchDrawables::Sprite:
                        drawable = &sprites[i];
                        break;

                    case BatchDrawables::Shape:
                        drawable = &shapes[i];
                        break;

                    case BatchDrawables::ShapeNoTexture:
                        drawable = &shapesNoTexture[i];
                        break;

                    case BatchDrawables::Text:
                        drawable = &texts[i];
                        break;
                }

                window.draw(*drawable);
            }
        }

        if (batchEnabled)
            batch.draw(window, sf::RenderStates(sf::BlendAlpha, sf::Transform::Identity, nullptr, nullptr));

        sf::Time time = clock.restart();

        timeHistory.push_back(time);

        if (timeHistory.size() > maxTimesSaved)
            timeHistory.erase(timeHistory.begin(), timeHistory.end() - static_cast<int>(maxTimesSaved));

        const sf::Time average = std::accumulate(timeHistory.begin(), timeHistory.end(), sf::Time::Zero) /
                                 static_cast<float>(timeHistory.size());

        if (framesTillRefUpdate <= 0)
        {
            framesTillRefUpdate = 18;
            timeReference       = time;
        }

        framesTillRefUpdate--;

        // clang-format off
        debugInfoText.setPosition(sf::Vector2f(0, 0));
        debugInfoText.setString(
            "Batching enabled:\n"
            "Current drawable:\n"
            "Time:\n"
            "Time (average):\n"
            "Drawables:\n"
            "Batch mode:\n");

        window.draw(debugInfoText);

        debugInfoText.setPosition(sf::Vector2f(250, 0));
        debugInfoText.setString(
            std::string(batchEnabled ? "true" : "false") + "\n" +
            getDisplayString(benchmarkedDrawable) + "\n" +
            std::to_string(timeReference.asSeconds() * 1000.0f) + "\n" +
            std::to_string(average.asSeconds() * 1000.0f) + "\n" +
            std::to_string(drawableCount) + "\n" +
            getDisplayString(batchMode) + "\n" +
            (useStaticBatch ? "Static batch is active" : ""));
        // clang-format on

        window.draw(debugInfoText);

        // Display things on screen
        window.display();
    }

    return EXIT_SUCCESS;
}
