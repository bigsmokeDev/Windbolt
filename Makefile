CC = clang

CFLAGS = -std=c99 -Wall -Iwindbolt -D_GNU_SOURCE
LDFLAGS = -lX11 -lGLX -lGL -lm

DEBUG_FLAGS = -O0 -g
RELEASE_FLAGS = -O3 -DNDEBUG

all: debug

bin/vendor.o: windbolt/vendor/build.c
	$(CC) -c windbolt/vendor/build.c -o bin/vendor.o -O3

debug: bin/vendor.o
	$(CC) bin/vendor.o windbolt/build.c game/main.c -o bin/out $(CFLAGS) $(LDFLAGS) $(DEBUG_FLAGS)
	./bin/out

release: bin/vendor.o
	$(CC) windbolt/build.c game/main.c -o bin/out $(CFLAGS) $(LDFLAGS) $(RELEASE_FLAGS)

clean:
	rm -f bin/out bin/vendor.o
