
sfml_add_example(sensor GUI_APP
                 SOURCES Sensor.cpp
                 DEPENDS SFML::Graphics)

# lock to portrait on iOS, as the sensor axes are fixed to the device
if(SFML_OS_IOS)
    set_target_properties(sensor PROPERTIES MACOSX_BUNDLE_INFO_PLIST ${CMAKE_CURRENT_SOURCE_DIR}/info.plist)
endif()
