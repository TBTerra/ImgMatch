#pragma once
#include "imports/lodepng.c" //ajust for your own lodepng location. use lodepng.h if you are compiling lodepng to separate object

typedef struct{
	uint32_t width, height;
	uint8_t* data;
}image;//always assumed to be 24bit RGB format (always true if use load24 function)

typedef struct{
	uint8_t r,g,b;
}col;

typedef struct{
	int x,y;
}point;

col getPixel(image* img, int x, int y){
	col out;
	out.r = img->data[3*((img->width*y)+x)];
	out.g = img->data[3*((img->width*y)+x)+1];
	out.b = img->data[3*((img->width*y)+x)+2];
	return out;
}

unsigned loadImage(image* img, const char* name){
	unsigned err = lodepng_decode24_file(&(img->data), &(img->width), &(img->height), name);
	//printf("loaded image of size %dx%d\n",img->width,img->height);
	return err;
}
void freeImage(image* img){
	img->width=img->height=0;
	free(img->data);
}

void printCol(col A){
	printf("[%d,%d,%d]",A.r,A.g,A.b);
}