@echo off
set COMPILER=gcc

set INCLUDE_DIR=C:\Users\ASUS\Downloads\SDL2-devel-2.28.3-mingw\SDL2-2.28.3\x86_64-w64-mingw32\include
set LIB_DIR=C:\Users\ASUS\Downloads\SDL2-devel-2.28.3-mingw\SDL2-2.28.3\x86_64-w64-mingw32\lib

%COMPILER% main.c -I"%INCLUDE_DIR%" -I"%INCLUDE_DIR%\SDL2" -L"%LIB_DIR%" -lmingw32 -lSDL2main -lSDL2 -o main.exe

pause

