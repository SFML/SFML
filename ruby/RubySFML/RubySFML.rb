require "RubySFML.so"

module SFML

	# A simple class for dealing with tiled sprites.
	class TSprite < Sprite
		# Use to set/get the tile column
		attr_accessor :tx
		# Use to set/get the tile row
		attr_accessor :ty

		# Pass in the image, the tile width, and the tile height.
		def initialize(image, w, h)
			super(image)
			@tx, @ty, @tw, @th = 0, 0, w, h
			setRotationCenter(@tw/2, @th/2)
		end

		# Overrides Sprite::render to call setSubRect() for you to choose a
		# tile based on tx and ty before the sprite is rendered.
		def render(win)
			l, t = @tw*@tx, @th*@ty
			setSubRect(IntRect.new(l, t, l+@tw, t+@th))
			super(win)
		end
	end

	# A simple class for dealing with sprites and 2D velocity/acceleration
	class VSprite < TSprite
		# Use to get/set the x component of the velocity vector
		attr_accessor :vx
		# Use to get/set the y component of the velocity vector
		attr_accessor :vy

		# Pass in the image, the tile width, and the tile height.
		def initialize(image, w, h)
			super(image, w, h)
			@vx = @vy = 0.0
		end

		# Specify a number of degrees to rotate left
		def turn_left(degrees) rotate(degrees); end
		# Specify a number of degrees to rotate right
		def turn_right(degrees) rotate(-degrees); end

		# Returns the magnitude of the velocity vector
		def speed
			return Math.sqrt(@vx**2 + @vy**2)
		end

		# Returns the direction of the velocity vector
		def direction
			s = speed
			return [@vx/s, @vy/s]
		end

		# Accelerates the sprite in the direction vector v by the amount "thrust"
		def accelerate(v, thrust)
			@vx += v[0] * thrust
			@vy += v[1] * thrust
		end

		# Scales the speed component of the velocity
		def scale_speed(amount)
			@vx *= amount
			@vy *= amount
		end

		# Moves the sprite in the direction vector v by the amound d
		def move(v, d)
			self.x += v[0] * d
			self.y += v[1] * d
		end

		# Updates the sprite's position based on its velocity and its time slice
		def update(time)
			self.x += @vx * time
			self.y += @vy * time
		end
	end

end

