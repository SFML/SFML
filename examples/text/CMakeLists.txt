# all source files
set(SRC Text.cpp)
if(SFML_OS_IOS)
    set(RESOURCES
        resources/NotoNaskhArabic-Regular.ttf
        resources/NotoSerif-Regular.ttf
        resources/NotoSerifArmenian-Regular.ttf
        resources/NotoSerifDevanagari-Regular.ttf
        resources/NotoSerifEthiopic-Regular.ttf
        resources/NotoSerifGeorgian-Regular.ttf
        resources/NotoSerifHebrew-Regular.ttf
        resources/NotoSerifJP-Regular.subset.ttf
        resources/NotoSerifKhmer-Regular.ttf
        resources/NotoSerifKR-Regular.subset.ttf
        resources/NotoSerifMyanmar-Regular.ttf
        resources/NotoSerifTamil-Regular.ttf
        resources/NotoSerifTC-Regular.subset.ttf
        resources/NotoSerifThai-Regular.ttf
        resources/NotoSerifTibetan-Regular.ttf
        resources/tuffy.ttf)
    set_source_files_properties(${RESOURCES} PROPERTIES MACOSX_PACKAGE_LOCATION Resources)
endif()

# define the text target
sfml_add_example(text GUI_APP
                 SOURCES ${SRC}
                 BUNDLE_RESOURCES ${RESOURCES}
                 DEPENDS SFML::Graphics)
