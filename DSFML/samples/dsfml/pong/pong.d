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
	import std.perf;
	import std.random;
}

void main()
{
	// Defines PI
	const float PI = 3.14159f;

	// Create the window of the application
	RenderWindow app = new RenderWindow(VideoMode(800, 600, 32), "SFML Pong");

	app.useVerticalSync(false);
	
	Input i = app.input;
	
	// Load the sounds used in the game
	Sound BallSound = new Sound(new SoundBuffer("Data/ball.wav"));

	// Load the images used in the game
	Image PaddleImage = new Image("Data/paddle.tga");
	Image BallImage = new Image("Data/ball.tga");


	// Initialize the end text
	Text End = new Text(""c);
	Font font = new Font("Data/cheeseburger.ttf");
	End.font = font;
	End.characterSize = 60;
	End.move(150.f, 200.f);
	End.color = Color(50, 50, 250);

	Text fps = new Text(""c, font, 30);
	fps.move(50.f, 50.f);
	fps.color = Color.BLACK;
	
	// Create the sprites of the background, the paddles and the ball
	Sprite LeftPaddle = new Sprite(PaddleImage);
	Sprite RightPaddle = new Sprite(PaddleImage);
	Sprite Ball = new Sprite(BallImage);

	LeftPaddle.move(10, (app.view.getHeight() - LeftPaddle.size.y) / 2);
	RightPaddle.move(app.view.getWidth() - RightPaddle.size.x - 10, (app.view.getHeight() - RightPaddle.size.y) / 2);
	Ball.move((app.view.getWidth() - Ball.size.x) / 2, (app.view.getHeight() - Ball.size.y) / 2);

	// Define the paddles properties
	auto AITimer = new PerformanceCounter();
	const long AITime	 = 100; // 100 ms
	float LeftPaddleSpeed  = 400.f;
	float RightPaddleSpeed = 400.f;

	// Define the ball properties
	float BallSpeed = 400.f;
	float BallAngle = 0.f;
	do
	{
		// Make sure the ball initial angle is not too much vertical
		BallAngle = uniform(0.f, 2 * PI);
	} while (abs(cos(BallAngle)) < 0.7f);

	bool IsPlaying = true;

	Event evt;
	uint iFps = 0;
	auto fpsClock = new PerformanceCounter();
	
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
			if (i.isKeyDown(KeyCode.Up) && (LeftPaddle.position.y > 5.f))
				LeftPaddle.move(0.f, -LeftPaddleSpeed * app.frameTime);
			if (i.isKeyDown(KeyCode.Down) && (LeftPaddle.position.y < app.view.getHeight() - LeftPaddle.size.y - 5.f))
				LeftPaddle.move(0.f, LeftPaddleSpeed * app.frameTime);
			
			// Move the computer's paddle
			if (((RightPaddleSpeed < 0.f) && (RightPaddle.position.y > 5.f)) ||
				((RightPaddleSpeed > 0.f) && (RightPaddle.position.y < app.view.getHeight() - RightPaddle.size.y - 5.f)))
			{
				RightPaddle.move(0.f, RightPaddleSpeed * app.frameTime);
			}

			// Update the computer's paddle direction according to the ball position
			AITimer.stop();
			if (AITimer.milliseconds > AITime)
			{
				AITimer.start();
				if ((RightPaddleSpeed < 0) && (Ball.position.y + Ball.size.y > RightPaddle.position.y + RightPaddle.size.y))
					RightPaddleSpeed = -RightPaddleSpeed;
				if ((RightPaddleSpeed > 0) && (Ball.position.y < RightPaddle.position.y))
					RightPaddleSpeed = -RightPaddleSpeed;
			}



			// Move the ball
			float Factor = BallSpeed * app.frameTime;
			Ball.move(cos(BallAngle) * Factor, sin(BallAngle) * Factor);

			// Check collisions between the ball and the screen
			if (Ball.position.x < 0.f)
			{
				IsPlaying = false;
				End.text("You lost !\n(press escape to exit)"c);
			}

			if (Ball.position.x + Ball.size.x > app.view.getWidth())
			{
				IsPlaying = false;
				End.text("You won !\n(press escape to exit)"c);
			}

			if (Ball.position.y < 0.f)
			{
				BallSound.play();
				BallAngle = -BallAngle;
				Ball.y = 0.1f;
			}

			if (Ball.position.y + Ball.size.y > app.view.getHeight())
			{
				BallSound.play();
				BallAngle = -BallAngle;
				Ball.y = app.view.getHeight() - Ball.size.y - 0.1f;
			}
			// Check the collisions between the ball and the paddles
			// Left Paddle
			if (Ball.position.x < LeftPaddle.position.x + LeftPaddle.size.x && 
				Ball.position.x > LeftPaddle.position.x + (LeftPaddle.size.x / 2.0f) &&
				Ball.position.y + Ball.size.y >= LeftPaddle.position.y &&
				Ball.position.y <= LeftPaddle.position.y + LeftPaddle.size.y)
			{
				BallSound.play();
				BallAngle = PI - BallAngle;
				Ball.x = LeftPaddle.position.x + LeftPaddle.size.x + 0.1f;
			}

			// Right Paddle
			if (Ball.position.x + Ball.size.x > RightPaddle.position.x &&
				Ball.position.x + Ball.size.x < RightPaddle.position.x + (RightPaddle.size.x / 2.0f) &&
				Ball.position.y + Ball.size.y >= RightPaddle.position.y &&
				Ball.position.y <= RightPaddle.position.y + RightPaddle.size.y)
			{
				BallSound.play();
				BallAngle = PI - BallAngle;
				Ball.x = RightPaddle.position.x - Ball.size.x - 0.1f;
			}
		}

		// Draw the background, paddles and ball sprites

		app.draw(LeftPaddle);
		app.draw(RightPaddle);
		app.draw(Ball);

		fpsClock.stop();
		if(fpsClock.seconds >= 1)
		{
			fps.text = std.string.format("%d fps", iFps);
			iFps = 0;
			fpsClock.start();
		}
		++iFps;
		app.draw(fps);
		// If the game is over, display the end message
		if (!IsPlaying)
			app.draw(End);

		// Display things on screen
		app.display();
	}
}