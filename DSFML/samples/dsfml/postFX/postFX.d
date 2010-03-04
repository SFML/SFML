module postfx;

import dsfml.graphics.all;
import dsfml.system.all;
import dsfml.window.all;

const char[][5] EFFECTS = ["nothing", "blur", "colorize", "fisheye", "wave"];

void main()
{
	int actualIndex;
	
	// Check that the system can use post effects
	if (PostFX.canUsePostFX() == false)
		assert(0, "Your system doesn't support Post Effects.");

	// Create the main window
	RenderWindow app = new RenderWindow(VideoMode(800, 600), "SFML PostFX");
	app.setFramerateLimit(100);

	// Load a cute background image to display :)
	Sprite background = new Sprite(new Image("Data/background.jpg"));

	// Load the image needed for the wave effect
	Image WaveImage = new Image("Data/wave.jpg");

	// Load all effects
	PostFX[char[]] Effects;
	foreach(char[] c; EFFECTS)
	{
		Effects[c] = new PostFX("Data/" ~ c ~ ".sfx");
	}
	PostFX currentEffect = Effects[EFFECTS[actualIndex]];

	// Do specific initializations
	Effects["nothing"].setTexture("framebuffer", null);
	Effects["blur"].setTexture("framebuffer", null);
	Effects["blur"].setParameter("offset", 0.f);
	Effects["colorize"].setTexture("framebuffer", null);
	Effects["colorize"].setParameter("color", 1.f, 1.f, 1.f);
	Effects["fisheye"].setTexture("framebuffer", null);
	Effects["wave"].setTexture("framebuffer", null);
	Effects["wave"].setTexture("wave", WaveImage);

	Font f = new Font("Data/cheeseburger.ttf");
	
	// Define a string for displaying current effect description
	Text curFXStr = new Text("Current effect is " ~ EFFECTS[actualIndex]);
	curFXStr.setFont(f);
	curFXStr.setPosition(20.f, 0.f);

	// Define a string for displaying help
	Text infoStr = new Text("Move your mouse to change the effect parameters\nPress numpad + and - to change effect\nWarning : some effects may not work\ndepending on your graphics card"c);
	infoStr.setFont(f);
	infoStr.setPosition(20.f, 460.f);
	infoStr.setColor(Color(200, 100, 150));
	
	// Start the game loop
	while (app.isOpened())
	{
		// Process events
		Event evt;
		while (app.getEvent(evt))
		{
			// Close window : exit
			if (evt.Type == Event.EventType.CLOSED ||
					evt.Type == Event.EventType.KEYPRESSED && evt.Key.Code == KeyCode.ESCAPE)
				app.close();

			if (evt.Type == Event.EventType.KEYPRESSED)
			{
				// Add key : next effect
				if (evt.Key.Code == KeyCode.ADD)
				{
					if (actualIndex == 4)
						actualIndex = 0;
					else
						actualIndex++;
					currentEffect = Effects[EFFECTS[actualIndex]];
					curFXStr.setText("Current effect is " ~ EFFECTS[actualIndex]);
				}

				// Subtract key : previous effect
				if (evt.Key.Code == KeyCode.SUBTRACT)
				{
					if (actualIndex == 0)
						actualIndex = 4;
					else
						actualIndex--;
					currentEffect = Effects[EFFECTS[actualIndex]];
					curFXStr.setText("Current effect is " ~ EFFECTS[actualIndex]);
				}
			}
		}

		// Get the mouse position in the range [0, 1]
		float X = app.getInput().getMouseX() / cast(float) app.getWidth();
		float Y = app.getInput().getMouseY() / cast(float) app.getHeight();

		// Update the current effect
		if	  (EFFECTS[actualIndex] == "blur")	 currentEffect.setParameter("offset", X * Y * 0.1f);
		else if (EFFECTS[actualIndex] == "colorize") currentEffect.setParameter("color", 0.3f, X, Y);
		else if (EFFECTS[actualIndex] == "fisheye")  currentEffect.setParameter("mouse", X, 1.f - Y);
		else if (EFFECTS[actualIndex] == "wave")	 currentEffect.setParameter("offset", X, Y);

		// Draw background and apply the post-fx
		app.draw(background);
		app.draw(currentEffect);

		// Draw interface strings
		app.draw(curFXStr);
		app.draw(infoStr);

		// Finally, display the rendered frame on screen
		app.display();
	}
}
