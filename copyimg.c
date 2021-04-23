#include "image.h"

main(argc,argv)
int argc;
char **argv;
{
 register IMAGE *inimage,*outimage;
 register int y, xsize, ysize;
 register int z, zsize;
 unsigned short *buf;

 if (argc<3) {
	fprintf(stderr,"usage: %s inimage.sgi outimage.sgi\n",argv[0]);
	exit(1);
    } 
 if ((inimage=iopen(argv[1],"r")) == NULL) {
	fprintf(stderr,"%s: can't open input file %s\n",argv[0],argv[1]);
	exit(1);
	}
 xsize = inimage->xsize;
 ysize = inimage->ysize;
 zsize = inimage->zsize;
 outimage = iopen(argv[2],"w",RLE(1),zsize,xsize,ysize,zsize);
 buf = (unsigned short *) malloc(xsize*sizeof(unsigned short));
 for (y=0; y<ysize; y++)
    for (z=0; z<zsize; z++) {
	getrow(inimage,buf,y,z);
	putrow(outimage,buf,y,z);
	}
 iclose(outimage);
 iclose(inimage);
}
