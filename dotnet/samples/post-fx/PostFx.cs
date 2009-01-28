using System;
using System.Collections.Generic;
using SFML;
using SFML.Graphics;
using SFML.Window;

namespace sample_postfx
{
    static class Program
    {
        private static Dictionary<string, PostFx>            Effects;
        private static Dictionary<string, PostFx>.Enumerator CurrentEffect;
        private static String2D                              CurFXStr;

        /// <summary>
        /// The main entry point for the application.
        /// </summary>
        static void Main()
        {
            // Create the main window
            RenderWindow App = new RenderWindow(new VideoMode(800, 600), "SFML.Net PostFX");

            // Setup event handlers
            App.Closed     += new EventHandler(OnClosed);
            App.KeyPressed += new EventHandler<KeyEventArgs>(OnKeyPressed);

            // Check that the system can use post effects
            if (PostFx.CanUsePostFX == false)
            {
                DisplayError(App);
                return;
            }

            // Load a cute background image to display :)
            Sprite Background = new Sprite(new Image("datas/post-fx/background.jpg"));

            // Load the text font
            Font Cheeseburger = new Font("datas/post-fx/cheeseburger.ttf");

            // Load the image needed for the wave effect
            Image WaveImage = new Image("datas/post-fx/wave.jpg");

            // Load all effects
            Effects = new Dictionary<string, PostFx>();
            Effects["nothing"]  = new PostFx("datas/post-fx/nothing.sfx");
            Effects["blur"]     = new PostFx("datas/post-fx/blur.sfx");
            Effects["colorize"] = new PostFx("datas/post-fx/colorize.sfx");
            Effects["fisheye"]  = new PostFx("datas/post-fx/fisheye.sfx");
            Effects["wave"]     = new PostFx("datas/post-fx/wave.sfx");
            CurrentEffect = Effects.GetEnumerator();
            CurrentEffect.MoveNext();

            // Do specific initializations
            Effects["nothing"].SetTexture("framebuffer", null);
            Effects["blur"].SetTexture("framebuffer", null);
            Effects["blur"].SetParameter("offset", 0.0F);
            Effects["colorize"].SetTexture("framebuffer", null);
            Effects["colorize"].SetParameter("color", 1.0F, 1.0F, 1.0F);
            Effects["fisheye"].SetTexture("framebuffer", null);
            Effects["wave"].SetTexture("framebuffer", null);
            Effects["wave"].SetTexture("wave", WaveImage);

            // Define a string for displaying current effect description
            CurFXStr = new String2D();
            CurFXStr.Text = "Current effect is \"" + CurrentEffect.Current.Key + "\"";
            CurFXStr.Font = Cheeseburger;
            CurFXStr.Position = new Vector2(20.0F, 0.0F);

            // Define a string for displaying help
            String2D InfoStr = new String2D();
            InfoStr.Text = "Move your mouse to change the effect parameters\nPress numpad + to change effect\nWarning : some effects may not work\ndepending on your graphics card";
            InfoStr.Font = Cheeseburger;
            InfoStr.Position = new Vector2(20.0F, 460.0F);
            InfoStr.Color = new Color(200, 100, 150);

            // Start the game loop
            while (App.IsOpened())
            {
                // Process events
                App.DispatchEvents();

                // Get the mouse position in the range [0, 1]
                float X = App.Input.GetMouseX() / (float)App.Width;
                float Y = App.Input.GetMouseY() / (float)App.Height;

                // Update the current effect
                if      (CurrentEffect.Current.Key == "blur")     CurrentEffect.Current.Value.SetParameter("offset", X * Y * 0.1f);
                else if (CurrentEffect.Current.Key == "colorize") CurrentEffect.Current.Value.SetParameter("color", 0.3f, X, Y);
                else if (CurrentEffect.Current.Key == "fisheye")  CurrentEffect.Current.Value.SetParameter("mouse", X, 1.0F - Y);
                else if (CurrentEffect.Current.Key == "wave")     CurrentEffect.Current.Value.SetParameter("offset", X, Y);

                // Clear the window
                App.Clear();

                // Draw background and apply the post-fx
                App.Draw(Background);
                App.Draw(CurrentEffect.Current.Value);

                // Draw interface strings
                App.Draw(CurFXStr);
                App.Draw(InfoStr);

                // Finally, display the rendered frame on screen
                App.Display();
            }
        }

        /// <summary>
        /// Fonction called when the post-effects are not supported ;
        /// Display an error message and wait until the user exits
        /// </summary>
        private static void DisplayError(RenderWindow App)
        {
            // Define a string for displaying the error message
            String2D ErrorStr = new String2D("Sorry, your system doesn't support post-effects");
            ErrorStr.Position = new Vector2(100.0F, 250.0F);
            ErrorStr.Color = new Color(200, 100, 150);

            // Start the game loop
            while (App.IsOpened())
            {
                // Process events
                App.DispatchEvents();

                // Clear the window
                App.Clear();

                // Draw the error message
                App.Draw(ErrorStr);

                // Finally, display the rendered frame on screen
                App.Display();
            }
        }

        /// <summary>
        /// Function called when the window is closed
        /// </summary>
        static void OnClosed(object sender, EventArgs e)
        {
            RenderWindow window = (RenderWindow)sender;
            window.Close();
        }

        /// <summary>
        /// Function called when a key is pressed
        /// </summary>
        static void OnKeyPressed(object sender, KeyEventArgs e)
        {
            RenderWindow window = (RenderWindow)sender;
            if (e.Code == KeyCode.Escape)
            {
                // Close the window
                window.Close();
            }
            else if (e.Code == KeyCode.Add)
            {
                // Advance to the next effect
                if (CurrentEffect.MoveNext() == false)
                {
                    CurrentEffect = Effects.GetEnumerator();
                    CurrentEffect.MoveNext();
                }
                CurFXStr.Text = "Current effect is \"" + CurrentEffect.Current.Key + "\"";
            }
        }
    }
}
