@echo off
setlocal EnableExtensions
setlocal EnableDelayedExpansion

call ..\maketools\detect_vs.bat %*

set DEPSDIR=%BASEDIR%\deps
set ARCH=Win32

set BOOST_INCLUDEDIR=%DEPSDIR%\boost_1_54_0\
set BOOST_LIBRARYDIR=%DEPSDIR%\boost_1_54_0\stage\lib\

set GLEW_INCLUDEDIR=%DEPSDIR%\glew-1.10.0\include\
set GLEW_LIBRARYDIR=%DEPSDIR%\glew-1.10.0\lib\Release\%ARCH%\

set GLFW_INCLUDEDIR=%DEPSDIR%\glfw-3.0.3\include\
set GLFW_LIBRARYDIR=%DEPSDIR%\glfw-3.0.3\build\src\Release\

set GLM_INCLUDEDIR=%DEPSDIR%\glm_0_9_4_6\

set LIBOGG_INCLUDEDIR=%DEPSDIR%\libogg-1.3.1\include\
set LIBOGG_LIBRARYDIR=%DEPSDIR%\libogg-1.3.1\lib\release\

set LIBVORBIS_INCLUDEDIR=%DEPSDIR%\libvorbis-1.3.3\include\
set LIBVORBIS_LIBRARYDIR=%DEPSDIR%\libvorbis-1.3.3\bin\lib\release\

set PNG_INCLUDEDIR=%DEPSDIR%\lpng166\
set PNG_LIBRARYDIR=%DEPSDIR%\lpng166\build\Release\

set PORTAUDIO_CMAKE_INCLUDEDIR=%DEPSDIR%\portaudio_v19\
set PORTAUDIO_INCLUDEDIR=%DEPSDIR%\portaudio_v19\include\
set PORTAUDIO_LIBRARYDIR=%DEPSDIR%\portaudio_v19\build_dir\bin\%ARCH%\Release\

set ZLIB_INCLUDEDIR=%DEPSDIR%\zlib-1.2.8\
set ZLIB_LIBRARYDIR=%DEPSDIR%\zlib-1.2.8\build\Release\

set ENET_INCLUDEDIR=%DEPSDIR%\enet-1.3.12\include\
set ENET_LIBRARYDIR=%DEPSDIR%\enet-1.3.12\Release\

%BASEDIR%\maketools\cmake-2.8.12-win32-x86\bin\cmake.exe .. -G %GENERATOR_NAME%
