@echo off

set MSBUILD=%WINDIR%\Microsoft.NET\Framework\v3.5\msbuild

echo Compiling release libraries...
%MSBUILD% ..\SFML.net.sln /verbosity:q /maxcpucount /target:sfml-window,sfml-graphics,sfml-audio /property:Configuration="Release"

echo Done
