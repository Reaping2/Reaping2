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
%DEVENV% GLFW.sln /Build "Release"
cd %PWD%
md %BASEDIR%\deps\zlib-1.2.8\build
cd %BASEDIR%\deps\zlib-1.2.8\build
cmake ..
%DEVENV% zlib.sln /Build
%DEVENV% zlib.sln /Build "Release"
copy zconf.h ..
md %BASEDIR%\deps\lpng166\build
cd %BASEDIR%\deps\lpng166\build
set ZLIB_INCLUDE_DIR=%BASEDIR%\deps\zlib-1.2.8\
set ZLIB_LIBRARY=%BASEDIR%\deps\zlib-1.2.8\build\Debug\zlibd.lib
cmake -DZLIB_LIBRARY:FILEPATH=%ZLIB_LIBRARY% -DZLIB_INCLUDE_DIR:PATH=%ZLIB_INCLUDE_DIR% ..
%DEVENV% libpng.sln /Build "Debug"
set ZLIB_LIBRARY=%BASEDIR%\deps\zlib-1.2.8\build\Release\zlib.lib
cmake -DZLIB_LIBRARY:FILEPATH=%ZLIB_LIBRARY% -DZLIB_INCLUDE_DIR:PATH=%ZLIB_INCLUDE_DIR% ..
%DEVENV% libpng.sln /Build "Release"
cd %BASEDIR%\deps\lpng166
copy scripts\pnglibconf.h.prebuilt pnglibconf.h
cd %PWD%
