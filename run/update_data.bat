@echo off
..\bin\debug\r2pkg.exe -c data
copy data.pkg ..\bin\debug\
copy data.pkg ..\bin\release\
