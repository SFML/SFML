require 'mkmf'

dir_config("window")
have_library("sfml-window") 
create_makefile("sfml/window", "window")
