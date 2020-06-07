CC = gcc

apple: mandelbrot.c mandelbrot.cl
	gcc mandelbrot.c -framework OpenCL -o mandelbrot

linux: mandelbrot.c mandelbrot.cl
	gcc mandelbrot.c -lOpenCL -o mandelbrot

clean:
	rm out.dat mandelbrot