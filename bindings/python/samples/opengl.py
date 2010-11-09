#!/usr/bin/python

from PySFML import sf

from OpenGL.GL import *
from OpenGL.GLUT import *
from OpenGL.GLU import *


def main():

	# Create main window
	App = sf.RenderWindow(sf.VideoMode(800, 600), "SFML OpenGL")
	App.SetActive()

	# Create a sprite for the background
	BackgroundImage = sf.Image()
	if not BackgroundImage.LoadFromFile("../../samples/bin/datas/opengl/background.jpg"):
		return
	Background = sf.Sprite(BackgroundImage)

	# Load an OpenGL texture.
	# We could directly use a sf.Image as an OpenGL texture (with its Bind() member function),
	# but here we want more control on it (generate mipmaps, ...) so we create a new one

	Image = sf.Image()
	if not Image.LoadFromFile("../../samples/bin/datas/opengl/texture.jpg"):
		return
	# The next line is a bit different from the C++ version
	Texture = glGenTextures(1) # instead of glGenTextures(1, &Texture);
	glBindTexture(GL_TEXTURE_2D, Texture)
	# It is almost the same line there, except in C++, the last argument was Image.GetPixelsPtr().
	# In python, GetPixels simply returns a string.
	gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, Image.GetWidth(), Image.GetHeight(), GL_RGBA, GL_UNSIGNED_BYTE, Image.GetPixels())
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR)


	# Enable Z-buffer read and write
	glEnable(GL_DEPTH_TEST)
	glDepthMask(GL_TRUE)
	glClearDepth(1.)

	# Setup a perspective projection
	glMatrixMode(GL_PROJECTION)
	glLoadIdentity()
	gluPerspective(90., 1., 1., 500.)

	# Bind our texture
	glEnable(GL_TEXTURE_2D)
	glBindTexture(GL_TEXTURE_2D, Texture)
	glColor4f(1., 1., 1., 1.)

	# Create a clock for measuring the time elapsed
	Clock = sf.Clock()

    # Start game loop
	while App.IsOpened():
		# Process events
		Event = sf.Event()
		while App.GetEvent(Event):
			# Close window : exit
			if Event.Type == sf.Event.Closed:
				App.Close()

			# Escape key : exit
			if (Event.Type == sf.Event.KeyPressed) and (Event.Key.Code == sf.Key.Escape):
				App.Close()

			# Adjust the viewport when the window is resized
			if Event.Type == sf.Event.Resized:
				glViewport(0, 0, Event.Size.Width, Event.Size.Height)

		# Draw background
		App.Draw(Background)

		# Active window to be able to perform OpenGL commands.
		App.SetActive()

		# Clear depth buffer
		glClear(GL_DEPTH_BUFFER_BIT)

		# Apply some transformations
		glMatrixMode(GL_MODELVIEW)
		glLoadIdentity()
		glTranslatef(0, 0, -200)
		glRotatef(Clock.GetElapsedTime() * 50, 1, 0, 0)
		glRotatef(Clock.GetElapsedTime() * 30, 0, 1, 0)
		glRotatef(Clock.GetElapsedTime() * 90, 0, 0, 1)

		# Draw a cube
		glBegin(GL_QUADS)

		glTexCoord2f(0, 0)
		glVertex3f(-50., -50., -50.)
		glTexCoord2f(0, 1)
		glVertex3f(-50.,  50., -50.)
		glTexCoord2f(1, 1)
		glVertex3f( 50.,  50., -50.)
		glTexCoord2f(1, 0)
		glVertex3f( 50., -50., -50.)

		glTexCoord2f(0, 0)
		glVertex3f(-50., -50., 50.)
		glTexCoord2f(0, 1)
		glVertex3f(-50.,  50., 50.)
		glTexCoord2f(1, 1)
		glVertex3f( 50.,  50., 50.)
		glTexCoord2f(1, 0)
		glVertex3f( 50., -50., 50.)

		glTexCoord2f(0, 0)
		glVertex3f(-50., -50., -50.)
		glTexCoord2f(0, 1)
		glVertex3f(-50.,  50., -50.)
		glTexCoord2f(1, 1)
		glVertex3f(-50.,  50.,  50.)
		glTexCoord2f(1, 0)
		glVertex3f(-50., -50.,  50.)

		glTexCoord2f(0, 0)
		glVertex3f(50., -50., -50.)
		glTexCoord2f(0, 1)
		glVertex3f(50.,  50., -50.)
		glTexCoord2f(1, 1)
		glVertex3f(50.,  50.,  50.)
		glTexCoord2f(1, 0)
		glVertex3f(50., -50.,  50.)

		glTexCoord2f(0, 1)
		glVertex3f(-50., -50.,  50.)
		glTexCoord2f(0, 0)
		glVertex3f(-50., -50., -50.)
		glTexCoord2f(1, 0)
		glVertex3f( 50., -50., -50.)
		glTexCoord2f(1, 1)
		glVertex3f( 50., -50.,  50.)

		glTexCoord2f(0, 1)
		glVertex3f(-50., 50.,  50.)
		glTexCoord2f(0, 0)
		glVertex3f(-50., 50., -50.)
		glTexCoord2f(1, 0)
		glVertex3f( 50., 50., -50.)
		glTexCoord2f(1, 1)
		glVertex3f( 50., 50.,  50.)

		glEnd()

		# Draw some text on top of our OpenGL object
		Text = sf.Text("This is a rotating cube")
		Text.SetPosition(230., 300.)
		Text.SetColor(sf.Color(128, 0, 128))
		App.Draw(Text)

		# Finally, display the rendered frame on screen
		App.Display()

    # Don't forget to destroy our texture
	# In C++, the call to this function was a bit different
	glDeleteTextures(Texture) # instead of glDeleteTextures(1, &Texture);

	return
main()

