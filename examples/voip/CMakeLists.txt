# define the voip target
sfml_add_example(voip
                 DEPENDS SFML::Audio SFML::Network)
target_sources(voip PRIVATE VoIP.cpp Client.cpp Server.cpp)
