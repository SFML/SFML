# define the ftp target
sfml_add_example(ftp
                 DEPENDS SFML::Network)
target_sources(ftp PRIVATE Ftp.cpp)
