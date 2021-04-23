#include <stdio.h>
#include <math.h>
#include "lib/dave.h"

UCHAR *clamp_image(int *sum,int x,int y)
{
 int i;
 UCHAR *ip,*img;
 int *sp;
 img = arralloc(UCHAR,x*y);
 for (i=x*y, ip=img, sp=sum; i; i--, ip++, sp++)
	if (*sp>255) *ip = 255;
	else if (*sp<0) *ip = 0;
	else *ip = *sp;
 return(img);
}

void add_image(UCHAR *img,int *sum,int x,int y,float fract)
{
 int i;
 for (i=x*y; i; i--, img++, sum++)
	*sum += fract * (*img);
}

main(int argc,char **argv)
{
 UCHAR *red,*green,*blue;
 int *rsum,*gsum,*bsum;
 int xdim,ydim,zdim,i,x,y,z,min,max,bpp;
 char type[256],storage[256];
 float fract;
 if (argc<4) {
	fprintf(stderr,"Usage: %s outimage inimage1 fraction1 ...\n",argv[0]);
	exit();
	}
 if (!read_sgi_image_header(argv[2],&xdim,&ydim,&zdim,&min,&max,&bpp,type,storage)) exit();
 rsum = arralloc(int,xdim*ydim);
 bzero(rsum,xdim*ydim*sizeof(int));
 if (zdim>1) {
	gsum = arralloc(int,xdim*ydim);
	bzero(gsum,xdim*ydim*sizeof(int));
	}
 if (zdim>2) {
	bsum = arralloc(int,xdim*ydim);
	bzero(bsum,xdim*ydim*sizeof(int));
	}
 for (i=2; i<argc; i+=2) {
	if (!read_sgi_image(argv[i],&red,&green,&blue,&x,&y,&z)) exit();
	fract = atof(argv[i+1]);
	if ((x!=xdim) || (y!=ydim) || (z!=zdim)) {
		fprintf(stderr,"Error: images must be same size\n");
		exit();
		}
	add_image(red,rsum,x,y,fract); free(red);
	if (zdim>1) add_image(green,gsum,x,y,fract), free(green);
	if (zdim>2) add_image(blue,bsum,x,y,fract), free(blue);
	}
 red = clamp_image(rsum,x,y);
 if (zdim>1) green = clamp_image(gsum,x,y);
 if (zdim>2) blue = clamp_image(bsum,x,y);
 write_sgi_image(argv[1],red,green,blue,xdim,ydim,zdim);
}
