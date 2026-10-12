#include <SFML/Graphics.hpp>

#include <SFML/Audio.hpp>

#include <SFML/Network.hpp>

#include <SFML/Window.hpp>

#include <SFML/System.hpp>

// Do we want to showcase direct JNI/NDK interaction?
// Undefine this to get real cross-platform code.
// Uncomment this to try JNI access; this seems to be broken in latest NDKs
//#define USE_JNI

#if defined(USE_JNI)
// These headers are only needed for direct NDK/JDK interaction
#include <android/native_activity.h>
#include <jni.h>

// Since we want to get the native activity from SFML, we'll have to use an
// extra header here:
#include <SFML/System/NativeActivity.hpp>

// NDK/JNI sub example - call Java code from native code
int vibrate(sf::Time duration)
{
    // First we'll need the native activity handle
    ANativeActivity& activity = *sf::getNativeActivity();

    // Retrieve the JVM and JNI environment
    JavaVM& vm  = *activity.vm;
    JNIEnv& env = *activity.env;

    // First, attach this thread to the main thread
    JavaVMAttachArgs attachargs;
    attachargs.version = JNI_VERSION_1_6;
    attachargs.name    = "NativeThread";
    attachargs.group   = nullptr;
    jint res           = vm.AttachCurrentThread(&env, &attachargs);

    if (res == JNI_ERR)
        return EXIT_FAILURE;

    // Retrieve class information
    jclass natact  = env.FindClass("android/app/NativeActivity");
    jclass context = env.FindClass("android/content/Context");

    // Get the value of a constant
    jfieldID fid    = env.GetStaticFieldID(context, "VIBRATOR_SERVICE", "Ljava/lang/String;");
    jobject  svcstr = env.GetStaticObjectField(context, fid);

    // Get the method 'getSystemService' and call it
    jmethodID getss   = env.GetMethodID(natact, "getSystemService", "(Ljava/lang/String;)Ljava/lang/Object;");
    jobject   vib_obj = env.CallObjectMethod(activity.clazz, getss, svcstr);

    // Get the object's class and retrieve the member name
    jclass    vib_cls = env.GetObjectClass(vib_obj);
    jmethodID vibrate = env.GetMethodID(vib_cls, "vibrate", "(J)V");

    // Determine the timeframe
    jlong length = duration.asMilliseconds();

    // Bzzz!
    env.CallVoidMethod(vib_obj, vibrate, length);

    // Free references
    env.DeleteLocalRef(vib_obj);
    env.DeleteLocalRef(vib_cls);
    env.DeleteLocalRef(svcstr);
    env.DeleteLocalRef(context);
    env.DeleteLocalRef(natact);

    // Detach thread again
    vm.DetachCurrentThread();
}
#endif

// This is the actual Android example. You don't have to write any platform
// specific code, unless you want to use things not directly exposed.
// ('vibrate()' in this example; undefine 'USE_JNI' above to disable it)
int main(int argc, char* argv[])
{
    sf::VideoMode screen(sf::VideoMode::getDesktopMode());

    sf::RenderWindow window(screen, "");
    window.setFramerateLimit(30);

    const sf::Texture texture("image.png");

    sf::Sprite image(texture);
    image.setPosition(sf::Vector2f(screen.size) / 2.f);
    image.setOrigin(sf::Vector2f(texture.getSize()) / 2.f);

    const sf::Font font("tuffy.ttf");
    const sf::Font unifont("unifont-17.0.04.otf");


    sf::Text text(font, "Tap anywhere to move the logo.", 64);
    text.setFillColor(sf::Color::Black);
    text.setPosition({10, 10});

    sf::View  view       = window.getDefaultView();
    sf::Color background = sf::Color::White;

    // We shouldn't try drawing to the screen while in background
    // so we'll have to track that. You can do minor background
    // work, but keep battery life in mind.
    bool active = true;

    // Text input UI
    sf::RectangleShape inputBox;
    inputBox.setPosition({40.f, 120.f});
    inputBox.setSize({screen.size.x - 80.f, 100.f});
    inputBox.setFillColor(sf::Color(245, 245, 245));
    inputBox.setOutlineThickness(4.f);
    inputBox.setOutlineColor(sf::Color::Black);

    sf::String inputString;

    sf::Text inputText(unifont, "", 48);
    inputText.setFillColor(sf::Color::Black);
    inputText.setPosition({60.f, 135.f});

    sf::Text placeholder(font, "Tap here to type...", 48);
    placeholder.setFillColor(sf::Color(130, 130, 130));
    placeholder.setPosition({60.f, 135.f});

    bool inputFocused = false;

    auto updateInputBox = [&]()
    {
        inputBox.setOutlineColor(inputFocused ? sf::Color::Blue : sf::Color::Black);
        inputText.setString(inputString);
    };

    updateInputBox();

    while (window.isOpen())
    {
        while (const std::optional event = active ? window.pollEvent() : window.waitEvent())
        {
            if (event->is<sf::Event::Closed>() ||
                (event->is<sf::Event::KeyPressed>() &&
                 event->getIf<sf::Event::KeyPressed>()->code == sf::Keyboard::Key::Escape))
            {
                window.close();
            }
            else if (const auto* resized = event->getIf<sf::Event::Resized>())
            {
                const auto size = sf::Vector2f(resized->size);
                view.setSize(size);
                view.setCenter(size / 2.f);
                window.setView(view);

                inputBox.setSize({size.x - 80.f, 100.f});
            }
            else if (event->is<sf::Event::FocusLost>())
            {
                background = sf::Color::Black;
            }
            else if (event->is<sf::Event::FocusGained>())
            {
                background = sf::Color::White;
            }
            // On Android MouseLeft/MouseEntered are (for now) triggered,
            // whenever the app loses or gains focus.
            else if (event->is<sf::Event::MouseLeft>())
            {
                active = false;
            }
            else if (event->is<sf::Event::MouseEntered>())
            {
                active = true;
            }
            else if (const auto* touchBegan = event->getIf<sf::Event::TouchBegan>())
            {
                if (touchBegan->finger == 0)
                {
                    const sf::Vector2f touchPos(touchBegan->position);

                    if (inputBox.getGlobalBounds().contains(touchPos))
                    {
                        inputFocused = true;
                        sf::Keyboard::setVirtualKeyboardVisible(true);
                    }
                    else
                    {
                        inputFocused = false;
                        sf::Keyboard::setVirtualKeyboardVisible(false);

                        // Keep original tap-to-move logic
                        image.setPosition(touchPos);
#if defined(USE_JNI)
                        vibrate(sf::milliseconds(10));
#endif
                    }

                    updateInputBox();
                }
            }
            else if (const auto* textEntered = event->getIf<sf::Event::TextEntered>())
            {
                if (!inputFocused)
                    continue;

                const char32_t unicode = textEntered->unicode;

                // Backspace
                if (unicode == 8)
                {
                    if (!inputString.isEmpty())
                        inputString.erase(inputString.getSize() - 1, 1);
                }
                // Enter closes input
                else if (unicode == 13 || unicode == '\n' || unicode == '\r')
                {
                    inputFocused = false;
                    sf::Keyboard::setVirtualKeyboardVisible(false);
                }
                // Printable chars
                else if (unicode >= 32)
                {
                    inputString += unicode;
                }

                updateInputBox();
            }
        }

        if (active)
        {
            window.clear(background);
            window.draw(image);
            window.draw(text);
            window.draw(inputBox);

            if (inputString.isEmpty())
                window.draw(placeholder);
            else
                window.draw(inputText);

            window.display();
        }
        else
        {
            sf::sleep(sf::milliseconds(100));
        }
    }
}
