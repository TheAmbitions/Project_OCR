# Simple SDL mini Makefile

CC=gcc

CPPFLAGS= `pkg-config --cflags sdl` -MMD
CFLAGS= -Wall -Wextra -Werror -std=c99 -O3
LDFLAGS=
LDLIBS= `pkg-config --libs sdl` -lSDL_image -lm

all: main

main: main.o pixel_operations.o display.o sigmoid.o matrix.o

clean:
	${RM} *.o
	${RM} *.d
	${RM} main

# END
