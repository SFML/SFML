plugins {
    alias(libs.plugins.android.library)
}

android {
    namespace = "com.sfml.sfml"

    defaultConfig {
        minSdk = 24
        compileSdk = 36
        externalNativeBuild {
            cmake {
                arguments += "-DSFML_BUILD_EXAMPLES=ON"
            }
        }
    }

    externalNativeBuild {
        cmake {
            path = file("../../../CMakeLists.txt")
            version = "3.28.0+"
        }
    }
}
