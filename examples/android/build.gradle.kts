// Top-level build file where you can add configuration options common to all sub-projects/modules.

buildscript {
    repositories {
        google()
        mavenCentral()
    }
    dependencies {
        classpath("com.android.tools.build:gradle:7.4.1")
    }
}

allprojects {
    repositories {
        google()
        mavenCentral()
    }
}

tasks.withType<Wrapper> {
    gradleVersion = "7.6.2"
    distributionType = Wrapper.DistributionType.BIN
}

tasks.register("clean", Delete::class) {
    delete(rootProject.layout.buildDirectory)
}
