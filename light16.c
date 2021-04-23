#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <math.h>
#include "image.h"

#define SPACING 100.0


int light3(short v0,short v1,short v2)
	{
	float diff1,diff2,length,intensity;
	diff1 = v1 - v0;
	diff2 = v2 - v0;
	length = sqrtf(diff1*diff1 + diff2*diff2 + SPACING*SPACING);
	intensity = (0.5774 * diff1 + 0.5774 * diff2 + 0.5774 * SPACING) / length;
	if (intensity < 0)
		return 0;
	else if (intensity > 1)
		return 255;
	else
		return (int)(intensity * 255);
	}


main(int argc,char **argv)
	{
	IMAGE *outimage;
	int x, y, xsize, ysize;
	unsigned short *outbuf;
	short *inbuf1, *inbuf2, *tmp;
	int inFD;
	if (argc < 5)
		{
		fprintf(stderr,"usage: %s inimage.16 outimage.bw xdim ydim\n",argv[0]);
		exit(1);
		}
	xsize = atoi(argv[3]);
	ysize = atoi(argv[4]);
	if ((inFD=open(argv[1],O_RDONLY)) == -1)
		{
		perror(argv[1]);
		exit(1);
		}
	outimage = iopen(argv[2],"w",RLE(1),3,xsize,ysize,1);
	outbuf = (unsigned short *) malloc(xsize*sizeof(unsigned short));
	inbuf1 = (short *) malloc(xsize*sizeof(short));
	inbuf2 = (short *) malloc(xsize*sizeof(short));
	read(inFD,inbuf1,xsize*sizeof(short));
	for (y=0; y < ysize; y++)
		{
		if (y < ysize - 1)
			read(inFD,inbuf2,xsize*sizeof(short));
		outbuf[0] = light3(inbuf1[0],inbuf1[1],inbuf2[0]);
		for (x=1; x < xsize; x++)
			outbuf[x] = light3(inbuf1[x],inbuf1[x-1],inbuf2[x]);
		putrow(outimage,outbuf,y,0);
		tmp = inbuf1;
		inbuf1 = inbuf2;
		inbuf2 = tmp;
		}
	iclose(outimage);
	}


#if 0

int light2(short v0,short v1)
	{
	float diff,length,intensity;
	diff = v1 - v0;
	length = sqrtf(diff*diff + SPACING*SPACING);
	intensity = (0.707 * diff + 0.707 * SPACING) / length;
	if (intensity < 0)
		return 0;
	else if (intensity > 1)
		return 255;
	else
		return (int)(intensity * 255);
	}

main(int argc,char **argv)
	{
	IMAGE *outimage;
	int x, y, xsize, ysize;
	unsigned short *outbuf;
	short *inbuf;
	int inFD;
	if (argc < 5)
		{
		fprintf(stderr,"usage: %s inimage.16 outimage.bw xdim ydim\n",argv[0]);
		exit(1);
		}
	xsize = atoi(argv[3]);
	ysize = atoi(argv[4]);
	if ((inFD=open(argv[1],O_RDONLY)) == -1)
		{
		perror(argv[1]);
		exit(1);
		}
	outimage = iopen(argv[2],"w",RLE(1),3,xsize,ysize,1);
	outbuf = (unsigned short *) malloc(xsize*sizeof(unsigned short));
	inbuf = (short *) malloc(xsize*sizeof(short));
	for (y=0; y < ysize; y++)
		{
		read(inFD,inbuf,xsize*sizeof(short));
		outbuf[0] = light2(inbuf[0],inbuf[1]);
		for (x=1; x < xsize; x++)
			outbuf[x] = (short) light2(inbuf[x-1],inbuf[x]);
		putrow(outimage,outbuf,y,0);
		}
	iclose(outimage);
	}
#endif
