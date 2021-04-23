#include "image.h"
#include <stdio.h>

main(int argc,char **argv)
{
 IMAGE *iimage, *oimage;
 int x, y, z, f;
 int xsize, ysize, zsize;
 short *outbuf,*inbuf;
 int *sum,*count;

 if (argc<3) {
	fprintf(stderr,"usage: %s outimage inimage ...\n",argv[0]);
	exit(1);
	} 
 if ((iimage=iopen(argv[2],"r")) == NULL) {
	fprintf(stderr,"%s: can't open input file %s\n",argv[0],argv[1]);
	exit(1);
	}
 xsize = iimage->xsize;
 ysize = iimage->ysize;
 zsize = iimage->zsize;
 oimage = iopen(argv[1],"w",RLE(1),iimage->dim,xsize,ysize,zsize); 
 iclose(iimage);
 outbuf = (short *) malloc(xsize*sizeof(short));
 inbuf = (short *) malloc(xsize*sizeof(short));
 sum = (int *) malloc(xsize*sizeof(int));
 count = (int *) malloc(xsize*sizeof(int));
 for (z=0; z<zsize; z++)
    for (y=0; y<ysize; y++) {
	bzero(sum,xsize*sizeof(int));
	bzero(count,xsize*sizeof(int));
	for (f = 2; f<argc; f++) {
	    if ((iimage=iopen(argv[f],"r")) == NULL) {
		fprintf(stderr,"%s: can't open input file %s\n",argv[0],argv[1]);
		exit(1);
		}
	    getrow(iimage,inbuf,y,z);
	    for (x=0; x<xsize; x++)
		if (inbuf[x]) {
			count[x]++;
			sum[x] += inbuf[x];
			}
	    iclose(iimage);
	    }
	for (x=0; x<xsize; x++)
	    if (count[x]) outbuf[x] = sum[x] / count[x];
	    else outbuf[x] = 0;
	putrow(oimage,outbuf,y,z);
	}
 iclose(oimage);
}
