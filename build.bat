@echo off

if [%1]==[] goto usage
if "%~1"=="all" goto script
if "%~1"=="re" goto script
if "%~1"=="clean" goto script
if "%~1"=="fclean" goto script
if "%~1"=="test" goto script

:usage
echo Usage: %0 {all,re,clean,fclean,test}
exit 1

:script
cd tinky
nmake /NOLOGO %1
cd ..\winkey
nmake /NOLOGO %1
cd ..\
if "%~1"=="all" goto get_exec
if "%~1"=="re" goto get_exec
if "%~1"=="test" goto get_exec
if "%~1"=="fclean" goto clean_exec
exit 0

:get_exec
@echo Copy files in the current directory
@echo svc.exe
copy tinky\svc.exe .
@echo winkey.exe
copy winkey\winkey.exe .
exit 0

:clean_exec
del svc.exe
del winkey.exe
exit 0
