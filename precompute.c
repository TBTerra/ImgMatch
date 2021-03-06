#include <stdio.h>
#include <stdlib.h>
#include "image.h"

int main(int argc, char* argv[]){
	if(argc < 2){
		printf("usage: precompute <upto> [factor]");
		return -1;
	}
	//work out how far we are going and how much we are scaling down
	int finish = atoi(argv[1]);
	int scale = 4;
	if(argc==3){
		scale = atoi(argv[2]);
	}
	char Cname[100];
	image img;
	imageB imgBW;
	for(int i=1;i<=finish;i++){
		printf("\r%04d",i);
		sprintf(Cname,"data/%04d.png",i);
		if(loadImage(&img, Cname)){
			continue;
		}
		downsampleImage(&img,scale);
		if(isColor(&img)){
			sprintf(Cname,"dataC/%04d.png",i);
			saveImage(&img, Cname);
		}
		downsampleImageBW(&img,&imgBW,1);
		sprintf(Cname,"dataB/%04d.png",i);
		saveImageBW(&imgBW, Cname);
		freeImage(&img);
		freeImage((image*)&imgBW);
	}
	return 0;
}