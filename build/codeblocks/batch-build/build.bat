@echo off

REM Retrieve the SFML path
set SFML="%CD%\..\..\.."
set TMPDIR="%CD%\temp"

REM Add the Code::Blocks path to the PATH
set PATH=%PATH%;"%CD%"

REM Compile the SFML libraries
cd %SFML%\build\codeblocks

echo Compiling sfml-main...
codeblocks -na -nd -ns --build sfml-main.cbp --target="Debug_Win32"
codeblocks -na -nd -ns --build sfml-main.cbp --target="Release_Win32"

echo Compiling debug static libraries...
codeblocks -na -nd -ns --build sfml-system.cbp --target="Debug static_Win32"
codeblocks -na -nd -ns --build sfml-window.cbp --target="Debug static_Win32"
codeblocks -na -nd -ns --build sfml-graphics.cbp --target="Debug static_Win32"
codeblocks -na -nd -ns --build sfml-audio.cbp --target="Debug static_Win32"
codeblocks -na -nd -ns --build sfml-network.cbp --target="Debug static_Win32"

echo Compiling debug dynamic libraries...
codeblocks -na -nd -ns --build sfml-system.cbp --target="Debug DLL_Win32"
codeblocks -na -nd -ns --build sfml-window.cbp --target="Debug DLL_Win32"
codeblocks -na -nd -ns --build sfml-graphics.cbp --target="Debug DLL_Win32"
codeblocks -na -nd -ns --build sfml-audio.cbp --target="Debug DLL_Win32"
codeblocks -na -nd -ns --build sfml-network.cbp --target="Debug DLL_Win32"

echo Compiling release static libraries...
codeblocks -na -nd -ns --build sfml-system.cbp --target="Release static_Win32"
codeblocks -na -nd -ns --build sfml-window.cbp --target="Release static_Win32"
codeblocks -na -nd -ns --build sfml-graphics.cbp --target="Release static_Win32"
codeblocks -na -nd -ns --build sfml-audio.cbp --target="Release static_Win32"
codeblocks -na -nd -ns --build sfml-network.cbp --target="Release static_Win32"

echo Compiling release dynamic libraries...
codeblocks -na -nd -ns --build sfml-system.cbp --target="Release DLL_Win32"
codeblocks -na -nd -ns --build sfml-window.cbp --target="Release DLL_Win32"
codeblocks -na -nd -ns --build sfml-graphics.cbp --target="Release DLL_Win32"
codeblocks -na -nd -ns --build sfml-network.cbp --target="Release DLL_Win32"
codeblocks -na -nd -ns --build sfml-audio.cbp --target="Release DLL_Win32"

REM Copy the SFML static libraries into a temporary folder
IF NOT EXIST %TMPDIR% mkdir %TMPDIR%
cd %SFML%\lib\mingw
xcopy libsfml2-*-s-d.a %TMPDIR%/q /y
xcopy libsfml2-*-s.a %TMPDIR%/q /y
cd %TMPDIR%

REM Add external libraries to sfml-window
echo Adding external libraries to libsfml2-window-s...
ar x %SFML%\build\codeblocks\batch-build\libgdi32.a
ar x %SFML%\build\codeblocks\batch-build\libopengl32.a
ar x %SFML%\build\codeblocks\batch-build\libwinmm.a
ar rs libsfml2-window-s.a *.o
ar rs libsfml2-window-s-d.a *.o
del *.o /f /q

REM Add external libraries to sfml-graphics
echo Adding external libraries to libsfml2-graphics-s...
ar x %SFML%\extlibs\libs-mingw\libfreetype.a
ar x %SFML%\extlibs\libs-mingw\libglew.a
ar x %SFML%\extlibs\libs-mingw\libjpeg.a
ar rs libsfml2-graphics-s.a *.o
ar rs libsfml2-graphics-s-d.a *.o
del *.o /f /q

REM Add external libraries to sfml-audio
echo Adding external libraries to libsfml2-audio-s...
ar x %SFML%\extlibs\libs-mingw\libopenal32.a
ar x %SFML%\extlibs\libs-mingw\libsndfile.a
ar rs libsfml2-audio-s.a *.o
ar rs libsfml2-audio-s-d.a *.o
del *.o /f /q

REM Add external libraries to sfml-network
echo Adding external libraries to libsfml2-network-s...
ar x %SFML%\build\codeblocks\batch-build\libws2_32.a
ar rs libsfml2-network-s.a *.o
ar rs libsfml2-network-s-d.a *.o
del *.o /f /q

REM Copy the SFML libraries to their final location
echo Cleaning up...
xcopy *.a %SFML%\lib\mingw /q /y
del *.a /f /q

REM Remove the temporary directory we've just created
cd ..
rmdir %TMPDIR% /s /q

echo Done
