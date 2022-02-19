# define the sockets target
sfml_add_example(sockets
                 DEPENDS SFML::Network)
target_sources(sockets PRIVATE Sockets.cpp TCP.cpp UDP.cpp)
