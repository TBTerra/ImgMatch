#include <stdio.h>
#include <conio.h>
#include <time.h>
#include "image.h"

uint32_t findMin(image* imgS, image* imgL, point* where);
uint32_t getScore(image* imgS, image* imgL, int offX, int offY);
uint32_t findMinB(image* imgS, image* imgL, point* where);
uint32_t getScoreB(image* imgS, image* imgL, int offX, int offY);
uint32_t difPixel(col A, col B);
int nextImg(image* img, FILE* f, int bytes);

int main(){
	int start = clock();
	image smallI;
	image largeI;
	loadImage(&smallI, "image.png");
	uint32_t best = -1;
	uint32_t best2 = -1;
	int bestI = -1;
	int bestI2 = -1;
	if(isColor(&smallI)){
		downsampleImage(&smallI, 8);
		FILE* f = fopen("8C.dat","rb");
		setvbuf(f, NULL, _IOFBF, 1000000);
		uint32_t num;
		while(num = nextImg(&largeI, f, 3)){
			uint32_t score = findMin(&smallI,&largeI,NULL);
			if(score<best){
				best2 = best;
				bestI2 = bestI;
				best = score;
				bestI = num;
			}
			freeImage(&largeI);
		}
	}else{
		downsampleImageBW(&smallI, (imageB*)&smallI, 8);
		FILE* f = fopen("8B.dat","rb");
		setvbuf(f, NULL, _IOFBF, 1000000);
		uint32_t num;
		while(num = nextImg(&largeI, f, 1)){
			uint32_t score = findMinB(&smallI,&largeI,NULL);
			if(score<best){
				best2 = best;
				bestI2 = bestI;
				best = score;
				bestI = num;
			}
			freeImage(&largeI);
		}
	}
	int stop = clock();
	printf("best match was %d with a score of %u\nNext best was %d with a score of %u\n",bestI,best,bestI2,best2);
	printf("took %d ms to do\n",stop-start);
	return 0;
}

uint32_t findMin(image* imgS, image* imgL, point* where){
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
	if(where!=NULL){
		where->x = bX;
		where->y = bY;
	}
	return best;
}

uint32_t findMinB(image* imgS, image* imgL, point* where){
	int32_t scanW = imgL->width - imgS->width;
	int32_t scanH = imgL->height - imgS->height;
	if((scanW <= 0) || (scanH <= 0)){
		printf("cant work if small image is larger than large image");
		return -1;
	}
	if((imgS->width*imgS->height)>65000){
		printf("cant guarantee this is gonna work. image too large, may cause int overflow");
		return -1;
	}
	uint32_t best = -1;
	int bX=-1;int bY=-1;
	for(int j=0;j<=scanH;j++){
		for(int i=0;i<=scanW;i++){
			int score = getScoreB(imgS, imgL, i,j);
			if(score < best){
				best = score;
				bX=j;bY=i;
			}
		}
	}
	if(where!=NULL){
		where->x = bX;
		where->y = bY;
	}
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

uint32_t getScoreB(image* imgS, image* imgL, int offX, int offY){
	uint32_t score = 0;
	for(int j=0;j<imgS->height;j++){
		for(int i=0;i<imgS->width;i++){
			int S = imgS->data[((imgS->width*j)+i)];
			int L = imgL->data[((imgL->width*(j+offY))+i+offX)];
			score += (S-L)*(S-L);
		}
	}
	//printf("score %u at [%d,%d]\n",score,offX,offY);
	return score;
}

uint32_t difPixel(col A, col B){
	int r = (int)A.r-(int)B.r;
	int g = (int)A.g-(int)B.g;
	int b = (int)A.b-(int)B.b;
	return (r*r)+(g*g)+(b*b);
}

int nextImg(image* img, FILE* f, int bytes){
	uint32_t num=0;
	if(fread(&num,4,1,f)==0){
		return 0;
	}
	fread(&(img->width),4,1,f);
	fread(&(img->height),4,1,f);
	img->data = malloc(img->width*img->height*bytes);
	fread(img->data,1,img->width*img->height*bytes,f);
	return num;
}