@echo off
call "cmd /c start main -s -m data"
call "cmd /c start main -h localhost -n host -m data -f host"
call "cmd /c start main -c localhost -n client1 -m data -f client"
rem start main -m data
