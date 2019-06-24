# Changelog

## SFML 2.5.1

Also available on the website: https://www.sfml-dev.org/changelog.php#sfml-2.5.1

### General

  * Various CMake fixes (#1414, #1416, #1436, #1439, #1467, #1470)
  * Fixed the installation of pkg-config files (#1466)
  * Fixed two conversion warnings (#1454)
  * [Android] Fixes all symbols in sfml-main are hidden (#1457, #1460)
  * [Android] Fixed some `#define` flag problem (#1458)
  * [Android] Fix deadlock in main cleanup (#1265)
  * [iOS] Modernized toolchain file (#1411)
  * [iOS] Check that `<SFML/Main.hpp>` is used (#1412)
  * [macOS] Add `-ObjC` flag to fix static linking on macOS (#1485)

### Window

**Bugfixes**

  * [iOS] Use default supported rotations when none are specified (#1417)
  * [iOS] Fixed autocomplete window overlaps keyboard (#1473, #1482)
  * [Linux] Fixed dual monitor issue (#1226, #1238)
  * [Linux] Fixed issue where fullscreen window didn't go over task bars on top and left on in Ubuntu (#1224)
  * [Linux] Fixed the Unix clipboard implementation causing an abort due to internal data races in Xlib (#1437)
  * [macOS] Added additional system cursors (#1401, #1413, #1425)
  * [Windows] Fixed swapped colors for custom cursors (#1464, #1465, #1491)

### Graphics

**Bugfixes**

  * Fixed a bug in which a `sf::RenderTexture` would not be re-activated after being re-created (#1438)
  * Fixed `sf::RenderTextureImplFBO`'s destructor incorrectly triggering deletion of other `sf::RenderTextureImplFBO`'s active FBOs (#1440)
  * Fix `sf::RenderWindow::setActive` incorrectly trying to unbind an FBO during deactivation (#1442)
  * Fixed `sf::RenderTexture::display()` dereferencing a NULL pointer when being called before `sf::RenderTexture::create()` (#1446)
  * Fixed bug in `sf::Text` when applying an outline color/thickness (#1176)
  * Squash duplicated `sf::Font` glyphs to single chars (#1461)
  * Fixed two issues with glyph sub-pixel positioning (#1452)
  * Reduced context locking & unlocking while creating textures (#1459)
  * Fixed the error message when the wrong bitmap font size is selected (#1456, #1474, #1492)

### Audio

**Bugfixes**

  * Fixed performance issue with reading WAV files (#1450)

## SFML 2.5.0

Also available on the website: https://www.sfml-dev.org/changelog.php#sfml-2.5.0

### General

  * Replaced FindSFML.cmake with SFMLConfig.cmake (#1335)
  * Markdown'd and updated readme, changelog, contributing and license files (#1196, #1368, #1317)
  * Improve packaging support (#1173)
  * Added Tagfile generation and search (#1327)
  * Added CMake variables to select the modules to be built (#798, #800)
  * Do not install extlibs if `SFML_USE_SYSTEM_DEPS` is true (#1236, #1237)
  * Fixed various type conversion/comparison warnings (#1325)
  * [Android] Increased minimum API version to 14 (#1362)
  * [Android] Removed custom toolchain and added support for the newest NDK version and Gradle (#1350, #1393)
  * [iOS] Updated the binary libs from exlibs/libs-ios (#1207, #1209)
  * [iOS] Use a CMake toolchain file for iOS build (#1268, #1269)
  * [iOS] Install extlibs if needed (#1348)
  * [iOS] Drop 32 bit support (#1374)
  * [iOS] Force correct iOS architecture for cmake (#1373, #1377)
  * [iOS] Added iOS example (#1378)
  * [macOS] Fixed launch of cocoa examples (#1334)
  * [macOS] Improved application signing process (#1020, #1036, #1194)
  * [macOS] Improved CMake script (#1215, #1371)
  * [macOS] Use `-stdlib=libc++` (#1361)
  * [OpenBSD] Added support for OpenBSD (#1330)

### System

**Bugfixes**

  * Added protected destructor to `sf::NonCopyable` to prevent possible resource leaks (#1125, #1161)
  * Fixed crash when `sf::Clock` is constructed in a global scope (#1258)

### Window

**Features**

  * Implemented Cursor API (#269, #784, #827)
  * Implemented Clipboard API (#715, #1204, #1221)
  * Renamed a few key codes (#1395)
  * Added joystick example (#1363)
  * [Windows] Added support for interfacing with joysticks via DirectInput when it is available (#1251, #1326)
  * [Windows] Fix discrete GPU preference symbols being exported from the wrong place (#1192, #1406)

**Bugfixes**

  * [Android] Return correct key code for delete/backspace (#1309, #1362)
  * [iOS] Don't need to find vorbisfile or vorbisenc (#1347)
  * [Linux] Fixed `sf::Window::getPosition()` returning incorrect position because of differences in window managers (#1228, #1266)
  * [Linux] Fix X11 key repeat handling not filtering out events from other windows (#1223, #1230, #1291)
  * [Linux] Restore fullscreen of a non-visible window (#1339)
  * [macOS] Fixed window menu not working (#1091, #1180, #1193)
  * [macOS] Fixed crash with application messing hardware detection e.g. TeamViewer (#1323)
  * [macOS] Added support for (some) Hat/POV axis (#1248)
  * [Windows] Prevent uninitialized read by zeroing memory (#1264)
  * [Windows] Fixed modifier keys handling (#1357)

### Graphics

**Features**

  * Implemented additional line spacing and letter spacing in `sf::Text` (#928, #1366)
  * Added `sf::VertexBuffer` class (#1308)
  * Added GPU local texture copying support, allowing performance optimizations and texture swapping (#1119, #1319, #1320)
  * Optimize performance by skipping `glTexCoordPointer()` call if not needed (#1015)
  * Generate shape outline vertices only if necessary (#925, #1356)
  * Removed dependency to libjpeg, stb_image_write now supports writing JPEG files (#1278, #1279)
  * Enable comparing `sf::Transform` and optimize resetting OpenGL back to the identity matrix (#1298)
  * Added missing `setActive()` virtual method to `sf::RenderTarget` (#1157)
  * Updated stb_image to v2.16 and stb_image_write to v1.07 (#1270)
  * Added `sf::RenderTexture` stencil and multisampling support (#1274, #1285)
  * Added example demonstrating `sf::VertexBuffer`, `sf::Shader` and `sf::Thread` usage (#1352)
  * Optimized `sf::RenderTexture` performance (#1379)

**Bugfixes**

  * Properly free memory in `sf::Font::cleanup()` (#1119)
  * Fixed memory leak in `sf::Font` (#1216)
  * Fix OpenGL texture coordinate pointer not being updated correctly under certain conditions (#1297)
  * Fix for broken text when the font is reloaded (#1345)
  * Fix memory leak in `sf::Text` (#1233, #1360)
  * Fixed strict aliasing punning warning when generating the key of a glyph in Font.cpp (#1187, #1396)
  * Fixed OpenGL version string being parsed incorrectly on some platforms (#1249, #1390)
  * [macOS] Worked around render target bug (#1132, #1342)
  * [Windows] Replaced time-based joystick poll with a hardware event handler (#1179, #1195, #1198, #1199, #1421)

### Audio

**Features**

  * Added loop point support to `sf::Music` (#177, #629)
  * Added support for the extensible PCM wave file format (#1296)
  * [iOS] Enable audio module (#1338)

**Bugfixes**

  * Fixed inconsistent seek behavior in `sf::SoundStream` (#1118)
  * Fixed stack overflow in `sf::SoundStream::fillAndPushBuffer()` (#1154)
  * Fixed seeking quirks in the FLAC reader (#966, #1162)
  * Allow polymorphism with `sf::SoundSource` (#1185)
  * Fixed WAV file writer writing wrong header values (#1280, #1281)
  * Small bugfix to argument of `alcCaptureOpenDevice()` (#1304, #1305)
  * [iOS] Find OpenAL correctly (#1263, #1376)
  * [Windows] Updated OpenAL Soft to 1.18.1 fixing crashes (#1247, #1260)

### Network

**Features**

  * Add append/overwrite parameter to Ftp::upload (#1072, #1399)

**Bugfixes**

  * Fixed wrong condition for building network support (#1253)
  * Changed TCP listen backlog from 0 to SOMAXCONN (#1369, #1407)
  * Fixed socket reuse not conforming to documentation (#1346, #1408)

## SFML 2.4.2

Also available on the website: https://www.sfml-dev.org/changelog.php#sfml-2.4.2

### System

**Bugfixes**

  * [Windows] Removed thread affinity changes in sf::Clock (#1107)

### Window

**Bugfixes**

  * Fixed bug where TransientContextLock would hang (#1165, #1172)
  * [Linux] Fixed GLX extensions being loaded too late (#1183)
  * [Linux] Fix wrong types passed to XChangeProperty (#1168, #1171)
  * [Windows] Make context disabling via wglMakeCurrent more tolerant of broken drivers (#1186)

### Graphics

**Bugfixes**

  * Optimized sf::Image::create and made it more exception safe (#1166)

## SFML 2.4.1

Also available on the website: https://www.sfml-dev.org/changelog.php#sfml-2.4.1

### General

  * [kFreeBSD] Define SFML_OS_FREEBSD when compiling for kFreeBSD (#1129)
  * [Windows] Added some simple messaging when trying to build under Cygwin (#1153)

### Window

**Bugfixes**

  * Fixed stack overflow on GlContext creation with multiple threads (#989, #1002)
  * Adjusted mouse cursor grab documentation (#1133)
  * [iOS] Fixed orientation change not rescaling window size properly (#1049, #1050)
  * [Linux] Fixed fullscreen issue (#921, #1138)
  * [Linux] Switched from XCB back to Xlib for windowing (#1138)
  * [Linux] Fixed window icon not showing up on some distros (#1087, #1088)
  * [Linux] Fixed an issue where GNOME flags window unresponsive (#1089, #1138)
  * [Linux] Fixed leak of XVisualInfo objects during GlxContext creation (#1135)
  * [Linux] Fixed possible hang when setting visibility if external window sources (#1136)
  * [macOS] Fixed inconsistency between doc and impl on macOS for the grab feature (#1133, #1148, #1150)
  * [Windows] Fixed context memory leaks (#1143, #1002)

### Graphics

**Bugfixes**

  * Adjusted uniform error message (#1131)
  * Clarify documentation on Rect::contains function bounds (#1151)

### Network

**Bugfixes**

  * Fixed a typo in comment for void unbind() (#1121)

## SFML 2.4.0

Also available on the website: https://www.sfml-dev.org/changelog.php#sfml-2.4.0

### General

  * Added deprecation macro (#969)
  * Fixed issues reported by Coverity Scan static analysis (#1064)
  * Fixed some initialization issues reported by Cppcheck (#1008)
  * Changed comment chars in FindSFML.cmake to # (#1090)
  * Fixed some typos (#1098, #993, #1099, #956, #963, #979)
  * Updated/fixed string comparisons in Config.cmake (#1102)
  * Added the missing -s postfix for the RelWithDebInfo config (#1014)
  * [Android] Fixed current Android compilation issues (#1116, #1111, #1079)
  * [macOS] Update Xcode template material (#976, #968)
  * [Windows] Added support for VS 2015 (#972)
  * [Windows] Create and install PDB debug symbols alongside binaries (#1037)

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

  * [Android] Added sf::getNativeActivity() (#1005, #680)

**Bugfixes**

  * Added missing <iterator> include in String.hpp (#1069, #1068)
  * Fixed encoding of UTF-16 (#997)
  * [Android] Fixed crash when trying to load a non-existing font file (#1058)

### Window

**Features**

  * Added ability to grab cursor (#614, #394, #1107)
  * Added Multi-GPU preference (#869, #867)
  * Added support for sRGB capable framebuffers (#981, #175)
  * [Linux, Windows] Improved OpenGL context creation (#884)
  * [Linux, Windows] Added support for pbuffers on Windows and Unix (#885, #434)

**Bugfixes**

  * Updated platform-specific handle documentation (#961)
  * [Android] Accept touch events from "multiple" devices (#954, #953)
  * [Android] Copy the selected EGL context's settings to SFML (#1039)
  * [Linux] Fixed modifiers causing sf::Keyboard::Unknown being returned (#1022, #1012)
  * [macOS] Improved memory management on macOS (#962, #790)
  * [macOS] Fixed crash when resizing a window to a zero-height/width size (#986, #984)
  * [macOS] Use the mouse button constant instead of 0 to avoid a compiler error on macOS (#1035)
  * [macOS] macOS improvement: warnings + bugfix + refactoring, the lot! (#1042)

### Graphics

**Features**

  * Added support for outlined text (#840)
  * Add support for geometry shaders (#886, #428)
  * Feature/blend mode reverse subtract (#945, #944)
  * Implemented support for mipmap generation (#973, #498, #123)
  * Added new API to set shader uniforms (#983, #538)
  * Rewrite RenderWindow::capture (#1001)

**Bugfixes**

  * Exporting some Glsl utility functions due to linking issues (#1044, #1046)
  * Fixed missing initialisation of Font::m_stroker (#1059)
  * Changed primitive types to be grammatically correct (#1095, #939)

### Audio

**Features**

  * Implemented stereo audio recording (#1010)

**Bugfixes**

  * Added an assignment operator to SoundSource (#864)
  * [macOS] Updates OpenAL-soft for macOS to version 1.17.2 (#1057, #900, #1000)
  * Fixed a bug where vorbis can't handle large buffers (#1067)
  * Added support for 24-bit .wav files (#958, #955)
  * Fixed threading issue in sf::SoundRecorder (#1011)
  * Made WAV file reader no longer assume that data chunk goes till end of file to prevent reading trailing metadata as samples (#1018)
  * Fixed seeking in multi channel FLAC files (#1041, #1040)

### Network

**Features**

  * Added optional argument on which address to bind (socket). (#850, #678)

**Bugfixes**

  * Fixed FTP directory listing blocking forever (#1086, #1025)

## SFML 2.3.2

Also available on the website: https://www.sfml-dev.org/changelog.php#sfml-2.3.2

### General

  * Fixed an issue where FindSFML.cmake couldn't find older versions of SFML (#903)
  * Robust alCheck and glCheck macros (#917)
  * Fixed FindSFML.cmake to use the uppercase FLAC name (#923)
  * Added a CONTRIBUTING file so GitHub shows a message when creating a new issue (#932)

### Window

**Bugfixes**

  * [Linux] Fixed an issue where the keypad's key weren't being detected (#910)
  * [Linux] Revert to Xlib event handling (#934)
  * [Linux] Fixed `XK_*` inconsistency in InpuImpl.cpp (#947)
  * [Linux] Fix `_NET_WM_PING` messages not being replied to properly (#947)

### Graphics

**Bugfixes**

  * Fixed clear bug on RenderTextures (#915)
  * Fixed image file extension detection (#929, #930, #931)
  * Secure function against random data return (#935, #942)

## SFML 2.3.1

Also available on the website: https://www.sfml-dev.org/changelog.php#sfml-2.3.1

### Window

**Bugfixes**

  * [Android] Make sure a window still exists before trying to access its dimensions (#854)
  * [Android] Added Android API level checks (#856)
  * [Android] Updated the JNI/event handling code (#906)
  * [Linux] Resized events are only spawned when the window size actually changes (#878, #893)
  * [Linux] Whitelisted X SHAPE events (#879, #883)
  * [Linux] Remap Unix keyboard when user changes layout (#895, #897)
  * [Linux] Fix undefined behavior in ewmhSupported() (#892, #901)

### Graphics

**Bugfixes**

  * Added support for GL_EXT_texture_edge_clamp for systems that don't expose GL_SGIS_texture_edge_clamp (#880, #882)

### Audio

**Bugfixes**

  * [Android] Fixed audio files not loading (and possibly crashing) (#855, #887)

## SFML 2.3

Also available on the website: https://www.sfml-dev.org/changelog.php#sfml-2.3

### General

  * Examples only link against sfml-main in release mode (#610, #766)
  * Replaced unsigned int with std::size_t for array indices and sizes (#739)
  * Fixed some issues with the Doxygen documentation (#750)
  * Added support for EditorConfig (#751)
  * Hide success message for CMake in quiet mode (#753)
  * Improved documentation for statuses with sf::Ftp (#763)
  * Moved stb_image into the extlibs directory (#795)
  * Changed the SOVERSION to major.minor (#812)
  * Fixed warnings about switch-statements (#863)
  * Added missing includes in the general headers (#851)
  * [Android] Updated toolchain file and dependencies (#791)
  * [Linux] Fixed missing pthread dependency (#794)
  * [macOS] Relaxed CMake installation rules regarding framework dependencies (#767)

### Deprecated API

  * sf::Event::MouseWheelEvent: This event is deprecated and potentially inaccurate. Use MouseWheelScrollEvent instead.

### Window

**Features**

  * Added new events for handling high-precision scrolling (#95, #810, #837)
  * Switched from Xlib to XCB (#200, #319, #694, #780, #813, #825)
  * Added support for OpenGL 3 core context creation (#654, #779)

**Bugfixes**

  * Fixed glXSwapIntervalSGI being broken for some driver implementations (#727, #779)
  * Fixed simultaneous context operations causing crashes on some AMD hardware (#778, #779)
  * Fixed joystick identification (#809, #811)
  * [iOS] Fixed various issues including stencil bits, device orientation and retina support (#748)
  * [iOS] Fixed inconsistency between sf::Touch::getPosition and touch events (#875)
  * [Linux] Fixed Alt+F4 not getting triggered in window mode (#274)
  * [Linux] Fixed Unix joystick stuff (#838)
  * [macOS] Fixed typo in JoystickImpl.cpp to prevent a crash (#762, #765)
  * [macOS] Fixed an issue in InputImpl::getSFOpenGLViewFromSFMLWindow (#782, #792)

### Graphics

**Features**

  * Replaced GLEW with loader generated by glLoadGen (#779)
  * Added a new constructor to sf::Color that takes an sf::Uint32 (#722)
  * Updated stb_image to v2.02 (#777)
  * Updated FreeType to v2.5.5 (#799, #804)
  * Added checks for software OpenGL (#870)

**Bugfixes**

  * Fixed GL_ARB_compatibility not being detected (#859)
  * Fixed pixel format selection (#862)
  * Bumped back the OpenGL version requirement to 1.1 (#858)

### Audio

**Features**

  * Dropped libsndfile and started using Vorbis, FLAC and OGG directly (#604, #757)
  * Added a FLAC file to the sound example (#815)

**Bugfixes**

  * Fixed access violation error in the destructor of sf::AudioDevice (#30, #602)
  * [macOS] Fixed threading issue with sf::SoundStream and OpenAL (#541, #831)

### Network

**Bugfixes**

  * Fixed sf::TcpSocket not handling partial sends properly (#749, #796)

## SFML 2.2

Also available on the website: https://www.sfml-dev.org/changelog.php#sfml-2.2

### General

  * Support for iOS and Android platform (#410, #440)
  * Various documentation corrections (#438, #496, #497, #714)
  * Fixed support for compilers on Debian FreeBSD (#380, #578)
  * Added support for Visual Studio 2013 and proper support for the TDM builds (#482)
  * Fixed CMake problems related to FindSFML and cached variables (#637, #684)
  * Switched and enforced LF line endings (#708, #712)
  * Updated OpenAL to version 1.15.1 (d077210)
  * Made compiler and OS variable names much clearer in CMake files (9b0ed30)
  * Re-enabled RPATH feature (e157e7a)
  * Slight adjustments to the examples (#737)
  * [FreeBSD] Various configuration fixes (#577, #578)
  * [Linux] Updated FindSFML.cmake to add UDev to SFML's dependencies (#728, #729, #734, #736)
  * [macOS] Fixed incorrect symlink in freetype.framework (#519)
  * [macOS] CMake module for correct dependencies (#548)
  * [macOS] Fixed SFML target for Xcode (#595, #596)
  * [macOS] Updated implementation, mainly reverting to non-ARC (#601)
  * [macOS] Fixed memory leaks and dead store (#615)
  * [macOS] Improved event handling and performance (#617)
  * [macOS] Reduced memory usage (#672, #698)
  * [macOS] macOS 10.10 support (#691, #699)
  * [macOS] Improve flexibility of dependencies' locations (#713)
  * [Windows] Removed the hack that copied external libraries into SFML static libraries (dbf01a7)

### System

**Features**

  * Added substring and replace functions to sf::String (#21, #355)
  * Added toUtfX to sf::String (#501)
  * Added fromUtfX functions to set the internal data to a string by converting from another string in a fixed encoding (#196)
  * Added modulo operator for sf::Time (#429, #430)
  * Added division operator for sf::Time (#453)

**Bugfixes**

  * Ensured a high resolution for sf::sleep (#439, #475)
  * [Windows] Fixed stack unalignment by two internal functions (#412)

### Window

**Features**

  * Added window methods to request and to check focus (#518, #525, #613, #723, #735)
  * Provide name, manufacturer ID and product ID via sf::Joystick (#152, #528)
  * [FreeBSD] Joystick support (#477)
  * [macOS] Improved integration with menus and dock actions (#11)
  * [macOS] Support for OpenGL 3.2 (#84)
  * [macOS] Improved fullscreen support (#343)
  * [macOS] Added support for retina displays (#353, #388)
  * [Windows] Removed support for Windows 9x (#469)
  * [Windows] Fixed typo in Windows keyboard implementation (#516)

**Bugfixes**

  * sf::Window::create() now also resets framerate limit (#371)
  * Fixed OpenGL context leak (#635, #705)
  * Fixed various joystick problems (memory leak, accelerometer detected, code refactoring) (#660, #686, #742, #743)
  * Optimized sf::Window::waitEvent a bit, no sleep if events are available at first try (ff555d6)
  * [Linux] Output error message when XOpenDisplay() fails (#508, #616)
  * [Linux] Resize window with setSize when sf::Style::Resize is set (#466)
  * [Linux] Fixed broken key repeat on window recreation (#564, #567)
  * [macOS] Fixed KeyReleased not being fired in fullscreen mode (#465)
  * [macOS] Fixed an issue where disconnecting the keyboard would cause a crash (#467)
  * [macOS] Fixed unexpected resizing behavior (#468)
  * [macOS] Improved resizing windows (#474)
  * [macOS] Fixed memory leak with sf::Window::create() (#484)
  * [macOS] Fixed menu shortcuts in fullscreen on macOS (#527)
  * [macOS] Improved cursor hiding (#703)
  * [macOS] Fixed right click not detected with trackpads (#716, #730)
  * [Windows] Fixed joystick POV values (ef1d29b)
  * [Windows] Fixed Unicode inconsistency (#635)
  * [Windows] Fixed Alt+F4 and mouse clicks issues (#437, #457)
  * [Windows] Send MouseButtonReleased event when the mouse is outside of the window (#455, #457)
  * [Windows] Fixed sf::Joystick wrong registry usage (#701, #702, #706)

### Graphics

**Features**

  * Provide more information about the loaded font in sf::Font (#164)
  * Implemented a more flexible blending system (#298)
  * Added strikethrough text style (#243, #362, #682)
  * Slight optimization for sf::Text::setString (#413)
  * Added subtraction operator for sf::Color (#114, #145)
  * Optimized sf::Image::flipVertically/flipHorizontally (#555)
  * Changed sf::Font measurements from int to float to allow better underline drawing (#693)

**Bugfixes**

  * Improved text quality for small and pixelated fonts (#228)
  * Yet another fix for Intel GPUs with sf::RenderTexture (#418)
  * Removed VTab since it causes issues and doesn't have a use nowadays (#442, #445, #460, #588)
  * Fixed broken BDF and PCF font formats (#448)
  * Fixed compilation issue with newer versions of GCC for sf::Rect (#458)
  * Fixed resetGLStates() not explicitly setting the default polygon mode (#480)
  * Fixed division-by-zero in sf::RectangleShape (#499)
  * Fixed potential memory leak in sf::Font (#509)
  * Updated glext and removed glxext (#511, #583)
  * Make sure texture unit 0 is active when resetting sf::RenderTarget states (#523, #591)
  * Fixed texture rect computation in fonts (#669)
  * Improved rendering of underlined text (#593)
  * Avoided repeated output of error messages (#566)
  * Fixed text rendered with vertical offset on ascent and font size mismatch (#576)
  * Fixed rounding problem for viewports (#598)
  * Fixed sf::Shader::isAvailable() possibly breaking context management (#211, #603, #608, #603)
  * Fixed sf::Texture::getMaximumSize() possibly breaking context management (#666)
  * Fixed various sf::Text rendering issues (#692, #699)
  * The texture matrix is now reset in sf::Texture::bind(NULL) (7c4b058)
  * [Windows] Fixed DPI scaling causing strange window behavior (#679, #681, #688)

### Audio

**Features**

  * Added support for selecting the audio capture device (#220, #470)
  * Make sf::SoundRecorder processing frequency configurable (#333)
  * Added up vector to sf::Listener (#545)

**Bugfixes**

  * Prevented sf::SoundStream::setPlayingOffset() from restarting playing even when paused (#203, #592)
  * Fixed sf::SoundBuffer contents not being able to be updated when still attached to sounds (#354, 367, #390, #589)
  * Catch audio format error and prevent division by zero (#529)
  * Fixed sf::SoundBuffer returning wrong duration for sounds containing more than ~4.3 million samples (2ff58ed)
  * Optimized sf::Listener with a cache (d97e524)

### Network

**Features**

  * Added support for PUT and DELETE in sf::Http (#257, #312, #607)
  * Added support for chunked HTTP transfers (#296, #337)
  * Added support for 64-bit integers in sf::Packet (#710)
  * Made sf::Ftp::sendCommand() public (2c5cab5)

**Bugfixes**

  * Checked socket descriptor limit (#153, #628, #683)
  * Fixed sf::TcpSocket::connect()'s switching from blocking to non-blocking mode on immediate connection success (#221)
  * Fixed FTP download and upload file sizes being limited by available RAM (#565, #590)
  * Fixed C++11 compiler warnings for sf::Uint8 (#731, #732)

## SFML 2.1

Also available on the website: https://www.sfml-dev.org/changelog.php#sfml-2.1

### General

  * Updated the Window and OpenGL examples (got rid of GLU and immediate mode)

### Window

**Features**

  * Now using inotify on Linux to avoid constantly polling joystick connections (#96)
  * Add keypad return, equal and period keys support for macOS
  * Improved mouse events on macOS regarding fullscreen mode
  * Improved mouse events on macOS (#213, #277)
  * Improved reactivity of setMousePosition on macOS (#290)
  * Added support for right control key on macOS
  * Improved TextEntered for macOS (#377)
  * Improved the performances of Window::getSize() (the size is now cached)
  * Added the WM_CLASS property to SFML windows on Linux
  * Fake resize events are no longer sent when the window is moved, on Linux
  * Pressing ALT or F10 on Windows no longer steals the focus

**Bugfixes**

  * Fixed MouseMove event sometimes not generated when holding left button on Windows (#225)
  * Fixed ContextSettings ignored when creating a 3.x/4.x OpenGL context on Linux (#258)
  * Fixed ContextSettings ignored on Linux when creating a window (#35)
  * Fixed windows bigger than the desktop not appearing on Windows (#215)
  * Fixed KeyRelease events sometimes not reported on Linux (#404)
  * Fixed mouse moved event on macOS when dragging the cursor (#277)
  * Fixed KeyRelease event with CMD key pressed (#381)
  * Fixed taskbar bugs on Windows (#328, #69)
  * Fixed Window::getPosition() on Linux (#346)
  * Unicode characters outside the BMP (> 0xFFFF) are now correctly handled on Windows (#366)

### Graphics

**Features**

  * Checking errors in RenderTarget::pushGLStates() to avoid generating false error messages when user leaves unchecked OpenGL errors (#340)
  * Optimized Shader::setParameter functions, by using a cache internally (#316, #358)

**Bugfixes**

  * Fixed bounding rect of sf::Text ignoring whitespaces (#216)
  * Solved graphics resources not updated or corrupted when loaded in a thread (#411)
  * Fixed white pixel showing on first character of sf::Text (#414)
  * sf::Rect::contains and sf::Rect::intersects now handle rectangles with negative dimensions correctly (#219)
  * Fixed Shape::setTextureRect not working when called before setTexture

### Audio

**Features**

  * loadFromStream functions now explicitly reset the stream (seek(0)) before starting to read (#349)

**Bugfixes**

  * Added a workaround for a bug in the macOS implementation of OpenAL (unsupported channel count no properly detected) (#201)
  * Fixed SoundBuffer::loadFromStream reading past the end of the stream (#214)

### Network

**Features**

  * Replaced the deprecated gethostbyname with getaddrinfo (#47)
  * Minor improvements to sf::Packet operators (now using strlen and wcslen instead of explicit loops) (#118)

**Bugfixes**

  * Fixed non-blocking connection with a sf::TcpSocket on Windows
  * Fixed TCP packet data corruption in non-blocking mode (#402, #119)
  * On Unix systems, a socket disconnection no longer stops the program with signal SIGPIPE (#72)

## SFML 2.0

Also available on the website: https://www.sfml-dev.org/changelog.php#sfml-2.0

No changelog available. *Everything changed.*

## Older Releases

See the website for changelogs of older releases: https://www.sfml-dev.org/changelog.php
