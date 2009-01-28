////////////////////////////////////////////////////////////
/// \mainpage
///
/// \section welcome Welcome
/// Welcome to the official SFML documentation for C. Here you will find a detailed
/// view of all the SFML <a href="./globals_func.htm">functions</a>, as well as source
/// <a href="./files.htm">files</a>. <br/>
/// If you are looking for tutorials, you can visit the official website
/// at <a href="http://www.sfml-dev.org/tutorials/">www.sfml-dev.org</a>.
///
/// \section example Short example
/// Here is a short example, to show you how simple it is to use SFML in C :
///
/// \code
///
/// #include <SFML/Audio.h>
/// #include <SFML/Graphics.h>
/// 
/// int main()
/// {
///     sfWindowSettings Settings = {24, 8, 0};
///     sfVideoMode Mode = {800, 600, 32};
///     sfRenderWindow* App;
///     sfImage* Image;
///     sfSprite* Sprite;
///     sfFont* Font;
///     sfString* Text;
///     sfMusic* Music;
///     sfEvent Event;
///
///     /* Create the main window */
///     App = sfRenderWindow_Create(Mode, "SFML window", sfResize | sfClose, Settings);
///     if (!App)
///         return EXIT_FAILURE;
///
///     /* Load a sprite to display */
///     Image = sfImage_CreateFromFile("cute_image.jpg");
///     if (!Image)
///         return EXIT_FAILURE;
///     Sprite = sfSprite_Create();
///     sfSprite_SetImage(Sprite, Image);
/// 
///     /* Create a graphical string to display */
///     Font = sfFont_CreateFromFile("arial.ttf", 50, NULL);
///     if (!Font)
///         return EXIT_FAILURE;
///     Text = sfString_Create();
///     sfString_SetText(Text, "Hello SFML");
///     sfString_SetFont(Text, Font);
///     sfString_SetSize(Text, 50);
/// 
///     /* Load a music to play */
///     Music = sfMusic_CreateFromFile("nice_music.ogg");
///     if (!Music)
///         return EXIT_FAILURE;
///
///     /* Play the music */
///     sfMusic_Play(Music);
/// 
///     /* Start the game loop */
///     while (sfRenderWindow_IsOpened(App))
///     {
///         /* Process events */
///         while (sfRenderWindow_GetEvent(App, &Event))
///         {
///             /* Close window : exit */
///             if (Event.Type == sfEvtClosed)
///                 sfRenderWindow_Close(App);
///         }
/// 
///         /* Clear the screen */
///         sfRenderWindow_Clear(App, sfBlack);
/// 
///         /* Draw the sprite */
///         sfRenderWindow_DrawSprite(App, Sprite);
/// 
///         /* Draw the string */
///         sfRenderWindow_DrawString(App, Text);
/// 
///         /* Update the window */
///         sfRenderWindow_Display(App);
///     }
/// 
///     /* Cleanup resources */
///     sfMusic_Destroy(Music);
///     sfString_Destroy(Text);
///     sfFont_Destroy(Font);
///     sfSprite_Destroy(Sprite);
///     sfImage_Destroy(Image);
///     sfRenderWindow_Destroy(App);
/// 
///     return EXIT_SUCCESS;
/// }
/// \endcode
////////////////////////////////////////////////////////////
