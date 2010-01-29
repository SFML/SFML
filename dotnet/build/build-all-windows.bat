@echo off

echo Compiling Visual C++ 2008 libraries...
cd vc2008\batch-build
call build.bat

cd ..\..

echo Done
pause
