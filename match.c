#include <stdio.h>
#include <stdint.h>
#include "imports/lodepng.c" //ajust for your own lodepng location. use lodepng.h if you are compiling lodepng to separate object

typedef struct{
	uint32_t width, height;
	uint8_t* data;
}image;//always assumed to be 24bit RGB format (always true if use load24 function)

typedef struct{
	uint8_t r,g,b;
}col;

unsigned loadImage(image* img, const char* name);
void freeImage(image* img);
uint32_t findMin(image* imgS, image* imgL);
uint32_t getScore(image* imgS, image* imgL, int offX, int offY);
col getPixel(image* img, int x, int y);
uint32_t difPixel(col A, col B);

//there should be 2 arguments (for now)
//1st argument: the file with a section of image
//2nd argument: the file of the full image to test against
//in future if the 2nd argument is missing then it will match against all files in the data dir

void printCol(col A){
	printf("[%d,%d,%d]",A.r,A.g,A.b);
}

int main(int argc, char* argv[]){
	if(argc < 3){
		printf("usage: match <small img> <images to match against>");
		return -1;
	}
	
	image smallI;
	image largeI;
	loadImage(&smallI, argv[1]);
	loadImage(&largeI, argv[2]);
	uint32_t best = findMin(&smallI,&largeI);
	printf("best score was %u",best);
	
	return 0;
}

uint32_t findMin(image* imgS, image* imgL){
	int32_t scanW = imgL->width - imgS->width;
	int32_t scanH = imgL->height - imgS->height;
	if((scanW <= 0) || (scanH <= 0)){
		printf("cant work if small image is larger than large image");
		return -1;
	}
	if((imgS->width*imgS->height)>21800){
		printf("cant guarantee this is gonna work. image too large, may cause int overflow");
		return -1;
	}
	uint32_t best = -1;
	int bX=-1;int bY=-1;
	for(int j=0;j<=scanH;j++){
		for(int i=0;i<=scanW;i++){
			int score = getScore(imgS, imgL, i,j);
			if(score < best){
				best = score;
				bX=j;bY=i;
			}
		}
	}
	printf("at [%d,%d]",bX,bY);
	return best;
}

uint32_t getScore(image* imgS, image* imgL, int offX, int offY){
	uint32_t score = 0;
	for(int j=0;j<imgS->height;j++){
		for(int i=0;i<imgS->width;i++){
			col S = getPixel(imgS, i, j);
			col L = getPixel(imgL, i+offX, j+offY);
			score += difPixel(S,L);
		}
	}
	//printf("score %u at [%d,%d]\n",score,offX,offY);
	return score;
}

col getPixel(image* img, int x, int y){
	col out;
	out.r = img->data[3*((img->width*y)+x)];
	out.g = img->data[3*((img->width*y)+x)+1];
	out.b = img->data[3*((img->width*y)+x)+2];
	return out;
}

uint32_t difPixel(col A, col B){
	int r = (int)A.r-(int)B.r;
	int g = (int)A.g-(int)B.g;
	int b = (int)A.b-(int)B.b;
	return (r*r)+(g*g)+(b*b);
}

unsigned loadImage(image* img, const char* name){
	unsigned err = lodepng_decode24_file(&(img->data), &(img->width), &(img->height), name);
	printf("loaded image of size %dx%d\n",img->width,img->height);
	return err;
}
void freeImage(image* img){
	img->width=img->height=0;
	free(img->data);
}