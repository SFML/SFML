module pong;

import dsfml.system.all;
import dsfml.audio.all;
import dsfml.window.all;
import dsfml.graphics.all;

version (Tango)
{
	import tango.io.Stdout;
	import tango.math.Math;
}
else
{
	import std.math;
}

void main()
{
	// Defines PI
	const float PI = 3.14159f;

	// Create the window of the application
	RenderWindow app = new RenderWindow(VideoMode(800, 600, 32), "SFML Pong");

	app.useVerticalSync(true);
	
	Input i = app.getInput();
	
	// Load the sounds used in the game
	Sound BallSound = new Sound(new SoundBuffer("Data/ball.wav"));

	// Load the images used in the game
	Image PaddleImage = new Image("Data/paddle.tga");
	Image BallImage = new Image("Data/ball.tga");


	// Initialize the end text
	Text End = new Text(""c);
	Font font = new Font("Data/cheeseburger.ttf");
	End.setFont(font);
	End.setCharacterSize(60);
	End.move(150.f, 200.f);
	End.setColor(Color(50, 50, 250));

	// Create the sprites of the background, the paddles and the ball
	Sprite LeftPaddle = new Sprite(PaddleImage);
	Sprite RightPaddle = new Sprite(PaddleImage);
	Sprite Ball = new Sprite(BallImage);

	LeftPaddle.move(10, (app.getView().getHeight() - LeftPaddle.getSize().y) / 2);
	RightPaddle.move(app.getView().getWidth() - RightPaddle.getSize().x - 10, (app.getView().getHeight() - RightPaddle.getSize().y) / 2);
	Ball.move((app.getView().getWidth() - Ball.getSize().x) / 2, (app.getView().getHeight() - Ball.getSize().y) / 2);

	// Define the paddles properties
	Clock AITimer = new Clock();
	const float AITime	 = 0.1f;
	float LeftPaddleSpeed  = 400.f;
	float RightPaddleSpeed = 400.f;

	// Define the ball properties
	float BallSpeed = 400.f;
	float BallAngle = 0.f;
	do
	{
		// Make sure the ball initial angle is not too much vertical
		BallAngle = Randomizer.random(0.f, 2 * PI);
	} while (abs(cos(BallAngle)) < 0.7f);

	bool IsPlaying = true;

	Event evt;
	while (app.isOpened())
	{
		app.clear(Color(255, 255, 255, 255));
		// Handle events
		
		while (app.getEvent(evt))
		{
			// Window closed or escape key pressed : exit
			if ((evt.Type == EventType.Closed) || 
				((evt.Type == EventType.KeyPressed) && (evt.Key.Code == KeyCode.Escape)))
			{
				app.close();
				break;
			}		
		}

		if (IsPlaying)
		{
			// Move the player's paddle
			if (i.isKeyDown(KeyCode.Up) && (LeftPaddle.getPosition().y > 5.f))
				LeftPaddle.move(0.f, -LeftPaddleSpeed * app.getFrameTime());
			if (i.isKeyDown(KeyCode.Down) && (LeftPaddle.getPosition().y < app.getView().getHeight() - LeftPaddle.getSize().y - 5.f))
				LeftPaddle.move(0.f, LeftPaddleSpeed * app.getFrameTime());
			
			// Move the computer's paddle
			if (((RightPaddleSpeed < 0.f) && (RightPaddle.getPosition().y > 5.f)) ||
				((RightPaddleSpeed > 0.f) && (RightPaddle.getPosition().y < app.getView().getHeight() - RightPaddle.getSize().y - 5.f)))
			{
				RightPaddle.move(0.f, RightPaddleSpeed * app.getFrameTime());
			}

			// Update the computer's paddle direction according to the ball position
			if (AITimer.getElapsedTime() > AITime)
			{
				AITimer.reset();
				if ((RightPaddleSpeed < 0) && (Ball.getPosition().y + Ball.getSize().y > RightPaddle.getPosition().y + RightPaddle.getSize().y))
					RightPaddleSpeed = -RightPaddleSpeed;
				if ((RightPaddleSpeed > 0) && (Ball.getPosition().y < RightPaddle.getPosition().y))
					RightPaddleSpeed = -RightPaddleSpeed;
			}



			// Move the ball
			float Factor = BallSpeed * app.getFrameTime();
			Ball.move(cos(BallAngle) * Factor, sin(BallAngle) * Factor);

			// Check collisions between the ball and the screen
			if (Ball.getPosition().x < 0.f)
			{
				IsPlaying = false;
				End.setString("You lost !\n(press escape to exit)"c);
			}

			if (Ball.getPosition().x + Ball.getSize().x > app.getView().getWidth())
			{
				IsPlaying = false;
				End.setString("You won !\n(press escape to exit)"c);
			}

			if (Ball.getPosition().y < 0.f)
			{
				BallSound.play();
				BallAngle = -BallAngle;
				Ball.setY(0.1f);
			}

			if (Ball.getPosition().y + Ball.getSize().y > app.getView().getHeight())
			{
				BallSound.play();
				BallAngle = -BallAngle;
				Ball.setY(app.getView().getHeight() - Ball.getSize().y - 0.1f);
			}
			// Check the collisions between the ball and the paddles
			// Left Paddle
			if (Ball.getPosition().x < LeftPaddle.getPosition().x + LeftPaddle.getSize().x && 
				Ball.getPosition().x > LeftPaddle.getPosition().x + (LeftPaddle.getSize().x / 2.0f) &&
				Ball.getPosition().y + Ball.getSize().y >= LeftPaddle.getPosition().y &&
				Ball.getPosition().y <= LeftPaddle.getPosition().y + LeftPaddle.getSize().y)
			{
				BallSound.play();
				BallAngle = PI - BallAngle;
				Ball.setX(LeftPaddle.getPosition().x + LeftPaddle.getSize().x + 0.1f);
			}

			// Right Paddle
			if (Ball.getPosition().x + Ball.getSize().x > RightPaddle.getPosition().x &&
				Ball.getPosition().x + Ball.getSize().x < RightPaddle.getPosition().x + (RightPaddle.getSize().x / 2.0f) &&
				Ball.getPosition().y + Ball.getSize().y >= RightPaddle.getPosition().y &&
				Ball.getPosition().y <= RightPaddle.getPosition().y + RightPaddle.getSize().y)
			{
				BallSound.play();
				BallAngle = PI - BallAngle;
				Ball.setX(RightPaddle.getPosition().x - Ball.getSize().x - 0.1f);
			}
		}

		// Draw the background, paddles and ball sprites

		app.draw(LeftPaddle);
		app.draw(RightPaddle);
		app.draw(Ball);

		// If the game is over, display the end message
		if (!IsPlaying)
			app.draw(End);

		// Display things on screen
		app.display();
	}
}
