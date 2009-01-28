require "mkmf"
$CFLAGS = "/MD /O2 /EHsc /DWIN32 /DNDEBUG /D_WINDOWS"
#$CFLAGS = "/MD /Zi /EHsc /DWIN32 /DNDEBUG /D_WINDOWS"
#$LDFLAGS = "/Zi"

libs = [
	"opengl32",
	"glu32",
]

libs.each {|lib|
	unless have_library(lib)
		puts "Unable to find #{lib}.lib!"
		puts
		exit
	end
}

create_makefile("glu")

