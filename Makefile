all: main.cc
	clang -O2 main.cc -o raytracer

#bmp.o: bmp.c
#	clang -c -O2 bmp.c -o bmp.o

