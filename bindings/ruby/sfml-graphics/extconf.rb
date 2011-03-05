# rbSFML - Copyright (c) 2010 Henrik Valter Vogelius Hansson - groogy@groogy.se
# This software is provided 'as-is', without any express or
# implied warranty. In no event will the authors be held
# liable for any damages arising from the use of this software.
#
# Permission is granted to anyone to use this software for any purpose,
# including commercial applications, and to alter it and redistribute
# it freely, subject to the following restrictions:
#
# 1. The origin of this software must not be misrepresented;
#    you must not claim that you wrote the original software.
#    If you use this software in a product, an acknowledgment
#    in the product documentation would be appreciated but
#    is not required.
#
# 2. Altered source versions must be plainly marked as such,
#    and must not be misrepresented as being the original software.
#
# 3. This notice may not be removed or altered from any
#    source distribution.

require 'mkmf'
require 'rbconfig'

on_windows = Config::CONFIG['host_os'] =~ /mswin|mingw/


dir_config("graphics")
if (on_windows)
	have_library("winmm")
	have_library("jpeg")
	have_library("gdi32")
	have_library("opengl32")
	have_library("glew")
	have_library("sfml-system")
	have_library("sfml-window")
	have_library("sfml-graphics")
else
	have_library("sfml-graphics")	
end
find_header("main.hpp", "../sfml-system/system")
create_makefile("sfml/graphics", "graphics")
