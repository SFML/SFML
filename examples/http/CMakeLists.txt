# all source files
set(SRC Http.cpp)

# define the http target
sfml_add_example(http
                 SOURCES ${SRC}
                 DEPENDS SFML::Network)
