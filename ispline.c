#include <stdio.h>
#include <math.h>
#include "dave.h"

void spline_image(UCHAR *i0,UCHAR *i1,UCHAR *i2,UCHAR *i3,UCHAR *out,float param,int size);

main(int argc,char **argv)
{
 UCHAR *r0,*r1,*r2,*r3,*g0,*g1,*g2,*g3,*b0,*b1,*b2,*b3;
 UCHAR *rout=NULL,*gout=NULL,*bout=NULL,*malloc();
 int xdim,ydim,zdim,x,y,z;
 float param;
 if (argc<7) {
	fprintf(stderr,"Usage: %s img0 img1 img2 img3 outimage param\n",argv[0]);
	exit();
	}
 if (!read_sgi_image(argv[1],&r0,&g0,&b0,&xdim,&ydim,&zdim)) exit();
 if (!read_sgi_image(argv[2],&r1,&g1,&b1,&x,&y,&z)) exit();
 if ((x!=xdim) || (y!=ydim) || (z!=zdim)) {
	fprintf(stderr,"Error: all images must have the same dimensions\n");
	exit();
	}
 if (!read_sgi_image(argv[3],&r2,&g2,&b2,&x,&y,&z)) exit();
 if ((x!=xdim) || (y!=ydim) || (z!=zdim)) {
	fprintf(stderr,"Error: all images must have the same dimensions\n");
	exit();
	}
 if (!read_sgi_image(argv[4],&r3,&g3,&b3,&x,&y,&z)) exit();
 if ((x!=xdim) || (y!=ydim) || (z!=zdim)) {
	fprintf(stderr,"Error: all images must have the same dimensions\n");
	exit();
	}
 param = atof(argv[6]);
 rout = malloc(xdim*ydim);
 spline_image(r0,r1,r2,r3,rout,param,xdim*ydim);
 if (zdim>1) {
	gout = malloc(xdim*ydim);
	spline_image(g0,g1,g2,g3,gout,param,xdim*ydim);
	}
 if (zdim>2) {
	bout = malloc(xdim*ydim);
	spline_image(b0,b1,b2,b3,bout,param,xdim*ydim);
	}
 write_sgi_image(argv[5],rout,gout,bout,xdim,ydim,zdim);
}

void spline_image(UCHAR *i0,UCHAR *i1,UCHAR *i2,UCHAR *i3,UCHAR *out,float param,int size)
{
 float v0,v1,v2,v3;
 int i,val;
 v0 = -0.5*param*param*param + param*param - 0.5*param;
 v1 = 1.5*param*param*param - 2.5*param*param + 1.0;
 v2 = -1.5*param*param*param + 2.0*param*param + 0.5*param;
 v3 = 0.5*param*param*param - 0.5*param*param;
 for (i=0; i<size; i++, i0++, i1++, i2++, i3++, out++) {
	val = *i0*v0 + *i1*v1 + *i2*v2 + *i3*v3;
	if (val<0) *out = 0;
	else if (val>255) *out = 255;
	else *out = val;
	}
}
