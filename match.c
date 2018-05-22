#include <stdio.h>
#include <stdint.h>
#include "imports/lodepng.c" #ajust for your own lodepng location. use lodepng.h if you are compiling lodepng to separate object

typedef struct{
	unsigned int width, height;
	unsigned char* data;
}image;//always asumed to be 32bit RGBA format (always true if use load32 function)

unsigned loadImage(image* img, const char* name);
void freeImage(image* img);
uint32_t findMin(image* imgS, image* imgL);
uint32_t getScore(image* imgS, image* imgL, uint32_t offX, uint32_t offY);

//there should be 2 arguments (for now)
//1st argument: the file with a section of image
//2nd argument: the file of the full image to test against
//in future if the 2nd argument is missing then it will match against all files in the data dir

int main(int argc, char* argv[]){
	if(argc < 3){
		printf("usage: match <small img> <images to match against>");
		return -1;
	}
	
	image smallI;
	image largeI;
	loadImage(&smallI, argv[1]);
	loadImage(&largeI, argv[2]);
	best = findMin(&smallI,&largeI)
	
	return 0;
}

uint32_t findMin(image* imgS, image* imgL){
	int32_t scanW = imgL->width - imgS->width;
	int32_t scanH = imgL->height - imgS->height;
	if((scanW <= 0) || (scanH <= 0)){
		printf("cant work if small image is larger than large image")
		return -1;
	}
	if((imgS->width*imgS->height)>21800){
		printf("cant guarantee this is gonna work. image too large, may cause int overflow")
		return -1
	}
	uint32_t best = -1;
	for(int i=0;i<scanH;i++){
		for(int j=0;j<scanW;j++){
			int score = getScore(imgS, imgL, j,i);
			if(score < best)
				best = score;
		}
	}
	return best;
}

uint32_t getScore(image* imgS, image* imgL, int offX, int offY){
	return -1;
}

unsigned loadImage(image* img, const char* name){
	unsigned err = lodepng_decode32_file(&(img->data), &(img->width), &(img->height), name);
	printf("loaded image of size %dx%d\n",img->width,img->height);
	return err;
}
void freeImage(image* img){
	img->width=img->height=0;
	free(img->data);
}