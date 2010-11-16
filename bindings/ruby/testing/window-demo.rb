require 'sfml/system'
require 'sfml/window'

app = SFML::Window.new( [800, 600], "My Ruby SFML" )
app.framerate = 100
app.position = [300, 300]
input = app.input

while app.open?
  while event = app.get_event
    if event.type == SFML::Event::Closed
      app.close
    end
  end
  
  p [input.mouse_x, input.mouse_y]
  
  app.display
end
