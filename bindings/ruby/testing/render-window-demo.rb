require 'sfml/system'
require 'sfml/window'
require 'sfml/graphics'

app = SFML::RenderWindow.new
app.create( [800, 600], "My Ruby SFML" )
app.framerate = 100
app.position = [300, 300]
input = app.input

shape = SFML::Shape.rectangle( [-10, -10, 20, 20], SFML::Color::White )

image = SFML::Image.new
image.create( 100, 100, [255, 0, 0] )
sprite = SFML::Sprite.new( image, [500, 500] )

text = SFML::Text.new( "This is a test!" )
text.position = [ 20, 20 ]

while app.open?
  while event = app.get_event
    if event.type == SFML::Event::Closed
      app.close
    end
  end
  
  app.clear
  shape.position = [input.mouseX, input.mouseY]
  app.draw shape
  app.draw sprite
  app.draw text
  app.display
end 
