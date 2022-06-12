# all source files
set(SRC Sockets.cpp
        TCP.cpp
        UDP.cpp)

# define the sockets target
sfml_add_example(sockets
                 SOURCES ${SRC}
                 DEPENDS SFML::Network)
