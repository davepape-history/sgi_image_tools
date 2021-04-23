#include "image.h"
#include <stdio.h>

short rowbuf2[8192];
short alpha[8192];

main(argc,argv)
int argc;
char **argv;
{
    register IMAGE *iimage2, *iimage3, *oimage;
    register int y, z;
    int xsize1, ysize1, zsize;
    int xsize2, ysize2;
    int ozsize;
    int ysrc;
    int ucolor[3];

    if( argc<7 ) {
	fprintf(stderr,"usage: %s image.rgb under.alpha outimage red green blue\n",argv[0]);
	exit(1);
    } 
    if( (iimage2=iopen(argv[1],"r")) == NULL ) {
	fprintf(stderr,"%s: can't open input file %s\n",argv[0],argv[1]);
	exit(1);
    }
    if( (iimage3=iopen(argv[2],"r")) == NULL ) {
	fprintf(stderr,"%s: can't open input file %s\n",argv[0],argv[2]);
	exit(1);
    }
    xsize1 = iimage2->xsize;
    ysize1 = iimage2->ysize;
    xsize2 = iimage2->xsize;
    ysize2 = iimage2->ysize;
    zsize = iimage2->zsize;
    if ((xsize2 != iimage3->xsize) || (ysize2 != iimage3->ysize)) {
	fprintf(stderr,"%s: image.rgb and under.alpha must be same size\n",argv[0]);
	exit(1);
	}
    ucolor[0] = atoi(argv[4]);
    ucolor[1] = atoi(argv[5]);
    ucolor[2] = atoi(argv[6]);
    ozsize = iimage2->zsize;
    if (ozsize>3) ozsize = 3;
    oimage = iopen(argv[3],"w",RLE(BPP(iimage2->type)),
					iimage2->dim,xsize1,ysize1,ozsize); 
    isetname(oimage,iimage2->name);
    for (z=0; z<ozsize; z++)
	for (y=0; y<ysize1; y++) {
	    getrow(iimage2,rowbuf2,y,z);
	    getrow(iimage3,alpha,y,0);
	    alphaoverrow(ucolor[z],rowbuf2,xsize2,alpha);
	    putrow(oimage,rowbuf2,y,z);
	    }
    iclose(oimage);
}

alphaoverrow(ucolor,buf2,n2,alpha)
register int ucolor;
register short *buf2;
int n2;
register short *alpha;
{
    short a;
    while (n2--) {
	a = *alpha++;
	if (a == 0) {
	    *buf2++ = ucolor;
	    }
	else if (a >= 255) {
	    buf2++;
	    }
	else {
	    *buf2 = ((a * *buf2) + ((255-a) * ucolor))/255;
	    buf2++;
	    }
	}
}
