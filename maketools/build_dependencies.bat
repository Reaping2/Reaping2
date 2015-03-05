@echo off
setlocal EnableExtensions
setlocal EnableDelayedExpansion

call detect_vs.bat %*

rem ************************************************************
rem Setup finished, you can select what dependency to build next
rem ************************************************************

rem goto Build_Glfw

:Build_Boost
cd %BASEDIR%\deps\boost_1_54_0
call bootstrap.bat %BOOTSTRAP_TOOLSET%
b2 toolset=%B2_TOOLSET% --with-filesystem --with-exception --with-system --with-thread --with-date_time --with-program_options --with-atomic --with-serialization
echo boost built successfully.

:Build_Glfw
rd /S /Q %BASEDIR%\deps\glfw-3.0.3\build
md %BASEDIR%\deps\glfw-3.0.3\build
cd %BASEDIR%\deps\glfw-3.0.3\build
%CMAKE_CMD% ..
%DEVENV% GLFW.sln /Build
%DEVENV% GLFW.sln /Build "Release"
echo glfw built successfully

:Build_ZLib
cd %PWD%
rd /S /Q %BASEDIR%\deps\zlib-1.2.8\build
md %BASEDIR%\deps\zlib-1.2.8\build
cd %BASEDIR%\deps\zlib-1.2.8\build
%CMAKE_CMD% ..
%DEVENV% zlib.sln /Build
%DEVENV% zlib.sln /Build "Release"
echo zlib built successfully
copy zconf.h ..

:Build_LibPng
rd /S /Q %BASEDIR%\deps\lpng166\build
md %BASEDIR%\deps\lpng166\build
cd %BASEDIR%\deps\lpng166\build
set ZLIB_INCLUDE_DIR=%BASEDIR%\deps\zlib-1.2.8\
set ZLIB_LIBRARY=%BASEDIR%\deps\zlib-1.2.8\build\Debug\zlibd.lib
%CMAKE_CMD% -DZLIB_LIBRARY:FILEPATH=%ZLIB_LIBRARY% -DZLIB_INCLUDE_DIR:PATH=%ZLIB_INCLUDE_DIR% ..
%DEVENV% libpng.sln /Build "Debug"
set ZLIB_LIBRARY=%BASEDIR%\deps\zlib-1.2.8\build\Release\zlib.lib
%CMAKE_CMD% -DZLIB_LIBRARY:FILEPATH=%ZLIB_LIBRARY% -DZLIB_INCLUDE_DIR:PATH=%ZLIB_INCLUDE_DIR% ..
%DEVENV% libpng.sln /Build "Release"
echo libpng built successfully
cd %BASEDIR%\deps\lpng166
copy scripts\pnglibconf.h.prebuilt pnglibconf.h
echo libpng copied

:Build_LibOgg
cd %PWD%
cd ..\deps\libogg-1.3.1\win32
cd %XIPH_FOLDER%
%DEVENV% libogg_static.sln /upgrade
%DEVENV% libogg_static.sln /Build "Debug"
%DEVENV% libogg_static.sln /Build "Release"
echo libogg built
mkdir ..\..\lib
mkdir ..\..\lib\debug
mkdir ..\..\lib\release
copy Win32\Debug\libogg_static.lib ..\..\lib\debug
copy Win32\Release\libogg_static.lib ..\..\lib\release
echo libogg copied

:Build_LibVorbis
cd %PWD%
cd ..\deps\libvorbis-1.3.3\win32
cd %XIPH_FOLDER%
%DEVENV% vorbis_static.sln /upgrade
%DEVENV% vorbis_static.sln /Build "Debug"
%DEVENV% vorbis_static.sln /Build "Release"
echo libvorbis built
mkdir ..\..\bin
mkdir ..\..\bin\lib
mkdir ..\..\bin\lib\debug
mkdir ..\..\bin\lib\release
copy Win32\Debug\*.lib ..\..\bin\lib\debug
copy Win32\Release\*.lib ..\..\bin\lib\release
echo libvorbis copied

:Build_Portaudio
cd %PWD%
cd ..\deps\portaudio_v19
mkdir build_dir
cd build_dir
%CMAKE_CMD% ..
echo #define PA_WDMKS_NO_KSGUID_LIB >> options_cmake.h
%DEVENV% portaudio.sln /upgrade
%DEVENV% portaudio.sln /Build "Debug"
%DEVENV% portaudio.sln /Build "Release"
echo portaudio built
copy options_cmake.h ..\include\options_cmake.h
echo portaudio options_cmake.h copied

:Build_GLEW
echo start of building glew
cd %PWD%
cd ..\deps\glew-1.10.0\build
cd %GLEW_FOLDER%
echo VCBUILDER: !VCBUILDER! 
if !VCBUILDER!=="" (
	echo building glew static
	%DEVENV% glew.sln /Build "Debug" /project "glew_static"
	%DEVENV% glew.sln /Build "Release" /project "glew_static" 
) else ( 
	%VCBUILD% glew_static.dsp /Upgrade
	%VCBUILD% glew_static.vcproj
)
echo end of building glew

:Build_ENET
echo start of building enet
cd %PWD%
cd ..\deps\enet-1.3.12
;%DEVENV% enet.dsp /upgrade
echo start of building enet debug
%DEVENV% enet.sln /Build "Debug"
echo start of building enet release
%DEVENV% enet.sln /Build "Release"
echo end of building enet

:end

