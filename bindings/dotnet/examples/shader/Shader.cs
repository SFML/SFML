using System;
using System.Collections.Generic;
using SFML;
using SFML.Graphics;
using SFML.Window;

namespace shader
{
    /// <summary>
    /// A class to simplify shader selection
    /// </summary>
    class ShaderSelector
    {
        // Constructor
        public ShaderSelector(Dictionary<string, Shader> owner)
        {
            myOwner = owner;
            myIterator = owner.GetEnumerator();
            myIterator.MoveNext();
        }

        // Select the next shader
        public void GotoNext()
        {
            if (myIterator.MoveNext() == false)
            {
                myIterator = myOwner.GetEnumerator();
                myIterator.MoveNext();
            }
        }

        // Update the shader parameters
        public void Update(float x, float y)
        {
            if      (myIterator.Current.Key == "blur")     myIterator.Current.Value.SetParameter("offset", x * y * 0.05f);
            else if (myIterator.Current.Key == "colorize") myIterator.Current.Value.SetParameter("color", 0.3f, x, y);
            else if (myIterator.Current.Key == "fisheye")  myIterator.Current.Value.SetParameter("mouse", x, y);
            else if (myIterator.Current.Key == "wave")     myIterator.Current.Value.SetParameter("offset", x, y);
            else if (myIterator.Current.Key == "pixelate") myIterator.Current.Value.SetParameter("mouse", x, y);
        }

        // Get the name of the current shader
        public string Name
        {
            get {return myIterator.Current.Key;}
        }

        // Get the current shader
        public Shader Shader
        {
            get {return myIterator.Current.Value;}
        }

        private Dictionary<string, Shader> myOwner;
        private Dictionary<string, Shader>.Enumerator myIterator;
    };

    static class Program
    {
        private static Dictionary<string, Shader> shaders;
        private static ShaderSelector             backgroundShader;
        private static ShaderSelector             entityShader;
        private static ShaderSelector             globalShader;
        private static Text                       shaderText;

        /// <summary>
        /// The main entry point for the application.
        /// </summary>
        static void Main()
        {
            // Create the main window
            RenderWindow window = new RenderWindow(new VideoMode(800, 600), "SFML.Net Shader");

            // Setup event handlers
            window.Closed     += new EventHandler(OnClosed);
            window.KeyPressed += new EventHandler<KeyEventArgs>(OnKeyPressed);

            // Check that the system can use shaders
            if (Shader.IsAvailable == false)
            {
                DisplayError(window);
                return;
            }

            // Create the render image
            RenderImage image = new RenderImage(window.Width, window.Height);

            // Load a background image to display
            Sprite background = new Sprite(new Image("resources/background.jpg"));
            background.Image.Smooth = false;

            // Load a sprite which we'll move into the scene
            Sprite entity = new Sprite(new Image("resources/sprite.png"));

            // Load the text font
            Font font = new Font("resources/arial.ttf");

            // Load the image needed for the wave effect
            Image waveImage = new Image("resources/wave.jpg");

            // Load all effects
            shaders = new Dictionary<string, Shader>();
            shaders["nothing"]  = new Shader("resources/nothing.sfx");
            shaders["blur"]     = new Shader("resources/blur.sfx");
            shaders["colorize"] = new Shader("resources/colorize.sfx");
            shaders["fisheye"]  = new Shader("resources/fisheye.sfx");
            shaders["wave"]     = new Shader("resources/wave.sfx");
            shaders["pixelate"] = new Shader("resources/pixelate.sfx");
            backgroundShader = new ShaderSelector(shaders);
            entityShader     = new ShaderSelector(shaders);
            globalShader     = new ShaderSelector(shaders);

            // Do specific initializations
            shaders["nothing"].SetCurrentTexture("texture");
            shaders["blur"].SetCurrentTexture("texture");
            shaders["blur"].SetParameter("offset", 0.0F);
            shaders["colorize"].SetCurrentTexture("texture");
            shaders["colorize"].SetParameter("color", 1.0F, 1.0F, 1.0F);
            shaders["fisheye"].SetCurrentTexture("texture");
            shaders["wave"].SetCurrentTexture("texture");
            shaders["wave"].SetTexture("wave", waveImage);
            shaders["pixelate"].SetCurrentTexture("texture");

            // Define a string for displaying current effect description
            shaderText = new Text();
            shaderText.Font = font;
            shaderText.CharacterSize = 20;
            shaderText.Position = new Vector2(5.0F, 0.0F);
            shaderText.Color = new Color(250, 100, 30);
            shaderText.DisplayedString = "Background shader: \"" + backgroundShader.Name + "\"\n" +
                                         "Flower shader: \"" + entityShader.Name + "\"\n" +
                                         "Global shader: \"" + globalShader.Name + "\"\n";

            // Define a string for displaying help
            Text infoText = new Text();
            infoText.Font = font;
            infoText.CharacterSize = 20;
            infoText.Position = new Vector2(5.0F, 500.0F);
            infoText.Color = new Color(250, 100, 30);
            infoText.DisplayedString = "Move your mouse to change the shaders' parameters\n" +
                                       "Press numpad 1 to change the background shader\n" +
                                       "Press numpad 2 to change the flower shader\n" +
                                       "Press numpad 3 to change the global shader";

            // Start the game loop
            float time = 0.0F;
            while (window.IsOpened())
            {
                // Process events
                window.DispatchEvents();

                // TOFIX -- using window.Input together with image.Draw apparently causes a memory corruption
                // Get the mouse position in the range [0, 1]
                //float x = window.Input.GetMouseX() / (float)window.Width;
                //float y = window.Input.GetMouseY() / (float)window.Height;
                float x = (float)(Math.Cos(time * 1.3) + 1) * 0.5F;
                float y = (float)(Math.Sin(time * 0.8) + 1) * 0.5F;

                // Update the shaders
                backgroundShader.Update(x, y);
                entityShader.Update(x, y);
                globalShader.Update(x, y);

                // Animate the sprite
                time += window.GetFrameTime();
                float entityX = (float)(Math.Cos(time * 1.3) + 1.2) * 300;
                float entityY = (float)(Math.Cos(time * 0.8) + 1.2) * 200;
                entity.Position = new Vector2(entityX, entityY);
                entity.Rotation = time * 100;

                // Draw the background and the moving entity to the render image
                image.Draw(background, backgroundShader.Shader);
                image.Draw(entity, entityShader.Shader);
                image.Display();

                // Draw the contents of the render image to the window
                window.Draw(new Sprite(image.Image), globalShader.Shader);

                // Draw interface texts
                window.Draw(shaderText);
                window.Draw(infoText);

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
            Text error = new Text("Sorry, your system doesn't support shaders");
            error.Position = new Vector2(100.0F, 250.0F);
            error.Color = new Color(200, 100, 150);

            // Start the game loop
            while (window.IsOpened())
            {
                // Process events
                window.DispatchEvents();

                // Clear the window
                window.Clear();

                // Draw the error message
                window.Draw(error);

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

            // Escape key : exit
            if (e.Code == KeyCode.Escape)
                window.Close();

            // Numpad : switch effect
            switch (e.Code)
            {
                case KeyCode.Numpad1 : backgroundShader.GotoNext(); break;
                case KeyCode.Numpad2 : entityShader.GotoNext();     break;
                case KeyCode.Numpad3 : globalShader.GotoNext();     break;
            }

            // Update the text
            shaderText.DisplayedString = "Background shader: \"" + backgroundShader.Name + "\"\n" +
                                         "Flower shader: \"" + entityShader.Name + "\"\n" +
                                         "Global shader: \"" + globalShader.Name + "\"\n";
        }
    }
}
