CC = gcc
CFLAGS = -g -O0

UNAME_S = $(shell uname -s)

ifeq ($(UNAME_S),Darwin)
	OCLFLAGS = -framework OpenCL
endif
ifeq ($(UNAME_S),Linux)
	OCLFLAGS = -lOpenCL
endif

oclinfo: oclinfo.c
	$(CC) $(CFLAGS) oclinfo.c $(OCLFLAGS) -o oclinfo

clean:
	rm oclinfo
