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
	sprintf(Cname,"%dC.dat",scale);
	FILE* f = fopen(Cname, "wb");
	sprintf(Cname,"%dB.dat",scale);
	FILE* fBW = fopen(Cname, "wb");
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
			fwrite(&i,4,1,f);
			fwrite(&img.width,4,1,f);
			fwrite(&img.height,4,1,f);
			fwrite(img.data,1,img.width*img.height*3,f);
		}
		downsampleImageBW(&img,&imgBW,1);
		fwrite(&i,4,1,fBW);
		fwrite(&imgBW.width,4,1,fBW);
		fwrite(&imgBW.height,4,1,fBW);
		fwrite(imgBW.data,1,imgBW.width*imgBW.height,fBW);
		freeImage(&img);
		freeImage((image*)&imgBW);
	}
	fclose(f);
	fclose(fBW);
	return 0;
}