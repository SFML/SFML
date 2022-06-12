# all source files
set(SRC VoIP.cpp
        Client.cpp
        Server.cpp)

# define the voip target
sfml_add_example(voip
                 SOURCES ${SRC}
                 DEPENDS SFML::Audio SFML::Network)
