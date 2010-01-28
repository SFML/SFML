@echo off

set MSBUILD=%WINDIR%\Microsoft.NET\Framework\v2.0.50727\msbuild

echo Compiling debug dynamic libraries...
%MSBUILD% ..\CSFML.sln /verbosity:q /target:csfml-main,csfml-system,csfml-network,csfml-window,csfml-graphics,csfml-audio /property:Configuration="Debug DLL"

echo Compiling release dynamic libraries...
%MSBUILD% ..\CSFML.sln /verbosity:q /target:csfml-main,csfml-system,csfml-network,csfml-window,csfml-graphics,csfml-audio /property:Configuration="Release DLL"

echo Done
