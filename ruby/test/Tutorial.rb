require 'RubySFML'
include SFML

TURN = 180
THRUST = 200

class Player < VSprite
	attr_reader :score

	def initialize(image)
		super(image, image.w, image.h)
		@sb = SoundBuffer.new("media/Beep.wav")
		@beep = Sound.new(@sb)
		@score = 0
	end

	def warp(x, y)
		self.x = x
		self.y = y
	end

	def turn_left(time) super(TURN*time); end
  	def turn_right(time) super(TURN*time); end
	def accelerate(time) super(upVector, THRUST*time); end
	def update(time)
		super(time)
		self.x %= 640
		self.y %= 480
		scale_speed(1.0 - 0.5*time)
	end

	def collect(stars)
		stars.reject! { |star|
			if distance(star) < 35 then
				@score += 10
				@beep.play
				true
			else
				false
			end
		}
	end
end

class Star < TSprite
	def initialize(image)
		super(image, image.h, image.h)
		self.color = Color.new(rand(255 - 40) + 40, rand(255 - 40) + 40, rand(255 - 40) + 40, 255)
		self.x = rand * 640
		self.y = rand * 480
		self.subRect = IntRect.new(0, 0, image.h, image.h)
		@last_change = $clock.to_f
	end

	def update(time)
		if $clock.to_f - @last_change >= 0.1 # Change every 10th of a second
			@tx = (@tx+1) % 10
			@last_change = $clock.to_f
		end
	end
end

$clock = Clock.new
mode = VideoMode.new(640, 480, 32)
win = RenderWindow.new(mode, "RubySFML Test", Window::Fullscreen)
win.showMouseCursor(false)
win.useVerticalSync(true)
background_image = Image.new("media/Space.png")
player_image = Image.new("media/Starfighter.png")
star_image = Image.new("media/Star.png")

background = Sprite.new(background_image)
player_image.createMaskFromColor(Color.new(255, 0, 255, 255))
player = Player.new(player_image)
player.warp(320, 240)
stars = []
10.times { stars << Star.new(star_image) }

# Simple game loop
done = false
while !done
	while e = win.getEvent()
		done = true if e.type == Event::Closed or
			(e.type == Event::KeyReleased and e.code == Key::Escape) 
	end
	time = win.frameTime
	input = win.input
	player.turn_left(time) if input.isKeyDown(Key::Left)
	player.turn_right(time) if input.isKeyDown(Key::Right)
	player.accelerate(time) if input.isKeyDown(Key::Up)
	player.update(time)
	player.collect(stars)
    stars << Star.new(star_image) if rand(100) < 1 and stars.size < 10
	stars.each {|star| star.update(time) }

	win.draw(background)
	stars.each {|star| win.draw(star) }
	win.draw(player)
	win.draw(Text.new("Score: #{player.score}", "", 20))
	win.display()
end

