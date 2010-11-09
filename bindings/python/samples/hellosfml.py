# coding=utf-8
from PySFML import sf
import random

# Simple class for an apple.
class Apple:
	sprite = None
	speed = (2, 2)
	rotationstep = 1

	def __init__( self, image ):
		self.sprite = sf.Sprite( image )
		self.sprite.SetOrigin( image.GetWidth() / 2, image.GetHeight() / 2 )

# Set resolution and create the window.
Resolution = (800, 600)

wnd = sf.RenderWindow( sf.VideoMode( Resolution[0], Resolution[1], 32 ), "Hello SFML!" )
wnd.UseVerticalSync( True )

# Load a fancy font.
cheese = sf.Font()
cheese.LoadFromFile( "data/cheeseburger.ttf" )

# Create a text.
text = sf.Text( u"Hello SFML from Python!", cheese, 50 )
text.SetOrigin( text.GetRect().GetSize()[0] / 2, text.GetRect().GetSize()[1] / 2 )
text.SetPosition( 400, 300 )
text.SetColor( sf.Color( 0, 100, 0, 100 ) )

# Create a text for FPS display.
fpstext = sf.Text( u"FPS: --", cheese )
fpstext.SetColor( sf.Color( 0, 0, 0 ) )
currentfps = 0
fpsclock = sf.Clock()

# Load apple image from file.
appleimage = sf.Image()
appleimage.LoadFromFile( "data/apple.png" )

# Create some apples with random position, speed, rotation and color.
apples = [Apple( appleimage ) for num in range( 0, 100 )]
for apple in apples:
	apple.sprite.SetOrigin( appleimage.GetWidth() / 2, appleimage.GetHeight() / 2 )
	apple.sprite.SetPosition(
		random.randint( apple.sprite.GetOrigin()[0], Resolution[0] - apple.sprite.GetOrigin()[0] ),
		random.randint( apple.sprite.GetOrigin()[1], Resolution[1] - apple.sprite.GetOrigin()[1] )
	)
	apple.sprite.SetColor( sf.Color( random.randint( 100, 255 ), random.randint( 100, 255 ), random.randint( 100, 255 ) ) )

	randx = random.randint( -3, 3 )
	randy = random.randint( -3, 3 )
	apple.speed = (1 if randx == 0 else randx, 1 if randy == 0 else randy)

	apple.rotationstep = random.uniform( 1.0, 20.0 ) - 10.0

event = sf.Event()

# Main loop.
while wnd.IsOpened():
	# Fetch all pending events and process them.
	while wnd.GetEvent( event ):
		# Quit when window has been closed or Escape has been pressed.
		if event.Type == sf.Event.Closed:
			wnd.Close()
		elif event.Type == sf.Event.KeyPressed and event.Key.Code == sf.Key.Escape:
			wnd.Close()
	
	# Clear window to white color.
	wnd.Clear( sf.Color( 255, 255, 255 ) )

	# Draw all apples and texts.
	for apple in apples:
		wnd.Draw( apple.sprite )

	wnd.Draw( text )
	wnd.Draw( fpstext )

	wnd.Display() # Display everything.

	# Count FPS.
	currentfps += 1
	if fpsclock.GetElapsedTime() >= 1.0:
		fpsclock.Reset()
		fpstext.SetString( u"FPS: " + unicode( currentfps ) )
		currentfps = 0

	# Update apples (for the "bounce effect").
	for apple in apples:
		apple.sprite.Move( apple.speed[0], apple.speed[1] )
		apple.sprite.Rotate( apple.rotationstep )

		realpos = (apple.sprite.GetPosition()[0] - apple.sprite.GetOrigin()[0], apple.sprite.GetPosition()[1] - apple.sprite.GetOrigin()[1])
		if (apple.speed[0] > 0 and realpos[0] >= Resolution[0] - appleimage.GetWidth()) or (apple.speed[0] < 0 and realpos[0] <= 0):
			apple.speed = (apple.speed[0] * -1, apple.speed[1])

		if (apple.speed[1] > 0 and realpos[1] >= Resolution[1] - appleimage.GetWidth()) or (apple.speed[1] < 0 and realpos[1] <= 0):
			apple.speed = (apple.speed[0], apple.speed[1] * -1)
