require 'sfml/system'
require 'sfml/window'
require 'sfml/graphics'

class MyDrawable
  include SFML::Drawable
end

drawable = MyDrawable.new
p drawable.position
p drawable.is_a?( SFML::Drawable )
