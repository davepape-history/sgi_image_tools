#include "image.h"

void HSVtoRGB(unsigned short hue,unsigned short sat,unsigned short val,
		unsigned short *red,unsigned short *green,unsigned short *blue)
{
 float r,g,b,h,s,v,f,p,q,t;
 int i;
 h = hue/255.0*360.0;
 s = sat/255.0;
 v = val/255.0;
 if (s == 0.0) r = g = b = v;
 else {
	if (h == 360.0) h = 0.0;
	h /= 60.0;
	i = h;
	f = h-i;
	p = v*(1.0-s);
	q = v*(1.0-s*f);
	t = v*(1.0-s*(1.0-f));
	switch (i) {
		case 0: r = v;  g = t;  b = p; break;
		case 1: r = q;  g = v;  b = p; break;
		case 2: r = p;  g = v;  b = t; break;
		case 3: r = p;  g = q;  b = v; break;
		case 4: r = t;  g = p;  b = v; break;
		case 5: r = v;  g = p;  b = q; break;
		}
	}
 *red = r*255.0;
 *green = g*255.0;
 *blue = b*255.0;
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
 if (zsize<3) {
	fprintf(stderr,"Error: HSV image must have 3 bands\n");
	exit();
	}
 outimage = iopen(argv[2],"w",RLE(1),3,xsize,ysize,3);
 rbuf = (unsigned short *) malloc(xsize*sizeof(unsigned short));
 gbuf = (unsigned short *) malloc(xsize*sizeof(unsigned short));
 bbuf = (unsigned short *) malloc(xsize*sizeof(unsigned short));
 hbuf = (unsigned short *) malloc(xsize*sizeof(unsigned short));
 sbuf = (unsigned short *) malloc(xsize*sizeof(unsigned short));
 vbuf = (unsigned short *) malloc(xsize*sizeof(unsigned short));
 for (y=0; y<ysize; y++) {
	getrow(inimage,hbuf,y,0);
	getrow(inimage,sbuf,y,1);
	getrow(inimage,vbuf,y,2);
	for (x=0; x<xsize; x++)
		HSVtoRGB(hbuf[x],sbuf[x],vbuf[x],&rbuf[x],&gbuf[x],&bbuf[x]);
	putrow(outimage,rbuf,y,0);
	putrow(outimage,gbuf,y,1);
	putrow(outimage,bbuf,y,2);
	}
 iclose(outimage);
 iclose(inimage);
}
