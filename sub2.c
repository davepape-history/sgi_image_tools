#include "image.h"
#include <stdio.h>

#define MAX(a, b)       (((a) > (b)) ? (a) : (b))

short rowbuf1[16384];
short rowbuf2[16384];

main(argc,argv)
int argc;
char **argv;
{
 register IMAGE *iimage1, *iimage2, *oimage;
 register int y, z;
 int xsize, ysize, zsize1, zsize2;
 int ozsize;

 if (argc<4) {
	fprintf(stderr,"usage: %s inimage1 inimage2 outimage\n",argv[0]);
	exit(1);
	} 
 if ((iimage1=iopen(argv[1],"r")) == NULL) {
	fprintf(stderr,"%s: can't open input file %s\n",argv[0],argv[1]);
	exit(1);
	}
 if( (iimage2=iopen(argv[2],"r")) == NULL ) {
	fprintf(stderr,"%s: can't open input file %s\n",argv[0],argv[2]);
	exit(1);
 }
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
	    subrow(rowbuf1,rowbuf2,xsize);
	    putrow(oimage,rowbuf2,y,z);
	}
 iclose(oimage);
}

subrow(buf1,buf2,n)
register short *buf1;
register short *buf2;
register int n;
{
 register int val;

 while (n--) {
	val = (int)*buf1 - *buf2;
	if (val< 0) val = 0;
	*buf2 = val;
	buf1++;
	buf2++;
 }
}
