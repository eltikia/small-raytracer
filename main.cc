#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "bmp.c"

typedef struct {
	double  x;
	double 	y;
	double 	z;
} Vector;
typedef Vector Point;
typedef Vector Color;

typedef struct {
	Point origin;
	Vector direction;
} Ray;

typedef struct {
	Vector normal;
	double distance;
	Color color;
} Plane;

Ray getRay(int x, int y) {
	Ray r;
	r.origin.x = 0;
	r.origin.y = 0;
	r.origin.z = 0;
	r.direction.x = -320 + x + 0.5;
	r.direction.y = 240 - y + 0.5;
	r.direction.z = -120;
	return r;
}

Plane* createBox() {
	Plane* box = (Plane*)malloc(5*sizeof(Plane));
	int size = 5;
	Plane p0 = {-1, 0, 0, size, 255, 0, 100};//links
	box[0] = p0;
	Plane p1 = {1, 0, 0, size, 0, 255, 0};//rechts
	box[1] = p1;
	Plane p2 = {0, 0, -1, size, 255,255,255};//hinten
	box[2] = p2;
	Plane p3 = {0, 1, 0, size, 255, 255, 255};//oben
	box[3] = p3;
	Plane p4 = {0, -1, 0, size, 255, 255, 255};//unten
	box[4] = p4;
	return box;
}	

Point rayHitsPlane(Ray r, Plane p, double* hit) {
	double gamma = r.direction.x * p.normal.x + r.direction.y * p.normal.y + r.direction.z * p.normal.z;
	double side = p.distance - (p.normal.x * r.origin.x + p.normal.y * r.origin.y + p.normal.z * r.origin.z);
	Point point = {0,0,0};
	if( gamma * side <= 0) {
		*hit = 0;
	} else {
		double lambda = side / gamma;
		point.x = r.origin.x + r.direction.x * lambda;
		point.y = r.origin.y + r.direction.y * lambda;
		point.z = r.origin.z + r.direction.z * lambda;
		*hit = lambda;
	}
	return point;
}

inline double vectorLength(Vector v) {
	return sqrt(v.x*v.x + v.y*v.y + v.z*v.z);
}

extern int write_bmp(const char* filename, int width, int height, char* rgb);

int main(int argc, char* argv[]) {
	Plane* box = createBox();
	char* rgb = (char*)malloc(3*640*480*sizeof(char));	

	double hit = 0;
	for(int x=0; x<640; x++) {
		for(int y=0; y<480; y++) {
			Ray r = getRay(x, y);
			for(int i=0;i<5;i++) {
				Point point = rayHitsPlane(r, box[i], &hit);
				if(hit > 0) {
					Vector toLight = { -2 - point.x, 2 - point.y, -1 - point.z };
					Ray rayToLight = { point, toLight };
					double lightHit = 0;
					for(int j=0; j<5; j++) {
						if(i == j) continue;
						Point p2 = rayHitsPlane(rayToLight, box[j], &lightHit);
						if(lightHit < 1 && lightHit > 0) {
							break;
						}
					}
					if(lightHit >= 1 || lightHit == 0) {
						double length = vectorLength(toLight);
						double factor  = 13 / (length * length);
						int ipos = 3*(640*y+x);
						double red = box[i].color.z * factor;
						double green  = box[i].color.y * factor;
						double blue = box[i].color.x * factor;

						rgb[ipos + 2] = (unsigned char)(red > 255 ? 255 : red); 
						rgb[ipos + 1] = (unsigned char)(green > 255 ? 255 : green);
						rgb[ipos] = (unsigned char)(blue > 255 ? 255 : blue);
					}
				}
			}
		}
	}
	write_bmp("test.bmp", 640, 480, rgb);
	free(box);free(rgb);
	return 0;
}
