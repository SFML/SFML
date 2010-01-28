@echo off

set MSBUILD=%WINDIR%\Microsoft.NET\Framework\v3.5\msbuild

echo Compiling debug dynamic libraries...
%MSBUILD% ..\CSFML.sln /verbosity:q /maxcpucount /target:csfml-main,csfml-system,csfml-network,csfml-window,csfml-graphics,csfml-audio /property:Configuration="Debug DLL"

echo Compiling release dynamic libraries...
%MSBUILD% ..\CSFML.sln /verbosity:q /maxcpucount /target:csfml-main,csfml-system,csfml-network,csfml-window,csfml-graphics,csfml-audio /property:Configuration="Release DLL"

echo Done
