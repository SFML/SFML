Android Project Template for SFML
=================================

*The files contained in this directory provide you with a minimalistic, barebone Android project you can use as a starting point for your own native Android app or a port of an existing program or game.*

Prerequirements
---------------

Before you're able to build your project, you'll have to prepare your system.

First of all, you should be familiar with the basic usage of the **Android SDK** as well as the **Android NDK**.

The basic procedure is identical, no matter whether you're starting from scratch or working with existing source code.

Before you start, make sure you've got the following installed on your system (while older versions might work, you should be able to just grab the latest versions):

* [Android SDK](http://developer.android.com/sdk/index.html)

  *Grab the standalone version for an existing SDK if you're not sure which download to get.*

* [Android NDK](http://developer.android.com/tools/sdk/ndk/index.html)

* [Apache Ant](https://www.apache.org/dist/ant/binaries/)

  *You may as well use [Gradle](http://www.gradle.org/) as an alternative build tool, but you might have to convert the project yourself.*

* [Java JDK](http://www.oracle.com/technetwork/java/javase/downloads/index.html)

If you haven't done so, make sure you've [built and installed SFML in your NDK](https://github.com/LaurentGomila/SFML/wiki/Tutorial%3A-Building-SFML-for-Android).


Update the Project Files
------------------------

Getting your basic setup to work is pretty trivial. Just follow the following steps:

* Create a copy of this directory as your working directory. Don't work with the original files, since those might be replaced or updated with future revisions of the source code.

* Before you modify any files, you'll have to come up with a few variables:

  * **App Name:** A name to be shown in places such as the list of installed apps. This can be anything, like
  * **Package Name:** A unique identifier for your app. This is typically your homepage in reverse with some suffix, e.g. **"com.awesomegame.android"**. This should only consist of alphanumeric characters and underscores.
  * **Activity Name:** The name shown for your app in the launcher. This may be the same as your app name.
  * **Library Name:** The internal name of the shared library that will contain your app code. This should only consist of alphanumeric characters, dashes, and underscores.

* Open the file `AndroidManifest.xml` with a text editor.

* Right at the beginning you'll find the line containing `package="com.example.sfml"`. Replace `com.example.sfml` with your *package name*.

* Scroll down and look for the line `<meta-data android:name="sfml.app.lib_name" android:value="my-sfml-project" />`. Replace `my-sfml-project` with your *library name*.

* Save and close the file.

* Open the file `jni/Android.mk` with a text editor.

* Look for the line `LOCAL_MODULE := my-sfml-project` and replace `my-sfml-project` with your *library name*.

* Save and close the file.

* Open the file `jni/Application.mk` with a text editor.

* Look for the line `APP_MODULES := sfml-activity my-sfml-project` and replace `my-sfml-project` with your *library name*.

* Once again save and close the file.

* Open the file `res/values/strings.xml` with a text editor, update all values found inside, and save your change.

* Open a shell or terminal window and navigate to your project directory (where your `AndroidManifest.xml` is located).

* Run the following command: `<SDK PATH>/tools/android update project --path . --target 1`


Building and Installing
-----------------------

Building happens in two steps:

* First you'll have to compile the native part of your app:

  * To do so, issue the command `<NDK PATH>/ndk-build`.

  * To force a rebuild, run `<NDK PATH>/ndk-build -B`.

  * To clean your build directory, manually delete the sub directories `bin`, `libs`, and `obj`.

* Then you're ready to build the actual *APK* file:

  * Run `ant debug` to create a debug build or `ant release` to create a release build.

  * Run `ant installd` to install a previously built debug built; `ant installr` for the release version.

  * To build and install in one step, run `ant debug install` or `ant release install`.

* Once that's done, you should find your new app in your launcher. Unless you modified the source files, the app will show you a blank screen only.


Updating the Code
-----------------

To start working with your code, you can edit the file `jni/main.cpp`.

If you'd like to add additional files (or replace the `main.cpp`, e.g. with an absolute or relative path), open `jni/Android.mk` and update `LOCAL_SRC_FILES` accordingly.


Writing Cross-Platform Code
---------------------------

You can write custom code for Android, but you don't have to. Simply use the preprocessor define `ANDROID` to include/exclude platform specific code.
