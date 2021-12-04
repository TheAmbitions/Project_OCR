# Simple SDL mini Makefile

CC=gcc

CPPFLAGS= `pkg-config --cflags sdl` -MMD
CFLAGS= -Wall -Wextra -Werror -std=c99 -O3
LDFLAGS=
LDLIBS= `pkg-config --libs sdl SDL_image` -lSDL_gfx -lm 


all: hough

hough: pixel_operations.o display.o rotation.o otsu.o detection.o hough.o


clean:
	${RM} *.o
	${RM} *.d
	${RM} hough

# END
