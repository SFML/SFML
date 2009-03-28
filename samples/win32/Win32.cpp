
////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Graphics.hpp>
#include <windows.h>
#include <cmath>

HWND Button;


////////////////////////////////////////////////////////////
/// Function called whenever one of our windows receives a message
///
////////////////////////////////////////////////////////////
LRESULT CALLBACK OnEvent(HWND Handle, UINT Message, WPARAM WParam, LPARAM LParam)
{
    switch (Message)
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
            if (reinterpret_cast<HWND>(LParam) == Button)
            {
                PostQuitMessage(0);
                return 0;
            }
        }
    }

    return DefWindowProc(Handle, Message, WParam, LParam);
}


////////////////////////////////////////////////////////////
/// Entry point of application
///
/// \param Instance : Instance of the application
///
/// \return Error code
///
////////////////////////////////////////////////////////////
INT WINAPI WinMain(HINSTANCE Instance, HINSTANCE, LPSTR, INT)
{
    // Define a class for our main window
    WNDCLASS WindowClass;
    WindowClass.style         = 0;
    WindowClass.lpfnWndProc   = &OnEvent;
    WindowClass.cbClsExtra    = 0;
    WindowClass.cbWndExtra    = 0;
    WindowClass.hInstance     = Instance;
    WindowClass.hIcon         = NULL;
    WindowClass.hCursor       = 0;
    WindowClass.hbrBackground = reinterpret_cast<HBRUSH>(COLOR_BACKGROUND);
    WindowClass.lpszMenuName  = NULL;
    WindowClass.lpszClassName = TEXT("SFML App");
    RegisterClass(&WindowClass);

    // Let's create the main window
    HWND Window = CreateWindow(TEXT("SFML App"), TEXT("SFML Win32"), WS_SYSMENU | WS_VISIBLE, 200, 200, 660, 520, NULL, NULL, Instance, NULL);

    // Add a button for exiting
    Button = CreateWindow(TEXT("BUTTON"), TEXT("Quit"), WS_CHILD | WS_VISIBLE, 560, 440, 80, 40, Window, NULL, Instance, NULL);

    // Let's create two SFML views
    HWND View1 = CreateWindow(TEXT("STATIC"), NULL, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS, 20,  20, 300, 400, Window, NULL, Instance, NULL);
    HWND View2 = CreateWindow(TEXT("STATIC"), NULL, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS, 340, 20, 300, 400, Window, NULL, Instance, NULL);
    sf::RenderWindow SFMLView1(View1);
    sf::RenderWindow SFMLView2(View2);

    // Load some images to display
    sf::Image Image1, Image2;
    if (!Image1.LoadFromFile("datas/win32/image1.jpg") || !Image2.LoadFromFile("datas/win32/image2.jpg"))
        return EXIT_FAILURE;
    sf::Sprite Sprite1(Image1);
    sf::Sprite Sprite2(Image2);
    Sprite1.SetCenter(Sprite1.GetSize() / 2.f);

    // Create a clock for measuring elapsed time
    sf::Clock Clock;

    // Loop until a WM_QUIT message is received
    MSG Message;
    Message.message = static_cast<UINT>(~WM_QUIT);
    while (Message.message != WM_QUIT)
    {
        if (PeekMessage(&Message, NULL, 0, 0, PM_REMOVE))
        {
            // If a message was waiting in the message queue, process it
            TranslateMessage(&Message);
            DispatchMessage(&Message);
        }
        else
        {
            // Clear views
            SFMLView1.Clear();
            SFMLView2.Clear();

            // Draw sprite 1 on view 1
            Sprite1.SetRotation(Clock.GetElapsedTime() * 100);
            SFMLView1.Draw(Sprite1);

            // Draw sprite 2 on view 2
            Sprite2.SetX(cos(Clock.GetElapsedTime()) * 100);
            SFMLView2.Draw(Sprite2);

            // Display each view on screen
            SFMLView1.Display();
            SFMLView2.Display();
        }
    }

    // Destroy the main window (all its child controls will be destroyed)
    DestroyWindow(Window);

    // Don't forget to unregister the window class
    UnregisterClass(TEXT("SFML App"), Instance);

    return EXIT_SUCCESS;
}
