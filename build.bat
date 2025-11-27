:: this build.bat uses MinGW64
@echo off

IF "%1"=="" (
	echo "usage: build.bat [RELEASE]"
	exit /b
)

set CFLAGS=-std=c99 -Wall -Iwindbolt
set LDFLAGS=-luser32 -ld3d11 -ldxgi -ld3dcompiler

set DEBUG_FLAGS=-O0 -g
set RELEASE_FLAGS=-O3 -DNDEBUG

IF "%1"=="RELEASE" (
	gcc windbolt/build.c game/main.c -o bin/out.exe %CFLAGS% %LDFLAGS% %RELEASE_FLAGS%
) ELSE (
	gcc windbolt/build.c game/main.c -o bin/out.exe %CFLAGS% %LDFLAGS% %DEBUG_FLAGS%
)
