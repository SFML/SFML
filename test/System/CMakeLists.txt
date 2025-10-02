set(SYSTEM_SRC
    Angle.test.cpp
    Clock.test.cpp
    Config.test.cpp
    Err.test.cpp
    Exception.test.cpp
    FileInputStream.test.cpp
    MemoryInputStream.test.cpp
    Sleep.test.cpp
    String.test.cpp
    Time.test.cpp
    Utf.test.cpp
    Vector2.test.cpp
    Vector3.test.cpp
)
sfml_add_test(test-sfml-system "${SYSTEM_SRC}" "")
target_compile_definitions(test-sfml-system PRIVATE
    EXPECTED_SFML_VERSION_MAJOR=${SFML_VERSION_MAJOR}
    EXPECTED_SFML_VERSION_MINOR=${SFML_VERSION_MINOR}
    EXPECTED_SFML_VERSION_PATCH=${SFML_VERSION_PATCH}
    EXPECTED_SFML_VERSION_IS_RELEASE=$<IF:$<BOOL:${VERSION_IS_RELEASE}>,true,false>
)

if(XCODE AND SFML_OS_IOS)
    set(SYSTEM_RESOURCE 
        test.txt
        test2.txt
        test-ń.txt
        test-🐌.txt
    )
    target_sources(test-sfml-system PRIVATE ${SYSTEM_RESOURCE})
    set_source_files_properties(${SYSTEM_RESOURCE} PROPERTIES MACOSX_PACKAGE_LOCATION "System")
endif()