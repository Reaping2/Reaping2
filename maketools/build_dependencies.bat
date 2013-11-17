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
if /i "%MSVS_VER%" == "vs80" (
	set GENERATOR_NAME="Visual Studio 8 2005"
	set "B2_TOOLSET=msvc-8.0"
	set XIPH_FOLDER="VS2005"
)
if /i "%MSVS_VER%" == "vs90" (
	set GENERATOR_NAME="Visual Studio 9 2008"
	set "B2_TOOLSET=msvc-9.0"
	set XIPH_FOLDER="VS2008"
)
if /i "%MSVS_VER%" == "vs100" (
	set GENERATOR_NAME="Visual Studio 10"
	set "B2_TOOLSET=msvc-10.0"
	set XIPH_FOLDER="VS2010"
)
if /i "%MSVS_VER%" == "vs110" (
	set GENERATOR_NAME="Visual Studio 11"
	set "B2_TOOLSET=msvc-11.0"
	set XIPH_FOLDER="VS2011"
)
if /i "%MSVS_VER%" == "vs120" (
	set GENERATOR_NAME="Visual Studio 12"
	set "B2_TOOLSET=msvc-12.0"
	set XIPH_FOLDER="VS2012"
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

echo "Using %DEVENV% compiler"

set PWD=%CD%
set BASEDIR=%PWD%\..
echo "Working dir: " %PWD%
echo "Basedir: " %BASEDIR%
set PATH=%PATH%;%PWD%\cmake-2.8.12-win32-x86\bin;%PWD%\premake4.3
cd %BASEDIR%\deps\boost_1_54_0
call bootstrap.bat
b2 toolset=%B2_TOOLSET% --with-filesystem --with-exception --with-system --with-thread --with-date_time --with-program_options
rd /S /Q %BASEDIR%\deps\glfw-3.0.3\build
md %BASEDIR%\deps\glfw-3.0.3\build
cd %BASEDIR%\deps\glfw-3.0.3\build
%CMAKE_CMD% ..
%DEVENV% GLFW.sln /Build
%DEVENV% GLFW.sln /Build "Release"
cd %PWD%
rd /S /Q %BASEDIR%\deps\zlib-1.2.8\build
md %BASEDIR%\deps\zlib-1.2.8\build
cd %BASEDIR%\deps\zlib-1.2.8\build
%CMAKE_CMD% ..
%DEVENV% zlib.sln /Build
%DEVENV% zlib.sln /Build "Release"
copy zconf.h ..
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
cd %BASEDIR%\deps\lpng166
copy scripts\pnglibconf.h.prebuilt pnglibconf.h
cd %PWD%
cd ..\deps\libogg-1.3.1\win32
cd %XIPH_FOLDER%
%DEVENV% libogg_static.sln /Build "Debug"
%DEVENV% libogg_static.sln /Build "Release"
mkdir ..\..\lib
mkdir ..\..\lib\debug
mkdir ..\..\lib\release
copy Win32\Debug\libogg_static.lib ..\..\lib\debug
copy Win32\Release\libogg_static.lib ..\..\lib\release
cd %PWD%
cd ..\deps\libvorbis-1.3.3\win32
cd %XIPH_FOLDER%
%DEVENV% vorbis_static.sln /Build "Debug"
%DEVENV% vorbis_static.sln /Build "Release"
mkdir ..\..\bin
mkdir ..\..\bin\lib
mkdir ..\..\bin\lib\debug
mkdir ..\..\bin\lib\release
copy Win32\Debug\*.lib ..\..\bin\lib\debug
copy Win32\Release\*.lib ..\..\bin\lib\release
cd %PWD%
cd ..\deps\portaudio_v19
mkdir build_dir
cd build_dir
%CMAKE_CMD% ..
%DEVENV% portaudio.sln /Build "Debug"
%DEVENV% portaudio.sln /Build "Release"
copy options_cmake.h ..\include\pa_options_cmake.h
cd %PWD%
