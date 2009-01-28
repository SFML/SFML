require "mkmf"

dir_config('SFML')

libs = [
	"GL",
	"GLU",
	"sfml-system",
	"sfml-window",
	"sfml-graphics",
	"sfml-audio",
]

libs.each {|lib|
	unless have_library(lib)
		puts "Unable to find #{lib}.lib!"
		puts "Please specify the path to them using:"
		puts "--with-SFML-lib=lib_path"
		puts "--with-SFML-include=include_path"
		puts
		exit
	end
}

create_makefile("RubySFML")

