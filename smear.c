#include <stdio.h>
#include <math.h>
#include "dave.h"

ULONG interp_point(float x,float y,ULONG *img,int xdim,int ydim)
{
 int ix,iy,v0,v1,v2,v3,va,vb;
 UCHAR r,g,b;
 float dx,dy;
 ix = x;      iy = y;
 dx = x-ix;   dy = y-iy;

 v0 = img[ix+iy*xdim] & 0xff;
 v1 = img[ix+1+iy*xdim] & 0xff;
 v2 = img[ix+(iy+1)*xdim] & 0xff;
 v3 = img[ix+1+(iy+1)*xdim] & 0xff;
 va = v0+(v1-v0)*dx;
 vb = v2+(v3-v2)*dx;
 r = va+(vb-va)*dy;

 v0 = (img[ix+iy*xdim] >> 8) & 0xff;
 v1 = (img[ix+1+iy*xdim] >> 8) & 0xff;
 v2 = (img[ix+(iy+1)*xdim] >> 8) & 0xff;
 v3 = (img[ix+1+(iy+1)*xdim] >> 8) & 0xff;
 va = v0+(v1-v0)*dx;
 vb = v2+(v3-v2)*dx;
 g = va+(vb-va)*dy;

 v0 = (img[ix+iy*xdim] >> 16) & 0xff;
 v1 = (img[ix+1+iy*xdim] >> 16) & 0xff;
 v2 = (img[ix+(iy+1)*xdim] >> 16) & 0xff;
 v3 = (img[ix+1+(iy+1)*xdim] >> 16) & 0xff;
 va = v0+(v1-v0)*dx;
 vb = v2+(v3-v2)*dx;
 b = va+(vb-va)*dy;

 return(r | g<<8 | b<<16);
}

main(int argc,char **argv)
{
 ULONG *in,*out,*op;
 UCHAR *sdat,*sp;
 int xdim,ydim,i,j,ix,iy;
 float scale=1.0,dx,dy,sx,sy;
 if (argc<4) {
	fprintf(stderr,"Usage: %s inimage smearimage outimage [scale]\n",argv[0]);
	exit();
	}
 if (argc>4) scale = atof(argv[4]);
 if (scale==0.0) scale=1.0;
 if (!dpReadSgiImageLong(argv[1],&in,&xdim,&ydim)) exit();
 if (!dpReadSgiImageByte(argv[2],&sdat,&i,&j)) exit();
 if ((i!=xdim) || (j!=ydim)) {
	fprintf(stderr,"Error: images must be same size\n");
	exit();
	}
 out = (ULONG *) malloc(xdim*ydim*sizeof(ULONG));
 bzero(out,xdim*4);
 op = out;
 sp = sdat;
 for (j=0; j<ydim; j++)
    for (i=0; i<xdim; i++, op++, sp++) {
	dx = ((int)*(sp-1) - *(sp+1))*scale;
	dy = ((int)*(sp-xdim) - *(sp+xdim))*scale;
	sx = i+dx;
	while (sx>=xdim) sx-=xdim;
	while (sx<0) sx+=xdim;
	sy = j+dy;
	while (sy>=ydim) sy-=ydim;
	while (sy<0) sy+=ydim;
	*op = interp_point(sx,sy,in,xdim,ydim);
	}
 dpWriteSgiImageLong(argv[3],out,xdim,ydim);
}
