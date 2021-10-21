# Simple SDL mini Makefile

CC=gcc

CPPFLAGS= `pkg-config --cflags sdl` -MMD
CFLAGS= -Wall -Wextra -Werror -std=c99 -O3
LDFLAGS=
LDLIBS= `pkg-config --libs sdl SDL_image`


all: main

main: pixel_operations.o sobel.o main.o


clean:
	${RM} *.o
	${RM} *.d
	${RM} sobel
	${RM} main

# END
