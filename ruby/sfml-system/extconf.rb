require 'mkmf'

dir_config("system")
have_library("sfml-system") 
create_makefile("sfml/system", "system")
