#include "image.h"

void RGBtoHSV(unsigned short red,unsigned short green,unsigned short blue,
		unsigned short *hue,unsigned short *sat,unsigned short *val)
{
 float r,g,b,h,s,v,min,max,delta;
 r = red/255.0;
 g = green/255.0;
 b = blue/255.0;
 if (r>g) if (r>b) max = r; else max = b;
 else if (g>b) max = g; else max = b;
 if (r<g) if (r<b) min = r; else min = b;
 else if (g<b) min = g; else min = b;
 v = max;
 if (max != 0.0) s = (max-min)/max;
 else s = 0.0;
 if (s == 0.0) h = 0.0;
 else {
	delta = max-min;
	if (r == max) h = (g-b)/delta;
	else if (g == max) h = 2+(b-r)/delta;
	else h = 4+(r-g)/delta;
	h = h*60.0;
	if (h<0) h += 360.0;
	}
 *hue = h*255.0/360.0;
 *sat = s*255.0;
 *val = v*255.0;
}

main(argc,argv)
int argc;
char **argv;
{
 register IMAGE *inimage,*outimage;
 register int x, y, xsize, ysize;
 register int z, zsize;
 unsigned short *rbuf,*gbuf,*bbuf,*hbuf,*sbuf,*vbuf;

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
 outimage = iopen(argv[2],"w",RLE(1),3,xsize,ysize,3);
 rbuf = (unsigned short *) malloc(xsize*sizeof(unsigned short));
 if (zsize>1) {
	gbuf = (unsigned short *) malloc(xsize*sizeof(unsigned short));
	bbuf = (unsigned short *) malloc(xsize*sizeof(unsigned short));
	}
 else gbuf = bbuf = rbuf;
 hbuf = (unsigned short *) malloc(xsize*sizeof(unsigned short));
 sbuf = (unsigned short *) malloc(xsize*sizeof(unsigned short));
 vbuf = (unsigned short *) malloc(xsize*sizeof(unsigned short));
 for (y=0; y<ysize; y++) {
	getrow(inimage,rbuf,y,0);
	if (zsize > 1) {
		getrow(inimage,gbuf,y,1);
		getrow(inimage,bbuf,y,2);
		}
	for (x=0; x<xsize; x++)
		RGBtoHSV(rbuf[x],gbuf[x],bbuf[x],&hbuf[x],&sbuf[x],&vbuf[x]);
	putrow(outimage,hbuf,y,0);
	putrow(outimage,sbuf,y,1);
	putrow(outimage,vbuf,y,2);
	}
 iclose(outimage);
 iclose(inimage);
}
