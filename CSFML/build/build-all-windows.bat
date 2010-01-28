@echo off

echo Compiling MinGW libraries...
cd codeblocks\batch-build
call build.bat

echo Compiling Visual C++ 2005 libraries...
cd ..\..\vc2005\batch-build
call build.bat

echo Compiling Visual C++ 2008 libraries...
cd ..\..\vc2008\batch-build
call build.bat

cd ..\..

echo Done
pause
