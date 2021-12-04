# Makefile

CC = gcc

CPPFLAGS= `pkg-config --cflags sdl` -MMD -g
CFLAGS = `pkg-config --cflags gtk+-3.0` -Wall -Wextra -Werror -std=c99 -O3
LDLIBS = `pkg-config --libs gtk+-3.0` `pkg-config --libs sdl` -lSDL_image -lSDL_gfx -lm

all: main hough

main: display.o rotations.o otsu.o pixel_operations.o resolve.o display_sudoku.o network.o matrix.o hough.o detection.o

hough: hough.o display.o rotations.o detection.o matrix.o network.o display.o pixel_operations.o otsu.o

clean:
	${RM} *.o
	${RM} *.d
	${RM} main

# END
