@echo off

:: this build.bat uses MinGW64

set CFLAGS = -std=c99 -Wall -Iwindbolt
set LDFLAGS = -luser32 -ld3d11 -ldxgi -ld3dcompiler

set DEBUG_FLAGS = -O0 -g
set RELEASE_FLAGS = -O3 -DNDEBUG

gcc windbolt/build.c game/main.c -o bin/out.exe %CFLAGS% %LDFLAGS% %DEBUG_FLAGS%
