# all source files
set(SRC SoundStage.cpp imgui-SFML.cpp imgui.cpp imgui_widgets.cpp imgui_draw.cpp imgui_tables.cpp imgui_stdlib.cpp)

# define the sound-stage target
sfml_add_example(sound-stage GUI_APP
                 SOURCES ${SRC}
                 DEPENDS SFML::Audio SFML::Graphics
                 RESOURCES_DIR resources)

# external dependency headers
target_include_directories(sound-stage SYSTEM PRIVATE ${PROJECT_SOURCE_DIR}/examples/sound_stage)
