# all source files
set(SRC Sftp.cpp)

# define the sftp target
sfml_add_example(sftp
                 SOURCES ${SRC}
                 DEPENDS SFML::Network)
