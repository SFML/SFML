# all source files
set(SRC Ftp.cpp)

# define the ftp target
sfml_add_example(ftp
                 SOURCES ${SRC}
                 DEPENDS SFML::Network)
