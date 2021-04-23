#include <stdio.h>
#include <math.h>
#include "dave.h"

int compute_grad(UCHAR *img,int xdim,int up,int down,int left,int right);
UCHAR *scale_grad(int *grad,int size);

main(int argc,char **argv)
{
 UCHAR *in,*out,*ip;
 int *grad,*gp;
 int xdim,ydim,i,j;
 if (argc<3) {
	fprintf(stderr,"Usage: %s inimage outimage\n",argv[0]);
	exit();
	}
 if (!dpReadSgiImageByte(argv[1],&in,&xdim,&ydim)) exit();
 grad = (int *) malloc(xdim*ydim*sizeof(int));
 ip = in;
 gp = grad;
 *gp = compute_grad(ip,xdim,0,1,0,1);
 ip++, gp++;
 for (i=1; i<xdim-1; i++, ip++, gp++)
	*gp = compute_grad(ip,xdim,0,1,1,1);
 *gp = compute_grad(ip,xdim,0,1,1,0);
 ip++, gp++;
 for (j=1; j<ydim-1; j++) {
    *gp = compute_grad(ip,xdim,1,1,0,1);
    ip++, gp++;
    for (i=1; i<xdim-1; i++, ip++, gp++)
	*gp = compute_grad(ip,xdim,1,1,1,1);
    *gp = compute_grad(ip,xdim,1,1,1,0);
    ip++, gp++;
    }
 *gp = compute_grad(ip,xdim,1,0,0,1);
 ip++, gp++;
 for (i=1; i<xdim-1; i++, ip++, gp++)
	*gp = compute_grad(ip,xdim,1,0,1,1);
 *gp = compute_grad(ip,xdim,1,0,1,0);
 out = scale_grad(grad,xdim*ydim);
 dpWriteSgiImageByte(argv[2],out,xdim,ydim);
}

int compute_grad(UCHAR *img,int xdim,int up,int down,int left,int right)
{
 int gradx,grady,grad;
 gradx = grady = 0;
 if (up) grady -= *(img-xdim);
 if (down) grady += *(img+xdim);
 if (left) gradx -= *(img-1);
 if (right) gradx += *(img+1);
 grad = gradx*gradx + grady*grady;
 grad = fsqrt((float)grad);
 return(grad);
}

UCHAR *scale_grad(int *grad,int size)
{
 UCHAR *img,*ip;
 int *gp,i,min,max;
 img = (UCHAR *) malloc(size*sizeof(UCHAR));
 min = max = *grad;
 for (i=0, gp=grad; i<size; i++, gp++)
	if (*gp > max) max = *gp;
	else if (*gp < min) min = *gp;
 if (max==min) max++;
 for (i=0, gp=grad, ip=img; i<size; i++, gp++, ip++)
	*ip = ((*gp-min)*255)/(max-min);
 return(img);
}
