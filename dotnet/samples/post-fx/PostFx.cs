using System;
using System.Collections.Generic;
using SFML;
using SFML.Graphics;
using SFML.Window;

namespace sample_postfx
{
    static class Program
    {
        private static Dictionary<string, PostFx>            effects;
        private static Dictionary<string, PostFx>.Enumerator currentEffect;
        private static String2D                              curFXStr;

        /// <summary>
        /// The main entry point for the application.
        /// </summary>
        static void Main()
        {
            // Create the main window
            RenderWindow window = new RenderWindow(new VideoMode(800, 600), "SFML.Net PostFX");

            // Setup event handlers
            window.Closed     += new EventHandler(OnClosed);
            window.KeyPressed += new EventHandler<KeyEventArgs>(OnKeyPressed);

            // Check that the system can use post effects
            if (PostFx.CanUsePostFX == false)
            {
                DisplayError(window);
                return;
            }

            // Load a background image to display
            Sprite background = new Sprite(new Image("datas/post-fx/background.jpg"));

            // Load a sprite which we'll move into the scene
            Sprite entity = new Sprite(new Image("datas/post-fx/sprite.png"));

            // Load the text font
            Font cheeseburger = new Font("datas/post-fx/cheeseburger.ttf");

            // Load the image needed for the wave effect
            Image waveImage = new Image("datas/post-fx/wave.jpg");

            // Load all effects
            effects = new Dictionary<string, PostFx>();
            effects["nothing"]  = new PostFx("datas/post-fx/nothing.sfx");
            effects["blur"]     = new PostFx("datas/post-fx/blur.sfx");
            effects["colorize"] = new PostFx("datas/post-fx/colorize.sfx");
            effects["fisheye"]  = new PostFx("datas/post-fx/fisheye.sfx");
            effects["wave"]     = new PostFx("datas/post-fx/wave.sfx");
            effects["pixelate"] = new PostFx("datas/post-fx/pixelate.sfx");
            currentEffect = effects.GetEnumerator();
            currentEffect.MoveNext();

            // Do specific initializations
            effects["nothing"].SetTexture("framebuffer", null);
            effects["blur"].SetTexture("framebuffer", null);
            effects["blur"].SetParameter("offset", 0.0F);
            effects["colorize"].SetTexture("framebuffer", null);
            effects["colorize"].SetParameter("color", 1.0F, 1.0F, 1.0F);
            effects["fisheye"].SetTexture("framebuffer", null);
            effects["wave"].SetTexture("framebuffer", null);
            effects["wave"].SetTexture("wave", waveImage);
            effects["pixelate"].SetTexture("framebuffer", null);

            // Define a string for displaying current effect description
            curFXStr = new String2D();
            curFXStr.Text = "Current effect is \"" + currentEffect.Current.Key + "\"";
            curFXStr.Font = cheeseburger;
            curFXStr.Position = new Vector2(20.0F, 0.0F);
            curFXStr.Color = new Color(150, 70, 110);

            // Define a string for displaying help
            String2D infoStr = new String2D();
            infoStr.Text = "Move your mouse to change the effect parameters\nPress numpad + to change effect\nWarning : some effects may not work\ndepending on your graphics card";
            infoStr.Font = cheeseburger;
            infoStr.Position = new Vector2(20.0F, 460.0F);
            infoStr.Color = new Color(200, 100, 150);

            // Start the game loop
            float time = 0.0F;
            while (window.IsOpened())
            {
                // Process events
                window.DispatchEvents();

                // Get the mouse position in the range [0, 1]
                float x = window.Input.GetMouseX() / (float)window.Width;
                float y = window.Input.GetMouseY() / (float)window.Height;

                // Update the current effect
                if      (currentEffect.Current.Key == "blur")     currentEffect.Current.Value.SetParameter("offset", x * y * 0.1f);
                else if (currentEffect.Current.Key == "colorize") currentEffect.Current.Value.SetParameter("color", 0.3f, x, y);
                else if (currentEffect.Current.Key == "fisheye")  currentEffect.Current.Value.SetParameter("mouse", x, 1.0F - y);
                else if (currentEffect.Current.Key == "wave")     currentEffect.Current.Value.SetParameter("offset", x, y);
                else if (currentEffect.Current.Key == "pixelate") currentEffect.Current.Value.SetParameter("mouse", x, y);

                // Animate the sprite
                time += window.GetFrameTime();
                float entityX = (float)(Math.Cos(time * 1.3) + 1.2) * 300;
                float entityY = (float)(Math.Cos(time * 0.8) + 1.2) * 200;
                entity.Position = new Vector2(entityX, entityY);
                entity.Rotation = time * 100;

                // Clear the window
                window.Clear();

                // Draw background, the sprite and apply the post-fx
                window.Draw(background);
                window.Draw(entity);
                window.Draw(currentEffect.Current.Value);

                // Draw interface strings
                window.Draw(curFXStr);
                window.Draw(infoStr);

                // Finally, display the rendered frame on screen
                window.Display();
            }
        }

        /// <summary>
        /// Fonction called when the post-effects are not supported ;
        /// Display an error message and wait until the user exits
        /// </summary>
        private static void DisplayError(RenderWindow window)
        {
            // Define a string for displaying the error message
            String2D errorStr = new String2D("Sorry, your system doesn't support post-effects");
            errorStr.Position = new Vector2(100.0F, 250.0F);
            errorStr.Color = new Color(200, 100, 150);

            // Start the game loop
            while (window.IsOpened())
            {
                // Process events
                window.DispatchEvents();

                // Clear the window
                window.Clear();

                // Draw the error message
                window.Draw(errorStr);

                // Finally, display the rendered frame on screen
                window.Display();
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
                if (currentEffect.MoveNext() == false)
                {
                    currentEffect = effects.GetEnumerator();
                    currentEffect.MoveNext();
                }
                curFXStr.Text = "Current effect is \"" + currentEffect.Current.Key + "\"";
            }
        }
    }
}
