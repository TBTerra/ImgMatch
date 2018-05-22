#include <stdio.h>
#include <stdint.h>
#include "image.h"

#define SKIP 8
#define SAMPLE 4
#define THRESHOLD 4000000

uint32_t findMin(image* imgS, image* imgL, point* where);
uint32_t getScore(image* imgS, image* imgL, int offX, int offY);
uint32_t difPixel(col A, col B);

//there should be 2 arguments (for now)
//1st argument: the file with a section of image
//2nd argument: the file of the full image to test against
//in future if the 2nd argument is missing then it will match against all files in the data dir

int main(int argc, char* argv[]){
	if(argc < 2){
		printf("usage: match <small img> [image to match against]");
		return -1;
	}
	
	image smallI;
	image largeI;
	loadImage(&smallI, argv[1]);
	if(argc == 2){
		uint32_t best = -1;
		uint32_t best2 = -1;
		int bestI = -1;
		int bestI2 = -1;
		char Cname[100];
		for(int i=1;i<=2497;i++){
			printf("\r%04d",i);
			sprintf(Cname,"data/%04d.png",i);
			if(loadImage(&largeI, Cname)){
				continue;
			}
			uint32_t score = findMin(&smallI,&largeI,NULL);
			if(score<best){
				best2 = best;
				bestI2 = bestI;
				best = score;
				bestI = i;
			}
			freeImage(&largeI);
			if(best<THRESHOLD){
				printf("early exit\n");
				break;
			}
		}
		printf("best match was %d with a score of %u\nNext best was %d with a score of %u",bestI,best,bestI2,best2);
	}else{
		loadImage(&largeI, argv[2]);
		point where;
		uint32_t best = findMin(&smallI,&largeI, &where);
		printf("best score was %u at [%d,%d]",best,where.x,where.y);
	}
	
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
	for(int j=0;j<=scanH;j+=SKIP){
		for(int i=0;i<=scanW;i+=SKIP){
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

uint32_t getScore(image* imgS, image* imgL, int offX, int offY){
	uint32_t score = 0;
	for(int j=0;j<imgS->height;j+=SAMPLE){
		for(int i=0;i<imgS->width;i+=SAMPLE){
			col S = getPixel(imgS, i, j);
			col L = getPixel(imgL, i+offX, j+offY);
			score += difPixel(S,L);
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