require 'RubySFML'
include SFML
require 'gl'
include Gl
require 'glu'
include Glu

class Array
	def magnitude
		return Math::sqrt(self[0]**2 + self[1]**2 + self[2]**2)
	end
	def normalize
		mag = self.magnitude
		return [self[0]/mag, self[1]/mag, self[2]/mag]
	end
	def normalize!
		mag = self.magnitude
		self[0] /= mag
		self[1] /= mag
		self[2] /= mag
	end
	def vec(pos)
		return [pos[0]-self[0], pos[1]-self[1], pos[2]-self[2]]
	end
	def dot(vec)
		return self[0]*vec[0] + self[1]*vec[1] + self[2]*vec[2]
	end
	def cross(vec)
		return [
			self[1]*vec[2] - self[2]*vec[1],
			self[2]*vec[0] - self[0]*vec[2],
			self[0]*vec[1] - self[1]*vec[0]
		]
	end
end

class Player
	attr_accessor :pos, :view, :score
	def initialize(x, y)
		@score = 0
		@pos = [x+0.5, 10, y+0.5]
		@view = [0, 0, -1]
	end

	def turn_left(time)
		angle = -2.0 * time
		@view[0], @view[2] = @view[0]*Math::cos(angle) - @view[2]*Math::sin(angle), @view[0]*Math::sin(angle) + @view[2]*Math::cos(angle)
		@view.normalize!
	end

	def turn_right(time)
		angle = 2.0 * time
		@view[0], @view[2] = @view[0]*Math::cos(angle) - @view[2]*Math::sin(angle), @view[0]*Math::sin(angle) + @view[2]*Math::cos(angle)
		@view.normalize!
	end

	def move_right(time)
		angle = 0.5*Math::PI
		right = [@view[0]*Math::cos(angle) - @view[2]*Math::sin(angle), 0, @view[0]*Math::sin(angle) + @view[2]*Math::cos(angle)]
		d = 5*time
		@pos = [@pos[0]+right[0]*d, @pos[1], @pos[2]+right[2]*d]
	end

	def move_left(time)
		angle = 0.5*Math::PI
		right = [@view[0]*Math::cos(angle) - @view[2]*Math::sin(angle), 0, @view[0]*Math::sin(angle) + @view[2]*Math::cos(angle)]
		d = 5*time
		@pos = [@pos[0]-right[0]*d, @pos[1],  @pos[2]-right[2]*d]
	end

	def move_forward(time)
		d = 5*time
		@pos = [@pos[0]+@view[0]*d, @pos[1], @pos[2]+@view[2]*d]
	end

	def move_backward(time)
		d = 5*time
		@pos = [@pos[0]-@view[0]*d, @pos[1], @pos[2]-@view[2]*d]
	end

	def init_view
		glLoadIdentity()
		gluLookAt(@pos[0], @pos[1], @pos[2], @pos[0]+@view[0], @pos[1]+@view[1], @pos[2]+@view[2], 0, 1, 0)
	end

	def update(win, map)
		pos = @pos
		input, time = win.input, win.frameTime
		turn_left(time) if input.isKeyDown(Key::Left)
		turn_right(time) if input.isKeyDown(Key::Right)
		move_forward(time) if input.isKeyDown(Key::W)
		move_backward(time) if input.isKeyDown(Key::S)
		move_left(time) if input.isKeyDown(Key::A)
		move_right(time) if input.isKeyDown(Key::D)
		@pos = pos if map.solid?(@pos[0], @pos[2])
	end
end

class Block
	attr_accessor :pos
	def initialize(x, y)
		@pos = [x.to_i+0.5, 0, y.to_i+0.5]
	end

	def draw()
		glPushMatrix()
		glTranslatef(@pos[0], @pos[1], @pos[2])
		glColor4f(1, 0, 0, 1)
		glBegin(GL_QUADS)
		# Facing +z
		glNormal3d(0, 0, 1)
		glVertex3f(0.5, 0.5, 0.5)
		glVertex3f(-0.5, 0.5, 0.5)
		glVertex3f(-0.5, -0.5, 0.5)
		glVertex3f(0.5, -0.5, 0.5)
		# Facing -z
		glNormal3d(0, 0, -1)
		glVertex3f(-0.5, 0.5, -0.5)
		glVertex3f(0.5, 0.5, -0.5)
		glVertex3f(0.5, -0.5, -0.5)
		glVertex3f(-0.5, -0.5, -0.5)
		# Facing +x
		glNormal3d(1, 0, 0)
		glVertex3f(0.5, 0.5, 0.5)
		glVertex3f(0.5, -0.5, 0.5)
		glVertex3f(0.5, -0.5, -0.5)
		glVertex3f(0.5, 0.5, -0.5)
		# Facing -x
		glNormal3d(-1, 0, 0)
		glVertex3f(-0.5, 0.5, 0.5)
		glVertex3f(-0.5, 0.5, -0.5)
		glVertex3f(-0.5, -0.5, -0.5)
		glVertex3f(-0.5, -0.5, 0.5)
		glEnd()
		glPopMatrix()
	end
end

class Billboard
	attr_accessor :pos
	def initialize(x, y, size=0.25)
		@size = size
		@pos = [x.to_i+0.5, 0, y.to_i+0.5]
	end

	def draw(camera)
		normal = @pos.vec(camera.pos).normalize
		up = [0, 1, 0]
		right = up.cross(normal).normalize

		glPushMatrix()
		glTranslatef(@pos[0], @pos[1], @pos[2])
		glColor4f(1, 1, 1, 1)
		glBegin(GL_QUADS)
		# Facing camera
		glNormal3d(normal[0], normal[1], normal[2])
		glTexCoord2f(1, 0)
		glVertex3f((right[0]+up[0])*@size, (right[1]+up[1])*@size, (right[2]+up[2])*@size)
		glTexCoord2f(0, 0)
		glVertex3f((-right[0]+up[0])*@size, (-right[1]+up[1])*@size, (-right[2]+up[2])*@size)
		glTexCoord2f(0, 1)
		glVertex3f(-(right[0]+up[0])*@size, -(right[1]+up[1])*@size, -(right[2]+up[2])*@size)
		glTexCoord2f(1, 1)
		glVertex3f((right[0]-up[0])*@size, (right[1]-up[1])*@size, (right[2]-up[2])*@size)
		glEnd()
		glPopMatrix()
	end
end

# Map class holds and draws tiles and gems.
class Map
	attr_reader :width, :height, :blocks, :gems

	def initialize(filename, player)
		@start = Time.now
		@sb = SoundBuffer.new("media/Beep.wav")
		@sound = Sound.new(@sb)
		@gem_image = Image.new("media/Gem.png")
		@exit_image = Image.new("media/Star.png")
		@exit_image.resize(25, 25, Color.Black)
		@exit = nil
		@blocks = []
		@gems = []

		lines = File.readlines(filename).map { |line| line.chop }
		@height = lines.size
		@width = lines[0].size
		@tiles = Array.new(@width) do |x|
			Array.new(@height) do |y|
				case lines[y][x, 1]
				when '"', '#' then @blocks << Block.new(x, y); @blocks[-1]
				when 'S' then player.pos = [x+0.5, 0, y+0.5]; nil
				when 'E' then @exit = Billboard.new(x, y); nil
				when 'x' then @gems << Billboard.new(x, y); nil
				else nil
				end
			end
		end
	end

	def draw(player)
		@blocks.each {|b| b.draw()}
		glEnable(GL_TEXTURE_2D)
		@gem_image.bind
		@gems.each {|g| g.draw(player)}
		@exit_image.bind
		@exit.draw(player)
		glDisable(GL_TEXTURE_2D)
	end

	def exit?(player)
		player.pos[0].to_i == @exit.pos[0].to_i and player.pos[2].to_i == @exit.pos[2].to_i
	end

	# Solid at a given pixel position?
	def solid?(x, y)
		y < 0 or x < 0 or x >= @width or y >= @width or @tiles[x.to_i][y.to_i]
	end

	def collect_gems(player)
		@gems.reject! {|g|
			if player.pos.vec(g.pos).magnitude <= 0.5
				@sound.play
				player.score += 10
				true
			else
				false
			end
		}
	end
end

$clock = Clock.new
mode = VideoMode.new(640, 480, 32)
win = RenderWindow.new(mode, "RubySFML Test", 0)
win.showMouseCursor(false)
win.useVerticalSync(true)
sky_image = Image.new("media/Space.png")
player = Player.new(0, 5)
levels = Dir["media/Level*.txt"].sort
level = 1
level_start = Time.now
map = Map.new(levels.shift, player)

# Simple game loop
done = false
while !done
	if map.exit?(player)
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
	time = win.frameTime
	player.update(win, map)

	#win.beginOpenGL()
	glEnable(GL_DEPTH_TEST)
	glEnable(GL_CULL_FACE)
	glEnable(GL_BLEND)
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA)
	glMatrixMode(GL_PROJECTION)
	glPushMatrix()
	glLoadIdentity()
	gluPerspective(45.0, 640.0/480.0, 0.1, 100.0)
	glMatrixMode(GL_MODELVIEW)
	glPushMatrix()
	player.init_view()
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT)
	glEnable(GL_COLOR_MATERIAL)
	glEnable(GL_LIGHTING)
	glEnable(GL_LIGHT0)
	glEnable(GL_NORMALIZE)
	glDisable(GL_TEXTURE_2D)
	glLightfv(GL_LIGHT0, GL_POSITION, player.pos + [1])
	glLightfv(GL_LIGHT0, GL_DIFFUSE, [1, 1, 1, 1])
	glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, 0.25)
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, [0, 0, 0, 1])
	map.collect_gems(player)
	map.draw(player)
	glPopMatrix()
	glMatrixMode(GL_PROJECTION)
	glPopMatrix()
	#win.endOpenGL()

	text = Text.new("Score: #{player.score}", "", 20)
	win.draw(text)

	if game_over
		text = Text.new("Game Over!", "", 60)
		text.left, text.top = 200, 200
		win.draw(text)
	elsif Time.now - level_start < 3
		text = Text.new("Level #{level}!", "", 60)
		text.left, text.top = 200, 200
		win.draw(text)
		text = Text.new("(Collect the gems and find the exit)", "", 25)
		text.left, text.top = 125, 260
		win.draw(text)
	end

	win.display()
	sleep(0.01)
end

