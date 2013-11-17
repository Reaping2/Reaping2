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
			set "MSVS_VER=vs%%x0"
			set VS_MAJOR=%%x
			goto got_version
		)
	)
) else (
	set "MSVS_VER=%1"
)
:got_version
set GENERATOR_NAME=""
if /i "%MSVS_VER%" == "vs80" (
	set GENERATOR_NAME="Visual Studio 8 2005"
	set "VS_VER=vs2005"
)
if /i "%MSVS_VER%" == "vs90" (
	set GENERATOR_NAME="Visual Studio 9 2008"
	set "VS_VER=vs2008"
)
if /i "%MSVS_VER%" == "vs100" (
	set GENERATOR_NAME="Visual Studio 10"
	set "VS_VER=vs2010"
)
if /i "%MSVS_VER%" == "vs110" (
	set GENERATOR_NAME="Visual Studio 11"
	set "VS_VER=vs2010"
)
if /i "%MSVS_VER%" == "vs120" (
	set GENERATOR_NAME="Visual Studio 12"
	set "VS_VER=vs2010"
)
..\maketools\premake-dev\premake-dev.exe %VS_VER%
echo "Using %MSVS_VER%"

if /i "%MSVS_VER%" == "vs110" (
	set "COMMON_TOOLS=%MSVS_VER%COMNTOOLS"
	set "DEVENV="!%COMMON_TOOLS%!..\IDE\devenv.exe"

	echo "Using %MSVS_VER% compiler"
	echo "ez miakurvaanny·Èrt nem megy nekem? :D"
	"c:\Program Files (x86)\Microsoft Visual Studio 11.0\Common7\IDE\devenv.exe" "..\build\Reaping2.sln" /upgrade
)

