CC = gcc

apple: oclinfo.c
	gcc oclinfo.c -framework OpenCL -o oclinfo

linux: oclinfo.c
	gcc oclinfo.c -lOpenCL -o oclinfo

clean:
	rm oclinfo