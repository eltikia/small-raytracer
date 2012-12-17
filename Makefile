all: main.c bmp.o
	gcc -O2 bmp.o main.c -o raytracer

#Darf aus irgendeinem Grund nicht mit O2 kompiliert werden...
bmp.o: bmp.c
	gcc -c bmp.c -o bmp.o

