# Simple SDL mini Makefile

CC=gcc

CPPFLAGS= `pkg-config --cflags sdl` -MMD
CFLAGS= -Wall -Wextra -Werror -std=c99 -O3
LDFLAGS=
LDLIBS= `pkg-config --libs sdl` -lSDL_image -lm

all: main display hough

hough: hough.o pixel_operations.o display.o
display: main_display.o display.o pixel_operations.o
main: main.o pixel_operations.o grayscale.o resolve.o brute_force.o input_sudoku.o output_sudoku.o isSafe.o network.o backward.o feedforward.o matrix.o sigmoid.o saveimage.o recognition.o display.o sobel.o rotations.o kernel.o

clean:
	${RM} *.o
	${RM} *.d
	${RM} main
	${RM} display
	${RM} hough

# END
