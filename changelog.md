# Changelog

## SFML 2.5

Also available on the website: https://www.sfml-dev.org/changelog.php#sfml-2.5

### General

  * Markdown'd and updated readme, changelog, contributing and license files ([#1196](https://github.com/SFML/SFML/pull/1196), [#1368](https://github.com/SFML/SFML/pull/1368), [#1317](https://github.com/SFML/SFML/pull/1317))
  * Improve packaging support ([#1173](https://github.com/SFML/SFML/pull/1173))
  * Added Tagfile generation and search ([#1327](https://github.com/SFML/SFML/pull/1327))
  * Set policy CMP0042 if cmake version > 3.0 ([#1206](https://github.com/SFML/SFML/pull/1206))
  * Added CMake variables to select the modules to be built ([#800](https://github.com/SFML/SFML/pull/800))
  * Updated the binary libs from exlibs/libs-ios in order to work with Xcode 8 and iOS 10 ([#1209](https://github.com/SFML/SFML/pull/1209))
  * Do not install extlibs if `SFML_USE_SYSTEM_DEPS` is true ([#1237](https://github.com/SFML/SFML/pull/1237))
  * Updated stb_image to v2.16 and stb_image_write to v1.07 ([#1270](https://github.com/SFML/SFML/pull/1270))
  * Fixed various type conversion/comparison warnings ([#1325](https://github.com/SFML/SFML/pull/1325))
  * [macOS] Fixed launch of cocoa examples ([#1334](https://github.com/SFML/SFML/pull/1334))
  * [macOS] Improved application signing process ([#1194](https://github.com/SFML/SFML/pull/1194))
  * [macOS] Improve cmake script ([#1215](https://github.com/SFML/SFML/pull/1215))
  * [macOS] Use `-stdlib=libc++` ([#1361](https://github.com/SFML/SFML/pull/1361))
  * [macOS] Fix CMake warning since CMake 3.9 ([#1371](https://github.com/SFML/SFML/pull/1371))
  * [iOS] Use a CMake toolchain file for iOS build ([#1269](https://github.com/SFML/SFML/pull/1269))
  * [iOS] Install extlibs if needed ([#1348](https://github.com/SFML/SFML/pull/1348))
  * [iOS] Drop 32 bit support ([#1374](https://github.com/SFML/SFML/pull/1374))
  * [iOS] Force correct iOS architecture for cmake ([#1377](https://github.com/SFML/SFML/pull/1377))
  * [iOS] Find OpenAL correctly ([#1376](https://github.com/SFML/SFML/pull/1376))
  * [Android] Increased minimum API version to 14 ([#1362](https://github.com/SFML/SFML/pull/1362))
  * [Android] Removed custom toolchain and added support for the newest NDK version ([#1350](https://github.com/SFML/SFML/pull/1350))

### System

**Bugfixes**

  * Fixed crash when `sf::Clock` is constructed in a global scope ([#1258](https://github.com/SFML/SFML/pull/1258))

### Window

**Features**

  * Implemented Cursor API ([#827](https://github.com/SFML/SFML/pull/827))
  * Implemented Clipboard API ([#1221](https://github.com/SFML/SFML/pull/1221))
  * [Windows] Added support for interfacing with joysticks via DirectInput when it is available ([#1326](https://github.com/SFML/SFML/pull/1326))

**Bugfixes**

  * Updated documentation ([#1255](https://github.com/SFML/SFML/pull/1255), [#1310](https://github.com/SFML/SFML/pull/1310))
  * [macOS] Fixed window menu not working ([#1193](https://github.com/SFML/SFML/pull/1193))
  * [macOS] Removed unused variable ([#1276](https://github.com/SFML/SFML/pull/1276))
  * [macOS] Fixed crash with application messing hardware detection e.g. TeamViewer ([#1323](https://github.com/SFML/SFML/pull/1323))
  * [macOS] Added support for (some) Hat/POV axis ([#1248](https://github.com/SFML/SFML/pull/1248))
  * [Linux] Fixed `sf::Window::getPosition()` returning incorrect position because of differences in window managers ([#1228](https://github.com/SFML/SFML/pull/1228), [#1266](https://github.com/SFML/SFML/pull/1266))
  * [Linux] Fix X11 key repeat handling not filtering out events from other windows ([#1291](https://github.com/SFML/SFML/pull/1291), [#1223](https://github.com/SFML/SFML/pull/1223), [#1230](https://github.com/SFML/SFML/pull/1230))
  * [Linux] Restore fullscreen of a non-visible window ([#1339](https://github.com/SFML/SFML/pull/1339))
  * [iOS] Fix build error when using @import ([#1272](https://github.com/SFML/SFML/pull/1272))
  * [iOS] Don't need to find vorbisfile or vorbisenc ([#1347](https://github.com/SFML/SFML/pull/1347))
  * [Windows] Prevent uninitialized read by zeroing memory ([#1264](https://github.com/SFML/SFML/pull/1264))
  * [Windows] Fixed modifier keys handling ([#1357](https://github.com/SFML/SFML/pull/1357))
  * [Android] Return correct key code for delete/backspace ([#1362](https://github.com/SFML/SFML/pull/1362))

### Graphics

**Features**

  * Implemented additional line spacing and letter spacing in `sf::Text` ([#928](https://github.com/SFML/SFML/pull/928), [#1366](https://github.com/SFML/SFML/pull/1366))
  * Added `sf::VertexBuffer` class ([#1308](https://github.com/SFML/SFML/pull/1308))
  * Added GPU local texture copying support, allowing performance optimizations and texture swapping ([#1119](https://github.com/SFML/SFML/pull/1119), [#1320](https://github.com/SFML/SFML/pull/1320))
  * Optimize performance by skipping `glTexCoordPointer()` call if not needed ([#1015](https://github.com/SFML/SFML/pull/1015))
  * Generate shape outline vertices only if necessary ([#1356](https://github.com/SFML/SFML/pull/1356))
  * Removed dependency to libjpeg, stb_image_write now supports writing JPEG files ([#1279](https://github.com/SFML/SFML/pull/1279))
  * Enable comparing `sf::Transform` and optimize resetting OpenGL back to the identity matrix ([#1298](https://github.com/SFML/SFML/pull/1298))

**Bugfixes**

  * Fixed typo in assert in `sf::Texture::update()` ([#1210](https://github.com/SFML/SFML/pull/1210))
  * Properly free memory in `sf::Font::cleanup()` ([#1119](https://github.com/SFML/SFML/pull/1119))
  * Fixed memory leak in `sf::Font` ([#1216](https://github.com/SFML/SFML/pull/1216))
  * Fix OpenGL texture coordinate pointer not being updated correctly under certain conditions ([#1297](https://github.com/SFML/SFML/pull/1297))
  * Fix for broken text when the font is reloaded ([#1345](https://github.com/SFML/SFML/pull/1345))
  * Fix memory leak in `sf::Text` ([#1360](https://github.com/SFML/SFML/pull/1360))
  * Refactoring of `sf::Text` ([#928](https://github.com/SFML/SFML/pull/928))
  * Updated documentation ([#1197](https://github.com/SFML/SFML/pull/1197))
  * [Windows] Replaced time-based joystick poll with a hardware event handler ([#1195](https://github.com/SFML/SFML/pull/1195), [#1179](https://github.com/SFML/SFML/pull/1179))
  * [macOS] Worked around render target bug ([#1342](https://github.com/SFML/SFML/pull/1342))

### Audio

**Features**

  * Added loop point support to `sf::Music` ([#629](https://github.com/SFML/SFML/pull/629))
  * Added support for extensible format PCM wave files ([#1296](https://github.com/SFML/SFML/pull/1296))
  * [iOS] Enable audio module ([#1338](https://github.com/SFML/SFML/pull/1338))

**Bugfixes**

  * Allow polymorphism with `sf::SoundSource` ([#1185](https://github.com/SFML/SFML/pull/1185))
  * Fixed WAV file writer writing wrong header values ([#1281](https://github.com/SFML/SFML/pull/1281))
  * Small bugfix to argument of `alcCaptureOpenDevice()` ([#1305](https://github.com/SFML/SFML/pull/1305))
  * Removed dead code ([#1261](https://github.com/SFML/SFML/pull/1261))
  * Updated documentation ([#1321](https://github.com/SFML/SFML/pull/1321), [#1353](https://github.com/SFML/SFML/pull/1353))
  * [Windows] Updated OpenAL Soft to 1.18.1 ([#1260](https://github.com/SFML/SFML/pull/1260))

### Network

**Bugfixes**

  * Fixed wrong condition for building network support ([#1253](https://github.com/SFML/SFML/pull/1253))

## SFML 2.4.2

Also available on the website: https://www.sfml-dev.org/changelog.php#sfml-2.4.2

### System

**Bugfixes**

  * [Windows] Removed thread affinity changes in sf::Clock ([#1107](https://github.com/SFML/SFML/pull/1107))

### Window

**Bugfixes**

  * Fixed bug where TransientContextLock would hang ([#1165](https://github.com/SFML/SFML/pull/1165), [#1172](https://github.com/SFML/SFML/pull/1172))
  * [Linux] Fixed GLX extensions being loaded too late ([#1183](https://github.com/SFML/SFML/pull/1183))
  * [Linux] Fix wrong types passed to XChangeProperty ([#1168](https://github.com/SFML/SFML/pull/1168) [#1171](https://github.com/SFML/SFML/pull/1171))
  * [Windows] Make context disabling via wglMakeCurrent more tolerant of broken drivers ([#1186](https://github.com/SFML/SFML/pull/1186))

### Graphics

**Bugfixes**

  * Optimized sf::Image::create and made it more exception safe ([#1166](https://github.com/SFML/SFML/pull/1166))

## SFML 2.4.1

Also available on the website: https://www.sfml-dev.org/changelog.php#sfml-2.4.1

### General

  * [kFreeBSD] Define SFML_OS_FREEBSD when compiling for kFreeBSD ([#1129](https://github.com/SFML/SFML/pull/1129))
  * [Windows] Added some simple messaging when trying to build under Cygwin ([#1153](https://github.com/SFML/SFML/pull/1153))

### Window

**Bugfixes**

  * Fixed stack overflow on GlContext creation with multiple threads ([#989](https://github.com/SFML/SFML/pull/989), [#1002](https://github.com/SFML/SFML/pull/1002))
  * Adjusted mouse cursor grab documentation ([#1133](https://github.com/SFML/SFML/pull/1133))
  * [iOS] Fixed orientation change not rescaling window size properly ([#1049](https://github.com/SFML/SFML/pull/1049), [#1050](https://github.com/SFML/SFML/pull/1050))
  * [Linux] Fixed fullscreen issue ([#921](https://github.com/SFML/SFML/pull/921), [#1138](https://github.com/SFML/SFML/pull/1138))
  * [Linux] Switched from XCB back to Xlib for windowing ([#1138](https://github.com/SFML/SFML/pull/1138))
  * [Linux] Fixed window icon not showing up on some distros ([#1087](https://github.com/SFML/SFML/pull/1087), [#1088](https://github.com/SFML/SFML/pull/1088))
  * [Linux] Fixed an issue where GNOME flags window unresponsive ([#1089](https://github.com/SFML/SFML/pull/1089), [#1138](https://github.com/SFML/SFML/pull/1138))
  * [Linux] Fixed leak of XVisualInfo objects during GlxContext creation ([#1135](https://github.com/SFML/SFML/pull/1135))
  * [Linux] Fixed possible hang when setting visibility if external window sources ([#1136](https://github.com/SFML/SFML/pull/1136))
  * [OS X] Fixed inconsistency between doc and impl on OS X for the grab feature ([#1133](https://github.com/SFML/SFML/pull/1133), [#1148](https://github.com/SFML/SFML/pull/1148), [#1150](https://github.com/SFML/SFML/pull/1150))
  * [Windows] Fixed context memory leaks ([#1143](https://github.com/SFML/SFML/pull/1143), [#1002](https://github.com/SFML/SFML/pull/1002))

### Graphics

**Bugfixes**

  * Adjusted uniform error message ([#1131](https://github.com/SFML/SFML/pull/1131))
  * Clarify documentation on Rect::contains function bounds ([#1151](https://github.com/SFML/SFML/pull/1151))

### Network

**Bugfixes**

  * Fixed a typo in comment for void unbind() ([#1121](https://github.com/SFML/SFML/pull/1121))

## SFML 2.4.0

Also available on the website: https://www.sfml-dev.org/changelog.php#sfml-2.4.0

### General

  * Added deprecation macro ([#969](https://github.com/SFML/SFML/pull/969))
  * Fixed issues reported by Coverity Scan static analysis ([#1064](https://github.com/SFML/SFML/pull/1064))
  * Fixed some initialization issues reported by Cppcheck ([#1008](https://github.com/SFML/SFML/pull/1008))
  * Changed comment chars in FindSFML.cmake to # ([#1090](https://github.com/SFML/SFML/pull/1090))
  * Fixed some typos ([#1098](https://github.com/SFML/SFML/pull/1098), [#993](https://github.com/SFML/SFML/pull/993), [#1099](https://github.com/SFML/SFML/pull/1099), [#956](https://github.com/SFML/SFML/pull/956), [#963](https://github.com/SFML/SFML/pull/963), [#979](https://github.com/SFML/SFML/pull/979))
  * Updated/fixed string comparisons in Config.cmake ([#1102](https://github.com/SFML/SFML/pull/1102))
  * Added the missing -s postfix for the RelWithDebInfo config ([#1014](https://github.com/SFML/SFML/pull/1014))
  * [Android] Fixed current Android compilation issues ([#1116](https://github.com/SFML/SFML/pull/1116), [#1111](https://github.com/SFML/SFML/pull/1111), [#1079](https://github.com/SFML/SFML/pull/1079))
  * [OS X] Update Xcode template material ([#976](https://github.com/SFML/SFML/pull/976), [#968](https://github.com/SFML/SFML/pull/968))
  * [Windows] Added support for VS 2015 ([#972](https://github.com/SFML/SFML/pull/972))
  * [Windows] Create and install PDB debug symbols alongside binaries ([#1037](https://github.com/SFML/SFML/pull/1037))

### Deprecated API

  * sf::RenderWindow::capture(): Use a sf::Texture and its sf::Texture::update(const Window&) function and copy its contents into an sf::Image instead.
  * sf::Shader::setParameter(): Use setUniform() instead.
  * sf::Text::getColor(): There is now fill and outline colors instead of a single global color. Use getFillColor() or getOutlineColor() instead.
  * sf::Text::setColor(): There is now fill and outline colors instead of a single global color. Use setFillColor() or setOutlineColor() instead.
  * sf::LinesStrip: Use LineStrip instead.
  * sf::TrianglesFan: Use TriangleFan instead.
  * sf::TrianglesStrip: Use TriangleStrip instead.

### System

**Features**

  * [Android] Added sf::getNativeActivity() ([#1005](https://github.com/SFML/SFML/pull/1005), [#680](https://github.com/SFML/SFML/pull/680))

**Bugfixes**

  * Added missing <iterator> include in String.hpp ([#1069](https://github.com/SFML/SFML/pull/1069), [#1068](https://github.com/SFML/SFML/pull/1068))
  * Fixed encoding of UTF-16 ([#997](https://github.com/SFML/SFML/pull/997))
  * [Android] Fixed crash when trying to load a non-existing font file ([#1058](https://github.com/SFML/SFML/pull/1058))

### Window

**Features**

  * Added ability to grab cursor ([#614](https://github.com/SFML/SFML/pull/614), [#394](https://github.com/SFML/SFML/pull/394), [#1107](https://github.com/SFML/SFML/pull/1107))
  * Added Multi-GPU preference ([#869](https://github.com/SFML/SFML/pull/869), [#867](https://github.com/SFML/SFML/pull/867))
  * Added support for sRGB capable framebuffers ([#981](https://github.com/SFML/SFML/pull/981), [#175](https://github.com/SFML/SFML/pull/175))
  * [Linux, Windows] Improved OpenGL context creation ([#884](https://github.com/SFML/SFML/pull/884))
  * [Linux, Windows] Added support for pbuffers on Windows and Unix ([#885](https://github.com/SFML/SFML/pull/885), [#434](https://github.com/SFML/SFML/pull/434))

**Bugfixes**

  * Updated platform-specific handle documentation ([#961](https://github.com/SFML/SFML/pull/961))
  * [Android] Accept touch events from "multiple" devices ([#954](https://github.com/SFML/SFML/pull/954), [#953](https://github.com/SFML/SFML/pull/953))
  * [Android] Copy the selected EGL context's settings to SFML ([#1039](https://github.com/SFML/SFML/pull/1039))
  * [Linux] Fixed modifiers causing sf::Keyboard::Unknown being returned ([#1022](https://github.com/SFML/SFML/pull/1022), [#1012](https://github.com/SFML/SFML/pull/1012))
  * [OS X] Improved memory management on OS X ([#962](https://github.com/SFML/SFML/pull/962), [#790](https://github.com/SFML/SFML/pull/790))
  * [OS X] Fixed crash when resizing a window to a zero-height/width size ([#986](https://github.com/SFML/SFML/pull/986), [#984](https://github.com/SFML/SFML/pull/984))
  * [OS X] Use the mouse button constant instead of 0 to avoid a compiler error on OSX ([#1035](https://github.com/SFML/SFML/pull/1035))
  * [OS X] OS X improvement: warnings + bugfix + refactoring, the lot! ([#1042](https://github.com/SFML/SFML/pull/1042))

### Graphics

**Features**

  * Added support for outlined text ([#840](https://github.com/SFML/SFML/pull/840))
  * Add support for geometry shaders ([#886](https://github.com/SFML/SFML/pull/886), [#428](https://github.com/SFML/SFML/pull/428))
  * Feature/blend mode reverse subtract ([#945](https://github.com/SFML/SFML/pull/945), [#944](https://github.com/SFML/SFML/pull/944))
  * Implemented support for mipmap generation ([#973](https://github.com/SFML/SFML/pull/973), [#498](https://github.com/SFML/SFML/pull/498), [#123](https://github.com/SFML/SFML/pull/123))
  * Added new API to set shader uniforms ([#983](https://github.com/SFML/SFML/pull/983), [#538](https://github.com/SFML/SFML/pull/538))
  * Rewrite RenderWindow::capture ([#1001](https://github.com/SFML/SFML/pull/1001))

**Bugfixes**

  * Exporting some Glsl utility functions due to linking issues ([#1044](https://github.com/SFML/SFML/pull/1044), [#1046](https://github.com/SFML/SFML/pull/1046))
  * Fixed missing initialisation of Font::m_stroker ([#1059](https://github.com/SFML/SFML/pull/1059))
  * Changed primitive types to be grammatically correct ([#1095](https://github.com/SFML/SFML/pull/1095), [#939](https://github.com/SFML/SFML/pull/939))

### Audio

**Features**

  * Implemented stereo audio recording ([#1010](https://github.com/SFML/SFML/pull/1010))

**Bugfixes**

  * Added an assignment operator to SoundSource ([#864](https://github.com/SFML/SFML/pull/864))
  * [OS X] Updates OpenAL-soft for OS X to version 1.17.2 ([#1057](https://github.com/SFML/SFML/pull/1057), [#900](https://github.com/SFML/SFML/pull/900), [#1000](https://github.com/SFML/SFML/pull/1000))
  * Fixed a bug where vorbis can't handle large buffers ([#1067](https://github.com/SFML/SFML/pull/1067))
  * Added support for 24-bit .wav files ([#958](https://github.com/SFML/SFML/pull/958), [#955](https://github.com/SFML/SFML/pull/955))
  * Fixed threading issue in sf::SoundRecorder ([#1011](https://github.com/SFML/SFML/pull/1011))
  * Made WAV file reader no longer assume that data chunk goes till end of file to prevent reading trailing metadata as samples ([#1018](https://github.com/SFML/SFML/pull/1018))
  * Fixed seeking in multi channel FLAC files ([#1041](https://github.com/SFML/SFML/pull/1041), [#1040](https://github.com/SFML/SFML/pull/1040))

### Network

**Features**

  * Added optional argument on which address to bind (socket). ([#850](https://github.com/SFML/SFML/pull/850), [#678](https://github.com/SFML/SFML/pull/678))

**Bugfixes**

  * Fixed FTP directory listing blocking forever ([#1086](https://github.com/SFML/SFML/pull/1086), [#1025](https://github.com/SFML/SFML/pull/1025))

## SFML 2.3.2

Also available on the website: https://www.sfml-dev.org/changelog.php#sfml-2.3.2

### General

  * Fixed an issue where FindSFML.cmake couldn't find older versions of SFML ([#903](https://github.com/SFML/SFML/pull/903))
  * Robust alCheck and glCheck macros ([#917](https://github.com/SFML/SFML/pull/917))
  * Fixed FindSFML.cmake to use the uppercase FLAC name ([#923](https://github.com/SFML/SFML/pull/923))
  * Added a CONTRIBUTING file so GitHub shows a message when creating a new issue ([#932](https://github.com/SFML/SFML/pull/932))

### Window

**Bugfixes**

  * [Linux] Fixed an issue where the keypad's key weren't being detected ([#910](https://github.com/SFML/SFML/pull/910))
  * [Linux] Revert to Xlib event handling ([#934](https://github.com/SFML/SFML/pull/934))
  * [Linux] Fixed `XK_*` inconsistency in InpuImpl.cpp ([#947](https://github.com/SFML/SFML/pull/947))
  * [Linux] Fix `_NET_WM_PING` messages not being replied to properly ([#947](https://github.com/SFML/SFML/pull/947))

### Graphics

**Bugfixes**

  * Fixed clear bug on RenderTextures ([#915](https://github.com/SFML/SFML/pull/915))
  * Fixed image file extension detection ([#929](https://github.com/SFML/SFML/pull/929), [#930](https://github.com/SFML/SFML/pull/930), [#931](https://github.com/SFML/SFML/pull/931))
  * Secure function against random data return ([#935](https://github.com/SFML/SFML/pull/935), [#942](https://github.com/SFML/SFML/pull/942))

## SFML 2.3.1

Also available on the website: https://www.sfml-dev.org/changelog.php#sfml-2.3.1

### Window

**Bugfixes**

  * [Android] Make sure a window still exists before trying to access its dimensions ([#854](https://github.com/SFML/SFML/pull/854))
  * [Android] Added Android API level checks ([#856](https://github.com/SFML/SFML/pull/856))
  * [Android] Updated the JNI/event handling code ([#906](https://github.com/SFML/SFML/pull/906))
  * [Linux] Resized events are only spawned when the window size actually changes ([#878](https://github.com/SFML/SFML/pull/878), [#893](https://github.com/SFML/SFML/pull/893))
  * [Linux] Whitelisted X SHAPE events ([#879](https://github.com/SFML/SFML/pull/879), [#883](https://github.com/SFML/SFML/pull/883))
  * [Linux] Remap Unix keyboard when user changes layout ([#895](https://github.com/SFML/SFML/pull/895), [#897](https://github.com/SFML/SFML/pull/897))
  * [Linux] Fix undefined behavior in ewmhSupported() ([#892](https://github.com/SFML/SFML/pull/892), [#901](https://github.com/SFML/SFML/pull/901))

### Graphics

**Bugfixes**

  * Added support for GL_EXT_texture_edge_clamp for systems that don't expose GL_SGIS_texture_edge_clamp ([#880](https://github.com/SFML/SFML/pull/880), [#882](https://github.com/SFML/SFML/pull/882))

### Audio

**Bugfixes**

  * [Android] Fixed audio files not loading (and possibly crashing) ([#855](https://github.com/SFML/SFML/pull/855), [#887](https://github.com/SFML/SFML/pull/887))

## SFML 2.3

Also available on the website: https://www.sfml-dev.org/changelog.php#sfml-2.3

### General

  * Examples only link against sfml-main in release mode ([#610](https://github.com/SFML/SFML/pull/610), [#766](https://github.com/SFML/SFML/pull/766))
  * Replaced unsigned int with std::size_t for array indices and sizes ([#739](https://github.com/SFML/SFML/pull/739))
  * Fixed some issues with the Doxygen documentation ([#750](https://github.com/SFML/SFML/pull/750))
  * Added support for EditorConfig ([#751](https://github.com/SFML/SFML/pull/751))
  * Hide success message for CMake in quiet mode ([#753](https://github.com/SFML/SFML/pull/753))
  * Improved documentation for statuses with sf::Ftp ([#763](https://github.com/SFML/SFML/pull/763))
  * Moved stb_image into the extlibs directory ([#795](https://github.com/SFML/SFML/pull/795))
  * Changed the SOVERSION to major.minor ([#812](https://github.com/SFML/SFML/pull/812))
  * Fixed warnings about switch-statements ([#863](https://github.com/SFML/SFML/pull/863))
  * Added missing includes in the general headers ([#851](https://github.com/SFML/SFML/pull/851))
  * [Android] Updated toolchain file and dependencies ([#791](https://github.com/SFML/SFML/pull/791))
  * [Linux] Fixed missing pthread dependency ([#794](https://github.com/SFML/SFML/pull/794))
  * [OS X] Relaxed CMake installation rules regarding framework dependencies ([#767](https://github.com/SFML/SFML/pull/767))

### Deprecated API

  * sf::Event::MouseWheelEvent: This event is deprecated and potentially inaccurate. Use MouseWheelScrollEvent instead.

### Window

**Features**

  * Added new events for handling high-precision scrolling ([#95](https://github.com/SFML/SFML/pull/95), [#810](https://github.com/SFML/SFML/pull/810), [#837](https://github.com/SFML/SFML/pull/837))
  * Switched from Xlib to XCB ([#200](https://github.com/SFML/SFML/pull/200), [#319](https://github.com/SFML/SFML/pull/319), [#694](https://github.com/SFML/SFML/pull/694), [#780](https://github.com/SFML/SFML/pull/780), [#813](https://github.com/SFML/SFML/pull/813), [#825](https://github.com/SFML/SFML/pull/825))
  * Added support for OpenGL 3 core context creation ([#654](https://github.com/SFML/SFML/pull/654), [#779](https://github.com/SFML/SFML/pull/779))

**Bugfixes**

  * Fixed glXSwapIntervalSGI being broken for some driver implementations ([#727](https://github.com/SFML/SFML/pull/727), [#779](https://github.com/SFML/SFML/pull/779))
  * Fixed simultaneous context operations causing crashes on some AMD hardware ([#778](https://github.com/SFML/SFML/pull/778), [#779](https://github.com/SFML/SFML/pull/779))
  * Fixed joystick identification ([#809](https://github.com/SFML/SFML/pull/809), [#811](https://github.com/SFML/SFML/pull/811))
  * [iOS] Fixed various issues including stencil bits, device orientation and retina support ([#748](https://github.com/SFML/SFML/pull/748))
  * [iOS] Fixed inconsistency between sf::Touch::getPosition and touch events ([#875](https://github.com/SFML/SFML/pull/875))
  * [Linux] Fixed Alt+F4 not getting triggered in window mode ([#274](https://github.com/SFML/SFML/pull/274))
  * [Linux] Fixed Unix joystick stuff ([#838](https://github.com/SFML/SFML/pull/838))
  * [OS X] Fixed typo in JoystickImpl.cpp to prevent a crash ([#762](https://github.com/SFML/SFML/pull/762), [#765](https://github.com/SFML/SFML/pull/765))
  * [OS X] Fixed an issue in InputImpl::getSFOpenGLViewFromSFMLWindow ([#782](https://github.com/SFML/SFML/pull/782), [#792](https://github.com/SFML/SFML/pull/792))

### Graphics

**Features**

  * Replaced GLEW with loader generated by glLoadGen ([#779](https://github.com/SFML/SFML/pull/779))
  * Added a new constructor to sf::Color that takes an sf::Uint32 ([#722](https://github.com/SFML/SFML/pull/722))
  * Updated stb_image to v2.02 ([#777](https://github.com/SFML/SFML/pull/777))
  * Updated FreeType to v2.5.5 ([#799](https://github.com/SFML/SFML/pull/799), [#804](https://github.com/SFML/SFML/pull/804))
  * Added checks for software OpenGL ([#870](https://github.com/SFML/SFML/pull/870))

**Bugfixes**

  * Fixed GL_ARB_compatibility not being detected ([#859](https://github.com/SFML/SFML/pull/859))
  * Fixed pixel format selection ([#862](https://github.com/SFML/SFML/pull/862))
  * Bumped back the OpenGL version requirement to 1.1 ([#858](https://github.com/SFML/SFML/pull/858))

### Audio

**Features**

  * Dropped libsndfile and started using Vorbis, FLAC and OGG directly ([#604](https://github.com/SFML/SFML/pull/604), [#757](https://github.com/SFML/SFML/pull/757))
  * Added a FLAC file to the sound example ([#815](https://github.com/SFML/SFML/pull/815))

**Bugfixes**

  * Fixed access violation error in the destructor of sf::AudioDevice ([#30](https://github.com/SFML/SFML/pull/30), [#602](https://github.com/SFML/SFML/pull/602))
  * [OS X] Fixed threading issue with sf::SoundStream and OpenAL ([#541](https://github.com/SFML/SFML/pull/541), [#831](https://github.com/SFML/SFML/pull/831))

### Network

**Bugfixes**

  * Fixed sf::TcpSocket not handling partial sends properly ([#749](https://github.com/SFML/SFML/pull/749), [#796](https://github.com/SFML/SFML/pull/796))

## SFML 2.2

Also available on the website: https://www.sfml-dev.org/changelog.php#sfml-2.2

### General

  * Support for iOS and Android platform ([#410](https://github.com/SFML/SFML/pull/410), [#440](https://github.com/SFML/SFML/pull/440))
  * Various documentation corrections ([#438](https://github.com/SFML/SFML/pull/438), [#496](https://github.com/SFML/SFML/pull/496), [#497](https://github.com/SFML/SFML/pull/497), [#714](https://github.com/SFML/SFML/pull/714))
  * Fixed support for compilers on Debian FreeBSD ([#380](https://github.com/SFML/SFML/pull/380), [#578](https://github.com/SFML/SFML/pull/578))
  * Added support for Visual Studio 2013 and proper support for the TDM builds ([#482](https://github.com/SFML/SFML/pull/482))
  * Fixed CMake problems related to FindSFML and cached variables ([#637](https://github.com/SFML/SFML/pull/637), [#684](https://github.com/SFML/SFML/pull/684))
  * Switched and enforced LF line endings ([#708](https://github.com/SFML/SFML/pull/708), [#712](https://github.com/SFML/SFML/pull/712))
  * Updated OpenAL to version 1.15.1 (d077210)
  * Made compiler and OS variable names much clearer in CMake files (9b0ed30)
  * Re-enabled RPATH feature (e157e7a)
  * Slight adjustments to the examples ([#737](https://github.com/SFML/SFML/pull/737))
  * [FreeBSD] Various configuration fixes ([#577](https://github.com/SFML/SFML/pull/577), [#578](https://github.com/SFML/SFML/pull/578))
  * [Linux] Updated FindSFML.cmake to add UDev to SFML's dependencies ([#728](https://github.com/SFML/SFML/pull/728), [#729](https://github.com/SFML/SFML/pull/729), [#734](https://github.com/SFML/SFML/pull/734), [#736](https://github.com/SFML/SFML/pull/736))
  * [OS X] Fixed incorrect symlink in freetype.framework ([#519](https://github.com/SFML/SFML/pull/519))
  * [OS X] CMake module for correct dependencies ([#548](https://github.com/SFML/SFML/pull/548))
  * [OS X] Fixed SFML target for Xcode ([#595](https://github.com/SFML/SFML/pull/595), [#596](https://github.com/SFML/SFML/pull/596))
  * [OS X] Updated implementation, mainly reverting to non-ARC ([#601](https://github.com/SFML/SFML/pull/601))
  * [OS X] Fixed memory leaks and dead store ([#615](https://github.com/SFML/SFML/pull/615))
  * [OS X] Improved event handling and performance ([#617](https://github.com/SFML/SFML/pull/617))
  * [OS X] Reduced memory usage ([#672](https://github.com/SFML/SFML/pull/672), [#698](https://github.com/SFML/SFML/pull/698))
  * [OS X] OS X 10.10 support ([#691](https://github.com/SFML/SFML/pull/691), [#699](https://github.com/SFML/SFML/pull/699))
  * [OS X] Improve flexibility of dependencies' locations ([#713](https://github.com/SFML/SFML/pull/713))
  * [Windows] Removed the hack that copied external libraries into SFML static libraries (dbf01a7)

### System

**Features**

  * Added substring and replace functions to sf::String ([#21](https://github.com/SFML/SFML/pull/21), [#355](https://github.com/SFML/SFML/pull/355))
  * Added toUtfX to sf::String ([#501](https://github.com/SFML/SFML/pull/501))
  * Added fromUtfX functions to set the internal data to a string by converting from another string in a fixed encoding ([#196](https://github.com/SFML/SFML/pull/196))
  * Added modulo operator for sf::Time ([#429](https://github.com/SFML/SFML/pull/429), [#430](https://github.com/SFML/SFML/pull/430))
  * Added division operator for sf::Time ([#453](https://github.com/SFML/SFML/pull/453))

**Bugfixes**

  * Ensured a high resolution for sf::sleep ([#439](https://github.com/SFML/SFML/pull/439), [#475](https://github.com/SFML/SFML/pull/475))
  * [Windows] Fixed stack unalignment by two internal functions ([#412](https://github.com/SFML/SFML/pull/412))

### Window

**Features**

  * Added window methods to request and to check focus ([#518](https://github.com/SFML/SFML/pull/518), [#525](https://github.com/SFML/SFML/pull/525), [#613](https://github.com/SFML/SFML/pull/613), [#723](https://github.com/SFML/SFML/pull/723), [#735](https://github.com/SFML/SFML/pull/735))
  * Provide name, manufacturer ID and product ID via sf::Joystick ([#152](https://github.com/SFML/SFML/pull/152), [#528](https://github.com/SFML/SFML/pull/528))
  * [FreeBSD] Joystick support ([#477](https://github.com/SFML/SFML/pull/477))
  * [OS X] Improved integration with menus and dock actions ([#11](https://github.com/SFML/SFML/pull/11))
  * [OS X] Support for OpenGL 3.2 ([#84](https://github.com/SFML/SFML/pull/84))
  * [OS X] Improved fullscreen support ([#343](https://github.com/SFML/SFML/pull/343))
  * [OS X] Added support for retina displays ([#353](https://github.com/SFML/SFML/pull/353), [#388](https://github.com/SFML/SFML/pull/388))
  * [Windows] Removed support for Windows 9x ([#469](https://github.com/SFML/SFML/pull/469))
  * [Windows] Fixed typo in Windows keyboard implementation ([#516](https://github.com/SFML/SFML/pull/516))

**Bugfixes**

  * sf::Window::create() now also resets framerate limit ([#371](https://github.com/SFML/SFML/pull/371))
  * Fixed OpenGL context leak ([#635](https://github.com/SFML/SFML/pull/635), [#705](https://github.com/SFML/SFML/pull/705))
  * Fixed various joystick problems (memory leak, accelerometer detected, code refactoring) ([#660](https://github.com/SFML/SFML/pull/660), [#686](https://github.com/SFML/SFML/pull/686), [#742](https://github.com/SFML/SFML/pull/742), [#743](https://github.com/SFML/SFML/pull/743))
  * Optimized sf::Window::waitEvent a bit, no sleep if events are available at first try (ff555d6)
  * [Linux] Output error message when XOpenDisplay() fails ([#508](https://github.com/SFML/SFML/pull/508), [#616](https://github.com/SFML/SFML/pull/616))
  * [Linux] Resize window with setSize when sf::Style::Resize is set ([#466](https://github.com/SFML/SFML/pull/466))
  * [Linux] Fixed broken key repeat on window recreation ([#564](https://github.com/SFML/SFML/pull/564), [#567](https://github.com/SFML/SFML/pull/567))
  * [OS X] Fixed KeyReleased not being fired in fullscreen mode ([#465](https://github.com/SFML/SFML/pull/465))
  * [OS X] Fixed an issue where disconnecting the keyboard would cause a crash ([#467](https://github.com/SFML/SFML/pull/467))
  * [OS X] Fixed unexpected resizing behavior ([#468](https://github.com/SFML/SFML/pull/468))
  * [OS X] Improved resizing windows ([#474](https://github.com/SFML/SFML/pull/474))
  * [OS X] Fixed memory leak with sf::Window::create() ([#484](https://github.com/SFML/SFML/pull/484))
  * [OS X] Fixed menu shortcuts in fullscreen on OS X ([#527](https://github.com/SFML/SFML/pull/527))
  * [OS X] Improved cursor hiding ([#703](https://github.com/SFML/SFML/pull/703))
  * [OS X] Fixed right click not detected with trackpads ([#716](https://github.com/SFML/SFML/pull/716), [#730](https://github.com/SFML/SFML/pull/730))
  * [Windows] Fixed joystick POV values (ef1d29b)
  * [Windows] Fixed Unicode inconsistency ([#635](https://github.com/SFML/SFML/pull/635))
  * [Windows] Fixed Alt+F4 and mouse clicks issues ([#437](https://github.com/SFML/SFML/pull/437), [#457](https://github.com/SFML/SFML/pull/457))
  * [Windows] Send MouseButtonReleased event when the mouse is outside of the window ([#455](https://github.com/SFML/SFML/pull/455), [#457](https://github.com/SFML/SFML/pull/457))
  * [Windows] Fixed sf::Joystick wrong registry usage ([#701](https://github.com/SFML/SFML/pull/701), [#702](https://github.com/SFML/SFML/pull/702), [#706](https://github.com/SFML/SFML/pull/706))

### Graphics

**Features**

  * Provide more information about the loaded font in sf::Font ([#164](https://github.com/SFML/SFML/pull/164))
  * Implemented a more flexible blending system ([#298](https://github.com/SFML/SFML/pull/298))
  * Added strikethrough text style ([#243](https://github.com/SFML/SFML/pull/243), [#362](https://github.com/SFML/SFML/pull/362), [#682](https://github.com/SFML/SFML/pull/682))
  * Slight optimization for sf::Text::setString ([#413](https://github.com/SFML/SFML/pull/413))
  * Added subtraction operator for sf::Color ([#114](https://github.com/SFML/SFML/pull/114), [#145](https://github.com/SFML/SFML/pull/145))
  * Optimized sf::Image::flipVertically/flipHorizontally ([#555](https://github.com/SFML/SFML/pull/555))
  * Changed sf::Font measurements from int to float to allow better underline drawing ([#693](https://github.com/SFML/SFML/pull/693))

**Bugfixes**

  * Improved text quality for small and pixelated fonts ([#228](https://github.com/SFML/SFML/pull/228))
  * Yet another fix for Intel GPUs with sf::RenderTexture ([#418](https://github.com/SFML/SFML/pull/418))
  * Removed VTab since it causes issues and doesn't have a use nowadays ([#442](https://github.com/SFML/SFML/pull/442), [#445](https://github.com/SFML/SFML/pull/445), [#460](https://github.com/SFML/SFML/pull/460), [#588](https://github.com/SFML/SFML/pull/588))
  * Fixed broken BDF and PCF font formats ([#448](https://github.com/SFML/SFML/pull/448))
  * Fixed compilation issue with newer versions of GCC for sf::Rect ([#458](https://github.com/SFML/SFML/pull/458))
  * Fixed resetGLStates() not explicitly setting the default polygon mode ([#480](https://github.com/SFML/SFML/pull/480))
  * Fixed division-by-zero in sf::RectangleShape ([#499](https://github.com/SFML/SFML/pull/499))
  * Fixed potential memory leak in sf::Font ([#509](https://github.com/SFML/SFML/pull/509))
  * Updated glext and removed glxext ([#511](https://github.com/SFML/SFML/pull/511), [#583](https://github.com/SFML/SFML/pull/583))
  * Make sure texture unit 0 is active when resetting sf::RenderTarget states ([#523](https://github.com/SFML/SFML/pull/523), [#591](https://github.com/SFML/SFML/pull/591))
  * Fixed texture rect computation in fonts ([#669](https://github.com/SFML/SFML/pull/669))
  * Improved rendering of underlined text ([#593](https://github.com/SFML/SFML/pull/593))
  * Avoided repeated output of error messages ([#566](https://github.com/SFML/SFML/pull/566))
  * Fixed text rendered with vertical offset on ascent and font size mismatch ([#576](https://github.com/SFML/SFML/pull/576))
  * Fixed rounding problem for viewports ([#598](https://github.com/SFML/SFML/pull/598))
  * Fixed sf::Shader::isAvailable() possibly breaking context management ([#211](https://github.com/SFML/SFML/pull/211), [#603](https://github.com/SFML/SFML/pull/603), [#608](https://github.com/SFML/SFML/pull/608), [#603](https://github.com/SFML/SFML/pull/603))
  * Fixed sf::Texture::getMaximumSize() possibly breaking context management ([#666](https://github.com/SFML/SFML/pull/666))
  * Fixed various sf::Text rendering issues ([#692](https://github.com/SFML/SFML/pull/692), [#699](https://github.com/SFML/SFML/pull/699))
  * The texture matrix is now reset in sf::Texture::bind(NULL) (7c4b058)
  * [Windows] Fixed DPI scaling causing strange window behavior ([#679](https://github.com/SFML/SFML/pull/679), [#681](https://github.com/SFML/SFML/pull/681), [#688](https://github.com/SFML/SFML/pull/688))

### Audio

**Features**

  * Added support for selecting the audio capture device ([#220](https://github.com/SFML/SFML/pull/220), [#470](https://github.com/SFML/SFML/pull/470))
  * Make sf::SoundRecorder processing frequency configurable ([#333](https://github.com/SFML/SFML/pull/333))
  * Added up vector to sf::Listener ([#545](https://github.com/SFML/SFML/pull/545))

**Bugfixes**

  * Prevented sf::SoundStream::setPlayingOffset() from restarting playing even when paused ([#203](https://github.com/SFML/SFML/pull/203), [#592](https://github.com/SFML/SFML/pull/592))
  * Fixed sf::SoundBuffer contents not being able to be updated when still attached to sounds ([#354](https://github.com/SFML/SFML/pull/354), 367, [#390](https://github.com/SFML/SFML/pull/390), [#589](https://github.com/SFML/SFML/pull/589))
  * Catch audio format error and prevent division by zero ([#529](https://github.com/SFML/SFML/pull/529))
  * Fixed sf::SoundBuffer returning wrong duration for sounds containing more than ~4.3 million samples (2ff58ed)
  * Optimized sf::Listener with a cache (d97e524)

### Network

**Features**

  * Added support for PUT and DELETE in sf::Http ([#257](https://github.com/SFML/SFML/pull/257), [#312](https://github.com/SFML/SFML/pull/312), [#607](https://github.com/SFML/SFML/pull/607))
  * Added support for chunked HTTP transfers ([#296](https://github.com/SFML/SFML/pull/296), [#337](https://github.com/SFML/SFML/pull/337))
  * Added support for 64-bit integers in sf::Packet ([#710](https://github.com/SFML/SFML/pull/710))
  * Made sf::Ftp::sendCommand() public (2c5cab5)

**Bugfixes**

  * Checked socket descriptor limit ([#153](https://github.com/SFML/SFML/pull/153), [#628](https://github.com/SFML/SFML/pull/628), [#683](https://github.com/SFML/SFML/pull/683))
  * Fixed sf::TcpSocket::connect()'s switching from blocking to non-blocking mode on immediate connection success ([#221](https://github.com/SFML/SFML/pull/221))
  * Fixed FTP download and upload file sizes being limited by available RAM ([#565](https://github.com/SFML/SFML/pull/565), [#590](https://github.com/SFML/SFML/pull/590))
  * Fixed C++11 compiler warnings for sf::Uint8 ([#731](https://github.com/SFML/SFML/pull/731), [#732](https://github.com/SFML/SFML/pull/732))

## SFML 2.1

Also available on the website: https://www.sfml-dev.org/changelog.php#sfml-2.1

### General

  * Updated the Window and OpenGL examples (got rid of GLU and immediate mode)

### Window

**Features**

  * Now using inotify on Linux to avoid constantly polling joystick connections ([#96](https://github.com/SFML/SFML/pull/96))
  * Add keypad return, equal and period keys support for OS X
  * Improved mouse events on OS X regarding fullscreen mode
  * Improved mouse events on OS X ([#213](https://github.com/SFML/SFML/pull/213), [#277](https://github.com/SFML/SFML/pull/277))
  * Improved reactivity of setMousePosition on OS X ([#290](https://github.com/SFML/SFML/pull/290))
  * Added support for right control key on OS X
  * Improved TextEntered for OS X ([#377](https://github.com/SFML/SFML/pull/377))
  * Improved the performances of Window::getSize() (the size is now cached)
  * Added the WM_CLASS property to SFML windows on Linux
  * Fake resize events are no longer sent when the window is moved, on Linux
  * Pressing ALT or F10 on Windows no longer steals the focus

**Bugfixes**

  * Fixed MouseMove event sometimes not generated when holding left button on Windows ([#225](https://github.com/SFML/SFML/pull/225))
  * Fixed ContextSettings ignored when creating a 3.x/4.x OpenGL context on Linux ([#258](https://github.com/SFML/SFML/pull/258))
  * Fixed ContextSettings ignored on Linux when creating a window ([#35](https://github.com/SFML/SFML/pull/35))
  * Fixed windows bigger than the desktop not appearing on Windows ([#215](https://github.com/SFML/SFML/pull/215))
  * Fixed KeyRelease events sometimes not reported on Linux ([#404](https://github.com/SFML/SFML/pull/404))
  * Fixed mouse moved event on OS X when dragging the cursor ([#277](https://github.com/SFML/SFML/pull/277))
  * Fixed KeyRelease event with CMD key pressed ([#381](https://github.com/SFML/SFML/pull/381))
  * Fixed taskbar bugs on Windows ([#328](https://github.com/SFML/SFML/pull/328), [#69](https://github.com/SFML/SFML/pull/69))
  * Fixed Window::getPosition() on Linux ([#346](https://github.com/SFML/SFML/pull/346))
  * Unicode characters outside the BMP (> 0xFFFF) are now correctly handled on Windows ([#366](https://github.com/SFML/SFML/pull/366))

### Graphics

**Features**

  * Checking errors in RenderTarget::pushGLStates() to avoid generating false error messages when user leaves unchecked OpenGL errors ([#340](https://github.com/SFML/SFML/pull/340))
  * Optimized Shader::setParameter functions, by using a cache internally ([#316](https://github.com/SFML/SFML/pull/316), [#358](https://github.com/SFML/SFML/pull/358))

**Bugfixes**

  * Fixed bounding rect of sf::Text ignoring whitespaces ([#216](https://github.com/SFML/SFML/pull/216))
  * Solved graphics resources not updated or corrupted when loaded in a thread ([#411](https://github.com/SFML/SFML/pull/411))
  * Fixed white pixel showing on first character of sf::Text ([#414](https://github.com/SFML/SFML/pull/414))
  * sf::Rect::contains and sf::Rect::intersects now handle rectangles with negative dimensions correctly ([#219](https://github.com/SFML/SFML/pull/219))
  * Fixed Shape::setTextureRect not working when called before setTexture

### Audio

**Features**

  * loadFromStream functions now explicitly reset the stream (seek(0)) before starting to read ([#349](https://github.com/SFML/SFML/pull/349))

**Bugfixes**

  * Added a workaround for a bug in the OS X implementation of OpenAL (unsupported channel count no properly detected) ([#201](https://github.com/SFML/SFML/pull/201))
  * Fixed SoundBuffer::loadFromStream reading past the end of the stream ([#214](https://github.com/SFML/SFML/pull/214))

### Network

**Features**

  * Replaced the deprecated gethostbyname with getaddrinfo ([#47](https://github.com/SFML/SFML/pull/47))
  * Minor improvements to sf::Packet operators (now using strlen and wcslen instead of explicit loops) ([#118](https://github.com/SFML/SFML/pull/118))

**Bugfixes**

  * Fixed non-blocking connection with a sf::TcpSocket on Windows
  * Fixed TCP packet data corruption in non-blocking mode ([#402](https://github.com/SFML/SFML/pull/402), [#119](https://github.com/SFML/SFML/pull/119))
  * On Unix systems, a socket disconnection no longer stops the program with signal SIGPIPE ([#72](https://github.com/SFML/SFML/pull/72))

## SFML 2.0

Also available on the website: https://www.sfml-dev.org/changelog.php#sfml-2.0

No changelog available. *Everything changed.*

## Older Releases

See the website for changelogs of older releases: https://www.sfml-dev.org/changelog.php
