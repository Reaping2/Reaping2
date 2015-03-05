@echo off

set KEY_NAME=HKEY_CLASSES_ROOT\VisualStudio.DTE.

if "%1"=="" (
	for /l %%x in (12, -1, 8) do (
		set "CUR_KEY=%KEY_NAME%%%x.0"
		reg query !CUR_KEY! >> nul 2>&1
		if !ERRORLEVEL! == 0 (
			echo msvc%%x is installed
			set "MSVS_VER=VS%%x0"
			set VS_MAJOR=%%x
			goto got_version
		)
	)
) else (
	set "MSVS_VER=%1"
)
:got_version
echo Using %MSVS_VER%

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

echo Cmake command: %CMAKE_CMD%
echo Boost toolset: %B2_TOOLSET%
set "COMMON_TOOLS=%MSVS_VER%COMNTOOLS"
set DEVENV="!%COMMON_TOOLS%!..\IDE\devenv.exe"
echo VCBUILDER !VCBUILDER!
set VCBUILD="!%COMMON_TOOLS%!!VCBUILDER!"

echo Using %DEVENV% compiler
echo Using %VCBUILD% vcbuild

set PWD=%CD%
set BASEDIR=%PWD%\..
echo Working dir: %PWD%
echo Basedir: %BASEDIR%
set PATH=%PATH%;%PWD%\cmake-2.8.12-win32-x86\bin;%PWD%\premake4.3
