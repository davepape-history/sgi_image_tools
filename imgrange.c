#include <stdio.h>
#include "dave.h"

void remap(UCHAR *img,int min,int max,int newmin,int newmax,int size);

main(int argc,char **argv)
{
 UCHAR *r,*g,*b;
 int xdim,ydim,zdim,min,max,bpp,newmin,newmax;
 char type[64],storage[64];
 if (argc<5) {
	fprintf(stderr,"Usage: %s in-image out-image new-min new-max\n",argv[0]);
	exit();
	}
 newmin = atoi(argv[3]);
 newmax = atoi(argv[4]);
 if (newmin>newmax) {
	fprintf(stderr,"Error: min must be less than max\n");
	exit();
	}
 if (!dpReadSgiImageHeader(argv[1],&xdim,&ydim,&zdim,&min,&max,&bpp,
				type,storage))  exit();
 if (!dpReadSgiImage(argv[1],&r,&g,&b,&xdim,&ydim,&zdim)) exit();
 remap(r,min,max,newmin,newmax,xdim*ydim);
 if (zdim>1) remap(g,min,max,newmin,newmax,xdim*ydim);
 if (zdim>2) remap(b,min,max,newmin,newmax,xdim*ydim);
 dpWriteSgiImage(argv[2],r,g,b,xdim,ydim,zdim);
}

void remap(UCHAR *img,int min,int max,int newmin,int newmax,int size)
{
 int i;
 float scale;
 UCHAR *p;
 if (max==min) scale = 0.0;
 else scale = ((float)(newmax-newmin))/((float)(max-min));
 for (i=0, p=img; i<size; i++, p++)
	*p = ((*p - min)*scale+0.5) + newmin;
}
