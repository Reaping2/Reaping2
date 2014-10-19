@echo off
setlocal EnableExtensions
setlocal EnableDelayedExpansion
set KEY_NAME=HKEY_CLASSES_ROOT\VisualStudio.DTE.

if "%1"=="" (
	for /l %%x in (12, -1, 8) do (
		set "CUR_KEY=%KEY_NAME%%%x.0"
		reg query !CUR_KEY! >> nul 2>&1
		if !ERRORLEVEL! == 0 (
			echo "msvc%%x is installed"
			set "MSVS_VER=VS%%x0"
			set VS_MAJOR=%%x
			goto got_version
		)
	)
) else (
	set "MSVS_VER=%1"
)
:got_version
echo "Using %MSVS_VER%"

set GENERATOR_NAME=""
set XIPH_FOLDER=""
set GLEW_FOLDER=""
set VCBUILDER=""
set BOOTSTRAP_TOOLSET=""
if /i "%MSVS_VER%" == "vs80" (
	set GENERATOR_NAME="Visual Studio 8 2005"
	set "B2_TOOLSET=msvc-8.0"
	set BOOTSTRAP_TOOLSET=vc8
	set XIPH_FOLDER="VS2005"
	set GLEW_FOLDER="vc6"
	set "VCBUILDER=..\..\VC\vcpackages\vcbuild.exe"
)
if /i "%MSVS_VER%" == "vs90" (
	set GENERATOR_NAME="Visual Studio 9 2008"
	set "B2_TOOLSET=msvc-9.0"
	set BOOTSTRAP_TOOLSET=vc9
	set XIPH_FOLDER="VS2008"
	set GLEW_FOLDER="vc6"
	set "VCBUILDER=..\..\VC\vcpackages\vcbuild.exe"
)
if /i "%MSVS_VER%" == "vs100" (
	set GENERATOR_NAME="Visual Studio 10"
	set "B2_TOOLSET=msvc-10.0"
	set BOOTSTRAP_TOOLSET=vc10
	set XIPH_FOLDER="VS2010"
	set GLEW_FOLDER="vc10"
)
if /i "%MSVS_VER%" == "vs110" (
	set GENERATOR_NAME="Visual Studio 11"
	set "B2_TOOLSET=msvc-11.0"
	set BOOTSTRAP_TOOLSET=vc11
	set XIPH_FOLDER="VS2010"
	set GLEW_FOLDER="vc10"
)
if /i "%MSVS_VER%" == "vs120" (
	set GENERATOR_NAME="Visual Studio 12"
	set "B2_TOOLSET=msvc-12.0"
	set BOOTSTRAP_TOOLSET=vc12
	set XIPH_FOLDER="VS2010"
	set GLEW_FOLDER="vc10"
)

if %GENERATOR_NAME%=="" (
	set CMAKE_CMD=cmake
) else (
	set CMAKE_CMD=cmake -G %GENERATOR_NAME%
)

echo "Cmake command: %CMAKE_CMD%"
echo "Boost toolset: %B2_TOOLSET%"
set "COMMON_TOOLS=%MSVS_VER%COMNTOOLS"
set DEVENV="!%COMMON_TOOLS%!..\IDE\devenv.exe"
echo "VCBUILDER !VCBUILDER!"
set VCBUILD="!%COMMON_TOOLS%!!VCBUILDER!"

echo "Using %DEVENV% compiler"
echo "Using %VCBUILD% vcbuild"

set PWD=%CD%
set BASEDIR=%PWD%\..
echo "Working dir: " %PWD%
echo "Basedir: " %BASEDIR%
set PATH=%PATH%;%PWD%\cmake-2.8.12-win32-x86\bin;%PWD%\premake4.3;c:\Program Files (x86)\Microsoft Visual Studio 10.0\VC\bin\amd64\

rem ******************************************
rem here you can junp to any of building stuff
rem ******************************************

rem goto Build_GLEW

:Build_Boost
cd %BASEDIR%\deps\boost_1_54_0
call bootstrap.bat %BOOTSTRAP_TOOLSET%
b2 toolset=%B2_TOOLSET% --with-filesystem --with-exception --with-system --with-thread --with-date_time --with-program_options --with-atomic
echo "boost built successfully."
:Build_Glfw
rd /S /Q %BASEDIR%\deps\glfw-3.0.3\build
md %BASEDIR%\deps\glfw-3.0.3\build
cd %BASEDIR%\deps\glfw-3.0.3\build
%CMAKE_CMD% ..
%DEVENV% GLFW.sln /Build
%DEVENV% GLFW.sln /Build "Release"
echo "glfw built successfully"
:Build_ZLib
cd %PWD%
rd /S /Q %BASEDIR%\deps\zlib-1.2.8\build
md %BASEDIR%\deps\zlib-1.2.8\build
cd %BASEDIR%\deps\zlib-1.2.8\build
%CMAKE_CMD% ..
%DEVENV% zlib.sln /Build
%DEVENV% zlib.sln /Build "Release"
echo "zlib built successfully"
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
echo "libpng built successfully"
cd %BASEDIR%\deps\lpng166
copy scripts\pnglibconf.h.prebuilt pnglibconf.h
echo "libpng copied ok"
:Build_LibOgg
cd %PWD%
cd ..\deps\libogg-1.3.1\win32
cd %XIPH_FOLDER%
%DEVENV% libogg_static.sln /upgrade
%DEVENV% libogg_static.sln /Build "Debug"
%DEVENV% libogg_static.sln /Build "Release"
echo "libogg1"
mkdir ..\..\lib
mkdir ..\..\lib\debug
mkdir ..\..\lib\release
copy Win32\Debug\libogg_static.lib ..\..\lib\debug
copy Win32\Release\libogg_static.lib ..\..\lib\release
echo "libogg2"
:Build_LibVorbis
cd %PWD%
cd ..\deps\libvorbis-1.3.3\win32
cd %XIPH_FOLDER%
%DEVENV% vorbis_static.sln /upgrade
%DEVENV% vorbis_static.sln /Build "Debug"
%DEVENV% vorbis_static.sln /Build "Release"
echo "libvorbis1"
mkdir ..\..\bin
mkdir ..\..\bin\lib
mkdir ..\..\bin\lib\debug
mkdir ..\..\bin\lib\release
copy Win32\Debug\*.lib ..\..\bin\lib\debug
copy Win32\Release\*.lib ..\..\bin\lib\release
echo "libvorbis2"
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
echo "pa1"
copy options_cmake.h ..\include\pa_options_cmake.h
echo "pa2"
cd %PWD%
:Build_GLEW
echo "start of building glew"
cd %PWD%
cd ..\deps\glew-1.10.0\build
cd %GLEW_FOLDER%
echo "VCBUILDER: !VCBUILDER! "
if !VCBUILDER!=="" (
	echo "building glew static"
	%DEVENV% glew.sln /Build "Debug" /project "glew_static"
	%DEVENV% glew.sln /Build "Release" /project "glew_static" 
) else ( 
	%VCBUILD% glew_static.dsp /Upgrade
	%VCBUILD% glew_static.vcproj
)
echo "end of building glew"
