CC = cc

CFLAGS = -std=c99 -Wall -Iwindbolt -D_GNU_SOURCE
LDFLAGS = -lX11 -lGLX -lGL -lm

DEBUG_FLAGS = -O0 -g
RELEASE_FLAGS = -O3 -DNDEBUG

debug:
	$(CC) windbolt/build.c game/main.c -o bin/out $(CFLAGS) $(LDFLAGS) $(DEBUG_FLAGS)
	./bin/out

release:
	$(CC) windbolt/build.c game/main.c -o bin/out $(CFLAGS) $(LDFLAGS) $(RELEASE_FLAGS)
