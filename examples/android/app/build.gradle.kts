val NDK_VERSION by extra(project.properties["NDK_VERSION"] as? String ?: "26.1.10909125")
val ARCH_ABI by extra(project.properties["ARCH_ABI"] as? String ?: "arm64-v8a")
val MIN_SDK by extra((project.properties["MIN_SDK"] as? String ?: "21").toInt())
val TARGET_SDK by extra((project.properties["TARGET_SDK"] as? String ?: "33").toInt())
val STL_TYPE by extra(project.properties["STL_TYPE"] as? String ?: "c++_shared")
val SFML_STATIC by extra(project.properties["SFML_STATIC"] as? String ?: "OFF")

plugins {
    id("com.android.application")
}

android {
    namespace = "org.sfmldev.android"
    ndkVersion = NDK_VERSION
    compileSdk = TARGET_SDK
    defaultConfig {
        applicationId = "org.sfmldev.android"
        minSdk = MIN_SDK
        targetSdk = TARGET_SDK
        versionCode = 1
        versionName = "1.0"
        ndk {
            abiFilters.add(ARCH_ABI)
        }
        externalNativeBuild {
            cmake {
                arguments.add("-DANDROID_STL=${STL_TYPE}")
                arguments.add("-DSFML_STATIC_LIBRARIES=${SFML_STATIC}")
            }
        }
    }
    buildTypes {
        release {
            isMinifyEnabled = false
            proguardFiles(getDefaultProguardFile("proguard-android.txt"), "proguard-rules.pro")
        }
    }
    externalNativeBuild {
        cmake {
            path("src/main/jni/CMakeLists.txt")
        }
    }
}

dependencies {
    implementation(fileTree(mapOf("dir" to "libs", "include" to listOf("*.jar"))))
}
