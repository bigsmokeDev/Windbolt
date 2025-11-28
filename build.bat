:: this build.bat uses MinGW64
@echo off

set CFLAGS=-std=c99 -Wall -Iwindbolt
set LDFLAGS=-luser32 -lgdi32 -lopengl32

set DEBUG_FLAGS=-O0 -g
set RELEASE_FLAGS=-O3 -DNDEBUG

IF "%1"=="RELEASE" (
	gcc windbolt/build.c game/main.c -o bin/out.exe %CFLAGS% %LDFLAGS% %RELEASE_FLAGS%
) ELSE (
	gcc windbolt/build.c game/main.c -o bin/out.exe %CFLAGS% %LDFLAGS% %DEBUG_FLAGS%
)
