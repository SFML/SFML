#!/usr/bin/python

from PySFML import sf
import math
import random

class Menu:
	def __init__(self, screen_width, screen_height):
		self.selection = 0
		text_color = sf.Color(220, 220, 20, 255)
		self.spacing = screen_height/7

		self.title = sf.Text("PyWorm!")
		self.title.SetColor(text_color)
		self.title.SetPosition(screen_width/2-80., self.spacing)

		levels = ["Very Easy", "Easy", "Medium", "Hard"]
		x_align = [-80., -50., -70., -50.]
		self.strings = []
		for i in range(0, 4):
			string = sf.Text(levels[i])
			string.SetColor(text_color)
			string.SetPosition(screen_width/2+x_align[i], (2+i)*self.spacing+20)
			self.strings.append(string)

		self.rectangle = sf.Shape.Rectangle(0, 0, screen_width, 40, sf.Color(50, 50, 10))

	def next_frame(self, win):
		self.rectangle.SetY(self.spacing*(2 + self.selection)+20)
		win.Draw(self.rectangle)
		win.Draw(self.title)
		win.Draw(self.strings)

	def key_up(self, pressed):
		if pressed:
			self.selection = (self.selection - 1) % 4
	def key_down(self, pressed):
		if pressed:
			self.selection = (self.selection + 1) % 4



class Apple(sf.Sprite):
	def __init__(self):
		apple_img = sf.Image() # Apple's image
		if not apple_img.LoadFromFile("./data/apple.png"):
			pass
			# print "Could not load data/apple.png"
		sf.Sprite.__init__(self, apple_img)
		self.SetOrigin(apple_img.GetWidth()/2, apple_img.GetHeight()/2)
		self.size = apple_img.GetWidth()

	def random_move(self, arena):
		self.SetPosition( \
			random.randrange(arena.arena_left+arena.border, arena.arena_right-arena.border), \
			random.randrange(arena.arena_top+arena.border, arena.arena_bottom-arena.border) \
		)


class Arena(dict):
	shrink_value, border, arena_top = 20, 30, 20
	def __init__(self, window_width, window_height):
		self.window_width = window_width
		self.arena_bottom, self.exit_left, self.exit_right = window_height-80, window_width/2 - 50, window_width/2 + 50
		self['level_str'] = sf.Text()
		self['level_str'].SetColor(sf.Color.White)
		self['level_str'].SetPosition(60., window_height-60)
		self['score_str'] = sf.Text()
		self['score_str'].SetColor(sf.Color.White)
		self['score_str'].SetPosition(260., window_height-60)
		self.exit_rect = sf.Shape.Rectangle(self.exit_left, 0, self.exit_right, self.arena_top, sf.Color.Black)
		self.reset()
		self.update_arena_rect()

	def update_arena_rect(self):
		self['arena_rect'] = sf.Shape.Rectangle(self.arena_left, self.arena_top, self.arena_right, self.arena_bottom, sf.Color.Black)

	def reset(self):
		self.level, self.score, self.arena_left, self.arena_right = 1, 0, self.shrink_value, self.window_width-self.shrink_value
		self.update_arena_rect()
		self['level_str'].SetString("Level: 1")
		self['score_str'].SetString("Score: 0")

	def update_score(self):
		self.score += 1
		self['score_str'].SetString("Score: " + str(self.score))

	def next_level(self):
		self.level += 1
		self['level_str'].SetString("Level: " + str(self.level))
		self.arena_left += self.shrink_value
		self.arena_right -= self.shrink_value
		self.update_arena_rect()
		self.score += 4
		self.update_score()

class Part(sf.Sprite):
	def __init__(self, rond, x, y):
		sf.Sprite.__init__(self, rond)
		self.SetOrigin(rond.GetWidth()/2, rond.GetHeight()/2)
		self.SetPosition(x, y)

class Worm(list):
	parts_spacing, turn_step, part_size, start_x, start_y, required_length, grow_length = 3, 0.15, 6.0, 50., 50., 300, 20

	def __init__(self, difficulty):
		self.parts_per_frame = 1 + difficulty
		self.angle = 0
		self.direction = 0 # 0, 1 or -1 according to the key pressed
		self.rond = sf.Image()
		self.level_completed = False
		if not self.rond.LoadFromFile("./data/rond2.png"):
			pass
			# print "Could not load data/rond2.png"

	def reset(self, arena):
		self.targeted_length, self.angle, self.direction = 30, 0, 0
		self[:] = [Part(self.rond, arena.arena_left+self.start_x, arena.arena_top+self.start_y)]

	def left(self, pressed):
		if pressed:
			self.direction = -1
		elif self.direction == -1:
			self.direction = 0
	def right(self, pressed):
		if pressed:
			self.direction = 1
		elif self.direction == 1:
			self.direction = 0
	def restart(self, arena):
		if self.targeted_length == 0 and not self.level_completed:
			arena.reset()
			self.reset(arena)
			return True
		else:
			return False

	def crash(self):
		self.targeted_length = 0

	def move(self, arena, apple):
		head_x, head_y = -1, -1
		if self.is_running(): # Create new parts and check collisions if the worm hasn't crashed yet
			for i in range(self.parts_per_frame): # We create PartsPerFrame Worm's parts
				self.angle += self.direction*self.turn_step
				head_x, head_y = self[-1].GetPosition()
				head_x += self.parts_spacing*math.cos(self.angle)
				head_y += self.parts_spacing*math.sin(self.angle)
				if self.is_running() and self.targeted_length <= self.required_length: # let's check if the worm ate the apple
					if math.hypot(apple.GetPosition()[0] - head_x, apple.GetPosition()[1] - head_y) < apple.size/2 + self.part_size/2: # Yes it did
						arena.update_score()
						self.targeted_length += self.grow_length # The worm gets longer
						apple.random_move(arena)
				if head_x<arena.arena_left+self.part_size/2 or head_x>arena.arena_right-self.part_size/2 or head_y<arena.arena_top+self.part_size/2 or head_y>arena.arena_bottom-self.part_size/2: # Crash into a wall
					if len(self) > self.required_length:
						if head_y<arena.arena_top+self.part_size/2:
							if head_x<arena.exit_left+self.part_size/2 or head_x>arena.exit_right-self.part_size/2: # Crash into the exit walls
								self.crash()
							elif head_y < 0:
								self.level_completed = True
								self.targeted_length = 0
						else:
							self.crash()
					elif self.is_running():
						self.crash()
				if self.is_running():
					self.append(Part(self.rond, head_x, head_y))
		if len(self) > self.targeted_length:
			if len(self) - self.targeted_length >= self.parts_per_frame:
				del self[0:self.parts_per_frame]
			else:
				del self[0:len(self) - self.targeted_length]

		if (head_x, head_y) == (-1, -1) and len(self) > 0:
			head_x, head_y = self[-1].GetPosition()

		if len(self) > self.part_size/self.parts_spacing + 1:
			for i in range(len(self)):
				if i < len(self) - self.part_size/self.parts_spacing - 1:
					test_x, test_y = self[i].GetPosition()
					if math.hypot(head_x-test_x, head_y-test_y) < self.part_size and self.is_running(): # Check for collision
						self.crash()

		if len(self) == 0:
			if self.level_completed:
				self.level_completed = False
				arena.next_level()
				self.reset(arena)

	def is_running(self):
		return (self.targeted_length > 0) and not self.level_completed
	def draw_exit(self):
		return self.targeted_length > self.required_length or self.level_completed

class Game:
	def __init__(self, difficulty, window_width, window_height):
		self.arena = Arena(window_width, window_height)
		self.worm = Worm(difficulty)
		self.worm.reset(self.arena)
		self.apple = Apple()
		self.apple.random_move(self.arena)
		self.pause = False

	def enter(self, pressed):
		if pressed:
			if not self.worm.restart(self.arena):
				self.pause = not self.pause

	def next_frame(self, win):
		win.Draw(self.arena.values())
		if not self.pause:
			self.worm.move(self.arena, self.apple)
		if self.worm.draw_exit():
			win.Draw(self.arena.exit_rect)
		elif self.worm.is_running():
			win.Draw(self.apple)
		win.Draw(self.worm)


class Main:
	# Entry Point
	def __init__(self):
		# Initialize the window
		self.win = sf.RenderWindow(sf.VideoMode(800, 600,32), "PyWorm", sf.Style.Close) # Creates the window
		self.win.EnableKeyRepeat(False)
		background_color = sf.Color(100, 100, 0, 255)
		self.win.SetFramerateLimit(30)
		event = sf.Event()
		self.keys = {} # keys to watch
		self.menu_begin()

		# Boucle principale
		while self.win.IsOpened():
			while self.win.GetEvent(event): # Event Handler
				if event.Type == sf.Event.Closed:
					self.win.Close()
				elif event.Type == sf.Event.KeyPressed or event.Type == sf.Event.KeyReleased:
					if event.Key.Code in self.keys:
						self.keys[event.Key.Code](event.Type == sf.Event.KeyPressed)
			self.win.Display()
			self.win.Clear(background_color)
			self.next_frame(self.win)

	# Menu
	def menu_begin(self):
		self.menu = Menu(self.win.GetWidth(), self.win.GetHeight())
		self.keys = {sf.Key.Escape:self.close_window, sf.Key.Up:self.menu.key_up, sf.Key.Down:self.menu.key_down, sf.Key.Return:self.menu_end}
		self.next_frame = self.menu.next_frame

	def close_window(self, pressed):
		if pressed:
			self.win.Close()

	def menu_end(self, pressed):
		if pressed:
			selection = self.menu.selection
			del self.menu
			self.game_begin(selection)

	# Game
	def game_begin(self, selection):
		self.game = Game(selection, self.win.GetWidth(), self.win.GetHeight())
		self.keys =  {sf.Key.Left:self.game.worm.left, sf.Key.Right:self.game.worm.right, sf.Key.Return:self.game.enter, sf.Key.Escape:self.game_end}
		self.next_frame = self.game.next_frame

	def game_end(self, pressed):
		if pressed:
			del self.game
			self.menu_begin()

Main()

