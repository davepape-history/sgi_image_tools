#include <stdio.h>
#include <math.h>
#include "dave.h"

UCHAR interp(UCHAR *c,float t)
{
 float val;
 UCHAR bval;
 val = -1.0*c[0] + 3.0*c[1] - 3.0*c[2] + c[3];
 val = val*t + 2.0*c[0] - 5.0*c[1] + 4.0*c[2] - c[3];
 val = val*t - c[0] + c[2];
 val = val*t + 2.0*c[1];
 val /= 2.0;
 if (val < 0.0) return(0);
 else if (val>255.0) return(255);
 bval = val;
 return(bval);
}

UCHAR interp_0(UCHAR *c,float t)
{
 float val;
 val = -1.0*c[0] + 3.0*c[0] - 3.0*c[1] + c[2];
 val = val*t + 2.0*c[0] - 5.0*c[0] + 4.0*c[1] - c[2];
 val = val*t - c[0] + c[1];
 val = val*t + 2.0*c[0];
 val /= 2.0;
 if (val < 0.0) return(0);
 else if (val>255.0) return(255);
 else return((UCHAR)val);
}

UCHAR interp_last(UCHAR *c,float t)
{
 float val;
 val = -1.0*c[0] + 3.0*c[1] - 3.0*c[2] + c[2];
 val = val*t + 2.0*c[0] - 5.0*c[1] + 4.0*c[2] - c[2];
 val = val*t - c[0] + c[2];
 val = val*t + 2.0*c[1];
 val /= 2.0;
 if (val < 0.0) return(0);
 else if (val>255.0) return(255);
 else return((UCHAR)val);
}

void spline(UCHAR *c,int nc,UCHAR *out,int len)
{
 int i,it;
 float t,max;
 max = nc - 1.0;
 for (i=0; i<len; i++) {
	t = (float)i/(len-1.0) * max;
	it = t;
	t -= it;
	if (it >= max) out[i] = c[(int)max];
	else if (it == 0) out[i] = interp_0(c,t);
	else if (it == nc-2) out[i] = interp_last(c+it-1,t);
	else out[i] = interp(c+it-1,t);
	}
}

zoom(UCHAR *in,UCHAR *out,int xdim,int ydim,int outx,int outy)
{
 UCHAR *tmp,*icol,*ocol;
 int x,y;
 tmp = arralloc(UCHAR,outx*ydim);
 for (y=0; y<ydim; y++)
	spline(in+y*xdim,xdim,tmp+y*outx,outx);
 icol = arralloc(UCHAR,ydim);
 ocol = arralloc(UCHAR,outy);
 for (x=0; x<outx; x++) {
	for (y=0; y<ydim; y++) icol[y] = tmp[x+y*outx];
	spline(icol,ydim,ocol,outy);
	for (y=0; y<outy; y++) out[x+y*outx] = ocol[y];
	}
 free(tmp);
 free(icol);
 free(ocol);
}

main(int argc,char **argv)
{
 UCHAR *ir,*ig,*ib,*or,*og,*ob;
 int xdim,ydim,zdim,outx,outy;
 float xzoom,yzoom;
 if (argc<5) {
	fprintf(stderr,"Usage: %s inimage outimage xzoom yzoom\n",argv[0]);
	exit();
	}
 xzoom = atof(argv[3]);
 yzoom = atof(argv[4]);
 if ((xzoom <= 0.0) || (yzoom <= 0.0)) exit();
 if (!read_sgi_image(argv[1],&ir,&ig,&ib,&xdim,&ydim,&zdim)) exit();
 outx = xdim * xzoom + 0.5;
 outy = ydim * yzoom + 0.5;
 or = arralloc(UCHAR,outx*outy);
 if (zdim>1) og = arralloc(UCHAR,outx*outy);
 if (zdim>2) ob = arralloc(UCHAR,outx*outy);
 zoom(ir,or,xdim,ydim,outx,outy);
 if (zdim>1) zoom(ig,og,xdim,ydim,outx,outy);
 if (zdim>2) zoom(ib,ob,xdim,ydim,outx,outy);
 write_sgi_image(argv[2],or,og,ob,outx,outy,zdim);
}
