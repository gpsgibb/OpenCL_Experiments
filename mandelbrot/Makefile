CC = gcc

CFLAGS = -g -O0

UNAME_S = $(shell uname -s)

ifeq ($(UNAME_S),Darwin)
        OCLFLAGS = -framework OpenCL
endif
ifeq ($(UNAME_S),Linux)
        OCLFLAGS = -lOpenCL
endif

mandelbrot: mandelbrot.c mandelbrot.cl
	$(CC) $(CFLAGS) mandelbrot.c $(OCLFLAGS) -o mandelbrot

clean:
	rm out.dat mandelbrot