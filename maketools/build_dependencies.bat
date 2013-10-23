set VCDIR="c:\Program Files (x86)\Microsoft Visual Studio 9.0\"

@echo OFF
set DEVENV=%VCDIR%\Common7\IDE\devenv.exe
set PWD=%CD%
set BASEDIR=%PWD%\..
echo "Working dir: " %PWD%
echo "Basedir: " %BASEDIR%
set PATH=%PATH%;%PWD%\cmake-2.8.12-win32-x86\bin;%PWD%\premake4.3
md %BASEDIR%\deps\glfw-3.0.3\build
cd %BASEDIR%\deps\glfw-3.0.3\build
cmake ..
%DEVENV% GLFW.sln /Build
cd %PWD%
