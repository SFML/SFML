# Copyright (c) 2016, Ruslan Baratov
# All rights reserved.

set(SRCROOT ${PROJECT_SOURCE_DIR}/examples/X11)

# all source files
set(SRC ${SRCROOT}/X11.cpp)

# define the X11 target
sfml_add_example(X11 GUI_APP
                 SOURCES ${SRC}
                 DEPENDS sfml-window)
