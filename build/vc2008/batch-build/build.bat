@echo off

echo Compiling debug static libraries...
msbuild ..\SFML.sln /verbosity:q /maxcpucount /target:SFML\sfml-main,SFML\sfml-system,SFML\sfml-network,SFML\sfml-window,SFML\sfml-graphics,SFML\sfml-audio /property:Configuration="Debug static"

echo Compiling debug dynamic libraries...
msbuild ..\SFML.sln /verbosity:q /maxcpucount /target:SFML\sfml-main,SFML\sfml-system,SFML\sfml-network,SFML\sfml-window,SFML\sfml-graphics,SFML\sfml-audio /property:Configuration="Debug DLL"

echo Compiling release static libraries...
msbuild ..\SFML.sln /verbosity:q /maxcpucount /target:SFML\sfml-main,SFML\sfml-system,SFML\sfml-network,SFML\sfml-window,SFML\sfml-graphics,SFML\sfml-audio /property:Configuration="Release static"

echo Compiling release dynamic libraries...
msbuild ..\SFML.sln /verbosity:q /maxcpucount /target:SFML\sfml-main,SFML\sfml-system,SFML\sfml-network,SFML\sfml-window,SFML\sfml-graphics,SFML\sfml-audio /property:Configuration="Release DLL"

echo Done
pause
