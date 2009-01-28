require 'RubySFML'
include SFML

# A simple class to handle a single puzzle piece
class Piece
	# Each shape is made up of 4 blocks:
	#   *    *     *     *   **   **   **
	#   *    **    **   **   *     *   **
	#   *    *      *   *    *     *
	#   *
	SHAPES = [ # Negative y is down
		[[0, 0], [0, 1], [0, -1], [0, -2]],
		[[0, 0], [0, 1], [1, 0], [0, -1]],
		[[0, 0], [0, 1], [1, 0], [1, -1]],
		[[1, 0], [1, 1], [0, 0], [0, -1]],
		[[0, 0], [0, 1], [1, 1], [0, -1]],
		[[1, 0], [1, 1], [0, 1], [1, -1]],
		[[0, 0], [0, 1], [1, 1], [1, 0]],
	]
	# Each shape has its own color
	COLORS = [Color.Red, Color.Yellow, Color.Cyan, Color.Magenta, Color.Green, Color.Blue, Color.new(255, 128, 64)]

	attr_accessor :x, :y, :r
	def initialize(n, x, y)
		@n, @x, @y = n, x, y
		@r = 0
	end

	def shape(); SHAPES[@n]; end
	def color(); COLORS[@n]; end
	def rleft(); @r = (@r+1) % 4; end
	def rright(); @r = (@r-1) % 4; end
	def each() # Iterate each block coordinate (corrected for rotation)
		case @r
			when 0 then shape.each {|c| yield([c[0], c[1]])}
			when 1 then shape.each {|c| yield([-c[1], c[0]])}
			when 2 then shape.each {|c| yield([-c[0], -c[1]])}
			when 3 then shape.each {|c| yield([c[1], -c[0]])}
		end
	end
end

class Grid
	HEIGHT = 20
	WIDTH = 10
	BLOCK = 18
	BORDER = 1
	BSIZE = BLOCK + BORDER

	def initialize()
		@rows = []
		HEIGHT.times { @rows.push Array.new(WIDTH) }
		@background_image = Image.new(WIDTH*(BSIZE)+BORDER, HEIGHT*BSIZE+BORDER, Color.White)
		@background = Sprite.new(@background_image, (640-WIDTH*BSIZE)/2-BORDER, (480-HEIGHT*BSIZE)/2-BORDER)
		@block_image = Image.new(BLOCK, BLOCK, Color.White)
		@block = Sprite.new(@block_image)

		@score = 0
		@last_down = $clock.to_f
		@piece = Piece.new(rand(7), WIDTH/2-1, HEIGHT-1)
		@next_piece = Piece.new(rand(7), WIDTH/2-1, HEIGHT-1)
	end

	def [](y)
		return @rows[y]
	end

	def draw_block(win, x, y, color)
		@block.left = x*BSIZE + @background.left+BORDER
		@block.top = ((HEIGHT-1)-y)*BSIZE + @background.top+BORDER
		@block.color = color
		win.draw(@block)
	end

	def solid?(x, y)
		return true if x < 0 or y < 0 or x >= WIDTH or y >= HEIGHT
		return @rows[y][x]
	end

	def left
		x = @piece.x
		@piece.x -= 1
		@piece.each {|c| @piece.x = x if solid?(@piece.x+c[0], @piece.y+c[1])}
	end

	def right
		x = @piece.x
		@piece.x += 1
		@piece.each {|c| @piece.x = x if solid?(@piece.x+c[0], @piece.y+c[1])}
	end

	def up
		r = @piece.r
		@piece.rright
		@piece.each {|c| @piece.r = r if solid?(@piece.x+c[0], @piece.y+c[1])}
	end

	def down
		@last_down = $clock.to_f
		y = @piece.y
		@piece.y -= 1
		done = false
		@piece.each {|c| done = true if solid?(@piece.x+c[0], @piece.y+c[1])}
		if done
			@piece.y = y
			@piece.each {|c| @rows[@piece.y+c[1]][@piece.x+c[0]] = @piece.color}
			@piece = @next_piece
			@next_piece = Piece.new(rand(7), WIDTH/2-1, HEIGHT-1)
		end
	end

	def update(win)
		speed = 0.5 - 0.1 * (@score/1000)
		down if $clock.to_f - @last_down >= speed

		collapsed = 0
		while y = find_full_row()
			collapse_row(y)
			collapsed += 1
		end
		@score += 5 * 2**collapsed if collapsed > 0
	end

	def render(win)
		win.draw(@background)

		HEIGHT.times {|y|
			WIDTH.times {|x|
				c = @rows[y][x] || Color.Black
				draw_block(win, x, y, c)
			}
		}

		@piece.each {|c|
			x, y, c = @piece.x+c[0], @piece.y+c[1], @piece.color
			next if y < 0 or y > HEIGHT-1
			draw_block(win, x, y, c)
		}

		score = Text.new("Score: #{@score}", "", 20)
		#score.left, score.top = 10, 10
		win.draw(score)

		text = Text.new("Coming Next:", "", 20)
		text.left, text.top = 450, 50
		win.draw(text)
		@next_piece.each {|c|
			x, y, c = 14+c[0], 16+c[1], @next_piece.color
			next if y < 0 or y > HEIGHT-1
			draw_block(win, x, y, c)
		}
	end

	def collapse_row(i)
		while i < HEIGHT-1
			@rows[i] = @rows[i+1]
			i += 1
		end
		@rows[HEIGHT-1] = Array.new(WIDTH, nil)
	end

	def find_full_row()
		HEIGHT.times {|y| return y unless @rows[y].index(nil) }
		return nil
	end
end

$clock = Clock.new
mode = VideoMode.new(640, 480, 32)
win = RenderWindow.new(mode, "RubySFML Test", 0)
win.showMouseCursor(false)
win.useVerticalSync(true)
grid = Grid.new()

# Simple game loop
done = false
while !done
	while e = win.getEvent()
		case e.type
			when Event::Closed then done = true
			when Event::KeyReleased then done = true if e.code == Key::Escape
			when Event::KeyPressed
				case e.code
					when Key::Up then grid.up
					when Key::Left then grid.left
					when Key::Right then grid.right
					when Key::Down then grid.down
				end
		end
	end
	grid.update(win)
	grid.render(win)
	win.display()
	sleep(0.01)
end

