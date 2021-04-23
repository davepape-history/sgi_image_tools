/*
 *	addover - 
 *		Add one (smaller) image to another image.
 *		Code taken from "over.c"
 *
 */
#include "image.h"
#include <stdio.h>

main(argc,argv)
int argc;
char **argv;
{
    register IMAGE *iimage1, *iimage2, *oimage;
    register int y, z;
    int xsize1, ysize1, zsize, maxx;
    int xsize2, ysize2;
    int ozsize;
    int xorg, yorg;
    int ysrc;
 short *rowbuf1,*rowbuf2;

    if ((argc!=4) && (argc!=6)) {
	fprintf(stderr,"usage: addover underimage overimage outimage [xpos ypos]\n");
	exit(1);
    } 
    if( (iimage1=iopen(argv[1],"r")) == NULL ) {
	fprintf(stderr,"addover: can't open input file %s\n",argv[1]);
	exit(1);
    }
    if( (iimage2=iopen(argv[2],"r")) == NULL ) {
	fprintf(stderr,"addover: can't open input file %s\n",argv[2]);
	exit(1);
    }
    xsize1 = iimage1->xsize;
    ysize1 = iimage1->ysize;
    xsize2 = iimage2->xsize;
    ysize2 = iimage2->ysize;
    zsize = iimage2->zsize;
    if (argc>4) xorg = atoi(argv[4]); else xorg = 0;
    if (argc>5) yorg = atoi(argv[5]); else yorg = 0;
    ozsize = iimage1->zsize;
    if (ozsize>3) ozsize = 3;
    oimage = iopen(argv[3],"w",RLE(BPP(iimage1->type)),
					iimage1->dim,xsize1,ysize1,ozsize); 
    isetname(oimage,iimage1->name);
 if (xsize1>xsize2) maxx = xsize1;
 else maxx = xsize2;
 rowbuf1 = (short *) malloc(maxx*sizeof(short));
 rowbuf2 = (short *) malloc(maxx*sizeof(short));
    for (z=0; z<ozsize; z++)
	for (y=0; y<ysize1; y++) {
	    getrow(iimage1,rowbuf1,y,z);
	    ysrc = y-yorg;
	    if (ysrc>=0 && ysrc<iimage2->ysize) {
		    if (zsize == 1)
			getrow(iimage2,rowbuf2,ysrc,0);
		    else
			getrow(iimage2,rowbuf2,ysrc,z);
		    addoverrow(rowbuf1,xsize1,rowbuf2,xsize2,xorg);
	    	    }
	    putrow(oimage,rowbuf1,y,z);
	    }
    iclose(oimage);
}

addoverrow(buf1,n1,buf2,n2,xorg)
register short *buf1;
int n1;
register short *buf2;
int n2;
int xorg;
{
    short a;

    if((n2+xorg)>n1) 
	n2 = n1-xorg; 
    buf1+=xorg;
    while (n2--)
	*buf1++ += *buf2++;
}
