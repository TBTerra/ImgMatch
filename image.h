#pragma once
#include <stdint.h>
#include "imports/lodepng.c" //ajust for your own lodepng location. use lodepng.h if you are compiling lodepng to separate object

typedef struct{
	uint32_t width, height;
	uint8_t* data;
}image;//always assumed to be 24bit RGB format (always true if use load24 function)

typedef struct{
	uint32_t width, height;
	uint8_t* data;
}imageB;//always assumed to be 8bit greyscale format

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
unsigned loadImageBW(imageB* img, const char* name){
	unsigned err = lodepng_decode_file(&(img->data), &(img->width), &(img->height), name, LCT_GREY, 8);
	//printf("loaded image of size %dx%d\n",img->width,img->height);
	return err;
}
unsigned saveImage(image* img, const char* name){
	return lodepng_encode24_file(name, img->data, img->width, img->height);
}
unsigned saveImageBW(imageB* img, const char* name){
	return lodepng_encode_file(name, img->data, img->width, img->height, LCT_GREY, 8);
}
void freeImage(image* img){
	img->width=img->height=0;
	free(img->data);
}

void printCol(col A){
	printf("[%d,%d,%d]",A.r,A.g,A.b);
}

int downsampleImage(image* img, uint8_t scale){
	if(scale<=1)return 0;
	int newW = img->width/scale;
	int newH = img->height/scale;
	if((newW==0) || (newH==0))return 0;
	image temp = {newW,newH,NULL};
	temp.data = malloc(3*newW*newH);
	//do the actual down sampling
	for(int j=0;j<newH;j++){
		for(int i=0;i<newW;i++){
			uint32_t totalR=0;
			uint32_t totalG=0;
			uint32_t totalB=0;
			//loop though old pixels
			for(int l=0;l<scale;l++){
				for(int k=0;k<scale;k++){
					col p = getPixel(img, k+(scale*i), l+(scale*j));
					totalR += p.r;
					totalG += p.g;
					totalB += p.b;
				}
			}
			//set new pixel
			temp.data[3*((temp.width*j)+i)] = totalR/(scale*scale);
			temp.data[3*((temp.width*j)+i)+1] = totalG/(scale*scale);
			temp.data[3*((temp.width*j)+i)+2] = totalB/(scale*scale);
		}
	}
	//move over to new image
	free(img->data);
	img->data = temp.data;
	img->width = temp.width;
	img->height = temp.height;
	return 1;
}

int downsampleImageBW(image* img, imageB* imgBW, uint8_t scale){
	if(scale<=1)return 0;
	int newW = img->width/scale;
	int newH = img->height/scale;
	if((newW==0) || (newH==0))return 0;
	image temp = {newW,newH,NULL};
	temp.data = malloc(newW*newH);
	//do the actual down sampling
	for(int j=0;j<newH;j++){
		for(int i=0;i<newW;i++){
			uint32_t total = 0;
			//loop though old pixels
			for(int l=0;l<scale;l++){
				for(int k=0;k<scale;k++){
					total += img->data[3*((img->width*(j*scale+l))+(i*scale+k))];
				}
			}
			//set new pixel
			temp.data[(temp.width*j)+i] = total/(scale*scale);
		}
	}
	//move over to new image
	imgBW->data = temp.data;
	imgBW->width = temp.width;
	imgBW->height = temp.height;
	return 1;
}

int isColor(image* img){
	for(int j=0;j<img->height;j++){
		for(int i=0;i<img->width;i++){
			int r = img->data[3*((img->width*j)+i)];
			int g = img->data[3*((img->width*j)+i)+1];
			int b = img->data[3*((img->width*j)+i)+2];
			if((((r-g)>50)||((r-g)<-50)) || (((b-g)>50)||((b-g)<-50)) || (((r-b)>50)||((r-b)<-50))){
				return 1;
			}
		}
	}
	return 0;
}