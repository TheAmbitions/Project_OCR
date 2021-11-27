# Simple SDL mini Makefile

CC=gcc

CPPFLAGS= `pkg-config --cflags sdl` -MMD
CFLAGS= -Wall -Wextra -Werror -std=c99 -O3
LDFLAGS=
LDLIBS= `pkg-config --libs sdl2` -lSDL2_ttf

all: sudrend

sudrend: sudrend.o


clean:
	${RM} *.o
	${RM} *.d
	${RM} sudrend
	${RM} *.ppm

# END
