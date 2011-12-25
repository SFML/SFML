
////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Graphics.hpp>
#include <windows.h>
#include <cmath>

HWND button;


////////////////////////////////////////////////////////////
/// Function called whenever one of our windows receives a message
///
////////////////////////////////////////////////////////////
LRESULT CALLBACK OnEvent(HWND handle, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
        // Quit when we close the main window
        case WM_CLOSE :
        {
            PostQuitMessage(0);
            return 0;
        }

        // Quit when we click the "quit" button
        case WM_COMMAND :
        {
            if (reinterpret_cast<HWND>(lParam) == button)
            {
                PostQuitMessage(0);
                return 0;
            }
        }
    }

    return DefWindowProc(handle, message, wParam, lParam);
}


////////////////////////////////////////////////////////////
/// Entry point of application
///
/// \param Instance : Instance of the application
///
/// \return Error code
///
////////////////////////////////////////////////////////////
INT WINAPI WinMain(HINSTANCE instance, HINSTANCE, LPSTR, INT)
{
    // Define a class for our main window
    WNDCLASS windowClass;
    windowClass.style         = 0;
    windowClass.lpfnWndProc   = &OnEvent;
    windowClass.cbClsExtra    = 0;
    windowClass.cbWndExtra    = 0;
    windowClass.hInstance     = instance;
    windowClass.hIcon         = NULL;
    windowClass.hCursor       = 0;
    windowClass.hbrBackground = reinterpret_cast<HBRUSH>(COLOR_BACKGROUND);
    windowClass.lpszMenuName  = NULL;
    windowClass.lpszClassName = TEXT("SFML App");
    RegisterClass(&windowClass);

    // Let's create the main window
    HWND window = CreateWindow(TEXT("SFML App"), TEXT("SFML Win32"), WS_SYSMENU | WS_VISIBLE, 200, 200, 660, 520, NULL, NULL, instance, NULL);

    // Add a button for exiting
    button = CreateWindow(TEXT("BUTTON"), TEXT("Quit"), WS_CHILD | WS_VISIBLE, 560, 440, 80, 40, window, NULL, instance, NULL);

    // Let's create two SFML views
    HWND view1 = CreateWindow(TEXT("STATIC"), NULL, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS, 20,  20, 300, 400, window, NULL, instance, NULL);
    HWND view2 = CreateWindow(TEXT("STATIC"), NULL, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS, 340, 20, 300, 400, window, NULL, instance, NULL);
    sf::RenderWindow SFMLView1(view1);
    sf::RenderWindow SFMLView2(view2);

    // Load some textures to display
    sf::Texture texture1, texture2;
    if (!texture1.LoadFromFile("resources/image1.jpg") || !texture2.LoadFromFile("resources/image2.jpg"))
        return EXIT_FAILURE;
    sf::Sprite sprite1(texture1);
    sf::Sprite sprite2(texture2);
    sprite1.SetOrigin(texture1.GetWidth() / 2.f, texture1.GetHeight() / 2.f);
    sprite1.SetPosition(sprite1.GetOrigin());

    // Create a clock for measuring elapsed time
    sf::Clock clock;

    // Loop until a WM_QUIT message is received
    MSG message;
    message.message = static_cast<UINT>(~WM_QUIT);
    while (message.message != WM_QUIT)
    {
        if (PeekMessage(&message, NULL, 0, 0, PM_REMOVE))
        {
            // If a message was waiting in the message queue, process it
            TranslateMessage(&message);
            DispatchMessage(&message);
        }
        else
        {
            sf::Uint32 time = clock.GetElapsedTime();

            // Clear views
            SFMLView1.Clear();
            SFMLView2.Clear();

            // Draw sprite 1 on view 1
            sprite1.SetRotation(time * 0.1f);
            SFMLView1.Draw(sprite1);

            // Draw sprite 2 on view 2
            sprite2.SetPosition(std::cos(time * 0.001f) * 100.f, 0.f);
            SFMLView2.Draw(sprite2);

            // Display each view on screen
            SFMLView1.Display();
            SFMLView2.Display();
        }
    }

    // Destroy the main window (all its child controls will be destroyed)
    DestroyWindow(window);

    // Don't forget to unregister the window class
    UnregisterClass(TEXT("SFML App"), instance);

    return EXIT_SUCCESS;
}
