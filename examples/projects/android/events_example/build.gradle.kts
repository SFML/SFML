plugins {
    alias(libs.plugins.android.application)
    alias(libs.plugins.kotlin.android)
    alias(libs.plugins.kotlin.compose)
}

android {
    namespace = "com.sfml.events_example"
    compileSdk = 36
    sourceSets["main"].assets {
        srcDir("../../../event_handling")

    }

    defaultConfig {
        applicationId = "com.sfml.events_example"
        minSdk = 24
        targetSdk = 36
        versionCode = 1
        versionName = "1.0"
    }
}

dependencies {
    implementation(project(":SFML"))
}
