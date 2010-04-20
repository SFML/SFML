module opengl;

import dsfml.system.all;
import dsfml.audio.all;
import dsfml.window.all;
import dsfml.graphics.all;

import std.perf;

import derelict.opengl.gl;
import derelict.opengl.glu;

void main()
{
	// Create the main window
	auto window = new RenderWindow(VideoMode(800, 600), "DSFML OpenGL", Style.Default, ContextSettings(24,8,0,3,1));

	DerelictGL.load();
	DerelictGLU.load();
	
	// Create a sprite for the background
	auto backgroundImage = new Image("Data/opengl/background.jpg");
	auto background = new Sprite(backgroundImage);

	// Load an OpenGL texture.
	// We could directly use an Image as an OpenGL texture (with its Bind() member function),
	// but here we want more control on it (generate mipmaps, ...) so we create a new one from the image pixels
	GLuint texture = 0;
	{
		auto image = new Image("Data/opengl/texture.jpg");

		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, image.width, image.height, GL_RGBA, GL_UNSIGNED_BYTE, image.getPixelsArray().ptr);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	}

	// Enable Z-buffer read and write
	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);
	glClearDepth(1.f);

	// Setup a perspective projection
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(90.f, 1.f, 1.f, 500.f);

	// Bind our texture
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture);
	glColor4f(1.f, 1.f, 1.f, 1.f);

	// Create a clock for measuring the time elapsed
	auto clock = new PerformanceCounter();
	clock.start();
	
	// Start game loop
	while (window.isOpened())
	{
		// Process events
		Event event;
		while (window.getEvent(event))
		{
			// Close window : exit
			if (event.Type == EventType.Closed)
				window.close();

			// Escape key : exit
			if ((event.Type == EventType.KeyPressed) && (event.Key.Code == KeyCode.Escape))
				window.close();

			// Adjust the viewport when the window is resized
			if (event.Type == EventType.Resized)
				glViewport(0, 0, event.Size.Width, event.Size.Height);
		}

		// Draw the background
		window.saveGLStates();
		window.draw(background);
		window.restoreGLStates();

		// Activate the window before using OpenGL commands.
		// This is useless here because we have only one window which is
		// always the active one, but don't forget it if you use multiple windows
		window.active = true;

		// Clear the depth buffer
		glClear(GL_DEPTH_BUFFER_BIT);

		// We get the position of the mouse cursor, so that we can move the box accordingly
		float x =  window.input.mouseX * 200.f / window.width  - 100.f;
		float y = -window.input.mouseY * 200.f / window.height + 100.f;

		// Apply some transformations
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glTranslatef(x, y, -100.f);
		
		clock.stop();
		glRotatef(clock.microseconds() * 0.000001 * 50, 1.f, 0.f, 0.f);
		glRotatef(clock.microseconds() * 0.000001 * 30, 0.f, 1.f, 0.f);
		glRotatef(clock.microseconds() * 0.000001 * 90, 0.f, 0.f, 1.f);

		// Draw a cube
		float size = 20.f;
		glBegin(GL_QUADS);

			glTexCoord2f(0, 0); glVertex3f(-size, -size, -size);
			glTexCoord2f(0, 1); glVertex3f(-size,  size, -size);
			glTexCoord2f(1, 1); glVertex3f( size,  size, -size);
			glTexCoord2f(1, 0); glVertex3f( size, -size, -size);

			glTexCoord2f(0, 0); glVertex3f(-size, -size, size);
			glTexCoord2f(0, 1); glVertex3f(-size,  size, size);
			glTexCoord2f(1, 1); glVertex3f( size,  size, size);
			glTexCoord2f(1, 0); glVertex3f( size, -size, size);

			glTexCoord2f(0, 0); glVertex3f(-size, -size, -size);
			glTexCoord2f(0, 1); glVertex3f(-size,  size, -size);
			glTexCoord2f(1, 1); glVertex3f(-size,  size,  size);
			glTexCoord2f(1, 0); glVertex3f(-size, -size,  size);

			glTexCoord2f(0, 0); glVertex3f(size, -size, -size);
			glTexCoord2f(0, 1); glVertex3f(size,  size, -size);
			glTexCoord2f(1, 1); glVertex3f(size,  size,  size);
			glTexCoord2f(1, 0); glVertex3f(size, -size,  size);

			glTexCoord2f(0, 1); glVertex3f(-size, -size,  size);
			glTexCoord2f(0, 0); glVertex3f(-size, -size, -size);
			glTexCoord2f(1, 0); glVertex3f( size, -size, -size);
			glTexCoord2f(1, 1); glVertex3f( size, -size,  size);

			glTexCoord2f(0, 1); glVertex3f(-size, size,  size);
			glTexCoord2f(0, 0); glVertex3f(-size, size, -size);
			glTexCoord2f(1, 0); glVertex3f( size, size, -size);
			glTexCoord2f(1, 1); glVertex3f( size, size,  size);

		glEnd();

		// Draw some text on top of our OpenGL object
		window.saveGLStates();
		Text text = new Text("DSFML / OpenGL demo"c);
		text.position = Vector2f(250.f, 450.f);
		text.color = Color(255, 255, 255, 170);
		window.draw(text);
		window.restoreGLStates();

		// Finally, display the rendered frame on screen
		window.display();
	}

	// Don't forget to destroy our texture
	glDeleteTextures(1, &texture);
}