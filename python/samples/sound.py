#!/usr/bin/python

# You can notice that here we use PySFML.sf instead of just PySFML
# Therefore it won't be needed to put sf. in front of SFML classes

from PySFML.sf import *


def Main():
	Buffer = SoundBuffer()
	if not Buffer.LoadFromFile("data/fart.wav"): # Loads the sound
		return
	Fart = Sound(Buffer, False)

	WindowWidth, WindowHeight = 640, 480
	App = RenderWindow(VideoMode(WindowWidth,WindowHeight,32), "Sound with PySFML", Style.Close, WindowSettings(24,8,0))
	App.SetFramerateLimit(30)

	EventHandler = Event()
	InputHandler = App.GetInput()

	Text = String("Turn the sound on.\nClick anywhere on the screen.\nMove the mouse. Click again.\nTry clicking in the corners.")
	Text.SetX(30.)
	Text.SetY(20.)
	Text.SetColor(Color(150, 100, 10, 255))

	while App.IsOpened(): # Main loop
		while App.GetEvent(EventHandler): # Event Handler
			if EventHandler.Type == Event.Closed:
				App.Close()
			if EventHandler.Type == Event.KeyPressed and EventHandler.Key.Code == Key.Escape:
				App.Close()
			if EventHandler.Type == Event.MouseButtonPressed and EventHandler.MouseButton.Button == Mouse.Left:
				Fart.SetPitch(1.5 - 1.*InputHandler.GetMouseY()/WindowHeight)
				Fart.SetPosition( 1.*(InputHandler.GetMouseX() - WindowWidth/2)/(WindowWidth/20), 2., -2.)
				Fart.Play()
		App.Draw(Text)
		App.Display()
		App.Clear(Color.Black)


Main()

