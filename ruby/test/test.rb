require "RubySFML"
include SFML

require "gl"
include Gl

class TextTest < Text
	# Test overriding a virtual C++ method
	def render(win)
		self.color = Color.new(color.r+1, color.g+2, color.b+3, 255)
		super # Call base class's version of render (in C++)

		# Custom OpenGL calls
		rect = getRect()
		#win.beginOpenGL()
		glColor4f(1, 1, 1, 1)
		glLineWidth(5)
		glBegin(GL_LINES)
		glVertex2f(0, 0)
		glVertex2f(rect.width, rect.height)
		glVertex2f(rect.width, 0)
		glVertex2f(0, rect.height)
		glEnd()
		#win.endOpenGL()
	end
end

# Test Clock class
clock = Clock.new
10.times { sleep 0.01; puts "Clock - #{clock}" }
puts

# Test VideoMode class
puts "Current desktop mode - #{VideoMode::desktop}"
puts "32-bit video modes:"
VideoMode::each {|mode| puts mode.to_s if mode.bpp == 32 }
puts

# Test RenderWindow class
mode = VideoMode.new(800, 600, 32)
win = RenderWindow.new(mode, "RubySFML Test")
win.useVerticalSync(false)
win.showMouseCursor(false)
win.setBackgroundColor(Color.Red)
puts "Window - #{win}"
puts "View Rect - #{win.view}"
puts

# Test Color, Image, and Sprite classes
puts format("Red = 0x%X", Color.Red.toRGBA)
puts format("Green = 0x%X", Color.Green.toRGBA)
puts format("Blue = 0x%X", Color.Blue.toRGBA)
puts format("Black = 0x%X", Color.Black.toRGBA)
image = Image.new("media/cursor.png")
sprite = Sprite.new(image)
image.width.times {|x| puts "image[#{x},#{x}] = #{image[x,x]}" }
image.createMaskFromColor(Color.Black) # Set black to transparent
sprite.x = mode.w/2 - sprite.w/2
sprite.y = mode.h/2 - sprite.h/2
puts "Image - #{image}"
puts "Sprite - #{sprite}"
puts

# Test Text (sfString) class
text = TextTest.new("This is a test!")

# Test Sound and SoundBuffer classes
sb = SoundBuffer.new("media/cannon.wav")
sound = Sound.new(sb)
sound.volume = 100
puts "SoundBuffer - #{sound.buffer}"
puts "Sound - #{sound}"

# Simple game loop
done = false
while !done
	while e = win.getEvent()
		puts "Event - #{e.to_s}" # Dump events to stdout

		done = true if
			e.type == Event::Closed or
			(e.type == Event::KeyReleased and e.code == Key::Escape) 

		sound.play if
			(e.type == Event::KeyPressed and e.code == Key::Space) or
			(e.type == Event::MouseButtonPressed and e.buttons == Mouse::Left)

		sprite.x, sprite.y = e.x-sprite.w/2, e.y-sprite.h/2 if
			e.type == Event::MouseMoved
	end

	text.left = 0 if (text.left += 1) >= 800
	text.top = 0 if (text.top += 1) >= 600
	win.draw(text)
	win.draw(sprite)

	win.display()
end

