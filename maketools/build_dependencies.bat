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
if /i "%MSVS_VER%" == "vs80" (
	set GENERATOR_NAME="Visual Studio 8 2005"
	set "B2_TOOLSET=msvc-8.0"
)
if /i "%MSVS_VER%" == "vs90" (
	set GENERATOR_NAME="Visual Studio 9 2008"
	set "B2_TOOLSET=msvc-9.0"
)
if /i "%MSVS_VER%" == "vs100" (
	set GENERATOR_NAME="Visual Studio 10"
	set "B2_TOOLSET=msvc-10.0"
)
if /i "%MSVS_VER%" == "vs110" (
	set GENERATOR_NAME="Visual Studio 11"
	set "B2_TOOLSET=msvc-11.0"
)
if /i "%MSVS_VER%" == "vs120" (
	set GENERATOR_NAME="Visual Studio 12"
	set "B2_TOOLSET=msvc-12.0"
)

if %GENERATOR_NAME%=="" (
	set CMAKE_CMD=cmake
) else (
	set CMAKE_CMD=cmake -G %GENERATOR_NAME%
)

echo "Cmake command: %CMAKE_CMD%"
ech "Boost toolset: %B2_TOOLSET%"
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
b2 toolset=$B2_TOOLSET$ --with-filesystem --with-exception --with-system --with-thread --with-date_time --with-program_options
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
