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
	set "PREMAKE_VS_ACTION=vs2005"
)
if /i "%MSVS_VER%" == "vs90" (
	set GENERATOR_NAME="Visual Studio 9 2008"
	set "PREMAKE_VS_ACTION=vs2008"
)
if /i "%MSVS_VER%" == "vs100" (
	set GENERATOR_NAME="Visual Studio 10"
	set "PREMAKE_VS_ACTION=vs2010"
)
if /i "%MSVS_VER%" == "vs110" (
	set GENERATOR_NAME="Visual Studio 11"
	set "PREMAKE_VS_ACTION=vs2012"
)
if /i "%MSVS_VER%" == "vs120" (
	set GENERATOR_NAME="Visual Studio 12"
	set "PREMAKE_VS_ACTION=vs2013"
)

echo "premake-dev.exe %PREMAKE_VS_ACTION%"
call ..\maketools\premake-dev\premake-dev.exe %PREMAKE_VS_ACTION%