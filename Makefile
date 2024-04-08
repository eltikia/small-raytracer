all: main.c bmp.o
	gcc -Wall -O2 bmp.o main.c -o raytracer -lm

#Darf aus irgendeinem Grund nicht mit O2 kompiliert werden...
bmp.o: bmp.c
	gcc -Wall -c bmp.c -o bmp.o

