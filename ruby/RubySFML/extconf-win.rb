require "mkmf"
$CFLAGS = "/MD /O2 /EHsc /DWIN32 /DNDEBUG /D_WINDOWS"
#$CFLAGS = "/MD /Zi /EHsc /DWIN32 /DNDEBUG /D_WINDOWS"
#$LDFLAGS = "/Zi"

dir_config('SFML')

libs = [
	"gdi32",
	"opengl32",
	"glu32",
	"sfml-system-s",
	"sfml-window-s",
	"sfml-graphics-s",
	"sfml-audio-s",
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

