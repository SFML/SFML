#!/usr/bin/python

from PySFML import *
import math
import random
import sys


def Game(Difficulty):

	PartsPerFrame = 1 + Difficulty # Number of drawn base parts each frame
	PartsSpacing = 3 # Each worm's base part is separated by PartsSpacing pixels
	TurnStep = 0.15 # Turn the worm's head of 0.15 rad

	PartSize = 6.0 # worm's base part size for collision
	PartRealSize = 18.0 # worm's real base part size for drawing

	# Load images
	Rond = sf.Image() # Image containing the base part of the worm
	if not Rond.LoadFromFile("./data/rond2.png"):
		print "Could not load data/rond2.png"
		return
	WormPart = sf.Sprite(Rond)
	WormPart.SetCenter(Rond.GetWidth()/2, Rond.GetHeight()/2)
	AppleImg = sf.Image() # Apple's image
	if not AppleImg.LoadFromFile("./data/apple.png"):
		print "Could not load data/apple.png"
		return
	Apple = sf.Sprite(AppleImg, 0, 0, 1, 1, 0) # Corresponding sprite
	Black = sf.Color(0,0,0,255)
	UglyYellow = sf.Color(220, 220, 20, 255)

	Stop = False

	Event = sf.Event() # Our events manager

	Level = 0
	ShrinkValue = 20

	Border = 30
	ArenaTop = 20
	ArenaBottom = 520

	RequiredLength = 300

	ExitLeft = 350
	ExitRight = 450
	ExitImg = sf.Image(ExitRight-ExitLeft, ArenaTop, Black)
	Exit = sf.Sprite(ExitImg, ExitLeft, 0, 1, 1, 0)

	Score = 0

	HeadX, HeadY = 0, 0

	while not Stop:

		#Initialize a new game

		Level += 1

		ArenaLeft = ShrinkValue*Level
		ArenaRight = 800-ShrinkValue*Level
		ArenaImg = sf.Image(ArenaRight-ArenaLeft, ArenaBottom-ArenaTop, Black)
		Arena = sf.Sprite(ArenaImg, ArenaLeft, ArenaTop, 1, 1, 0)

		AppleX, AppleY = random.randrange(ArenaLeft+Border, ArenaRight-Border), random.randrange(ArenaTop+Border, ArenaBottom-Border)
		Apple.SetX(AppleX - AppleImg.GetWidth()/2) # We move the apple to somewhere else, randomly
		Apple.SetY(AppleY - AppleImg.GetHeight()/2)

		Crash = False
		Running = True

		LevelStr = sf.String("Level: " + str(Level))
		LevelStr.SetPosition(60., 540.)
		LevelStr.SetColor(UglyYellow)

		ScoreStr = sf.String("Score: 0")
		ScoreStr.SetPosition(260., 540.)
		ScoreStr.SetColor(UglyYellow)

		Length = 1
		TargetedLength = 30

		Worm = [[ArenaLeft+50., ArenaTop+50.]]

		Angle = 0
		i = 0
		Dir = 0

		while Running: # Game main loop
			while App.GetEvent(Event): # Event Handler
				if Event.Type == sf.Event.Closed:
					App.Close()
					return
				if Event.Type == sf.Event.KeyPressed:
					if Event.Key.Code == sf.Key.Escape:
						Running = False
						Stop = True
					if Event.Key.Code == sf.Key.Left:
						Dir = -1
					if Event.Key.Code == sf.Key.Right:
						Dir = 1
					if Crash and Length<=1:
						Running = False
				if Event.Type == sf.Event.KeyReleased:
					if Event.Key.Code == sf.Key.Left and Dir == -1:
						Dir = 0
					if Event.Key.Code == sf.Key.Right and Dir == 1:
						Dir = 0

			App.Draw(Arena)


			if not Crash: # Create new parts and check collisions if the worm hasn't crashed yet
				for i in range(0, PartsPerFrame): # We create PartsPerFrame Worm's parts
					Angle += Dir*TurnStep
					HeadX, HeadY = Worm[Length-1][0]+PartsSpacing*math.cos(Angle), Worm[Length-1][1]+PartsSpacing*math.sin(Angle)
					if TargetedLength <= RequiredLength:
						if math.sqrt ( (AppleX - HeadX)**2 + (AppleY - HeadY)**2 ) < 14 + PartSize/2: # The Worm ate the apple
							Score += 1
							TargetedLength += 20 # The worm gets longer
							if TargetedLength <= RequiredLength:
								AppleX, AppleY = random.randrange(ArenaLeft+Border, ArenaRight-Border), random.randrange(ArenaTop+Border, ArenaBottom-Border)
								Apple.SetX(AppleX - AppleImg.GetWidth()/2) # We move the apple to somewhere else, randomly
								Apple.SetY(AppleY - AppleImg.GetHeight()/2)
						App.Draw(Apple)

					if HeadX<ArenaLeft+PartSize/2 or HeadX>ArenaRight-PartSize/2 or HeadY<ArenaTop+PartSize/2 or HeadY>ArenaBottom-PartSize/2: # Crash into a wall
						if Length > RequiredLength:
							if HeadY<ArenaTop+PartSize/2:
								if HeadX<ExitLeft+PartSize/2 or HeadX>ExitRight-PartSize/2:
									Crash = True
								elif HeadY < 0:
									Length = 0
									Running = False # Level completed!
							else:
								Crash = True
						elif Running:
							Crash = True
					if not Crash:
						Worm.append([HeadX, HeadY])
						Length += 1


			if TargetedLength > RequiredLength:
				App.Draw(Exit)

			if Length >= TargetedLength:
				Worm[0:TargetedLength] = Worm[Length-TargetedLength:Length]
				for i in range(Length, TargetedLength):
					del Worm[i]
				Worm[TargetedLength:Length] = []
				Length = TargetedLength

			for i in range(0, Length):
				WormPart.SetPosition(Worm[i][0], Worm[i][1])
				App.Draw(WormPart) # Draw the part on screen
				if i < Length - PartSize/PartsSpacing - 1:
					if math.sqrt( (HeadX-Worm[i][0])**2 + (HeadY-Worm[i][1])**2 ) < PartSize and Running: # Check for collision
						Crash = True

			if Crash and Length>0:
				TargetedLength -= PartsPerFrame


			ScoreStr.SetText("Score: " + str(Score))

			App.Draw(ScoreStr)
			App.Draw(LevelStr)
			App.Display() # Refresh Screen
			App.Clear(BGColor)


		# End of the game
		if Crash:
			Level = 0
			Score = 0
		else:
			Score += 5 # End level bonus

		del Worm
		del Arena
		del ArenaImg

def Menu():

	Selection = 0

	TextColor = sf.Color(220, 220, 20, 255)

	Running = True
	Event = sf.Event()

	Title = sf.String("PyWorm!")
	Title.SetX(320.)
	Title.SetY(50.)
	Title.SetColor(TextColor)

	Levels = ["Very Easy", "Easy", "Medium", "Hard"]
	Xs = [320., 350., 330., 350.]
	Strings = [0,0,0,0]
	for i in range(0, 4):
		Strings[i] = sf.String(Levels[i])
		Strings[i].SetColor(TextColor)
		Strings[i].SetPosition(Xs[i], 200. + 80*i)

	RectangleImg = sf.Image(ScreenWidth, 40, sf.Color(50,50,10,255))
	Rectangle = sf.Sprite(RectangleImg, 0, 350, 1, 1, 0)

	while App.IsOpened(): # Game main loop
		while App.GetEvent(Event): # Event Handler
			if Event.Type == sf.Event.Closed:
				App.Close()
			if Event.Type == sf.Event.KeyPressed:
				if Event.Key.Code == sf.Key.Escape:
					App.Close()
				elif Event.Key.Code == sf.Key.Up:
					Selection = (Selection - 1) % 4
				elif Event.Key.Code == sf.Key.Down:
					Selection = (Selection + 1) % 4
				elif Event.Key.Code == sf.Key.Return:
					Game(Selection)

		Rectangle.SetY(200 + Selection*80)
		App.Draw(Rectangle)
		App.Draw(Title)
		for i in range(0,4):
			App.Draw(Strings[i])
		App.Display()
		App.Clear(BGColor)
	


# Initialize the window
ScreenWidth, ScreenHeight = 800, 600
App = sf.RenderWindow(sf.VideoMode(ScreenWidth,ScreenHeight,32), "PyWorm", sf.Style.Close) # Creates the window
BGColor = sf.Color(100,100,0,255)
App.SetFramerateLimit(30)
Menu()

