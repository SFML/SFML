require 'RubySFML'
include SFML

module Tiles
	Grass = 0
	Earth = 1
end

# Collectible item.
class Gem2 < Sprite
	def initialize(image, x, y)
		super(image)
		self.x = x
		self.y = y
	end

	def update
		self.rotation = 25 * Math.sin($clock.to_f*6)
	end
end

class Player < VSprite
	attr_reader :score

	def initialize(image, w, h)
		super(image, w, h)
		@sb = SoundBuffer.new("media/Beep.wav")
		@sound = Sound.new(@sb)
		@score = 0
	end

	def update(window, map)
		time = window.frameTime
		input = window.input

		# If there's no ground beneath my feet, increase speed downward
		if !map.solid?(self.x, self.bottom)
			@vy += 1
		# If there is ground beneath my feet, and the user presses up,
		# boost speed upwards to jump
		elsif input.isKeyDown(Key::Up)
			@vy = -20
		end

		super(1) # Update position based on velocity

		# If we just moved down, see it we landed
		if @vy > 0
			if map.solid?(self.x, self.bottom)
				@vy = 0
				self.top -= self.top % 50
			end
		# If we just moved up, see if we hit our head
		elsif @vy < 0
			if map.solid?(self.x, self.top)
				@vy = 0
				self.top += 50 - self.top % 50
			end
		end

		moved = false
		if input.isKeyDown(Key::Left)
			moved = true
			self.x -= 10
			# If we just moved left, see if we hit a wall
			if map.solid?(self.left, self.y)
				self.left += 50 - self.left % 50
			end
		end

		if input.isKeyDown(Key::Right)
			moved = true
			self.x += 10
			# If we just moved right, see if we hit a wall
			if map.solid?(self.right, self.y)
				self.left -= self.left % 50
			end
		end

		if @vy < 0
			@tx = 3
		elsif moved
			@tx = (@tx + 1) % 3
		else
			@tx = 0
		end

		# Same as in the tutorial game.
		map.gems.reject! do |c|
			if (c.x - self.x).abs < 50 and (c.y - self.y).abs < 50
				@sound.play
				@score += 10
				true
			else
				false
			end
		end
	end
end

# Map class holds and draws tiles and gems.
class Map < Drawable
	attr_reader :width, :height, :gems

	def initialize(filename, player)
		@start = Time.now
		@tileset_image = Image.new("media/Tileset.png")
		@tileset = TSprite.new(@tileset_image, 50, 50)
		@gem_image = Image.new("media/Gem.png")
		@gems = []
		@exit_image = Image.new("media/Star.png")
		@exit = TSprite.new(@exit_image, 25, 25)

		lines = File.readlines(filename).map { |line| line.chop }
		@height = lines.size
		@width = lines[0].size
		@tiles = Array.new(@width) do |x|
			Array.new(@height) do |y|
				case lines[y][x, 1]
				when '"' then Tiles::Grass
				when '#' then Tiles::Earth
				when 'S' then player.x, player.y = x*50, y*50; nil
				when 'E' then @exit.left, @exit.top = x*50+25, y*50+25; nil
				when 'x' then @gems << Gem2.new(@gem_image, x * 50 + 25, y * 50 + 25); nil
				else nil
				end
			end
		end
	end

	def done?(player)
		player.x.to_i/50 == @exit.x.to_i/50 and player.y.to_i/50 == @exit.y.to_i/50
	end

	def render(window)
		# Very primitive drawing function:
		# Draws all the tiles, some off-screen, some on-screen.
		@height.times do |y|
			@width.times do |x|
				tile = @tiles[x][y]
				if tile
					@tileset.tx = tile
					@tileset.left = x * 50
					@tileset.top = y * 50
					window.draw(@tileset) if
						@tileset.right > window.view.left and
						@tileset.left < window.view.right and
						@tileset.bottom > window.view.top and
						@tileset.top < window.view.bottom
				end
			end
		end
		@gems.each { |c|
			c.update
			window.draw(c) if
				c.right > window.view.left and
				c.left < window.view.right and
				c.bottom > window.view.top and
				c.top < window.view.bottom
		}
		window.draw(@exit)
	end

	# Solid at a given pixel position?
	def solid?(x, y)
		x < 0 or y < 0 or x >= @width*50 or y >= @height*50 or @tiles[x / 50][y / 50]
	end
end

$clock = Clock.new
mode = VideoMode.new(640, 480, 32)
win = RenderWindow.new(mode, "RubySFML Test", 0)
view = View.new(FloatRect.new(0, 0, 640, 480))
win.showMouseCursor(false)
win.useVerticalSync(true)
sky_image = Image.new("media/Space.png")
sky = Sprite.new(sky_image)
player_image = Image.new("media/CptnRuby.png")
player = Player.new(player_image, 50, 50)
levels = Dir["media/Level*.txt"].sort
level = 1
level_start = Time.now
map = Map.new(levels.shift, player)

# Simple game loop
done = false
game_over = false
while !done
	if map.done?(player)
		if levels.empty?
			game_over = true
		else
			level += 1
			level_start = Time.now
			map = Map.new(levels.shift, player)
		end
	end

	while e = win.getEvent()
		done = true if e.type == Event::Closed or
			(e.type == Event::KeyReleased and e.code == Key::Escape) 
	end

	player.update(win, map)

	left = player.x - 640/2
	left = 0 if left < 0
	left = map.width*50-640 if left > map.width*50-640
	top = player.y - 480/2
	top = 0 if top < 0
	top = map.height*50-480 if top > map.height*50-480
	view.rect = FloatRect.new(left, top, left+640, top+480)
	win.setView(view)

	sky.left = view.rect.left
	sky.top = view.rect.top
	win.draw(sky)
	win.draw(map)
	win.draw(player)
	score = Text.new("Score: #{player.score}", "", 20)
	score.left, score.top = sky.left, sky.top
	win.draw(score)

	if game_over
		text = Text.new("Game Over!", "", 60)
		text.left, text.top = sky.left+200, sky.top+200
		win.draw(text)
	elsif Time.now - level_start < 3
		text = Text.new("Level #{level}!", "", 60)
		text.left, text.top = sky.left+200, sky.top+200
		win.draw(text)
		text = Text.new("(Collect the gems and find the exit)", "", 25)
		text.left, text.top = sky.left+125, sky.top+260
		win.draw(text)
	end

	win.display()
	sleep(0.01)
end

