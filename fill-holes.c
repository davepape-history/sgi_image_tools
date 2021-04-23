#include <stdio.h>
#include "dave.h"

UCHAR fill(int x,int y,UCHAR *img,int xsize,int ysize)
{
 int sum,count,p;
 sum = count = 0;
 p = x+y*xsize;
 if ((x) && (img[p-1])) { sum += img[p-1]; count++; }
 if ((x<xsize-1) && (img[p+1])) { sum += img[p+1]; count++; }
 if ((y) && (img[p-xsize])) { sum += img[p-xsize]; count++; }
 if ((y<ysize-1) && (img[p+xsize])) { sum += img[p+xsize]; count++; }
 if (!count) return(0);
 else return(sum/count);
}

main(argc,argv)
int argc;
char **argv;
{
 UCHAR *inimg,*outimg,*tmp;
 int x, y, xsize, ysize, i, npasses=1;
 if (argc<3) {
	fprintf(stderr,"usage: %s inimage.bw outimage.bw [num-passes]\n",
		argv[0]);
	exit(1);
	}
 if (argc>3) npasses = atoi(argv[3]);
 if (!dpReadSgiImageByte(argv[1],&inimg,&xsize,&ysize)) { 
	fprintf(stderr,"%s: can't open input file %s\n",argv[0],argv[1]);
	exit(1);
	}
 outimg = (UCHAR *) malloc(xsize*ysize*sizeof(UCHAR));
 while (npasses--) {
	for (y=0, i=0; y<ysize; y++) {
	    for (x=0; x<xsize; x++, i++) {
		if (inimg[i]) outimg[i] = inimg[i];
		else outimg[i] = fill(x,y,inimg,xsize,ysize);
		}
	    }
	if (npasses) {
		tmp = inimg; inimg = outimg; outimg = tmp;
		}
	}
 dpWriteSgiImageByte(argv[2],outimg,xsize,ysize);
}
