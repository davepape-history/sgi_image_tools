#include "image.h"

main(argc,argv)
int argc;
char **argv;
{
 register IMAGE *inimage,*outimage;
 register int x, y, xsize, ysize;
 register int z, zsize;
 int bg[3],i,maxdist=0,d;
 unsigned short *buf[3],*outbuf;

 if (argc<4) {
	fprintf(stderr,"usage: %s inimage.sgi outmatte.sgi bg-r bg-g bg-b [dist]\n",
		argv[0]);
	exit(1);
    } 
 bg[0] = atoi(argv[3]);
 if (argc>4) bg[1] = atoi(argv[4]); else bg[1] = bg[0];
 if (argc>5) bg[2] = atoi(argv[5]); else bg[2] = bg[1];
 if (argc>6) maxdist = atoi(argv[6]);
 maxdist *= maxdist;
 if ((inimage=iopen(argv[1],"r")) == NULL) {
	fprintf(stderr,"%s: can't open input file %s\n",argv[0],argv[1]);
	exit(1);
	}
 xsize = inimage->xsize;
 ysize = inimage->ysize;
 zsize = inimage->zsize;
 if (zsize>3) zsize=3;
 outimage = iopen(argv[2],"w",RLE(1),3,xsize,ysize,1);
 for (i=0; i<zsize; i++)
	buf[i] = (unsigned short *) malloc(xsize*sizeof(unsigned short));
 outbuf = (unsigned short *) malloc(xsize*sizeof(unsigned short));
 for (y=0; y<ysize; y++) {
    for (z=0; z<zsize; z++) {
	getrow(inimage,buf[z],y,z);
	}
    for (x=0; x<xsize; x++) {
	d = 0;
	for (z=0; z<zsize; z++) d += (buf[z][x]-bg[z])*(buf[z][x]-bg[z]);
	if (d <= maxdist) outbuf[x] = 0;
	else outbuf[x] = 255;
	}
    putrow(outimage,outbuf,y,0);
    }
 iclose(outimage);
 iclose(inimage);
}
