#include <stdio.h>
#include <math.h>
#include "dave.h"

#define TYPE short

TYPE interp(TYPE *c,float t)
{
 float val;
 TYPE rval;
 val = -1.0*c[0] + 3.0*c[1] - 3.0*c[2] + c[3];
 val = val*t + 2.0*c[0] - 5.0*c[1] + 4.0*c[2] - c[3];
 val = val*t - c[0] + c[2];
 val = val*t + 2.0*c[1];
 val /= 2.0;
 rval = val;
 return(rval);
}

TYPE interp_0(TYPE *c,float t)
{
 float val;
 val = -1.0*c[0] + 3.0*c[0] - 3.0*c[1] + c[2];
 val = val*t + 2.0*c[0] - 5.0*c[0] + 4.0*c[1] - c[2];
 val = val*t - c[0] + c[1];
 val = val*t + 2.0*c[0];
 val /= 2.0;
 return((TYPE)val);
}

TYPE interp_last(TYPE *c,float t)
{
 float val;
 val = -1.0*c[0] + 3.0*c[1] - 3.0*c[2] + c[2];
 val = val*t + 2.0*c[0] - 5.0*c[1] + 4.0*c[2] - c[2];
 val = val*t - c[0] + c[2];
 val = val*t + 2.0*c[1];
 val /= 2.0;
 return((TYPE)val);
}

void spline(TYPE *c,int nc,TYPE *out,int len)
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

zoom(TYPE *in,TYPE *out,int xdim,int ydim,int outx,int outy)
{
 TYPE *tmp,*icol,*ocol;
 int x,y;
 tmp = arralloc(TYPE,outx*ydim);
 for (y=0; y<ydim; y++)
	spline(in+y*xdim,xdim,tmp+y*outx,outx);
 icol = arralloc(TYPE,ydim);
 ocol = arralloc(TYPE,outy);
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
 TYPE *ibuf,*obuf;
 int inx,iny,outx,outy;
 if (argc<7) {
	fprintf(stderr,"Usage: %s inimage outimage inx iny outx outy\n",argv[0]);
	exit();
	}
 inx = atoi(argv[3]);
 iny = atoi(argv[4]);
 outx = atoi(argv[5]);
 outy = atoi(argv[6]);
 ibuf = arralloc(TYPE,inx*iny);
 obuf = arralloc(TYPE,outx*outy);
 if (!read_bin_file(argv[1],(UCHAR *)ibuf,inx*iny*sizeof(TYPE))) exit();
 zoom(ibuf,obuf,inx,iny,outx,outy);
 write_bin_file(argv[2],(UCHAR *)obuf,outx*outy*sizeof(TYPE));
}
