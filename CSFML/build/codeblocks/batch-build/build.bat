@echo off

cd ..

echo Compiling csfml-main...
codeblocks -na -nd -ns --build csfml-main.cbp --target="Debug_Win32"
codeblocks -na -nd -ns --build csfml-main.cbp --target="Release_Win32"

echo Compiling debug dynamic libraries...
codeblocks -na -nd -ns --build csfml-system.cbp --target="Debug DLL_Win32"
codeblocks -na -nd -ns --build csfml-window.cbp --target="Debug DLL_Win32"
codeblocks -na -nd -ns --build csfml-graphics.cbp --target="Debug DLL_Win32"
codeblocks -na -nd -ns --build csfml-audio.cbp --target="Debug DLL_Win32"
codeblocks -na -nd -ns --build csfml-network.cbp --target="Debug DLL_Win32"

echo Compiling release dynamic libraries...
codeblocks -na -nd -ns --build csfml-system.cbp --target="Release DLL_Win32"
codeblocks -na -nd -ns --build csfml-window.cbp --target="Release DLL_Win32"
codeblocks -na -nd -ns --build csfml-graphics.cbp --target="Release DLL_Win32"
codeblocks -na -nd -ns --build csfml-network.cbp --target="Release DLL_Win32"
codeblocks -na -nd -ns --build csfml-audio.cbp --target="Release DLL_Win32"

cd batch-build

echo Done
