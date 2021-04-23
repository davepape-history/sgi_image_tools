#include "image.h"
#include <stdio.h>

#define MAX(a, b)       (((a) > (b)) ? (a) : (b))

short rowbuf1[16384];
short rowbuf2[16384];

int scale=255;

main(argc,argv)
int argc;
char **argv;
{
 register IMAGE *iimage1, *iimage2, *oimage;
 register int y, z;
 int xsize, ysize, zsize1, zsize2;
 int ozsize;

 if (argc<4) {
	fprintf(stderr,"usage: %s band1.bw band2.bw out-ndvi.bw [scale]\n",argv[0]);
	exit(1);
	} 
 if ((iimage1=iopen(argv[1],"r")) == NULL) {
	fprintf(stderr,"%s: can't open input file %s\n",argv[0],argv[1]);
	exit(1);
	}
 if ((iimage2=iopen(argv[2],"r")) == NULL ) {
	fprintf(stderr,"%s: can't open input file %s\n",argv[0],argv[2]);
	exit(1);
	}
 if (argc>4) scale = atoi(argv[4]);
 xsize = iimage1->xsize;
 ysize = iimage1->ysize;
 zsize1 = iimage1->zsize;
 zsize2 = iimage2->zsize;
 if( (iimage2->xsize != xsize) || (iimage2->ysize != ysize) ) {
	fprintf(stderr,"%s: image dimensions must be equal\n",argv[0]);
	exit(1);
 }
 ozsize = MAX(zsize1,zsize2);
 oimage = iopen(argv[3],"w",RLE(1),iimage1->dim,xsize,ysize,ozsize); 
 isetname(oimage,iimage1->name);
 for(z=0; z<ozsize; z++)
	for(y=0; y<ysize; y++) {
	    getrow(iimage1,rowbuf1,y,z%zsize1);
	    getrow(iimage2,rowbuf2,y,z%zsize2);
	    ndvirow(rowbuf1,rowbuf2,xsize);
	    putrow(oimage,rowbuf2,y,z);
	}
 iclose(oimage);
}

ndvirow(buf1,buf2,n)
short *buf1;
short *buf2;
int n;
{
 int val,b1,b2;
 while (n--) {
	b1 = *buf1;
	b2 = *buf2;
	if ((b2+b1) == 0) val = 0;
	else val = (b2-b1)*scale/(b2+b1);
	if (val<0) val = 0;
	else if (val>255) val = 255;
	*buf2 = val;
	buf1++;
	buf2++;
 }
}
